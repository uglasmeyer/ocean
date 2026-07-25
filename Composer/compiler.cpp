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
 * compiler.cpp
 *
 *  Created on: Nov 12, 2025
 * Author: Ulrich.Glasmeyer@web.de
 */




#include <Composer/Interpreter.h>
#include <fstream>

using namespace Interpreter_ns;

/**************************************************
 * line_struct
 *************************************************/
#include <regex>

line_struct::line_struct( uint _no, string _line, string _name )
{
	auto assign_keyw = [ ]( String& Str, const string_vec_t arr )
	{
		if( arr.size() > 1 )
			if( arr[1][0] == '=' ) // do not change to lower in case of variable assignment
				return arr[1];
		return Str.to_lower();
	};

	auto match_concat = [  ]( string line )
	{
        std::smatch search_match;
		std::regex pattern( R"((\w+) = (\w+) [+] (\w+)(\s*))", std::regex_constants::icase );
		bool result = regex_search( line, search_match, pattern );
		cout << line << " matching: " << search_match[0] << endl;
		return result ? CONCAT : ASSIGN;
	};

	if ( _line.length() == 0 )
		return;
	no		= _no;
	jmp		= JMPUNDEF;
	name	= ( _name.length() > 0 ) ? _name : "stdin" ;
	String
	Str 	{ _line };
	Str.replace_comment();
	Str.replace_char('\t' , ' ');
	line	= Str.Str;
	args 	= Str.to_unique_array( ' ' );
	Str.Str	= ( args.size() > 0 ) ? args[0] : "";

	keyw	= assign_keyw( Str, args );
	key		= GetKeywordId( keyw );

	if( key == ASSIGN )
		key = match_concat( line );
}


/**************************************************
 * Composer_class
 *************************************************/
pos_stack_t pos_stack;
string tmp_var = "";

Composer_class::Composer_class( Dataworld_class* data, Interpreter_class* interpreter ) :
	Logfacility_class("Composer_class")
	, Interface_base( data )
{
	this->Interpreter	= interpreter;
	this->Program_file	= fs->program_file;
	this->test_mode		= false;
	PrgTable.opt.Ident	= 0;
	PrgTable.AddColumn	( "Id", 4 );
	PrgTable.AddColumn	( "File(no)", 20 );
	PrgTable.AddColumn	( "Program Line", term_columns() - 36 );
	PrgTable.AddColumn	( "Jump", 4 );

	this->Program.clear	();
};
Composer_class::~Composer_class()
{
	DESTRUCTOR( className );
};

void Composer_class::SetTestMode( bool mode )
{
	test_mode	= mode;
}
CODE_e Composer_class::Show( string_vec_t arr )
{
	Interpreter->expect = { "stack" };
	Interpreter->Intro( arr, 1 );
	Interpreter->SetKeyword		( 1 );

	if( Interpreter->Cmpkeyword( "stack" ) )
	{
		Show_Program( Program_file );
		return NOEXEC;
	}

	Interpreter->Wrong_keyword( Interpreter->expect, arr[0] );
	return NOEXEC;
}

int Composer_class::find_subroutine_pos(const program_vec_t &program, string keyw)
{
	// return the progam line id starting with keyword
	int pos = JMPUNDEF;
	for (line_struct_t row : program)
	{
		if (strEqual(keyw, row.keyw))
		{
			if( pos == JMPUNDEF  )
				pos = row.Id;
			else
				Exception( "duplicat definition of "  +
							keyw +
							" in " +
							to_string( row.no ) +
							" and "+
							to_string( pos ) );
		}
	}

	if( pos > JMPUNDEF )
		return pos;

	// not found
	std::for_each(program.begin(), program.end(), [](line_struct_t line)
	{
		if (line.keyw[0] == ':' )
			cout << line.keyw << " ";
	}
	);
	Interpreter->If_Exception("line >" + keyw + " not found");
	return -2;
}


void Composer_class::show_prgline( const line_data_t& row )
{
	string file_name_no = row.name + "(" + to_string( row.no ) + ":0)";
	PrgTable.AddRow( row.Id, file_name_no, row.line, row.jmp );
}
void Composer_class::Show_Program( string name )
{
	coutf << Line( term_columns() ) << endl;

	Info( "Program", name );
	PrgTable.PrintHeader();

	size_t pos = 0;
	line_data_t row = Program[pos];

	do
	{

		row = Program[pos];
		show_prgline( row );
		pos = row.jmp;

		switch ( row.key )
		{
			case KEYWORD_e::ENDIF 		:
			case KEYWORD_e::RETURN		: {	pos = pos_stack.Pop()			; break; }
			case KEYWORD_e::CALL		:
			case KEYWORD_e::IF  		: { pos_stack.Push( int(row.Id + 1) )	;
																; break; }
			default						: { 					; break; }
		}

	}
	while( pos >=0 and pos < Program.size() );
}

string_vec_t Composer_class::multi_cmd_line( string cmds )
{
	string_vec_t 	cmd_vec	{};
	string			tmp		{};
	for( char ch : cmds )
	{
		if ( ch ==  ';' )
		{
			cmd_vec.push_back( tmp );
			tmp = "";
		}
		else
		{
			tmp.push_back( ch );
		}
	}
	cmd_vec.push_back( tmp );
	return cmd_vec;
};

void Composer_class::add_row( line_struct_t prg_line )
{
	int	Id 		= 0;
	if( Program.size() > 0 )
		Id = Program.back().Id + 1;
	prg_line.Id = Id;
	Program.push_back( prg_line );
}

set<string> file_set {};

bool Composer_class::PreCompile( string batch_file )
{
	auto replace_keyword = [ this ]( line_struct program_line )
	{
		if ( strEqual( program_line.keyw, "for" ) )
		{
			make_loop( program_line );
			return true;
		}
		if( strEqual( program_line.keyw, "repeat" ) )
		{
			replace_repeat( program_line );
			return true;
		}
		return false;
	};

	fstream 		File	;
	File.open( batch_file, fstream::in );
	if ( not File.is_open())
	{
		Comment(ERROR, "pre-compiler: Input file: " + batch_file + " does not exist" );
		return false;
	}
	else
	{
		Comment( INFO, "Processing input file: \n" + batch_file  );
	}

	filesystem::path	filePath{ batch_file };
	string			file_line	= "";
	string			filename	= filePath.stem();
	if( file_set.contains( filename ) )
	{
		Info( "include file", batch_file, "already included" );
		return true; 	// avoid duplicate includes
	}
	file_set.insert				( filename );
	uint 			lineno		= 1;
	getline			( File, file_line);


	do
	{
		line_struct program_line = line_struct( lineno, file_line, filename );

		if ( program_line.keyw.compare( "include" ) == 0 )
		{
			string incname = ( program_line.args.size() > 1) ? program_line.args[1] : "";
			if ( incname.length() > 0 )
			{
				PreCompile( fs->includedir + incname + ".inc" );
			}
			else
			{
				Comment( WARN, "empty include clause ignored" );
			}
		}
		else
		{
			if ( program_line.keyw.length() != 0 )
			{
				if ( Interpreter->dialog_mode )
					program_line.name = "stdin";
				else
					program_line.name = filename;
				if ( not replace_keyword( program_line ) )
				{
					add_row( program_line );
				}
			}
		}
		lineno++;
	}
	while( getline( File, file_line ));

	Info( "Pre-processed program lines: ", lineno );

	return true;
}

void Composer_class::replace_repeat( line_struct prgline )
{	// repeat 5 :sub
	// arg1=5, arg2=:sub
	string 		times	= prgline.args[1];	// number of times to repeat
	string		cmd		= "reg 0 load " + times;
	line_struct newline	= line_struct( prgline.no, cmd );
				add_row	( newline );

	string		subprg	= prgline.args[2];	// the sub routine to call repeated times
				cmd 	= "if " + subprg;
				newline = line_struct( prgline.no, cmd );
				add_row	( newline );
}
void Composer_class::make_loop( line_struct prgline )
{
	auto get_all = [  ]( string_vec_t args )
	{
		string_vec_t all {};
		bool begin = false;
		for ( string& arg : args )
		{
			if( strEqual( arg, ")" ) )
				begin = false;
			if( begin )
				all.push_back( arg );
			if( strEqual( arg, "(" ) )
				begin = true;
		}
		return all;
	};
	auto forloop_args = [  ]( string_vec_t args )
	{
		string	cmd		{};
		bool 	begin	= false;
		for( string& arg : args )
		{
			if( begin )
				cmd += arg + " ";
			if( strEqual( arg, ")" ) )
				begin = true;
		}
		return cmd;
	};
	auto replaceAll = [  ] ( const string str, const  std::string &from, const std::string &to)
	{
	    if (from.empty()) return str; // Avoid infinite loop

	    string ret = str;
	    size_t startPos = 0;
	    while ((startPos = ret.find(from, startPos)) != std::string::npos)
	    {
	        ret.replace(startPos, from.length(), to);
	        startPos += to.length(); // Move past the replaced part
	    }
	    return ret;
	};

	string_vec_t ALL = get_all( prgline.args );
	string forcmds= forloop_args( prgline.args );
	for( string& arg : ALL  )
	{
		string cmds = replaceAll( forcmds, "ALL", arg );
		string_vec_t cmd_vec = multi_cmd_line( cmds );
		for( string cmd : cmd_vec )
		{
			line_struct newline{ prgline.no, cmd };
			add_row( newline );
		}
	}
}

auto is_assignment = [ ]( const string_vec_t& arr )
{
	return ( arr.size() > 1 ) ? ( arr[1][0] == '=' ) : false;
};

bool Composer_class::ProgramFlow()
{
	// defines next jump for all program rows.
	// expected result as follows:
	//pos	keyw				jmp		comment
	// 15	call :sub_call		30 		<- find subprocess
	// 16   call :sub_call		30
 	// 17	if :sub_if			35 		<- find subprocess
	// 18	...
	// 30	:sub_call			31 		evaluate args by interpreter
	// 31	...
	// 34	return				-1		dynamic return pos
	//
	// 35	:sub_if				36
	// 36	...
	// 39	endif 0				-1		return pos evaluated by processor

	auto call_pos = [ this ]( int pos )
	{
		string subroutine = Program[pos].args[1];
		return find_subroutine_pos( Program, subroutine );
	};

	for( line_struct_t& row : Program )
	{
		int 	pos 		= row.Id;

		switch ( row.key )
		{
			case KEYWORD_e::SUB :
			{
				Program[pos].jmp = row.Id + 1;
				break;
			}
			case KEYWORD_e::IF :
			case KEYWORD_e::CALL :
			{
				Program[pos].jmp	= call_pos( pos );
				break;
			}
			case KEYWORD_e::RETURN :
			case KEYWORD_e::ENDIF :
			{
				break;
			}
			case KEYWORD_e::EXIT :
			{
				Program[pos].jmp 	= Program.size();
				break;
			}
			default :
			{
				Program[pos].jmp	= row.Id + 1;
				break;
			}
		} // switch key
		show_prgline( row );
	} // for
	return true;
}


int Composer_class::Interprete( line_data_t row )
{
	auto sub_args = [ this ]( string value, string_vec_t arr  )
	{
		if( arr.size() == 1 )
			return; // nothing to do
		if( value.size() == 0 )
			return; // nothing catched
		Add( arr[1], value );
	};

	auto call_args = [  ]( string_vec_t arr )
	{
		return ( arr.size() > 2 ) ? arr[2] : "";
	};

	show_prgline( row );
	Interpreter->Set_prgline( row.no, row.Id ); // used in processor stack

	bool mode = ( row.name.compare("stdin") == 0 );
	Interpreter->Set_dialog_mode( mode );
	if( test_mode )
		Interpreter->Set_dialog_mode( true );

	string_vec_t	arr 	= ReplaceVariable( row.args );
	switch ( row.key )
	{

		case KEYWORD_e::RETURN		: {	RemoveVar( tmp_var )	;
										return pos_stack.Pop()				; break; }
		case KEYWORD_e::CALL		: { pos_stack.Push( int(row.Id + 1) )	;
										tmp_var = call_args( arr )			; break; }
		case KEYWORD_e::SUB			: { sub_args( tmp_var, arr );
										return row.jmp 						; break; }
		case KEYWORD_e::EXIT		: { return row.jmp						; break; }

		case KEYWORD_e::ADDMUSIC 	: { Interpreter->AddMusic( arr )		; break; }
		case KEYWORD_e::ADSR 		: { Interpreter->Adsr( arr )			; break; }
		case KEYWORD_e::ASSIGN 		: { Add ( arr[0], arr[2] )				; break; }
		case KEYWORD_e::CONCAT 		: { Interpreter->Addvariable( arr )		; break; }
		case KEYWORD_e::DEBUG 		: { Interpreter->Debug( arr )			; break; }
		case KEYWORD_e::DYN  		: { Interpreter->Dynamic( arr )			; break; }
		case KEYWORD_e::ENDIF  		: { Interpreter->Endif( arr )			;
										return pos_stack.Pop()				; break; }
		case KEYWORD_e::IF  		: { pos_stack.Push( int(row.Id + 1) )	;
										Interpreter->Ifpos( arr )			; break; }
		case KEYWORD_e::INSTRUMENT  : { Interpreter->Instrument(arr)		; break; }
		case KEYWORD_e::NOTES  		: { Interpreter->Notes( arr )			; break; }
		case KEYWORD_e::OSC  		: { Interpreter->Osc( arr )				; break; }
		case KEYWORD_e::PAUSE  		: { Interpreter->Pause( arr )			; break; }
//		case KEYWORD_e::PLAY  		: { Interpreter->Play( arr )			; break; }
		case KEYWORD_e::RECORD  	: { Interpreter->RecFile( arr )			; break; }
		case KEYWORD_e::RANDOM  	: { Interpreter->Random( arr )			; break; }
		case KEYWORD_e::RESET 		: { Interpreter->Reset( arr )			; break; }
		case KEYWORD_e::REG  		: { Interpreter->Register( arr )		; break; }
		case KEYWORD_e::SET  		: { Interpreter->Set( arr )				; break; }
		case KEYWORD_e::STA  		: { Interpreter->StA( arr )				; break; }
		case KEYWORD_e::START  		: { Interpreter->Start_bin( arr )		; break; }
		case KEYWORD_e::STOP  		: { Interpreter->Stop_bin( arr )		; break; }
		case KEYWORD_e::TEXT 		: { Interpreter->Text( arr )			; break; }
		default						: {	Interpreter->Wrong_keyword( Keywords, row.keyw );
										break; }
	} // switch key

	if ( Interpreter->error > 0 )
	{
		Interpreter->If_Exception( "Compiler error with return code: " +
									to_string(  Interpreter->error ) );
	}


	return row.jmp; // continue with the next prg row
}
bool Composer_class::Compile()
{
	pos_stack.Clear();
	if( not ProgramFlow() )
	{
		Exception( "cannot establish program flow" );
	}
	uint 				pos 			= 0;
	uint 				nr_of_prgLines = Program.size();

	while ( ( pos >= 0 ) and ( pos < nr_of_prgLines ) )
	{
		line_data_t 	row	= Program[pos];
		pos = Interprete( row );
	} ;
	return true;
}
