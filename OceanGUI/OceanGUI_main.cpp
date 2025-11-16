// License Notice
/******************************************************************************
-> Ocean Sound Lab Application Suite (short Ocean-SL)
Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
Linux based Operating Systems, that allows to generate, play and record sound.
It includes interfaces for musicxml-files and supports the sound drivers:
-> native ALSA and
-> Pulseaudio
The software includes a simple keyboard to play and record music as you type.
It comes with a limited set of sample files for instruments and notes to start
with. Funny play.

-> Ocean-SL GitHub site: https://github.com/uglasmeyer/ocean
-> Copyright (c) 2024-2025 Ulrich.Glasmeyer@web.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. Any person wishing to
distribute modifications to the Software is asked to send the modifications to
the original developer so that they can be incorporated into the canonical
version. This is, however, not a binding provision of this license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/

// Qt
#include <QApplication>
#include <QThread>

// Ocean
#include <App.h>

// OceanGUI
#include <include/Dispatcher.h>
#include <include/Mainwindow.h>



Exit_class			Exit		{};
Logfacility_class	Log			{ "Ocean Sound Lab" };
Statistic_class 	Statistic	{ Log.className };
const string		TitleModule = Log.className;

/*
 * https://doc.qt.io/qt-6/qthread.html
 */
void set_title( MainWindow* window )
{
    QString QVersion 	= Qstring( Version_str );
    QString QModule		= Qstring( TitleModule);
    QString QDir		= Qstring( window->App.Cfg->fs->installdir );
    window->setWindowTitle( QModule + " " + QVersion  + " (" + QDir + ")");
}

void exit_proc( int signal )
{
//	Log.Set_Loglevel( DBG2, true );
	if( signal == SIGILL )
		exit( 0 );
	coutf << TitleModule << " exit on signal " + to_string( signal ) << endl;
	Statistic.Show_Statistic( );
    QApplication::exit(0);
//	exit(0);
}

int main(int argc, char *argv[])
{
    QApplication 		Windowapp(argc, argv);

    MainWindow 			Window{};

//    QIcon icon("/home/sirius/Software/eclipse/eclipse32.png");
//    Windowapp.setWindowIcon( icon );


    Controller_class Controller{ Window };

    Window.App.Start( argc, argv );

    set_title( &Window);

    Window.show();

    Windowapp.exec();

    exit_proc( 0 );

}
