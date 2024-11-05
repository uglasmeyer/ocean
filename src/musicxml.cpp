/*
 * musicxml.cpp
 *
 *  Created on: Nov 1, 2024
 *      Author: sirius
 */

#include <notes/MusicXML.h>

using namespace tinyxml2;

string Musicxml_class::get_text( XMLElement* p  )
{
	const char* data  = p->GetText();
	return string( data );
}

int Musicxml_class::get_int( XMLElement* p  )
{
	string str = get_text( p );
	return stoi( str ) ;
}

Note_base::pitch_t Musicxml_class::get_pitch( XMLElement* cp )
{

	pitch_t pitch= pitch_struct();
	if ( not cp ) return pitch ;

	XMLElement*
	p = cp->FirstChildElement("octave");
	if( p )
	{
		pitch.octave = get_int( p );
		Noteline.push_back('|');
		Noteline.push_back( int2char( pitch.octave ) );
	}
	p = cp->FirstChildElement("step");
	if( p )
	{
		pitch.step_char = get_text( p )[0];
		pitch.step 	= Notes.Notechar2Step( pitch.step_char );
	}
	p = cp->FirstChildElement("alter");
	if( p )
	{
		pitch.alter = get_int( p );
		if ( pitch.alter < 0 )
			Noteline.push_back( ',');
		if ( pitch.alter < 0 )
			Noteline.push_back('\'');
	}
	pitch.freq = Notes.CalcFreq( oct_base_freq, pitch );
	return pitch;
}

void Musicxml_class::appendMeasure( XMLElement* part, XMLElement* measure )
{
	XMLElement* newmeasure = NewElement( "measure" );
	part->InsertAfterChild( measure , newmeasure );

	XMLElement*	note_p 	= newmeasure->InsertNewChildElement("note");
	XMLElement*
	p = note_p->InsertNewChildElement("duration");
	p->SetText("4");

	XMLElement* pitch_p = note_p->InsertNewChildElement("pitch");
	p = pitch_p->InsertNewChildElement("step");
	p->SetText(".");

	XMLdoc.SaveFile("/tmp/demo2.xml");
}

// https://www.w3.org/2021/06/musicxml40/tutorial/midi-compatible-part/
Note_class::musicxml_t Musicxml_class::Xml2notelist( const string& filename )
{

    int err = XMLdoc.LoadFile( filename.data() );
    if ( err  )
    {
    	cout << "xml error " << err  << endl;
    	cout << "in conjunction with " + filename << endl;
    	return musicxml;

   	}; // Load XML file

    musicxml.notelist.clear();

    XMLElement* root_p 		= XMLdoc.RootElement(); // Get root element
    XMLElement* partlist_p	= root_p->FirstChildElement("part-list");
    XMLElement* scorepart_p = partlist_p->FirstChildElement("score-part");
    XMLElement* scoreinst_p = scorepart_p->FirstChildElement("score-instrument");
    XMLElement*
	p = scoreinst_p->FirstChildElement("instrument-name");
    musicxml.instrument_name = get_text( p );
    cout << "I: "<< musicxml.instrument_name << endl;

	XMLElement* part_p 		= root_p->FirstChildElement("part");

	XMLElement* measure_p 	= part_p->FirstChildElement("measure");
	XMLElement* measure_e;
	while( measure_p )
	{
		measure_e = measure_p;
		XMLElement*
		attr_p = measure_p->FirstChildElement("attributes");
		if ( attr_p )
		{
			XMLElement* // get divisions from attributes
			p = attr_p->FirstChildElement("divisions" );
			musicxml.divisions = get_int( p ) ;

			XMLElement* // get beats from time
			time_p = attr_p->FirstChildElement("time" );
			p = time_p->FirstChildElement("beats");
			musicxml.beats = get_int( p ) ;
		}

		XMLElement* // get notes from measure
		note_p = measure_p->FirstChildElement("note");
		while( note_p )
		{
			note_t	note { note_struct() };

			XMLElement* // get pitch from note
			p = note_p->FirstChildElement("pitch");
			pitch_t
			pitch = get_pitch( p ) ;

						// get rest from note
			p = note_p->FirstChildElement("rest");
			if( p )
			{
				pitch.step_char = '.';
				pitch.step 		= -12;
				note.volume 	= 0;
			}
						// get duration from note
			p = note_p->FirstChildElement("duration");
			if( p )
			{
				note.duration = ( get_int( p ) * 250 ) / musicxml.divisions;
			}
							// get chord from note
			p = note_p->FirstChildElement("chord");
			if( p ) // is member of a chord
			{
				musicxml.notelist.back().chord.push_back( pitch );
				musicxml.notelist.back().str.push_back(pitch.step_char );
				musicxml.notelist.back().glide.push_back( { pitch, false });
				Noteline.push_back( pitch.step_char );
			}
			else
			{
				Notes.note_buffer 	= note_struct();
				note.chord.push_back(pitch);
				note.str.push_back(pitch.step_char);
				note.octave = pitch.octave;
				note.glide[0] = { pitch, false };
				musicxml.notelist.push_back( note );

				Noteline.push_back( '(' );
				Noteline.push_back( pitch.step_char );
			}
			note_p = note_p->NextSiblingElement("note");

			if ( note_p )
			if ( not note_p->FirstChildElement("chord") )
				Noteline.push_back( ')' );

		}

		measure_p = measure_p->NextSiblingElement("measure");
	}
//	if ( measure_e)
//		appendMeasure( part_p, measure_e );
	note_t pause = Notes.pause_note;
	pause.duration = 1000;

	musicxml.notelist.push_back( pause );
	return musicxml;
}


