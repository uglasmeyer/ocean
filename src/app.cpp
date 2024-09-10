/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#include <App.h>

/*
 * Synthesizer.cpp extensions
 */

void record_thead_fcn( 	Interface_class* SDS,
						External_class* External,
						Record_class* 	Record,
						bool*			SaveRecordFlag,
						bool*			RecordThreadDone )
{
	Logfacility_class Log("RecordThread");
	Log.Comment( INFO, "record thread started ");

	Value fileno {0};
	while ( not *RecordThreadDone ) 			// run until exit_proc or empty_exit_proc is called
	{
		if ( ( *SaveRecordFlag ) )				// triggered by RECORDWAVFILEKEY
		{
			fileno = (int) SDS->addr->FileNo;
			Log.Comment( INFO, "record thread received job " + fileno.str);

			External->Save_record_data( fileno.i );

			// clean up
			*SaveRecordFlag = false;
			Record->Unset();
			SDS->Update( RECORDWAVFILEFLAG ); 	// feedback to GUI

		}
	    std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	Log.Comment( INFO, "record thread terminated ");

}



#include <Mixer.h>
#include <Keyboard.h>

void SynthesizerTestCases()
{
	Logfacility_class Log("Synthesizer test");
	Loop_class Loop;
	String 					TestStr{""};
	Note_class 				Notes;
	Oscillator 				TestOsc{ TESTID };
	Interface_class			SDS;
	Instrument_class 		Instrument(SDS.addr );
	Mixer_class 			Mixer;
	Keyboard_class			Keyboard( &Instrument, &Mixer.StA[MbIdKeyboard] );
	External_class 			External( &Mixer.StA[ MbIdExternal] );
	Config_class* 			Cfg = new Config_class;

	Log.Init_log_file();
	Log.Test_Logging();

	Loop.Test();

	TestStr.test();

	Notes.Test();

	TestOsc.Test();

	Instrument.Test_Instrument();

	Mixer.Test_Logging();

	Keyboard.Test();

	External.test();

	Cfg->read_synthesizer_config( );
	External.Id3tool_cmd( Cfg->Get["Title"], Cfg->Get["Author"], Cfg->Get["Genre"], Cfg->Get["Album"]);
	string I = Cfg->Get["int"];
	cout << dec << atoi(I.data()) <<endl;
    Server_struct().TERM = Cfg->Get["TERM"];
    cout << Server_struct().cmd( Audio_Srv, "help") << endl;
    delete(Cfg);
}


/*
 *
 */



Application_class::Application_class( string name, uint id, uint8_t* status ) :
Logfacility_class("App")
{
	this->status_p 			= status;
	this->Name 				= name;
	this->This_Application 	= Application + Name + " " + Version_str;
	this->client_id 		= id;
	Comment( INFO, This_Application + " initialized ");
}

Application_class::~Application_class()
{
}

void Application_class::DeRegister( ifd_t* ifd )
{
	cout << endl;
	Comment( INFO, "De-register " + Name );
	*status_p = OFFLINE;
	if(( ifd->UserInterface != OFFLINE ) and  ( client_id == SYNTHID ) )
	{
		ifd->UserInterface = UPDATEGUI;
	}
	ifd->UpdateFlag = true;

	Comment( INFO, "Closing stderr");

	if ( redirect_stderr )
		std::fclose(stderr);

	std::ifstream cFile( file_structure().err_file );
    string out = "";
    do
    {
    	cout.flush() << out;

    } while( getline ( cFile, out ));
    cFile.close( );

}

void Application_class::Shutdown_instance( )
{
	if ( *status_p == RUNNING )
	{
		*status_p	= EXITSERVER;
		Comment( INFO, "Shutdown running instances of " + Name );
		long int 	max_wait 	= 2 * SECOND;
		long int 	amoment 	= 100 * MILLISECOND;
		int 		moments		= 0;
		while (( *status_p == EXITSERVER ) and ( amoment*moments < max_wait ))
		{
			Wait( amoment );
			Comment( WARN, "-" ) ;
			moments++;
		}

		if ( amoment * moments >= max_wait )
		{
			Comment( ERROR, "Giving up" );
			*status_p = RUNNING;
		}
	}
	else
	{
		Comment( INFO, "No other " + Name + " is running"	);
	}
}

void Application_class::Start()
{
	if (( client_id == GUI_ID ) or ( client_id == COMPID ) )
	{
		Init_log_file();
	}
	Comment(INFO, Line );
	Comment( INFO, "Entering application init for ");
	Comment( INFO, This_Application );
	Comment(INFO, Line );

	redirect_stderr = (bool) std::freopen( file_structure().err_file.data(), "w", stderr);
	if ( redirect_stderr )
		Comment( INFO, "Redirecting stderr");


}


