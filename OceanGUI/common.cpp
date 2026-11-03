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
 * common.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

// Qt
#include <QScreen>

// OceanGUI
#include <GUIcommon.h>
#include <Oscbase.h>
#include <Test.h>

bps_struct QBps {};

QStringList Qstringlist ( const list<string>& str_lst )
{
	QStringList Qlist{};
	for ( string str : str_lst )
	{
		Qlist.push_back( ( Qstring( str ) ) );
	}
	return Qlist;
};

QStringList	Qstringvector( const string_vec_t& str_vec )
{
	QStringList Qlist{};
	for ( string str : str_vec )
	{
        Qlist.push_back( ( Qstring( str ) ) );
	}
	return Qlist;
	;
}


vector<QString> Vstringvector( const vector<string>& str_vec )
{
	vector<QString> Qvec {};
	for ( string str : str_vec )
	{
		Qvec.push_back( ( Qstring( str ) ) );
	}
	return Qvec;

};



void updateLogState( QToolButton* tB, LOG_e state )
{
	Qt::GlobalColor color = Qt::red;
	switch (state )
	{
		case INFO	: { color =Qt::green; break; }
		case WARN	: { color =Qt::yellow; break; }
		case ERROR	: { color =Qt::red; break; }
		default 	: { break; }
	} // switch state
	setButtonColor( tB, color, "" );
}
void set_StateButton( QPushButton* pB, StateId_e state, QString name, QString alt_txt  )
{
	Qt::GlobalColor	color 	= Qt::lightGray;
	QString			txt		= name;

	switch ( state )
	{
    	case ENABLED 	: { color = Qt::green		; txt = alt_txt 	; break; }
		case RECORDING	: { color = Qt::red			; txt = "Stop Rec"	; break; }
		case RECORDSTART: { color = Qt::yellow		; txt = "waiting"	; break; }
		case STARTING	:
		case STOPPING	:
		case INACTIVE	: { color = Qt::gray		; break; }
		case OFFLINE	: { color = Qt::darkGray 	; break; }
		case SLEEPING	: { color = Qt::yellow		; break; }
		case DISABLED 	: { color = Qt::lightGray  	; break; }
		case RUNNING	: { color = Qt::green		; break; }
		case EXITSERVER	: { color = Qt::white  		; break; }
		case DEFAULT	: { break; }
		default : { break; }
	} // switch  state

	setButtonColor( pB, color, txt );
};

/**************************************************
 * direvent_struct
 *************************************************/
direvent_struct::direvent_struct( 	SDSId_t id, QComboBox* _cb, EVENTKEY_e _event,
									cstring_name* _value, string _dir, string _ext )
	: dircontent_struct( { _dir }, _ext )
	, Id( id )
	, cb( _cb )
	, event( _event )
	, cstring( _value, SDSNAME_SIZE )
	, path( _dir, _ext )
{
	this->current_value = cstring.read();
}

void direvent_struct::SetSubdir( string _subdir )
{
	if( Id == DIRSES_ID )
	{
		UpdatePath( { path.dir } );
		filename_vec.push_back( "" );
	}
	else
	{
		if( _subdir.length() == 0 )
			UpdatePath( { path.dir } );
		else
		{
			string subdir 	= trailing_dirslash( path.dir + _subdir );
			UpdatePath( { subdir, path.dir } );
		}
	}
}
QStringList direvent_struct::qfilenames(  )
{
    return Qstringvector( filename_vec );
}
#include <string.h>
void direvent_struct::Update_cb()
{
	string	name		= cstring.read();;
	if( Id != DIRSES_ID ) // sessiondir does not select a file
	{
		Select				( name, default_name );
	}
//	const string wrt	{ name.substr(0, SDSNAME_SIZE-1 ) };
	cstring.save();
//	strcpy				( *value, wrt.data() );

	current_value		= name;

	cb->clear			();
	cb->addItems		( qfilenames() );
	cb->setCurrentText	( Qstring( current_value ));

}


