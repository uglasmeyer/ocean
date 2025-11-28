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
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Osc.h>
#include <Oscwaveform.h>

Oscillator::Oscillator( RoleId_e role_id,  OSCID_e _type_id, buffer_t bytes )
	: Logfacility_class	( "Oscillator" )
	, Oscillator_base		( _type_id )
	, ADSR_class			( _type_id )
	, Mem_vco				( bytes )
	, Mem_fmo				( bytes )
	, Mem					( bytes )
	, scanner				( Mem.Data, min_frames, Mem.mem_ds.data_blocks )
{
	className 		= Logfacility_class::className;
	mem_frames		= Mem.mem_ds.data_blocks ;

	typeId			= _type_id;
	osctype_name	= typeNames[typeId];

	roleId			= role_id;
	oscrole_name	= roleNames[roleId];

	is_osc_type 	= ( typeId == OSCID );
	is_fmo_type		= ( typeId == FMOID );
	is_vco_type		= ( typeId == VCOID );
	has_kbd_role 	= ( roleId == KBDROLE );
	has_notes_role 	= ( roleId == NOTESROLE );
	has_instr_role 	= ( roleId == INSTRROLE );

	Connection_reset();
	Data_reset();

	Mem_vco.DsInfo	( oscrole_name + ":" + osctype_name );
	Mem_fmo.DsInfo	( oscrole_name + ":" + osctype_name );
	Mem.DsInfo		( oscrole_name + ":" + osctype_name );

	Comment( INFO, oscrole_name + ":" + osctype_name + " initialized" );

}

void Oscillator::operator= ( Oscillator& osc)
{
/*    Setwp( osc.wp );
    Set_feature( osc.feature );
    Set_adsr( osc.Get_adsr() );
    Set_spectrum( osc.spectrum );
*/
	this->wp = osc.wp;
	this->spectrum = osc.spectrum;
	Set_adsr( osc.Get_adsr() );
    Set_feature( osc.features );
}

void Oscillator::self_Test()
{
	TEST_START( className );
	cout << "waveFunction_vec: " << waveFunction_vec.size() << endl;
	cout << "waveform_str_vec: " << waveform_str_vec.size() << endl;
	ASSERTION( 	waveform_str_vec.size() > 0, "waveform_str_vec.size()",
				waveform_str_vec.size(), ">0" );


	OSC(0);
	TEST_END( className );
}


void Oscillator::Phase_reset()
{
	phase = default_phase;
}
void Oscillator::Data_reset(  )
{
//	this->fp.Mem->Clear_data( 0 );
//	this->vp.Mem->Clear_data( 0 );//max_data_amp );
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
void Oscillator::Reset_vol_data()
{
	Connect_vol_data( this );
}
void Oscillator::Reset_frq_data()
{
	Connect_frq_data( this );
}

void Oscillator::Connection_reset( )
{
	Reset_frq_data();
	Reset_vol_data();
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
auto hallphi = [  ]( uint8_t adsr_hall, phi_t max )
{
	return max * adsr_hall * percent;
};


void Oscillator::OSC (  buffer_t frame_offset )
/*
 * Generator of sound waves data in a ring buffer
 */
{
	buffer_t 			frames 		= wp.frames;
	if( has_kbd_role  )
		assert( frames == max_frames );
//	cout << "run osc " << osc_name  << phase[0] << endl;
	phi_t 				dt 			= 1.0 / ( frames_per_sec );	//seconds per frame
	buffer_t			offset		= frame_offset;
	Data_t* 			oscData		= this->Mem.Data;//this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmoData		= this->fp.Mem->Data;//+ frame_offset;// * sizeof_data;
	Data_t* 			vcoData		= this->vp.Mem->Data;//+ frame_offset;// * sizeof_data;

	float 				fmo_vol 	= fmo_scale* (float)this->fp.volume;
	float				vol_per_cent= this->spectrum.volidx[0] * percent; // the volume of the main osc is managed by the mixer!

	if ( is_osc_type )
		if ( not has_notes_role )
			vol_per_cent	= 1;// the volume of the osc is constant for the instr role
								// because this volume is managed by the mixer
								// If the osc has the notes role the osc volume is managed
								// by the rhythm volume and the mixer
	if ( has_kbd_role )
		phase = default_phase;

	Data_t 	vco_adjust 	= max_data_amp / 2;
	Data_t	vol_adjust	= ( vco_adjust * features.adjust ) * percent;

	Sum( spectrum );
	DynFrequency.SetDelta( features.glide_effect );
	param_t 	param 		= param_struct();
				param.pmw	= 1.0 + (float)features.PMW_dial * percent;
	frq_t		frq			= 0.0;
	for ( size_t channel = 0; channel < SPECARR_SIZE; channel++ )
	{
		if ( spectrum.volidx[channel] > 0 )
		{
			frq_t 			freq 			= DynFrequency.Reset_state();
			frq_t			frq_adjust		= spectrum.frqadj[channel];
			uint8_t 		wfid			= spectrum.wfid[channel];
			wave_function_t	fnc 			= waveFunction_vec[ wfid ].fnc;
							param.maxphi	= abs( waveFunction_vec[ wfid ].maxphi );
							param.phi		= phase[channel];
							param.amp		= spectrum.vol[channel];
			phi_t 			dT				= param.maxphi * dt;
			buffer_t		pos				= offset;
			for( buffer_t m = 0; m < frames; m++ )
			{
				float 	vco_vol 	= ( vco_adjust 	+ vcoData[pos] ) * vol_per_cent ;
						oscData[pos]= oscData[pos]	+ vol_adjust 	+ vco_vol * fnc( param );

						freq 		= DynFrequency.Get();
						frq			= ( freq + fmo_vol * fmoData[pos] ) * frq_adjust;
						param.dphi	= MODPHI( dT * frq, param.maxphi ); // max frq < 48000 / 2*pi
																		// ~11000 Hz
						param.phi	= param.phi + param.dphi;
						param.phi 	= MODPHI( param.phi, param.maxphi );
						pos 		= ( pos + 1 ) % mem_frames;
			}
			if ( param.maxphi < abs(param.phi) )
				show_param	( osctype_name, param, dT, freq, frq );
			phase[channel] = param.phi;
			DynFrequency.Update();
		}
	}

	Apply_adsr( frames, MemData_p(), offset );
}
void Oscillator::Setwp_frames( uint16_t msec )
{
	wp.msec 	= msec;
	wp.frames	= check_range( frames_range,  wp.msec * frames_per_msec, "Setwp_frames" );
	Set_bps	( );
}
void Oscillator::Set_long_note( bool l )
{
	longnote = l ;
}

void Oscillator::Reset_beat_cursor()
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

	vector_str_t arr = { "TYPE","VCO","Sinus","17","2000","100","2","1","1","69","2","0","-1","0","42" };
	Line_interpreter( arr );
	float f = GetFrq( spectrum.frqidx[0] );
	ASSERTION( fcomp( f, 8) , "Frequency", f, 8 );
	Setwp_frames( max_msec );

	ASSERTION( fcomp( oct_base_freq, GetFrq( C0 )), "osc_base_freq" , oct_base_freq, GetFrq( C0 ));
	spectrum 	= default_spectrum;
	features 	= feature_struct();

	longnote = true;
	adsr_t adsr = default_adsr;
	adsr.attack = 50;
	Set_adsr( adsr );
	adsr = Get_adsr();
	ASSERTION( adsr.attack == 50 , "adsr.attack", adsr.attack, 50 );


	Oscillator testosc { INSTRROLE,  OSCID, monobuffer_bytes };
	uint A3 = testosc.Index("A3");
	ASSERTION( fcomp( frqArray[ A3 ], 220), "frq index", frqArray[ A3 ], 220 );

	phase[0]	= 0.0;
	testosc.Set_frequency( A3, FIXED ); // 220 Hz
	testosc.Set_volume( 100, FIXED );

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

	float fthis = GetFrq( this->spectrum.frqidx[0] );
	float ftest = GetFrq( testosc.spectrum.frqidx[0]);
	ASSERTION( fcomp( 220, fthis) , "copy constructor", 220 , fthis );
	ASSERTION( fcomp( ftest, fthis) , "copy constructor", ftest , fthis );

	TEST_END( className );

}
