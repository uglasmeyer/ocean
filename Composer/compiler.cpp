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
 * compiler.cpp
 *
 *  Created on: Nov 12, 2025
 * Author: Ulrich.Glasmeyer@web.de
 */




#include <Composer/Interpreter.h>

/**************************************************
 * Composer_class
 *************************************************/
bool Composer_class::Compile()
{

	uint 				pos 			= 0;
	uint 				nr_of_filelines = Program.size();

	while ( ( pos >= 0 ) and ( pos < nr_of_filelines ) )
	{
		line_struct 	program_line	= Program[pos];
						pos 			= SetProgramCursor( Program, program_line );
		Interpreter->Set_prgline( pos );
		if ( not Interprete( program_line ) )
		{
			return false;
		}
		else
		{
			if ( Interpreter->CompilerExit )
				return true;
		}
		pos++;

	} ;


	return true;
}

int program_lineId = 0;
bool Composer_class::PreCompile( string batch_file )
{
	fstream 		File	;
	File.open( batch_file, fstream::in );
	if ( not File.is_open())
	{
		Comment(ERROR, "Input file: " + batch_file + " does not exist" );
		raise (SIGINT );
	}
	else
	{
		Comment( INFO, "Processing input file: \n" + batch_file  );
	}


	string 			path		= filesystem::path( batch_file );
	string			file_line	= "";
	uint 			lineno		= 1;
	getline			( File, file_line);


	do
	{
		line_struct program_line = line_struct( lineno, file_line, batch_file );

		if ( program_line.keyw.compare( "include" ) == 0 )
		{
			string filename = ( program_line.args.size() > 1) ? program_line.args[1] : "";
			if ( filename.length() > 0 )
			{
				PreCompile( Cfg->fs->includedir + filename + ".inc" );
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
				program_line.Id = program_lineId;
				Program.push_back( program_line );
				program_lineId++;		}
		}
		lineno++;
	}
	while( getline( File, file_line ));
	Info( "Pre-processed program lines: ", lineno );

	return true;
}

int Composer_class::return_pos( int pos )
{

	if ( pos_stack.size() == 0 )
	{
		Exception( " return without goto at pos: " + to_string( pos+1 ) ) ;
	}
	pos = pos_stack.back( );// access the last element
	pos_stack.pop_back();	// delete the last element
	return pos;
}

int Composer_class::call_pos( int pos, const program_vec_t& program )
{
	pos_stack.push_back( pos );
	pos = Interpreter->Find_position( program, program[pos].args );
	return pos;
}

int Composer_class::SetProgramCursor( const program_vec_t& program, line_struct prgLine )
{
	int 				pos 		= prgLine.Id;
	string 				keyword 	= prgLine.keyw;
	if ( strEqual( keyword, "return") )
		pos = return_pos ( pos );
	if ( strEqual( keyword, "call"  ) )
		pos = call_pos( pos, program );
	return pos;
}

bool Composer_class::Interprete( line_struct_t program_line )
{
	program_line.show();

	if ( program_line.name.compare("stdin") == 0 )
		Interpreter->Set_dialog_mode( true );
	else
		Interpreter->Set_dialog_mode( false );

	vector_str_t	arr 	= Interpreter->InsertVariable( program_line.args );
	string 			keyword = program_line.keyw;//( arr.size() > 0 ) ? arr[0] : "";

	if ( Interpreter->Check_input( keyword ) )
	{
		if ( keyword.compare("add") 		== 0 )	Interpreter->Add( arr );
		if ( keyword.compare("adsr") 		== 0 )	Interpreter->Adsr( arr );
		if ( keyword.compare("exit") 		== 0 )	return Interpreter->Exit();
		if ( keyword.compare("instrument") 	== 0 ) 	Interpreter->Instrument( arr );
		if ( keyword.compare("notes") 		== 0 )	Interpreter->Notes( arr );
		if ( keyword.compare("osc") 		== 0 )	Interpreter->Osc( arr );
		if ( keyword.compare("pause") 		== 0 )	Interpreter->Pause( arr );
		if ( keyword.compare("play") 		== 0 )	Interpreter->Play( arr );
		if ( keyword.compare("rec") 		== 0 )	Interpreter->RecStA( arr );
		if ( keyword.compare("record") 		== 0 )	Interpreter->RecFile( arr );
		if ( keyword.compare("random") 		== 0 )	Interpreter->Random( arr );
		if ( keyword.compare("set") 		== 0 )	Interpreter->Set( arr );
		if ( keyword.compare("start") 		== 0 )	Interpreter->Start_bin( arr );
		if ( keyword.compare("stop") 		== 0 )	Interpreter->Stop_bin( arr );
		if ( keyword.compare("text")		== 0 )	Interpreter->Text( arr );

		if ( Interpreter->error > 0 )
		{
			Exception( "Compiler error with return code: " + to_string(  Interpreter->error ) );
		}
	}
	else // is not a keyword
	{
		if ( keyword[0] == ':' ) // is a call function
			return true;

		Interpreter->Addvariable( arr );

		if ( Interpreter->error > 0 )
		{
			Interpreter->Wrong_keyword( Keywords, keyword );
			return false;
		}
	}

	return true; // continue
}
