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
#include <Composer/Composer.h>


auto headline = [ ]( string comment  )
{
	Log.Comment( BINFO, Line( 80 - 26 ) );
	Log.Comment( BINFO, comment );
	Log.Comment(  INFO, Line( 80 - 26) );
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
			if ( Composer.PreCompile( Cfg.fs->includedir + prgLine.args[1]  ) )
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

	Processor_class Processor{ &App };
	Processor.Test_Processor();
	Processor.Set_Loglevel( TEST, true );
	Interpreter.Test( );

	string arg = "-t";
	char* args = arg.data();
	App.Start(1, &args );
	Interpreter.Variation.Test();
	Charset_class A("abdefabdef");
	A.test();
}

void exit_proc( int signal )
{
	coutf;
	exit(0);
}

int main( int argc, char* argv[] )
{
	App.Start( argc, argv );

	if ( DaTA.Cfg_p->Config.test == 'y' )
	{
		ComposerTestCases();
		Statistic.Show_Statistic(  );

		exit_proc( 0 );
	}


    App.Ready();
	if ( DaTA.Cfg_p->Config.dialog == 'y' )
	{
		composer_dialog();
		exit_proc( 0 );
	}

    headline( "Starting Pre-processor section" );
	if ( Composer.PreCompile( Cfg.fs->program_file ) )
	{
	    headline( "Starting Interpreter section" );
		if ( Composer.Compile() )
		{
		    headline( "Starting Processor section" );
		    App.Kbd.Reset(); // to make getc work
			Interpreter.Execute(  );
		}
	}


	exit_proc( 0);
}



