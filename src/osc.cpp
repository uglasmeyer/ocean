/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#include <Osc.h>
#include <Oscwaveform.h>

Oscillator::Oscillator( char role_id,  char type_id, buffer_t bytes ) :
	Logfacility_class	( "Oscillator" ),
	ADSR_class			(),
	Mem_vco				( bytes ),
	Mem_fmo				( bytes ),
	Mem					( bytes ),
	scanner				( Mem.Data, min_frames, Mem.mem_ds.data_blocks )
{
	className 		= Logfacility_class::className;
	mem_frames		= Mem.mem_ds.data_blocks ;

	typeId			= type_id;
	osctype_name	= OscRole.types[typeId];

	roleId			= role_id;
	oscrole_name	= OscRole.roles[roleId];

	is_osc_type 	= ( typeId == OscRole.OSCID );
	is_fmo_type		= ( typeId == OscRole.FMOID );
	is_vco_type		= ( typeId == OscRole.VCOID );
	has_kbd_role 	= ( roleId == OscRole.KBDID );
	has_notes_role 	= ( roleId == OscRole.NOTESID );
	has_instr_role 	= ( roleId == OscRole.INSTRID );

	Connection_reset();
	Data_reset();

	Mem_vco.DsInfo	( oscrole_name + ":" + osctype_name );
	Mem_fmo.DsInfo	( oscrole_name + ":" + osctype_name );
	Mem.DsInfo		( oscrole_name + ":" + osctype_name );

	Comment( INFO, oscrole_name + ":" + osctype_name + " initialized" );

}

void Oscillator::operator= (const Oscillator& osc)
{
    this->wp 					= osc.wp;
    this->adsr					= osc.adsr;
    this->spectrum				= osc.spectrum;
}

void Oscillator::self_Test()
{
	TEST_START( className );
	cout << waveFunction_vec.size() << endl;
	cout << waveform_str_vec.size() << endl;
	cout << wf_v.size() << endl;
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

void Oscillator::Connect_vol_data( Oscillator* osc)
{	// connect this volume with osc data
	if ( this->typeId == osc->typeId )
	{
		this->vp.Mem 		= &Mem_vco;
		this->connect.vol	= false;
		this->vp.volume		= 0;
	}
	else
	{
		this->vp.Mem		= &osc->Mem;
		this->connect.vol 	= true;
		this->vp.volume 	= osc->wp.volume;
	}
	this->vp.osc_id = osc->typeId;
	this->vp.name 	= osc->osctype_name;
}

void Oscillator::Connect_frq_data( Oscillator* osc )
{	// connect this frequency with osc data
	if ( this->typeId == osc->typeId )
	{
		this->fp.Mem 		= &Mem_fmo;
		this->connect.vol	= false;
		this->fp.volume 	= 0;
	}
	else
	{
		this->fp.Mem		= &osc->Mem;
		this->connect.vol 	= true;
		this->fp.volume 	= osc->wp.volume;
	}
	this->fp.osc_id = osc->typeId;
	this->fp.name 	= osc->osctype_name;
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

auto check_phi = [ ]( string type, param_t param, phi_t dT, float freq )
{
	if ( param.maxphi < abs(param.phi) )
	{
		cout << "osctype    " << type	  		<< endl;
		cout << "maxphi     " << param.maxphi 	<< endl;
		cout << "phi        " << param.phi  	<< endl;
		cout << "dT         " << dT   			<< endl;
		cout << "freq       " << freq 			<< endl;
		EXCEPTION( "maxphi exceeds limit: " + to_string(param.maxphi) + " < " + to_string( abs(param.dphi)) );
	}
};
auto hallphi = [  ]( uint8_t adsr_hall, phi_t max )
{
	//	const float 	d0 		= 10.0; // distance to the receiver of sound
	//	const float 	distance= adsr.hall * 0.05  + d0 ; // distance to a wall in meter [m]
	//	const float		c		= 330.0; // speed of sound  [m/s]
	//	const float 	hT 		= distance / c; // time delay [seconds]
	//	const buffer_t 	hframes	= rint( hT * frames_per_sec  ) ;// frame delay [# of frames]

	return max * adsr_hall * 0.01;
};
#define MODPHI( phi, maxphi )\
	( abs(phi) > (maxphi) ) ? (phi) - sgn(phi)*(maxphi) : phi;

void Oscillator::OSC (  buffer_t frame_offset )
/*
 * Generator of sound waves data in a ring buffer
 */
{

	buffer_t 			frames 		= wp.frames;
	if(has_kbd_role  )assert(frames==max_frames);
//	cout << "run osc " << osc_name  << phase[0] << endl;
	phi_t 				dt 			= 1.0 / ( frames_per_sec );	//seconds per frame
	buffer_t			offset		= frame_offset;
	Data_t* 			oscData		= this->Mem.Data;//this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmoData		= this->fp.Mem->Data;//+ frame_offset;// * sizeof_data;
	Data_t* 			vcoData		= this->vp.Mem->Data;//+ frame_offset;// * sizeof_data;

	float 				fmo_vol 	= 0.001*(float)this->fp.volume;
	float				vol_per_cent= this->wp.volume * 0.01; // the volume of the main osc is managed by the mixer!

	if ( is_osc_type )
		if ( not has_notes_role  )
			vol_per_cent	= 1;// the volume of the osc is constant for the instr role
								// because this volume is managed by the mixer
								// If the osc has the notes role the osc volume is managed
								// by the rhythm volume and the mixer
	if ( has_kbd_role )
		phase = default_phase;

	Data_t 	vco_adjust 	= max_data_amp / 2;
	Data_t	vol_adjust	= ( vco_adjust * wp.adjust ) * 0.01;



	Sum( spectrum );
	DynFrequency.SetDelta( wp.glide_effect );
	param_t 	param 		= param_struct();
				param.pmw	= 1.0 + (float)wp.PMW_dial * 0.01;

	for ( size_t channel = 0; channel < spec_arr_len; channel++ )
	{
		if ( spectrum.volidx[channel] > 0 )
		{
			frq_t 			freq 			= DynFrequency.Reset_state();
			frq_t			frq_adjust		= spectrum.frqadj[channel];
			uint8_t 		wfid			= spectrum.wfid[channel];
			wave_function_t	fnc 			= waveFunction_vec[ wfid ].fnc;
							param.maxphi	= waveFunction_vec[ wfid ].maxphi;
							param.phi		= phase[channel];
							param.amp		= spectrum.vol[channel];
			phi_t 			dT				= param.maxphi * dt;
			uint			pos				= offset;
			for( buffer_t m = 0; m < frames; m++ )
			{

				float 	vco_vol 	= (vco_adjust 	+ vcoData[pos]) 	* vol_per_cent ;
						oscData[pos]= oscData[pos]	+ vol_adjust 	+ vco_vol * fnc( param );

						freq 		= DynFrequency.Get();
						param.dphi	= dT *( freq + fmo_vol * fmoData[pos] ) * frq_adjust;

						param.phi	= param.phi + param.dphi;
						param.phi 	= MODPHI( param.phi, param.maxphi );
						pos 		= ( pos + 1 ) % mem_frames;
			}
			check_phi( osctype_name, param, dT, freq );
			phase[channel] = param.phi;
			DynFrequency.Update();
		}
	}

	Apply_adsr( frames, &oscData[0], offset );
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
	float f = GetFrq( wp.frqidx);
	ASSERTION( fcomp( f, 8) , "Frequency", f, 8 );
	Setwp_frames( max_msec );

	ASSERTION( fcomp( oct_base_freq, GetFrq( C0 )), "osc_base_freq" , oct_base_freq, GetFrq( C0 ));
	spectrum 	= spec_struct();
	adsr 		= feature_struct();

	longnote = true;
	adsr.attack = 50;
	adsr.decay = 5;

	Oscillator testosc {osc_struct::INSTRID,  osc_struct::OSCID, monobuffer_bytes };
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
	Comment( TEST, testosc.Show_this_spectrum (  ) );
	cout << "A3: " << A3 << " a0: " << a0 << " a2: " << a2 << " a2-a0: "<<  a2-a0 << endl;
//	for( uint n = 0; n<10;n++ )
//		cout << dec << (int)testosc.Mem.Data[n] << endl;
	ASSERTION( fcomp( a0, a2 ), "gen data", a0-a2, "null" );

	testosc = *this; // test copy constructor

	ASSERTION( adsr.attack == 50 , "", adsr.attack, 50 );
	float fthis = GetFrq( this->wp.frqidx );
	float ftest = GetFrq( testosc.wp.frqidx);
	ASSERTION( fcomp( ftest, fthis) , "copy constructor", ftest , fthis );

	ADSR_class Adsr {};
	TEST_END( className );

}
