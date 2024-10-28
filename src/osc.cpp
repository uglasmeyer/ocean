/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */





#include <Osc.h>

double	dphi 	= 0.0;
double 	maxphi 	= 2*pi;

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
	if ( value > 0) return value;
	else return 0;

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
constexpr float modulo(  const float& x,  const int& n )
{
	long int d = floor ( x / n ); // floor corresponds to the gaussian bracket
	return x - d*n;
}

constexpr Data_t maximum(  const float& x,  const float& y)
{
	if ( x > y )
		return x;
	else
		return y;
}


constexpr Data_t One( const float& amp, const float& phi )
{
	return 1;
}
constexpr Data_t Zero( const float& amp, const float& phi)
{
	return 0;
}
constexpr Data_t Sin(  const float& amp,  const float& phi )
{
//	return rint(  amp * sin( phi ));
	return (  amp * sin( phi ));
}
constexpr Data_t SignSin( const float& amp, const float& phi )
{
//	return rint( signum( amp * sin ( phi ) ));
	return ( signum( amp * sin ( phi ) ));
}
constexpr Data_t rectangle( const float& amp, const float& phi ) // 'r'
{ // maxima: round(mod(phi,1))*amp*2-amp;
	return -amp*( round(floor(phi)-phi)) - amp/2;
}
constexpr Data_t Triangle( const float& amp, const float& phi ) // 'T'
{ // maxima: f:2*amp*(abs(max( 1 - mod(phi, 2), -1 ) )-1)+amp;
	return round(2*amp*(abs(maximum( 1 - modulo(phi, 2), -1 ) )-1)+amp);
}
constexpr Data_t sawtooth( const float& amp,  const float& phi )
{
	return rint( amp * modulo(phi,1 ));
}
constexpr Data_t Sawtooth( const float& amp,  const float& phi )
{
	return rint(amp* (1.0 - modulo(phi,1 )));
}

Oscillator::Oscillator( uint8_t id ) :
		Logfacility_class( "OSC" ),
		Oscillator_base()
{
	mem_init();
	osc_id		= id;
	osc_type 	= osc_type_vec[id];
	Mem_vco.Info( osc_type );
	Mem_fmo.Info( osc_type );
	Mem.Info( osc_type );
	Comment( INFO, osc_type + " initialized" );

};
Oscillator::~Oscillator(){};

void Oscillator::Set_start_freq( float freq )
{
	start_freq = freq;
}

double Oscillator::get_delta_freq( float freq )
{
	// 0..100 |-> 0..frames = 0..max_sec
	buffer_t 			frames  	= ( this->wp.msec*audio_frames) / 1000;

	if ( abs(start_freq) < 1E-4 ) return freq;  			// do nothing
	float dframes =  ( wp.glide_effect * frames / 100.0 ) ;

	if ( abs(dframes) < 1E-4 ) return freq - start_freq; 	// do nothing
	return ( freq - start_freq ) / dframes;

}
void Oscillator::set_phi( double phi, double mod ) //phase at the end of the osc
{
	this->phase	= phi;//( phi > mod ) ? phi-mod : phi ;
}

double Oscillator::get_phi( )// phase at the begin of the osc
{
//	return fmodulo( this->wp.phi, mod );
	if ( osc_id == NOTESID )
		this->phase = 0.0;
	return this->phase;
}

void Oscillator::mem_init()
{
	for ( buffer_t n = 0; max_frames > n; n++ )
	{
		this->Mem_vco.Data[n] 	= max_data_amp;
		this->Mem_fmo.Data[n] 	= 0;
		this->Mem.Data[n] 		= 0;
	}

	this->wp.touched = true;

	this->vp.data = this->Mem_vco.Data;
	this->fp.data = this->Mem_fmo.Data;
	this->vp.name = this->osc_type;
	this->fp.name = this->osc_type;
	return;
}

void Oscillator::OSC (  buffer_t frame_offset )
/*
 * Generator of sound waves
 */
{
	buffer_t 			n;
	buffer_t 			frames  	= ( this->wp.msec*audio_frames) / 1000;
	double 				dt 			= 1.0/audio_frames;//seconds per frame
	float				volume  	= (float) this->wp.volume;
	Data_t* 			data 		= this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmo_data	= this->fp.data 	+ frame_offset;// * sizeof_data;
	Data_t* 			vco_data	= this->vp.data 	+ frame_offset;// * sizeof_data;
	float 				norm_freq 	= 0.001*(float)this->fp.volume;

	float freq = this->wp.frequency;

	Data_t 	vco_shift 	= max_data_amp/2;
	if	(
		( osc_id == VCOID ) or
		( osc_id == FMOID )
		)
	{
		vco_shift = 0;
	}

	float	vol_per_cent =  volume / 100.0; // the volume of the main osc is managed by the mixer!
	if ( osc_id == MAINID )
		vol_per_cent= 1; // the volume of the main osc is managed by the mixer
						 // or notes (NOTEID)!

	if ( frames > max_frames )
		frames = max_frames;
	this->wp.frames = frames;

	Sum( spectrum );
	if ( spectrum.sum == 0 ) spectrum.sum  = 1;

	wp.touched = false;
	switch ( spectrum.id )
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
			double 			phi 	= this->phase;
			dphi					= 0.0;
			const double 	dT		= 2*dt;
			const float 	frames2 = 1.0 + (float)wp.PMW_dial / 100.0;

			for ( n = 0; frames > n; n++ )
			{
				float vco_vol = ((vco_shift + vco_data[n]) * vol_per_cent ) ; // VCO envelope
				float phi2 	= phi * frames2 ;
				data[n] = round(vco_vol)*		abs(round(modulo(phi , maxphi))) *
												abs(round(modulo(phi2, maxphi)));
				dphi		= dT  * freq ;
				phi 		= phi + dphi;
				phi			= ( abs(phi) > maxphi ) ? phi-sgn(phi)*maxphi : phi ;
				if ( ( phi > maxphi ) or ( phi < -maxphi ) )
					cout << "phi overflow: " << phi << endl;
			}
			set_phi( phi, maxphi );

			apply_adsr( this->adsr, frames, data);
			apply_hall( this->adsr, frames, data) ;

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
			;
			maxphi = 1;
			F  = rnd;
			break;
		}
		case RANDOM :// random
		{
			;
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

	if ( frame_offset + frames > max_frames )
	{
		Comment(WARN, "buffer overflow: " +
				to_string( frame_offset ) +" + "+
				to_string( frames ) +" > "+
				to_string( max_frames ));
		return;
	}

	double 			omega_t		= 0;
	const double 	dT 			= maxphi * dt; // 2pi dt
	double 			phi			= get_phi( );

	float 			start_freq 	= this->start_freq;
	float			delta_freq		= get_delta_freq( freq );
				// difference to the target frequency <freq> - <start_freq>
	for ( n = 0; n < frames ; n++ )
	{
		if (( this->osc_id != NOTESID )) // enable polyphone adding of notes - notes::note2memory
			data[n] = 0;

		float vco_vol = ((vco_shift + vco_data[n]) * vol_per_cent ) / spectrum.sum; // VCO envelope
		for ( size_t mode = 0; mode < spec_dta_len; mode++ )
		{
			if ( spectrum.dta[mode] != 0 )
			{
				omega_t =   phi * ( 2 + mode ) * 0.5 ;
				data[n]	=   data[n] + F(spectrum.dta[mode]*vco_vol, omega_t);
			}
		}

		if ( abs(freq - start_freq) > 1 ) start_freq = start_freq + delta_freq;
		dphi	= dT * ( start_freq + norm_freq*fmo_data[n] );
		phi 	+= dphi;
		if ( maxphi < abs(dphi) )
		{
			cout << "osc_id     " << osc_type_vec[osc_id]    	<< endl;
			cout << "dphi       " << dphi    	<< endl;
			cout << "maxphi     " << maxphi 	<< endl;
			cout << "phi        " << phi  		<< endl;
			cout << "dT         " << dT   		<< endl;
			cout << "start_freq " << start_freq << endl;
			Exception( "maxphi exceeds limit < " + to_string( abs(dphi)) );
		}
		phi		= ( abs(phi) > maxphi ) ? phi-sgn(phi)*maxphi : phi ;
		if ( ( phi > maxphi ) or ( phi < -maxphi ) )
			cout << "phi overflow: " << phi << endl;
//		assert( phi <=  lfo_mod );
//		assert( phi >= -lfo_mod );
	}

	set_phi( phi, maxphi );
	Set_start_freq(freq);

	apply_adsr( this->adsr, frames, data);
	apply_hall( this->adsr, frames, data) ;

	return;

}

void Oscillator::Set_long( bool l )
{
	longnote = l ;
}

bool main_id( int id )
{
	for ( int ID : { MAINID, NOTESID, KBDID })
	{
		if ( id == ID ) return true;
	}
	return false;
}

void Oscillator::apply_adsr(adsr_t adsr, buffer_t frames, Data_t* data  )
{
	auto attack = [ frames, data ]( int duration, buffer_t aframes, float da )
		{
		for ( int step = 0; step < duration; step++ )
		{
			buffer_t offs = ( frames * step ) / duration;
			for ( buffer_t n = 0; n < aframes ; n++)
			{
				data[n + offs ] = data[n + offs ] * (n+1)*da;
			}
		}
		};
	auto decay = [ this, data ]( int duration, buffer_t rframes, buffer_t aframes, float alpha0 )
		{
		for ( int step = 0; step < duration; step++ )
		{
			buffer_t offs = (rframes*step  ); ///duration;
			for ( buffer_t n = aframes; n < rframes; n++ )
			{
				float alpha 	= (n - aframes + decay_shift) * alpha0;
				data[n+offs]	= data[n+offs] * exp( -alpha )   ;
			}
		}

		};

	if ( adsr.bps == 0 ) 		return;
	if ( not main_id( osc_id ) ) 	return;


	int 		duration = 1; // each note has a single attack/decay
	if ( osc_id == MAINID )
		//		duration 	= bps_struct().getbps( adsr.bps );
				duration 	= adsr.bps;
	buffer_t 	aframes		= 0;
	float 		da			= 0;

	if ( longnote )
	// decay_shift is last n
		aframes = 0;
	else
	{
		aframes 	= (( frames * adsr.attack ) / 100 ) / duration;
		decay_shift	= 0;
		da 			= 1.0/aframes;
	}

	attack( duration, aframes, da );

	buffer_t  	rframes	=  frames / duration;
	float 		alpha0 	= ( duration * (100 - adsr.decay )/2.0) / frames;

	decay( duration, rframes, aframes, alpha0 );

	decay_shift = rframes; // remember last n
}

Memory 	memtmp	{ monobuffer_size }; //adsr hall - not member of Oscillator

void Oscillator::apply_hall( adsr_t adsr, buffer_t frames, Data_t* data )
{ 	// adsr.hall determines the distance to a wall
	// db describes the decay of an amplitude given at the origon n
	// dn is the distance of the wall in frame units
	//	buffer_t dn 	= ( ( adsr.hall*adsr.hall )/100.0 * max_frames ) / 100;;

	if ( adsr.hall == 0 ) 		return;
	if ( not main_id( osc_id )) return;

	float 		d0 		= 1; // distance to the receiver of sound
	float 		distance= d0 + adsr.hall/10.0; // distance to a wall in meter [m]
	uint 		c		= 330; // sound speed [m/s]
	float 		dt 		= (2.0 * distance) / c; // time delay [seconds]
	buffer_t 	dn 		= rint(dt * frames_per_sec) ;// frame delay [# of frames]

	float db 		= 0.5; // the decay is a constant of the wall

	for ( buffer_t n = 0; n < memtmp.ds.data_blocks; n++ )
	{
		buffer_t m =  ( n + dn ) % frames;
		memtmp.Data[m]	=  data[m] + rint( data[ n ] * db )  ;
	}
	for ( buffer_t n = 0; n < frames; n++ )
	{
		data[n] = memtmp.Data[n];
	}
}


void Oscillator::Connect_vco_data( Oscillator* osc)
{
	this->wp.touched = true;
	this->vp.data 	= osc->Mem.Data;
	this->vp.volume = osc->wp.volume;
	this->vp.name 	= osc->osc_type;
}

void Oscillator::Connect_fmo_data( Oscillator* osc )
{
	this->wp.touched = true;
	this->fp.data 	= osc->Mem.Data;
	this->fp.volume = osc->wp.volume;
	this->fp.name 	= osc->osc_type;
}

void Oscillator::Reset_data( Oscillator* osc )
{
	this->wp.touched = true;
	this->Mem_fmo.Clear_data(0);
	this->Mem_vco.Clear_data(max_data_amp);

	this->vp.data = this->Mem_vco.Data;
	this->fp.data = this->Mem_fmo.Data;

	this->vp.volume = 50;
	this->fp.volume = 50;

	this->fp.name = osc->osc_type;
	this->vp.name = osc->osc_type;

}

void Oscillator::Test()
{
	Set_Loglevel(TEST, true );

	Comment( TEST, "Osc test start");
	osc_id 			= NOTESID;
	assert( ( Mem_vco.Data[0] - max_data_amp)	< 1E-8 );
	assert( ( Mem_fmo.Data[0]				)	< 1E-8 );
	assert( Mem.ds.data_blocks 	== frames_per_sec );

	vector_str_t arr = { "OSC","MAIN","Sinus","480","1000","40","2","1","1","69","2","0","-1","0","42" };
	Line_interpreter( arr );
	assert( abs(wp.frequency) - 480 < 1 );
	assert( wp.frequency > 0.0 );
	assert( abs( wp.frequency - 480 ) < 1E-8 );
	spectrum = spec_struct();
	OSC( 0 );
	cout << "Phase: " << phase <<  " " << 2*pi << endl;
	if ( (phase - 2*pi ) > 1E-5)
		assert(false);

	longnote = true;
	adsr.attack = 50;
	adsr.decay = 5;
	assert( decay_shift == frames_per_sec );


	Comment( TEST, "Osc test finished");
}

