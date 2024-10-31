#include <QApplication>
#include <Mainwindow.h>

#include <App.h>
#include <data/Interface.h>
#include <Exit.h>

const string		TitleModule = "Ocean Sound Lab";

Exit_class	Exit{};
Statistic_class 	Statistic	{  TitleModule };



void exit_proc( int signal )
{
	cout << endl;
	cout << TitleModule << " exit on signal " + to_string( signal ) << endl;
    QApplication::exit(0);

	exit(0);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow 			Win;

    QString QVersion 	= QString::fromStdString( Version_str );
    QString QModule		= QString::fromStdString( TitleModule);
    Win.setWindowTitle( QModule + " " + QVersion  );

    Application_class* App = &Win.App;
    App->Start( argc, argv );
	App->Sds->Announce( );


    Win.setwidgetvalues();
    Win.show();

    app.exec();
    Statistic.Show_Statistic( );

    return 0;
}
