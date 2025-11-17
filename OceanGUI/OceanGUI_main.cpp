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

    Window.App.Ready();
    Windowapp.exec();

    exit_proc( 0 );

}
