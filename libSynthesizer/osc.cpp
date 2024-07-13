/*
 * osc.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */





#include <osc.h>

// https://stackoverflow.com/questions/6339970/c-using-function-as-parameter



uint16_t signum( float value)
{
	if ( value > 0) return value;
	else return 0;

}
random_device rd;
mt19937 engine(rd());
uniform_real_distribution<> distrib( -1, 1 );

float rnd( float amp, float phi )
{ 	// provides values betwee -amp .. amp
	float rint = amp * distrib( engine );
	return rint;
}

float fmodulo( float x, float y )
{ 	// for small steps dx < y
	return ( x >= y ) ? x-y : x ;
}

float dphi = 0.0;
float modc = 2*pi;
float delta( float x, float y )
{
	float f1 = fmodulo(y+dphi, modc + dphi);
	float f2 = fmodulo(y+dphi, modc );
	return x*( f1-f2 );
}

float rndprev = 0.0;
float rnd_step( float amp, float y )
{
	float trigger = delta(y+dphi, modc );
	if( abs( trigger ) > 1E-2 )
	{
		rndprev = rnd(amp, 0);
		return rndprev;
	}
	else
		return rndprev;
}
float modulo( float x, int n )
{
	long int d = floor ( x / n ); // floor corresponds to the gaussian bracket
	return x - d*n;
}

float maximum( float x, float y)
{
	if ( x > y )
		return x;
	else
		return y;
}


Data_t One( float amp, float phi )
{
	return 1;
}
Data_t Zero( float amp, float phi)
{
	return 0;
}
Data_t Sin( float amp, float phi )
{
//	return rint(  amp * sin( phi ));
	return (  amp * sin( phi ));
}
Data_t SignSin( float amp, float phi )
{
//	return rint( signum( amp * sin ( phi ) ));
	return ( signum( amp * sin ( phi ) ));
}
Data_t rectangle( float amp, float phi ) // 'r'
{ // maxima: round(mod(phi,1))*amp*2-amp;
	return -amp*( round(floor(phi)-phi)) - amp/2;
}
Data_t Triangle( float amp, float phi ) // 'T'
{ // maxima: f:2*amp*(abs(max( 1 - mod(phi, 2), -1 ) )-1)+amp;
	return round(2*amp*(abs(maximum( 1 - modulo(phi, 2), -1 ) )-1)+amp);
}
Data_t sawtooth( float amp,  float phi )
{
	return rint( amp * modulo(phi,1 ));
}
Data_t Sawtooth( float amp,  float phi )
{
	return rint(amp* (1.0 - modulo(phi,1 )));
}

void Oscillator::set_start_freq( float freq )
{
	start_freq = freq;
}

double Oscillator::get_delta_freq( double freq )
{
	// 0..100 |-> 0..frames = 0..max_sec
	buffer_t 			frames  	= ( this->wp.msec*audio_frames) / 1000;

	if ( abs(start_freq) < 1 ) return freq;  			// do nothing
	float dframes =  ( wp.glide_effect * frames / 100.0 ) ;

	if ( abs(dframes) < 1 ) return freq - start_freq; 	// do nothing
	return ( freq - start_freq ) / dframes;

}
void Oscillator::set_phi( double phi ) //phase at the end of the osc
{
	this->phase = phi;
}

double Oscillator::get_phi( )// phase at the begin of the osc
{
//	return fmodulo( this->wp.phi, mod );
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
//	F = Zero;

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
	//cout << "modulo: " << dec << modulo( pi, 4 ) << endl;
	//exit(1);
	buffer_t 			n;
	buffer_t 			frames  	= ( this->wp.msec*audio_frames) / 1000;
	double 				dt 			= 1.0/audio_frames;//seconds per frame
	float				volume  	= (float) this->wp.volume;
	spec_struct_t		spectrum	= this->wp.spectrum;
	Data_t* 			data 		= this->Mem.Data	+ frame_offset;// * sizeof_data; // define snd data ptr
	Data_t*				fmo_data	= this->fp.data 	+ frame_offset;// * sizeof_data;
	Data_t* 			vco_data	= this->vp.data 	+ frame_offset;// * sizeof_data;
	float 				norm_freq 	= 0.002*(float)this->fp.volume;

	float freq = this->wp.ffreq;
	if ( ID == LFOID )
		freq = (float) this->wp.frequency/20.0;

	Data_t 	vco_shift 	= max_data_amp/2;
	if (( ID == VCOID ) or
		( ID == LFOID ) or
		( ID == FMOID ))
		vco_shift = 0;

	float	vol_per_cent =  volume / 100.0; // the volume of the main osc is managed by the mixer!
	if ( ID == MAINID )
		vol_per_cent= 1; // the volume of the main osc is managed by the mixer
						 // or notes (NOTEID)!

	if ( frames > max_frames )
		frames = max_frames;
	this->wp.frames = frames;

	switch ( this->wp.spectrum.id )
	{
		case 0 : // s inus + spectrum
		{
			modc 	= 2*pi;
			F = Sin;
			break;
		}
		case 1 : // Sinus
		{
			modc 	= 2*pi;
			F = Sin;
			break;
		}
		case 2 : // sgn Sin
		{
			modc 	= 2*pi;
			F = SignSin;
			break;
		}
		case 3 : // rectangle
		{
			modc 	= 1;
			F = rectangle;
			break;
		}
		case 4 : // sawtooth
		{
			modc 	= 1;
			F = sawtooth;
			break;
		}
		case 5 : // Sawtooth ( reverse sawtooth )
		{
			modc 	= 1;
			F = Sawtooth;
			break;
		}
		case 6 :// triangle https://de.wikipedia.org/wiki/Dreiecksfunktion
		{
			modc 	= 2;
			F = Triangle;
			break;
		}
		case 7 :// PMW
		{
			modc = 1;
			float phi 		= get_phi();
			dphi			= 0.0;
			float dT 		= 2*dt;
			float frames2 	= 1.0 + this->wp.PMW_dial/100.0;
			for ( n = 0; frames > n; n++ )
			{
				float vco_vol = ((vco_shift + vco_data[n]) * vol_per_cent ) / spectrum.sum; // VCO envelope
				float phi2 	= phi * frames2 ;
				data[n]		= round(vco_vol)*
								abs(round(modulo(phi , modc))) *
								abs(round(modulo(phi2, modc)));
				dphi		= dT  * freq ;
				phi 		= phi + dphi;
				phi 		= fmodulo(phi, modc);
			}
			set_phi( phi );

			Adsr( this->adsr, frames, data);
			hall_effect( this->adsr, frames, data) ;

			return;
			break;
		}
		case 8 : // delta
		{
			modc = 1;
			F = delta;
			break;
		}
		case 9 :// noise
		{
			;
			modc = 1;
			F  = rnd;
			break;
		}
		case 10 :// random
		{
			;
			modc = 1;
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

	if ( spectrum.sum == 0 ) spectrum.sum  = 1;
	float 		omega_t;
	float 		dT 		= modc * dt;
	float		lfo_mod = modc;
	double 		phi		= get_phi( );

	float 		start_freq = this->start_freq;
	float		delta_f	= get_delta_freq( freq );
				// difference to the target frequency <freq> - <start_freq>
	for ( n = 0; n < frames ; n++ )
	{

		if ( this->ID != NOTESID ) // enable polyphone adding of notes - notes::note2memory
			data[n] = 0;
		Data_t fmodata = norm_freq*fmo_data[n];

		float vco_vol = ((vco_shift + vco_data[n]) * vol_per_cent ) / spectrum.sum; // VCO envelope
		for ( size_t df = 0; df < spec_dta_len; df++ )
		{
			if ( spectrum.dta[df] != 0 )
			{
				omega_t =   phi * (1 + df )  ;
				data[n]	=   data[n] + F(spectrum.dta[df]*vco_vol, omega_t);
			}
		}

		if ( abs(freq - start_freq) > 1 ) start_freq = start_freq + delta_f;
		dphi	= dT * ( start_freq + fmodata);
		phi 	= phi + dphi;
		phi		= ( phi > lfo_mod ) ? phi-lfo_mod : phi ;//fmodulo(phi, mod);
	}

	set_phi( phi );
	set_start_freq(freq);

	Adsr( this->adsr, frames, data);
	hall_effect( this->adsr, frames, data) ;


	return;

}

void Oscillator::set_long( bool l ){ longnote = l ;};

void Oscillator::Adsr(adsr_struc_t adsr, buffer_t frames, Data_t* data  )
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

	if ( adsr.bps_id == 0 ) return;
	if ( not (( ID == MAINID ) or ( ID == NOTESID ))) return;

	adsr.bps_id				= adsr.bps_id % Bps_array.size();
	int 		duration 	= Bps_array[adsr.bps_id ];
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

void Oscillator::hall_effect( adsr_struc_t adsr, buffer_t frames, Data_t* data )
{ 	// adsr.hall determines the distance to a wall
	// db describes the decay of an amplitude given at the origon n
	// dn is the distance of the wall in frame units
	//	buffer_t dn 	= ( ( adsr.hall*adsr.hall )/100.0 * max_frames ) / 100;;

	if ( adsr.hall == 0 ) return;
	if ( not (( ID == MAINID ) or ( ID == NOTESID ))) return;

	float 		d0 		= 1; // distance to the receiver of sound
	float 		distance= d0 + adsr.hall/10.0; // distance to a wall in meter [m]
	uint 		c		= 330; // sound speed [m/s]
	float 		dt 		= (2.0 * distance) / c; // time delay [seconds]
	buffer_t 	dn 		= rint(dt * frames_per_sec) ;// frame delay [# of frames]

	float db 		= 0.5; // the decay is a constant of the wall

	for ( buffer_t n = 0; n < memtmp.info.data_blocks; n++ )
	{
		buffer_t m =  ( n + dn ) % frames;
		memtmp.Data[m]	=  data[m] + rint( data[ n ] * db )  ;
	}
	for ( buffer_t n = 0; n < frames; n++ )
	{
		data[n] = memtmp.Data[n];
	}
}


void Oscillator::connect_vco_data( Oscillator* osc)
{
	assert( osc->Mem.Data != NULL );
	this->vp.data 	= osc->Mem.Data;
	this->vp.volume = osc->wp.volume;
	this->vp.name 	= osc->osc_type;
}

void Oscillator::connect_fmo_data( Oscillator* osc )
{
	assert( osc->Mem.Data != NULL );
	this->fp.data 	= osc->Mem.Data;
	this->fp.volume = osc->wp.volume;
	this->fp.name 	= osc->osc_type;
}

void Oscillator::reset_data( Oscillator* osc )
{
	this->Mem_fmo.clear_data(0);
	this->Mem_vco.clear_data(max_data_amp);

	this->vp.data = this->Mem_vco.Data;
	this->fp.data = this->Mem_fmo.Data;

	this->vp.volume = 50;
	this->fp.volume = 50;

	this->fp.name = osc->osc_type;
	this->vp.name = osc->osc_type;

}

void Oscillator::test()
{
	Set_Loglevel(TEST, true );
	Spectrum_class::Test();

	Comment( TEST, "Osc test start");
	ID 			= NOTESID;
	assert( ( Mem_vco.Data[0] - max_data_amp)	< 1E-8 );
	assert( ( Mem_fmo.Data[0]				)	< 1E-8 );
	assert( Mem.info.data_blocks 	== frames_per_sec );

	vector_str_t arr = { "OSC","MAIN","Sinus","480","1000","40","2","1","1","69","2","0","-1","0","42" };
	line_interpreter( arr );
	assert( wp.frequency == 480 );
	assert( wp.ffreq > 0.0 );
	assert( abs( wp.ffreq - 480 ) < 1E-8 );
	wp.spectrum = spec_struct();
	OSC( 0 );
	cout << "Phase: " << phase <<  " " << 2*pi << endl;
	if ( abs( (sin(phase)) ) > 8E-5)
		assert(false);

	longnote = true;
	adsr.attack = 50;
	adsr.decay = 5;
	assert( decay_shift == frames_per_sec );
	Comment( TEST, "Osc test start");
}

