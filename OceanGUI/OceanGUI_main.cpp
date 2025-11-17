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
