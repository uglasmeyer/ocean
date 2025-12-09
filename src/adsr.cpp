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
#include <Oscwaveform.h>

ADSR_class::ADSR_class( OSCID_e _typeid )
	: Logfacility_class("ADSR_class")
	, Spectrum_class()
	, Oscillator_base( _typeid )
{
	className 			= Logfacility_class::className;
	Set_adsr			( default_adsr );
	adsr_data.spec.osc	= _typeid;
};

ADSR_class::ADSR_class()
	: Logfacility_class("ADSR_class")
	, Spectrum_class()
	, Oscillator_base()
{
	className 	= Logfacility_class::className;
	Set_adsr	( default_adsr );
};


ADSR_class::~ADSR_class()
{
	if( LogMask[ DEBUG ] )
		coutf << "~" << className << endl;
}

Data_t* ADSR_class::AdsrMemData_p()
{
	return adsr_Mem.Data;
}

void ADSR_class::Apply_adsr( buffer_t frames, Data_t* Data, buffer_t frame_offset )
{
	if ( adsr_data.bps	== 0 ) return;
	if ( beat_frames 	== 0 ) return;

	if ( tainted ) adsrOSC( beat_frames );

	float		dbB 		= 0.5;
	float 		dbH			= 1.0 - dbB;
	uint		pos			= frame_offset;

	for ( uint m = 0; m < frames; m++ )
	{
		Data[ pos ] = Data[ pos ] * ( 	adsr_Mem.Data[ beat_cursor ]*dbB +
										adsr_Mem.Data[ hall_cursor ]*dbH );
		hall_cursor = ( hall_cursor + 1 ) % beat_frames;
		beat_cursor = ( beat_cursor + 1 ) % beat_frames;
		pos			= ( pos + 1 ) % adsr_frames;
	}
	if ( has_kbd_role )
		Comment( DEBUG, "beat_cursor: " , (int) beat_cursor );
}


auto adsr_fnc = [  ]( buffer_t aframes, buffer_t n, float y0, float dy, float d_delta  )
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

	if ( n < aframes ) 	// attack
	{
		return ( y0 + n*dy );
	}
	else				// decay
	{
		float delta 	= ( n - aframes ) * d_delta;
		return ( exp( -delta ) );
	}
};

void ADSR_class::adsrOSC( const buffer_t& bframes )
{

	if ( bframes == 0 ) 		return;
	if ( not tainted ) 			return;
	tainted 					= false;

	buffer_t		aframes 	= rint( bframes * 0.002 );
	if ( adsr_data.attack 	> 0 )
					aframes 	= rint ( bframes * adsr_data.attack * percent );
	const float 	d_delta		= ( ( 100 - adsr_data.decay ) / 3.0 ) / bframes;
	const float 	delta 		= ( bframes - aframes ) * d_delta;
	const float		y0 			= expf( - delta );
	const float 	dy			= (1.0 - y0) / aframes;

	Spectrum_class::Sum( adsr_data.spec );

	for ( buffer_t n = 0; n < bframes ; n++ )
	{
		adsr_Mem.Data[n] 		= adsr_data.spec.vol[0] * adsr_fnc( aframes, n, y0, dy, d_delta );
	}
	for ( buffer_t n = bframes; n < adsr_frames ; n++ )
	{
		adsr_Mem.Data[n] 		= 0.0;
	}



	param_t			param 		= param_struct();
					param.pmw	= 1.0 + (float)features.PWM * percent;
	spectrum_t		spec 		= adsr_data.spec;

	for ( size_t channel = 1; channel < SPECARR_SIZE; channel++ )
	{
		if ( spec.volidx[channel] > 0 )
		{

			uint8_t			wfid		= spec.wfid[ channel ];
			wave_function_t	fnc			= adsrFunction_vec[ wfid ].fnc ;
							param.maxphi= adsrFunction_vec[ wfid ].maxphi;
			float 			dT 			= param.maxphi / bframes;
							param.dphi	= dT * spec.frqadj[ channel ];// * adsr_data.bps;
							param.phi	= 0.0;
							param.amp	= spec.volidx[ channel ] * percent/adsrFunction_vec[ wfid ].width;
							//spec.vol[ channel ] / adsrFunction_vec[ wfid ].width  ;
			for ( buffer_t n = 0; n < bframes ; n++ )
			{
				adsr_Mem.Data[n] 	+= adsr_Mem.Data[n] * fnc( param );
				param.phi			= param.phi + param.dphi;
				param.phi 			= MODPHI( param.phi, param.maxphi );
			}
		}
	}
}

void ADSR_class::Set_hallcursor( buffer_t cursor )
{
	if ( beat_frames == 0 ) return;
	hall_cursor 	= cursor % beat_frames;

	if ( cursor == 0 )
	{
		buffer_t
		hframes		= rint( adsr_data.hall * min_frames * percent );
		hall_cursor	= ( beat_cursor + hframes ) % beat_frames;
	}
}
void ADSR_class::Set_beatcursor( buffer_t cursor )
{
	beat_cursor = cursor;
}

void ADSR_class::Set_feature( feature_t f )
{
	features = f;
}
void ADSR_class::Set_kbdbps( uint8_t bps )
{
	kbdbps = bps;
}
void ADSR_class::Set_bps()
{
	tainted 	= true;
	if ( ( has_notes_role ) )	// overwrite instrument settings
	{
		adsr_data.bps 	= 1;
		beat_frames 	= wp.frames;//rint( adsr_frames / adsr_data.bps );//
	}
	else
	{
		if( has_kbd_role )
			adsr_data.bps = kbdbps;
		if ( adsr_data.bps > 0 )
			beat_frames = rint( adsr_frames / adsr_data.bps );
		else
			beat_frames = 0;
	}
//	adsrOSC( beat_frames );

}
void ADSR_class::Set_adsr_spec	( spectrum_t spec )
{
	tainted 		= true;
	adsr_data.spec	= spec;
}

adsr_t ADSR_class::Set_adsr( adsr_t _adsr )
{
	adsr_data 	= _adsr;
	tainted 	= true;
	Set_bps( );
	Set_hallcursor();
	return adsr_data;
//	adsrOSC( beat_frames );
}

adsr_t ADSR_class::Get_adsr()
{
	return adsr_data;
}

string ADSR_class::Show_adsr	( adsr_t _adsr )
{
	return Show_spectrum_type( SPEW, _adsr.spec );
}



