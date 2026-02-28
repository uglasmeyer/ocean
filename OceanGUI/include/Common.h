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
#include <data/DataWorld.h>
#include <Ocean.h>
#include <System.h>
#include <EventKeys.h>

extern bps_t 		QBps;
extern Config_class Cfg;

constexpr QString Qstring( string str )
{
	return QString::fromStdString( str );
}

constexpr QString QReadStr ( SharedData_class* Sds, EVENTKEY_e key  )
{
    string str 		= Sds->Read_str( key );
	return Qstring( str );
};
//#76BAFA


template< typename T >
void setButton( T* pb, int state, bool init=false  )
{
	auto getcolor = [ init, state ](  )
	{
		if ( init ) return Qt::gray;
		switch (state)
		{
			case 0 : return Qt::red;
			case 1 : return Qt::gray;
			case 2 : return Qt::yellow;
			case 3 : return Qt::green;
		};
		return Qt::gray;
	};
	QPalette color = QPalette();
	color.setColor(QPalette::Button, getcolor() );
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

static Event_t 			EventXML( UPDATENOTESKEY	, Cfg.fs->xmldir		, Cfg.fs->xml_type );
static Event_t 			EventNTE( UPDATENOTESKEY	, Cfg.fs->notesdir		, Cfg.fs->nte_type );
static Event_t 			EventINS( SETINSTRUMENTKEY	, Cfg.fs->instrumentdir	, Cfg.fs->snd_type );
static Event_t 			EventWAV( OTHERSTR_KEY		, Cfg.fs->musicdir		, Cfg.fs->wav_type );
static vector<Event_t > EventVec{ EventXML, EventNTE, EventINS, EventWAV };

extern QStringList 		Qstringlist		( const list<string>& str_lst );
extern vector<QString> 	Vstringvector	( const vector<string>& spstr_vec );
extern QStringList	 	Qstringvector	( const vector<string>& str_vec );
extern QStringList		Qread_filenames	( const Path_t _path );
extern void 			SetGeometry		( QWidget* Widget );

/**************************************************
 * OceanGUI_base
 *************************************************/
class OceanGUI_base :
	virtual public 			Logfacility_class
{
public:
	Dataworld_class*		DaTA	;
	SharedData_class*		Sds		;
	interface_t*			sds_master;
	interface_t*			sds_p	;
	Id_t					SDS_ID	;

	OceanGUI_base( Dataworld_class* data ) :
		Logfacility_class	("OceanGUI_base")
	{
		this->DaTA			= data;
		this->sds_master 	= data->sds_master;
		SetSds				();
	};
	virtual ~OceanGUI_base()
	{
		DESTRUCTOR( className );
	};
	void SetSds( )
	{
		this->SDS_ID	= DaTA->sds_master->config;
		this->Sds 		= DaTA->SDS.GetSds( SDS_ID );
		this->sds_p 	= Sds->addr;
	}

private:
};

#endif
