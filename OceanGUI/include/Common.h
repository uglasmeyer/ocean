//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Common.h
 *
 *  Created on: Jan 7, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef GUICOMMON_H
#define GUICOMMON_H



// Qt
#include <QDialog>
#include <QComboBox>
#include <QString>
#include <QStringList>

// Ocean
#include <data/Config.h>
#include <data/Interface.h>
#include <Ocean.h>
#include <System.h>
#include <EventKeys.h>

constexpr QString Qstring( string str )
{
	return QString::fromStdString( str );
}

constexpr QString QReadStr ( Interface_class* Sds, EVENTKEY_e key  )
{
    string str 		= Sds->Read_str( key );
	return Qstring( str );
};
//#76BAFA
template< typename T >
void setButton( T* pb, int state, bool init=false  )
{
	QPalette color = QPalette();
	QColor default_color = Qt::gray;//QColor(153, 193, 241);
	if ( init )
		color.setColor( QPalette::Button, default_color );
	else
	{
		switch ( state )
		{
			case 0 :
			{
				color.setColor(QPalette::Button, Qt::red );
				break;
			}
			case 1 :
			{
				color.setColor(QPalette::Button, default_color);
				break;
			}
			case 2 :
			{
				color.setColor(QPalette::Button, Qt::yellow );
				break;
			}
			case 3 :
			{
				color.setColor(QPalette::Button, Qt::green );
				break;
			}

			default:
				break;
		}
	}
	pb->setPalette( color );
};

struct PathStruct
{
	string  dir	;
	string	ext ;
	QString Qext;
	PathStruct(){};
	PathStruct( const string& _dir, const string& _ext ) :
		dir(_dir),
		ext(_ext)
	{
		Qext = Qstring( ext );
	}
	~PathStruct(){};
} ;
typedef PathStruct Path_t;

struct EventStruct : PathStruct
{
	EVENTKEY_e	event;
	Path_t	 	path ;
	EventStruct( ) : event( NULLKEY ) {} ;
	EventStruct( const EVENTKEY_e& _event, const string& _dir, const string& _ext ) :
		event( _event ), path( _dir, _ext )
	{
	}
	~EventStruct(){};
} ;
typedef EventStruct Event_t;

static file_structure fs = file_structure();

static Event_t 			EventXML( XMLFILE_KEY		, fs.xmldir			, fs.xml_type );
static Event_t 			EventNTE( UPDATENOTESKEY	, fs.notesdir		, fs.nte_type );
static Event_t 			EventINS( SETINSTRUMENTKEY	, fs.instrumentdir	, fs.snd_type );
static Event_t 			EventWAV( OTHERSTR_KEY		, fs.musicdir		, fs.wav_type );
static vector<Event_t > EventVec{ EventXML, EventNTE, EventINS, EventWAV };

extern QStringList 		Qstringlist		( const list<string>& str_lst );
extern vector<QString> 	Vstringvector	( const vector<string>& spstr_vec );
extern QStringList	 	Qstringvector	( const vector<string>& str_vec );
extern QStringList		Qread_filenames	( const Path_t _path );

#endif
