/*
 * MusicXML.h
 *
 *  Created on: Nov 1, 2024
 *      Author: sirius
 */

#ifndef NOTES_MUSICXML_H_
#define NOTES_MUSICXML_H_

#include <notes/Notesbase.h>
#include <Ocean.h>
#include <notes/Notes.h>
#include <tinyxml2.h>

class Musicxml_class :
	public virtual Logfacility_class,
	public virtual Note_base
{
	string className = "";
public:


	Note_class Notes{};
	Note_class::musicxml_t musicxml;

	tinyxml2::XMLDocument XMLdoc;

	Musicxml_class() :
		Logfacility_class( "Musicxml_class"),
		Note_base()
	{
		className = Logfacility_class::module;
	};
	virtual ~Musicxml_class()
	{};

	Note_class::musicxml_t Xml2notelist( const string& filename );
	tinyxml2::XMLElement* NewElement( string name )
	{ return XMLdoc.NewElement( name.data() ) ; };

private:
	Note_base::pitch_t 	get_pitch( tinyxml2::XMLElement* cp );
	string 				get_text( tinyxml2::XMLElement* p  );
	int 				get_int( int, tinyxml2::XMLElement* p  );
	void 				appendMeasure( tinyxml2::XMLElement* parent, tinyxml2::XMLElement* child);

};



#endif /* NOTES_MUSICXML_H_ */
