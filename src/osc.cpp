/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Osc.h>

Oscillator::Oscillator	( RoleId_e role_id,
						OSCID_e _type_id,
						buffer_t bytes )
	: Logfacility_class	( "Oscillator" )
	, osctype_struct	( _type_id, role_id )
	, Oscillator_base	( _type_id, role_id )
	, Spectrum_class	( spectrum_ns::FREQ )
	, ADSR_class		( _type_id, role_id )
	, fourier			( osctype_name )
	, Mem_vco			( bytes )
	, Mem_fmo			( bytes )
	, Mem				( bytes )
	, mem_frames		( Mem.mem_ds.data_blocks )
	, scanner			( Mem.Data, mem_frames )
{

	Reset_connections	();
	Data_reset			();

	Mem_vco.DsInfo		( oscrole_name + ":" + osctype_name + "_vco" );
	Mem_fmo.DsInfo		( oscrole_name + ":" + osctype_name + "_fmo" );
	Mem.DsInfo			( oscrole_name + ":" + osctype_name + "main" );

	Comment( INFO, oscrole_name + ":" + osctype_name + " initialized" );

}

void Oscillator::operator= ( Oscillator& osc)
{
	this->wp = osc.wp;
	this->spectrum = osc.spectrum;
	Set_adsr( osc.Get_adsr() );
    Set_feature( osc.features );
}

void Oscillator::self_Test()
{
	TEST_START( className );
	cout << "waveFunction_vec: " << waveFunction_vec.size() << endl;
	Assert_lt( size_t( 0 ), waveFunction_vec.size() );

	OSC(0);
	TEST_END( className );
}

void Oscillator::Phase_reset()
{
	spectrum.phi = zero_phase;
}
void Oscillator::Data_reset(  )
{
	this->Mem.Clear_data( 0 );
}

void Oscillator::Connect_vol_data( Oscillator* sec)
{	// connect this volume with osc data
	if ( this->typeId == sec->typeId )
	{
		this->vp.Mem 	= &Mem_vco;
		this->vp.volume	= 0;
	}
	else
	{
		this->vp.Mem	= &sec->Mem;
		this->vp.volume = sec->spectrum.volidx[0];
	}
	this->Connect.vol 	= sec->typeId;
	this->vp.osc_id 	= sec->typeId;
	this->vp.name 		= sec->osctype_name;
}

void Oscillator::Connect_frq_data( Oscillator* sec )
{	// connect this frequency with osc data
	if ( this->typeId == sec->typeId ) // reset this
	{
		this->fp.Mem 	= &Mem_fmo;
		this->fp.volume = 0;
	}
	else
	{
		this->fp.Mem	= &sec->Mem;
		this->fp.volume = sec->spectrum.volidx[0];
	}
	this->Connect.frq 	= sec->typeId;
	this->fp.osc_id 	= sec->typeId;
	this->fp.name 		= sec->osctype_name;
}

void Oscillator::Set_connection( Oscillator* oscfp, Oscillator* oscvp )
{
	Connect_frq_data( oscfp );
	Connect_vol_data( oscvp );
}
void Oscillator::reset_vol_data()
{
	Connect_vol_data( this );
}
void Oscillator::reset_frq_data()
{
	Connect_frq_data( this );
}

void Oscillator::Reset_connections( )
{
	reset_frq_data();
	reset_vol_data();
}

Data_t* Oscillator::MemData_p()
{
	return Mem.Data;
}
Data_t Oscillator::MemData( const buffer_t& n )
{
	buffer_t offs = n % (2*max_frames);
	return Mem.Data[ offs ];
};
Data_t* Oscillator::GetData_p( const buffer_t& frame_offset )
{
	buffer_t offs = frame_offset % mem_frames;
	return this->Mem.Data + offs;//&Mem.Data[ offs ];
}

auto show_param = [ ]( string type, param_t param, phi_t dT, float freq, float frq )
{
	{
		cout << "osctype    " << type	  		<< endl;
		cout << "maxphi     " << param.maxphi 	<< endl;
		cout << "dphi       " << param.dphi		<< endl;
		cout << "phi        " << param.phi  	<< endl;
		cout << "dT         " << dT   			<< endl;
		cout << "base freq  " << freq 			<< endl;
		cout << "adj. freq  " << frq 			<< endl;
		Exception( "phi exceeds maxphi limit: " +
				to_string(param.maxphi) +
				" < " +
				to_string( abs(param.phi)) );
	}
};
#pragma GCC optimize("O3", "fast-math")
void Oscillator::oscFourier( const buffer_t& frame_offset )
{
	DynFrequency.SetDelta			( features.frq_slide_value, 0 );
	DynFrequency.Reset_state		( 0 );
	const Data_t 	vco_adjust 		= max_data_amp / 2;
	const uint8_t	waves			= 2; // number of waves (2pi) showin up in the wavedisplay
	waveform_t		waveFnc			= Waveform->Get_fnc( FOURIER );
	param_t			param			= param_struct();
					param.maxphi	= waveFnc.maxphi;
//					phi_t			dT				= param.maxphi * dt / 2;	// FFT.idx/2
					phi_t			dT				= param.maxphi * dt / waves;	// FFT.idx/2
	for	( buffer_t 	pos 			= frame_offset;
					pos 			< wp.frames + frame_offset;
					pos++ 			)
		{
			const float	vco_vol 		= ( vco_adjust + vp.Mem->Data[pos] );
			const frq_t	freq 			= DynFrequency.Get( 0 );
						param.dphi		= freq * dT;
						Mem.Data[pos]	= Oscillator::fourier.Fnc( param ) * vco_vol;
		}
	DynFrequency.Update( 0 );

}

constexpr auto osc_volume = []( Oscillator* osc_p)
{
	float				vol	= osc_p->spectrum.volidx[0] * percent;
	if ( osc_p->is_osc_type )
	{
		if ( not osc_p->has_notes_role )
			vol	= 1.0f;	// the volume of the main OSC is constant for the instr role
						// because this volume is managed by the StA volume
						// If the osc has the notes role the osc volume is managed
						// by the rhythm volume and the given StA-notes volume
	};
	return vol;
};


#pragma GCC optimize("O3", "fast-math")
void Oscillator::oscWave( const buffer_t& frame_offset )
{

	const float			vol_percent	= osc_volume( this );
	const float 		fmo_vol 	= fmo_scale * (float)this->fp.volume;
	const Data_t 		vco_adjust 	= max_data_amp / 2;
	const Data_t		vol_adjust	= ( vco_adjust * features.adjust ) * percent;
	const float 		volFactor	= VolFactor( spectrum.volidx );
	param_t 			param 		= param_struct();
						param.pmw	= 1.0 + (float)features.PWM * percent;

	Data_t* 			oscData		= this->Mem.Data;
	Data_t*				fmoData		= this->fp.Mem->Data;
	Data_t* 			vcoData		= this->vp.Mem->Data;

	auto osc_wave	= [ this,
						frame_offset, &param,
						vcoData, fmoData, oscData,
						vco_adjust, fmo_vol, volFactor, vol_adjust, vol_percent ]
						( size_t channel  )
	{
		const frq_t		frq_adjust		= Frqadj( channel, spectrum.frqidx[channel] );
		const WFID_e	wfid			= spectrum.wfid[channel];
		waveform_t		waveFnc			= Waveform->Get_fnc( wfid );

		wave_function_t	fnc 			= waveFnc.fnc;
						param.maxphi	= waveFnc.maxphi ;
						param.phi		= spectrum.phi[channel];
						param.amp		= spectrum.volidx[channel]*volFactor;
		const phi_t		dT				= param.maxphi * dt;

		for( buffer_t 	pos 			= frame_offset;
						pos 			< wp.frames + frame_offset;
						pos++ )
		{
			const float	vco_vol 	= ( vco_adjust 	+ vcoData[pos] ) * vol_percent ;
						oscData[pos]+= vol_adjust 	+ vco_vol * fnc( param );

			const frq_t	freq 		= DynFrequency.Get( channel );
			const frq_t	frq			= ( freq + fmo_vol * fmoData[pos] ) * frq_adjust;
						param.dphi	= MODPHI( dT * frq, param.maxphi ); // max frq < 48000 / 2*pi
																		// ~11000 Hz
						param.phi	= param.phi + param.dphi;
						param.phi 	= MODPHI( param.phi, param.maxphi );
		};	// for
		spectrum.phi[channel] 	= param.phi;

	};	// osc_wave

	for( size_t channel : osc_channel_vec )
		DynFrequency.SetDelta	(features.frq_slide_value, channel );

	for( size_t channel 	: osc_channel_vec )
	{
		if ( spectrum.volidx[channel] > 0 )
		{
			DynFrequency.Reset_state( channel );
			osc_wave				( channel );
			DynFrequency.Update		( channel );
		}

	}
}
void Oscillator::OSC ( buffer_t frame_offset )
/*
 * Generator of sound wave data
 */
{

	assert( wp.frames + frame_offset - 1 < mem_frames );


	if( ( spectrum.wfid[0] == FOURIER ) )
	{
		if( is_fmo_type ) return;
		oscFourier	( frame_offset );
	}
	else
	{
		oscWave		( frame_offset );
	}
	Apply_adsr( wp.frames, MemData_p(), frame_offset );
}
void Oscillator::Setwp_frames( uint16_t msec )
{
	wp.msec 	= msec;
	wp.frames	= check_range( frames_range,  wp.msec * frames_per_msec, "Setwp_frames" );
}
void Oscillator::Set_long_note( bool l )
{
	longnote = l ;
}

void Oscillator::ResetBeatCursor()
{
	Set_beatcursor( 0 );
	Set_hallcursor( 0 );
}



void Oscillator::Test()
{

	DynFrequency.TestFrq();

	self_Test();

	TEST_START( className );

	assert( abs( Mem_vco.Data[0] )	< 1E-8 );
	assert( abs( Mem_fmo.Data[0] )	< 1E-8 );
	Mem.DsInfo("osc");
	ASSERTION( Mem.mem_ds.data_blocks == mem_frames,"Mem.mem_ds.data_blocks", Mem.mem_ds.data_blocks
			,mem_frames );

	string_vec_t arr = { "TYPE","VCO","Sinus","17","2000","100","2","1","1","69","2","0","-1","0","42" };
	Line_interpreter( arr );
	float f = frqArray[ spectrum.frqidx[0] ];
	ASSERTION( fcomp( f, 8) , "Frequency", f, 8 );
	Setwp_frames( max_msec );

	Assert_equal(  oct_base_freq, frqArray[C0] , "osc_base_freq" );
	spectrum 	= default_spectrum;
	features 	= feature_struct();

	longnote = true;
	adsr_data_t adsr = default_adsr;
	adsr.attack = 50;
	Set_adsr( adsr );
	adsr = Get_adsr();
	ASSERTION( adsr.attack == 50 , "adsr.attack", adsr.attack, 50 );


	Oscillator testosc { INSTRROLE,  OSCID, monobuffer_bytes };
	uint A3 = testosc.Index("A3");
	ASSERTION( fcomp( frqArray[ A3 ], 220), "frq index", frqArray[ A3 ], 220 );

	spectrum.phi	= zero_phase;
	testosc.Set_frequency( A3, FIXED ); // 220 Hz
	testosc.Set_volume( 100 );

	testosc.OSC( 0 );
	testosc.OSC( 0 );
	float a2 = testosc.Mem.Data[0];
	testosc.OSC( 0 );
	float a0 = testosc.Mem.Data[0];
	cout << "A3: " << A3 << " a0: " << a0 << " a2: " << a2 << " a2-a0: "<<  a2-a0 << endl;
//	for( uint n = 0; n<10;n++ )
//		cout << dec << (int)testosc.Mem.Data[n] << endl;
	ASSERTION( fcomp( a0, a2 ), "gen data", a0-a2, "null" );

	testosc = *this; // test copy constructor

	float fthis = frqArray[ this->spectrum.frqidx[0] ];
	float ftest = frqArray[ testosc.spectrum.frqidx[0] ];
	ASSERTION( fcomp( 220, fthis) , "copy constructor", 220 , fthis );
	ASSERTION( fcomp( ftest, fthis) , "copy constructor", ftest , fthis );



	TEST_END( className );

}
