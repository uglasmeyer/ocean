//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <App.h>
#include <EventKeys.h>
#include <Frequency.h>
#include <Keyboard.h>
#include <Viewinterface.h>

#include <Appsymbols.h>

Keyboard_class			Keyboard		{};
ViewInterface_class		ViewSds			{ COMSTACKID, &DaTA };
int 					sdsid			= sds_master->config;

Frequency_class 		Frequency 		{};

int 					update_counter 	= 1;
uint					page			= F1;
bool 					tainted			= false;
const string			Module			= Log.className;


void usage( )
{

	Table_class Usage{ defaultopt };
	Usage.AddColumn( "keyboard Key"	, 15 );
	Usage.AddColumn( "Description"	, 20 );
	Usage.AddColumn( "keyboard Key"	, 15 );
	Usage.AddColumn( "Description"	, 20 );
	Usage.PrintHeader();
	Usage.AddRow( "<CrtlC>, <ESC>", "exit Comstack", "F1","more keyboard keys" );
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
		uint idx = Frequency.Index( input );
		setvalue( addr, to_string(idx) );
	}
	Sds->Event( event );
}

void show_ifd()
{
	if (( sds->UpdateFlag ) or (sds_master->UpdateFlag ))
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
	ViewSds.Appstate.Setup( sds, DaTA.sds_master );
	setupdate_flag( true );
}

key3struct_t Key_event( string charstr )
{
	String Str{ charstr };
	set<char> charset = Str.Set;
	key3struct_t key = Keyboard_base::key3_struct( 0,0,0 );

	while ( not charset.contains( (char)key.key ) )
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		if ( DaTA.sds_master->Comstack == state_struct::EXITSERVER )
			exit_proc( 0 );
		show_ifd();
		key = Keyboard.GetKeystruct( false );
	}

	return key;
}

void exit_proc( int signal )
{
	exit( signal );
}
void SpecialKey()
{
	key3struct_t special = Keyboard.GetKeystruct( false );
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
		default : { page = F1 ;break; }
	}

}
int main( int argc, char* argv[] )
{

	App.Start( argc, argv );

    DaTA.Appstate.Announce();

    ViewSds.ShowPage(Sds->addr, F1);
	key3struct_t keyevent { 0,0,0 };
	while( true )
	{
		string charset { "afmvr+-\x1B\x7E" }  ;
		keyevent = Key_event( charset );
		switch ( keyevent.key )
		{
			case '+' : { set_sdsid( 1); tainted = true; break; }
			case '-' : { set_sdsid(-1); tainted = true; break;}
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
					case F0 : { SpecialKey();break;}
					default : { page = F1; break; }
				}
				break;
			}

			case 'r' :
			{
				cout << "Reset run state: ";
				key3struct_t key = Key_event( "#acsu" );
				switch ( key.key )
				{
				case 'a': { DaTA.Appstate.Set( sds, AUDIOID, state_struct::EXITSERVER);break; }
				case 'c': { DaTA.Appstate.Set( sds, COMPID , state_struct::EXITSERVER);break; }
				case 's': { DaTA.Appstate.Set( sds, SYNTHID, state_struct::EXITSERVER);break; }
				case 'u': { DaTA.Appstate.Set( sds, GUI_ID , state_struct::EXITSERVER);break; }
				default : { break; }
				}
				tainted = true;
				break;
			}
			case 'm' :
			{
				cout << "Main ";
				key3struct_t keyevent = Key_event( "#faw" );
				switch ( keyevent.key )
				{
				case 'f' : { getfrq( &sds->OSC_wp.frqidx, "Frequency", OSCFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->Master_Amp, "Amplitude", MASTERAMP_KEY ); break; }
				case 'w' : { getvalue( &sds->OSC_spectrum.wfid[0], show_range(waveform_range), SETWAVEFORMMAINKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'f' :
			{
				cout << "FMO ";
				key3struct_t keyevent = Key_event("#faw");
				switch ( keyevent.key )
				{
				case 'f' : { getfrq( &sds->FMO_wp.frqidx, "Frequency", FMOFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->FMO_wp.volume, "Amplitude", FMOAMPKEY ); break; }
				case 'w' : { getvalue( &sds->FMO_spectrum.wfid[0], show_range(waveform_range), SETWAVEFORMFMOKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'v' :
			{
				cout << "VCO ";
				key3struct_t keyevent = Key_event("#faw");
				switch ( keyevent.key )
				{
				case 'f' : { getfrq( &sds->VCO_wp.frqidx, "Frequency", VCOFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->VCO_wp.volume, "Amplitude", VCOAMPKEY ); break; }
				case 'w' : { getvalue( &sds->VCO_spectrum.wfid[0], show_range(waveform_range), SETWAVEFORMVCOKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'a' :
			{
				std::cout << "ADSR ";
				key3struct_t keyevent = Key_event("#abgdh");
				switch ( keyevent.key )
				{
				case 'g' : { getvalue( &sds->OSC_wp.glide_effect, "Frequency", SOFTFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->OSC_adsr.attack	,"Atack", ADSR_KEY ); break; }
				case 'b' : { getvalue( &sds->OSC_adsr.bps  	, "Beats p.sec", ADSR_KEY ); break; }
				case 'd' : { getvalue( &sds->OSC_adsr.decay	,"Decay" , ADSR_KEY ); break; }
				case 'h' : { getvalue( &sds->OSC_adsr.hall  ,"Hall", ADSR_KEY ); break; }
				default  : break ;
				}
				break;
			}

			default:
				break;
		}
		if ( tainted )
		{
			tainted = false;
			setupdate_flag( true );
			show_ifd();
		}

	} // while key

	return 0;
}
