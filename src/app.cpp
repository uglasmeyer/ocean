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
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


/*
 * Synthesizer.cpp extensions
 */



#include <App.h>
#include <Version.h>


Application_class::Application_class( Dataworld_class* _DaTA ) :
		Logfacility_class( "Application_class" ),
		Statistic_class( _DaTA->Appstate.Name )
{
	this->ProgramName			= _DaTA->Appstate.Name;
	this->className				= Logfacility_class::className ;
	this->DaTA					= _DaTA;
	this->Cfg 					= DaTA->Cfg_p;
	this->sds_master			= DaTA->sds_master;
	this->Sds					= DaTA->GetSds( );
	this->sds					= Sds->addr;
	this->AppId					= DaTA->AppId;
	this->Appstate				= &DaTA->Appstate;
	this->This_Application 		= Application + ProgramName + " " + Version_str;
	Comment( INFO, This_Application + " initialized ");
}


Application_class::~Application_class()
{
	if ( LogMask[ TEST ] )
	{
		System_execute( " cat /tmp/log/Synthesizer.log 1>&2 " );
	}
	else
	{
		if ( ( AppId == KEYBOARDID ) or ( AppId == SYNTHID ) )
			Sds->Dump_ifd();
	}

	deRegister();

	DESTRUCTOR( className );

}
void Application_class::versionTxt()
{
	fstream File;
	File.open( Cfg->fs->version_txt, fstream::out );
	File << "Version: " << Version_No << endl;
	File.close();
}

void Application_class::app_properties()
{
	properties.logowner		= logowner.contains( AppId );
	properties.start_once	= Appstate->assignMasterSds.contains( AppId );
	properties.data_process	= Cfg->Process.data_process ;
	properties.keyboard		= Cfg->Process.keyboard;
	properties.pid			= Appstate->getPid( sds, AppId );
	properties.Show();
}

void Application_class::Start( int argc, char* argv[] )
{

	Info( "Using OCEADIR=", Cfg->fs->installdir );
	Cfg->CreateInstalldirs();

	Timer.TimeStamp();
	Cfg->Parse_argv(argc, argv );

	Appstate->Announce();
	app_properties();

	if ( logowner.contains( AppId ) )
	{
		Init_log_file();
	}
	Info(Line() );
	Info("Entering application init for \n", This_Application );
	Info(Line() );

	versionTxt();

	Cfg->Show_Config( false );
}


void Application_class::deRegister( )
{
	Timer.TimeStamp();

	pid_t thispid 	= getpid();
	pid_t sdspid 	= DaTA->Appstate.getPid( sds, AppId );
	if ( thispid != sdspid ) return;

	DaTA->Appstate.SetOffline( );//this->sds, this->AppId );


	if ( this->AppId != GUI_ID )
		if( DaTA->Appstate.IsRunning( sds_master, GUI_ID)  )
		{
			DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );
		}
	DaTA->Sem_p->Release( SEMAPHORE_EXIT );
}

void Application_class::Ready(  )
{
	Show_Statistic( );
	Info( "SDS ID: ", (int) DaTA->SDS_Id );
	cout << Line() << endl;
	DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );
    DaTA->Sem_p->Release( SEMAPHORE_STARTED );
    Info( Cfg->prgName, License_text, "\n" );

    string exit_hint = ", press <ESC> to exit";
    if ( AppId == GUI_ID )
    {
    	exit_hint = "";
    }
	Comment(INFO, "ready", exit_hint );

}

kbdInt_t Application_class::KeyboardKey( bool debug )
{
	kbdInt_t key = Kbd.GetKeyInt( debug );
	if( key == ESC )
	{
	    Info( "Exit by user requst <ESC>" );
		Appstate->SetExitserver( sds, AppId );
	}
	return key;

}

