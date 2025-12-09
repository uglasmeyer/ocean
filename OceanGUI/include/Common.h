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

extern bps_struct_t QBps;

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
