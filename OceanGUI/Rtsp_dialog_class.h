#ifndef RTSP_DIALOG_CLASS_H
#define RTSP_DIALOG_CLASS_H

#include <QDialog>

#include <data/DataWorld.h>
#include <Ocean.h>
#include <data/Interface.h>
#include <data/Semaphore.h>
#include <System.h>

namespace Ui {
const string ClassName = "Rtsp";
class Rtsp_Dialog_class;
}

class Rtsp_Dialog_class : public QDialog, virtual Logfacility_class, virtual Config_class
{
    Q_OBJECT

public:

    Semaphore_class*	sem;
    interface_t*		sds;
    Dataworld_class*	daTA;
    uint				SDS_ID = 0;

    explicit Rtsp_Dialog_class(
    		QWidget* 			parent 	= nullptr,
			Dataworld_class*	data	= nullptr,
			Semaphore_class* 	sem		= nullptr );
    ~Rtsp_Dialog_class();

private slots:

	void activate_S0();
	void activate_S1();
	void activate_S2();
	void activate_S3();
	void Exit_Rtsp();
	void Start_Rtsp();


private:
    Ui::Rtsp_Dialog_class *ui;
    void update_widgets();
    QPalette red_color	{};
    QPalette green_color{};
};

#endif // RTSP_DIALOG_CLASS_H
