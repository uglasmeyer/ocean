/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef TRACKCLASS_H_
#define TRACKCLASS_H_

#include <synthmem.h>
#include <Spectrum.h>
#include <Synthesizer.h>

const string 		NoteName[13] 		= 	{ "a","a#","b","c","c#", "d","d#", "e","f","#", "g","g#","A"};
typedef struct bps_struct
{
	const vector<string>	Bps_str_vec = { "0","1","2","4","5","8"}; 		// Beats per second
	const vector<int>   	Bps_array 	= { 0,1,2,4,5,8 };
	string getbps_str( int id )
	{
		return Bps_str_vec[ id ];
	}
	int getbps_id( string str )
	{
		for ( size_t i = 0; i < Bps_str_vec.size(); i++ )
			if ( Bps_str_vec[i].compare( str ) == 0 )
				return (int)i;
		return -1;
	}
	uint8_t getbps( int id )
	{
		return Bps_array[ id ];
	}

} bps_struct_t;


typedef struct freq_struct
{
	uint8_t 	oct		= 0;				// freq/55
	uint16_t	base 	= oct_base_freq; 	// oct * 55
	uint8_t 	note	= 0;				// ( freq - base ) % ( base / 12
	string		name 	= NoteName[0];
	float	 	pitch	= 0.0;				// freq - ( base + note)
	float		freq	= oct_base_freq;
	string 		str		= "";				// human readable freq_struct
} frequency_t;



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
		uint	 		glide_effect = 0;
		uint16_t 		msec		= max_sec*1000; 	// range 1 ... 8000
		uint16_t 		volume		= osc_default_volume; 	// range [0..100];
		buffer_t 		frames		= max_frames; 	// range 1 ... max_frames;
		frequency_t		fstruct;
		vector_str_t 	conf		= {};
		vector_str_t 	ops_str_arr = {};
		int 			ops_len  	= 0;
		bool 			touched 	= false;// true if set_frequency or set_volume
	} wave_t;

	adsr_t 		adsr 		= adsr_struct();
	wave_t 		wp 			= wave_struct();
	fmo_t 		fp 			= fmo_struct();
	vco_t 		vp 			= vco_struct();
	spectrum_t	spectrum	= spec_struct();

	Oscillator_base() : Logfacility_class("Osc"), Spectrum_base()
	{	};
	virtual ~Oscillator_base(){};

	frequency_t 	get_fstruct();
	frequency_t 	get_fstruct( int );
	void 			show_csv_comment( int );
	void 			set_frequency( float freq );
	void 			set_volume( uint16_t vol);
	void 			line_interpreter( vector_str_t arr );
	void 			Set_waveform( char  );
	void 			set_csv_comment ();
	void 			get_comment( bool  );
	void			Set_adsr( adsr_t );
	void			Set_pmw( uint8_t );
	void			Set_glide( uint8_t );
	void			Set_spectrum( spectrum_t );

private:

	string comment 		= "";
	string csv_comment 	= "";
	string command 		= "";

	frequency_t 	freq_to_freq_struct( float );

}; // close class Track class



#endif /* TRACKCLASS_H_ */
