/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

// Ocean
#include <data/Interface.h>


extern Config_class Cfg;
#define defaultButtonRGB 0,179,255

namespace Ocean
{
	enum color : uint8_t
	{
		error		= Qt::red,
		standard	= Qt::gray,
		warning		= Qt::yellow,
		active		= Qt::green,
		inactice	= Qt::blue,
		undefined	= Qt::gray
	};
}
extern void setButton( QPushButton* button, int state, QString txt = "" );
extern void set_StateButton( QPushButton* pB, StateId_e state, QString name  );

constexpr QString Qstring( string str )
{
	return QString::fromStdString( str );
}

constexpr QString QReadStr ( SharedData_class* Sds, CSTRKEY_e key  )
{
    string str 		= Sds->Read_str( key );
	return Qstring( str );
};




struct PathStruct
{
	string  dir	;
	string	ext ;
	QString Qext;
	PathStruct(){};
	PathStruct( string _dir, string _ext ) :
		dir(_dir),
		ext(_ext)
	{
		Qext = Qstring( ext );
	}
	~PathStruct(){};
} ;
typedef PathStruct Path_t;

/**************************************************
 * direvent_struct
 * keep information and update Combobox with available
 * filenames mathing the specified file extension
 *************************************************/
struct direvent_struct
	: PathStruct
	, dircontent_struct
{
	Id_t		Id 				= 0;
	QComboBox*	cb 				= nullptr;
	EVENTKEY_e	event 			= NULLKEY;
	cstring_T<cstring_name>		cstring;
//	cstring_name*	value		= nullptr;
	Path_t	 	path 			{};
	string		subdir 			= "";
	string		current_value	;

				direvent_struct	() : dircontent_struct( {}, "" ){};
				direvent_struct	( Id_t id, QComboBox* _cb, EVENTKEY_e _event,
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






extern QStringList 		Qstringlist		( const list<string>& str_lst );
extern vector<QString> 	Vstringvector	( const vector<string>& spstr_vec );
extern QStringList	 	Qstringvector	( const vector<string>& str_vec );
extern void 			SetGeometry		( QWidget* Widget );


#endif /* GUICOMMON_H */
