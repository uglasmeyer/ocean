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
//	ui 			= &UI_FileDialog_obj;

    ui->setupUi(this);


    CB_notes                = ui->cb_notefilenames;
    CB_instruments          = ui->cb_instrumentfiles;
    NotesDir          	= file_structure().Dir.xmldir;
    instruments_path    	= file_structure().Dir.instrumentdir;


    QNotestypes	= Qstringvector( file_structure().Notestypes );
    QNotesdirs	= Qstringvector( file_structure().Notesdirs );
    Noteskeys 	= { XMLFILE_KEY, UPDATENOTESKEY};

    int notestype = sds_p->NotestypeId;

    ui->cb_Notestype->addItems( QNotestypes );
    ui->cb_Notestype->setCurrentText( QNotestypes [ notestype] );

    CB_notes->clear();
    CB_notes->addItems( Qread_filenames(file_structure().Notesdirs[ notestype ],
    									file_structure().Notestypes [ notestype] ) );


    CB_instruments->clear();
	CB_instruments->addItems( Qread_filenames( instruments_path , file_structure().snd_type ) );

    QStringList
	QStrL = Qstringvector( convention_names );
    ui->cb_convention->addItems( QStrL );

    QStrL = Qstringvector( NpsChars.Vec );
	ui->cb_nps->addItems( QStrL );

	ui->sB_Octave->setMaximum( OctaveChars.Str.length()-1 );

	connect(ui->cb_Notestype, SIGNAL(activated( int )), this, SLOT( cb_Notestype( int )));
    connect(ui->sB_Octave, SIGNAL(valueChanged(int)), this, SLOT( sB_Octave(int)));
    connect(ui->cb_nps, SIGNAL(activated(int)), this, SLOT( cB_NotesPerSec(int)));
    connect(ui->cb_convention, SIGNAL(activated( int )), this, SLOT(cB_Convention( int ) ));
    connect(ui->pbInstrumentDone, SIGNAL(clicked()), this, SLOT(pb_Instrument_Done_clicked()) );
    connect(ui->pbNotesDone, SIGNAL(clicked()), this, SLOT(pb_Notes_Done_clicked()) );

    QWidget::update();
    Comment( INFO," File_Dialog initialized");


    Setup_widgets();
}


void File_Dialog_class::SetSds( Interface_class* Sds, int8_t id )
{
	this->Sds 		= Sds;
	this->sds_p 	= Sds->addr;
//	this->SDS_ID	= id;
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
    NotesDir         	= QNotesdirs[ cb_value ].toStdString() ;
    notes_type			= QNotestypes[ cb_value ].toStdString();
    CB_notes->clear();
    CB_notes->addItems( Qread_filenames( NotesDir, notes_type ) );
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

    QString QStr;
    Instrument_name = sds_p->Instrument;
    QStr = QString::fromStdString( Instrument_name );
    ui->lE_Instrument->setText( QStr );
    ui->cb_instrumentfiles->setCurrentText(QStr);
    New_Instrument();

    string Notes_name =sds_p->Notes;
    QStr = QString::fromStdString( Notes_name );
    ui->lE_NotesFile->setText( QStr );
    ui->cb_notefilenames->setCurrentText(QStr);

    if ( sds_p->NotestypeId == 0 )
    	return;

    // nte section follows
    Note_class::Read( Notes_name );
    string Notesline =  Note_class::Get_note_line();
    if ( this->Verify_noteline( Note_class::Noteline_prefix, Notesline ) )
    {
    	Set_button_color( ui->pbNotesDone, Qt::green );
    }
    else
    {
    	Set_button_color( ui->pbNotesDone, Qt::red );
    }

    QStr = QString::fromStdString( Notesline );
    ui->lE_Notes->setText( QStr );

    string Rhythmline = Note_class::Get_rhythm_line();
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

//    New_Notes();

}
File_Dialog_class::~File_Dialog_class()
{
 //   if( ui ) delete( ui );
}

void File_Dialog_class::Set_button_color( QPushButton* pb, QColor color  )
{
	QPalette status_color = QPalette();
	status_color.setColor(QPalette::Button, color);
	pb->setPalette( status_color );
};

void File_Dialog_class::on_cb_instrumentfiles_activated(const QString &arg1)
{
    QString QStr = arg1;
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
    QString QStr = ui->lE_Instrument->text();
    string instrument = QStr.toStdString();
//    qDebug() << "new Instrument " << QStr ;

    if ( instrument.length() > 0 )
    {
        Sds->Write_str( INSTRUMENTSTR_KEY, instrument );
        Sds->Set( sds_p->KEY , NEWINSTRUMENTKEY );
    }

}




// ----------------------------------------------------------------------------- Notes
void File_Dialog_class::New_Notes()
{
	if ( sds_p->NotestypeId == 0 )
	{
		Sds->Set( sds_p->KEY , Noteskeys[ 0 ] );
    	return;
	}

    QString QStr = ui->lE_Notes->text();
    string note_line = QStr.toStdString();

    if ( this->Verify_noteline( sds_p->noteline_prefix, note_line ) )
    {
        QStr = ui->lE_Rythm->text();
        string rhythm_line = QStr.toStdString();
        Note_class::Set_rhythm_line( rhythm_line );

        QStr = ui->lE_NotesFile->text();
        string notes_file = QStr.toStdString();


        Note_class::Save( notes_file, sds_p->noteline_prefix, note_line );

        // remote shall read and activate the new note line
        Sds->Write_str( NOTESSTR_KEY, notes_file);

        Sds->Set( sds_p->KEY , Noteskeys[ 1 ] );

    	Set_button_color( ui->pbNotesDone, Qt::green );
    	sem->Release( SEMAPHORE_SYNCNOTES );

    }
    else
    {
    	Set_button_color( ui->pbNotesDone, Qt::red );
    }

}



void File_Dialog_class::on_cb_notefilenames_activated(const QString &arg1)
{
//    qDebug()    <<"on_cb_notefilenames_activated";
//    qDebug() << "Notes name: " << arg1 ;
    if ( arg1.length() == 0 )
    	return;

    QString Note_name = arg1;
    ui->lE_NotesFile->setText( Note_name );
    string note_name = Note_name.toStdString();
    Sds->Write_str( NOTESSTR_KEY, note_name );

    if ( sds_p->NotestypeId == 0 )
    {
    	Sds->Set( sds_p->KEY , Noteskeys[ 0 ] );
		return;
    }

	// local noteline
	string  notes_line = Note_class::Read( note_name );
	QString Notes_line = QString::fromStdString( notes_line );
	ui->lE_Notes->setText( Notes_line );

	string rhythmline = Note_class::Get_rhythm_line();
	QString Rhythmline = QString::fromStdString( rhythmline );
	ui->lE_Rythm->setText( Rhythmline );

	// remote

	Sds->Set( sds_p->KEY , Noteskeys[ 1 ] );
}

void File_Dialog_class::pb_Instrument_Done_clicked()
{
    New_Instrument();
}
void File_Dialog_class::pb_Notes_Done_clicked()
{
    New_Notes();
}


