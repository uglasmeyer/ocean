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

string ViewInterface_class::Decode( uint8_t idx)
{
	return state_map[ idx ];
}

void ViewInterface_class::show_Que()
{
	cout << "Event que" << endl;
	for ( uint8_t ch : sds->deque )
	{
		cout.flush() << dec << setw(3) << (int)ch << ":" ;
	}
	cout << endl;
}
void ViewInterface_class::show_Ipc()
{
	Set_Loglevel( TEST, true );
	DaTA->Sem_p->State( SEMNUM_SIZE );
	Set_Loglevel( TEST, false );
//	system_execute( "ipcs" );
}
void ViewInterface_class::showStates()
{
	Table_class Connect { "Connections", 20 };
	Connect.AddColumn("OSC", 6);
	Connect.AddColumn("Frq", 6);
	Connect.AddColumn("Amp", 6);
	Connect.PrintHeader();
	for (uint oscid : { FMOID, VCOID, OSCID })
		Connect.AddRow(types[oscid], (bool) (sds->connect[oscid].frq),
				(bool) (sds->connect[oscid].vol));

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
					sds->StA_state[n].play,
					sds->StA_state[n].store,
					sds->StA_state[n].filled,
					(int)sds->StA_amp_arr[n] );

	Table_class State { "Mixer State ", 20 };
	State.AddColumn( "Property",18 );
	State.AddColumn( "Value", 8 );
	State.AddColumn( "Property",18 );
	State.AddColumn( "Value", 8 );
	State.PrintHeader();
	State.AddRow( "External:"	, sds->mixer_status.external, "Mute:"	,sds->mixer_status.mute);
	State.AddRow( "Note:"		, sds->mixer_status.notes, "Keyboard:"	,sds->mixer_status.kbd);
	State.AddRow( "Syncronize:" , sds->mixer_status.sync,"Instrumemt:"	,sds->mixer_status.instrument);

}
void ViewInterface_class::ShowPage( interface_t* sds, int nr )
{
	this->sds = sds;

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
	Keys.AddRow( "F1" , "more Keys", "F2", "OSCs" );
	Keys.AddRow( "F3" , "Processes", "F4", "States" );
	Keys.AddRow( "F5" , "Event Que", "F6", "Features" );
	Keys.AddRow( "F7" , "IPC", "", "" );
	Keys.AddRow( "" , "", "", "" );

}
void ViewInterface_class::show_Adsr()
{
	Table_class Adsr{ "OSC Features",20 };
	Adsr.AddColumn( "Feature", 20);
	Adsr.AddColumn( "Value", 10 );
	Adsr.PrintHeader();
	Adsr.AddRow( "ADSR (a)ttack", (int)sds->OSC_adsr.attack );
	Adsr.AddRow( "ADSR (d)ecay", (int)sds->OSC_adsr.decay );
	Adsr.AddRow( "(g)lide effect", (int)sds->OSC_wp.glide_effect );
	Adsr.AddRow( "(b)eats per second", (int)sds->OSC_adsr.bps );
	Adsr.AddRow( "(p)mw", (int)sds->OSC_wp.PMW_dial );
	Adsr.AddRow( "(h)all", (int)sds->OSC_adsr.hall );
	Adsr.AddRow( "(B)alance", (int)sds->mixer_balance );
}
void ViewInterface_class::showProcesses()
{

	Table_class Proc{ "Process States",20 };
	Proc.AddColumn( "Process", 20);
	Proc.AddColumn( "State", 10 );
	Proc.PrintHeader();
	for( uint appid = 0; appid < Appstate.appId_range.max; appid++ )
	{
		Proc.AddRow( AppIdName( appid ), Decode( Appstate.Get( sds, appid )) );
	}
	Proc.AddRow( "Keyboard", bool_str( sds->Keyboard, Decode(RUNNING), Decode(OFFLINE) ) );

	lline( "Mixer Volume:      " , (int)sds->MIX_Amp );
	rline( "Mixer Id           " , (int)sds->MIX_Id );

	lline( "Sync data id       " , (int) sds->SHMID);
	rline( "Event ID           " , Sds_p->Eventque.show());

	lline( "Record Progress   :" , (int)sds->RecCounter);
	rline( "File No.          :" , (int)sds->FileNo );

	rline( "Data Mode         :" , Decode(sds->MODE));

	lline( "Instrument        :" , sds->Instrument);
	rline( "Wav filename      :" , sds->Other );

	lline( "Notes             :" , sds->Notes + NotesExtension[ sds->NotestypeId ] );
	rline( "Noteline duration :" , (int) sds->Noteline_sec);


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
	Table.AddRow(	(int) sds->SDS_Id,
					(int) sds->config,
					to_hex( (long) sds ),
					Version_No,
					(int) sds->version );

}
void ViewInterface_class::showOSCs()
{
	auto frq_str = [ this ](uint8_t idx)
		{ return ( to_string( GetFrq( idx ) ) + ", " + frqNamesArray[idx] ); };

	lline( "(M)ain (F)requency:" , frq_str( sds->OSC_wp.frqidx ));
	rline( "(M)aster(A)mplitude:", (int)sds->Master_Amp );
	rline( "(M)ain (W)aveform: " , waveform_str_vec[ (int)sds->OSC_spectrum.wfid[0] ]);
	rline( "                   " , 0 );

	lline( "(F)MO  (F)requency:" , frq_str( sds->FMO_wp.frqidx ) );
	rline( "(V)CO  (F)requency:" , frq_str( sds->VCO_wp.frqidx ) );

	lline( "(F)MO  (A)mplitude:" , (int)sds->FMO_wp.volume);
	rline( "(V)CO  (A)mplitude:" , (int)sds->VCO_wp.volume);

	lline( "(F)MO  (W)aveform: " , waveform_str_vec[ (int)sds->FMO_spectrum.wfid[0] ]);
	rline( "(V)CO  (W)aveform: " , waveform_str_vec[ (int)sds->VCO_spectrum.wfid[0] ]);

	rline( "                   " , 0 );
	rline( "Audio frames", (int)sds->audioframes );

	rline( "Spectrum volume    " , Show_spectrum_type( SPEV, sds->OSC_spectrum ));
	rline( "Spectrum frequency " , Show_spectrum_type( SPEF, sds->OSC_spectrum ));
	rline( "Spectrum wafeform  " , Show_spectrum_type( SPEW, sds->OSC_spectrum ));

	rline( "Spectrum volume    " , Show_spectrum_type( SPEV, sds->VCO_spectrum ));
	rline( "Spectrum frequency " , Show_spectrum_type( SPEF, sds->VCO_spectrum ));
	rline( "Spectrum wafeform  " , Show_spectrum_type( SPEW, sds->VCO_spectrum ));

	rline( "Spectrum volume    " , Show_spectrum_type( SPEV, sds->FMO_spectrum ));
	rline( "Spectrum frequency " , Show_spectrum_type( SPEF, sds->FMO_spectrum ));
	rline( "Spectrum wafeform  " , Show_spectrum_type( SPEW, sds->FMO_spectrum ));




}
