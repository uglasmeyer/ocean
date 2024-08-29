/*
 * Interpreter.h
 *
 *  Created on: Apr 11, 2024
 *      Author: sirius
 */


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <Interface.h>
#include <keys.h>
#include <Processor.h>
#include <variation.h>
#include <Spectrum.h>
#include <Synthesizer.h>

typedef struct line_struct
{
	uint no 	= 0;
	string name	{} ; // filename
	string line	{} ; // line string
	string keyw {} ; // keyword
	string arg1 {} ;
} line_struct_t;

const vector<string> Keywords {
						"add",
						"adsr",
						"call",
						"exit",
						"instrument",
						"notes",
						"osc",
						"play",
						"pause",
						"random",
						"rec",
						"record",
						"return",
						"set",
						"start",
						"stop",
						"text"
					};

template<typename T>
void show_items( T all_items )
{
    for (auto item : all_items )
    	cout << item << " ";
    cout << endl;
}

class Interpreter_class : public virtual Logfacility_class, public Processor_class
{
	typedef struct osc_struct
	{
		string 		name = "none";
		uint8_t 	wfkey=0;
		uint8_t*	wf=nullptr;
		uint8_t		ampkey=0;
		uint8_t* 	amp=nullptr;
		uint8_t		freqkey=0;
		float*		freq=nullptr;

	} osc_struct_t;

	typedef struct var_struct
	{
		string		name;
		string		value;
	} var_struct_t;


public:
	Interface_class* GUI;
	ifd_t* ifd;// = GUI->ifd_data;
	Variation_class Variation{};
	osc_struct_t main_view, fmo_view, vco_view;

	String Str{""};
	string cmdline = "";
	string option_default = "";
	bool 	dialog_mode = false;


	int error = 0;
	int duration = 0;
	String keyword {""};
	vector<string> expect {};

	Interpreter_class( Interface_class* gui ) ;
	virtual ~Interpreter_class();

	void start_bin( vector_str_t );
	void stop_bin( vector_str_t );
	void instrument( vector_str_t );
	void notes( vector_str_t );
	void random( vector_str_t );
	void osc_view( osc_struct_t, vector_str_t );
	void Osc( vector_str_t );
	void adsr( vector_str_t );
	void rec( vector_str_t );
	void record( vector_str_t );
	void Wrong_keyword( vector<string>, string );
	void pause( vector_str_t );
	void play( vector_str_t );
	void text( vector_str_t );
	void run();
	void add( vector_str_t );
	void addvariable( vector_str_t );
	vector_str_t insvariable( vector_str_t );
	int  find_position( vector<line_struct_t>*, vector_str_t );
	void set_dialog_mode( bool );
	void exit_interpreter();
	void set( vector_str_t );
	bool cmpkeyword ( const string  );
	void clear_stack();
	void test(  );

private:
	Spectrum_class Spectrum;
	vector_str_t stack {};
	vector<var_struct_t> varlist {};
	bool 	testrun 	= false;
	bool	testreturn 	= false;

	void 	Loop( int , int   );
	bool 	check_count( vector_str_t, size_t );
	string 	pop_stack( int );
	bool	set_stack( vector_str_t, uint );
	int 	pop_int( int, int );
	void 	show_expected(  );
	void 	check_file( vector_str_t, string );
	bool 	no_error( int );
	void 	intro( vector_str_t, uint );

};

#endif /* INTERPRETER_H_ */
