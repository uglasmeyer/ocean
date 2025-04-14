#include <Logfacility.h>
#include <Exit.h>
#include <notes/MusicXML.h>
#include <notes/Notes.h>



Musicxml_class 	MusicXML{};
Note_class		Notes{};


void exit_proc( int signal )
{
	exit(0);
}

#include <Mixer.h>
#include <data/SharedDataSegment.h>

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
	Dataworld_class DaTA{ SYNTHID };
	Wavedisplay_class WD{ DaTA.Sds_p };
	Mixer_class Mixer{ &DaTA, &WD };
	Mixer.status = DaTA.sds_master->mixer_status;
	cout << "Instrument: " << boolalpha << Mixer.status.instrument << endl;

	return 0;
}
