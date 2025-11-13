// License Notice
/******************************************************************************
-> Ocean Sound Lab Application Suite (short Ocean-SL)
Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
Linux based Operating Systems, that allows to generate, play and record sound.
It includes interfaces for musicxml-files and supports the sound drivers:
-> native ALSA and
-> Pulseaudio
The software includes a simple keyboard to play and record music as you type.
It comes with a limited set of sample files for instruments and notes to start
with. Funny play.

-> Ocean-SL GitHub site: https://github.com/uglasmeyer/ocean
-> Copyright (c) 2024-2025 Ulrich.Glasmeyer@web.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. Any person wishing to
distribute modifications to the Software is asked to send the modifications to
the original developer so that they can be incorporated into the canonical
version. This is, however, not a binding provision of this license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/

#include <Composer/Composer.h>


auto headline = [ ]( string comment  )
{
	Log.Comment( BINFO, Line( 80 - 26 ) );
	Log.Comment( BINFO, comment );
	Log.Comment(  INFO, Line( 80 - 26) );
};




void composer_dialog()
{

	string line{};
	Interpreter.Set_dialog_mode( true );
	headline( "Entering Composer dialog mode" );
	while ( not strEqual( line, "exit" ) )
	{
		cout << "Ocean command line> ";
		getline( cin, line );
		line_struct prgLine = line_struct( 0, line );
		if ( Composer.Interprete( prgLine) )
		{
			Interpreter.Execute(  );
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
	    App.Kbd.Reset(); // to make getc work
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



