/*
 * Common.h
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
 */

#ifndef GUICOMMON_H
#define GUICOMMON_H

#include <Ocean.h>
#include <System.h>
#include <Config.h>
#include <Keys.h>

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QString>
#include <QStringList>


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
		Qext = QString::fromStdString( ext );
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

static dir_struct_t fs = file_structure();

static Event_t Event0( XMLFILE_KEY, fs.Dir.xmldir, fs.xml_type );
static Event_t Event1( UPDATENOTESKEY, fs.Dir.notesdir, fs.nte_type );
static Event_t Event2( SETINSTRUMENTKEY, fs.Dir.instrumentdir, fs.snd_type );
static vector<EventStruct > Path_vec { Event0, Event1, Event2 };

extern QStringList 		Qstringlist( const list<string>& str_lst );
extern vector<QString> 	Vstringvector( const vector<string>& str_vec );
extern QStringList	 	Qstringvector( const vector<string>& str_vec );

extern QStringList		Qread_filenames( const Path_t _path );

#endif
