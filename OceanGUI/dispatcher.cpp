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
 * Dispatcher.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <include/Dispatcher.h>

Dispatcher_class::Dispatcher_class( Dataworld_class* _data ) :
	Logfacility_class( "Dispatcher_class")
{
	this->className = Logfacility_class::className;
	Sem_p = _data->Sem_p;
	Sds_p = _data->Sds_master;
	Sds_master = _data->Sds_master;
};
Dispatcher_class::~Dispatcher_class()
{
	cout.flush() << "reset SEMAPHORE_EVENT" << endl;
	Sem_p->Reset( SEMAPHORE_EVENT );
	DESTRUCTOR( className );
};
void Dispatcher_class::done()
{
	loop_done = true;
	Sem_p->Release( SEMAPHORE_EVENT );
}
void Dispatcher_class::Loop(  )
{
	QString Qlog_str;
	Info( "Dispatcher start" ) ;
	while( true )
	{
		Sem_p->Lock( SEMAPHORE_EVENT );
		if ( loop_done ) break;

		string ev_str = Sds_master->Read_str( UPDATELOG_EVENT );
		Qlog_str = Qstring( ev_str );

		switch ( Sds_p->addr->FLAG )
		{
			case UPDATELOG_EVENT :
			{
				emit UpdateRtspLog( Qlog_str);
				Sds_master->Write_str( UPDATELOG_EVENT, "" );
				break;
			}
			case READ_EXTERNAL_WAVFILE :
			{
				emit( UpdateFileList() );
				break;
			}
			case NEWINSTRUMENTFLAG :
			case NEWNOTESLINEFLAG :
			{
				emit( UpdateFileDialog() );
				break;
			}
			case APPSTATE_FLAG :
			{
				QString FixStr{ "Change run state " };
				emit( UpdateRtspLog( FixStr + Qlog_str ));
				Sds_master->Write_str( UPDATELOG_EVENT, "" );
				emit( UpdateColorButtons() );
				break;
			}
			case RECORDWAVFILEFLAG :
			{
				emit( UpdateFileList() );
				break;
			}
			default:
				break;
		} // switch FLAG
	}
}

Controller_class::Controller_class( MainWindow& window ) :
	Dispatcher( &window.DaTA )
{
	MainWindow*		window_p = &window;
	Dispatcher_class* Dispatcher_p = &Dispatcher;
	Dispatcher_p->moveToThread( Thread_p );

	window.connect( Thread_p, &QThread::started,
					Dispatcher_p, &Dispatcher_class::Loop );
	window.connect( Dispatcher_p, &Dispatcher_class::finished,
					Thread_p, &QThread::quit);
	window.connect( Dispatcher_p, &Dispatcher_class::finished,
					Dispatcher_p, &Dispatcher_class::deleteLater);
	window.connect( Thread_p, &QThread::finished,
					Thread_p, &QThread::deleteLater);

	window.connect( Dispatcher_p, SIGNAL( UpdateFileList() ),
					window_p, SLOT( update_CB_external()) );
	window.connect( Dispatcher_p, SIGNAL( UpdateRtspLog(QString) ),
					window.Rtsp_Dialog_p, SLOT( UpdateLog(QString)) );
	window.connect( Dispatcher_p, SIGNAL( UpdateFileDialog() ),
					window.File_Dialog_p, SLOT( Setup_widgets()) );
	window.connect( Dispatcher_p, SIGNAL( UpdateColorButtons() ),
					window_p, SLOT( updateColorButtons() ) );

	Thread.start();
}

Controller_class::~Controller_class()
{
	Dispatcher.done();
    Thread.quit();
    Thread.wait();
}

