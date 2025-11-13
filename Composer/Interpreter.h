/*
 * Interpreter.h
 *
 *  Created on: Apr 11, 2024
 *      Author: sirius
 */


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <data/Config.h>
#include <data/DataWorld.h>
#include <data/Interface.h>
#include <EventKeys.h>
#include <Spectrum.h>
#include <Ocean.h>
#include <Variation.h>
#include <String.h>
#include <Frequency.h>
#include "Composer/Processor.h"

typedef struct line_struct
{
	int				Id	= 0; // Program Position
	uint 			no	= 0; // filename line no.
	string 			name; // filename
	string 			line; // program line string
	string 			keyw; // keyword
	string 			arg1; // unused
	vector_str_t	args; // program line arr

	line_struct( uint _no, string _line, string _name = "" )
	{
		no		= _no;
		line 	= _line;
		name	= ( _name.length() > 0 ) ? filesystem::path(_name).stem() : "stdin" ;
		String Str { _line };
		Str.replace_comment();
		Str.replace_char('\t' , ' ');
		args 	= Str.to_unique_array( ' ' );
		keyw 	= ( args.size() > 0 ) ? args[0] : "";
		Str.to_lower( keyw );
	}
	void show()
	{
		cout 	<< dec << left
				<< setw( 4) << no << ":"
				<< setw(16) << name
				<< setw(60) << line
				<<endl;
	}
} line_struct_t;
typedef vector<line_struct_t> 	program_vec_t;

#include <Logfacility.h>



const set<string> Keywords {
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
/**************************************************
 * Interpreter_class
 *************************************************/
class Interpreter_class
	: virtual 			Logfacility_class
	, virtual 			osc_struct
	, virtual public 	Processor_class
	, virtual 			Device_class
	, virtual			oscwaveform_struct
//	,					sdsstate_struct

{
	string			className = "";
	typedef struct view_struct
	{
		string 		name 	= "none";
		uint8_t		oscid	= 0;
		EVENTKEY_e 	wfkey	= NULLKEY;
		uint8_t*	wf		= nullptr;
		EVENTKEY_e	ampkey	= NULLKEY;
		uint8_t* 	amp		= nullptr;
		EVENTKEY_e	freqkey	= NULLKEY;
		uint8_t*	frqidx	= nullptr;

	} view_struct_t;

	typedef struct var_struct
	{
		string		name;
		string		value;
	} var_struct_t;


public:
	Interface_class* 	Sds;
	Config_class*		Cfg;
	file_structure*		fs;
	interface_t* 		sds;
	Variation_class 	Variation;
	view_struct_t main_view, fmo_view, vco_view;

	String Str{""};
	string cmdline = "";
	string option_default = "";
	bool 	dialog_mode = false;
	bool	CompilerExit= false;

	int error = 0;
	int duration = 0;
	String keyword {""};
	set<string> expect {};

	Interpreter_class( Application_class* app ) ;
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
	void Wrong_keyword( set<string>, string );
	void Pause( vector_str_t );
	void Play( vector_str_t );
	void Text( vector_str_t );
	void Add( vector_str_t );
	void Addvariable( vector_str_t );
	vector_str_t InsertVariable( vector_str_t );
	int  Find_position( const program_vec_t& program, vector_str_t );
	void Set_dialog_mode( bool );
	bool Exit();
	void Set( vector_str_t );
	void Clear_stack();
	bool Check_input( string keyword );

	void Test(  );

private:
	Spectrum_class 		Spectrum	{};
	Frequency_class 	Frequency	{};
	vector_str_t stack {};
	vector<var_struct_t> varlist {};
	string	command 	{""};
	bool 	testrun 	= false;
	bool	testreturn 	= false;

	void 	Loop( uint8_t , EVENTKEY_e   );
	bool 	check_count( vector_str_t, size_t );
	string 	pop_stack( int );
	bool	set_stack( vector_str_t, uint );
	void 	show_expected(  );
	void 	check_file( vector_str_t, string );
	bool 	no_error( int );
	void 	intro( vector_str_t, uint );
	void 	osc_view( view_struct_t, vector_str_t );
	bool	cmpkeyword ( const string&  );

	template <typename T >
		T pop_T( range_T<T> range );
};


/**************************************************
 * Composer_class
 *************************************************/
class Composer_class :
	virtual public Logfacility_class
{
	string 					className 		= "";
	Dataworld_class*		DaTA			;
	Config_class*			Cfg				;
	Interpreter_class*		Interpreter		;
	vector<int>				pos_stack 		{};


public:
	program_vec_t			Program;

	Composer_class( Dataworld_class* data, Interpreter_class* interpreter ) :
		Logfacility_class("Compiler_class")
	{
		className 			= Logfacility_class::className;
		this->DaTA			= data;
		this->Cfg			= DaTA->Cfg_p;
		this->Interpreter	= interpreter;
	};
	virtual ~Composer_class()
	{
		DESTRUCTOR( className );
	};
	bool				Compile			();
	bool 				PreCompile		( string batch_file );
	bool				Interprete		( line_struct_t program_line );
	int 				SetProgramCursor( const program_vec_t& program, line_struct prgLine );

private:
	int 				return_pos		( int pos );
	int 				call_pos		( int pos, const program_vec_t& program );

};

#endif /* INTERPRETER_H_ */
