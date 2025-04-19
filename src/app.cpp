/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */


/*
 * Synthesizer.cpp extensions
 */



#include <App.h>
#include <Event.h>
#include <Config.h>
//#include <Mixer.h>
#include <Keyboard.h>
#include <Exit.h>
#include <Oscgroup.h>
#include <notes/MusicXML.h>



Application_class::Application_class( Dataworld_class* _DaTA ) :
		Logfacility_class( "Application_class" ),
		Statistic( _DaTA->Cfg.prgname )
{
	this->ProgramName			= _DaTA->Cfg.prgname;
	this->className				= Logfacility_class::className ;
	this->DaTA					= _DaTA;
	this->sds_master			= DaTA->sds_master;
	this->This_Application 		= Application + ProgramName + " " + Version_str;
	Comment( INFO, This_Application + " initialized ");

}

void Application_class::Init_Sds( )
{
	this->Sds		= DaTA->GetSds( );
	assert( this->Sds != nullptr );
	this->state_p	= DaTA->Appstate.ptr;

	this->sds		= Sds->addr;
}

void Application_class::VersionTxt()
{
	fstream File;
	File.open( file_structure().version_txt, fstream::out );
	File << "Version: " << Version_No << endl;
	File.close();
}

void Application_class::Start( int argc, char* argv[] )
{
    std::set<int> logowner =  { GUI_ID, COMPID, RTSPID };
	if ( logowner.contains( DaTA->AppId ) )
	{
		Init_log_file();
	}
	Info(Line );
	Info("Entering application init for ", This_Application );
	Info(Line );

	VersionTxt();

	redirect_stderr = false;//(bool) std::freopen( errFile.data(), "w", stderr);
	if ( redirect_stderr )
	{
		Info( "Redirecting stderr");
		fprintf( stderr, "%s\n", "error file content:\n");
	}

	this->Cfg = DaTA->Cfg_p;

	Cfg->Parse_argv(argc, argv );
	Cfg->Show_Config( );
	Init_Sds( );

	if ( DaTA->Cfg.Config.clear == 'y' )
	{
		DaTA->Reg.Clear_procregister();
		EXCEPTION( "Restart processes");
	}

}

Application_class::~Application_class()
{
	deRegister();

    if (( DaTA->AppId == SYNTHID ) and ( not LogMask[TEST] ))
	{
		Sds->Dump_ifd();
	}
}



void Application_class::deRegister( )
{
/*	auto closeStderr = [ this ]( string errFile )
	{
		Info2(1, "Closing stderr");

		if ( redirect_stderr )
			std::fclose(stderr);

		std::ifstream cFile( errFile );
	    string out = "";
	    do
	    {
	    	cout.flush() << out << endl;

	    } while( getline ( cFile, out ));
	    cout << endl;

	    cFile.close( );
	};
*/

	auto setState = [ this ](  )
	{
		cout << endl;
		Info( "De-register " , DaTA->Appstate.Name  );
		assert ( state_p != nullptr );
		*state_p 	= OFFLINE;

	};

	setState( );
	if(( sds_master->UserInterface == RUNNING ) )
	{
		DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );
	}
	sds->UpdateFlag = true;//	closeStderr( errFile ); TODO causes valgrind error
}

void Application_class::Ready(  )
{
	Statistic.Show_Statistic( );
	if ( state_p )
	{
		*state_p	= RUNNING;
	}
	Comment(INFO, ProgramName + " is ready");
	Info( "SDS ID: ", (int) DaTA->SDS_Id );
	cout << Line << endl;
	DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );

}

void Application_class::Shutdown_instance( )
{

	if ( *state_p == RUNNING )
	{
		*state_p	= EXITSERVER;
	}
	else
	{
		Info( "No other " , ProgramName , " is running"	);
	}
	Server_init = false;
}

