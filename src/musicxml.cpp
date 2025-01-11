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
	if ( not p ) return "";
	const char* data  = p->GetText();
	return string( data );
}

int Musicxml_class::get_int( int Default, XMLElement* p  )
{
	string str = get_text( p );
	if ( str.length() == 0  )
	{
		if (Default < 0 )
			Exception( "Empty mandatory value in xml file" );
		else
			return Default;
	}
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
		pitch.octave = get_int( -1, p );
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
		pitch.alter = get_int( 0, p );
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
//	XMLElement* measure_e;
	while( measure_p )
	{
//		measure_e = measure_p;
		XMLElement*
		attr_p = measure_p->FirstChildElement("attributes");
		if ( attr_p )
		{
			XMLElement* // get divisions from attributes
			p = attr_p->FirstChildElement("divisions" );
			if ( p )
				musicxml.divisions = get_int( musicxml.divisions, p ) ;

			XMLElement* // get beats from time
			time_p = attr_p->FirstChildElement("time" );
			if ( time_p )
			{
				XMLElement*
				t = time_p->FirstChildElement("beats");
				if ( t )
					musicxml.beats = get_int( musicxml.beats, p ) ;
			}
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
				note.duration = ( get_int( -1, p ) * 250 ) / musicxml.divisions;
				musicxml.scoreduration += note.duration;
			}
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
				Notes.note_buffer 	= note_struct();
				note.chord.push_back(pitch);
				note.str.push_back(pitch.step_char);
				note.octave = pitch.octave;
				note.glide[0] = { pitch, false };
				musicxml.notelist.push_back( note );

			}
			note_p = note_p->NextSiblingElement("note");


		}

		measure_p = measure_p->NextSiblingElement("measure");
	}
//	if ( measure_e)
//		appendMeasure( part_p, measure_e );
	note_t pause = Notes.pause_note;
	pause.duration = max_milli_sec; // duration takes one measure;
	musicxml.scoreduration += pause.duration;
	musicxml.notelist.push_back( pause ); // add a pause at the end of the score
	return musicxml;
}


