/*
 * Time.h
 *
 *  Created on: Sep 11, 2024
 *      Author: sirius
 */

#ifndef TIME_H_
#define TIME_H_

#include <Synthesizer.h>

class Time_class : virtual Logfacility_class
{
	// https://en.cppreference.com/w/cpp/chrono

public:

	std::chrono::time_point<std::chrono::steady_clock>
					start_time;
	std::chrono::time_point<std::chrono::steady_clock>
					stop_time;
	const long int 	wait 		= max_sec * 1000 ;
	long int 		duration, ms_wait;
	long int 		latency = 0;
	uint8_t*		time_elapsed = nullptr;

	Time_class( uint8_t* t );
	virtual ~Time_class();

	long int Time_elapsed();
	void Start();
	void Stop();
	void Block();
	void Wait( const uint&  );
	uint Performance( );
	void Test();
};




#endif /* TIME_H_ */
