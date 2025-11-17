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
 * Rtsp_dialog.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#ifndef RTSP_DIALOG_CLASS_H
#define RTSP_DIALOG_CLASS_H

// Qt
#include <QDialog>
#include <QtWidgets/QTableWidget>

// OceanGUI
#include "ui_Rtsp_dialog_class.h"

// Ocean
#include <data/DataWorld.h>
#include <Ocean.h>
#include <data/Interface.h>
#include <data/Semaphore.h>
#include <System.h>

namespace Ui {
class Rtsp_Dialog_class ;
}

class Rtsp_Dialog_class :
	public 				QDialog,
	virtual 			Logfacility_class,
						AppMap_struct
{
    Q_OBJECT
	string 				className 				= "";
    Dataworld_class*	DaTA;
	unique_ptr<Ui::Rtsp_Dialog_class>			ui;
	const int			max_table_rows			= MAXCONFIG + 1;

public:

    void 				Proc_table_update		( interface_t* sds, APPID apppid );
    void 				Proc_table_update_all	();


    explicit 			Rtsp_Dialog_class		(
    											QWidget* 			parent 	= nullptr,
												Dataworld_class*	data	= nullptr);
    virtual 			~Rtsp_Dialog_class		();

    public slots:
	void 				UpdateLog				( const QString& logstr );


private:
	void 				proc_table( uint row, uint col, string text);
};

#endif // RTSP_DIALOG_CLASS_H
