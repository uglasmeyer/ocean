/*
 * Interpreter.h
 *
 *  Created on: Apr 11, 2024
 *      Author: sirius
 */


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <Interface.h>
#include <Keys.h>
#include <Processor.h>
#include <Spectrum.h>
#include <Synthesizer.h>
#include <Variation.h>

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

/*
*/
class Interpreter_class : virtual Logfacility_class, public Processor_class
{
	typedef struct osc_struct
	{
		string 		name 	= "none";
		uint8_t 	wfkey	= 0;
		uint8_t*	wf		= nullptr;
		uint8_t		ampkey	= 0;
		uint8_t* 	amp		= nullptr;
		uint8_t		freqkey	= 0;
		float*		freq	= nullptr;

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

	void Start_bin( vector_str_t );
	void Stop_bin( vector_str_t );
	void Instrument( vector_str_t );
	void Notes( vector_str_t );
	void Random( vector_str_t );
	void Osc( vector_str_t );
	void Adsr( vector_str_t );
	void RecStA( vector_str_t );
	void RecFile( vector_str_t );
	void Wrong_keyword( vector<string>, string );
	void Pause( vector_str_t );
	void Play( vector_str_t );
	void Text( vector_str_t );
	void Add( vector_str_t );
	void Addvariable( vector_str_t );
	vector_str_t InsertVariable( vector_str_t );
	int  Find_position( vector<line_struct_t>*, vector_str_t );
	void Set_dialog_mode( bool );
	void ExitInterpreter();
	void Set( vector_str_t );
	void Clear_stack();
	void Test(  );

private:
	Spectrum_base Spectrum;
	vector_str_t stack {};
	vector<var_struct_t> varlist {};
	string	command 	{""};
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
	void 	osc_view( osc_struct_t, vector_str_t );
	bool	cmpkeyword ( const string&  );

};

#endif /* INTERPRETER_H_ */
