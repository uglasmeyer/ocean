#include <QApplication>

#include <signal.h>
#include <Interface.h>
#include <App.h>
#include <Mainwindow.h>

Application_class* App;




int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow win;

    QString QVersion 	= QString::fromStdString( Version_str );
    QString QModule		= QString::fromStdString( Module);
    win.setWindowTitle( QModule + " " + QVersion  );
    App = &win.App;
    App->Start( argc, argv );
    win.setwidgetvalues();
    win.show();
    app.exec();

    return 0;
}
