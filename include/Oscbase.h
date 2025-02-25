/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef TRACKCLASS_H_
#define TRACKCLASS_H_

#include <data/Memory.h>
#include <Spectrum.h>
#include <Ocean.h>
#include <String.h>
#include <Frequency.h>

typedef struct bps_struct
{
	const set<uint8_t>   	Bps_set = { 0,1,2,4,5,8 };
	const vector<uint8_t>	Bps_vec	= set2vector( Bps_set );
	list<string>			Bps_lst	= {};
	set<string>			Bps_str_set	= {};

	bps_struct( )
	{
		uint8_t index = 0;
		for( uint8_t bps : Bps_vec )
		{
			string str = to_string( bps );
			Bps_str_set.insert( string(1,int2char(bps)) );
			Bps_lst.push_back( str );
			index++;
		}
	}

} bps_struct_t;




typedef	struct wave_struct
{
	float	 		frequency 	= oct_base_freq;	// base_freq + note pitch
	uint8_t			frqidx		= 0;
	float			start_frq 	= frequency;
	uint8_t			PMW_dial 	= 50;
	uint8_t 		glide_effect= 0;
	uint16_t 		msec		= max_milli_sec; 	// range 1 ... 8000
	uint8_t 		volume		= osc_default_volume; 	// range [0..100];
	uint8_t			adjust		= 0;
	buffer_t 		frames		= max_frames; 	// range 1 ... max_frames;
} wave_t;

typedef struct adsr_struct
{
	uint8_t  bps 	= 0; // { 0, 1/2, 1, 2, 3, 4 }  => 2, 0, 1, 1/2, 1/4, 1/5, 1/8 sec.,
	uint8_t attack 	= 0; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t decay  	= 100;
	uint8_t hall	= 0; // hall effect [0..100} data shift
} adsr_t;

typedef struct fmo_struct
{
	Data_t*			data		= nullptr; // ptr to the fm data
	char 			osc_id		= -1;
	uint16_t 		volume		= 0; // volume of the fm track
	string			name		= "";
	bool			stored		= false;
	bool			generating 	= false;
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Data_t*			data		= nullptr; // ptr to the vc data
	char 			osc_id		= -1;
	uint16_t 		volume		= 0; // volume of the vc track
	string			name		= "";
	bool			stored		= false; // stored vco data
	bool 			generated	= false;
} vco_t;  // all wave parameter for vco


class Oscillator_base :
	virtual public Logfacility_class,
	virtual public Spectrum_class
{

public:

	char			osctype_id	= -1;//osc_struct::OSCID;
	char			oscrole_id	= -1;//osc_struct::INSTRID;
	string 			osc_role 	= "";
	string 			osc_type 	= "";

	bool			is_main_osc = false;
	bool			is_notes_role = false;


	DataVec_t		adsrdata 	{ };

	adsr_t 			adsr 		= adsr_struct();
	wave_t 			wp 			= wave_struct();
	fmo_t 			fp 			= fmo_struct();
	vco_t 			vp 			= vco_struct();
	spectrum_t		spectrum	= spec_struct();

	Oscillator_base() :
		Logfacility_class("Oscillator_base"),
		Spectrum_class()
	{};

	virtual 	~Oscillator_base(){};

	void 		Show_csv_comment( int );
	void 		Set_frequency( int idx );
	void 		Set_volume( uint16_t vol);
	void 		Line_interpreter( vector_str_t arr );
	void 		Set_waveform( spec_arr_8t wf_vec   );
	void 		Set_csv_comment ();
	void 		Get_comment( bool  );
	void		Set_adsr( adsr_t );
	void		Set_pmw( uint8_t );
	void		Set_glide( uint8_t );
	void		Set_spectrum( spectrum_t );
	void		Set_duration( uint16_t );
	void		Gen_adsrdata( buffer_t );
	stringstream Get_sound_stack();

private:
	string 		comment 		= "";
	string 		csv_comment 	= "";
	string 		command 		= "";

}; // close class Track class



#endif /* TRACKCLASS_H_ */
