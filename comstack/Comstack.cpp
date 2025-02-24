//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <Comstack.h>

using namespace std;
char get_char( string text )
{
	string s{};
	cout << text << ": ";
	cin >> s;
	return s[0];
}

bool tainted=false;
int getvalue(string text )
{
	String S {""};
	string s;
	cout << text << ": ";
	Keyboard.Reset();
	cin >> s;
	Keyboard.Init();
	tainted = true;
	return S.to_int(s);

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
				case 'f' : { sds->OSC_wp.frqidx = getvalue( "Frequency" ); sds->EVENT = OSCFREQUENCYKEY; break; }
				case 'a' : { sds->Master_Amp  = getvalue( "Amplitude" ); sds->EVENT = MASTERAMP_KEY; break; }
				case 'w' : { sds->OSC_spectrum.wfid[0]  = getvalue( waveform_string ); sds->EVENT = SETWAVEFORMMAINKEY; break; }
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
				case 'f' : { sds->FMO_wp.frqidx = getvalue( "Frequency" ); sds->EVENT = FMOFREQUENCYKEY; break; }
				case 'a' : { sds->FMO_wp.volume  = getvalue( "Amplitude" ); sds->EVENT = FMOAMPKEY; break; }
				case 'w' : { sds->FMO_spectrum.wfid[0]  = getvalue( waveform_string ); sds->EVENT = SETWAVEFORMFMOKEY; break; }
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
				case 'f' : { sds->VCO_wp.frqidx = getvalue( "Frequency" ); sds->EVENT = VCOFREQUENCYKEY; break; }
				case 'a' : { sds->VCO_wp.volume  = getvalue( "Amplitude" ); sds->EVENT = VCOAMPKEY; break; }
				case 'w' : { sds->VCO_spectrum.wfid[0]  = getvalue( waveform_string ); sds->EVENT = SETWAVEFORMVCOKEY; break; }
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
				case 'g' : { sds->OSC_wp.glide_effect 	= getvalue( "Frequency" ); sds->EVENT = SOFTFREQUENCYKEY; break; }
				case 'a' : { sds->OSC_adsr.attack  	= getvalue( "Atack" ); sds->EVENT = ADSR_KEY; break; }
				case 'b' : { sds->OSC_adsr.bps  		= getvalue( "Beats p.sec" ); sds->EVENT = ADSR_KEY; break; }
				case 'd' : { sds->OSC_adsr.decay  		= getvalue( "Decay" ); sds->EVENT = ADSR_KEY; break; }
				case 'h' : { sds->OSC_adsr.hall  		= getvalue( "Hall" ); sds->EVENT = ADSR_KEY; break; }
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
