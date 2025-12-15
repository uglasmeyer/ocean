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

class Keyboad_Dialog_class :
		public QDialog//, virtual public QSpinBox
{
    Q_OBJECT
	QWidget* 		parent			= nullptr;

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
	void kbdbps( int );
	void sliding_mode(bool);
	void save();

private:
	QString textFromValue( int );
//	int valueFromText( const QString& ) ;
};

#endif // KEYBOAD_DIALOG_H
