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
 * File_Dialog.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#ifndef FILE_DIALOG_CLASS_H
#define FILE_DIALOG_CLASS_H

// Qt
//#include <QObject>
#include <QDialog>
#include <QComboBox>

// Ocean
#include <notes/Notes.h>
#include <data/DataWorld.h>

// OceanGUI
#include <ui_File_Dialog_class.h>
#include <GUIcommon.h>


/**************************************************
 * File_Dialoh_class
 *************************************************/
QT_BEGIN_NAMESPACE
namespace Ui
{
	class File_Dialog_class;
}
QT_END_NAMESPACE

class File_Dialog_class :
	public 					QDialog
	,virtual	public		Interface_base

{
    Q_OBJECT
    EventLog_class*				Eventlog_p			;
	Musicxml_class				Musicxml			;
public:
	Ui::File_Dialog_class*		ui					;

	direvent_t					EventXML, EventNTE	,
								EventINS, EventSES	;
	vector< direvent_t* > 		EventVec			{};
	string						scorename			;
	cstring_T<cstring_long>		Noteline			;
	cstring_T<cstring_long>		Rythmline			;
	noteline_prefix_t			Noteline_prefix		;

private:
    Semaphore_class* 			sem					;

public:

    explicit					File_Dialog_class(	QWidget* 		 		parent	= nullptr,
    												Dataworld_class* 		_data 	= nullptr,
													EventLog_class*  		_log	= nullptr,
													WfDisplay_que_struct* 	wf		= nullptr);
    virtual						~File_Dialog_class	();
    void 						SetSds				();

private:
    WfDisplay_que_struct*		WfDisplay_que		;
    void 						change_dirs			( string subdir );
    void						init_Direvent		();
    void 						init_Combobox		();
    void 						sdsCommand			( EVENTKEY_e key );
    void 						update_notesWidgets	( const string& nl,
    												const string& rl,
													const string& sn );

public slots:
    void 						updateWidgets		();
    void						Dialog				();
    void						Scorename			( QString name );

private slots:

    void 						Notes_Select		( QString arg1 );
    void 						Instrument_Select	( QString arg1 );
    void 						Instrument_Save		();
    void 						Notes_Done_clicked	();
    void 						cB_NotesPerSec		( int );
    void 						cB_Convention		( int );
    void 						sB_Octave			( int);
    void 						cb_Notestype		( int );
    void 						Longnote			( bool );
    void 						EditMusicXML		();
};

//namespace Ui { class File_Dialog_class{ }; }

#endif // FILE_DIALOG_H
