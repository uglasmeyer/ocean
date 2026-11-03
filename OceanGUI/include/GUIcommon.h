/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
#include <QPushButton>
#include <QToolButton>

// Ocean
#include <data/Interface.h>

extern Config_class Cfg;

#define defaultButtonRGB 0,179,255

extern QStringList 		Qstringlist		( const list<string>& str_lst );
extern vector<QString> 	Vstringvector	( const vector<string>& spstr_vec );
extern QStringList	 	Qstringvector	( const vector<string>& str_vec );
extern void 			SetGeometry		( QWidget* Widget );
extern void 			updateLogState	( QToolButton* tB, LOG_e state );
extern void 			set_StateButton	( QPushButton* pB,
										StateId_e state,
										QString name,
										QString alt_txt = QString("") );
struct qwd_rolenames_struct
{
	vector<QString> names;
	qwd_rolenames_struct()
	{
		names			= Vstringvector	( sta_role_map().StANames );
		names.push_back	( "Audio" );
	}
};
const vector<QString> Qwd_role_names = qwd_rolenames_struct().names;

inline constexpr QString Qstring( string str )
{
	return QString::fromStdString( str );
}

inline constexpr StateId_e colorstate( bool state )
{
	return ( state ? StateId_e::ENABLED : StateId_e::DISABLED );
};


struct path_data
{
	string  dir	;
	string	ext ;
	QString Qext;
	path_data(){};
	path_data( string _dir, string _ext ) :
		dir(_dir),
		ext(_ext)
	{
		Qext = Qstring( ext );
	}
	~path_data(){};
} ;
typedef path_data Path_t;

/**************************************************
 * direvent_struct
 * keep information and update Combobox with available
 * filenames mathing the specified file extension
 *************************************************/
struct direvent_struct
	: path_data
	, dircontent_struct
{
	SDSId_t		Id 				= 0;
	QComboBox*	cb 				= nullptr;
	EVENTKEY_e	event 			= NULLKEY;
	cstring_T<cstring_name>		cstring;
	Path_t	 	path 			{};
	string		subdir 			= "";
	string		current_value	;

				direvent_struct	() : dircontent_struct( {}, "" ){};
				direvent_struct	( SDSId_t id, QComboBox* _cb, EVENTKEY_e _event,
								cstring_name* value, string _dir, string _ext );
	virtual 	~direvent_struct() = default;
	void 		SetSubdir		( string dir );
	QStringList qfilenames		();
	void 		Update_cb		();

} ;
typedef direvent_struct direvent_t;

enum DIREVENTID_e : uint8_t
{
	DIRXML_ID = XML_ID,
	DIRNTE_ID = NTE_ID,
	DIRINS_ID = 2,	// instrument
	DIRSES_ID = 3,	// session
	DIRWAV_ID = 4	// wav file
};

template< class Button >
auto setButtonColor ( Button* pB, Qt::GlobalColor color, QString txt )
{
	QPalette GUIpalette	= QPalette	{};
	GUIpalette.setColor	( QPalette::All, QPalette::Button, color );
	pB->setPalette		( GUIpalette );
	if( txt.length() == 0 )	return; // do not change button text
	pB->setText( txt );
};

#endif /* GUICOMMON_H */
