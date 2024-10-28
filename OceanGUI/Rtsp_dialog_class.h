#ifndef RTSP_DIALOG_CLASS_H
#define RTSP_DIALOG_CLASS_H

#include <QDialog>
#include <QtWidgets/QTableWidget>


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

    Dataworld_class*		DaTA;
    Interface_class*		Sds;
    interface_t*			Sds_master;
    uint					SDS_ID = 0;
    Ui::Rtsp_Dialog_class*	ui;

	QTableWidgetItem	twItem {};
	QTableWidgetItem*	twItem_p = new QTableWidgetItem();

    void proc_table_update_row( uint row);
    void proc_table_update_all( );

    explicit Rtsp_Dialog_class(
    		QWidget* 			parent 	= nullptr,
			Dataworld_class*	data	= nullptr);
    ~Rtsp_Dialog_class();

private slots:



private:
	void proc_table( uint row, uint col, string text);
};

#endif // RTSP_DIALOG_CLASS_H
