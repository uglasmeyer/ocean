/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#include <Osc.h>

Oscillator::Oscillator( char role_id,  char type_id ) :
		Logfacility_class( "Oscillator" ),
		Oscillator_base()
{
	className 		= Logfacility_class::className;
	oscId		= type_id;
	osc_name 	= OscRole.types[oscId];

	oscroleId		= role_id;
	oscrole_name		= OscRole.roles[oscroleId];

	is_osc_type 	= ( oscId == OscRole.OSCID );
	is_fmo_type		= ( oscId == OscRole.FMOID );
	is_vco_type		= ( oscId == OscRole.VCOID );
	has_kbd_role 	= ( oscroleId == OscRole.KBDID );
	has_notes_role 	= ( oscroleId == OscRole.NOTESID );
	has_instr_role 	= ( oscroleId == OscRole.INSTRID );

	Connection_reset();
	Data_reset();

	Mem_vco.Info( osc_name );
	Mem_fmo.Info( osc_name );
	Mem.Info	( osc_name );

	Comment( INFO, oscrole_name + " " + osc_name + " initialized" );


}

void Oscillator::operator= (const Oscillator& osc)
{
    this->wp 					= osc.wp;
    this->adsr					= osc.adsr;
    this->spectrum				= osc.spectrum;
}


void Oscillator::Phase_reset()
{
	phase = default_phase;
}
void Oscillator::Data_reset(  )
{
	this->fp.Mem->Clear_data( 0 );
	this->vp.Mem->Clear_data( 0 );//max_data_amp );
	this->Mem.Clear_data( 0 );
}

void Oscillator::Connect_vol_data( Oscillator* osc)
{	// connect this volume with osc data
	if ( this->oscId == osc->oscId )
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
	this->vp.osc_id = osc->oscId;
	this->vp.name 	= osc->osc_name;
}

void Oscillator::Connect_frq_data( Oscillator* osc )
{	// connect this frequency with osc data
	if ( this->oscId == osc->oscId )
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
	this->fp.osc_id = osc->oscId;
	this->fp.name 	= osc->osc_name;
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
Data_t Oscillator::MemData( buffer_t n )
{
	buffer_t offs = check_range( frames_range, n );
	return Mem.Data[ offs ];
};
Data_t* Oscillator::GetData_p( const buffer_t& frame_offset )
{
	buffer_t offs = check_range( frames_range, frame_offset );
	return &Mem.Data[ offs ];
}

auto check_phi = [ ]( string type, param_t param, phi_t dT, float freq )
{
	if ( param.maxphi < abs(param.dphi) )
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

void Oscillator::OSC (  const buffer_t& frame_offset )
/*
 * Generator of sound waves
 */
{


	buffer_t 			frames 		= wp.frames;

	phi_t 				dt 			= 1.0/frames_per_sec;	//seconds per frame

	Data_t* 			oscData		= this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmoData		= this->fp.Mem->Data+ frame_offset;// * sizeof_data;
	Data_t* 			vcoData		= this->vp.Mem->Data+ frame_offset;// * sizeof_data;

	float 				fmo_vol 	= 0.001*(float)this->fp.volume;
	float				vol_per_cent= this->wp.volume * 0.01; // the volume of the main osc is managed by the mixer!

	if ( is_osc_type )
		if ( not has_notes_role  )
			vol_per_cent	= 1;// the volume of the osc is constant for the instr role
								// because this volume is managed by the mixer
								// If the osc has the notes role the osc volume is managed
								// by the rhythm volume and the mixer

	Data_t 	vco_adjust 	= max_data_amp / 2;
	Data_t	vol_adjust	= ( vco_adjust * wp.adjust ) * 0.01;


	if ( frame_offset + frames > max_frames )
	{
		Comment(ERROR, "frames overflow: " , frame_offset , " + ", frames, " > " , max_frames );
		return;
	}

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
							phi_t dT		= param.maxphi * dt;

			for( buffer_t n = 0; n < frames; n++ )
			{
				float 	vco_vol 	= (vco_adjust 	+ vcoData[n]) 	* vol_per_cent ;
						oscData[n]	= oscData[n]	+ vol_adjust 	+ vco_vol * fnc( param );

						freq 		= DynFrequency.Get();
						param.dphi	= dT *( freq + fmo_vol * fmoData[n] ) * frq_adjust;

						param.phi	= param.phi + param.dphi;
						param.phi 	= MODPHI( param.phi, param.maxphi );
			}
			check_phi( osc_name, param, dT, freq );
			phase[channel] = param.phi;
			DynFrequency.Update();
		}
	}

	if (  is_osc_type )
		apply_adsr( frames, oscData );
}

void Oscillator::Shift_data( buffer_t offs )
{
	for ( buffer_t n = 0; n < max_frames - offs ; n++ )
		Mem.Data[ n ] = Mem.Data[ n + offs ];
	for ( buffer_t n = max_frames - offs; n < max_frames; n++ )
	{
		Mem.Data[ n ] = 0;
	}
};

void Oscillator::Set_long_note( bool l )
{
	longnote = l ;
}

void Oscillator::Reset_beat_cursor()
{
	beat_cursorL = 0;
	set_beatcursorR();
}
void Oscillator::apply_adsr( buffer_t frames, Data_t* data )
{
	if ( adsr.bps == 0 ) return;
	if ( beat_frames == 0 ) return;

	float	dbL = 0.5;
	float 	dbR	= 1.0 - dbL;
	for ( uint n = 0; n < frames; n++ )
	{
		data[ n ] = data[ n ] * ( adsrdata[ beat_cursorL ]*dbL + adsrdata[ beat_cursorR ]*dbR );
		beat_cursorL = ( beat_cursorL + 1 ) % beat_frames;;
		beat_cursorR = ( beat_cursorR + 1 ) % beat_frames;;
	}
	if ( has_kbd_role )
		Comment( DEBUG, "beat_cursor: " , (int) beat_cursorL );
}






void Oscillator::Test()
{

	DynFrequency.TestFrq();

	TEST_START( className );


	assert( abs( Mem_vco.Data[0] )	< 1E-8 );
	assert( abs( Mem_fmo.Data[0] )	< 1E-8 );
	assert( Mem.ds.data_blocks 	== max_frames );

	vector_str_t arr = { "TYPE","VCO","Sinus","17","2000","100","2","1","1","69","2","0","-1","0","42" };
	Line_interpreter( arr );
	float f = GetFrq( wp.frqidx);
	ASSERTION( fcomp( f, 8) , "Frequency", f, 8 );
	Setwp_frames( max_msec );

	ASSERTION( fcomp( oct_base_freq, GetFrq( C0 )), "osc_base_freq" , oct_base_freq, GetFrq( C0 ));
	spectrum 	= spec_struct();
	adsr 		= adsr_struct();


	longnote = true;
	adsr.attack = 50;
	adsr.decay = 5;

	Oscillator testosc {osc_struct::INSTRID,  osc_struct::OSCID };
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


	TEST_END( className );
}

/*
               Receiver                Data[Event]              Data[Event]
              _Amplitude          _    Emission                 Receiver
                         |___r__|_ w |                          present data[0], past data[h]
                         |\     |    |
                         | \    |    |
                         |  \   |   /|
                         |   \  |  / |
                         |    \ | /  |                          past data[h]
               h=2r/c    |\    \|/   | present data[0]
                         | \    |    |
                         |  \   |\   |
                         |   \  | \  |                          present data[h]
                         |\   \ |  \ |                                        ]
               he=r/c    | \   \|   \| past data[0]
                         |  \   |   /|
                         |   \  |\ / |
                 _____________\_|_/  |                          present data[0], past data[h1]
                         |\    \|/ \ | present data[h]
                         | \    |   \| past data[0]
                         |  \   |\  /|
                         |   \  | \/ |
 A[(2w+r)/c]  =A[h]+A[t] |    \ |_/\ |                          past data[h0]
                         |\    \|/  \| present data[0[
                         | \    |    |
                         |  \   |\   |
                         |   \  | \  |                          present data[h]
                         |\   \ |  \ |
                         | \   \|   \| past data[h1]
                         |  \   |   /|
                         |   \  |\ / |
           __________________ \_| \  |_                         present data[h0], past data[0]
                         |\    \|/ \ | present data[h1]
                         | \    |   \| past data[h0]
                         |  \   |   /|
                         |   \  |  / |
  	                     |    \ | /  |
                         |  r  \|/ w | present data[h0]
 	              Receiver   Sender   Wall (db coefficient)





 *
 */
