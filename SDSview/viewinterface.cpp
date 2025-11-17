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
 * viewinterface.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Viewinterface.h>
#include <Keyboard.h>

/**************************************************
 * ViewInterface_class
 *************************************************/
ViewInterface_class::ViewInterface_class( APPID appid, Dataworld_class* DaTA_p )
	: Logfacility_class( "ViewInterface_class")
	, sdsstate_struct()
	, interface_struct()
	, Device_class( DaTA_p->sds_master )
	, ADSR_class()
	, Appstate( appid, DaTA_p->SDS.vec )
{
	className 		= Logfacility_class::className;
	this->DaTA		= DaTA_p;
	Sds_p			= DaTA->Sds_master;
	sds_master		= DaTA->sds_master;
	set_sds			( sds_master );
	ViewInterface_class::
	selfTest		();
};
void ViewInterface_class::selfTest()
{
	Comment( INFO, "selfTest ", className );
	for ( APPID appid : AppIds )
	{
		string state = Appstate.GetStateStr( sds_p, appid );
		coutf << state << endl;
	}
	coutf << (int)sds_p->spectrum_arr[OSCID].sum << endl;
	coutf << Show_spectrum_type( SPEV, sds_p->spectrum_arr[OSCID] );
}

void ViewInterface_class::show_Que()
{
	cout << "Event ptr first : " << (int)sds_p->eventptr.first << endl;
	cout << "          last  : " << (int)sds_p->eventptr.last  << endl;
	cout << "          length: " << (int)sds_p->eventptr.length << endl;
	string color = black;
	uint8_t pos = 0;
	for ( uint8_t ch : sds_p->deque )
	{
		if( pos == sds_p->eventptr.last)
			color = bblue;
		if( pos == sds_p->eventptr.first )
			color = bred;
		if( pos > sds_p->eventptr.last )
			color = black;
		cout.flush() << color << dec << setw(3) << (int)ch << ":" ;
		pos++;
	}
	cout.flush() << endcolor << endl;
	cout << "Wavedata [ 0 ... 20 ]" << endl;
	cout.precision(5);
	for( uint n = 0; n< 20; n++ )
		cout << sds_p->wavedata[n] << " " ;
	cout << " ..." << endl;
	Table_class Table { "Wavedisplay Status" };
	Table.AddColumn( "Feature", 20 );
	Table.AddColumn( "Value", 10 );
	Table.PrintHeader();
	Table.AddRow( "Role"	, roleNames[ sds_p->WD_status.roleId ] );
	Table.AddRow( "Osc"		, typeNames[ sds_p->WD_status.oscId ] );
	Table.AddRow( "Mode"	, wavedisplay_struct::types[ sds_p->WD_status.wd_mode ] );
	Table.AddRow( "FFT"		, wavedisplay_struct::fftmodes[ sds_p->WD_status.fftmode ] );
	Table.AddRow( "display curor"	, sds_p->WD_status.cursor );
}
void ViewInterface_class::show_Ipc()
{
	Set_Loglevel( TEST, true );
	DaTA->Sem_p->State( SEMNUM_SIZE );
	Set_Loglevel( TEST, false );
}

void ViewInterface_class::show_system()
{
	Table_class T { "Manage processes" };
	T.AddColumn( "Function.",25);
	T.AddColumn( "Key."		,15);
	T.PrintHeader();

	T.AddRow( "Shutdown all"			, "s/S" );
	T.AddRow( "Reset SDS"				, "r/R" );
	T.AddRow( "Destroy Shared Memory"	, "d/D" );
	T.AddRow( "Show log"				, "l/L" );


}
void ViewInterface_class::show_spectrum()
{

    string instrument {};
    instrument.assign( sds_p->Instrument );
    Info( "Instrument name: ", instrument );

    Table_class* T = Get_spectrum_table();
    T->PrintHeader();
	for ( Id_t oscid : oscIds )
	{
		Show_spectrum_table( nullptr, sds_p->spectrum_arr[oscid], false );
	}
	Table_class Adsr{ "OSC Features",15 };
	Adsr.AddColumn( "Feature"		, 20);
	Adsr.AddColumn( "Value"			, 6 );
	Adsr.AddColumn( "Feature"		, 20);
	Adsr.AddColumn( "Value"			, 6 );
	Adsr.PrintHeader();
	Adsr.AddRow( 	"(g)lide effect", (int)sds_p->features[OSCID].glide_effect,
					"(b)eats per second", (int)sds_p->adsr_arr[OSCID].bps );
	Adsr.AddRow( 	"PWM", (int)sds_p->features[OSCID].PMW_dial,
					"(h)all", (int)sds_p->adsr_arr[OSCID].hall );
	Adsr.AddRow( 	"Balance", (int)sds_p->mixer_balance,
					"chord delay", (int)sds_p->noteline_prefix.chord_delay );
	Adsr.AddRow( 	"Frq Slider Mode", slidermodes[ sds_p->frq_slidermode ] ) ;

}
void ViewInterface_class::show_Adsr()
{
	for ( Id_t oscid : oscIds )
	{
		adsr_arr[oscid].spec.frqidx[0] = sds_p->adsr_arr[oscid].bps;
		Show_spectrum_table( nullptr, adsr_arr[oscid].spec );
	}
	Table_class Adsr{ "OSC Features",15 };
	Adsr.AddColumn( "Feature", 20);
	Adsr.AddColumn( "Value", 6 );
	Adsr.AddColumn( "Feature", 20);
	Adsr.AddColumn( "Value", 6 );
	Adsr.PrintHeader();
	Adsr.AddRow( 	"OSC (a)ttack", (int)sds_p->adsr_arr[OSCID].attack,
					"OSC (d)ecay ", (int)sds_p->adsr_arr[OSCID].decay );
	Adsr.AddRow( 	"VCO (a)ttack", (int)sds_p->adsr_arr[VCOID].attack ,
					"VCO (d)ecay ", (int)sds_p->adsr_arr[VCOID].decay );
	Adsr.AddRow( 	"FMO (a)ttack", (int)sds_p->adsr_arr[FMOID].attack,
					"FMO (d)ecay ", (int)sds_p->adsr_arr[FMOID].decay );

}

void ViewInterface_class::F3_showStates()
{
	Table_class Table { "Connections", 20 };
	Table.AddColumn("OSC", 8 );
	Table.AddColumn("Frq", 6 );
	Table.AddColumn("Vol", 6 );
	Table.PrintHeader();
	for (uint oscid : oscIds )
	{
		connectName_t oscnames = Get_connection_names( oscid );
		Table.AddRow(typeNames[oscid], oscnames.frq, oscnames.vol );
	}

	Table_class StA{ "Storage Area", 20 };
	StA.AddColumn( "StA Id",  6);
	StA.AddColumn( "Id Name",  16);
	StA.AddColumn( "Play",6);
	StA.AddColumn( "Store.",6);
	StA.AddColumn( "Stored.",6);
	StA.AddColumn( "Forget", 6 );
	StA.AddColumn( "Amp.",6);
	StA.PrintHeader();

	for( STAID_e n : StAMemIds )
	{
		StA.AddRow( (int) n,
					StAIdName( n ),
					sds_p->StA_state_arr[n].play,
					sds_p->StA_state_arr[n].store,
					sds_p->StA_state_arr[n].filled,
					sds_p->StA_state_arr[n].forget,
					(int)sds_p->StA_amp_arr[n] );
	}
	Table_class T{ "Mixer State ", 20 };
	T.AddColumn( "Property",18 );
	T.AddColumn( "Value", 8 );
	T.AddColumn( "Property",18 );
	T.AddColumn( "Value", 8 );
	T.PrintHeader();
	T.AddRow( "External:"	, sds_p->mixer_state.external, "Mute:"	,sds_p->mixer_state.mute);
	T.AddRow( "Note:"		, sds_p->mixer_state.notes, "Keyboard:"	,sds_p->mixer_state.kbd);
	T.AddRow( "Syncronize:" , sds_p->mixer_state.sync,"Instrumemt:"	,sds_p->mixer_state.instrument);
	T.AddRow( "Record:" 	, sdsstateName( sds_p->Record_state ),""	,"");

}

void ViewInterface_class::showKeys()
{
	Table_class T{ "Navigation",20 };
	T.AddColumn( "Key", 10);
	T.AddColumn( "SDS Table", 15 );
	T.AddColumn( "|", 1 );

	T.AddColumn( "Key", 10);
	T.AddColumn( "SDS Table", 15 );
	T.PrintHeader();
	T.AddRow( "F1" , "more Keys"	, "|","F2", "OSCs" );
	T.AddRow( "F3" , "Processes"	, "|","F4", "States" );
	T.AddRow( "F5" , "Event Que"	, "|","F6", "Features" );
	T.AddRow( "F7" , "IPC"		, "|","F8", "Spectrum" );
	T.AddRow( "" , ""			, "|","", "" );

}
void ViewInterface_class::F2_showProcesses()
{
	for( APPID appid = AUDIOID; appid < NoAPPID; appid++ )
	{
		Appstate.CheckAppstate(sds_p, appid );
	}
	Table_class T{ "Process States"	, 15 };
	T.AddColumn( "Process"			, 15);
	T.AddColumn( "State"				, 15 );
	T.AddColumn( "PID"				, 15 );
	T.AddColumn( "Type Id"			, 10 );
	T.PrintHeader();
	for( APPID appid = AUDIOID;
			appid < NoAPPID;
			appid++ )
	{
		T.AddRow( AppIdName( (APPID)appid ),
								Appstate.GetStateStr( 	sds_p, appid ),
								Appstate.GetPidStr( sds_p, appid ),
								Appstate.Type(		sds_p, appid ));
	}
	coutf << Line(80) << endl;
	T.AddRow( 	"Mixer Id" 		, (int)sds_p->MIX_Id,
				"Sync data id" 	, (int) sds_p->SHMID);
	T.AddRow(	"Audio frames"	, (int)sds_p->audioframes,
				"Record Progress" , (int)sds_p->RecCounter);
	T.AddRow(	"File No."		, (int)sds_p->FileNo );
};

vector<string> typeNames_b { "(V)CO", "(F)MO", "(M)ain" };
void ViewInterface_class::showOSCs()
{
	auto frq_str = [ this ](uint8_t idx)
	{
		float	f	= GetFrq( idx );
		stringstream strs;
		strs << setprecision(5) << f;
		return strs.str() + ", " + frqNamesArray[idx] ;
	};

	string instr_name { sds_p->Instrument };
	Table_class T { "Instrument: " + instr_name };
	T.opt.Ident	= 0;

	T.AddColumn( "Oscillator"	, 20 );
	T.AddColumn( "Property name", 20 );
	T.AddColumn( "Value"		, 20 );
	T.AddColumn( "Unit"			, 5 );
	T.PrintHeader();
	for( OSCID_e oscid : oscIds )
	{
		T.AddRow( typeNames_b[oscid],
						"(F)requency"	, frq_str( sds_p->spectrum_arr[oscid].frqidx[0] ), "Hz" );
		T.AddRow( "", 	"(V)olume"	, (int) sds_p->spectrum_arr[oscid].volidx[0], "%" );
		T.AddRow( "", 	"(W)aveform"	, Get_waveform_str( sds_p->spectrum_arr[oscid].wfid[0])  );
	}
	cout << Line(80) << endl;
	T.AddRow( 	"Notes" , sds_p->Notes + NotesExtension[ sds_p->NotestypeId ],
				"Notes duration" , (int) sds_p->Noteline_sec);
	T.AddRow(	"Wav filename" , sds_p->Other );
	T.AddRow( 	"Prefix Octave", (int) sds_p->noteline_prefix.Octave,
				"Prefix shift",  (int) sds_p->noteline_prefix.octave_shift );
}

void ViewInterface_class::printHeader( KEYCODE keycode )
{
	string name = Keymap.Menu( keycode );
	Table_class T { name , 23  };
	T.AddColumn("Sds Id", 10);
	T.AddColumn("Config", 8 );
	T.AddColumn("Addres", 14 );
	T.AddColumn("Version", 8);
	T.PrintHeader();
	T.AddRow(	(int) sds_p->SDS_Id,
					(int) sds_p->config,
					to_hex( (long) sds_p ),
					Version_No);
	coutf<< Line (80) << endl;
}

void ViewInterface_class::IncCommitcounter()
{
	counter++;
}
void ViewInterface_class::SetFooter( const string& _footer )
{
	footer = _footer;
}

void ViewInterface_class::printFooter()
{
	const string counter_str = "Commit counter: " + to_string( counter ) + " ";

	coutf << setw(80) << setfill('-') << counter_str << endl;

	string exit_key = "<CrtlC>, <ESC>";
	string exit_txt = "exit SDSview";
	if ( footer.length() > 0 )
	{
		exit_key  	= "other key";
		exit_txt	= "return to more";
	}
	Table_class T{ defaultopt };
	T.AddColumn( "keyboard Key"	, 15 );
	T.AddColumn( "Description"	, 20 );
	T.AddColumn( "|"	, 1 );

	T.AddColumn( "keyboard Key"	, 15 );
	T.AddColumn( "Description"	, 20 );
	T.PrintHeader();
	T.AddRow( exit_key, exit_txt, "|", "F1","more keyboard keys" );
	T.AddRow( footer);
}

void ViewInterface_class::set_sds( interface_t* sds )
{
	this->sds_p = sds;
	Device_class::Set_sds( sds_p );
}
KEYCODE ViewInterface_class::ShowPage( interface_t* sds )
{
	set_sds( sds );

	ClearScreen();
	KEYCODE keycode = sds_master->sdsview_page;
	printHeader( keycode );

	switch ( keycode )
	{
		case F1 : { showKeys();			break;}
		case F2 : { F2_showProcesses();	break;}
		case F3 : { F3_showStates();	break;}
		case F4 : { show_Que();			break;}
		case F5 : { showOSCs();			break;}
		case F6 : { show_Adsr();		break;}
		case F7 : { show_spectrum();	break;}
		case F8 : { show_Ipc();			break;}
		case F9 : { show_system();		break;}

		default	: { break;}
	}
	printFooter();
	return sds_master->sdsview_page;
}


/**************************************************
 * Menue_class
 *************************************************/
#include <SDSMenu.h>
Menue_class::Menue_class(	Dataworld_class*	data,
							switch_fnc_t 		fnc,
							string 				footer ) :
	Logfacility_class("Menue_class"),
	ViewSds( SDSVIEWID, data )
{
	className 		= Logfacility_class::className;
	this->Switch 	= fnc;
	this->DaTA		= data;
	this->sdsid		= data->sds_master->config;
	this->sds		= data->SDS.GetSdsAddr( sdsid );
	this->sds_master= data->sds_master;
	this->page		= sds_master->sdsview_page;

	ViewSds.SetFooter( footer );
};
Menue_class::~Menue_class()
{
	DESTRUCTOR( className );
};
void Menue_class::Set_sdsid( int delta )
{
	int
	max 	= (MAXCONFIG - 1);
	sdsid 	= ( sdsid + delta );

	if ( sdsid < 0 ) sdsid = max;
	if ( sdsid > max ) sdsid = 0;
	if( delta == 0 )
		sdsid = sds_master->config;

	Interface_class*
	Sds 	= DaTA->SDS.GetSds( sdsid );
	sds 	= Sds->addr;
	DaTA->Appstate.Setup( sds, sds_master );
}

void Menue_class::Loop()
{
	kbdInt_t key 			= Switch( sds_master->sdsview_page );;
	while ( key != ESC )
	{
		ViewSds.ShowPage( sds );
		key 	= Key_event();
		switch( key )
		{
			case '+' 	: { Set_sdsid( 1 ); break;}
			case '-' 	: { Set_sdsid(-1 ); break;}
			default		: { key		= Switch( key ); break; }
		}
	}
}


kbdInt_t Menue_class::Key_event( )
{
	auto reset_updateFlag = [this  ](  )
	{
		sds->UpdateFlag 		= false;
		sds_master->UpdateFlag 	= false;
	};
	auto is_updateFlag = [this  ](  )
	{
		return ((sds->UpdateFlag ) 	or (sds_master->UpdateFlag ));
	};
	key3struct_t key {};
	while( key.Int == 0 )
	{
		key.Int = ViewSds.Kbd.GetKeyInt( true );
		this_thread::sleep_for( std::chrono::milliseconds(100) );
		if ( is_updateFlag() )
		{
			ViewSds.ShowPage( sds );
			reset_updateFlag();
		}
		if( ViewSds.Appstate.IsExitserver( sds_master, SDSVIEWID ) )
			raise( 1 );

	}
	return key.Int;
}
