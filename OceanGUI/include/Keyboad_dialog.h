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
 * Keyboard_dialog.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#ifndef KEYBOAD_DIALOG_H
#define KEYBOAD_DIALOG_H

//Qt
#include <QDialog>

// Ocean
#include <data/DataWorld.h>
#include <Keyboard.h>

namespace Ui {
class Keyboad_Dialog_class;
}

class Keyboad_Dialog_class : public QDialog
{
    Q_OBJECT

public:
    Ui::Keyboad_Dialog_class*		ui;
    Dataworld_class* DaTA			= nullptr;
    Interface_class* Sds			= nullptr;
    interface_t*	sds_p			= nullptr;
    EventLog_class*	Eventlog_p		= nullptr;
    Kbd_pitch_class Kbd_pitch 		{};

    Id_t			SDS_ID			= 0;
    QString			keyboard_key	= "Key";

    void 							Setup_Widget		();
    explicit Keyboad_Dialog_class(	QWidget* 			parent 	= nullptr,
    								Dataworld_class* 	_data 	= nullptr,
    								EventLog_class*  	_log	= nullptr);
    ~Keyboad_Dialog_class();

private slots:
	void buffer_mode( int );
	void base_octave(int);
	void flats(int);
	void sharps(int);
	void decay_mode(bool);
	void sliding_mode(bool);
	void save();

private:
};

#endif // KEYBOAD_DIALOG_H
