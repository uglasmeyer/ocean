/*
 * common.h
 *
 *  Created on: Jan 4, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Synthesizer.h>
#include <Interface.h>


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
extern void show_prgarg_struct( prgarg_struct_t );
extern prgarg_struct_t parse_argv( int , char**  );

class Config_class : virtual Logfacility_class
{
public:
	typedef std::unordered_map<string,string> 	config_map_t ;
	config_map_t Get = {};

	Config_class() : Logfacility_class("Config_class"){};
	~Config_class(){};
	void read_synthesizer_config();
};

class DirStructure_class : virtual Logfacility_class
{
public:

	DirStructure_class() : Logfacility_class("DirStructure"){};
	~DirStructure_class(){};
	void Create();

private:
	vector <string> dirs = {
			dir_struct().homedir,
			dir_struct().basedir,
			dir_struct().etcdir,
			dir_struct().bindir,
			dir_struct().libdir,
			dir_struct().tmpdir,
			dir_struct().vardir,
			dir_struct().musicdir,
			dir_struct().instrumentdir,
			dir_struct().logdir,
			dir_struct().notesdir,
			dir_struct().includedir,
			dir_struct().autodir
	};
};

extern void system_execute( const string& );
extern bool cmpstr( const string& , const string&  );
extern vector_str_t List_directory( const string& path, const string& filter );
extern void Exception( );

extern void Wait(long  int );

const static long int SECOND		= 1000000;
const static long int MILLISECOND	= 1000;
const static long int MICROSECOND	= 1;




#endif /* INCLUDE_COMMON_H_ */
