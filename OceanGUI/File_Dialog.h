#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QMainWindow>


// Synthesizer
#include <notes.h>
#include <Logfacility.h>

using namespace std;

namespace Ui {
class File_Dialog;
}

class File_Dialog_class : public QDialog,  Note_class, Interface_class
{
    Q_OBJECT

public:
    Ui::File_Dialog* ui;
    Logfacility_class
                    Log				{"FileDialog"};
    QPalette        status_color 	= QPalette();
    QComboBox*      CB_notes		= nullptr  ;
    QComboBox*      CB_instruments	= nullptr;
    QSlider*		Sl_Main_Hz		= nullptr;
    string          notes_path;
    string          instruments_path  ;
    QString         QNote_Chars		{"Notes: ( )"};
    const QString   NotesON 		= "Notes Off" ;
    const QString   NotesOFF		= "Play Notes";
    bool            SWITCHON 		= false;


    explicit File_Dialog_class(QWidget *parent = nullptr, QSlider* = nullptr );
    ~File_Dialog_class();
    void Setup_widgets();
    void New_Notes();
    void New_Instrument();
    void set_le_instrument( QString );
    void set_le_notes( QString );

private slots:
    void on_cb_notefilenames_activated(const QString &arg1);

    void on_cb_instrumentfiles_activated(const QString &arg1);

    void pb_PlayNotes_OnOff();
    void pb_Instrument_Done_clicked();
    void pb_Notes_Done_clicked();
    void cB_NotesPerSec(int);
    void cB_Convention( int );
    void sB_Octave(int);
};

#endif // FILE_DIALOG_H
