/*
 * common.h
 *
 *  Created on: Jan 4, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Synthesizer.h>

template<typename T> char type_of( T obj )
{
	const std::type_info& tid = typeid( obj );
	const std::type_info& ti1 = typeid( string );
	const std::type_info& ti2 = typeid( char );
	const std::type_info& ti4 = typeid( int );
	const std::type_info& ti3 = typeid( uint16_t );
	if ( ti1 == tid ) return 's';
	if ( ti2 == tid ) return 'c';
	if ( ti3 == tid ) return 16;
	if ( ti4 == tid ) return 'i';
	return 0;
};


typedef struct prgarg_struct
{
		~prgarg_struct(){};
		uint channel 	= 2;  		// -c
		uint rate 		= audio_frames;	// -r
		uint device 	= 0;		// -d
		uint ch_offs 	= 0; 		// -o
		uint shm_key_a 	= rate; 	// -k
		char test 		= 'n';		// -t run sanity check on classes and exit = 'y'
		char dialog		= 'n';		// -d dialog mode of the composer = 'y'
		uint shm_key_b 	= rate + 11;	//
} prgarg_struct_t;

typedef std::unordered_map<string,string> 	config_map_t ;

extern config_map_t read_synthesizer_config( );
extern void creat_dir_structure();

extern void system_execute( const string& );
extern bool cmpstr( const string& , const string&  );
extern vector_str_t List_directory( const string& path, const string& filter );
extern void Exception( );

extern void show_prgarg_struct( prgarg_struct_t );
extern prgarg_struct_t parse_argv( int , char**  );
extern void Wait(long  int );

const static long int SECOND		= 1000000;
const static long int MILLISECOND	= 1000;
const static long int MICROSECOND	= 1;


using namespace std::chrono;
class Time_class : public Logfacility_class
{
	// https://en.cppreference.com/w/cpp/chrono

public:

	time_point<steady_clock> start_time;
	time_point<steady_clock> stop_time;
	const long int wait = max_sec * 1000 ;
	long int duration, ms_wait;
	long int latency = 0;

	Time_class();
	~Time_class(){};
	long int Time_elapsed();
	void Start();
	void Stop();
	void Block();
};


#endif /* INCLUDE_COMMON_H_ */
