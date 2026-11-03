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
#include <fstream>

Application_class::Application_class( Dataworld_class* _data ) :
		Logfacility_class	( "Application_class" ),
		Interface_base		( _data ),
		Statistic_class		( _data->Appstate.Name )
{

	this->ProgramName			= _data->Appstate.Name;
	this->Cfg 					= _data->Cfg_p;
	this->AppId					= _data->AppId;
	this->Appstate				= &_data->Appstate;
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
	properties.start_once	= Appstate->UniqueRunApp.contains( AppId );
	properties.data_process	= Cfg->Process.data_process ;
	properties.keyboard		= Cfg->Process.keyboard;
	properties.pid			= Appstate->GetPid( sds_p, AppId );
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
	pid_t sdspid 	= DaTA_p->Appstate.GetPid( sds_p, AppId );
	if ( thispid != sdspid ) return;

	DaTA_p->Appstate.SetOffline( );//this->sds, this->AppId );

	if ( this->AppId != GUI_ID )
		if( DaTA_p->Appstate.IsRunning( sds_master, GUI_ID)  )
		{
			DaTA_p->EmitEvent( APPSTATE_FLAG, ProgramName );
		}
	DaTA_p->Sem_p->Release( SEMAPHORE_EXIT );
}

void Application_class::Ready(  )
{
	Show_Statistic( );
	Info( "SDS ID: ", (int) DaTA_p->SDS_Id );
	cout << Line() << endl;
	DaTA_p->EmitEvent( APPSTATE_FLAG, ProgramName );
    DaTA_p->Sem_p->Release( SEMAPHORE_STARTED );
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
	if( not is_atty )
		return NOKEY;
	kbdInt_t key = Kbd.GetKeyInt( debug );
	switch ( key )
	{
		case KEYCODE::ESC :
		{
		    Info( "Exit by user request <ESC>" );
			Appstate->SetExitserver( sds_p, AppId );
			break;
		}
		case NOKEY :
		{
			break;
		}
		default :
		{
			sds_p->Kbd_state.key = key;
			break;
		}
	} // switch  key

	return key;

}

SDSId_t Application_class::StartApp( APPID_e		appid,
									string 			terminal,
									string 			args )
{
	string opt = args;
	string binary = fs->archbindir + AppMap_struct().AppIdName( appid );
	string cmd = DaTA_p->Cfg_p->Server_cmd( terminal, binary, opt );
	if( strEqual( terminal, "nohup"  ) )
	{
		int ret = System_execute_bg( binary, fs->nohup_logfile );	// comId will be defined by startup App
		sds_p->logState = ( ret > 0 )	? INFO	: ERROR;
	}
	else
	{
		sds_p->logState = System_execute( cmd );
	}
	if( sds_p->logState == INFO )
		DaTA_p->Sem_p->Lock( SEMAPHORE_STARTED, 2 ); 	// wait for comId
	return sds_master->comId;

}
SDSId_t Application_class::ChangeAppState(	interface_t* 	sds,
										APPID_e			appid,
										string 			terminal,
										string 			args )
{
	if( Appstate->UniqueRunApp.contains( appid ) )
		sds = sds_master;
	if( Appstate->IsRunning( sds_master, RTSPID ) )
	{
		if ( DaTA_p->Sem_p->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
			DaTA_p->Sem_p->Release( SYNTHESIZER_START );
		return sds_master->comId;
	}
	if( Appstate->IsRunning( sds, appid ) )
	{
		Appstate->SetExitserver( sds, appid ) ;
		SDSId_t sdsid = sds_master->comId;
		if( Appstate->MultiRunApp.contains( appid ) )
		{
			DaTA_p->Sem_p->Lock	( SEMAPHORE_EXIT );
			sdsid 				= Appstate->Find_running( SYNTHID );
			Set_masterComId		( sdsid );
		}
		return sdsid;
	}

	return StartApp( appid, terminal, args );
}

