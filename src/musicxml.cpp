/*
 * musicxml.cpp
 *
 *  Created on: Nov 1, 2024
 *      Author: sirius
 */

#include <notes/MusicXML.h>
#include <notes/Notes.h>

inline bool next_measure( uint note_duration, uint& score_duration )
{
	score_duration += note_duration;
	uint mod = score_duration % (measure_duration);
	return ( mod < note_duration );
}

map<int, string> type_id_map = // @suppress("Invalid arguments")
{
	{32, "32th" 	},
	{16, "16th" 	},
	{ 8, "eighth" 	},
	{ 4, "quarter"	},
	{ 2, "half" 	},
	{ 1, "whole" 	}
};
map<string, int> type_str_map = imap( type_id_map );

auto get_text = []( XMLElement* p  )
{
	if ( not p ) return string("");
	const char* cstr  = p->GetText();
	return string( cstr );
};

auto get_int = []( int Default, XMLElement* p  )
{
	string str = get_text( p );
	if ( str.length() == 0  )
	{
		if (Default < 0 )
		{
			EXCEPTION( "Empty mandatory value in xml file" );
		}
		else
			return Default;
	}
	return stoi( str ) ;
};
auto xml_value = [  ]( const string& name, int& _default, XMLElement* root )
{
	XMLElement*
		child = root->FirstChildElement( name.data() );
	if ( child )
		_default = get_int( _default, child ) ;
};
auto xml_text = [  ]( const string& name, string& _default, XMLElement* root )
{
	XMLElement*
		child = root->FirstChildElement( name.data() );
	if ( child )
		_default = get_text( child ) ;
};

auto get_note_duration = []( int divisions, XMLElement* root )
{
	if ( not root ) return 0 ;
	int 	xml_duration 	= -1;
	string 	type_str 		= "";
	xml_value( "duration", xml_duration, root );
	xml_text( "type", type_str, root );
	if( type_str.length() == 0 )
		return 0;

//	coutf << "duration: " << xml_duration << " type_str: " << type_str << endl;
	return xml_duration * 500 / divisions;
//			divisions * measure_duration * xml_duration / type_str_map[ type_str ];

};


Musicxml_class::Musicxml_class(  ) :
	Logfacility_class( "Musicxml_class")
{
	className = Logfacility_class::className;
};
Musicxml_class::~Musicxml_class()
{
	DESTRUCTOR( className )
};
bool Musicxml_class::load_document	( string filename )
{
	XMLdoc.Clear();
	int err = XMLdoc.LoadFile( filename.data() );
	if ( err  )
    {
    	Comment( ERROR, "loading xml file ", filename, " with code: ", XMLdoc.ErrorName() );
    	return false;
   	};
	Info( "Loaded musicxml from file: ", filename );
	return true;
};
bool Musicxml_class::save_document	( string filename )
{
	int err = XMLdoc.SaveFile( filename.data() );
	if ( err  )
    {
    	Comment( ERROR, "saving xml file ", filename, " with code: ", XMLdoc.ErrorName() );
    	return false;
   	};
	Info( "Saved musicxml to file: " + filename );
	return true;

};

pitch_t Musicxml_class::get_pitch( XMLElement* root )
{

	if ( not root ) return pitch_struct() ;

	int oct = -1;
	int alt = 0;
	string str = "";
	char ch = PAUSE_CH;

	xml_value( "octave", oct, root );
	xml_value( "alter", alt, root );
	xml_text( "step", str, root );
	if( str.length() > 0 ) ch = str[0];

	pitch_t pitch= pitch_struct( oct, ch, alt );

	return pitch;
}

string Musicxml_class::File( const string& name )
{
	return fs.xmldir + name + fs.xml_type;
}
// https://www.w3.org/2021/06/musicxml40/tutorial/midi-compatible-part/
musicxml_t Musicxml_class::XmlFile2notelist( const string& name )
{
	if ( not load_document( File( name ) ) )
		return musicxml;

	int 	note_number		= 0;
	auto 	notelist_append = [ this, &note_number ]( note_t note )
	{
		musicxml.notelist.push_back( ( note ) );
		note_number++;
	};

	xmlstatistic_struct xmlstatistic = xmlstatistic_struct();
    musicxml.notelist.clear();

    XMLElement* root_p 		= XMLdoc.RootElement(); // Get root element
    XMLElement* partlist_p	= root_p->FirstChildElement("part-list");
    XMLElement* scorepart_p = partlist_p->FirstChildElement("score-part");
    XMLElement* scoreinstr_p= scorepart_p->FirstChildElement("score-instrument");

    xml_text( "instrument-name", musicxml.instrument_name, scoreinstr_p );
    Info( "using Instrument: ", musicxml.instrument_name );

	XMLElement* part_p 		= root_p->FirstChildElement("part");

	XMLElement* measure_p 	= part_p->FirstChildElement("measure");
	while( measure_p )
	{
		XMLElement*
		attr_p = measure_p->FirstChildElement("attributes");
		if ( attr_p )
		{
			xml_value( "divisions", musicxml.divisions, attr_p );
			XMLElement* // get beats from time
			time_p = attr_p->FirstChildElement("time" );
			if ( time_p )
			{
				xml_value( "beats", musicxml.beats, time_p );
				xml_value( "beat-type", musicxml.beat_type, time_p );
//				coutf << "musicxml.beats" << musicxml.beats << endl;
			}
		}

		XMLElement* // get notes from measure
		note_p = measure_p->FirstChildElement("note");
		while( note_p )
		{
			note_t	note { note_struct( note_number ) };

			XMLElement* // get pitch from note
			p = note_p->FirstChildElement("pitch");
			pitch_t
			pitch = get_pitch( p ) ;

						// get rest from note
			p = note_p->FirstChildElement("rest");
			if( p )
			{
				pitch.step_char = PAUSE_CH;
				pitch.step 		= NONOTE;
				note.volume 	= 0;
			}
						// get duration from note

							// get chord from note
			p = note_p->FirstChildElement("chord");
			if( p ) // is member of a chord
			{
				musicxml.notelist.back().chord.push_back( pitch );
				musicxml.notelist.back().str.push_back(pitch.step_char );
				musicxml.notelist.back().glide.push_back( { pitch, false });
			}
			else
			{
				note.duration = get_note_duration( musicxml.divisions, note_p );
				note.chord.push_back(pitch);
				note.str.push_back(pitch.step_char);
				note.glide[0] = { pitch, false };
				notelist_append( note );
				musicxml.scoreduration += note.duration;
			}
			note_p = note_p->NextSiblingElement("note");


		}
		measure_p = measure_p->NextSiblingElement("measure");

	}

	note_t 	whole_rest_note 	= Notes.rest_note;
	whole_rest_note.number		= note_number;
	whole_rest_note.duration 	= measure_duration - ( musicxml.scoreduration % measure_duration );
	notelist_append( whole_rest_note ); // add a pause at the end of the score

	musicxml.scoreduration 		+= whole_rest_note.duration;
	xmlstatistic.Duration 		= musicxml.scoreduration;
	xmlstatistic.Show( true );

	cout << "Xml2notelist length " <<  musicxml.notelist.size() << " chords"  << endl;
	return musicxml;
}



typedef struct xmlnote_value_struct
{

	int duration 	= 0;
	string type 	= "";
	bool dot		= false;
	xmlnote_value_struct( note_t note, int divisions )
	{
		set<int> dot_set {3,6,12};
		int min_duration	= gcd( measure_duration, ( note.duration ));
//		duration		 	= note.duration / min_duration;
		duration			= note.duration * divisions / 500;
		int type_id			= measure_duration / ( min_duration ) ;
		if ( dot_set.contains( duration ) )
		{
			dot = true;
			type 				= type_id_map[ type_id / 2 ];
		}
		else
			type 				= type_id_map[ type_id ];
	}
	~xmlnote_value_struct() = default;

} xmlnote_value_t;

auto insert_child =[]( XMLElement* root, const string& tag, auto value )
{
	if( not value )
		return;
	root->InsertNewChildElement( tag.data() )->SetText( value );
};

auto insert_node = [  ]( XMLElement* root, const string& tag  )
{
	if ( root )
		return root->InsertNewChildElement( tag.data() );
	else
		return root;
};
auto edit_child = [  ]( XMLElement* root, const string& tag, auto value  )
{
    if ( root )
    	root->FirstChildElement( tag.data() )->SetText( value );
};
uint8_t 		divisions 	= 8;
XMLElement* 	xmlattr 	= nullptr;
XMLElement* Musicxml_class::appendNote(	XMLElement* xmlmeasure,
										XMLElement* xmlnote,
										const note_t& note,
										const uint& chord_id )
{
	auto insert_pitch = [ this, chord_id ]( XMLElement* xmlnote, pitch_t pitch )
	{
		string str {};
		str.push_back( pitch.step_char  );
//		coutf << pitch.step << pitch.step_char << endl;
		if( chord_id > 0 )
		{
			insert_child( xmlnote, "chord", "" );
		}
		XMLElement* 	xmlpitch = insert_node( xmlnote, "pitch" );
		insert_child( 	xmlpitch, "step", str.data() );
		insert_child( 	xmlpitch, "alter", pitch.alter );
		insert_child( 	xmlpitch, "octave", pitch.octave );
	};

	auto insert_notevalue = [ this ]( XMLElement* xmlnote, const note_t& note )
	{	// divisions = 8, # of note measures in a 2 sec-measure
		if( note.duration == 0 ) return;
		xmlnote_value_t xmlnote_value = xmlnote_value_struct( note, musicxml.divisions );
		if( note.chord[0].step < 0 )
		{
			insert_child( 	xmlnote, "rest", "" );
			insert_child( 	xmlnote, "duration"	, xmlnote_value.duration );
			insert_child( 	xmlnote, "type"		, xmlnote_value.type.data() );
		}
		else
		{
			insert_child( 	xmlnote, "duration"	, xmlnote_value.duration );
			insert_child( 	xmlnote, "type"		, xmlnote_value.type.data() );
			if( xmlnote_value.dot )
				insert_child( 	xmlnote, "dot"		, "" );
		}
	};

	XMLElement* newxmlnote = NewElement( "note" );
	if( not xmlnote )
		xmlmeasure->InsertAfterChild( xmlattr, newxmlnote );
	else
	{
		xmlmeasure->InsertAfterChild( xmlnote , newxmlnote );
	}
	if( note.chord[0].step >= 0 )
		insert_pitch( newxmlnote, note.chord[chord_id] );
	insert_notevalue( newxmlnote, note );

	return newxmlnote;
}
XMLElement* Musicxml_class::appendMeasure( 	XMLElement* xmlpart,
											XMLElement* xmlmeasure,
											const int&  number )
{
	XMLElement* newxmlmeasure = NewElement( "measure" );
	if( not xmlmeasure )
	{
		xmlpart->InsertFirstChild( newxmlmeasure );
	}
	else
	{
		xmlpart->InsertAfterChild( xmlmeasure , newxmlmeasure );
	}
	newxmlmeasure->SetAttribute( "number", to_string(number).data() );
	xmlattr = insert_node( newxmlmeasure,  "attributes" );
	insert_child( 	xmlattr, "divisions", musicxml.divisions );
//	The <divisions> element indicates how many divisions per quarter note
//	are used to indicate a note's duration.
//	For example, if duration = 1 and divisions = 2, this is an eighth note duration.
	// (duration/divisions) 1/2 * 1/4 = 1/8

	XMLElement* 	time_p = insert_node( xmlattr, "time" );
	insert_child( 	time_p, "beats", musicxml.beats );
	insert_child(	time_p, "beat-type", musicxml.beat_type );


	return newxmlmeasure;
}

void Musicxml_class::Notelist2xmlFile( 	const string& name,
										const notelist_t& note_list )
{

	// prepare XML document from template
	//
	if( not load_document( File( "template" ) ) )
	{
		return;
	}
	Comment( INFO, "Processing note list ");
	xmlstatistic_struct xmlstatistic = xmlstatistic_struct();

    XMLElement* xmlroot 		= XMLdoc.RootElement(); // Get root element score-partwise
	XMLElement* xmlpart			= nullptr;
    if( xmlroot )
    	xmlpart	= xmlroot->FirstChildElement("part" );
    else
    {
    	Comment( ERROR, "XML Root element parse error in ", __FILE__ );
    	return;
    }
    XMLElement* xmlcredit		= xmlroot->FirstChildElement( "credit" );
    edit_child( xmlcredit, "credit-words", name.data() );

    // Load data into XML Document from notelist
	int 		measure_number 	= 1;
	uint		score_duration	= 0;
    XMLElement* xmlmeasure		= appendMeasure( xmlpart, nullptr, measure_number );;
    XMLElement* xmlnote			= nullptr;

    for( note_t note : note_list )
    {
    	if( next_measure( note.duration, score_duration ) )
    	{
    		xmlmeasure = appendMeasure( xmlpart, xmlmeasure, measure_number );
    		xmlnote = nullptr;
    		measure_number++;
    	}
    	for( uint chord_id = 0; chord_id < note.chord.size(); chord_id++ )
    		xmlnote = appendNote( xmlmeasure, xmlnote, note, chord_id );
        xmlstatistic.xmlNotes += note.chord.size();
        if( note.chord.size() > 1 )
        	xmlstatistic.Chords++;

    }

    save_document( File( name ) );

    xmlstatistic.Measures = measure_number-1;
	xmlstatistic.Duration += score_duration;
    xmlstatistic.Show( true );
return;
}

void Musicxml_class::Test()
{
	TEST_START( className );

	Note_class Notes{};
	note_t note{};
	typedef struct testvalue_struct
	{
		string 	testid;
		int 	duration;
		int 	noteduration; // multipe of 1/4/divisions
		string 	type;
		bool 	dot;
	} testvalue_t;
	vector<testvalue_t> testvec =
	{
		{"test1", 125, 				1, "16th"	, false },
		{"test1", 250, 				2, "eighth"	, false },
		{"test2", 500, 				4, "quarter", false },
		{"test3", 750, 				6, "quarter", true	},
		{"test4", 1000, 			8, "half"	, false	},
		{"test5", 1500, 			12, "half"	, true	},
		{"test6", measure_duration,	16, "whole"	, false	},
		{"test7", 1875, 			15, "16th"	, false	}

	};
	string teststr = "";
	for( testvalue_t test : testvec )
	{
		teststr = test.testid + " duration";
		note.duration = test.duration;
		xmlnote_value_t xmlnote_value = xmlnote_value_struct( note, musicxml.divisions );
		ASSERTION( xmlnote_value.duration == test.noteduration, teststr,
				(int)xmlnote_value.duration, (int)test.noteduration );
		teststr = test.testid + " type";
		ASSERTION( strEqual(xmlnote_value.type, test.type) , teststr,
							xmlnote_value.type, test.type);
		ASSERTION( xmlnote_value.dot == test.dot , teststr,
							(bool) xmlnote_value.dot, (bool)test.dot);
	}


	noteline_prefix_t nlp {};
	nlp.nps = 4;
	string Noteline = "A2B3-C4D3-.D3A4..A2-B3-C4-D3-A-B-";
	notelist_t nl = Notes.Gen_notelist( nlp, Noteline );
	Notes.Show_note_list( nl );
	Notelist2xmlFile( "test" , nl );


	Notes.musicxml = XmlFile2notelist(  "test"  );
	Notes.Show_note_list( Notes.musicxml.notelist ); // @suppress("Invalid arguments")
	cout << "instrument_name: " <<  Notes.musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  Notes.musicxml.divisions <<endl;
	cout << "beats          : " <<  Notes.musicxml.beats<<endl;

	musicxml_t testxml = Note_base::musicxml_struct();
	int beats = Notes.musicxml.beats;
	ASSERTION(	beats == testxml.beats, "Music xml beats",beats, testxml.beats );
	int divisions = Notes.musicxml.divisions;
	ASSERTION(	divisions == testxml.divisions, "Music xml divisions",divisions, testxml.divisions );
	int duration = Notes.musicxml.scoreduration;
	ASSERTION(	duration == 8000, "Music xml scoreduration",	duration, 8000L );

	TEST_END( className );
}
