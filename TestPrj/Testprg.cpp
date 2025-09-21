//#include <Synthesizer.h>
#include <Appsymbols.h>
#include <data/Sdsbase.h>

void exit_proc( int s )
{
	Log.Set_Loglevel( DBG2, true );
	Log.Info( "Exit Process");
	exit(0);
}
#include <Kbd.h>
Kbd_base Kbd {};
Time_class Time {};
int main( int argc, char* argv[] )
{
	Kbd_base::kbdkey_t	key(1,2,3);//= Kbd.key3_struct(0,0,0);
	for( int i =0;i<4;i++)
		cout << (int)key.Arr[i]<< endl;
	key.Int = 0x44454647;
	Kbd.ShowKey( 0x01000031L );
	Kbd.ShowKey( 0x01000036L );
	for( int i =0;i<4;i++)
		cout << (char)key.Arr[i]<< endl;


	while( key.Int != KEYCODE::ESC )
	{
		key.Int = Kbd.GetKeyInt( true );
		Time.Wait(1);
	}
	/*
	MusicXML.Notes.Set_Loglevel( DEBUG, true );
	Notes.musicxml = MusicXML.Xml2notelist( file_structure().Dir.xmldir + "second.musicxml" );
	MusicXML.Notes.Show_note_list( Notes.musicxml.notelist ); // @suppress("Invalid arguments")

	cout << "instrument_name: " <<  Notes.musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  Notes.musicxml.divisions <<endl;
	cout << "beats          : " <<  Notes.musicxml.beats<<endl;

*/


return 0;

}
