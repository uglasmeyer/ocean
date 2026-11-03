/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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

#include <Appsymbols.h>
#include <Oscwaveform.h>
#include <Logfacility.h>
#include <Viewinterface.h>
#include <SDSMenu.h>


void ClearIPC()
{
	DaTA.Appstate.Shutdown_all	();
	for( SharedData_class& Sds : DaTA.SDS.Vec )
	{
		Sds.Remove_dumpFile();
		Sds.SHM.Delete();
//			Sds.SHM.Detach( Sds.SHM.shm_ds.addr );
	}
	System_execute( "ipcs", false );
	raise( SIGINT );
}

template <typename T>
void setvalue( T* addr, string value )
{
	String Str { };
	*addr = (T)Str.to_int( value );

}
string getInput( string text )
{
	string s ;
	s 		= App.Kbd.GetString( text + ": ");
	return s;
}
template <typename T>
void getvalue( T* addr, string text, EVENTKEY_e event )
{
	string s;
	s 		= getInput( text );
	setvalue( addr, s );
	Sds->Eventque.Add( event );

}

void getwf( string text , OSCID_e oscid )
{
	string	help	= show_items( waveform->waveform_Id_set );
	string	input	= getInput( text + "(" + help + ")" ) ;
	String	Str		{ input };
	if ( Str.is_number() )
	{
//		Ifb.Set_Sds( sds_master->sdsview_sdsid );
//		sds_p = Ifb.sds_p;
		sds_p->spectrum_arr[oscid].wfid[0]	= WFID_e( Str.secure_stoi() );
		sds_p->SpectrumOscId= oscid;
		Sds->Eventque.Add	( WAVEFORMKEY );
	}
}
void getfrq( uint8_t* addr, string text , OSCID_e oscid )
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
	sds_p->SpectrumOscId = oscid;
	Sds->Eventque.Add( FREQUENCYKEY );
}

#define SWITCH_COMMON( PAGE )\
	case ESC : { sds_master->sdsview_page = F1	; 			break; };\
	case PAGE: { sds_master->sdsview_page = PAGE; 			break; };\
	default  : { sds_master->sdsview_page = F1	; key = ESC;break; }

kbdInt_t SwitchF2( kbdInt_t key )
{
	interface_t* sds = DaTA.SDS.GetSdsAddr( DaTA.SDS.master->sdsview_sdsid );
	if( key < 255 )
		key	= kbdInt_t( toupper( key ) );
	switch ( toupper(key) )
	{
		case 'A': { App.Appstate->SetExitserver( sds, AUDIOID	);break; }
		case 'C': { App.Appstate->SetExitserver( sds, COMPOSERID);break; }
		case 'S': { App.Appstate->SetExitserver( sds, SYNTHID	);break; }
		case 'K': { App.Appstate->SetExitserver( sds, KEYBOARDID);break; }
		case 'G': { App.Appstate->SetExitserver( sds, GUI_ID 	);break; }
		SWITCH_COMMON( F2 );
	} // switch  key
	return key;
}
const string submenu_strF2 = "Reset: (A)udioserver, (C)omposer, (S)ynthesizer, (K)eyboard, (G)ui";
Menue_class MenuF2 { &DaTA, SwitchF2, submenu_strF2 };

kbdInt_t SwitchF5( kbdInt_t key )
{
	sds_p = DaTA.SDS.GetSdsAddr( DaTA.SDS.master->sdsview_sdsid );
	switch ( (key) )
	{
	case 'M' :
	case 'm' :
	{
		cout << "Main ";
		kbdInt_t keyevent = App.Kbd.GetKeyInt( 100 );
		switch ( keyevent )
		{
			case 'f' : { getfrq( &sds_p->spectrum_arr[OSCID].frqidx[0], "Frequency", OSCID ); break; }
			case 'v' : { getvalue( &sds_p->Master_Amp, "Amplitude", MASTERAMP_KEY ); break; }
			case 'w' : { getwf( "Waveform", OSCID ); break; }
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
			case 'f' : { getfrq( &sds_p->spectrum_arr[FMOID].frqidx[0], "Frequency", FMOID ); break; }
			case 'v' : { getvalue( &sds_p->spectrum_arr[FMOID].volidx[0], "Amplitude", FMOAMPKEY ); break; }
			case 'w' : { getwf( "Waveform", FMOID ); break; }
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
			case 'f' : { getfrq( &sds_p->spectrum_arr[VCOID].frqidx[0], "Frequency", VCOID ); break; }
			case 'v' : { getvalue( &sds_p->spectrum_arr[VCOID].volidx[0], "Amplitude", VCOAMPKEY ); break; }
			case 'w' : { getwf( "Waveform", VCOID ); break; }
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
	interface_t* sds = DaTA.SDS.GetSdsAddr( DaTA.SDS.master->sdsview_sdsid );
	if( key < 255 )
		key	= kbdInt_t( toupper( key ) );
	switch ( key )
	{
		case 'G' : { getvalue( &sds->features[OSCID].frq_slide_value, "effect"		, FREQDYNAMIC_KEY ); break; }
		case 'B' : { getvalue( &sds->adsr_arr[OSCID].bps  			, "Beats p.sec"	, ADSR_KEY ); break; }
		case 'H' : { getvalue( &sds->adsr_arr[OSCID].hall  			, "Hall"		, ADSR_KEY ); break; }
		SWITCH_COMMON( F7 );
	} // switch  key
	return key;
}
Menue_class MenuF7 { &DaTA, SwitchF7, "select feature to edit" };

kbdInt_t SwitchF9( kbdInt_t key )
{
	interface_t* sds = DaTA.SDS.GetSdsAddr( DaTA.SDS.master->sdsview_sdsid );
	switch ( key )
	{
		case 's' : { 	App.Appstate->Shutdown_all();
						break; }
		case 'd' : { 	App.Appstate->Shutdown_all();
						for( SharedData_class Sds : DaTA.SDS.Vec )
						{
							Sds.Remove_dumpFile();
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
		case 'l' : {	System_execute_bg( Cfg.Logviewer( Cfg_p->fs->nohup_logfile ), "" );
						break; }
		case 'c' : {	System_execute_bg( Cfg.Logviewer( Cfg_p->fs->composer_logfile ), "" );
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
	App.Ready();

	if( Cfg_p->Config.clearipc == 'y' )
	{
		ClearIPC();
	}

	MenuMain.Set_sdsid( Menue_class::UNCHANGED );

	MenuMain.Loop();

}

void exit_proc( int s )
{
//	Sem.Release( SEMAPHORE_EXIT );
//	Appstate->SetExitserver( sds_master, SDSVIEWID );
	exit(0);
}
