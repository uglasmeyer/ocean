

#include <Appsymbols.h>
#include <Kbd.h>
#include <Viewinterface.h>

ViewInterface_class		ViewSds			{ (char)SDSVIEWID, &DaTA };
Kbd_base				Keyboard		{};
Time_class				Timer			{};

int 					update_counter 	= 1;
int 					sdsid			= sds_master->config;
uint					page			= F1;
bool 					tainted			= false;

void usage( )
{
	Table_class Usage{ defaultopt };
	Usage.AddColumn( "keyboard Key"	, 15 );
	Usage.AddColumn( "Description"	, 20 );
	Usage.AddColumn( "|"	, 1 );

	Usage.AddColumn( "keyboard Key"	, 15 );
	Usage.AddColumn( "Description"	, 20 );
	Usage.PrintHeader();
	Usage.AddRow( "<CrtlC>, <ESC>", "exit", "|", "F1","more keyboard keys" );
}

void setupdate_flag( bool flag )
{
	sds->UpdateFlag 		= flag;
	sds_master->UpdateFlag 	= flag;
}
void setvalue( uint8_t* addr, string value )
{
	String Str { };
	*addr = (uint8_t)Str.to_int( value );

}
string getInput( string text )
{
	string s ;
	s 		= Keyboard.GetString( text + ": ");
	tainted = true;
	return s;
}
void getvalue( uint8_t* addr, string text, uint event )
{
	string s;
	s 		= getInput( text );
	setvalue( addr, s );
	Sds->Event( event );

}
void getfrq( uint8_t* addr, string text , uint event )
{
	string input = getInput( text );
	String Str{ input };
	if ( Str.is_number() )
	{
		setvalue( addr, input );
	}
	else
	{
		uint idx = ViewSds.Index( input );
		setvalue( addr, to_string(idx) );
	}
	Sds->Event( event );
}

void show_ifd()
{
	if ((sds->UpdateFlag ) 	or
		(sds_master->UpdateFlag ))
	{
		ViewSds.ShowPage( sds , page );
		Log.Info( "Commit counter " , update_counter );
		usage();


		setupdate_flag( false );
		update_counter++;
	}
}

void set_sdsid( int delta )
{
	int
	max 	= (MAXCONFIG - 1);
	sdsid 	= ( sdsid + delta );

	if ( sdsid < 0 ) sdsid = max;
	if ( sdsid > max ) sdsid = 0;
	if( delta == 0 )
		sdsid = DaTA.sds_master->config;

	Sds 	= DaTA.SDS.GetSds(sdsid);
	sds 	= Sds->addr;
	App.Appstate->Setup( sds, DaTA.sds_master );
	setupdate_flag( true );
}

void exit_proc( int signal )
{
	Log.Set_Loglevel( DBG2, true );
	exit( signal );
}

key3struct_t Key_event( string charstr )
{
	String Str{ charstr };
	set<char> charset = Str.Set;
	key3struct_t key = Kbd_base::key3_struct( 0,0,0 );

	while ( not charset.contains( (char)key.key ) )
	{
		this_thread::sleep_for( std::chrono::milliseconds(100));
		if ( DaTA.sds_master->Comstack == sdsstate_struct::EXITSERVER )
			exit_proc( 0 );
		show_ifd();
		key = Keyboard.GetKeystruct( true );
	}

	return key;
}

void while_MenuF9()
{
	key3struct_t key3 = Keyboard.Nullkey3;
//	Log.Set_Loglevel( DEBUG, true );
	while ( key3.key != ESC )
	{

		key3 = Keyboard.GetKeystruct( true );
		Timer.Wait( 100, "ms" );
		switch ( key3.key )
		{
			case 's' : { 	App.Appstate->Shutdown_all( DaTA.SDS.vec );
							tainted = true;
							break; }
			case 'd' : { 	App.Appstate->Shutdown_all( DaTA.SDS.vec );
							for( Interface_class Sds : DaTA.SDS.Vec )
							{
								Remove_file( Sds.dumpFile );
							}
							DaTA.SDS.Delete();
							DaTA.SHM_0.Stereo_buffer( Cfg.Config.SHM_keyl );
							DaTA.SHM_0.Delete() ;
							DaTA.SHM_1.Stereo_buffer( Cfg.Config.SHM_keyr );
							DaTA.SHM_1.Delete();
							System_execute( "ipcs -a", false );
							tainted = true;
							break; }
			case 'r' : {	App.Appstate->Shutdown( sds, SYNTHID );
							App.Appstate->Shutdown( sds, KEYBOARDID );
							App.Appstate->Shutdown( sds_master, AUDIOID );
							Sds->Reset_ifd();
							tainted = true;
							break; }
			default  : { 	break; }
		} // switch  key3.key
		if( tainted )
		{
			tainted = false;
			show_ifd();
			}
	}
	ViewSds.ShowPage( sds_master, F1 );
}
void SpecialKey()
{
	key3struct_t special = Keyboard.GetKeystruct( true );
	if ( special.val0 != ASC )
	{
		tainted = false;
		return;
	}
	tainted = true;
	switch( special.key )
	{
		case F5 : { page = F5 ;break ;}
		case F6 : { page = F6 ;break ;}
		case F7 : { page = F7 ;break ;}
		case F8 : { page = F8 ;break ;}
		case F9 : { page = F9 ;break ;}
		default : { page = F1 ;break; }
	}

}
int main( int argc, char* argv[] )
{

	App.Start( argc, argv );

    App.Appstate->Announce();

    ViewSds.ShowPage(Sds->addr, F1);
	key3struct_t keyevent { 0,0,0 };
	while( true )
	{
		string charset { "afmvr+-\x1B\x7E" }  ;
		keyevent = Key_event( charset );
		switch ( keyevent.key )
		{
			case '+' :
			{ 	set_sdsid( 1);
				tainted = true;
				break;
			}
			case '-' :
			{ 	set_sdsid(-1);
				tainted = true;
				break;
			}
			case ESC :
			{
				tainted = true;
				switch( keyevent.val1 )
				{
					case 0  : { exit_proc(0) ;break ;}
					case F1 : { page = F1 ;break ;}
					case F2 : { page = F2 ;break ;}
					case F3 : { page = F3 ; DaTA.Reg.Update_register(); break ;}
					case F4 : { page = F4 ;break ;}
					case S0 : { SpecialKey();break;}
					case S1 : { SpecialKey(); break; }
					default : { page = F1; break; }
				}
				break;
			}
			case 'r' :
			{	if( page != F3 ) break;
				cout << "Reset run state: ";
				key3struct_t key = Key_event( "#acksu" );
				switch ( key.key )
				{
					case 'a': { App.Appstate->Set( sds, APPID::AUDIOID, sdsstate_struct::EXITSERVER);break; }
					case 'c': { App.Appstate->Set( sds, APPID::COMPID , sdsstate_struct::EXITSERVER);break; }
					case 's': { App.Appstate->Set( sds, APPID::SYNTHID, sdsstate_struct::EXITSERVER);break; }
					case 'k': { App.Appstate->Set( sds, APPID::KEYBOARDID  , sdsstate_struct::EXITSERVER);break; }
					case 'g': { App.Appstate->Set( sds, APPID::GUI_ID , sdsstate_struct::EXITSERVER);break; }
					default : { break; }
				}
				tainted = true;
				break;
			}
			case 'm' :
			{	if( page != F2 ) break;
				cout << "Main ";
				key3struct_t keyevent = Key_event( "#faw" );
				switch ( keyevent.key )
				{
					case 'f' : { getfrq( &sds->spectrum_arr[osc_struct::OSCID].frqidx[0], "Frequency", OSCFREQUENCYKEY ); break; }
					case 'a' : { getvalue( &sds->Master_Amp, "Amplitude", MASTERAMP_KEY ); break; }
					case 'w' : { getvalue( &sds->spectrum_arr[osc_struct::OSCID].wfid[0], show_range(waveform_range), SETWAVEFORMMAINKEY ); break; }
					default  : break ;
				}
				break;
			}
			case 'f' :
			{	if( page != F2 ) break;
				cout << "FMO ";
				key3struct_t keyevent = Key_event("#faw");
				switch ( keyevent.key )
				{
					case 'f' : { getfrq( &sds->spectrum_arr[osc_struct::FMOID].frqidx[0], "Frequency", FMOFREQUENCYKEY ); break; }
					case 'a' : { getvalue( &sds->spectrum_arr[osc_struct::FMOID].volidx[0], "Amplitude", FMOAMPKEY ); break; }
					case 'w' : { getvalue( &sds->spectrum_arr[osc_struct::FMOID].wfid[0], show_range(waveform_range), SETWAVEFORMFMOKEY ); break; }
					default  : break ;
				}
				break;
			}
			case 'v' :
			{	if( page != F2 ) break;
				cout << "VCO ";
				key3struct_t keyevent = Key_event("#faw");
				switch ( keyevent.key )
				{
					case 'f' : { getfrq( &sds->spectrum_arr[osc_struct::VCOID].frqidx[0], "Frequency", VCOFREQUENCYKEY ); break; }
					case 'a' : { getvalue( &sds->spectrum_arr[osc_struct::VCOID].volidx[0], "Amplitude", VCOAMPKEY ); break; }
					case 'w' : { getvalue( &sds->spectrum_arr[osc_struct::VCOID].wfid[0], show_range(waveform_range), SETWAVEFORMVCOKEY ); break; }
					default  : break ;
				}
				break;
			}
			case 'a' :
			{	if( page != F6 ) break;
				std::cout << "ADSR ";
				key3struct_t keyevent = Key_event("#abgdh");

				switch ( keyevent.key )
				{
					case 'g' : { getvalue( &sds->features[osc_struct::OSCID].glide_effect	, "Frequency"	, SOFTFREQUENCYKEY ); break; }
					case 'a' : { getvalue( &sds->adsr_arr[osc_struct::OSCID].attack			,"Atack"		, ADSR_KEY ); break; }
					case 'b' : { getvalue( &sds->adsr_arr[osc_struct::OSCID].bps  				, "Beats p.sec"	, ADSR_KEY ); break; }
					case 'd' : { getvalue( &sds->adsr_arr[osc_struct::OSCID].decay				,"Decay" 		, ADSR_KEY ); break; }
					case 'h' : { getvalue( &sds->adsr_arr[osc_struct::OSCID].hall  			,"Hall"			, ADSR_KEY ); break; }
					default  : break ;
				}
				break;
			}
			default : break;
		} // switch keyevent.key
		if ( tainted )
		{
			tainted = false;
			setupdate_flag( true );
			show_ifd();
			switch ( page ) {
				case F9 : { while_MenuF9(); break; }
				default : { break; }
			}
		}
	} // while true
	return 0;
}
