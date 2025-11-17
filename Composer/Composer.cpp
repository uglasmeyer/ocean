//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/

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



