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
 * Mixerbase.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef MIXERBASE_H_
#define MIXERBASE_H_

//#include <data/Memorybase.h>

const array<std::string, STA_SIZE> StANames =
{
	"UserR+",
	"UserL-",
	"UserR-",
	"UserL+",
	"Instrument",
	"Keyboard",
	"Notes",
	"External"
};

constexpr string StAIdName( StAId_e staid )
{
	if( staid < StANames.size() )
		return StANames[ staid ];
	else
	{
		cout << "WARN: unknown Storage Area id: " << ( int)staid << endl;
		return "";
	}
}
typedef array<StAId_e, STA_SIZE> 	StAarray_t; //SDS related

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
const range_T<StAId_e> staid_range{ (StAId_e)0, STA_SIZE };

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
	RoleId_e roleid;
	StAId_e staid;
} sta_rolemap_data_t;

struct sta_role_map
{
	const vector<sta_rolemap_data_t> sta_map_vec =
	{
		{ INSTRROLE		, STA_INSTRUMENT },
		{ NOTESROLE		, STA_NOTES },
		{ KBDROLE		, STA_KEYBOARD },
		{ EXTERNALROLE	, STA_EXTERNAL },
		{ USER00ROLE	, STA_USER00 },
		{ USER01ROLE	, STA_USER01 },
		{ USER02ROLE	, STA_USER02 },
		{ USER03ROLE	, STA_USER03 }
	};
	StAId_e GetStaid( RoleId_e role )
	{
		if ( role < sta_map_vec.size() )
			return sta_map_vec[role].staid;
		else
			return STA_SIZE;
	}
	RoleId_e GetRoleid( StAId_e staid )
	{
		for( sta_rolemap_data_t roleid_map : sta_map_vec )
		{
			if ( staid == roleid_map.staid )
				return roleid_map.roleid;
		}
		return ROLE_SIZE;
	}
};


#endif /* MIXERBASE_H_ */
