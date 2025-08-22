//#include <Synthesizer.h>
#include <Appsymbols.h>
#include <App.h>
void exit_proc( int s )
{
	Log.Set_Loglevel( DBG2, true );
	Log.Info( "Exit Process");
	exit(0);
}
int main( int argc, char* argv[] )
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
	App.Start(argc, argv );

return 0;
}
