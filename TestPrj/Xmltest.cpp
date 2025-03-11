#include <Logfacility.h>
#include <Exit.h>
#include <notes/MusicXML.h>
#include <notes/Notes.h>
#include <Table.h>

LogVector_t LogFile{"Xmltest"};
Logfacility_class Log{"Xmltest"};

Musicxml_class 	MusicXML{};
Note_class		Notes{};

struct PathStruct
{
	string  dir	;
	string	ext ;
	PathStruct(){};
	PathStruct(  string& _dir,  string& _ext ) :
		dir(_dir),
		ext(_ext)
	{

	}
	~PathStruct(){};
} ;
struct EventStruct : PathStruct
{
	int				event;
	PathStruct	 	path ;
	EventStruct( ) : event(0){};
	EventStruct(  int& _event,  string& _dir,  string& _ext ) :
		event( _event ), path( _dir, _ext )
	{
	}
	~EventStruct(){};
} ;
typedef EventStruct EventStruct_t;

vector<EventStruct > Path_vec {};


void exit_proc( int signal )
{
	exit(0);
}


int main()
{


	Table_class Table {  };
	Table.TestTable(  );

/*
	Log.StartFileLogging( &LogFile);
	ASSERTION( 1 == 2, "False", 1 , 1);
	MusicXML.Notes.Set_Loglevel( DEBUG, true );
	Notes.musicxml = MusicXML.Xml2notelist( file_structure().Dir.xmldir + "second.musicxml" );
	MusicXML.Notes.Show_note_list( Notes.musicxml.notelist ); // @suppress("Invalid arguments")

	cout << "instrument_name: " <<  Notes.musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  Notes.musicxml.divisions <<endl;
	cout << "beats          : " <<  Notes.musicxml.beats<<endl;

	Log.WriteLogFile();
	*/
    return 0;
}
