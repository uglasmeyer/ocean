
#include <Time.h>
#include <Ocean.h>

using namespace std::chrono ;

Time_class::Time_class( uint8_t* t )
: Logfacility_class("Timer")
{
	this->time_elapsed = t;
	Start();
	Stop(); // duration is zero
}

Time_class::Time_class(  )
: Logfacility_class("Timer")
{
	this->time_elapsed 	= 0;
	this->duration 		= 0;
	this->ms_wait		= 0;
}

Time_class::~Time_class( )
{
	if ( this->time_elapsed )
		*this->time_elapsed = 0;
}
long int Time_class::Time_elapsed()
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
	duration = Time_elapsed();
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
	uint tel = Time_elapsed();
	Start();

	// limit the CPU load by thread sleep
	if ( tel < 100 )
		std::this_thread::sleep_for( chrono::milliseconds( 100 - tel  ) );

	*time_elapsed 	= (uint8_t) tel / 10;// time elapsed in percentage w.r.t. 1 second = 1000 msec
	return (uint) tel;
}

void Time_class::Wait( const uint& d )
{
    std::this_thread::sleep_for( chrono::seconds(d) );
}

void Time_class::Test()
{
	TEST_START( "Time_class" );
  	Comment( TEST, "wait for 2 seconds ");
	Start();
  	Wait( 2 );
	Comment( TEST, "elapsed time " + to_string( Time_elapsed()) + "ms");
	TEST_END( "Time_class" );
}


