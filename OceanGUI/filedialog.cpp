// System includes
#include <data/Interface.h>
#include <dirent.h>

// qtcreator includes
#include "ui_File_Dialog.h"
#include <File_Dialog.h>
#include <Mainwindow.h>

// Synthesizer includes
#include <Keys.h>
#include <Logfacility.h>
#include <Notes.h>
#include <Ocean.h>

// Qt-includes
#include <QDebug>
#include <QString>

void Qread_filelist( QComboBox* CB, const string& path, const string type )
{

    if ( not CB ) return;
    CB->clear();
    vector_str_t files = List_directory( path, type );
    sort( files.begin(), files.end() );
    for ( string& file : files )
    {
    	size_t len = file.length() - type.length();
    	string name = file.substr(0, len);
    	CB->addItem( QString::fromStdString( name ) );
    }
}

Ui::File_Dialog	UI_FileDialog_obj{};

File_Dialog_class::File_Dialog_class( 	QWidget *parent,
										Interface_class* sds) ://, QSlider* sl_main_hz ) :
    Logfacility_class("FileDialog"),
    Note_class( ),
	QDialog(parent)
//    ui(new Ui::File_Dialog)
{


//	Sl_Main_Hz				= sl_main_hz;
	this->sds 	= sds;
	this->addr 	= sds->addr;
	ui 			= &UI_FileDialog_obj;

    ui->setupUi(this);


    CB_notes                = ui->cb_notefilenames;
    CB_instruments          = ui->cb_instrumentfiles;
    notes_path          	= file_structure().Dir.notesdir;
    instruments_path    	= file_structure().Dir.instrumentdir;
    string file_type		= file_structure().file_type;
    Qread_filelist( CB_notes, notes_path, file_type);
    Qread_filelist( CB_instruments, instruments_path, file_type);
    for( string str : convention_names )
    {
    	QString Qstr = QString::fromStdString( str );
    	ui->cb_convention->addItem( Qstr );
    }
    for( int nps : Notes_per_Sec )
    {
    	QString QStr = QString::number( nps );
    	ui->cb_nps->addItem( QStr );
    }

    connect(ui->sB_Octave, SIGNAL(valueChanged(int)), this, SLOT( sB_Octave(int)));
    connect(ui->cb_nps, SIGNAL(activated(int)), this, SLOT( cB_NotesPerSec(int)));
    connect(ui->cb_convention, SIGNAL(activated( int )), this, SLOT(cB_Convention( int ) ));
    connect(ui->pbPlayNotes, SIGNAL(clicked()), this, SLOT(pb_PlayNotes_OnOff() ));
    connect(ui->pbInstrumentDone, SIGNAL(clicked()), this, SLOT(pb_Instrument_Done_clicked()) );
    connect(ui->pbNotesDone, SIGNAL(clicked()), this, SLOT(pb_Notes_Done_clicked()) );

    QWidget::update();
    Comment( INFO," File_Dialog initialized");


    Setup_widgets();
}

void File_Dialog_class::sB_Octave(int sb_value )
{
	addr->noteline_prefix.Octave = sb_value;
	addr->KEY = UPDATE_NLP_KEY;
//	int freq = Octave_freq ( sb_value );
//	Sl_Main_Hz->setValue( freq );
}
void File_Dialog_class::cB_Convention( int cb_value )
{
	QString notes = QString::fromStdString( convention_notes[ cb_value ] );
	ui->lbl_selected_notes->setText( "Notes ( " + notes + " )" );
	addr->noteline_prefix.convention = cb_value;
	addr->KEY = UPDATE_NLP_KEY;
}

void File_Dialog_class::cB_NotesPerSec(int cb_value )
{
	uint8_t nps = Notes_per_Sec[ cb_value ];
	addr->noteline_prefix.nps = nps;
	addr->KEY = UPDATE_NLP_KEY;

}
void File_Dialog_class::Setup_widgets()
{
    QString QStr;
    Instrument_name = addr->Instrument;
    QStr = QString::fromStdString( Instrument_name );
    ui->lE_Instrument->setText( QStr );
    New_Instrument();

    string Notes_name =addr->Notes;
    QStr = QString::fromStdString( Notes_name );
    ui->lE_NotesFile->setText( QStr );

    Note_class::Read( Notes_name );
    string Notesline =  Note_class::Get_note_line();
    if ( this->Verify_noteline( Note_class::Noteline_prefix, Notesline ) )
    {
        status_color.setColor(QPalette::Button, Qt::green);
    }
    else
    {
        status_color.setColor(QPalette::Button, Qt::red);
    }
    ui->pbPlayNotes->setPalette(status_color);

    QStr = QString::fromStdString( Notesline );
    ui->lE_Notes->setText( QStr );

    string Rhythmline = Note_class::Get_rhythm_line();
    QStr = QString::fromStdString( Rhythmline );
    ui->lE_Rythm->setText( QStr );

    int convention_id = addr->noteline_prefix.convention;
    QStr = QString::fromStdString( convention_names[ convention_id ] );
    ui->cb_convention->setCurrentText( QStr );
    QStr = QString::fromStdString( convention_notes[convention_id ] );
    ui->lbl_selected_notes->setText("Notes ( " + QStr + " )" );

    QStr = QString::number( addr->noteline_prefix.nps );
    ui->cb_nps->setCurrentText( QStr );

//    int freq = Octave_freq( addr->noteline_prefix.Octave );
//	Sl_Main_Hz->setValue( freq );
	ui->sB_Octave->setValue( addr->noteline_prefix.Octave );

    New_Notes();

}
File_Dialog_class::~File_Dialog_class()
{
//    if( ui ) delete ui;
}


void File_Dialog_class::on_cb_instrumentfiles_activated(const QString &arg1)
{
    qDebug() << "on_cb_instrumentfiles_activated";
    qDebug() << "Instrument " << arg1 ;
    QString QStr = arg1;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        sds->Write_str( INSTRUMENTSTR_KEY, str );
        addr->KEY = SETINSTRUMENTKEY;
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
        sds->Write_str( INSTRUMENTSTR_KEY, instrument );
        addr->KEY = NEWINSTRUMENTKEY;
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

    if ( this->Verify_noteline( addr->noteline_prefix, note_line ) )
    {
        QStr = ui->lE_Rythm->text();
        string rhythm_line = QStr.toStdString();
        Note_class::Set_rhythm_line( rhythm_line );

        QStr = ui->lE_NotesFile->text();
        string notes_file = QStr.toStdString();


        Note_class::Save( notes_file, addr->noteline_prefix, note_line );

        // remote shall read and activate the new note line
        sds->Write_str( NOTESSTR_KEY, notes_file);
        addr->KEY = NEWNOTESLINEKEY;
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
        addr->KEY = NOTESOFFKEY;
    }
    else
    {
        ui->pbPlayNotes->setText( NotesON );
        addr->KEY = NOTESONKEY;

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
        string  notes_line = Note_class::Read( note_name );
        QString Notes_line = QString::fromStdString( notes_line );
        ui->lE_Notes->setText( Notes_line );
        string rhythmline = Note_class::Get_rhythm_line();
        QString Rhythmline = QString::fromStdString( rhythmline );
        ui->lE_Rythm->setText( Rhythmline );

        // remote
        sds->Write_str( NOTESSTR_KEY, note_name );
        addr->KEY = UPDATENOTESKEY; // update notes
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


