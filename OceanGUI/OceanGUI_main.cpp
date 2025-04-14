
// Qt
#include <QApplication>
#include <QThread>

// OceanGUI
#include <Mainwindow.h>
#include <Dispatcher.h>

// Synthesizer
#include <App.h>
#include <Exit.h>
#include <Logfacility.h>
#include <Rtsp_dialog_class.h>


const string		TitleModule = "Ocean Sound Lab";

Exit_class			Exit		{};
Statistic_class 	Statistic	{  TitleModule };
Logfacility_class	Log			{ TitleModule };





/*
 * https://doc.qt.io/qt-6/qthread.html
 */

void exit_proc( int signal )
{
	cout.flush() << endl;
	cout.flush() << TitleModule << " exit on signal " + to_string( signal ) << endl;
	Statistic.Show_Statistic( );



    QApplication::exit(0);
}


int main(int argc, char *argv[])
{
    QApplication 		Windowapp(argc, argv);
    MainWindow 			Window{};

//    QIcon icon("/home/sirius/Software/eclipse/eclipse32.png");
//    Window.setWindowIcon( icon );

    QString QVersion 	= QString::fromStdString( Version_str );
    QString QModule		= QString::fromStdString( TitleModule);
    Window.setWindowTitle( QModule + " " + QVersion  );

    Controller_class Controller{ Window };

    Window.App.Start( argc, argv );

    Window.show();

    Windowapp.exec();

    exit_proc( 0 );

}
