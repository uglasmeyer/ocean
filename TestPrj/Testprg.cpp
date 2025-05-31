#include <Appsymbols.h>
#include <Keyboard.h>
//#include <Exit.h>
//#include <notes/MusicXML.h>
//#include <notes/Notes.h>
//#include <Mixer.h>
//#include <data/SharedDataSegment.h>
#include <Time.h>
//Time_class Timer {};



//Musicxml_class 	MusicXML{};
//Note_class		Notes{};

typedef struct base_struct
{
	int 	p 	= 0;
	string	str	= "empty";
	base_struct( int a, string b ) :
		p( a ), str( b ) {};
	~base_struct() = default;
} base_t;
typedef struct derived_struct : base_struct
{
	int 	dp 	= 0;
	string	dstr	= "empty";
	derived_struct( base_t b ) :
		base_struct(b.p, b.str) {};
	~derived_struct() = default;
} derived_t;

Keyboard_base Keyboard {};
void exit_proc( int s )
{
	exit(0);
}
int main()
{
/*
	MusicXML.Notes.Set_Loglevel( DEBUG, true );
	Notes.musicxml = MusicXML.Xml2notelist( file_structure().Dir.xmldir + "second.musicxml" );
	MusicXML.Notes.Show_note_list( Notes.musicxml.notelist ); // @suppress("Invalid arguments")

	cout << "instrument_name: " <<  Notes.musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  Notes.musicxml.divisions <<endl;
	cout << "beats          : " <<  Notes.musicxml.beats<<endl;

*/
//	base_t 	base { 5, "five" };
	key3struct_t key { 0,0,0 };
	do
	{
	    std::this_thread::sleep_for( chrono::milliseconds(1000) );
	    key = Keyboard.GetKeystruct();
	    cout << dec << (int)key.key << ":" << (int)key.val0 << ":" << (int)key.val1<< endl;
	}
	while ( not (( key.key == ESC ) and ( key.val0 == 0 )));

//	derived_t derived{ base };
//	cout << derived.dp << derived.p << derived.str << derived.dstr << endl;
	return 0;
}
