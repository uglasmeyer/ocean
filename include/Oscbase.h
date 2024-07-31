/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef TRACKCLASS_H_
#define TRACKCLASS_H_

#include <synthesizer.h>
#include <synthmem.h>
#include <Spectrum.h>

typedef struct adsr_struct
{
	uint bps_id 	= 1; // {0.1,2,3,4 }  => 0, 1, 1/2, 1/4, 1/8 sec., 0,1,2,4,8 beats per second
	uint attack 	= 80; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint decay  	= 0;
	uint hall		= 0; // mixing hall effect [0..100} data shift
} adsr_struc_t;

typedef struct freq_struct
{
	uint8_t 	oct		= 0;				// freq/55
	uint16_t	base 	= oct_base_freq; 	// oct * 55
	uint8_t 	note	= 1;				// ( freq - base ) % ( base / 12
	string		name 	= NoteName[0];
	uint8_t 	pitch	= 0;				// freq - ( base + note)
	uint16_t	freq	= oct_base_freq;
	string 		str		= "";				// human readable freq_struct
} freq_struc_t;



typedef struct fmo_struct
{
	Data_t*			data		= NULL; // ptr to the fm data
	uint16_t 		volume		= 1; // volume of the fm track
	uint16_t		frequency	= 1;
	string			name		= "NULL";
	bool			stored		= false;
	bool			generating 	= false;
} fmo_struc_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Data_t*			data		= NULL; // ptr to the fm data
	uint16_t 		volume		= 50; // volume of the fm track
	string			name		= "NULL";
	bool			stored		= false; // stored vco data
	bool 			generated	= false;
} vco_struc_t;  // all wave parameter for vco

#define OTHERID 0xFF
#define MAINID 	0
#define VCOID 	1
#define FMOID 	2
#define NOTESID 3
#define LFOID	4

class Oscillator_base : virtual public Logfacility_class, virtual public Spectrum_base
{
public:
	string 			osc_type 		= "NULL";
	uint8_t			ID 				= OTHERID;
	typedef	struct wave_struct
	{
		uint16_t 		frequency 	= oct_base_freq;	// base_freq + note pitch
		float 			ffreq		= (float) frequency;
		uint 			PMW_dial 	= 50;
		uint	 		glide_effect = 0;
		uint16_t 		msec		= max_sec*1000; 	// range 1 ... 8000
		uint16_t 		volume		= osc_default_volume; 	// range [0..100];
		buffer_t 		frames		= max_frames; 	// range 1 ... max_frames;
		freq_struc_t	fstruct;
		vector_str_t 	conf		= {};
		vector_str_t 	ops_str_arr = {};
		int 			ops_len  	= 0;

	} wave_struc_t;

	adsr_struc_t 	adsr 		= adsr_struct();
	wave_struc_t 	wp 			= wave_struct();
	fmo_struc_t 	fp 			= fmo_struct();
	vco_struc_t 	vp 			= vco_struct();

	Oscillator_base() : Logfacility_class("Osc"), Spectrum_base()
	{	};
	virtual ~Oscillator_base(){};

	freq_struc_t 	get_fstruct();
	freq_struc_t 	get_fstruct( int );
	void 			show_csv_comment( int );
	void 			set_frequency( int freq );
	int 			set_delta_frequency( int pitch );
	void 			set_volume( uint16_t vol);
	int	 			set_delta_volume( int pitch );
	void 			line_interpreter( vector_str_t arr );
	void 			Set_waveform( char  );
	void 			set_csv_comment ();
	void 			get_comment( bool  );

private:

	string comment 		= "";
	string csv_comment 	= "";
	string command 		= "";

	freq_struc_t 	freq_to_freq_struct( int );
	uint8_t 		osc_id(string);

}; // close class Track class



#endif /* TRACKCLASS_H_ */
