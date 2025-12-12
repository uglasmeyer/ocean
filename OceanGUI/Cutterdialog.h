#ifndef CUTTERDIALOG_H
#define CUTTERDIALOG_H

// Ocean
#include <data/DataWorld.h>

// Qt
#include <QDialog>

namespace Ui
{
	class CutterDialog_class;
}



class CutterDialog_class :
	public 	QDialog,
	virtual wavedisplay_struct
{
    Q_OBJECT
	Dataworld_class* 		DaTA;
    Interface_class* 		Sds;
    interface_t*			sds;
    interface_t*			sds_master;
    EventLog_class*			Eventlog;
    Id_t					SDS_ID;

public:
    explicit CutterDialog_class(	QWidget *parent = nullptr,
    								Dataworld_class* data = nullptr,
									EventLog_class* el = nullptr );
    ~CutterDialog_class();

private slots:
	void Step_forward();
	void Step_backward();
	void Step_front_forward();
	void Step_front_backward();
	void Step_to_end();
	void Cut_tail();
	void Save();

public:
	void Setup( Interface_class* Sds );

private:
	void setSds( Interface_class* Sds );

    Ui::CutterDialog_class *ui;

};

#endif // CUTTERDIALOG_H
