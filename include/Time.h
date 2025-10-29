/*
 * Time.h
 *
 *  Created on: Sep 11, 2024
 *      Author: sirius
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
