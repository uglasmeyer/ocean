#include <QApplication>
#include <Mainwindow.h>

#include <App.h>
#include <Exit.h>

const string		TitleModule = "Ocean Sound Lab";

Exit_class	Exit{};
Statistic_class 	Statistic	{  TitleModule };

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

    Window.App.Start( argc, argv );





    Window.show();

    Windowapp.exec();
    exit_proc( 0 );

}
