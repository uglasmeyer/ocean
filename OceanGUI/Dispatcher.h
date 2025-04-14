/*
 * Dispatcher.h
 *
 *  Created on: Mar 28, 2025
 *      Author: sirius
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

// QT
#include <QObject>
#include <QThread>
#include <Mainwindow.h>

// Synthesizer
#include <Ocean.h>
#include <Logfacility.h>
#include <data/Semaphore.h>
#include <data/DataWorld.h>
#include <EventKeys.h>

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

#endif /* DISPATCHER_H_ */
