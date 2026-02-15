/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
 * Interpreter.h
 *
 *  Created on: Apr 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <Variation.h>
#include <Composer/Processor.h>
#include <Composer/Interpreter_base.h>

/*
*/
/**************************************************
 * Interpreter_class
 *************************************************/
class Interpreter_class
	: virtual public	Logfacility_class
	, virtual public 	Processor_class
	, virtual 			Device_class

{
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

	bps_t			Bps					;

	Spectrum_class 			Spectrum			{};
	Frequency_class 		Frequency			{};
	vector_str_t 			stack 				{};
	vector<var_struct_t>	varlist 			{};
	string					command 			{""};
	bool 					testrun 			= false;
	bool					testreturn 			= false;
	int 					duration 			;
	String 					keyword 			{};
	String 					Str					{};
	string 					cmdline 			= "";
	string 					option_default		= "";
	Interface_class* 		Sds					;
	Config_class*			Cfg					;
	file_structure*			fs					;
	interface_t* 			sds					;
	Variation_class 		Variation			;
	view_struct_t 			main_view, fmo_view, vco_view;

public:

	bool 					dialog_mode 		;
	bool					CompilerExit		;
	int 					error 				;
	set<string> 			expect 				{};

							Interpreter_class	( Application_class* app ) ;
	virtual 				~Interpreter_class	();

	void 					Add					( vector_str_t );
	void 					Addvariable			( vector_str_t );
	void 					Adsr				( vector_str_t );
	void					Clear_stack			();
	bool 					Check_input			( string keyword );
	bool					Cmpkeyword 			( const string&  );
	void 					Debug				();
	void 					Dynamic				( vector_str_t arr );
	bool 					Exit				();
	int  					Find_position		( const program_vec_t& program,
												vector_str_t );
	void 					Instrument			( vector_str_t );
	vector_str_t 			InsertVariable		( vector_str_t );
	void 					Intro				( vector_str_t, uint );
	void					If_Exception		( string );
	void 					Notes				( vector_str_t );
	void 					Osc					( vector_str_t );
	void 					Pause				( vector_str_t );
	void 					Play				( vector_str_t );
	void 					Random				( vector_str_t );
	void 					Reset				( vector_str_t );
	void 					RecStA				( vector_str_t );
	void 					RecFile				( vector_str_t );
	void 					Set					( vector_str_t );
	void 					Set_dialog_mode		( bool );
	void 					Start_bin			( vector_str_t );
	void 					Stop_bin			( vector_str_t );
	void 					Text				( vector_str_t );
	void 					Test				();
	void 					Wrong_keyword		( set<string>, string );


private:

	void 					amploop				( uint8_t , EVENTKEY_e   );
	bool 					check_count			( vector_str_t, size_t );
	void 					check_file			( vector_str_t, string );
	bool 					no_error			( int );
	void 					osc_view			( view_struct_t, vector_str_t );
	string 					pop_stack			( int );
	bool					set_stack			( vector_str_t, uint );
	void 					show_expected		();

	template< typename Arg>
		void 				wrong_argument		( set<string> expected , Arg given );
	template <typename T >
		T 					pop_T				( range_T<T> range );
};


/**************************************************
 * Composer_class
 *************************************************/
enum CODE_e { EXECUTE, NOEXEC };

class Composer_class :
	virtual public Logfacility_class
{
	string 					className 		= "";
	Dataworld_class*		DaTA			;
	Config_class*			Cfg				;
	vector<int>				pos_stack 		{};


public:
	program_vec_t			Program;
	Interpreter_class*		Interpreter		;

	Composer_class( Dataworld_class* data, Interpreter_class* interpreter ) :
		Logfacility_class("Composer_class")
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
	CODE_e 				Show			( vector_str_t arr );

private:
	int 				return_pos		( int pos );
	int 				call_pos		( int pos, const program_vec_t& program );

};

#endif /* INTERPRETER_H_ */
