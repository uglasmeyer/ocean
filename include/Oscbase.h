/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef TRACKCLASS_H_
#define TRACKCLASS_H_

#include <Spectrum.h>
#include <Ocean.h>
#include <Synthmem.h>
#include <String.h>

const vector<string> NoteName =
	{ "a","a#","b","c","c#", "d","d#", "e","f","f#", "g","g#","A"};

typedef struct bps_struct
{
	const vector<uint>   	Bps_array 	= { 0,1,2,4,5,8 };
	vector<string>			Bps_str_vec = {};
	std::unordered_map< string, int > Bps_map;

	bps_struct( )
	{
		uint index = 0;
		for( uint bps : Bps_array )
		{
			string str = to_string( bps );
			Bps_map[ str ] = index ;
			Bps_str_vec.push_back( str );
			index++;
		}
	}

	string getbps_str( uint id )
	{
		if (( id < 0 ) or ( id  > Bps_array.size()-1 ))
			return "0";
		else
			return to_string( Bps_array[ id ]);
	}
	int getbps_id( string str )
	{
		return Bps_map[str] ;
	}
	uint8_t getbps( int id )
	{
		return Bps_array[ id ];
	}

} bps_struct_t;

typedef struct adsr_struct
{
	uint8_t bps_id 	= 1; // {0.1,2,3,4 }  => 0, 1, 1/2, 1/4, 1/8 sec., 0,1,2,4,8 beats per second
	uint8_t attack 	= 80; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t decay  	= 0;
	uint8_t hall		= 0; // mixing hall effect [0..100} data shift
} adsr_t;

typedef struct fmo_struct
{
	Data_t*			data		= NULL; // ptr to the fm data
	uint16_t 		volume		= 1; // volume of the fm track
	string			name		= "NULL";
	bool			stored		= false;
	bool			generating 	= false;
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Data_t*			data		= NULL; // ptr to the fm data
	uint16_t 		volume		= 100; // volume of the fm track
	string			name		= "NULL";
	bool			stored		= false; // stored vco data
	bool 			generated	= false;
} vco_t;  // all wave parameter for vco


class Oscillator_base : virtual public Logfacility_class, virtual public Spectrum_base
{
public:
	uint8_t			osc_id 			= OTHERID;
	string 			osc_type 		= osc_type_vec[ osc_id ] ;

	typedef	struct wave_struct
	{
		float	 		frequency 	= oct_base_freq;	// base_freq + note pitch
		uint 			PMW_dial 	= 50;
		uint	 		glide_effect= 0;
		uint16_t 		msec		= max_sec*1000; 	// range 1 ... 8000
		uint16_t 		volume		= osc_default_volume; 	// range [0..100];
		buffer_t 		frames		= max_frames; 	// range 1 ... max_frames;
		vector_str_t 	conf		= {};
		bool 			touched 	= false;// true if set_frequency or set_volume
	} wave_t;

	adsr_t 				adsr 		= adsr_struct();
	wave_t 				wp 			= wave_struct();
	fmo_t 				fp 			= fmo_struct();
	vco_t 				vp 			= vco_struct();
	spectrum_t			spectrum	= spec_struct();

	Oscillator_base() : Logfacility_class("Osc"), Spectrum_base()
	{	};
	virtual 		~Oscillator_base(){};

	void 			Show_csv_comment( int );
	void 			Set_frequency( float freq );
	void 			Set_volume( uint16_t vol);
	void 			Line_interpreter( vector_str_t arr );
	void 			Set_waveform( char  );
	void 			Set_csv_comment ();
	void 			Get_comment( bool  );
	void			Set_adsr( adsr_t );
	void			Set_pmw( uint8_t );
	void			Set_glide( uint8_t );
	void			Set_spectrum( spectrum_t );

private:

	string comment 		= "";
	string csv_comment 	= "";
	string command 		= "";


}; // close class Track class



#endif /* TRACKCLASS_H_ */
