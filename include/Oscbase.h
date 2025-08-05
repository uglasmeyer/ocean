/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef OSCBASE_H_
#define OSCBASE_H_

#include <data/Memory.h>
#include <Spectrum.h>
#include <Ocean.h>
#include <String.h>
#include <Frequency.h>

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
			string str 		= to_string( bps );
			Bps_str_set.insert( string( 1, int2char(bps)) );
			Bps_lst.push_back( str );
			index++;
		}
	}
	virtual ~bps_struct() = default;

} bps_struct_t;

typedef	struct wave_struct
{ // SDS related

	uint8_t		frqidx		= C4;
	frq_t		freq		= 220;
	uint8_t 	volume		= osc_default_volume; 	// range [0..100];
	uint8_t		PMW_dial 	= 50;
	uint16_t	msec		= max_msec; 	// min_milli_sec or max_milli_sec
	uint8_t		adjust		= 0; // used by vco and fmo, osc = 0
	buffer_t 	frames		= max_frames; 	// range 1 ... max_frames;
} wave_t;

typedef struct fmo_struct
{
	Memory*			Mem			= nullptr;
	char 			osc_id		= DEFAULT_ID;
	uint16_t 		volume		= 0; // volume of the fm track
	string			name		= "";
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Memory*			Mem			= nullptr;
	char 			osc_id		= DEFAULT_ID;
	uint16_t 		volume		= 0; // volume of the vc track
	string			name		= "";
} vco_t;  // all wave parameter for vco

typedef struct feature_struct
{ // SDS related. Is the same for all OSCs
	uint8_t glide_effect= 0;
} feature_t;

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


	struct connect_struct
	{ // SDS related
		bool 	vol = false;
		bool	frq = false;
	};
	typedef connect_struct connect_t;
	connect_t 		connect		= connect_struct();

	feature_t 		feature 	= feature_struct();
	wave_t 			wp 			= wave_struct();
	fmo_t 			fp 			= fmo_struct();
	vco_t 			vp 			= vco_struct();
	spectrum_t		spectrum	= spec_struct();

	Dynamic_class	DynFrequency{ frqarr_range };

					Oscillator_base() ;

	virtual 		~Oscillator_base() = default;

	bool			kbd_trigger = false;

	uint8_t 		Set_frequency( string frqName, uint mode );
	uint8_t			Set_frequency( int idx, uint mode );
	void 			Set_volume( int vol, uint mode);
	void 			Line_interpreter( vector_str_t arr );
	void 			Set_waveform( spec_arr_8t wf_vec   );
	void			Set_pmw( uint8_t );
	void			Set_spectrum( spectrum_t );
	void 			Set_glide( uint value );
	void			Setwp_frames( uint16_t );
	void			Setwp( wave_t wp );
	void 			Get_sound_stack( Table_class* T );

private:


}; // close class Oscillator_base


#endif /* OSCBASE_H_ */
