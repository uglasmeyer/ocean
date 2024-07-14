#include "mainwindow.h"

#include <QApplication>

#include <signal.h>


void exit_proc( int signal )
{
    exit( signal );
}


int main(int argc, char *argv[])
{
    signal( SIGINT, &exit_proc );
    signal( SIGABRT, &exit_proc );

    QApplication app(argc, argv);
    MainWindow win;


    win.setwidgetvalues();
    win.show();
    return app.exec();
}
