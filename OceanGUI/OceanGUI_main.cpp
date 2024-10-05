#include <QApplication>

#include <signal.h>
#include <App.h>
#include <data/Interface.h>

Statistic_class 	Statistic	{ };
const string		TitleModule = "Ocean Sound Lab";
#include <Mainwindow.h>

void exit_proc( int signal )
{
	exit(0);
}

int main(int argc, char *argv[])
{
	catch_signals( &exit_proc, { SIGINT, SIGHUP, SIGABRT } );

    QApplication app(argc, argv);

    MainWindow 			Win;

    QString QVersion 	= QString::fromStdString( Version_str );
    QString QModule		= QString::fromStdString( TitleModule);
    Win.setWindowTitle( QModule + " " + QVersion  );

    Application_class* App = &Win.App;
    App->Start( argc, argv );

    Interface_class* Sds = Win.Sds;
	Sds->Announce( App->client_id, &Sds->addr->UserInterface );

    Win.setwidgetvalues();
    Win.show();

    app.exec();

    Statistic.Show_Statistic( App->Name );
    return 0;
}
