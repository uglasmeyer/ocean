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
    Ui::File_Dialog* ui;
    QPalette        status_color 	= QPalette();
    QComboBox*      CB_notes		= nullptr  ;
    QComboBox*      CB_instruments	= nullptr;
    QSlider*		Sl_Main_Hz		= nullptr;
    string          NotesDir		= "";
    string          notes_type		= "";
    string          instruments_path= "";
    QString         QNote_Chars		{"Notes: ( )"};
    const QString   NotesON 		= "Notes Off" ;
    const QString   NotesOFF		= "Play Notes";
    bool            SWITCHON 		= false;
    Interface_class* Sds			= nullptr;
    Semaphore_class* sem			= nullptr;
    interface_t*	sds_p			= nullptr;
    int8_t			SDS_ID			= 0;
    QList<QString>	QNotestypes		{};
    QList<QString>	QNotesdirs		{};
    vector<int> 	Noteskeys		{};

    explicit File_Dialog_class(	QWidget* 		 parent	= nullptr,
    							Interface_class* sds 	= nullptr,
								Semaphore_class* sem 	= nullptr);
    ~File_Dialog_class();
    void Setup_widgets();
    void New_Notes();
    void New_Instrument();
    void set_le_instrument( QString );
    void set_le_notes( QString );
    void SetSds( Interface_class* sds, int8_t id );

private slots:
    void on_cb_notefilenames_activated(const QString &arg1);

    void on_cb_instrumentfiles_activated(const QString &arg1);

    void pb_Instrument_Done_clicked();
    void pb_Notes_Done_clicked();
    void cB_NotesPerSec(int);
    void cB_Convention( int );
    void sB_Octave(int);
    void cb_Notestype( int );
};

#endif // FILE_DIALOG_H
