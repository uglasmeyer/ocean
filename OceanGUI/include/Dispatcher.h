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
 * Dispatcher.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_DISPATCHER_H_
#define INCLUDE_DISPATCHER_H_

// QT
#include <QObject>
#include <QThread>
#include <Ocean.h>
#include <Logfacility.h>
#include <data/Semaphore.h>
#include <data/DataWorld.h>
#include <EventKeys.h>
#include <include/Mainwindow.h>

class Dispatcher_class :
		public QObject,
		virtual Logfacility_class
{
    Q_OBJECT
	Semaphore_class* 	Sem_p 		= nullptr;
	Interface_class* 	Sds_p 		= nullptr;
	Interface_class*	Sds_master	= nullptr;
	bool 				loop_done 	= false;

public:
    Dispatcher_class( Dataworld_class* _data ) ;
    virtual ~Dispatcher_class();
    void done();

public slots:
	void Loop( );

signals:
    void UpdateRtspLog(const QString& Qlog_str);
    void UpdateFileList();
    void UpdateFileDialog();
    void UpdateColorButtons();
    void finished();

private:
};

class Controller_class :
		public QObject
{
    Q_OBJECT
    QThread 			Thread		{};
    QThread* 			Thread_p	= &Thread;
	Dispatcher_class 	Dispatcher	= nullptr;

public:
    Controller_class( MainWindow& window ) ;
    virtual ~Controller_class();

signals:
    void operate(const QString &);
};

#endif /* INCLUDE_DISPATCHER_H_ */
