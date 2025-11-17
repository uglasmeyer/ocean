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
 * Record.h
 *
 *  Created on: Mar 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <data/Interface.h>
#include <data/Memory.h>
#include <Ocean.h>

class ProgressBar_class : virtual Logfacility_class
{
public:
	// static parameter
	uint8_t* RecCounter;
	ProgressBar_class(uint8_t* ); // addr of the counter to watch
	virtual ~ProgressBar_class();

	// dynamic parameter
	uint* counter;
	uint max_counter = 1;
	bool active = false;

	void Setup( uint8_t* sds );
	void SetValue( uint );
	void Set( uint* count, uint max );
	void Unset();
	void Reset();
	void Update( );
};

#endif /* PROGRESSBAR_H_ */
