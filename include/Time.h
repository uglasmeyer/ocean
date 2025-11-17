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
