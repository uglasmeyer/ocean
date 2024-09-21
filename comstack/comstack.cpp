//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <Keyboard.h>
#include <Logfacility.h>
#include <Interface.h>
#include <App.h>
#include <Keys.h>

using namespace std;

string					Module			= "comstack";
Logfacility_class 		Log( Module );
Keyboard_class			Keyboard;
Interface_class 		SDS;
ifd_t* 					sds 			= SDS.addr;
Application_class		App( Module, COMSTACKID, &SDS );

Keyboard_base::key_struct_t
						keys			= Keyboard_base::key_struct();
string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;


char get_char( string text )
{
	string s{};
	cout << text << ": ";
	cin >> s;
	return s[0];
}

int getvalue(string text )
{
	string s;
	cout << text << ": ";
	cin >> s;
	return stoi(s);

}

void show_ifd()
{
	if ( sds->UpdateFlag )
	{
		SDS.Show_interface();
		cout.flush() << "Exit with <#> or Ctrl c       " <<  "Commit counter " << update_counter;
		update_counter++;
		sds->UpdateFlag = false;
	}

}

char Key_event( string charlist )
{
	keys.key = '$';

	while ( charlist.find( keys.key ) == STRINGNOTFOUND )
	{
		this_thread::sleep_for(chrono::milliseconds(50));
//		Wait( 50*MILLISECOND );
		show_ifd();
		keys = Keyboard.GetKey();
	}
	return keys.key;
}

int main( int argc, char* argv[] )
{
	App.Start( argc, argv );

    SDS.Announce( App.client_id, &SDS.addr->Comstack );

 	SDS.Show_interface();
	cout << "Exit with <#> or Ctrl c" << endl ;

	keys.key = '$';
	char keyevent;
	while(  keys.key != '#' )
	{
		keyevent = Key_event("#mvfa");
		switch (keyevent)
		{
		case 'm' :
		{
			cout << "Main ";
			keyevent = Key_event("#faw");
			switch ( keyevent )
			{
			case 'f' : { sds->Main_Freq = getvalue( "Frequency" ); sds->KEY = MAINFREQUENCYKEY; break; }
			case 'a' : { sds->Master_Amp  = getvalue( "Amplitude" ); sds->KEY = MASTERAMP_KEY; break; }
			case 'w' : { sds->MAIN_spectrum.id  = getvalue( waveform_string ); sds->KEY = SETWAVEFORMMAINKEY; break; }
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
			case 'f' : { sds->FMO_Freq = getvalue( "Frequency" ); sds->KEY = FMOFREQUENCYKEY; break; }
			case 'a' : { sds->FMO_Amp  = getvalue( "Amplitude" ); sds->KEY = FMOAMPKEY; break; }
			case 'w' : { sds->FMO_spectrum.id  = getvalue( waveform_string ); sds->KEY = SETWAVEFORMFMOKEY; break; }
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
			case 'f' : { sds->VCO_Freq = getvalue( "Frequency" ); sds->KEY = VCOFREQUENCYKEY; break; }
			case 'a' : { sds->VCO_Amp  = getvalue( "Amplitude" ); sds->KEY = VCOAMPKEY; break; }
			case 'w' : { sds->VCO_spectrum.id  = getvalue( waveform_string ); sds->KEY = SETWAVEFORMVCOKEY; break; }
			default  : break ;
			}
			break;
		}
		case 'a' :
		{
			cout << "ADSR ";
			keyevent = Key_event("#gdush");
			switch ( keyevent )
			{
			case 'g' : { sds->Soft_freq = getvalue( "Frequency" ); sds->KEY = SOFTFREQUENCYKEY; break; }
			case 'd' : { sds->Main_adsr.attack  = getvalue( "Decay" ); sds->KEY = ADSR_KEY; break; }
			case 'u' : { sds->Main_adsr.bps_id  = getvalue( "Duration" ); sds->KEY = ADSR_KEY; break; }
			case 's' : { sds->Main_adsr.decay  = getvalue( "Sustain" ); sds->KEY = ADSR_KEY; break; }
			case 'h' : { sds->Main_adsr.decay  = getvalue( "Hall" ); sds->KEY = ADSR_KEY; break; }
			default  : break ;
			}
			break;
		}
		default:
			break;
		}
	} // while key

	return 0;
}
