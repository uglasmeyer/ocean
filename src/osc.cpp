/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#include <Osc.h>

Oscillator::Oscillator( ) :
		Logfacility_class( "Oscillator" ),
		Oscillator_base()
{
	className = Logfacility_class::className;
}
Oscillator::~Oscillator()
{
}

void Oscillator::SetId( char role, char type )
{
	osctype_id	= type;
	osc_type 	= OscRole.types[osctype_id];
	oscrole_id	= role;
	osc_role	= OscRole.roles[oscrole_id];

	is_osc_type = ( osctype_id == OscRole.OSCID );
	is_notes_role = ( oscrole_id == OscRole.NOTESID );
	is_instr_role = ( oscrole_id == OscRole.INSTRID );

	mem_init();
	Mem_vco.Info( osc_type );
	Mem_fmo.Info( osc_type );
	Mem.Info	( osc_type );

	Comment( INFO, osc_role + " " + osc_type + " initialized" );

}


void Oscillator::Set_start_freq( float freq )
{
	wp.start_frq = freq;
}

float Oscillator::get_delta_freq( float freq ) // TODO - working
{
	// 0..100 |-> 0..frames = 0..max_sec
//	buffer_t frames = ( this->wp.msec*audio_frames) / 1000;
	buffer_t frames = beatframes;
	if( not is_instr_role )
		frames = ( this->wp.msec*audio_frames) / 1000;

	if ( abs(wp.start_frq) < 1E-4 )
	{
		wp.start_frq = freq;
		return 0.0;  				// do nothing
	}
	float dframes =  ( wp.glide_effect * frames / 100.0 ) ;

	if ( abs(dframes) < 1E-4 )
	{
		wp.start_frq = freq;
		return 0.0;//freq - wp.start_frq; // do nothing
	}
	return ( freq - wp.start_frq ) / dframes;
}


void Oscillator::mem_init()
{
//	Reset_data( this );
//	return;

	for ( buffer_t n = 0; max_frames > n; n++ )
	{
		this->Mem_vco.Data[n] 	= max_data_amp;
		this->Mem_fmo.Data[n] 	= 0;
		this->Mem.Data[n] 		= 0;
	}

	this->vp.data = this->Mem_vco.Data;
	this->fp.data = this->Mem_fmo.Data;
	this->vp.name = this->osc_type;
	this->fp.name = this->osc_type;
	this->vp.osc_id 	= this->osctype_id;
	this->vp.osc_id 	= this->osctype_id;

	return;
}
void Oscillator::Reset_data( Oscillator* osc )
{
	this->Mem.Clear_data( 0 );
	this->Mem_fmo.Clear_data(0);
	this->Mem_vco.Clear_data(max_data_amp);

	this->vp.data = this->Mem_vco.Data;
	this->fp.data = this->Mem_fmo.Data;

	this->vp.volume = 0;
	this->fp.volume = 0;

	this->fp.name = osc->osc_type;
	this->vp.name = osc->osc_type;
	this->fp.osc_id = osc->osctype_id;
	this->vp.osc_id = osc->osctype_id;

}

#define MODPHI( phi, maxphi )\
	( abs(phi) > (maxphi) ) ? (phi) - sgn(phi)*(maxphi) : phi;

void Oscillator::OSC (  const buffer_t& frame_offset )
/*
 * Generator of sound waves
 */
{
	auto check_phi = [ this ]( param_t param, phi_t dT, float start_freq )
	{
		if ( param.maxphi < abs(param.dphi) )
		{
			cout << "osctype    " << osc_type  		<< endl;
			cout << "maxphi     " << param.maxphi 	<< endl;
			cout << "phi        " << param.phi  	<< endl;
			cout << "dT         " << dT   			<< endl;
			cout << "start_freq " << start_freq 	<< endl;
			EXCEPTION( "maxphi exceeds limit: " + to_string(param.maxphi) + " < " + to_string( abs(param.dphi)) );
		}
	};

	float				freq 		= this->wp.frequency;
	buffer_t 			frames  	= ( this->wp.msec * frames_per_sec) / 1000;
	phi_t 				dt 			= 1.0/frames_per_sec;	//seconds per frame

	Data_t* 			Data 		= this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmo_data	= this->fp.data 	+ frame_offset;// * sizeof_data;
	Data_t* 			vco_data	= this->vp.data 	+ frame_offset;// * sizeof_data;

	float 				delta_frq	= get_delta_freq( freq );
	float 				start_frq 	= wp.start_frq;


	float 				fmo_vol 	= 0.001*(float)this->fp.volume;
	Data_t 				vco_adjust 	= max_data_amp / 2;
	float				vol_per_cent= this->wp.volume * 0.01; // the volume of the main osc is managed by the mixer!

	if ( is_osc_type )
		if ( not is_notes_role  )
			vol_per_cent		= 1;// the volume of the osc is constant for the instr role
									// the volume is managed by the mixer
									 // If the osc has the notes role the osc volume is managed
									// by the rhythm volume and the mixer

	if ( not is_osc_type )
		vco_adjust = 0;

	if ( frames > max_frames )
		frames = max_frames;
	this->wp.frames = frames;

	if ( frame_offset + frames > max_frames )
	{
		Comment(WARN, "buffer overflow: " +
				to_string( frame_offset ) +" + "+
				to_string( frames ) +" > "+
				to_string( max_frames ));
		return;
	}

	Sum( spectrum );


	param_t param 	= param_struct();
	param.pmw		= 1.0 + (float)wp.PMW_dial * 0.01;
	phi_t dT 		= 0;
	float fmo_shift = 0;
				// difference to the target frequency <freq> - <start_freq>


	for ( size_t channel = 0; channel < spec_arr_len; channel++ )
	{
		if ( spectrum.vol[channel] > 0.0 )
		{
			param.phi	= phase[channel];
			uint8_t wfid= spectrum.wfid[channel];
			param.maxphi= waveFunction_vec[ wfid ].maxphi;
			dT 			= param.maxphi * dt; // 2pi dt
			param.amp	= spectrum.vol[channel];
			for( buffer_t n = 0; n < frames; n++ )
			{
				float vco_vol = ((vco_adjust + vco_data[n]) * vol_per_cent ); // VCO envelope
				fmo_shift = fmo_vol * fmo_data[n];
				Data[n]	=   Data[n] + vco_vol * waveFunction_vec[ wfid ].fnc( param );

				if ( abs(freq - start_frq) > 1 )
					start_frq = start_frq + delta_frq; // TODO - working
				param.dphi	=	dT *( start_frq + fmo_shift ) * spectrum.frqadj[channel] ;
				param.phi	+= param.dphi;
			}
			check_phi( param, dT, start_frq );
			param.phi 	=  MODPHI( param.phi, param.maxphi );
			phase[channel] = param.phi;
		}
	}

	Set_start_freq( start_frq );

	apply_adsr( frames, Data );
	apply_hall( frames, Data );

}

void Oscillator::Set_long( bool l )
{
	longnote = l ;
}
void Oscillator::Reset_cursor()
{
	beat_cursor = 0;
}
void Oscillator::apply_adsr( buffer_t frames, Data_t* data )
{
	if ( adsr.bps == 0 )	 	return;
	if ( not is_osc_type )		return;
	if ( adsrdata.size() == 0 )	return;

	for ( uint n = 0; n < frames; n++ )
	{
		data[ n ] = data[ n ] * adsrdata[ beat_cursor ];
		beat_cursor = ( beat_cursor + 1 ) % adsrdata.size();
	}
	if ( oscrole_id == OscRole.KBDID )
		cout << "beat_cursor: " << (int) beat_cursor << endl;
}

void Oscillator::apply_hall( buffer_t frames, Data_t* data )
{ 	// adsr.hall determines the distance to a wall
	// db describes the decay of an amplitude given at the origon n
	// dn is the distance of the wall in frame units
	//	buffer_t dn 	= ( ( adsr.hall*adsr.hall )/100.0 * max_frames ) / 100;;

	if ( adsr.hall == 0 )	return;
	if ( not is_osc_type ) 	return;

	auto gen_halldata = [ this, frames, data]( float db)
	{
		DataVec_t Data {};
		for ( buffer_t n = 0; n < frames; n++ )
		{
			Data.push_back( data[ n ] * db  );
		}
		return Data;
	};

	const float 	d0 		= 10.0; // distance to the receiver of sound
	const float 	distance= d0 + adsr.hall/10.0; // distance to a wall in meter [m]
	const float		c		= 330.0; // sound speed [m/s]
	const float 	dt 		= (2.0 * distance) / c; // time delay [seconds]
	const buffer_t 	dn 		= rint(dt * frames_per_sec) ;// frame delay [# of frames]
	const float		db 		= 0.4; // the decay is a constant of the wall

	DataVec_t halldata = gen_halldata( db );

	for ( buffer_t n = dn; n < frames; n++ )
	{
		data[n] = data[n] + halldata[n-dn];
	}
}

void Oscillator::Connect_vco_data( Oscillator* osc)
{
	this->vp.data 	= osc->Mem.Data;
	this->vp.osc_id = osc->osctype_id;
	this->vp.volume = osc->wp.volume;
	this->vp.name 	= osc->osc_type;
}

void Oscillator::Connect_fmo_data( Oscillator* osc )
{
	this->fp.data 	= osc->Mem.Data;
	this->fp.osc_id = osc->osctype_id;
	this->fp.volume = osc->wp.volume;
	this->fp.name 	= osc->osc_type;
}


void Oscillator::Test()
{
	TEST_START( className );


	SetId( OscRole.NOTESID, OscRole.VCOID );
	assert( abs( Mem_vco.Data[0] - max_data_amp)	< 1E-8 );
	assert( abs( Mem_fmo.Data[0]				)	< 1E-8 );
	assert( Mem.ds.data_blocks 	== max_frames );

	vector_str_t arr = { "TYPE","VCO","Sinus","17","2000","100","2","1","1","69","2","0","-1","0","42" };
	Line_interpreter( arr );
	ASSERTION( fcomp( wp.frequency, 8) , "Frequency", wp.frequency, 8 );
	Set_duration( max_milli_sec );

	ASSERTION( fcomp( oct_base_freq, Calc( C0 )), "osc_base_freq" , oct_base_freq, Calc( C0 ));
	spectrum 	= spec_struct();
	adsr 		= adsr_struct();


	longnote = true;
	adsr.attack = 50;
	adsr.decay = 5;

	Oscillator testosc {};
	testosc.SetId( osc_struct::INSTRID, osc_struct::OSCID );
	phase[0]	= 0.444;
	testosc.Set_frequency(220, FIXED );
	testosc.wp.volume	= 100;
	testosc.OSC( 0 );
	testosc.OSC( 0 );
	Comment( TEST, testosc.Show_this_spectrum() );
	float a0 = testosc.Mem.Data[0];
	float a2 = testosc.Mem.Data[2];
	ASSERTION( not fcomp( a0, a2 ), "gen data", a0-a2, "not null" );
	ASSERTION( fcomp( a0 , 56.5484  ), "Osc volume", a0 , 56.5484)

	testosc = *this; // test copy constructor
	ASSERTION( adsr.attack == 50 , "", adsr.attack, 50 );
	ASSERTION( wp.frequency == 8 , "",wp.frequency , 8 );


	TEST_END( className );
}

