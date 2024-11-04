
#include <notes/MusicXML.h>
#include <notes/Notes.h>

Musicxml_class MusicXML{};

int main()
{

	MusicXML.Notes.Set_Loglevel( DEBUG, true );
	MusicXML.Xml2notelist( file_structure().Dir.xmldir + "first.musicxml" );
	MusicXML.Notes.Show_note_list( MusicXML.notelist ); // @suppress("Invalid arguments")

//	cout << MusicXML.Noteline<<endl;

    return 0;
}
