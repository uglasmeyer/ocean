/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */



#include <Oscbase.h>




void Oscillator_base::Gen_adsrdata( buffer_t beatframes )
{
	/* maxima

attack  : 10;
decay	: 95;
beatframes : 24000;
aframes : beatframes * attack / 100;
d_delta	: ( (100 - decay )/3.0) / beatframes;
y0		: exp( - (beatframes-aframes) * d_delta );
d_alpha	: (1-y0)/aframes;
fattack : y0 + n*d_alpha;
fdecay	: exp( - ( n - aframes ) * d_delta );
plot2d([fattack, fdecay], [n,0,beatframes]);

	 */
	uint 	aframes 	= ( beatframes * adsr.attack ) / 100;
	if ( aframes == 0 )
		aframes = 1;
	const float d_delta 	= ( (100 - adsr.decay )/3.0) / beatframes;
	float y0 = 1.0;
	float delta = (beatframes - aframes) * d_delta;
	if ( d_delta > 0 )
		y0	= expf( - delta );
	const float d_alpha		= (1.0 - y0) / aframes;

	adsrdata.clear();
	for ( uint n = 0; n < beatframes ; n++ )
	{
		if ( n < aframes ) 	// attack
		{
			adsrdata.push_back( y0 + n*d_alpha );
		}
		else				// decay
		{
			float delta 	= (n - aframes ) * d_delta;
			adsrdata.push_back( exp( -delta ) );
		}
	}
}

void Oscillator_base::Set_adsr( adsr_t _adsr )
{
	this->adsr = _adsr;
	if (( oscrole_id == osc_struct::KBDID ) or ( oscrole_id == osc_struct::NOTESID ))
		this->adsr.bps = 1;
	if ( adsr.bps != 0 )
	{
		Gen_adsrdata( frames_per_sec / adsr.bps );
	}
}

void Oscillator_base::Set_duration( uint16_t msec )
{
	wp.msec 	= msec;
}

void Oscillator_base::Set_frequency( int idx )
{
	wp.frqidx			= idx;
	wp.frequency 		= frequency.Calc( idx );
	wp.start_frq 		= wp.frequency;
	spectrum.base		= wp.frequency;
}
void Oscillator_base::Set_volume( uint16_t vol)
{
	if ( vol < 1 ) vol = 0; // no output if below 2
	if ( vol > 100 ) vol = 100;
	wp.volume 		= vol;
}
void Oscillator_base::Set_pmw( uint8_t pmw )
{
	wp.PMW_dial = pmw;
}

void Oscillator_base::Set_glide( uint8_t value )
{
	wp.glide_effect = value;
}

void Oscillator_base::Set_waveform( char id )
{
	spectrum.id	= id;
	Info( "set waveform >" + Get_waveform_str(id) + "< for " + osc_type);

}

void Oscillator_base::Set_spectrum( spectrum_t spectrum )
{
	this->spectrum 	= spectrum;
}

void Oscillator_base::Line_interpreter( vector_str_t arr )
{
	String 			Str{""};

	command 		= osc_type;
	vp.name			= osc_type;
	fp.name			= osc_type;
	spectrum.id		= Get_waveform_id( arr[2] );
	int frqidx	 	= Str.secure_stoi( arr[3] );
	Set_frequency( frqidx );

	wp.msec 		= Str.secure_stoi(arr[4]);
	wp.volume 		= Str.secure_stoi(arr[5]);
	wp.frames 		= wp.msec*audio_frames/1000;
	wp.glide_effect = Str.secure_stoi( arr[13] );
	wp.PMW_dial 	= Str.secure_stoi( arr[14] );

	return ;
};

void Oscillator_base::Show_csv_comment( int loglevel )
{
	string str;
	str = csv_comment; //;+"\t" + wp.fstruct.str;
	Comment( loglevel, str) ;
}

void Oscillator_base::Set_csv_comment ()
{
	if ( osc_type.length() == 0 )
	{
		osc_type = "unknown";
		osctype_id = -1;
	}

	csv_comment = "";
	csv_comment.append(osc_type);
	csv_comment.append(",\t" + waveform_str_vec[spectrum.id] );
	csv_comment.append(",\t" + to_string( wp.frequency ));
	csv_comment.append(",\t" + to_string( wp.msec ) );
	csv_comment.append(",\t" + to_string( wp.volume ) );
}


void Oscillator_base::Get_comment( bool variable )
{
	comment = Get_waveform_str( spectrum.id );
	comment.append( "\t(" + to_string( wp.frequency ) + " Hz)");
	comment.append( to_string( wp.msec ) + " msec ");
	comment.append( "Vol: " + to_string( wp.volume ) );
	return ;
}

stringstream Oscillator_base::Get_sound_stack()
{
	const string star 	= "*";
	const string nostar = ".";

	string active  	= nostar;
	string fp_flag  = nostar;
	string vp_flag	= nostar;
	string fp_gen	= nostar;
	string vp_gen	= nostar;
	if ( vp.stored ) 		vp_flag = star;
	if ( fp.stored ) 		fp_flag = star;
	if ( vp.generated ) 	vp_gen = star;
	if ( fp.generating ) 	fp_gen = star;

	stringstream strs{""};
	strs 	<< active
			<< osc_type +"\t"
			<< Get_waveform_str( spectrum.id ) +"\t\t"
			<< fixed << setprecision(2) << wp.frequency << "\t"
			<< (int) wp.volume << "\t"
			<< (int) wp.msec << "\t"

			<< vp_flag
			<< vp_gen
			<< vp.name +"\t"

			<< fp_flag
			<< fp_gen
			<< fp.name
			;

	return strs;

}
