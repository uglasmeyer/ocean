//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
    void 						SetSds				( Interface_class* sds, Id_t id );

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
