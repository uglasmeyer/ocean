#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

// Qt includes
#include <QDialog>
#include <QComboBox>

// Synthesizer
#include <notes.h>
#include <Logfacility.h>

using namespace std;

namespace Ui {
class File_Dialog;
}

class File_Dialog_class : public QDialog,  Note_class, GUI_interface_class
{
    Q_OBJECT

public:
    Ui::File_Dialog* ui;
    Logfacility_class
                    Log				{"FileDialog"};
    QPalette        status_color 	= QPalette();
    QComboBox*      CB_notes  ;
    QComboBox*      CB_instruments;
    string          notes_path;
    string          instruments_path  ;
    QString         QNote_Chars		{"Notes: ( )"};
    const QString   NotesON 		= "Notes Off" ;
    const QString   NotesOFF		= "Play Notes";
    bool            SWITCHON 		= false;


    explicit File_Dialog_class(QWidget *parent = 0 );
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
};

#endif // FILE_DIALOG_H
