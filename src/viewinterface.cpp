/*
 * viewinterface.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: sirius
 */

#include <Viewinterface.h>
#include <Keyboard.h>

	auto Lline = []( string s, auto v )
		{ cout << setw(40) << dec  << setfill('.') 	<< left << s << setw(40) << v << endl;};
	auto lline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left << s << setw(20) << v ; };
	auto rline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left <<s << setw(20) << v << endl;};

ViewInterface_class::ViewInterface_class( char appid, Dataworld_class* DaTA_p )
	: Logfacility_class( "ViewInterface_class")
	, osc_struct()
	, sdsstate_struct()
	, interface_struct()
	, Device_class( DaTA_p->sds_master )
	, ADSR_class()
	, Appstate( appid, DaTA_p->sds_master, DaTA_p->sds_master, &DaTA_p->Reg )
{
	className 		= Logfacility_class::className;
	this->DaTA		= DaTA_p;
	Sds_p			= DaTA_p->Sds_master;
	sds_p			= DaTA_p->sds_master;
	selfTest		();
};
void ViewInterface_class::selfTest()
{
	Comment( INFO, "selfTest ", className );
	for ( uint appid = 0; appid < Appstate.appId_range.max; appid++ )
	{
		uint8_t state = Appstate.Get( sds_p, appid );
		coutf << Decode( state ) << endl;
	}
	coutf << (int)sds_p->spectrum_arr[OSCID].sum << endl;
	coutf << Show_spectrum_type( SPEV, sds_p->spectrum_arr[OSCID] );
}
string ViewInterface_class::Decode( Id_t idx)
{
	uint8_t id = idx % ( sdsstate_struct::LASTNUM );
	return sdsstate_struct().state_map[ id ];
}

void ViewInterface_class::show_Que()
{
	cout << "Event que" << endl;
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
	cout << "Event ptr first : " << (int)sds_p->eventptr.first << endl;
	cout << "          last  : " << (int)sds_p->eventptr.last  << endl;
	cout << "          length: " << (int)sds_p->eventptr.length << endl << endl;
	cout << "Wavedata [ 0 ... 20 ]" << endl;
	cout.precision(5);
	for( uint n = 0; n< 20; n++ )
		cout << sds_p->wavedata[n] << " " ;
	cout << " ..." << endl;
	Table_class Table { "Wavedisplay Status" };
	Table.AddColumn( "Feature", 20 );
	Table.AddColumn( "Value", 10 );
	Table.PrintHeader();
	Table.AddRow( "Role", osc_struct().roles[ sds_p->WD_status.roleId ] );
	Table.AddRow( "Osc"	, osc_struct().oscNames[ sds_p->WD_status.oscId ] );
	Table.AddRow( "Mode", wavedisplay_struct().types[ sds_p->WD_status.wd_mode ] );
	Table.AddRow( "FFT"	, wavedisplay_struct().fftmodes[ sds_p->WD_status.fftmode ] );
}
void ViewInterface_class::show_Ipc()
{
	Set_Loglevel( TEST, true );
	DaTA->Sem_p->State( SEMNUM_SIZE );
	Set_Loglevel( TEST, false );
//	system_execute( "ipcs" );
}

void ViewInterface_class::show_manage()
{
	Table_class Manage { "Manage processes" };
	Manage.AddColumn( "Function.",25);
	Manage.AddColumn( "Key.",15);
	Manage.PrintHeader();

	Manage.AddRow( "Shutdown all", "s" );
	Manage.AddRow( "Destroy Shared Memory", "d" );

}
void ViewInterface_class::show_spectrum()
{
    vector<spectrum_t> 	sds_spectrum_vec 	= {
    							sds_p->spectrum_arr[VCOID],
								sds_p->spectrum_arr[FMOID],
								sds_p->spectrum_arr[OSCID]};

	for ( spectrum_t& spec : sds_spectrum_vec )
	{
		coutf << Show_spectrum( spec );
	}
}
void ViewInterface_class::showStates()
{
	Table_class Table { "Connections", 20 };
	Table.AddColumn("OSC", 8 );
	Table.AddColumn("Frq", 6 );
	Table.AddColumn("Vol", 6 );
	Table.PrintHeader();
//	cout << to_hex( (long)Device_class::sds_p );
	for (uint oscid : oscIds )
	{
		connectName_t oscnames = Get_connection_names( oscid );
		Table.AddRow(oscNames[oscid], oscnames.frq, oscnames.vol );
	}

	Table_class StA{ "Storage Area", 20 };
	StA.AddColumn( "StA Id",  6);
	StA.AddColumn( "Id Name",  16);
	StA.AddColumn( "Play",6);
	StA.AddColumn( "Store.",6);
	StA.AddColumn( "Stored.",6);
	StA.AddColumn( "Amp.",6);
	StA.PrintHeader();
	for( uint n = 0; n <8; n++ )
		StA.AddRow( n,
					StAIdName( n ),
					sds_p->StA_state[n].play,
					sds_p->StA_state[n].store,
					sds_p->StA_state[n].filled,
					(int)sds_p->StA_amp_arr[n] );

	Table_class State { "Mixer State ", 20 };
	State.AddColumn( "Property",18 );
	State.AddColumn( "Value", 8 );
	State.AddColumn( "Property",18 );
	State.AddColumn( "Value", 8 );
	State.PrintHeader();
	State.AddRow( "External:"	, sds_p->mixer_status.external, "Mute:"	,sds_p->mixer_status.mute);
	State.AddRow( "Note:"		, sds_p->mixer_status.notes, "Keyboard:"	,sds_p->mixer_status.kbd);
	State.AddRow( "Syncronize:" , sds_p->mixer_status.sync,"Instrumemt:"	,sds_p->mixer_status.instrument);

}
void ViewInterface_class::ShowPage( interface_t* sds, int nr )
{
	this->sds_p = sds;
	Device_class::Set_sds( sds_p );

	ClearScreen();
	printHeader();

	switch ( nr )
	{
		case F1 : { showKeys();			break;}
		case F2 : { showOSCs();			break;}
		case F3 : { showProcesses();	break;}
		case F4 : { showStates();		break;}
		case F5 : { show_Que();			break;}
		case F6 : { show_Adsr();		break;}
		case F7 : { show_Ipc();			break;}
		case F8 : { show_spectrum();	break;}
		case F9 : { show_manage();		break;}

		default: break;
	}
	cout << endl;
}
void ViewInterface_class::showKeys()
{
	Table_class Keys{ "Navigation",20 };
	Keys.AddColumn( "Key", 10);
	Keys.AddColumn( "SDS Table", 15 );
	Keys.AddColumn( "Key", 10);
	Keys.AddColumn( "SDS Table", 15 );
	Keys.PrintHeader();
	Keys.AddRow( "F1" , "more Keys"	, "F2", "OSCs" );
	Keys.AddRow( "F3" , "Processes"	, "F4", "States" );
	Keys.AddRow( "F5" , "Event Que"	, "F6", "Features" );
	Keys.AddRow( "F7" , "IPC"		, "F8", "Spectrum" );
	Keys.AddRow( "" , ""			, "", "" );

}
void ViewInterface_class::show_Adsr()
{
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
	Adsr.AddRow( 	"(g)lide effect", (int)sds_p->features[OSCID].glide_effect,
					"(b)eats per second", (int)sds_p->adsr_arr[OSCID].bps );
	Adsr.AddRow( 	"(p)mw", (int)sds_p->features[OSCID].PMW_dial,
					"(h)all", (int)sds_p->adsr_arr[OSCID].hall );
	Adsr.AddRow( 	"(B)alance", (int)sds_p->mixer_balance,
					"chord delay", (int)sds_p->noteline_prefix.chord_delay );
	Adsr.AddRow( 	"Frq Slider Mode", slidermodes[ sds_p->frq_slidermode ] ) ;

    vector<adsr_t>	sds_adsrspec_vec	= {
    							sds_p->adsr_arr[VCOID],
    							sds_p->adsr_arr[FMOID],
								sds_p->adsr_arr[OSCID] };
	for ( adsr_t& adsr : sds_adsrspec_vec )
	{
		cout << Show_spectrum( adsr.spec );
		cout << "Bps: " << (int) adsr.bps << endl;
	}
}
void ViewInterface_class::showProcesses()
{

	Table_class Proc{ "Process States",20 };
	Proc.AddColumn( "Process", 20);
	Proc.AddColumn( "State", 10 );
	Proc.PrintHeader();
	for( uint appid = 0; appid < Appstate.appId_range.max; appid++ )
	{
		Proc.AddRow( AppIdName( appid ), Decode( Appstate.Get( sds_p, appid )) );
	}

	lline( "Mixer Id           " , (int)sds_p->MIX_Id );

	lline( "Sync data id       " , (int) sds_p->SHMID);
	rline( "Event ID           " , Sds_p->Eventque.show());

	lline( "Record Progress   :" , (int)sds_p->RecCounter);
	rline( "File No.          :" , (int)sds_p->FileNo );

	lline( "Instrument        :" , sds_p->Instrument);
	rline( "Wav filename      :" , sds_p->Other );

	lline( "Notes             :" , sds_p->Notes + NotesExtension[ sds_p->NotestypeId ] );
	rline( "Noteline duration :" , (int) sds_p->Noteline_sec);


}

void ViewInterface_class::printHeader()
{
	Table_class Table { "Shared Data Structure", 23  };
	Table.AddColumn("Sds Id", 10);
	Table.AddColumn("Config", 8 );
	Table.AddColumn("Addres", 14 );
	Table.AddColumn("Version", 8);
	Table.AddColumn("Sds Version", 12 );
	Table.PrintHeader();
	Table.AddRow(	(int) sds_p->SDS_Id,
					(int) sds_p->config,
					to_hex( (long) sds_p ),
					Version_No,
					(int) sds_p->version );

}
void ViewInterface_class::showOSCs()
{
	auto frq_str = [ this ](uint8_t idx)
		{ return ( to_string( GetFrq( idx ) ) + ", " + frqNamesArray[idx] ); };

	lline( "(M)ain (F)requency:" , frq_str( sds_p->spectrum_arr[OSCID].frqidx[0] ));
	rline( "(M)aster(A)mplitude:", (int)sds_p->Master_Amp );
	rline( "(M)ain (W)aveform: " , waveform_str_vec[ (int)sds_p->spectrum_arr[OSCID].wfid[0] ]);
	rline( "                   " , 0 );

	lline( "(F)MO  (F)requency:" , frq_str( sds_p->spectrum_arr[FMOID].frqidx[0] ) );
	rline( "(V)CO  (F)requency:" , frq_str( sds_p->spectrum_arr[VCOID].frqidx[0] ) );

	lline( "(F)MO  (A)mplitude:" , (int)sds_p->spectrum_arr[FMOID].volidx[0] );
	rline( "(V)CO  (A)mplitude:" , (int)sds_p->spectrum_arr[VCOID].volidx[0]);

	lline( "(F)MO  (W)aveform: " , waveform_str_vec[ (int)sds_p->spectrum_arr[FMOID].wfid[0] ]);
	rline( "(V)CO  (W)aveform: " , waveform_str_vec[ (int)sds_p->spectrum_arr[VCOID].wfid[0] ]);

	rline( "                   " , 0 );
	rline( "Audio frames", (int)sds_p->audioframes );






}
