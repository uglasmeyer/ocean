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

void setButton( QPushButton* button, int state, QString txt )
{
	QPalette	GUIpalette	= QPalette	{};
	auto setButtonColor = [ &GUIpalette ]( Qt::GlobalColor color  )
	{
		GUIpalette.setColor( QPalette::All, QPalette::Button, color );
	};

	switch ( state )
	{
		case 0 : { setButtonColor( Qt::red )		; break; }
		case 1 : { setButtonColor( Qt::lightGray )	; break; }
		case 2 : { setButtonColor( Qt::yellow )		; break; }
		case 3 : { setButtonColor( Qt::green )		; break; }
		case 4 : { setButtonColor( Qt::white )		; break; }
		default: { setButtonColor( Qt::darkGray )	; break; }
	};

	button->setPalette( GUIpalette );
	if( txt.length() == 0 )
		return;
	button->setText( txt );
};

void set_StateButton( QPushButton* pB, StateId_e state, QString name  )
{
    switch ( state )
	{
		case RECORDING	: { setButton( pB, 0,"Stop Rec"); break; }	// red
		case STARTING	:
		case STOPPING	:
		case RECORDSTART: { setButton( pB, 2,"waiting")	; break; }	//yellow
		case INACTIVE	: { setButton( pB, 1, name )	; break; }	// gray
		case RUNNING	: { setButton( pB, 3, name )	; break; }
		case EXITSERVER	: { setButton( pB, 4, name )	; break; }
		case OFFLINE	: { setButton( pB, 5, name )	; break; }
		case SLEEPING	: { setButton( pB, 2, name )	; break; }

		default : { break; }
	} // switch  state
};

/**************************************************
 * direvent_struct
 *************************************************/
direvent_struct::direvent_struct( 	Id_t id, QComboBox* _cb, EVENTKEY_e _event,
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


