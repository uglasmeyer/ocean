

#include <Appsymbols.h>
#include <Kbd.h>
#include <Viewinterface.h>

ViewInterface_class		ViewSds			{ (char)SDSVIEWID, &DaTA };
Kbd_base				Keyboard		{};
Time_class				Timer			{};

int 					update_counter 	= 1;
int 					sdsid			= sds_master->config;
kbdInt_t				page			= F1;
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

kbdInt_t Key_event( string charstr )
{
	String Str{ charstr };
	set<char> charset = Str.Set;
	key3struct_t key { 0,0,0 };

	while ( not charset.contains( (char)key.Arr[0] ) )
	{
		this_thread::sleep_for( std::chrono::milliseconds(100));
		show_ifd();
		key.Int = App.KeyboardKey( false );//Keyboard.GetKeyInt( true );
	}
	return key.Int;
}

void while_MenuF9()
{
	kbdInt_t key3 = 0;
	while ( key3 != ESC )
	{

		key3 = Keyboard.GetKeyInt( true );
		Timer.Wait( 100, "ms" );
		switch ( key3 )
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
		} // switch  key3.Arr[0]
		if( tainted )
		{
			tainted = false;
			show_ifd();
			}
	}
	ViewSds.ShowPage( sds_master, F1 );
}

int main( int argc, char* argv[] )
{

	App.Start( argc, argv );

    App.Appstate->Announce();

    ViewSds.ShowPage(Sds->addr, F1);
	kbdInt_t keyevent = 0;
	while( true )
	{
		string charset { "afmvr+-\x1b\x7e" }  ;
		keyevent = Key_event( charset );
		cout  << keyevent << endl;
		switch ( keyevent )
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
			case ESC  : { exit_proc(0) ;break ;}
			case F1 : { page = F1 ;tainted = true;break ;}
			case F2 : { page = F2 ;tainted = true;break ;}
			case F3 : { page = F3 ; DaTA.Reg.Update_register(); tainted = true;break ;}
			case F4 : { page = F4 ;tainted = true;break ;}
			case F5 : { page = F5 ;tainted = true;break ;}
			case F6 : { page = F6 ;tainted = true;break ;}
			case F7 : { page = F7 ;tainted = true;break ;}
			case F8 : { page = F8 ;tainted = true;break ;}
			case F9 : { page = F9 ;tainted = true;break ;}
			default : { page = F1 ;tainted = true; break; }
			case 'r' :
			{	if( page != F3 ) break;
				cout << "Reset run state: ";
				kbdInt_t key = Key_event( "#acksu" );
				switch ( key )
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
				kbdInt_t keyevent = Key_event( "#faw" );
				switch ( keyevent )
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
				kbdInt_t keyevent = Key_event("#faw");
				switch ( keyevent )
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
				kbdInt_t keyevent = Key_event("#faw");
				switch ( keyevent )
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
				kbdInt_t keyevent = Key_event("#abgdh");

				switch ( keyevent )
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
		} // switch keyevent.Arr[0]
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
