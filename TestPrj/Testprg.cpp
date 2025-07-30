#include <Synthesizer.h>


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
	SynthesizerTestCases();
	return 0;
}
