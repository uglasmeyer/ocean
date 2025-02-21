// System includes
#include <data/Interface.h>
#include <dirent.h>

// qtcreator includes
#include "ui_File_Dialog.h"
#include <File_Dialog.h>
#include <Mainwindow.h>
#include <Common.h>

// Synthesizer includes
#include <Keys.h>
#include <Logfacility.h>
#include <notes/Notes.h>
#include <Ocean.h>
#include <Config.h>

// Qt-includes
#include <QDebug>
#include <QString>



//Ui::File_Dialog	UI_FileDialog_obj{};

File_Dialog_class::File_Dialog_class( 	QWidget *parent,
										Interface_class* Sds,
										Semaphore_class* sem) ://, QSlider* sl_main_hz ) :
    Logfacility_class("FileDialog"),
    Note_class( ),
	QDialog(parent),
    ui(new Ui::File_Dialog)
{


	this->Sds 	= Sds;
	this->sds_p	= Sds->addr;
	this->sem	= sem;

	ui->setupUi(this);

	ui->cb_Notestype->clear();
    ui->cb_Notestype->addItems( QStringList{ Event0.path.Qext, Event1.path.Qext } );

    QStringList
	QStrL = Qstringvector( convention_names );
    ui->cb_convention->clear();
    ui->cb_convention->addItems( QStrL );

    QStrL = Qstringvector( NpsChars.Vec );
    ui->cb_nps->clear();
    ui->cb_nps->addItems( QStrL );

	ui->sB_Octave->setMaximum( OctaveChars.Str.length()-1 );

	connect(ui->cb_Notestype, SIGNAL(activated( int )), this, SLOT( cb_Notestype( int )));
    connect(ui->sB_Octave, SIGNAL(valueChanged(int)), this, SLOT( sB_Octave(int)));
    connect(ui->cb_nps, SIGNAL(activated(int)), this, SLOT( cB_NotesPerSec(int)));
    connect(ui->cb_convention, SIGNAL(activated( int )), this, SLOT(cB_Convention( int ) ));
    connect(ui->pbInstrumentDone, SIGNAL(clicked()), this, SLOT(pb_Instrument_Done_clicked()) );
    connect(ui->pbNotesDone, SIGNAL(clicked()), this, SLOT(pb_Notes_Done_clicked()) );

    Comment( INFO," File_Dialog initialized");
    Setup_widgets();
}
File_Dialog_class::~File_Dialog_class()
{
 //   if( ui ) delete( ui );
}

void File_Dialog_class::SetSds( Interface_class* Sds, int8_t id )
{
	this->Sds 		= Sds;
	this->sds_p 	= Sds->addr;
    Comment( INFO," File_Dialog set to SDS Id: " + to_string( (int) id ));

	Setup_widgets();

}

void File_Dialog_class::sB_Octave(int sb_value )
{
	Sds->Set( sds_p->noteline_prefix.Octave , sb_value );
	Sds->Set( sds_p->KEY , UPDATE_NLP_KEY );
}

void File_Dialog_class::cb_Notestype( int cb_value )
{
	Sds->Set( sds_p->NotestypeId 	, cb_value );
    ui->cb_notefilenames->clear();
    ui->cb_notefilenames->addItems( Qread_filenames( Path_vec[ cb_value].path ) );
}

void File_Dialog_class::cB_Convention( int cb_value )
{
	QString notes = QString::fromStdString( convention_notes[ cb_value ] );
	ui->lbl_selected_notes->setText( "Notes ( " + notes + " )" );
	Sds->Set( sds_p->noteline_prefix.convention , cb_value );
	Sds->Set( sds_p->KEY , UPDATE_NLP_KEY );
}

void File_Dialog_class::cB_NotesPerSec(int nps_id )
{
	int nps = ui->cb_nps->currentText().toInt();
	Sds->Set( sds_p->noteline_prefix.nps , nps );
	Sds->Set( sds_p->KEY , UPDATE_NLP_KEY );
}


void File_Dialog_class::Setup_widgets()
{
    uint8_t notestypeId = sds_p->NotestypeId;
	ui->cb_Notestype->setCurrentIndex( notestypeId );

    QString Instrument_name = QReadStr( Sds, INSTRUMENTSTR_KEY );
    ui->lE_Instrument->setText( Instrument_name );
    ui->cb_instrumentfiles->clear();
    ui->cb_instrumentfiles->addItems( Qread_filenames( Event2.path ));
    ui->cb_instrumentfiles->setCurrentText( Instrument_name );

    QString Notes_name = QReadStr( Sds, NOTESSTR_KEY ) ;
    ui->lE_NotesFile->setText( Notes_name  );
    ui->cb_notefilenames->clear();
    ui->cb_notefilenames->addItems( Qread_filenames( Path_vec[ notestypeId ].path ));
    ui->cb_notefilenames->setCurrentText( Notes_name );

    if ( sds_p->NotestypeId == XML_ID )
    	return;

    // nte section follows
    Note_class::Read( Notes_name.toStdString() );
    string Notesline =  Note_class::Get_note_line();
    bool verifyed = Verify_noteline( Noteline_prefix, Notesline );
    setButton( ui->pbNotesDone, verifyed );

    ui->lE_Notes->setText( QString::fromStdString( Notesline ) );

    string Rhythmline = Note_class::Get_rhythm_line();
    QString
    QStr = QString::fromStdString( Rhythmline );
    ui->lE_Rythm->setText( QStr );

    int convention_id = sds_p->noteline_prefix.convention;
    QStr = QString::fromStdString( convention_names[ convention_id ] );
    ui->cb_convention->setCurrentText( QStr );

    QStr = QString::fromStdString( convention_notes[convention_id ] );
    ui->lbl_selected_notes->setText("Notes ( " + QStr + " )" );

    QStr = QString::number( sds_p->noteline_prefix.nps );
    ui->cb_nps->setCurrentText( QStr );

	ui->sB_Octave->setValue( sds_p->noteline_prefix.Octave );
}

void File_Dialog_class::on_cb_instrumentfiles_activated(const QString &QStr)
{
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( INSTRUMENTSTR_KEY, str );
        Sds->Set( sds_p->KEY , SETINSTRUMENTKEY );
    }
    ui->lE_Instrument->setText( QStr );
}

void File_Dialog_class::New_Instrument()
{
    const QString QStr = ui->lE_Instrument->text();
    on_cb_instrumentfiles_activated( QStr );
}




// ----------------------------------------------------------------------------- Notes
void File_Dialog_class::New_Notes()
{
	if ( sds_p->NotestypeId == XML_ID )
	{
		Sds->Set( sds_p->KEY , Path_vec[ XML_ID ].event );
    	return;
	}

    QString QStr = ui->lE_Notes->text();
    string note_line = QStr.toStdString();
    bool verified = Verify_noteline( sds_p->noteline_prefix, note_line );
    setButton( ui->pbNotesDone, verified );
    if ( verified )
    {
        QStr = ui->lE_Rythm->text();
        string rhythm_line = QStr.toStdString();
        Note_class::Set_rhythm_line( rhythm_line );

        QStr = ui->lE_NotesFile->text();
        string notes_file = QStr.toStdString();

        Note_class::Save( notes_file, sds_p->noteline_prefix, note_line );

        // remote shall read and activate the new note line
        Sds->Write_str( NOTESSTR_KEY, notes_file);

        Sds->Set( sds_p->KEY , Path_vec[NTE_ID].event );
        Info( "sync notes" );
    	sem->Release( SEMAPHORE_SYNCNOTES );

    }
}

void File_Dialog_class::on_cb_notefilenames_activated(const QString& Note_name)
{
    if ( Note_name.length() == 0 ) return;

    ui->lE_NotesFile->setText( Note_name );

    string note_name = Note_name.toStdString();
    Sds->Write_str( NOTESSTR_KEY, note_name );
    uint8_t notestypeId = sds_p->NotestypeId;
    Sds->Set( sds_p->KEY , Path_vec[ notestypeId ].event);

    if ( notestypeId == XML_ID ) return;

	string  notes_line = Note_class::Read( note_name );
	QString Notes_line = QString::fromStdString( notes_line );
	ui->lE_Notes->setText( Notes_line );

	string rhythmline = Note_class::Get_rhythm_line();
	QString Rhythmline = QString::fromStdString( rhythmline );
	ui->lE_Rythm->setText( Rhythmline );
}

void File_Dialog_class::pb_Instrument_Done_clicked()
{
    New_Instrument();
}
void File_Dialog_class::pb_Notes_Done_clicked()
{
    New_Notes();
}


