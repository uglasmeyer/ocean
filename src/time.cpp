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
 * time.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Time.h>

using std::numeric_limits;
using std::chrono::current_zone;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::zoned_time;

Time_class::Time_class( uint8_t* tel )
	: Logfacility_class("Time_class")
{
	this->latency 		= 0;
	this->threshold		= numeric_limits<int>::max();
	this->time_elapsed	= tel; // external time_elapsed
	this->overload		= false;
	this->overload_p	= &overload;
	State				( ENABLED );
	Start				();
	Stop				(); // duration is zero
}

Time_class::Time_class()// @suppress("Class members should be properly initialized")
	: Logfacility_class("Time_class")
{
	this->tel			= 0;
	this->time_elapsed	= &tel; // local time_elapsed
	this->threshold		= numeric_limits<int>::max();
	this->overload		= false;
	this->overload_p	= &overload;//
	State				( ENABLED );
}


Time_class::~Time_class( )
{
	DESTRUCTOR( className )
}

void Time_class::Set_Threshold( int msec, bool* _overload )
{
	// set a treshold for time_elapsed in msec
	threshold	= msec;
	overload_p	= _overload;
}
long int Time_class::Time_elapsed()
{
	auto ms_count = [  ]( auto rep )
	{
		return duration_cast<milliseconds>( rep );
	};
	typedef long long int count_t;

	Stop();
	count_t start_count	= ms_count( start_time.time_since_epoch()).count();
	count_t stop_count 	= ms_count( stop_time.time_since_epoch()).count();
	count_t tel			= stop_count - start_count;
	*overload_p					= ( tel > threshold );
	return tel;
}
void Time_class::Start()
{
	start_time = steady_clock::now();
};

void Time_class::Stop()
{
	stop_time = steady_clock::now();
}

void Time_class::Block()
{
	long int duration	= Time_elapsed();
	long int ms_wait	= 0;

	latency = duration*100/wait;
	if ( latency > 100 )
		Comment( WARN, "runtime latency exceeds 100% " + to_string( latency ));


	if ( wait > duration )
		ms_wait = wait - duration ;
	usleep( ms_wait * 1000 ); //milli seconds

}
uint Time_class::Performance( bool* overload )
{
	//  calculate the time for one cycle
	if( state == StateId_e::DISABLED )
		return 0;
	Stop();
	uint	tel 			= Time_elapsed();
	Start();

	// limit the CPU load by thread sleep
	uint	min_wait 		= 10; // msec to wait at least
	if ( tel < min_wait )
		sleep_for( milliseconds( min_wait - tel  ) );
	uint8_t	tel_percent		= (uint8_t) (( tel * 100 ) / min_msec );
	*time_elapsed 	= tel_percent;// time elapsed in percentage w.r.t. 1 second = 1000 msec
	if ( tel_percent > 90 )
		*overload	= true;
	return tel;
}

void Time_class::State( StateId_e _state )
{
	state = _state;
}

void Time_class::TimeStamp()
{
    try
    {
        const zoned_time cur_time = zoned_time( current_zone(), system_clock::now() );
        Info( cur_time );
    }
    catch(const std::runtime_error& ex)
    {
        Comment( ERROR, ex.what() );
    }
}
void Time_class::Wait( const uint& d, const string& dur )
{
	switch ( dur[0] )
	{
		case 's' : { sleep_for( seconds(d) ); break; }
		case 'm' : { sleep_for( milliseconds(d) ); break; }
		default : { Comment( WARN, "unrecognized unit ", dur );break; }
	} // switch  dur[0]
}

void Time_class::Test()
{
	TEST_START( "Time_class" );
  	Comment( TEST, "wait for 2 seconds ");
	Start();
  	Wait( 1000, "msec" );
	Comment( TEST, "elapsed time " + to_string( Time_elapsed()) + "ms");
	TEST_END( "Time_class" );
}


