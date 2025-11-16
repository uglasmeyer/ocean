/*
 * time.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Time.h>

using namespace std::chrono ;

Time_class::Time_class( uint8_t* tel )
	: Logfacility_class("Time_class")
{
	className = Logfacility_class::className;
	this->time_elapsed = tel; // external time_elapsed
	Start();
	Stop(); // duration is zero
}

Time_class::Time_class()// @suppress("Class members should be properly initialized")
	: Logfacility_class("Time_class")
{
	Time_class { time_elapsed }; //local time_elapsed pointer
}


Time_class::~Time_class( )
{
	if ( this->time_elapsed )
		*this->time_elapsed = 0;
	DESTRUCTOR( className )
}
long int Time_class::Time_elapsed(  )
{
	Stop();
	long long int start_count = duration_cast<milliseconds>( start_time.time_since_epoch()).count();
	long long int stop_count  = duration_cast<milliseconds>( stop_time.time_since_epoch()).count();
	return stop_count - start_count;
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
uint Time_class::Performance( )
{
	Stop();
	uint
	tel 			= Time_elapsed();
	Start();

	// limit the CPU load by thread sleep
	uint
	min_wait 		= 10;
	if ( tel < min_wait )
		std::this_thread::sleep_for( std::chrono::milliseconds( min_wait - tel  ) );

	*time_elapsed 	= (uint8_t) (( tel * 100 ) / min_msec );// time elapsed in percentage w.r.t. 1 second = 1000 msec
	return (uint) tel;
}
void Time_class::TimeStamp()
{
 //   const auto tp_utc	{std::chrono::system_clock::now()};
    try
    {
        const std::chrono::zoned_time cur_time
        {
            std::chrono::current_zone(), // may throw
            std::chrono::system_clock::now()
        };
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
		case 's' : { std::this_thread::sleep_for( std::chrono::seconds(d) ); break; }
		case 'm' : { std::this_thread::sleep_for( std::chrono::milliseconds(d) ); break; }
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


