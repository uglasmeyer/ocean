#ifndef RTSP_DIALOG_CLASS_H
#define RTSP_DIALOG_CLASS_H

// Qt
#include <QDialog>
#include <QtWidgets/QTableWidget>

// OceanGUI
#include "ui_Rtsp_dialog_class.h"

// Synthesizer
#include <data/DataWorld.h>
#include <Ocean.h>
#include <data/Interface.h>
#include <data/Semaphore.h>
#include <System.h>

namespace Ui {
const string ClassName = "RtspDialog";
class Rtsp_Dialog_class;
}

class Rtsp_Dialog_class :
	public 	QDialog,
	virtual Config_class
{
    Q_OBJECT

public:

    Dataworld_class*		DaTA;
    Interface_class*		Sds;
    interface_t*			sds_master;
    uint					SDS_ID = 0;
//    Ui::Rtsp_Dialog_class*	ui;
	unique_ptr<Ui::Rtsp_Dialog_class>		ui;

	QTableWidgetItem	twItem {};
	QTableWidgetItem*	twItem_p = new QTableWidgetItem();

    void proc_table_update_row( uint row);
    void proc_table_update_all( );


    explicit Rtsp_Dialog_class(
    		QWidget* 			parent 	= nullptr,
			Dataworld_class*	data	= nullptr);
    virtual ~Rtsp_Dialog_class();

    public slots:
	void UpdateLog( const QString& logstr );


private:
	void proc_table( uint row, uint col, string text);
};

#endif // RTSP_DIALOG_CLASS_H
