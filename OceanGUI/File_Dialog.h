#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QMainWindow>

// qtcreator
#include <Common.h>

// Synthesizer
#include <Logfacility.h>
#include <notes/Notes.h>
#include <data/DataWorld.h>

using namespace std;


namespace Ui
{
class File_Dialog;
}

class File_Dialog_class :
		public QDialog,
		virtual Logfacility_class,
		virtual Note_class
{
    Q_OBJECT

public:
    unique_ptr<Ui::File_Dialog> 	ui;

    QString         QNote_Chars		{"Notes: ( )"};
    Dataworld_class* DaTA			= nullptr;
    Interface_class* Sds			= nullptr;
    Semaphore_class* sem			= nullptr;
    interface_t*	sds_p			= nullptr;
    EventLog_class*	Eventlog_p		= nullptr;
    int8_t			SDS_ID			= 0;


    explicit File_Dialog_class(	QWidget* 		 parent	= nullptr,
    							Dataworld_class* _data 	= nullptr,
								EventLog_class*  _log	= nullptr
								);
    virtual ~File_Dialog_class();
    void Setup_widgets();
    void New_Notes();
    void New_Instrument();
    void set_le_instrument( QString );
    void set_le_notes( QString );
    void SetSds( Interface_class* sds, int8_t id );

private:

private slots:
    void on_cb_notefilenames_activated(QString arg1);

    void Instrument_Select(QString arg1);

    void Instrument_Save();
    void pb_Notes_Done_clicked();
    void cB_NotesPerSec( int);
    void cB_Convention( int );
    void sB_Octave( int);
    void cb_Notestype( int );
};

#endif // FILE_DIALOG_H
