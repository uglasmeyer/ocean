//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Mixerbase.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef MIXERBASE_H_
#define MIXERBASE_H_

//#include <data/Memorybase.h>

enum STAID_e : unsigned char
{
	STA_USER00 = 0,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL,
	STA_SIZE,
	NO_STA
};

constexpr string StAIdName( STAID_e sta_id )
{
	switch ( sta_id )
	{
		case STA_USER00		: return "UserR+";
		case STA_USER01		: return "UserL-";
		case STA_USER02		: return "UserR-";
		case STA_USER03		: return "UserL+";
		case STA_INSTRUMENT	: return "Instrument";
		case STA_KEYBOARD	: return "Keyboard";
		case STA_NOTES		: return "Notes";
		case STA_EXTERNAL	: return "External";
		default 		: 	{
							cout << "WARN: unknown Storage Area id: " << ( int)sta_id << endl;
							return "";
							};
	}
	return "";
}
typedef array<STAID_e, STAID_e::STA_SIZE> 	StAarray_t; //SDS related

const 	StAarray_t 	StAMemIds =
{
	STA_USER00,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL
};
const range_T<STAID_e> staid_range{ (STAID_e)0, STA_SIZE };

struct mixer_state_struct
{	// SDS related
	bool 	sync			= true; // explicite sync mode
	bool 	notes			= false; // play notes
	bool 	external		= false; // external play or record
	bool	mute			= false; // mute master volume
	bool	kbd				= false; // play keyboard note
	bool	instrument		= true; // play instrument
} ;
typedef 	mixer_state_struct 	mixer_state_t;

typedef struct sta_rolemap_data
{
	OscroleId_t roleid;
	STAID_e staid;
} sta_rolemap_data_t;

struct sta_role_map
{
	const vector<sta_rolemap_data_t> sta_map_vec =
	{
		{ INSTRROLE		, STA_INSTRUMENT },
		{ NOTESROLE		, STA_NOTES },
		{ KBDROLE		, STA_KEYBOARD },
		{ EXTERNALROLE	, STA_EXTERNAL }
	};
	STAID_e GetStaid( OscroleId_t role )
	{
		if ( role < sta_map_vec.size() )
			return sta_map_vec[role].staid;
		else
			return NO_STA;

	}
};

#endif /* MIXERBASE_H_ */
