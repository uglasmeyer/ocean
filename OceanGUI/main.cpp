#include "mainwindow.h"

#include <QApplication>

#include <signal.h>
#include <Interface.h>
#include <App.h>

ifd_t* sds;
Application_class* App;

void exit_proc( int signal )
{
	App->Decline( sds );
    exit( signal );
}



int main(int argc, char *argv[])
{
    signal( SIGINT, &exit_proc );
    signal( SIGABRT, &exit_proc );

    QApplication app(argc, argv);
    MainWindow win;

    QString QVersion = QString::fromStdString( Version_str );
    win.setWindowTitle( "OceanGUI " + QVersion  );
    sds = win.sds;
    App = &win.App;
    win.setwidgetvalues();
    win.show();
    app.exec();

    return 0;
}
