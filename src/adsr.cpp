/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * adsr.cpp
 *
 *  Created on: Aug 1, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Adsr.h>

inline string alpha( bool flag )
{
	return ( flag ) ? "true" : "false";
}

uint8_t localclock = 1;


ADSR_class::ADSR_class( OSCID_e _typeid, RoleId_e roleId )
	: Logfacility_class	( "ADSR_class" )
	, osctype_struct	( _typeid, roleId )
	, Oscillator_base	( _typeid, roleId )
	, Spectrum_class	( spectrum_ns::ADSR )
	, adsr_Mem			( max_frames*sizeof_Data )
	, fourier			( osctype_name + "A" )
{
	this->tainted		= true; // becomes true if adsr_data changes
	this->beat_frames	= max_frames;
	this->adsr_wp_frames= max_frames;
	this->adsr_data		= adsr_data_struct();
	this->base_wfid		= &adsr_data.spec.wfid[0];

	Set_beatclock		( &localclock );
	Set_beatcursor		( 0 );
	Set_hallcursor		( 0 );
	KbdAttack			();
	Set_adsr			( default_adsr );

};

ADSR_class::~ADSR_class()
{
	DESTRUCTOR( className );
}

void ADSR_class::Set_beatclock( uint8_t* bpm )
{
	beatclock = bpm;
}

Data_t* ADSR_class::AdsrMemData_p()
{
	return adsr_Mem.Data;
}

void ADSR_class::Apply_adsr( buffer_t frames, Data_t* Data, buffer_t frame_offset )
{
	if ( not kbdattack )
	{
		if ( adsr_data.bps	== 0 )
		{
			beat_cursor = 0;
			return;
		}
	}

	if ( beat_frames 	== 0 )
	{
		beat_cursor= 0;

		return;
	}
	if ( tainted )
		adsrOSC( beat_frames );

	kbdattack			= false;

	const float	dbB 	= 0.5;
	const float dbH		= 1.0 - dbB;
	uint		pos		= frame_offset;

	// merge adsr data and osc data
	for ( uint m = 0; m < frames; m++ )
	{
		Data[ pos ] = Data[ pos ] * ( 	adsr_Mem.Data[ beat_cursor ]*dbB +
										adsr_Mem.Data[ hall_cursor ]*dbH );
		hall_cursor = ( hall_cursor + 1 )	% beat_frames;
		beat_cursor = ( beat_cursor + 1 )	% beat_frames;
		pos			= ( pos + 1 ) 			% adsr_frames;
	}


	if ( has_kbd_role )
		Comment( DEBUG, "Apply_adsr::", osctype_name,
						"beat_cursor:" , (int) beat_cursor );
}

typedef struct adsr_param_struct
{
	buffer_t 	aframes;
	float 		y0;
	float		dy;
	float 		d_delta;
} adsr_param_t;

auto adsr_fnc = [  ]( buffer_t n, const adsr_param_t& p  )
{
	/* maxima
	kill(all);
	attack  : 10;
	decay	: 95;
	beatframes : 24000;
	aframes : beatframes * attack^2 / 100;
	d_delta	: ( (100 - decay )/3.0) / beatframes;
	y0		: exp( - (beatframes-aframes) * d_delta );
	d_alpha	: (1-y0)/aframes;
	fattack : y0 + n^2*d_alpha;
	fdecay	: exp( - ( n - aframes ) * d_delta );
	plot2d([fattack, fdecay], [n,0,beatframes],[y,0,2]);
	*/

	if ( n < p.aframes ) 	// attack
	{
		return ( p.y0 + n * p.dy );
	}
	else					// decay
	{
		float delta 	= ( n - p.aframes ) * p.d_delta;
		return ( exp( -delta ) );
	}
};

#include <Fourier.h>

void ADSR_class::adsrOSC( buffer_t bframes )
{
	auto clearFNC = [ this ]( buffer_t frames )
	{	// delete unspecied data
		for ( buffer_t n = frames; n < adsr_frames ; n++ )
		{
			adsr_Mem.Data[n] 		= 0.0;
		}
	};

	auto fourierFNC = [ this, clearFNC, bframes ](  )
	{
		param_t 		param 		= param_struct();
						param.maxphi= 2*M_PI;
						param.dphi	= param.maxphi * dt / 2;	// FFT.idx/2

		for ( buffer_t n = 0; n < bframes ; n++ )
		{
			adsr_Mem.Data[n] 		= fourier.Fnc( param );
		}
		clearFNC( bframes );
	};

	auto adsrFNC = [ this, clearFNC, bframes ](  )
	{
		const buffer_t	aframes 	= ( adsr_data.attack > 0 ) ?
										rint ( bframes * adsr_data.attack * percent ) :
										rint ( bframes * 0.002 ); // minimal attack length

		const float 	d_delta		= ( ( 100 - adsr_data.decay ) / 3.0 ) / bframes;
		const float 	delta 		= ( bframes - aframes ) * d_delta;
		const float		y0 			= expf( - delta );
		const float 	dy			= (1.0 - y0) / aframes;

		adsr_param_t 	adsr_param	= { aframes, y0, dy, d_delta };
		if( ( kbdattack ) and ( *beatclock == 0 ) )
			adsr_param 				= { aframes, 0.0, 1.0f/float(aframes), 0.0 };

		for ( buffer_t n = 0; n < bframes ; n++ )
		{
			adsr_Mem.Data[n] 		= adsr_fnc( n, adsr_param );
		}
		clearFNC( bframes );
	};

	auto spectrumFNC = [ this, bframes ](  )
	{
		param_t			param 		= param_struct();
						param.pmw	= 1.0 + (float)features.PWM * percent;
		spectrum_t		spec 		= adsr_data.spec;

		// Note: channel 0 specifies the main waveform
		for ( uint8_t channel : dialog_channel_vec )
		{

			const WFID_e	wfid		= spec.wfid[ channel ];
			waveform_t		waveFnc		= Waveform->Get_fnc( wfid );
			wave_function_t	fnc			= waveFnc.fnc ;
							param.maxphi= waveFnc.maxphi;
			const float		dT 			= param.maxphi / bframes;
			const frq_t		adjust		= Frqadj( channel, spec.frqidx[channel] );
							param.dphi	= dT * adjust;
							param.phi	= 0.0;
							param.amp	= spec.volidx[ channel ]*volFactor/adsrFunction_vec[ wfid ].width;
			for ( buffer_t n = 0; n < bframes ; n++ )
			{
				adsr_Mem.Data[n] 	+= adsr_Mem.Data[n] * fnc( param );
				param.phi			= param.phi + param.dphi;
				param.phi 			= MODPHI( param.phi, param.maxphi );
			}
		}
	};
	////////////////////////////////////////////////////////////////////////

	if ( not tainted ) 			return;
	tainted 					= false;

	if ( bframes == 0 )
	{
		clearFNC( 0 );
		return;
	}

	Comment( DEBUG, "adsrOSC::", osctype_name, (int)bframes, alpha(kbdattack) );
	switch ( *base_wfid )
	{
		case WFID_e::FOURIER	: { fourierFNC();
									break; }
		default					: { adsrFNC();
									spectrumFNC();
									break; }
	} // switch key
}

bool ADSR_class::Sync()
{
	return ( beat_cursor == 0 );
}

void ADSR_class::Set_hallcursor( buffer_t cursor )
{
	if ( beat_frames == 0 ) return;
	hall_cursor 	= cursor % beat_frames;

	if ( cursor == 0 )
	{
		buffer_t
		hframes		= rint( adsr_data.hall * ( min_frames * percent ) );
		hall_cursor	= ( beat_cursor + hframes ) % beat_frames;
	}
}
void ADSR_class::Set_beatcursor( buffer_t cursor )
{
	beat_cursor = cursor;
}

void ADSR_class::KbdAttack()
{
	kbdattack	= ( *beatclock == 0 ) ? true : false;
	Set_bps		();
	Comment( DEBUG, "KbdAttack::", osctype_name, alpha(kbdattack) );
}
void ADSR_class::Set_bps()
{
	auto set_beatframes = [ this ]( uint8_t beat, buffer_t frames )
	{
		uint8_t bc = *beatclock;
		if( (bc * beat) > 0 )
		{
			beat_frames = frames / ( bc * beat );
		}
		else
		{
			beat_frames = 0; // adsr is not applied
			if ( kbdattack ) // generate new adsr
				beat_frames = adsr_frames;
		}

	};

	tainted 	= true;
	if( roleId == NOTESROLE )
		set_beatframes( 1				, wp.frames   );
	else
		set_beatframes( adsr_data.bps	, adsr_frames );
}

void ADSR_class::set_adsr_spec	( spectrum_t& spec )
{
	tainted 			= true;
	spec.volidx[0]		= 100;
	volFactor			= VolFactor( spec.volidx );
	spec.osc			= typeId;
	adsr_data.spec		= spec;
	if( *base_wfid == WFID_e::FOURIER )
	{
		if( not fourier.Is_ready() )
			*base_wfid = WFID_e::ADSR;
	}
	else
	{
		*base_wfid = WFID_e::ADSR;
	}
}

adsr_data_t ADSR_class::Set_adsr( adsr_data_t _adsr )
{
	adsr_data 		= _adsr;
	tainted 		= true;
	Set_bps			();
	Set_hallcursor	();
	set_adsr_spec	( adsr_data.spec );
	return 			adsr_data;
}

adsr_data_t ADSR_class::Get_adsr()
{
	return adsr_data;
}

string ADSR_class::Show_adsr( adsr_data_t _adsr )
{
	return Show_spectrum_type( spectrum_ns::SPEW, _adsr.spec );
}



