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

template< typename T>
T step( T src, T dst, range_T<int> range)
{
	int diff = dst - src;
	if (diff == 0 )
		return src; // src=dst, nothing to do
	int delta = diff/abs(diff);
	T value	= src + delta;
	if (( value < range.min) or (value > range.max ))
		return dst; // out of bounds
	return value;
};

typedef struct bps_struct
{
	const set<uint8_t>   	Bps_set = { 0,1,2,4,5,8,10 };
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
	uint8_t		frqidx		= A3;
	frq_t		freq		= 220;
	uint8_t 	volume		= osc_default_volume; 	// range [0..100];
	uint8_t		PMW_dial 	= 50;
	uint8_t 	glide_effect= 50;
	uint16_t	msec		= max_msec; 	// min_milli_sec or max_milli_sec
	uint8_t		adjust		= 0; // used by vco and fmo, osc = 0
	buffer_t 	frames		= max_frames; 	// range 1 ... max_frames;
} wave_t;

typedef struct adsr_struct
{
	uint8_t bps 	= 0; // { 0, 1/2, 1, 2, 3, 4 }  => 2, 0, 1, 1/2, 1/4, 1/5, 1/8 sec.,
	uint8_t attack 	= 0; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t decay  	= 100;
	uint8_t hall	= 0; // hall effect [0..100} data shift
} adsr_t;

typedef struct fmo_struct
{
//	Data_t*			data		= nullptr; // ptr to the fm data
	Memory*			Mem			= nullptr;
	char 			osc_id		= -1;
	uint16_t 		volume		= 0; // volume of the fm track
	string			name		= "";
	bool			stored		= false;
	bool			generating 	= false;
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
//	Data_t*			data		= nullptr; // ptr to the vc data
	Memory*			Mem			= nullptr;
	char 			osc_id		= -1;
	uint16_t 		volume		= 0; // volume of the vc track
	string			name		= "";
	bool			stored		= false; // stored vco data
	bool 			generated	= false;
} vco_t;  // all wave parameter for vco

constexpr buffer_t constbeatframes ()
{
	return max_frames;
}
const buffer_t 		max_beatframes 		= constbeatframes();

class Oscillator_base :
	virtual public Logfacility_class,
	virtual public Spectrum_class
{

public:

	char			typeId			= -1;//osc_struct::OSCID;
	string 			osctype_name 		= "";

	char			roleId		= -1;//osc_struct::INSTRID;
	string 			oscrole_name 	= "";

	bool			is_osc_type 	= false;
	bool			is_fmo_type 	= false;
	bool			is_vco_type 	= false;
	bool			has_kbd_role   	= false;
	bool			has_notes_role 	= false;
	bool			has_instr_role 	= false;

	buffer_t		beat_frames		= max_frames;
	array<Data_t, max_beatframes>
					adsrdata 		{ };
	struct connect_struct
	{
		bool 	vol = false;
		bool	frq = false;
	};
	typedef connect_struct connect_t;
	connect_t 		connect		= connect_struct();

	adsr_t 			adsr 		= adsr_struct();
	wave_t 			wp 			= wave_struct();
	fmo_t 			fp 			= fmo_struct();
	vco_t 			vp 			= vco_struct();
	spectrum_t		spectrum	= spec_struct();

	Dynamic_class	DynFrequency{ frqarr_range };

	Oscillator_base() :
		Logfacility_class("Oscillator_base"),
		Spectrum_class()
	{
	};

	virtual 	~Oscillator_base() = default;

	buffer_t 	hall_cursor = 0;
	buffer_t 	beat_cursor = 0;
	bool		kbd_trigger = false;

	uint8_t 	Set_frequency( string frqName, uint mode );
	uint8_t		Set_frequency( int idx, uint mode );
	void 		Set_volume( int vol, uint mode);
	void 		Line_interpreter( vector_str_t arr );
	void 		Set_waveform( spec_arr_8t wf_vec   );
	void		Set_pmw( uint8_t );
	void		Set_glide( uint8_t );
	void		Set_spectrum( spectrum_t );
	void		Setwp_frames( uint16_t );

	void		Set_adsr( adsr_t );
	void		Gen_adsrdata( buffer_t );
	void	 	set_hallcursor( buffer_t cursor = 0);
	void 		Get_sound_stack( Table_class* T );

private:
//	string 		comment 		= "";
//	string 		command 		= "";

}; // close class Track class



#endif /* TRACKCLASS_H_ */
