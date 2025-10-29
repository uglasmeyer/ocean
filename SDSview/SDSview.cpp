#include <Appsymbols.h>
void exit_proc( int s )
{
	Sem.Release( SEMAPHORE_EXIT );
	exit(0);
}

#include <Logfacility.h>
#include <Viewinterface.h>
#include <SDSMenu.h>

void setvalue( uint8_t* addr, string value )
{
	String Str { };
	*addr = (uint8_t)Str.to_int( value );

}
string getInput( string text )
{
	string s ;
	s 		= App.Kbd.GetString( text + ": ");
	return s;
}
void getvalue( uint8_t* addr, string text, EVENTKEY_t event )
{
	string s;
	s 		= getInput( text );
	setvalue( addr, s );
	Sds->Event( event );

}
void getfrq( uint8_t* addr, string text , EVENTKEY_t event )
{
	Frequency_class Frq {};
	string input = getInput( text );
	String Str{ input };
	if ( Str.is_number() )
	{
		setvalue( addr, input );
	}
	else
	{
		uint idx = Frq.Index( input );
		setvalue( addr, to_string(idx) );
	}
	Sds->Event( event );
}
#define SWITCH_COMMON( PAGE )\
	case ESC : { sds_master->sdsview_page = F1; break; };\
	case PAGE : {  sds_master->sdsview_page = PAGE; break; };\
	default : { sds_master->sdsview_page = F1; key = ESC; break; }

kbdInt_t SwitchF2( kbdInt_t key )
{
	switch ( toupper(key) )
	{
		case 'A': { App.Appstate->SetState( sds, AUDIOID	, sdsstate_struct::EXITSERVER);break; }
		case 'C': { App.Appstate->SetState( sds, COMPID 	, sdsstate_struct::EXITSERVER);break; }
		case 'S': { App.Appstate->SetState( sds, SYNTHID	, sdsstate_struct::EXITSERVER);break; }
		case 'K': { App.Appstate->SetState( sds, KEYBOARDID , sdsstate_struct::EXITSERVER);break; }
		case 'G': { App.Appstate->SetState( sds, GUI_ID 	, sdsstate_struct::EXITSERVER);break; }
		SWITCH_COMMON( F2 );
	} // switch  key


	return key;
}
Menue_class MenuF2 { &DaTA, SwitchF2, "Reset: (A)udioserver, (C)omposer, (S)ynthesizer, (K)eyboard, (G)ui" };

kbdInt_t SwitchF5( kbdInt_t key )
{
	switch ( (key) )
	{
	case 'M' :
	case 'm' :
	{
		cout << "Main ";
		kbdInt_t keyevent = App.Kbd.GetKeyInt( 100 );
		switch ( keyevent )
		{
			case 'f' : { getfrq( &sds->spectrum_arr[OSCID].frqidx[0], "Frequency", OSCFREQUENCYKEY ); break; }
			case 'v' : { getvalue( &sds->Master_Amp, "Amplitude", MASTERAMP_KEY ); break; }
			case 'w' : { getvalue( &sds->spectrum_arr[OSCID].wfid[0], show_range(waveform_range), SETWAVEFORMMAINKEY ); break; }
			default  : break ;
		}
		break;
	}
	case 'F' :
	case 'f' :
	{
		cout << "FMO ";
		kbdInt_t keyevent = App.Kbd.GetKeyInt( 100 );
		switch ( keyevent )
		{
			case 'f' : { getfrq( &sds->spectrum_arr[FMOID].frqidx[0], "Frequency", FMOFREQUENCYKEY ); break; }
			case 'v' : { getvalue( &sds->spectrum_arr[FMOID].volidx[0], "Amplitude", FMOAMPKEY ); break; }
			case 'w' : { getvalue( &sds->spectrum_arr[FMOID].wfid[0], show_range(waveform_range), SETWAVEFORMFMOKEY ); break; }
			default  : break ;
		}
		break;
	}
	case 'V' :
	case 'v' :
	{
		cout << "VCO ";
		kbdInt_t keyevent = App.Kbd.GetKeyInt( 100 );
		switch ( keyevent )
		{
			case 'f' : { getfrq( &sds->spectrum_arr[VCOID].frqidx[0], "Frequency", VCOFREQUENCYKEY ); break; }
			case 'v' : { getvalue( &sds->spectrum_arr[VCOID].volidx[0], "Amplitude", VCOAMPKEY ); break; }
			case 'w' : { getvalue( &sds->spectrum_arr[VCOID].wfid[0], show_range(waveform_range), SETWAVEFORMVCOKEY ); break; }
			default  : break ;
		}
		break;
	}

		SWITCH_COMMON( F5 );
	} // switch  key

	return key;
}
Menue_class MenuF5 { &DaTA, SwitchF5, "select Oscillator first to edit property" };

kbdInt_t SwitchF7( kbdInt_t key )
{
	switch ( (key) )
	{	case 'g' :
		case 'G' : { getvalue( &sds->features[OSCID].glide_effect	, "effect"	, SOFTFREQUENCYKEY ); break; }
		case 'b' :
		case 'B' : { getvalue( &sds->adsr_arr[OSCID].bps  			, "Beats p.sec"	, ADSR_KEY ); break; }
		case 'h' :
		case 'H' : { getvalue( &sds->adsr_arr[OSCID].hall  			, "Hall"		, ADSR_KEY ); break; }
		SWITCH_COMMON( F7 );
	} // switch  key
	return key;
}
Menue_class MenuF7 { &DaTA, SwitchF7, "select feature to edit" };

kbdInt_t SwitchF9( kbdInt_t key )
{
	switch ( key )
	{
		case 's' : { 	App.Appstate->Shutdown_all( DaTA.SDS.vec );
						break; }
		case 'd' : { 	App.Appstate->Shutdown_all( DaTA.SDS.vec );
						for( Interface_class Sds : DaTA.SDS.Vec )
						{
							Remove_file( Sds.dumpFile );
						}
						DaTA.SDS.Delete();
						System_execute( "ipcs -a", false );
						break; }
		case 'r' : {	App.Appstate->Shutdown( sds, SYNTHID );
						App.Appstate->Shutdown( sds, KEYBOARDID );
						App.Appstate->SaveStateArr( sds );
						Sds->Reset_ifd();
						App.Appstate->RestoreStateArr( sds );
						break; }
		case 'l' : {	System_execute_bg( "xterm -e tail -f " + fs.nohup_file );
						break; }
		SWITCH_COMMON( F9 );
	} // switch  key

	return key;
}

Menue_class MenuF9 { &DaTA, SwitchF9, "see keys above" };


kbdInt_t SwitchMain( kbdInt_t key )
{	// convet key to KEYCODE_t or exit

	switch (key)
	{
		case ESC: { exit_proc(0) ;break ;}
		case F1 : { sds_master->sdsview_page = F1; break;}
		case F2 : { sds_master->sdsview_page = F2; MenuF2.Loop(); break ;}
		case F3 : { sds_master->sdsview_page = F3; break;}
		case F4 : { sds_master->sdsview_page = F4; break;}
		case F5 : { sds_master->sdsview_page = F5; MenuF5.Loop(); break;}
		case F6 : { sds_master->sdsview_page = F6; break;}
		case F7 : { sds_master->sdsview_page = F7; MenuF7.Loop(); break;}
		case F8 : { sds_master->sdsview_page = F8; break;}
		case F9 : { sds_master->sdsview_page = F9; MenuF9.Loop(); break ;}
		default : { break; }
	} // switch key
	return key;
}
Menue_class MenuMain{ &DaTA, SwitchMain, "" };

int main( int argc, char* argv[] )
{
	App.Start( argc, argv );

	MenuMain.Loop();
	return 0;

}
