#ifndef FILE_DIALOG_CLASS_H
#define FILE_DIALOG_CLASS_H

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QMainWindow>

// Synthesizer
#include <Logfacility.h>
#include <notes/Notes.h>
#include <data/DataWorld.h>
#include <include/Common.h>

// OceanGUI
#include <ui_File_Dialog_class.h>




class File_Dialog_class : // class name refers to file name File_Dialog_class.ui
		public QDialog,
		virtual Logfacility_class,
		virtual Note_class
{
    Q_OBJECT

public:
    unique_ptr<Ui::File_Dialog_class> 	ui;
//	Ui::File_Dialog_class*			ui;
    QString         QNote_Chars		{"Notes: ( )"};
    Dataworld_class* DaTA			= nullptr;
    Interface_class* Sds			= nullptr;
    Semaphore_class* sem			= nullptr;
    interface_t*	sds_p			= nullptr;
    EventLog_class*	Eventlog_p		= nullptr;
    int8_t			SDS_ID			= 0;


    void New_Notes();
    void New_Instrument();
    void set_le_instrument( QString );
    void set_le_notes( QString );
    void SetSds( Interface_class* sds, int8_t id );

    explicit
	File_Dialog_class(	QWidget* 		 parent	= nullptr,
    					Dataworld_class* _data 	= nullptr,
						EventLog_class*  _log	= nullptr
						);
    virtual
	~File_Dialog_class();

private:

public slots:
    void Setup_widgets();

private slots:

    void Notes_Select(QString arg1);

    void Instrument_Select(QString arg1);
    void Instrument_Save();


    void pb_Notes_Done_clicked();
    void cB_NotesPerSec( int);
    void cB_Convention( int );
    void sB_Octave( int);
    void cb_Notestype( int );
    void Longnote(bool);
};

//namespace Ui { class File_Dialog_class{ }; }

#endif // FILE_DIALOG_H
