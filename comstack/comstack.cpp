//============================================================================
// Name        : comstack.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <common.h>
#include <Keyboard.h>
#include <keys.h>
#include <Logfacility.h>
#include <Interface.h>
#include <App.h>

using namespace std;

string					Module			= "comstack";
Logfacility_class 		Log( Module );
Keyboard_class			Keyboard;
Interface_class 		IFD;
ifd_t* 					ifd 			= IFD.addr;
Application_class		App( Module, COMSTACKID, &ifd->Comstack );

Keyboard_base::key_struct_t
						keys			= Keyboard_base::key_struct();
string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;

void exit_proc( int signal )
{
	App.Decline( &IFD.addr->UpdateFlag );
	Keyboard.Reset();
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
		IFD.Show_interface();
		cout.flush() << "Exit with <#> or Ctrl c       " <<  "Commit counter " << update_counter;
		update_counter++;
		ifd->UpdateFlag = false;
	}

}

char Key_event( string charlist )
{
	keys.key = '$';

	while ( charlist.find( keys.key ) == STRINGNOTFOUND )
	{
		Wait( 50*MILLISECOND );
		show_ifd();
		keys = Keyboard.GetKey();
	}
	return keys.key;
}

int main( int argc, char* argv[] )
{

 	signal(SIGINT , &exit_proc );
	signal(SIGABRT, &exit_proc );

    IFD.Announce( App.client_id, App.status );

 	IFD.Show_interface();
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
			keyevent = Key_event("#gdush");
			switch ( keyevent )
			{
			case 'g' : { ifd->Soft_freq = getvalue( "Frequency" ); ifd->KEY = SOFTFREQUENCYKEY; break; }
			case 'd' : { ifd->Main_adsr.attack  = getvalue( "Decay" ); ifd->KEY = ADSR_KEY; break; }
			case 'u' : { ifd->Main_adsr.bps_id  = getvalue( "Duration" ); ifd->KEY = ADSR_KEY; break; }
			case 's' : { ifd->Main_adsr.decay  = getvalue( "Sustain" ); ifd->KEY = ADSR_KEY; break; }
			case 'h' : { ifd->Main_adsr.decay  = getvalue( "Hall" ); ifd->KEY = ADSR_KEY; break; }
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
