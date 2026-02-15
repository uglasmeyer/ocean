/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Osctypes.h
 *
 *  Created on: Feb 14, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCTYPES_H_
#define OSCTYPES_H_

#include <string>
#include <vector>
#include <array>

#define OSCIDSIZE 3

enum OSCID_e : unsigned char
{
	VCOID,
	FMOID,
	OSCID,
	NOOSCID
};
const  std::vector<OSCID_e> oscIds  =
{
	VCOID,
	FMOID,
	OSCID
};

enum RoleId_e : unsigned char
{
	USER00ROLE,
	USER01ROLE,
	USER02ROLE,
	USER03ROLE,
	INSTRROLE,
	KBDROLE,
	NOTESROLE,
	EXTERNALROLE,
	AUDIOROLE,
	ADSRROLE,
	ROLE_SIZE
};

struct osctype_struct
{
	OSCID_e				typeId		;
	RoleId_e			roleId		;
	std::string 		osctype_name;
	std::string			oscrole_name;
	bool				is_osc_type ;
	bool				is_fmo_type ;
	bool				is_vco_type ;
	bool				has_kbd_role   	;
	bool				has_instr_role 	;
	bool				has_notes_role 	;

	const std::vector<std::string>
						roleNames =
	{
		"UserR+",
		"UserL-",
		"UserR-",
		"UserL+",
		"Instrument",
		"Keyboard",
		"Notes",
		"External",
		"Audio",
		"ADSR",
		"no role"
	};
	const std::vector<std::string>
						typeNames
	{
		"VCO",
		"FMO",
		"OSC",
		""
	};
	osctype_struct( OSCID_e _type, RoleId_e _role )//= NOOSCID )
	{
		this->typeId 		= _type					;
		this->roleId		= _role					;
		this->osctype_name	= this->typeNames[_type];
		this->oscrole_name	= this->roleNames[_role];
		this->is_osc_type	= ( _type == OSCID )	;
		this->is_fmo_type 	= ( _type == FMOID )	;
		this->is_vco_type 	= ( _type == VCOID )	;
		this->has_kbd_role	= ( _role == KBDROLE )  ;
		this->has_instr_role= ( _role == INSTRROLE );
		this->has_notes_role= ( _role == NOTESROLE );
	}

	~osctype_struct() = default;

};

const std::vector<std::string> typeNames = osctype_struct(NOOSCID, ROLE_SIZE).typeNames;
const std::vector<std::string> roleNames = osctype_struct(NOOSCID, ROLE_SIZE).roleNames;

#endif /* OSCTYPES_H_ */
