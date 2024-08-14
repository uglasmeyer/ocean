//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <common.h>
#include <kbd.h>
#include <keys.h>
#include <Logfacility.h>
#include <Interface.h>


using namespace std;

Kbd_class 				keyboard;
Logfacility_class 		Log("comstack");
Interface_class 		GUI;
ifd_t* 					ifd 			= GUI.addr;

key_struct_t 			keys			= key_struct();
string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;

void exit_proc( int signal )
{
	GUI.Announce("Comstack", false );

	keyboard.Reset();
	exit( signal );
}

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
	if ( ifd->UpdateFlag )
	{
		GUI.Show_interface();
		cout.flush() << "Exit with <#> or Ctrl c       " <<  "Commit counter " << update_counter;
		update_counter++;
		ifd->UpdateFlag = false;
	}

}

char Key_event( string charlist )
{
	keys.key = '$';

	while ( charlist.find( keys.key ) == string::npos )
	{
		Wait( 50*MILLISECOND);
		show_ifd();
		keys = keyboard.GetKey();
	}
	return keys.key;
}

int main( int argc, char* argv[] )
{

 	signal(SIGINT , &exit_proc );
	signal(SIGABRT, &exit_proc );

	GUI.Announce("Comstack", true);
 	GUI.Show_interface();
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
			case 'f' : { ifd->Main_Freq = getvalue( "Frequency" ); ifd->KEY = MAINFREQUENCYKEY; break; }
			case 'a' : { ifd->Master_Amp  = getvalue( "Amplitude" ); ifd->KEY = MASTERAMP_KEY; break; }
			case 'w' : { ifd->MAIN_spectrum.id  = getvalue( waveform_string ); ifd->KEY = SETWAVEFORMMAINKEY; break; }
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
			case 'f' : { ifd->FMO_Freq = getvalue( "Frequency" ); ifd->KEY = FMOFREQUENCYKEY; break; }
			case 'a' : { ifd->FMO_Amp  = getvalue( "Amplitude" ); ifd->KEY = FMOAMPKEY; break; }
			case 'w' : { ifd->FMO_spectrum.id  = getvalue( waveform_string ); ifd->KEY = SETWAVEFORMFMOKEY; break; }
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
			case 'f' : { ifd->VCO_Freq = getvalue( "Frequency" ); ifd->KEY = VCOFREQUENCYKEY; break; }
			case 'a' : { ifd->VCO_Amp  = getvalue( "Amplitude" ); ifd->KEY = VCOAMPKEY; break; }
			case 'w' : { ifd->VCO_spectrum.id  = getvalue( waveform_string ); ifd->KEY = SETWAVEFORMVCOKEY; break; }
			default  : break ;
			}
			break;
		}
		case 'a' :
		{
			cout << "ADSR ";
			keyevent = Key_event("#gdus");
			switch ( keyevent )
			{
			case 'g' : { ifd->Soft_freq = getvalue( "Frequency" ); ifd->KEY = SOFTFREQUENCYKEY; break; }
			case 'd' : { ifd->Main_adsr_attack  = getvalue( "Decay" ); ifd->KEY = ADSRDECAYKEY; break; }
			case 'u' : { ifd->Main_adsr_bps_id  = getvalue( "Duration" ); ifd->KEY = ADSRDURATIONKEY; break; }
			case 's' : { ifd->Main_adsr_decay  = getvalue( "Sustain" ); ifd->KEY = ADSRSUSTAINKEY; break; }
			default  : break ;
			}
			break;
		}
		default:
			break;
		}
	} // while key

	exit_proc(0);
}
