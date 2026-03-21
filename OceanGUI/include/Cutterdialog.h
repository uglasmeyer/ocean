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



#ifndef CUTTERDIALOG_H
#define CUTTERDIALOG_H

// Ocean

// Qt
#include <QObject>
#include <QDialog>

// OceanGUI
#include <ui_CutDesk_Dialog.h>
#include <data/DataWorld.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class CutDesk_Dialog_class;
}
QT_END_NAMESPACE

/**************************************************
 * CutDesk_Dialog_class
 *************************************************/
class CutDesk_Dialog_class :
	public 				QDialog				,
	virtual 			wavedisplay_struct,
	public virtual		Interface_base
{
    Q_OBJECT

    EventLog_class*		Eventlog			;
    Ui::CutDesk_Dialog_class*
						ui					;

private:

public:
    explicit 			CutDesk_Dialog_class( QWidget*		parent	= nullptr,
    										Dataworld_class* data 	= nullptr,
											EventLog_class* el 		= nullptr );
    virtual				~CutDesk_Dialog_class	();
	void 				Setup				( SharedData_class* Sds );
	void				SetSds				();

private slots:
	void 				Step_forward		();
	void 				Step_backward		();
	void 				Step_front_forward	();
	void 				Step_front_backward	();
	void 				Step_to_end			();
	void 				Cut_tail			();
	void 				Save				();


public slots:
	void 				updateCutDesk		();
	void				Dialog				();
};

#endif // CUTTERDIALOG_H
