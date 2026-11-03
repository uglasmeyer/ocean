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
 * Interpreter.h
 *
 *  Created on: Apr 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <Variation.h>
#include <Oscwaveform.h>
#include <Composer/Processor.h>
#include <Composer/Interpreter_base.h>


/**************************************************
 * assignment_struct
 * control the content of the assignment vector ( A = B )
 *************************************************/

typedef struct assignment_struct
{
private:
	typedef struct var_struct
	{
		string		name;
		string		value;
	} var_struct_t;

	vector<var_struct_t> assignment_vec {};

public:
	assignment_struct();
	~assignment_struct()	= default;
	string_vec_t ReplaceVariable( string_vec_t arr );
	void RemoveVar( string varname );
	void Show_const();
	void Add( string var, string val );
	void Clear();
	void Test();

} assignment_t;


/**************************************************
 * Interpreter_class
 *************************************************/
class Interpreter_class
	: virtual public	Logfacility_class
	, 					Frequency_class
	, virtual			Interface_base
	, virtual public 	Processor_class

{
	typedef struct view_struct
	{
		string 		name 	= "none";
		OSCID_e		oscid	= NOOSCID;
		EVENTKEY_e 	wfkey	= NULLKEY;
		WFID_e*		wf		= nullptr;
		EVENTKEY_e	ampkey	= NULLKEY;
		uint8_t* 	amp		= nullptr;
		EVENTKEY_e	freqkey	= NULLKEY;
		uint8_t*	frqidx	= nullptr;

	} view_struct_t;


	bps_t					Bps					;
	string_vec_t 			stack 				{};
	string					command 			{""};
	bool 					testrun 			= false;
	bool					testreturn 			= false;
	int 					duration 			;
	String 					keyword 			{};
	string 					cmdline 			= "";
	string 					option_default		= "";
	Config_class*			Cfg_p				;
	Musicxml_class			Musicxml			;
	Variation_class 		Variation			;
	view_struct_t 			main_view, fmo_view, vco_view;

public:

	bool 					dialog_mode 		;
	bool					CompilerExit		;
	int 					error 				;
	uset_t					expect 				{};

							Interpreter_class	( Application_class* app ) ;
	virtual 				~Interpreter_class	();

	void 					AddMusic			( string_vec_t );
	void 					Addvariable			( string_vec_t );
	void 					Adsr				( string_vec_t );
	bool 					Check_input			( string keyword );
	bool					Cmpkeyword 			( string word);
	void 					Debug				( string_vec_t arr);
	void 					Dynamic				( string_vec_t arr );
	void					Endif				( string_vec_t arr );
	bool 					Exit				();
	void 					Instrument			( string_vec_t );
	void 					Intro				( string_vec_t, uint );
	void					If_Exception		( string );
	void					Ifpos				( string_vec_t arr );
	void 					Notes				( string_vec_t );
	void 					Osc					( string_vec_t );
	void 					Pause				( string_vec_t );
//	void 					Play				( string_vec_t );
	void 					Random				( string_vec_t );
	void 					Reset				( string_vec_t );
	void 					StA					( string_vec_t );
	void					SubArgs				( string varname, string varvalue );
	void 					RecFile				( string_vec_t );
	void					Register			( string_vec_t arr );
	void 					Set					( string_vec_t );
	void 					Set_dialog_mode		( bool );
	void 					SetKeyword			( uint8_t min, uset_t valid = {} );
	void 					Start_bin			( string_vec_t );
	void 					Stop_bin			( string_vec_t );
	void 					Text				( string_vec_t );
	void 					Test				();
	void 					Wrong_keyword		( uset_t, string );


private:

	void 					master_amploop				( uint8_t amp, uint8_t time );
	bool 					check_count			( string_vec_t, size_t );
	void 					check_file			( string_vec_t, string );
	StAId_e		 			expect_staid		();
	bool 					no_error			( int );
	bool 					is_regid			( string str );
	void 					osc_view			( view_struct_t, string_vec_t );
	string 					pop_stack			( int );
	void 					set_osc_view		( interface_t* sds );
	bool					set_stack			( string_vec_t, uint );
	void 					show_expected		();
	string 					pop_set				( uset_t Set );

	template< typename Arg>
		void 				wrong_argument		( uset_t expected , Arg given );
	template <typename T >
		T 					pop_T				( range_T<T> range );
	template <typename T >
		T 					pop_T				( set<T> Set );
};



/**************************************************
 * Composer_class
 * provides pre-compiler and interpreter functionality
 * to the Composer
 * on the Program vector
 * in dialog or batch mode
 *************************************************/
enum CODE_e { EXECUTE, NOEXEC };

class Composer_class :
	virtual public 		Logfacility_class,
	public				Interface_base,
	public 				assignment_struct
{
	bool				test_mode		;
	Table_class			PrgTable		{};

public:
	program_vec_t		Program			;
	Interpreter_class*	Interpreter		;
	string				Program_file	;

						Composer_class	( Dataworld_class* data, Interpreter_class* interpreter );
	virtual 			~Composer_class	();
	bool				Compile			();
	bool 				PreCompile		( string batch_file );
	bool 				ProgramFlow		();
	int					Interprete		( Interpreter_ns::line_data_t program_line );
	void				SetTestMode		( bool mode );
	CODE_e 				Show			( string_vec_t arr );
	void				Show_Program	( string name );

private:
	void				add_row			( Interpreter_ns::line_struct_t prg_line );
	int 				call_pos		( int pos, const program_vec_t& program );
	int 				find_subroutine_pos	(const program_vec_t &program, string keyw );
	void				make_loop		( Interpreter_ns::line_struct prgline );
	string_vec_t 		multi_cmd_line	( string cmds );
	void 				replace_repeat	( Interpreter_ns::line_struct prgline );
	int 				return_pos		( int pos );
	void 				show_prgline	( const Interpreter_ns::line_data_t& row );
};

#endif /* INTERPRETER_H_ */
