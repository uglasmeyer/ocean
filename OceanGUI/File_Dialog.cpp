// System includes
#include <dirent.h>

// qtcreator includes
#include "ui_File_Dialog.h"
#include <File_Dialog.h>
#include <mainwindow.h>

// Synthesizer includes
#include <synthesizer.h>
#include <notes.h>
#include <GUIinterface.h>
#include <Logfacility.h>
#include <keys.h>

// Qt-includes
#include <QDebug>

void read_filelist( QComboBox* CB, string path, QString type )
{

    if ( not CB ) return;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (path.data())) != NULL)
    {
        CB->clear();
        vector<QString> filelist;
        while ((ent = readdir (dir)) != NULL)
        {
            QString filename    = ent->d_name;
            QString filetype    = filename.split('.')[1];
            QString name        = filename.split('.')[0];
            if ( filetype.compare( type ) == 0)
                filelist.push_back( name );
        }
        sort(filelist.begin(), filelist.end());
        for ( auto name : filelist )
            CB->addItem( name );
        closedir (dir);
    }
}

File_Dialog_class::File_Dialog_class(QWidget *parent) :
    Logfacility_class("FileDialog"),
    QDialog(parent),
    Note_class( ),
    GUI_interface_class(),
    ui(new Ui::File_Dialog)
{
    ui->setupUi(this);
    CB_notes                 = ui->cb_notefilenames;
    CB_instruments           = ui->cb_instrumentfiles;

    notes_path          = dir_struct().notesdir;
    instruments_path    = dir_struct().instrumentdir;
    read_filelist( CB_notes, notes_path, "kbd");
    read_filelist( CB_instruments, instruments_path, "kbd");

    connect(ui->pbPlayNotes, SIGNAL(clicked()), this, SLOT(pb_PlayNotes_OnOff() ));
    connect(ui->pbInstrumentDone, SIGNAL(clicked()), this, SLOT(pb_Instrument_Done_clicked()) );
    connect(ui->pbNotesDone, SIGNAL(clicked()), this, SLOT(pb_Notes_Done_clicked()) );

    QWidget::update();
    Log.Comment(Log.DEBUG," File_Dialog initialized");


    Setup_widgets();
}

void File_Dialog_class::Setup_widgets()
{
    QString QStr;
    Instrument_name = GUI_interface_class::addr->Instrument;
    QStr = QString::fromStdString( Instrument_name );
    ui->lE_Instrument->setText( QStr );
    New_Instrument();

    string Notes_name =GUI_interface_class::addr->Notes;
    QStr = QString::fromStdString( Notes_name );
    ui->lE_NotesFile->setText( QStr );

    Note_class::read( Notes_name );
    string Notesline =  Note_class::get_note_line();
    if ( this->verify_noteline( Note_class::Noteline_prefix, Notesline ) )
    {
        status_color.setColor(QPalette::Button, Qt::green);
        QNote_Chars = QString::fromStdString( Note_class::Note_Chars );
        ui->lbl_selected_notes->setText("Notes ( " + QNote_Chars + " )");
    }
    else
    {
        status_color.setColor(QPalette::Button, Qt::red);
    }
    ui->pbPlayNotes->setPalette(status_color);

    QStr = QString::fromStdString( Notesline );
    ui->lE_Notes->setText( QStr );

    string Rhythmline = Note_class::get_rhythm_line();
    QStr = QString::fromStdString( Rhythmline );
    ui->lE_Rythm->setText( QStr );

    New_Notes();

}
File_Dialog_class::~File_Dialog_class()
{
    delete ui;
}


void File_Dialog_class::on_cb_instrumentfiles_activated(const QString &arg1)
{
    qDebug() << "on_cb_instrumentfiles_activated";
    qDebug() << "Instrument " << arg1 ;
    QString QStr = arg1;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        GUI_interface_class::write_str('i', str );
        GUI_interface_class::addr->KEY = SETINSTRUMENTKEY;
    }
    ui->lE_Instrument->setText( QStr );
}

void File_Dialog_class::New_Instrument()
{
    QString QStr = ui->lE_Instrument->text();
    string instrument = QStr.toStdString();
    qDebug() << "new Instrument " << QStr ;

    if ( instrument.length() > 0 )
    {
        GUI_interface_class::write_str( 'i', instrument );
        GUI_interface_class::addr->KEY = NEWINSTRUMENTKEY;
    }

}



/*
void File_Dialog::set_le_instrument( QString name )
{
    ui->lE_Instrument->setText( name );
}
*/

// ----------------------------------------------------------------------------- Notes
void File_Dialog_class::New_Notes()
{
    QString QStr = ui->lE_Notes->text();
    string note_line = QStr.toStdString();
    qDebug() << "new notes " << QStr ;

    if ( this->verify_noteline( Note_class::Noteline_prefix, note_line ) )
    {
        QStr = ui->lE_Rythm->text();
        string rhythm_line = QStr.toStdString();
        Note_class::set_rhythm_line( rhythm_line );

        QNote_Chars = QString::fromStdString( Note_class::Note_Chars );
        ui->lbl_selected_notes->setText("Notes ( " + QNote_Chars + " )");

        QStr = ui->lE_NotesFile->text();
        string notes_file = QStr.toStdString();


        Note_class::save( notes_file, Note_class::Noteline_prefix, note_line );

        // remote shall read and activate the new note line
        GUI_interface_class::write_str('n', notes_file);
        GUI_interface_class::addr->KEY = NEWNOTESLINEKEY;
        GUI_interface_class::addr->MIX_Amp = 75;
        GUI_interface_class::addr->MIX_Id = MbIdNotes;
        // remote load file to note class
        status_color.setColor(QPalette::Button, Qt::green);
        ui->pbPlayNotes->setText( NotesON );
        SWITCHON = false;

    }
    else
    {
        qDebug( ) << "Warning: notes are not aligned to 4 " ;
        qDebug() << "unchanged ... ";
        status_color.setColor(QPalette::Button, Qt::red);
    }
    ui->pbPlayNotes->setPalette(status_color);

}

void File_Dialog_class::pb_PlayNotes_OnOff()
{
    SWITCHON = not SWITCHON;
    if ( SWITCHON )
    {
        ui->pbPlayNotes->setText( NotesOFF );
        GUI_interface_class::addr->KEY = NOTESOFFKEY;
    }
    else
    {
        ui->pbPlayNotes->setText( NotesON );
        GUI_interface_class::addr->KEY = NOTESONKEY;

    }
}

void File_Dialog_class::on_cb_notefilenames_activated(const QString &arg1)
{
    qDebug()    <<"on_cb_notefilenames_activated";
    qDebug() << "Notes name: " << arg1 ;
    QString Note_name = arg1;
    string note_name = Note_name.toStdString();
    if ( note_name.length() > 0 )
    {
        // local notename
        ui->lE_NotesFile->setText( Note_name );
        // local noteline
        string  notes_line = Note_class::read( note_name );
        QString Notes_line = QString::fromStdString( notes_line );
        ui->lE_Notes->setText( Notes_line );
        string rhythmline = Note_class::get_rhythm_line();
        QString Rhythmline = QString::fromStdString( rhythmline );
        ui->lE_Rythm->setText( Rhythmline );

        // remote
        GUI_interface_class::write_str('n', note_name );
        GUI_interface_class::addr->KEY = UPDATENOTESKEY; // update notes
    }
    else
    {
        qDebug() << "empty notefile name";
    }
}

void File_Dialog_class::pb_Instrument_Done_clicked()
{
    New_Instrument();
}
void File_Dialog_class::pb_Notes_Done_clicked()
{
    New_Notes();
}


