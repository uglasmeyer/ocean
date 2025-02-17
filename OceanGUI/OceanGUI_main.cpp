#include <QApplication>
#include <Mainwindow.h>

#include <App.h>
#include <data/Interface.h>
#include <Exit.h>

const string		TitleModule = "Ocean Sound Lab";

//Exit_class	Exit{};
//Statistic_class 	Statistic	{  TitleModule };



void exit_proc( int signal )
{
	cout.flush() << endl;
	cout.flush() << TitleModule << " exit on signal " + to_string( signal ) << endl;
    QApplication::exit(0);
}

int main(int argc, char *argv[])
{
    QApplication Windowapp(argc, argv);

    MainWindow 			Window{};

    QIcon icon("/usr/share/qtcreator/doc/qtcreator/images/front-gs.png");
    Window.setWindowIcon( icon );

    QString QVersion 	= QString::fromStdString( Version_str );
    QString QModule		= QString::fromStdString( TitleModule);
    Window.setWindowTitle( QModule + " " + QVersion  );

    Application_class* App = &Window.App;
    App->Start( argc, argv );
	App->Sds->Announce( );


//    Window.setwidgetvalues();
    Window.show();

    Windowapp.exec();
//    Statistic.Show_Statistic( );
    exit_proc( 0 );

    return 0;
}
