/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Trackclass.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCBASE_H_
#define OSCBASE_H_

#include <data/Memory.h>
#include <Spectrum.h>
#include <String.h>
#include <Frequency.h>

typedef struct bps_struct
{

	const set<uint8_t>   	Bps_set = { 0,1,2,4,5,8,10,16 };
	const vector<uint8_t>	Bps_vec	= set2vector( Bps_set );
	list<string>			Bps_lst	= {};
	set<string>			Bps_str_set	= {};
	const int			len		= Bps_set.size();
	range_T<int>			bps_range { 0, len-1 };
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

	uint16_t	msec		= max_msec; 	// min_milli_sec or max_milli_sec
	buffer_t 	frames		= max_frames; 	// range 1 ... max_frames;
} wave_t;

typedef struct fmo_struct
{
	Heap_Memory*	Mem			= nullptr;
	char 			osc_id		= DEFAULT_ID;
	uint16_t 		volume		= 0; // volume of the fm track
	string			name		= "";
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Heap_Memory*	Mem			= nullptr;
	char 			osc_id		= DEFAULT_ID;
	uint16_t 		volume		= 0; // volume of the vc track
	string			name		= "";
} vco_t;  // all wave parameter for vco

typedef struct feature_struct
{ // SDS related. Is the same for all OSCs
	uint8_t slide_frq= 0;
	uint8_t	adjust		= 0; // used by vco and fmo, osc = 0
	uint8_t	PWM 		= 50;
	bool	longplay	= false;
} feature_t;

constexpr feature_t default_feature = feature_struct();

typedef struct Connect_struct
{ // SDS related
	OSCID_e		vol 	= NOOSCID;
	OSCID_e		frq 	= NOOSCID;
} connectId_t;

constexpr bool operator==( const connectId_t& lhs, const connectId_t& rhs )
{
	if((lhs.frq == rhs.frq) and ( lhs.vol == rhs.vol ))
		return true;
	else
		return false;
};
constexpr ostream& operator<<( ostream& os, const connectId_t& connect )
{
	os << "frq=" << connect.frq << ", vol=" << connect.vol;
	return os;
}

/**************************************************
 * Oscillator_base
 *************************************************/

class Oscillator_base :
	virtual public 	Spectrum_class
{
public:
	OSCID_e			typeId			;
	string 			osctype_name 	= "";

	bool			is_osc_type 	= false;
	bool			is_fmo_type 	= false;
	bool			is_vco_type 	= false;
	bool			has_kbd_role   	= false;
	bool			has_notes_role 	= false;
	bool			has_instr_role 	= false;

	connectId_t		Connect			;
	feature_t 		features 		= feature_struct();
	wave_t 			wp 				= wave_struct();
	fmo_t 			fp 				= fmo_struct();
	vco_t 			vp 				= vco_struct();
	spectrum_t		spectrum		;
	const spec_arr_dt
					default_phase	= { 0.0, 0.0, 0.0, 0.0, 0.0};
	spec_arr_dt		phase 		;
	Dynamic_class	DynFrequency;

					Oscillator_base	( OSCID_e osc_type );
	virtual 		~Oscillator_base()
						{ DESTRUCTOR( className ); };

	uint8_t 		Set_frequency	( string frqName, DYNAMIC mode );
	uint8_t			Set_frequency	( int idx, DYNAMIC mode );
	void 			Set_spectrum_volume( int vol);
	void 			Line_interpreter( vector_str_t arr );
	void 			Set_waveform	( spec_arr_8t wf_vec   );
	void			Set_pmw			( uint8_t );
	void			Set_spectrum	( spectrum_t );
	void 			Set_slideFrq	( uint8_t value );
	void 			Get_sound_stack	( Table_class* T );

}; // close class Oscillator_base


#endif /* OSCBASE_H_ */
