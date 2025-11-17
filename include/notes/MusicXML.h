/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * MusicXML.h
 *
 *  Created on: Nov 1, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef NOTES_MUSICXML_H_
#define NOTES_MUSICXML_H_

#include <notes/Notesbase.h>
#include <Ocean.h>
#include <tinyxml2.h>

using namespace tinyxml2;

struct xmlstatistic_struct
{
	Table_class 	Table		{};
	int				Measures	= 0;
	int				xmlNotes	= 0;
	int				Chords		= 0;
	int				Align		= 0;
	int				Duration	= 0;
	xmlstatistic_struct()
	{
		InitTable();
	};
	~xmlstatistic_struct()=default;
	void InitTable()
	{
		Table.opt.Titel = "Score statistics";
		Table.AddColumn("Item", 25);
		Table.AddColumn( "Value", 6 );
	}
	void Show( bool debug )
	{
		if( not debug ) return;

		Table.PrintHeader();
		Table.AddRow( "Measures:"	, Measures );
		Table.AddRow( "Chords:"		, Chords );
		Table.AddRow( "Notes:"		, xmlNotes );
		Table.AddRow( "Align:"		, Align );
		Table.AddRow( "Score duration", Duration );
	}
};

class Musicxml_class
	: public virtual Logfacility_class
{
	string className = "";
public:


	musicxml_t 				musicxml;
	Note_base 				Notes{ };

	XMLDocument 			XMLdoc;
							Musicxml_class();
	virtual 				~Musicxml_class();

	musicxml_t 				XmlFile2notelist( const string& filename );
	void					Notelist2xmlFile( const string& filename, const notelist_t& nl );
	XMLElement* 			NewElement( string name )
							{ return XMLdoc.NewElement( name.data() ) ; };
	string					File			( const string& name );
	void 					Test();

private:
	bool					load_document	( string filename );
	bool					save_document	( string filename );
	XMLElement* 			appendMeasure	( XMLElement* part,
											XMLElement* measure,
											const int& number );
	pitch_t 				get_pitch		( XMLElement* cp );
	XMLElement*				appendNote		( XMLElement* measure_p,
											XMLElement* note_p,
											const note_t& note,
											const uint& chord_id );

	file_structure			fs				{};
	note_itr_t				note_itr		;
};



#endif /* NOTES_MUSICXML_H_ */
