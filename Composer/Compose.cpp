
#include <Composer.h>
extern void ComposerTestCases();

int return_pos( int pos )
{
	if ( pos_stack.size() == 0 )
	{
		cout << dec << pos+1 << ": return without goto" << endl;
		raise( SIGINT  );
	}
	pos = pos_stack.back( );// access the last element
	pos_stack.pop_back();	// delete the last element
	return pos;
}

int call_pos( int pos, vector_str_t arr )
{
	pos_stack.push_back( pos );
	pos = Compiler.Find_position( &Program, arr );
	return pos;
}

bool check_input( string keyword )
{
	if( Keywords.contains( keyword ) ) return true;
	return false;
}

bool interpreter( )
{
	string 				cmd				{};
	string 				line			{};
	string				name			{};
	vector_str_t 		arr				{};

	uint 				pos 			= 0;
	uint 				line_no 		= 0;
	uint 				nr_of_filelines = Program.size();

	while ( ( pos >= 0 ) and (pos < nr_of_filelines ) )
	{
		line 			= Program[ pos ].line;
		line_no 		= Program[ pos ].no;
		name 			= Program[ pos ].name;
		cout << dec << setw(4) << line_no << ":" << left << setw(16) << name ;
		cout << setw( 60) <<left << line <<endl;
		if ( name.compare("stdin") == 0 )
			Compiler.Set_dialog_mode( true );
		else
			Compiler.Set_dialog_mode( false );

		Str.Str			= line;
		arr		 		= Str.to_bracket_array( '\"' );

		String Keyword 	= ( arr.size() > 0 ) ? arr[0] : "";
		Keyword.to_lower();
		string keyword 	= Keyword.Str;
		arr 			= Compiler.InsertVariable( arr );

		if ( check_input( keyword ) )
		{
			if ( keyword.compare("return") 		== 0 )	pos = return_pos ( pos );
			if ( keyword.compare("call") 		== 0 )	pos = call_pos( pos, arr );
			Compiler.Set_prgline( pos );

			if ( keyword.compare("add") 		== 0 )	Compiler.Add( arr );
			if ( keyword.compare("adsr") 		== 0 )	Compiler.Adsr( arr );
			if ( keyword.compare("exit") 		== 0 )	return Compiler.Exit();
			if ( keyword.compare("instrument") 	== 0 ) 	Compiler.Instrument( arr );
			if ( keyword.compare("notes") 		== 0 )	Compiler.Notes( arr );
			if ( keyword.compare("osc") 		== 0 )	Compiler.Osc( arr );
			if ( keyword.compare("pause") 		== 0 )	Compiler.Pause( arr );
			if ( keyword.compare("play") 		== 0 )	Compiler.Play( arr );
			if ( keyword.compare("rec") 		== 0 )	Compiler.RecStA( arr );
			if ( keyword.compare("record") 		== 0 )	Compiler.RecFile( arr );
			if ( keyword.compare("random") 		== 0 )	Compiler.Random( arr );
			if ( keyword.compare("set") 		== 0 )	Compiler.Set( arr );
			if ( keyword.compare("start") 		== 0 )	Compiler.Start_bin( arr );
			if ( keyword.compare("stop") 		== 0 )	Compiler.Stop_bin( arr );
			if ( keyword.compare("text")		== 0 )	Compiler.Text( arr );

			if ( Compiler.error > 0 )
			{
				if ( (Compiler.dialog_mode ) )
					continue;
				else
					exit( Compiler.error );
			}
		}
		else
		{
			if ( Compiler.error > 0 )
				return false ;

			if ( keyword[0] != ':' ) // is a function
			{
				Compiler.Addvariable( arr );
			}

			if ( Compiler.error > 0 )
			{
				Compiler.Wrong_keyword( Keywords, keyword );
				return false;
			}
		}
		pos++;
	} ;

	return true;

}

bool preprocessor( string batch_file )
{
	fstream 		File;
	string			line;

	auto std_arr = [ & ]( string line )
	{
		Str.Str = line;
		Str.replace_comment();
		Str.replace_char('\t' , ' ');
		return Str.to_unique_array( ' ' );
	};

	File.open( batch_file, fstream::in );
	if ( not File.is_open())
	{
		Log.Comment(ERROR, "Input file: " + batch_file + " does not exist" );
		raise (SIGINT );
	}
	else
	{
		Log.Comment( INFO, "Processing input file: \n" + batch_file  );
	}


	string path = filesystem::path( batch_file );
	vector_str_t arr	= {};

	uint lineno = 1;
	getline( File, line);
	do
	{
		arr = std_arr(line);
		String keyword = ( arr.size() > 0 ) ? arr[0] : "";;
		keyword.to_lower();
		if ( keyword.Str.compare( "include" ) == 0 )
		{
			string filename = ( arr.size() > 1) ? arr[1] : "";
			if ( filename.length() > 0 )
			{
				preprocessor( file_structure().includedir + filename + ".inc" );
			}
			else
			{
				Log.Comment( ERROR, "empty include clause" );
			}
		}
		else
		{
			if ( keyword.Str.length() != 0 )
			{
				line_struct_t prgline = {	.no		=lineno,
											.name	=filesystem::path(batch_file).stem(),
											.line	=Str.Str,
											.keyw 	= keyword.Str,
											.arg1 	= ""
				};

				Program.push_back( prgline );
			}
		}
		lineno++;
	}
	while( getline( File, line ));

	return true;
}

void composer_dialog()
{

	string line{};
	Compiler.Set_dialog_mode( true );
	while ( not strEqual( line, "exit" ) )
	{
		Program.clear();
		Compiler.Clear_stack();
		getline( cin, line );
		Program.push_back( { 1, "stdin", line } );
		if ( interpreter( ) )
		{
			Compiler.Execute(  );
		}
		cout << "echo: " << line << endl;
	}

}


void exit_proc( int signal )
{
	DaTA.Sds_p->Restore_ifd();
	DaTA.Appstate.RestoreState();
	exit(0);
}

int main( int argc, char* argv[] )
{
	auto headline = [ ]( string comment  )
	{
	    Log.Comment( BINFO, Line( 80 - 26 ) );
	    Log.Comment( BINFO, comment );
	    Log.Comment(  INFO, Line( 80 - 26) );
	};
	App.Start( argc, argv );

	DaTA.Appstate.SaveState();
	DaTA.Sds_p->Reset_ifd( );

	if ( DaTA.Cfg_p->Config.test == 'y' )
	{
		ComposerTestCases();
		Statistic.Show_Statistic(  );

		exit_proc( 0 );
	}


    App.Ready();

    headline( "Starting Pre-processor section" );
	if ( preprocessor( file_structure().program_file ) )
	{
	    headline( "Starting Interpreter section" );
		if ( interpreter( ) )
		{
		    headline( "Starting Processor section" );
			Compiler.Execute(  );
		}
	}

	if ( DaTA.Cfg_p->Config.dialog == 'y' )
	{
		composer_dialog();
	}

	exit_proc( 0);
}
