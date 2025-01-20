/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */



#include <Oscbase.h>



void Oscillator_base::Show_csv_comment( int loglevel )
{
	string str;
	str = csv_comment; //;+"\t" + wp.fstruct.str;
	Comment( loglevel, str) ;
}
void Oscillator_base::Gen_adsrdata( buffer_t beatframes )
{
	uint aframes 	= ( beatframes * adsr.attack ) / 100;
	float da		= 1.0 / aframes;
	float d_alpha 	= ( (100 - adsr.decay )/3.0) / beatframes;

	adsrdata.clear();
	for ( uint n = 0; n < beatframes ; n++ )
	{
		if ( n < aframes ) 	// attack
		{
			adsrdata.push_back( da * ( n + 1 ) );
		}
		else				// decay
		{
			float alpha 	= (n - aframes ) * d_alpha;
			adsrdata.push_back( exp( -alpha ) );
		}
	}
}
void Oscillator_base::Set_adsr( adsr_t _adsr )
{
	this->adsr = _adsr;
	Gen_adsrdata( frames_per_sec / adsr.bps );
}

void Oscillator_base::Set_duration( uint16_t msec )
{
	wp.msec 	= msec;
}

void Oscillator_base::Set_frequency( float freq )
{
	//  min_f := 1/max_sec ,  freq = min_f*N = N/max_sec

	if ( freq < 0 ) freq= 0;
	wp.frequency 		= freq;
	wp.start_frq 		= freq;
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
	Comment(INFO,
			"set waveform >" + Get_waveform_str(id) + "< for " + osc_type);

}

void Oscillator_base::Set_spectrum( spectrum_t spectrum )
{
	this->spectrum 	= spectrum;
}
#include <System.h>

void Oscillator_base::Line_interpreter( vector_str_t arr )
{
	String 			Str{""};

//	wp.conf 		= arr;


	vp.name			= osc_type;
	fp.name			= osc_type;
	spectrum.id		= Get_waveform_id( arr[2] );
	wp.msec 		= Str.secure_stoi(arr[4]);
	wp.volume 		= Str.secure_stoi(arr[5]);
	wp.frames 		= wp.msec*audio_frames/1000;
	float freq	 	= stof(arr[3]);
	Set_frequency( freq );
	command 		= osc_type;
	wp.glide_effect 	= Str.secure_stoi( arr[13] );
	wp.PMW_dial 		= Str.secure_stoi( arr[14] );

	return ;
};


/*
frequency_t Oscillator_base::freq_to_freq_struct( float freq )
{
	frequency_t fstruct;

	if ( freq < oct_base_freq )
	{
		fstruct = freq_struct();
		fstruct.pitch = freq;
	}
	else
	{
		int oct = -1;
		int f = freq;
		while (  f >= oct_base_freq )
		{
			f = (f >> 1);
			oct++;
		}
		fstruct.oct = oct;

		fstruct.base= ( oct_base_freq << fstruct.oct );
		fstruct.note= floor ( ( freq - fstruct.base ) * 12 / fstruct.base );
		fstruct.name=NoteName[ fstruct.note ];
		fstruct.pitch = freq - fstruct.base - floor( fstruct.note*fstruct.base/12);
		fstruct.freq = fstruct.base + fstruct.note * fstruct.base/12 + fstruct.pitch;
	}

	fstruct.str  =
			"f "	+ to_string( fstruct.freq ) +
			" (oct "	+ to_string( fstruct.oct )  +
			" base " + to_string( fstruct.base ) +
			" note " + to_string( fstruct.note ) +
						fstruct.name +
			" pitch "+ to_string( fstruct.pitch )+ ")" ;

	return fstruct;
}

*/

void Oscillator_base::Set_csv_comment ()
{
	if ( osc_type.length() == 0 )
	{
		osc_type = "unknown";
		osc_id = OTHERID;
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
