#include <Appsymbols.h>
#include <notes/MusicXML.h>
#include <notes/Notes.h>

void exit_proc( int s )
{
//	Log.Set_Loglevel( DBG2, true );
	Log.Info( "Exit Process");
	exit(0);
}

Musicxml_class Music {};

int main( int argc, char* argv[] )
{
	App.Start( argc, argv );
	Note_class Notes{};
	Notes.musicxml = Music.XmlFile2notelist( "test" );
	Notes.Show_note_list( Notes.musicxml.notelist );
	Music.Notelist2xmlFile( "test", Notes.musicxml.notelist );

	exit_proc(0);
return 0;

}
