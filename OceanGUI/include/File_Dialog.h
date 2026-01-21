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
 * File_Dialog.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#ifndef FILE_DIALOG_CLASS_H
#define FILE_DIALOG_CLASS_H

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QMainWindow>

// Synthesizer
#include <Logfacility.h>
#include <notes/Notes.h>
#include <data/DataWorld.h>

// OceanGUI
#include <ui_File_Dialog_class.h>

class File_Dialog_class :
	public 						QDialog,
	virtual 					Logfacility_class,
	virtual 					Note_class,
								sdsstate_struct
{
    Q_OBJECT

public:
	//	Ui::File_Dialog_class*			ui;
    unique_ptr<Ui::File_Dialog_class>
    							ui;
    QString         			QNote_Chars			{ "Notes: ( )"};
    Dataworld_class* 			DaTA				= nullptr;
    Interface_class* 			Sds					= nullptr;
    Semaphore_class* 			sem					= nullptr;
    interface_t*				sds_p				= nullptr;
    EventLog_class*				Eventlog_p			= nullptr;
    int8_t						SDS_ID				= 0;


    void 						New_Notes			();
    void 						New_Instrument		();
    void 						set_le_instrument	( QString );
    void 						set_le_notes		( QString );
    void 						SetSds				( Interface_class* sds );

    explicit					File_Dialog_class(	QWidget* 		 parent	= nullptr,
    												Dataworld_class* _data 	= nullptr,
													EventLog_class*  _log	= nullptr);
    virtual						~File_Dialog_class	();

private:

public slots:
    void 						Setup_widgets		();

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
