
// Qt
#include <QApplication>
#include <QThread>

// OceanGUI
#include <App.h>
#include <Exit.h>
#include <include/Dispatcher.h>
#include <include/Mainwindow.h>
#include <include/Rtsp_dialog_class.h>
#include <Logfacility.h>



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
;
}

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


    Controller_class Controller{ Window };

    Window.App.Start( argc, argv );

    set_title( &Window);

    Window.show();

    Windowapp.exec();

    exit_proc( 0 );

}
