/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#include <Osc.h>

phi_t	dphi 	= 0.0;
phi_t 	maxphi 	= 2*pi;

random_device 	rd;
mt19937 		engine(rd());
uniform_real_distribution<> distrib( -1, 1 );

// https://stackoverflow.com/questions/6339970/c-using-function-as-parameter

constexpr int sgn( const float& value )
{
	return ( value >= 0 ) ? 1 : -1;
}

constexpr Data_t signum(  const float& value)
{
	return ( value > 0 ) ? value : 0 ;
}

float rnd(  const float& amp,  const float& phi )
{ 	// provides values between -amp .. amp
	float rint = amp * distrib( engine );
	return rint;
}

constexpr float fmodulo(  const float& x,  const float& y )
{ 	// for small steps dx < y
	return ( x >= y ) ? x-y : x ;
}


float delta(  const float& x,  const float& y )
{
	float f1 = fmodulo(y+dphi, maxphi + dphi);
	float f2 = fmodulo(y+dphi, maxphi );
	return x*( f1-f2 );
}

float rndprev = 0.0;
float rnd_step(  const float& amp,  const float& y )
{
	float trigger = delta(y+dphi, maxphi );
	if( abs( trigger ) > 1E-2 )
	{
		rndprev = rnd(amp, 0);
		return rndprev;
	}
	else
		return rndprev;
}
constexpr float modulo(  const float& x,  const float& n )
{
	float 	d 	= floor ( x / n ); // floor corresponds to the gaussian bracket
	return 	x - d*n;
}
constexpr Data_t maximum(  const float& x,  const float& y)
{
	return ( x > y ) ? x : y;
}
constexpr Data_t One( const float& amp, const phi_t& phi )
{
	return 1;
}
constexpr Data_t Zero( const float& amp, const phi_t& phi)
{
	return 0;
}
constexpr Data_t Sin(  const float& amp,  const phi_t& phi )
{
	return (  amp * sin( phi ));
}
constexpr Data_t SignSin( const float& amp, const phi_t& phi )
{
	return ( signum( amp * sin ( phi ) ));
}
constexpr Data_t rectangle( const float& amp, const phi_t& phi ) // 'r'
{ // maxima: round(mod(phi,1))*amp*2-amp;
	return -amp*( round(floor(phi)-phi)) - amp/2;
}
constexpr Data_t Triangle( const float& amp, const phi_t& phi ) // 'T'
{ // maxima: f:2*amp*(abs(max( 1 - mod(phi, 2), -1 ) )-1)+amp;
	return (2*amp*(abs(maximum( 1 - modulo(phi, 2), -1 ) )-1)+amp);
}
constexpr Data_t sawtooth( const float& amp,  const phi_t& phi )
{
	return ( amp * modulo(phi,1 ));
}
constexpr Data_t Sawtooth( const float& amp,  const phi_t& phi )
{
	return (amp* (1.0 - modulo(phi,1 )));
}
constexpr Data_t Pmw( const float& maxphi, const phi_t& phi )
{
	return abs(round(modulo(phi , maxphi)));
}



Oscillator::Oscillator( ) :
		Logfacility_class( "Oscillator" ),
		Oscillator_base()
{
	className 	= Logfacility_class::module;
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

	is_main_osc = ( osctype_id == OscRole.OSCID );
	is_notes_role = ( oscrole_id == OscRole.NOTESID );

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

double Oscillator::get_delta_freq( float freq )
{
	// 0..100 |-> 0..frames = 0..max_sec
	buffer_t 			frames  	= ( this->wp.msec*audio_frames) / 1000;

	if ( abs(wp.start_frq) < 1E-4 ) return freq;  			// do nothing
	float dframes =  ( wp.glide_effect * frames / 100.0 ) ;

	if ( abs(dframes) < 1E-4 ) return freq - wp.start_frq; 	// do nothing
//	Assert( dframes > 0, "dframes: " + to_string(dframes));
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
	float				freq 		= this->wp.frequency;
	buffer_t 			frames  	= ( this->wp.msec * frames_per_sec) / 1000;
	phi_t 				dt 			= 1.0/frames_per_sec;	//seconds per frame

	Data_t* 			Data 		= this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmo_data	= this->fp.data 	+ frame_offset;// * sizeof_data;
	Data_t* 			vco_data	= this->vp.data 	+ frame_offset;// * sizeof_data;

	float 				start_freq 	= wp.start_frq;
	float 				fmo_vol 	= 0.001*(float)this->fp.volume;
	Data_t 				vco_adjust 	= max_data_amp / 2;
	float				vol_per_cent= this->wp.volume * 0.01; // the volume of the main osc is managed by the mixer!

	if ( is_main_osc )
		if ( not is_notes_role  )
			vol_per_cent		= 1; // the volume of the main osc if it has not notes
										// role is managed by the mixer.
									 // If it has notes role the main osc volume is managrf by the rhythm volume
	// and the mixer

	if ( not is_main_osc )
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

	switch ( spectrum.wfid )
	{
		case SINUS0 : // s inus + spectrum
		{
			maxphi 	= 2*pi;
			F = Sin;
			break;
		}
		case SINUS1 : // Sinus
		{
			maxphi 	= 2*pi;
			F = Sin;
			break;
		}
		case SGNSIN : // sgn Sin
		{
			maxphi 	= 2*pi;
			F = SignSin;
			break;
		}
		case RECTANGLE : // rectangle
		{
			maxphi 	= 1;
			F = rectangle;
			break;
		}
		case SAWTOOTHL : // sawtooth
		{
			maxphi 	= 1;
			F = sawtooth;
			break;
		}
		case SAWTOOTHR : // Sawtooth ( reverse sawtooth )
		{
			maxphi 	= 1;
			F = Sawtooth;
			break;
		}
		case TRIANGLE :// triangle https://de.wikipedia.org/wiki/Dreiecksfunktion
		{
			maxphi 	= 2;
			F = Triangle;
			break;
		}
		case PMW :// PMW
		{
			maxphi 					= 1;
			double 			phi 	= this->phase[0];
			dphi					= 0.0;
			F						= Pmw;
			const double 	dT		= 2*dt;
			const float 	frames2 = 1.0 + (float)wp.PMW_dial * 0.01;

			for ( buffer_t n = 0; frames > n; n++ )
			{
				float vco_vol = ((vco_adjust + vco_data[n]) * vol_per_cent ) ; // VCO envelope
				float phi2 	= phi * frames2 ;
				Data[n] 	= vco_vol * F( maxphi , phi ) * F( maxphi, phi2 );
				dphi		= dT  * freq ;
				phi 		= phi + dphi;
				phi			= ( abs(phi) > maxphi ) ? phi-sgn(phi)*maxphi : phi ;
				if ( ( phi > maxphi ) or ( phi < -maxphi ) )
					cout << "phi overflow: " << phi << endl;
			}
			this->phase[0] = phi;

			apply_adsr( frames, Data );
			apply_hall( frames, Data ) ;

			return;
			break;
		}
		case DELTA : // delta
		{
			maxphi = 1;
			F = delta;
			break;
		}
		case NOISE :// noise
		{
			maxphi = 1;
			F  = rnd;
			break;
		}
		case RANDOM :// random
		{
			maxphi = 1;
			F  = rnd_step;
			break;
		}
		default :
		{
			F = Zero;
			break;
		}
	} // close switch waveform


	const phi_t 	dT 			= maxphi * dt; // 2pi dt
	float			delta_freq	= get_delta_freq( freq );
	phi_t			omega = 0, d_omega = 0;
				// difference to the target frequency <freq> - <start_freq>

	auto check_phi = [ this, dT, start_freq]( phi_t phi, phi_t maxphi, phi_t dphi )
	{
		if ( maxphi < abs(dphi) )
		{
			cout << "osctype    " << osc_type  	<< endl;
			cout << "maxphi     " << maxphi 	<< endl;
			cout << "phi        " << phi  		<< endl;
			cout << "dT         " << dT   		<< endl;
			cout << "start_freq " << start_freq << endl;
			EXCEPTION( "maxphi exceeds limit: " + to_string(maxphi) + " < " + to_string( abs(dphi)) );
		}
	};

	for ( buffer_t n = 0; n < frames ; n++ )
	{
		if (( this->oscrole_id != OscRole.NOTESID )) // enable polyphone adding of notes - notes::note2memory
			Data[n] = max_data_amp * wp.adjust * 0.01;

		float vco_vol = ((vco_adjust + vco_data[n]) * vol_per_cent ); // VCO envelope
		float fmo_shift = fmo_vol * fmo_data[n];

		for ( size_t channel = 0; channel < spec_arr_len; channel++ )
		{
			if ( spectrum.vol[channel] > 0.0 )
			{
				omega	= phase[channel];
				Data[n]	=   Data[n] + vco_vol * F( spectrum.vol[channel], omega );
				d_omega	=	dT *( start_freq + fmo_shift ) * spectrum.frqadj[channel] ;
				omega	+= d_omega;
				check_phi( omega, maxphi, d_omega );
				omega 	=  MODPHI( omega, maxphi );
				phase[channel] = omega;
			}
		}
		if ( abs(freq - start_freq) > 1 )
			start_freq = start_freq + delta_freq;
	}


	Set_start_freq(freq);

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
	if ( not is_main_osc )		return;
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
	if ( not is_main_osc ) 	return;

	auto gen_halldata = [ this, frames, data]( float db)
	{
		DataVec_t Data {};
		for ( buffer_t n = 0; n < frames; n++ )
		{
			Data.push_back( data[ n ] * db  );
		}
		return Data;
	};

	const float 	d0 		= 1.0; // distance to the receiver of sound
	const float 	distance= d0 + adsr.hall/10.0; // distance to a wall in meter [m]
	const float		c		= 330.0; // sound speed [m/s]
	const float 	dt 		= (2.0 * distance) / c; // time delay [seconds]
	const buffer_t 	dn 		= rint(dt * frames_per_sec) ;// frame delay [# of frames]
	const float		db 		= 0.8; // the decay is a constant of the wall

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

	ASSERTION( fcomp( modulo(1,2), 1 ), "modulo", modulo(1,2), "1")
	ASSERTION( fcomp( modulo(2,1.2), 0.8), "modulo", modulo(2,1.2), "0.8")
	ASSERTION( fcomp( modulo(1,0.9), 0.1), "modulo", modulo(1,0.9), "0.1")
	ASSERTION( fcomp( modulo(2,0.9), 0.2), "modulo", modulo(2,0.9), "0.2")

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
	testosc.Set_frequency(220);
	testosc.wp.volume	= 100;
	testosc.OSC( 0 );
	testosc.OSC( 0 );
	float a2 = testosc.Mem.Data[0];

	ASSERTION( fcomp( a2 , 56.5486  ), "Osc volume", a2 , 56.5486)

	testosc = *this; // test copy constructor
	ASSERTION( adsr.attack == 50 , "", adsr.attack, 50 );
	ASSERTION( wp.frequency == 8 , "",wp.frequency , 8 );


	TEST_END( className );
}

