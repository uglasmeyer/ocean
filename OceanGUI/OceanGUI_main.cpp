/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
#include <QWindow>
#include <QThread>

// Ocean
#include <Appsymbols.h>

// OceanGUI
#include <include/Dispatcher.h>
#include <include/Mainwindow.h>

const string		TitleModule = "Ocean Sound Lab Ui - ";

void set_title( MainWindow* window )
{
    QString QVersion 	= Qstring( Version_str );
    QString QModule		= Qstring( TitleModule);
    QString QDir		= Qstring( Cfg.fs->installdir );
    window->setWindowTitle( QModule + " " + QVersion  + " (" + QDir + ")" );
}

void exit_proc( int signal )
{
//	Log.Set_Loglevel( DBG2, true );
	if( signal == SIGILL )
		exit( 0 );
	Log.Info( TitleModule, "exit on signal", signal );
    QApplication::exit(0);
}

int main(int argc, char *argv[])
{
    QApplication 		Windowapp	{ argc, argv };
    MainWindow 			Window		{};
    Controller_class 	Controller	{ Window };

    set_title			( &Window);
    App.Start			( argc, argv );
    App.Ready			();
    Log.Comment			( DEBUG, "Window.showing" );
    Window.show			();
    Log.Comment			( DEBUG, "Window.showed" );
    Windowapp.exec		();

    exit_proc			( 0 );

}
