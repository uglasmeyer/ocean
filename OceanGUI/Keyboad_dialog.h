#ifndef KEYBOAD_DIALOG_H
#define KEYBOAD_DIALOG_H

//Qt
#include <QDialog>

// Ocean
#include <data/DataWorld.h>

namespace Ui {
class Keyboad_Dialog_class;
}

class Keyboad_Dialog_class : public QDialog
{
    Q_OBJECT

public:
    Dataworld_class* DaTA			= nullptr;
    Interface_class* Sds			= nullptr;
    interface_t*	sds_p			= nullptr;
    EventLog_class*	Eventlog_p		= nullptr;
    Id_t			SDS_ID			= 0;
    QString			keyboard_key	= "Key";

    void 							Setup_Widget		();
    explicit Keyboad_Dialog_class(	QWidget* 			parent 	= nullptr,
    								Dataworld_class* 	_data 	= nullptr,
    								EventLog_class*  	_log	= nullptr);
    ~Keyboad_Dialog_class();

private slots:
	void buffer_mode( int );
	void base_octave(int);
	void flats(int);
	void sharps(int);
	void decay_mode(bool);
	void sliding_mode(bool);

private:
    Ui::Keyboad_Dialog_class*		ui;
};

#endif // KEYBOAD_DIALOG_H
