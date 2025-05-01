/*
 * viewinterface.cpp
 *
 *  Created on: Apr 25, 2025
 *      Author: sirius
 */

#include <data/Interface.h>

	auto Lline = []( string s, auto v )
		{ cout << setw(40) << dec  << setfill('.') 	<< left << s << setw(40) << v << endl;};
	auto lline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left << s << setw(20) << v ; };
	auto rline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left <<s << setw(20) << v << endl;};
	auto conv_bool_s = []( bool b )
		{ return ( b ) ? string("yes") : string("no "); };


void ViewInterface_class::show_Que()
{
	cout << "Event que" << endl;
	for ( uint8_t ch : sds->deque )
	{
		cout.flush() << dec << setw(3) << (int)ch << ":" ;
	}
	cout << endl;
}
void ViewInterface_class::showSdsPage2()
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
	StA.AddColumn( "Id",  6);
	StA.AddColumn( "Play",6);
	StA.AddColumn( "Rec.",6);
	StA.AddColumn( "Amp.",6);
	StA.PrintHeader();
	for( uint n = 0; n <8; n++ )
		StA.AddRow( n, sds->StA_state[n].play, sds->StA_state[n].store, (int)sds->StA_amp_arr[n] );

	Table_class State { "Mixer State ", 0 };
	State.AddColumn( "Property",20 );
	State.AddColumn( "Value", 10 );
	State.AddColumn( "Property",20 );
	State.AddColumn( "Value", 10 );
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
//	std::cout << "\x1B[2J\x1B[H";
	switch ( nr ) {
		case F1 : { showSdsPage0();	break;}
		case F2 : { showSdsPage1();	break;}
		case F3 : { showSdsPage2();	break;}
		case F4 : { show_Que();		break;}
		case F5 : { show_Adsr();	break;}

		default: break;
	}
	cout << endl;
}

void ViewInterface_class::show_Adsr()
{
	Table_class Adsr{ "OSC Features",0 };
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
void ViewInterface_class::showSdsPage1()
{


	lline( "Mixer Volume:      " , (int)sds->MIX_Amp );
	rline( "Mixer Id           " , (int)sds->MIX_Id );

	lline( "Sync data id       " , (int) sds->SHMID);
	rline( "Event ID           " , Sds_p->Eventque.show());

	lline( "Record Progress   :" , (int)sds->RecCounter);
	rline( "File No.          :" , (int)sds->FileNo );

	lline( "(A)udioServer stat:" , Decode(sds->AudioServer));
	rline( "(C)omposer status :" , Decode(sds->Composer));

	lline( "(S)ynthesizer stat:" , Decode(sds->Synthesizer));
	rline( "(U)serinterface   :" , Decode(sds->UserInterface));

	lline( "Rtsp status       :" , Decode(sds->Rtsp));
	rline( "Data Mode         :" , Decode(sds->MODE));

	lline( "Instrument        :" , sds->Instrument);
	rline( "Wav filename      :" , sds->Other );

	lline( "Notes             :" , sds->Notes + NotesExtension[ sds->NotestypeId ] );
	rline( "Noteline duration :" , (int) sds->Noteline_sec);


}

void ViewInterface_class::printHeader()
{
	lline( "\nShared Data Str. ID ", to_string((int) sds->SDS_Id ) + ", "+ to_string( sds->config));
	rline( Version_str 			, sds->version);

}
void ViewInterface_class::showSdsPage0()
{
	auto frq_str = [ this ](uint8_t idx)
		{ return ( to_string( Frequency.GetFrq( idx ) ) + ", " + frqNamesArray[idx] ); };




	lline( "(M)ain (F)requency:" , frq_str( sds->OSC_wp.frqidx ));
	rline( "(A)DSR (G)lide freq:", (int)sds->OSC_wp.glide_effect);

	lline( "(M)aster(A)mplitude:", (int)sds->Master_Amp );
	rline( "(A)DSR (A)ttack:   " , (int)sds->OSC_adsr.attack );

	lline( "                   " , 0 );
	rline( "(A)DSR (B)eats Id  " , (int)sds->OSC_adsr.bps) ;

	lline( "(M)ain (W)aveform: " , waveform_str_vec[ (int)sds->OSC_spectrum.wfid[0] ]);
	rline( "(A)DSR (D)ecay:    " , (int)sds->OSC_adsr.decay );

	lline( "(F)MO  (F)requency:" , frq_str( sds->FMO_wp.frqidx ) );
	rline( "(V)CO  (F)requency:" , frq_str( sds->VCO_wp.frqidx ) );

	lline( "(F)MO  (A)mplitude:" , (int)sds->FMO_wp.volume);
	rline( "(V)CO  (A)mplitude:" , (int)sds->VCO_wp.volume);

	lline( "(F)MO  (W)aveform: " , waveform_str_vec[ (int)sds->FMO_spectrum.wfid[0] ]);
	rline( "(V)CO  (W)aveform: " , waveform_str_vec[ (int)sds->VCO_spectrum.wfid[0] ]);

	lline( "Time elapsed", (int)sds->time_elapsed );
	rline( "VCO  PMW dial      " , (int)sds->VCO_wp.PMW_dial) ;

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( SPEV, sds->OSC_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( SPEF, sds->OSC_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( SPEW, sds->OSC_spectrum ));

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( SPEV, sds->VCO_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( SPEF, sds->VCO_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( SPEW, sds->VCO_spectrum ));

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( SPEV, sds->FMO_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( SPEF, sds->FMO_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( SPEW, sds->FMO_spectrum ));




}
