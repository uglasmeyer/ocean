//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <Comstack.h>

using namespace std;


bool tainted=false;
void getvalue( uint8_t* addr, string text, uint event )
{
	String S {""};
	string s;
	cout << text << ": ";
	Keyboard.Reset();
	cin >> s;
	Keyboard.Init();
	tainted = true;
	*addr = (uint8_t)S.to_int(s);
	Sds->Event( event );

}

void show_ifd()
{
	if (( sds->UpdateFlag ) or (sds_master->UpdateFlag ))
	{
		Sds = DaTA.GetSds(sdsid);
		sds->UpdateFlag = false;
		sds_master->UpdateFlag = false;
		Sds->Show_interface();

		cout.flush() << "Exit with <#> or Ctrl c       " <<  "Commit counter " << update_counter;
		sds = DaTA.GetSdsAddr( sdsid );

		update_counter++;
	}
}

void set_sdsid( int delta )
{
	sdsid = ( sdsid + delta ) % MAXCONFIG;
	if ( sdsid < 0 ) sdsid = MAXCONFIG - 1;
	if( delta == 0 )
		sdsid = DaTA.sds_master->config;
	sds->UpdateFlag = true;
}

char Key_event( string charlist )
{
	String Str{ charlist };
	key_struct_t key = key_struct();
	while ( not Str.Set.contains( key.key )  )
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		show_ifd();
		key = Keyboard.GetKey();
	}
	return key.key;
}

void exit_proc( int signal )
{
	exit( signal );
}
int main( int argc, char* argv[] )
{

	App.Start( argc, argv );

    App.Sds->Announce( );

    App.Sds->Show_interface();
	cout << "Exit with <#> or Ctrl c" << endl ;
	char keyevent = '$';
	while( true )
	{
		keyevent = Key_event( "#mvfa+-" );
		switch (keyevent)
		{
			case '+' :
			{
				set_sdsid(1); break;
			}
			case '-' :
			{
				set_sdsid(-1); break;
			}
			case 'm' :
			{
				cout << "Main ";
				keyevent = Key_event( "#faw" );
				switch ( keyevent )
				{
				case 'f' : { getvalue( &sds->OSC_wp.frqidx, "Frequency", OSCFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->Master_Amp, "Amplitude", MASTERAMP_KEY ); break; }
				case 'w' : { getvalue( &sds->OSC_spectrum.wfid[0], waveform_string, SETWAVEFORMMAINKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'f' :
			{
				cout << "FMO ";
				keyevent = Key_event("#faw");
				switch ( keyevent )
				{
				case 'f' : { getvalue( &sds->FMO_wp.frqidx, "Frequency", FMOFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->FMO_wp.volume, "Amplitude", FMOAMPKEY ); break; }
				case 'w' : { getvalue( &sds->FMO_spectrum.wfid[0], waveform_string, SETWAVEFORMFMOKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'v' :
			{
				cout << "VCO ";
				keyevent = Key_event("#faw");
				switch ( keyevent )
				{
				case 'f' : { getvalue( &sds->VCO_wp.frqidx, "Frequency", VCOFREQUENCYKEY ); break; }
				case 'a' : { getvalue( &sds->VCO_wp.volume, "Amplitude", VCOAMPKEY ); break; }
				case 'w' : { getvalue( &sds->VCO_spectrum.wfid[0], waveform_string, SETWAVEFORMVCOKEY ); break; }
				default  : break ;
				}
				break;
			}
			case 'a' :
			{
				cout << "ADSR ";
				keyevent = Key_event("#abgdh");
				switch ( keyevent )
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
			Sds->Show_interface();
		}
		if ( keyevent == Keyboard.AppExit )
			exit_proc( 0 );
	} // while key

	return 0;
}
