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
#include <Composer/Composer.h>
#include <Default.h>

string select_program_file()
{
	file_structure* fs = Cfg.fs;
	Write_template_text( fs->prog_testfile, composer_testcases_template_text, OVERWRITE );
	Write_template_text( fs->prog_libfile, composer_lib_template_text, KEEPFILE );
	Write_template_text( fs->program_file, program_template_text, KEEPFILE );
	Write_template_text( fs->test_synth_file, composer_test_template_text, OVERWRITE );
    string program_name = sds_master->Session;
    if( Cfg.Config.filename.length() > 0 ) // argument 1
    {
    	// overwrite SDS session, Use instead program name from generic arg filename
    	program_name = Cfg.Config.filename;
    }
    return composerfiles.Select( program_name, Cfg.Fs.program_name );
}

auto headline = [ ]( string comment  )
{
	Log.Comment( BINFO, Line() );
	Log.Comment( BINFO, comment );
	Log.Comment(  INFO, Line() );
};




void composer_dialog()
{
	string line = "";
	CODE_e		code ; // subprogram return code
	Composer.Program.clear();

	Interpreter.Set_dialog_mode( true );
	headline( "Entering Composer dialog mode" );
	while ( true )
	{
		line = Edit.Line( line );
		coutf << endl;
		line_struct prgLine = line_struct( 0, line );
		string keyw = prgLine.keyw;
		if ( keyw.compare("exit")		== 0 )
			exit( 0 );

		code = EXECUTE;
		if ( keyw.compare("show")		== 0 )
			code = Composer.Show( prgLine.args );

		if ( keyw.compare("clear")		== 0 )
		{
			Composer.Program.clear();
			code = NOEXEC;
		}
		if ( keyw.compare("include") == 0 )
		{
			if ( Composer.PreCompile( Cfg_p->fs->includedir + prgLine.args[1]  ) )
			{
				Composer.Compile();
			}
			code = NOEXEC;
		}

		if ( code == EXECUTE ) // execute mode
		{
			if ( Composer.Interprete( prgLine) )
			{
				Composer.Program.push_back( prgLine );
				Composer.Interpreter->Execute(  );
			}
		}
	}

}

void ComposerTestCases()
{


	Write_template_text( Cfg_p->fs->testprogram_file ,composer_test_template_text, OVERWRITE );
	Write_template_text( Cfg_p->fs->prog_testfile, composer_testcases_template_text, OVERWRITE );

    headline( "Starting Pre-processor test section" );
	if ( Composer.PreCompile( Cfg_p->fs->testprogram_file ) )
	{
	    headline( "Starting Interpreter test section" );
		if ( Composer.Compile() )
		{
			Log.Info( "Composer Compile Test successful" );
		}
	}
	Interpreter.Test( );

	Processor_class Processor{ &App };
	Processor.Test_Processor();


	Processor.Set_Loglevel( TEST, true );
	Charset_class A("abdefabdef");
	A.test();

}

void ConfirmError()
{
	Log.Comment	( ERROR, "Press <return< to commit error" );
	App.Kbd.Reset();
	getc		( stdin );
}

void exit_proc( int signal )
{
	Log.Comment(INFO, "Entering exit procedure for " + Appstate->Name );
	sds_master->mixer_state.composer = false;

	if( signal == SIGILL )
		ConfirmError();

	exit(0);
}

int main( int argc, char* argv[] )
{
	App.Start( argc, argv );

	if ( DaTA.Cfg_p->Config.test == 'y' )
	{
		Composer.SetTestMode( true ); // ignore error - clear program
		ComposerTestCases();

		exit_proc( 0 );
	}


    App.Ready();
    sds_master->mixer_state.composer = true;
	if ( DaTA.Cfg_p->Config.dialog == 'y' )
	{
		composer_dialog();
		exit_proc( 0 );
	}

    Composer.Program_file = select_program_file();
    headline( "Starting Pre-processor section" );
    if ( Composer.PreCompile( Composer.Program_file ) )
	{
	    headline( "Starting Interpreter section" );
		if ( Composer.Compile() )
		{
			Composer.Show_Program( Composer.Program_file );
			Composer.Show_const();
		    App.Kbd.Reset(); // to make getc work
		    headline( "Starting Processor section" );
			Interpreter.Execute();
		}
		else
		{
			ConfirmError();
		}
	}
	else
		ConfirmError();


	exit_proc( 0);
}



