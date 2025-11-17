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
 * Statistic.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_STATISTIC_H_
#define DATA_STATISTIC_H_
#include <Ocean.h>
#include <Logfacility.h>

/*
 *
 */
typedef struct statistic_struct
{
	long 	shm 	= 0;
	long 	stereo	= 0;
	long 	data 	= 0;
	long 	total	= 0;
} statistic_t;
extern statistic_t statistic;


class Statistic_class :
	public virtual Logfacility_class
{
	string className = "";

public:

	string module = "";
	void Show_Statistic( );

	Statistic_class( string module );
	virtual ~Statistic_class();
};

#endif /* DATA_STATISTIC_H_ */
