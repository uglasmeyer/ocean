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
 * Dispatcher.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <include/Dispatcher.h>

/**************************************************
 * Dispatcher_class
 *************************************************/
Dispatcher_class::Dispatcher_class( MainWindow* _win ) :
	Logfacility_class( "Dispatcher_class")
{
	this->win		= _win;
	this->Sem_p 	= _win->Sem_p;
	this->Sds_master= _win->DaTA_p->SDS.Master;
	this->loop_done	= false;
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
	Info( "Dispatcher start" ) ;
	while( true )
	{
		Sem_p->Lock( SEMAPHORE_EVENT );
		if ( loop_done ) break;

		QString Qlog_str	= Qstring( Sds_master->Read_str( UPDATELOG_STR ) );
		Sds_master->Write_str( UPDATELOG_STR, "" );

		switch ( Sds_master->addr->FLAG )
		{
			case UPDATELOG_FLAG :
			{
				emit( UpdateRtspLog( Qlog_str) );
				break;
			}
			case UPDATE_KBDDIALOG_FLAG :
			{
				emit( UpdateKeyboardDialog() );
				break;
			}
			case NEWINSTRUMENTFLAG :
			{
				if( win->Spectrum_Dialog_p->isVisible() )
					emit( win->Spectrum_Dialog_p->SetInstrument() );
				if( win->File_Dialog_p->isVisible() )
					emit( win->File_Dialog_p->updateWidgets() );
				emit( win->Update_Statusbar( "" ) );
				break;
			}
			case NEWNOTESLINEFLAG :
			{
				if( win->File_Dialog_p->isVisible() )
					emit( win->File_Dialog_p->updateWidgets() );
				emit( win->Update_Statusbar( "" ) );
				break;
			}
			case APPSTATE_FLAG :
			{
				QString FixStr{ "Change run state " } ;
				emit( UpdateRtspLog( FixStr + Qlog_str ));
				emit( UpdateColorButtons() );
				break;
			}
			case UPDATE_WAV_FILELIST_FLAG :
			{
				emit( UpdateWavFileList() );
				break;
			}
			case UPDATE_FILEDIALOG_FLAG :
			{
				if( win->File_Dialog_p->isVisible() )
					emit( win->File_Dialog_p->updateWidgets() );
				break;
			}
			case UPDATE_CUTDESK_FLAG :
			{
				if( win->CutDesk_Dialog_p->isVisible() )
					emit( win->CutDesk_Dialog_p->updateCutDesk() );
				break;
			}
			case READWAVEDATAFLAG :
			{
				emit(win->UpdateDataDisplay() );
				if( win->Spectrum_Dialog_p->isVisible() )
					emit(win->Spectrum_Dialog_p->UpdateDataDisplay() );
				break;
			}
			default:
				break;
		} // switch FLAG
	}
}
/**************************************************
 * Controller_class
 *************************************************/
Controller_class::Controller_class( MainWindow& window ) :
	Dispatcher( &window )
{
	MainWindow*			window_p 		= &window;
	Dispatcher_class* 	Dispatcher_p 	= &Dispatcher;
	Dispatcher_p->moveToThread			( Thread_p );

	window.connect( Thread_p, &QThread::started,
					Dispatcher_p, &Dispatcher_class::Loop );
	window.connect( Dispatcher_p, &Dispatcher_class::finished,
					Thread_p, &QThread::quit);
	window.connect( Dispatcher_p, &Dispatcher_class::finished,
					Dispatcher_p, &Dispatcher_class::deleteLater);
	window.connect( Thread_p, &QThread::finished,
					Thread_p, &QThread::deleteLater);

	window.connect( Dispatcher_p, SIGNAL( UpdateWavFileList() ),
					window_p, SLOT( update_CB_external()) );
	window.connect( Dispatcher_p, SIGNAL( UpdateRtspLog(QString) ),
					window.Rtsp_Dialog_p, SLOT( UpdateLog(QString)) );
	window.connect( Dispatcher_p, SIGNAL( UpdateColorButtons() ),
					window_p, SLOT( updateColorButtons() ) );
	window.connect( Dispatcher_p, SIGNAL( UpdateKeyboardDialog() ),
					window.Keyboard_Dialog_p, SLOT( Setup_Widget() ) );


	Thread.start();
}

Controller_class::~Controller_class()
{
	Dispatcher.done();
    Thread.quit();
    Thread.wait();
}

