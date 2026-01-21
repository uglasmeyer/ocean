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

// Ocean-SL
#include <Logfacility.h>
#include <data/Semaphore.h>
#include <data/DataWorld.h>
#include <EventKeys.h>
#include <include/Mainwindow.h>

/**************************************************
 * Dispatcher_class
 *************************************************/
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
    void UpdateSpectrumDialog();
    void UpdateKeyboardDialog();
    void UpdateColorButtons();
    void UpdateCutDesk();
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
