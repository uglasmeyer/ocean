/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */



#include <Oscbase.h>




void Oscillator_base::show_csv_comment( int loglevel )
{
	string str;
	str = csv_comment +"\t" + wp.fstruct.str;
	Comment( loglevel, str) ;
}

freq_struc_t Oscillator_base::get_fstruct()
{
	return wp.fstruct ;
}

freq_struc_t Oscillator_base::get_fstruct( int freq )
{
	return freq_to_freq_struct( freq ) ;
}



void Oscillator_base::set_frequency( int freq )
{
	//  min_f := 1/max_sec ,  freq = min_f*N = N/max_sec

	if ( freq < 0 ) freq = 0;
	wp.frequency = freq;
	wp.ffreq 		= (float)wp.frequency;
	wp.fstruct		= freq_to_freq_struct( freq );
	set_csv_comment(  );
}
int Oscillator_base::set_delta_frequency( int pitch )
{
	int freq = wp.frequency;
	freq = freq  + pitch;
	if ( freq < 0 ) freq = 0;
	wp.frequency 	= freq;
	wp.fstruct		= freq_to_freq_struct( freq );
	set_csv_comment(  );
	return freq;
}
void Oscillator_base::set_volume( uint16_t vol)
{
	if ( vol < 1 ) vol = 0; // no output if below 2
	if ( vol > 100 ) vol = 100;
//		if ( vol > 80 ) vol = 80;
	wp.volume = vol;
	set_csv_comment(  );
}
int Oscillator_base::set_delta_volume( int pitch )
{
	int vol = wp.volume;
	vol = vol  + pitch;
	if ( vol < 1 ) vol = 1; // no output if below 2
	if ( vol > 100 ) vol = 100;
	wp.volume = vol;
	set_csv_comment(  );
	return vol;
}
uint8_t Oscillator_base::osc_id( string name )
{
	if ( name.compare("MAIN") == 0)
		return MAINID;
	if ( name.compare("VCO") == 0)
		return VCOID;
	if ( name.compare("FMO") == 0)
		return FMOID;
	Comment(ERROR,"Cannot convert track name " + name);
	exit(1);
//	return OTHERID;
}

void Oscillator_base::line_interpreter( vector_str_t arr )
{
	String 			Str{""};
	wp.conf 		= arr;

	osc_type		= arr[1];
	ID 				= osc_id( osc_type );
	vp.name			= osc_type;
	fp.name			= osc_type;
	wp.waveform_str	= arr[2];
	wp.msec 		= Str.secure_stoi(arr[4]);
	wp.volume 		= Str.secure_stoi(arr[5]);
	wp.frames 		= wp.msec*audio_frames/1000;
	int freq	 	= Str.secure_stoi(arr[3]);
	set_frequency( freq );
//	wp.fstruct		= freq_to_freq_struct( wp.frequency);
	command 		= "OSCd";
	int i;
	wp.ops_str_arr.clear();
	for( i=6; i<9; i++)
	{
		string str  = arr[i];
		wp.ops_str_arr.push_back(str) ;
	}
	wp.glide_effect 		= Str.secure_stoi( arr[13] );
	wp.PMW_dial 		= Str.secure_stoi( arr[14] );

	return ;
};




freq_struc_t Oscillator_base::freq_to_freq_struct( int freq )
{
	freq_struc_t fstruct;

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



void Oscillator_base::set_csv_comment ()
{
	list<string>::iterator itr;
	if ( osc_type.length() == 0 )
	{
		osc_type = "unknown";
		ID = OTHERID;
	}

	csv_comment = "";
	csv_comment.append(osc_type);
	csv_comment.append(",\t" + wp.waveform_str );
	csv_comment.append(",\t" + to_string( wp.frequency ));
	csv_comment.append(",\t" + to_string( wp.msec ) );
	csv_comment.append(",\t" + to_string( wp.volume ) );
	for ( string str : wp.ops_str_arr )
	{
		if ( str.length() > 0 )
			csv_comment.append("," + str ) ;

	}
}


void Oscillator_base::get_comment( bool variable )
{

// https://stackoverflow.com/questions/3222572/convert-a-single-character-to-a-string
	comment = wp.waveform_str;
	comment.append( "\t(" + to_string( wp.frequency ) + " Hz)");
	if ( variable )
	{
		comment.append(" frequency variable " );
	}
	else
	{
		comment.append( "Octave: ");
		comment.append( to_string( wp.fstruct.oct ) + " ");
		comment.append( " base " + to_string( wp.fstruct.base) + "Hz ");
		comment.append( "Note " + command.substr(3,1) + " \t");
	}
	comment.append( to_string( wp.msec ) + " msec ");
	comment.append( "Vol: " + to_string( wp.volume ) );
	return ;
}






