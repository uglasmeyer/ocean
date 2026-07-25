/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

#include <Osctypes.h>
#include <data/Memory.h>
#include <Spectrum.h>


typedef struct bps_struct
{

	const set<int>   	Bps_set 	= { 0,1,2,4,5,8,10,16 };
	const vector<int>	Bps_vec		= set2vector( Bps_set );
	const int			len			= Bps_set.size();
	list<string>		Bps_lst		{};
	uset_t				Bps_str_set	{};
	vector<string>		Bps_str_vec {};
	string				Bps_str		{};
	range_T<int>		idx_range 	{ 0, len-1 };
	range_T<int>		set_range 	{ 0, 16 };
	bps_struct( )
	{
		for( int bps : Bps_vec )
		{
			string	str			= to_string( bps );
			Bps_str.append		( str + ", ");
			Bps_str_set.insert	( int2str( bps ) );
			Bps_lst.push_back	( str );
			Bps_str_vec.push_back( str );
		}
	}
	virtual ~bps_struct() = default;

	int bps_idx( int val )
	{
		int idx = 0;
		for( int bps : Bps_vec )
		{
			if( bps >= val )
				return idx;
			idx++;
		}
		return 0;
	}

	int check_bps( int val ) // map val to fit best into bps set
	{
		if( Bps_set.contains( val ) )
			return val;
		val = check_range( set_range, val );

		for( int bps : Bps_vec )
		{
			if( bps >= val )
				return bps;
		}
		return 0;
	}

}	bps_t;

typedef	struct wave_struct
{ // SDS related

	uint16_t	msec		= max_msec; 	// min_milli_sec or max_milli_sec
	buffer_t 	frames		= max_frames; 	// range 1 ... max_frames;
} wave_t;

typedef struct fmo_struct
{
	Heap_Memory*	Mem			= nullptr;
	OSCID_e			osc_id		= FMOID;
	uint16_t 		volume		= 0; // volume of the fm track
	string			name		= "FMO";
} fmo_t;  // all wave parameter for the fmo

typedef struct vco_struct
{
	Heap_Memory*	Mem			= nullptr;
	OSCID_e			osc_id		= VCOID;
	uint16_t 		volume		= 0; // volume of the vc track
	string			name		= "VCO";
} vco_t;  // all wave parameter for vco

typedef struct feature_struct
{ // SDS related. Is the same for all OSCs
	uint8_t frq_slide_value	= 0;		// slide duration of the main osc
	uint8_t	adjust			= 0; 		// used by vco and fmo, osc = 0
	uint8_t	PWM 			= 50;
	bool	longplay		= false;
	bool	combine			= false;	// follow main osc frequency index
} feature_t;

constexpr feature_t default_feature = feature_struct();

typedef struct Connect_struct
{ // SDS related
	OSCID_e		vol 	= NOOSCID;
	OSCID_e		frq 	= NOOSCID;
} connectId_t;
const connectId_t default_connectId = Connect_struct();

constexpr bool operator==( const connectId_t& lhs, const connectId_t& rhs )
{
	if((lhs.frq == rhs.frq) and ( lhs.vol == rhs.vol ))
		return true;
	else
		return false;
};
constexpr ostream& operator<<( ostream& os, const connectId_t& connect )
{
	os << "frq=" << (int)connect.frq << ", vol=" << (int)connect.vol;
	return os;
}

/**************************************************
 * Oscillator_base
 *************************************************/

class Oscillator_base
	: virtual public 	Spectrum_class
	, virtual public	osctype_struct
{
public:

	feature_t 		features 			= feature_struct();
	wave_t 			wp 					= wave_struct();
	fmo_t 			fp 					= fmo_struct();
	vco_t 			vp 					= vco_struct();
	Dynamic_class	DynFrequency		;
	spectrum_t		spectrum			;

					Oscillator_base		( OSCID_e osc_type, RoleId_e role );
	virtual 		~Oscillator_base	()
						{ DESTRUCTOR( className ); };

	uint8_t 		Set_frequency		( string frqName, DYNAMIC_e mode );
	uint8_t			Set_frequency		( int idx, DYNAMIC_e mode );
	void 			Set_spectrum_volume	( int vol);
	void 			Line_interpreter	( string_vec_t arr );
	void 			Set_waveform		( spec_arr_8t wf_vec   );
	void			Set_pmw				( uint8_t );
	void			Set_spectrum		( spectrum_t, DYNAMIC_e mode );
	void 			Set_frq_slidevalue	( uint8_t value );
	void 			Set_feature			( feature_t f );
	void 			Get_sound_stack		( Table_class* T );

protected:
	connectId_t		Connect				;
};


#endif /* OSCBASE_H_ */
