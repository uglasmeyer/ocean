/*
 * Common.h
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
 */

#ifndef GUICOMMON_H
#define GUICOMMON_H


#include <QDialog>
#include <QComboBox>
#include <QString>
#include <QStringList>

#include <Ocean.h>
#include <System.h>
#include <Config.h>
#include <EventKeys.h>

constexpr QString Qstring( string str )
{
	return QString::fromStdString( str );
}

template< typename T >
QString QReadStr ( T* Sds, uint key  )
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
	int			event;
	Path_t	 	path ;
	EventStruct( ) : event(0) {} ;
	EventStruct( const int& _event, const string& _dir, const string& _ext ) :
		event( _event ), path( _dir, _ext )
	{
	}
	~EventStruct(){};
} ;
typedef EventStruct Event_t;

static file_structure fs = file_structure();

static Event_t EventXML( XMLFILE_KEY, fs.xmldir, fs.xml_type );
static Event_t EventNTE( UPDATENOTESKEY, fs.notesdir, fs.nte_type );
static Event_t EventINS( SETINSTRUMENTKEY, fs.instrumentdir, fs.snd_type );
static Event_t EventWAV( OTHERSTR_KEY, fs.musicdir, fs.wav_type );
static vector<Event_t > Event_vec { EventXML, EventNTE, EventINS, EventWAV };

extern QStringList 		Qstringlist		( const list<string>& str_lst );
extern vector<QString> 	Vstringvector	( const vector<string>& spstr_vec );
extern QStringList	 	Qstringvector	( const vector<string>& str_vec );
extern QStringList		Qread_filenames	( const Path_t _path );

#endif
