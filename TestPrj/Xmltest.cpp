
#include <notes/MusicXML.h>
#include <notes/Notes.h>
#include <Exit.h>

Musicxml_class MusicXML{};
Note_class	Notes{};

int main()
{
	ASSERTION( 1 == 2, "False", 1 , 1);
	MusicXML.Notes.Set_Loglevel( DEBUG, true );
	Notes.musicxml = MusicXML.Xml2notelist( file_structure().Dir.xmldir + "second.musicxml" );
	MusicXML.Notes.Show_note_list( Notes.musicxml.notelist ); // @suppress("Invalid arguments")

	cout << "instrument_name: " <<  Notes.musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  Notes.musicxml.divisions<<endl;
	cout << "beats          : " <<  Notes.musicxml.beats<<endl;

    return 0;
}
