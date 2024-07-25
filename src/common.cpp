/*
 * common.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */
#include <version.h>
#include <common.h>

using namespace std;

Logfacility_class Log_common{"System"};

string Get_application_name( string name )
{
	return "Sound Lab module " + name + Version_str;
}

void Wait( long int N)
{
	usleep( N ); //milliseconds
}

size_t kbdnote( char key)
{
	// check if key is in set KbdNote
	return KbdNote.find( key );
}


prgarg_struct_t parse_argv( int argc, char* argv[] )
{
	// https://en.cppreference.com/w/c/language/main_function

	prgarg_struct_t params;
	String 			Str{""};
	string 			next{""};

	for ( int ndx = 1; ndx < argc; ndx ++ )
	{
		string arg = argv[ ndx ];
		( ndx + 1 == argc  ) ? next = "" : next = argv[ ndx + 1 ];
		if ( arg.compare("-c") == 0 )
			params.channel	= Str.secure_stoi( next );
		if ( arg.compare("-r") == 0 )
			params.rate 	= Str.secure_stoi( next );
		if ( arg.compare("-d") == 0 )
			params.device 	= Str.secure_stoi( next );
		if ( arg.compare("-o") == 0 )
			params.ch_offs	= Str.secure_stoi( next );
		if ( arg.compare("-k") == 0 )
		{
			params.shm_key_a= Str.secure_stoi( next );
			params.shm_key_b= params.shm_key_a + 11;
		}
		if ( arg.compare("-t") == 0 )
			params.test 	= 'y';
		if ( arg.compare("-D") == 0 )
			params.dialog 	= 'y';
	}

	return params;
}

void show_prgarg_struct( prgarg_struct_t args )
{
	stringstream strs{};
	strs << setw(20) << left << "channel" << dec << args.channel 		<<endl;  		// -c
	strs << setw(20) << left << "sampline rate" << dec << args.rate		<<endl;  		// -c
	strs << setw(20) << left << "device nr" << dec << args.device		<<endl;  		// -d
	strs << setw(20) << left << "channel offs" << dec << args.ch_offs	<<endl; 		// -o
	strs << setw(20) << left << "shm key A" << dec << args.shm_key_a	<<endl;  		// -k
	strs << setw(20) << left << "shm key B" << dec << args.shm_key_b	<<endl;  		//
	strs << setw(20) << left << "test classes" << dec << args.test		<<endl;  		// -t
	strs << setw(20) << left << "dialog mode" << dec << args.dialog		<<endl;  		// -D
	Log_common.Comment( Log_common.INFO, strs.str() );
}


Time_class::Time_class()
: Logfacility_class("Timer")
{
	start();
	stop(); // duration is zero
}

long int Time_class::time_elapsed()
{
	stop();
	long long int start_count = duration_cast<milliseconds>(start_time.time_since_epoch()).count();
	long long int stop_count  = duration_cast<milliseconds>( stop_time.time_since_epoch()).count();
	return stop_count - start_count;
}
void Time_class::start()
{
	start_time = steady_clock::now();
};

void Time_class::stop()
{
	stop_time = steady_clock::now();
}
void Time_class::block()
{
	duration = time_elapsed();
	latency = duration*100/wait;
	if ( latency > 100 )
		Comment( WARN, "runtime latency exceeds 100% " + to_string( latency ));


	if ( wait > duration )
		ms_wait = wait - duration ;
	usleep( ms_wait * 1000 ); //milli seconds

}


