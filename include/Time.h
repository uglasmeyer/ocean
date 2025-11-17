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
 * Time.h
 *
 *  Created on: Sep 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef TIME_H_
#define TIME_H_

#include <Logfacility.h>
#include <chrono>

class Time_class :
	virtual public 			Logfacility_class
{
	// https://en.cppreference.com/w/cpp/chrono
	string					className		{};
	std::chrono::time_point<std::chrono::steady_clock>
							start_time;
	std::chrono::time_point<std::chrono::steady_clock>
							stop_time;
	const long int 			wait 			= max_sec * 1000 ;
	long int 				latency 		= 0;
	uint8_t					tel				= 0;
	uint8_t*				time_elapsed	= &tel;

public:

							Time_class		( uint8_t* tel );
							Time_class		();
	virtual 				~Time_class		();

	long int 				Time_elapsed	();
	void 					Start			();
	void 					Stop			();
	void 					Block			();
	void 					Wait			( const uint&, const string& dur = "sec"  );
	uint 					Performance		();
	void 					TimeStamp		();
	void 					Test			();
};

#endif /* TIME_H_ */
