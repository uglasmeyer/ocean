#include "mainwindow.h"

#include <QApplication>

#include <signal.h>
#include <Interface.h>
#include <App.h>

Application_class* App;




int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow win;

    QString QVersion = QString::fromStdString( Version_str );
    win.setWindowTitle( "OceanGUI " + QVersion  );
    App = &win.App;
    win.setwidgetvalues();
    win.show();
    app.exec();

    return 0;
}
