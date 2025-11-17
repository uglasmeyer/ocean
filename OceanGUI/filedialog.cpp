//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * filedialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

// Ocean includes
#include <Ocean.h>
#include <Utilities.h>
#include <data/Config.h>
#include <EventKeys.h>

// OceanGUI includes
#include <include/Common.h>
#include <include/File_Dialog.h>

File_Dialog_class::File_Dialog_class( 	QWidget*			parent,
										Dataworld_class* 	_data,
										EventLog_class*		_log) :
    Logfacility_class("FileDialog"),
    Note_class( _data->Sds_p->addr, _data->Cfg_p ),
	QDialog(parent),
	ui(new Ui::File_Dialog_class{} ) // Syntax: ptrname( new Ui::QDialog classname{} )
{

	this->DaTA 		= _data;
	this->Eventlog_p= _log;
	this->sem		= DaTA->Sem_p;
	this->fs		= DaTA->Cfg_p->fs;

	ui->setupUi(this);

	ui->cb_Notestype->clear();
    ui->cb_Notestype->addItems( QStringList{ EventXML.path.Qext, EventNTE.path.Qext } );

    QStringList
	QStrL = Qstringvector( convention_names );
    ui->cb_convention->clear();
    ui->cb_convention->addItems( QStrL );

    QStrL = Qstringvector( NpsChars.Vec );
    ui->cb_nps->clear();
    ui->cb_nps->addItems( QStrL );

	ui->sB_Octave->setMaximum( OctaveChars.Str.length()-1 );

	SetSds( DaTA->Sds_p, DaTA->Sds_p->addr->SDS_Id );

	connect(ui->cb_instrumentfiles	, SIGNAL(textActivated(QString)),this, SLOT(Instrument_Select(QString)) );
    connect(ui->cb_notefilenames	, SIGNAL(textActivated(QString)),this, SLOT(Notes_Select(QString)) );

    connect(ui->pbInstrumentDone	, SIGNAL(clicked())		,this, SLOT(Instrument_Save()) );

	connect(ui->cb_Notestype, SIGNAL(activated( int )), this, SLOT( cb_Notestype( int )));
    connect(ui->sB_Octave, SIGNAL(valueChanged(int)), this, SLOT( sB_Octave( int)));
    connect(ui->cb_nps, SIGNAL(activated(int)), this, SLOT( cB_NotesPerSec( int )));
    connect(ui->cb_convention, SIGNAL(activated( int )), this, SLOT(cB_Convention( int ) ));
    connect(ui->pbNotesDone, SIGNAL(clicked()), this, SLOT(Notes_Done_clicked()) );
    connect(ui->pB_EditMusicxml, SIGNAL(clicked()), this, SLOT(EditMusicXML()) );



    connect(ui->cb_longnote, SIGNAL(clicked(bool)), this, SLOT( Longnote(bool) ));
    Comment( INFO," File_Dialog initialized");
//    Setup_widgets();
}

File_Dialog_class::~File_Dialog_class() = default;

void File_Dialog_class::EditMusicXML()
{
	const 	string 	musicxml_editor 	= XMLEDITOR;
	const 	QString qfilename 			= ui->lE_NotesFile->text();
	const 	string 	filename 			= fs->xmldir + qfilename.toStdString()+ fs->xml_type;
	const 	string 	cmd 				= musicxml_editor + " " + filename;
			int		process_id 			= -1;

	if( filesystem::exists( filename ) )
	{
		process_id = System_execute_bg( cmd );
		DaTA->Appstate.SetState( sds_p, XMLEDITORID, RUNNING );
		DaTA->Appstate.SetPid( sds_p, XMLEDITORID, process_id );
	}
	else
		Comment( ERROR, "cannot execute", cmd );
}
void File_Dialog_class::Longnote( bool value )
{
	Sds->Set( sds_p->features[0].longplay , value );
}

void File_Dialog_class::SetSds( Interface_class* Sds, Id_t sdsid )
{
	this->Sds 		= Sds;
	this->sds_p 	= Sds->addr;
	this->SDS_ID	= sdsid;
	Assert_equal	( sdsid, sds_p->SDS_Id );

	Setup_widgets();

}

void File_Dialog_class::cb_Notestype( int cb_value )
{
	Sds->Set( sds_p->NotestypeId , (NOTETYPE_e)cb_value );
    ui->cb_notefilenames->clear();
    ui->cb_notefilenames->addItems( Qread_filenames( EventVec[ cb_value].path ) );
}

void File_Dialog_class::sB_Octave( int value )
{	// dynamically set during sound generation. see Notes->gen_chord_data
//    int8_t octave_shift = value;// - sds_p->noteline_prefix.Octave;

	Sds->Set( sds_p->noteline_prefix.octave_shift , (int8_t) value );
	Eventlog_p->add( SDS_ID, UPDATE_NLP_KEY );
}

void File_Dialog_class::cB_Convention( int cb_value )
{
	QString notes = Qstring( convention_notes[ cb_value ] );
	ui->lbl_selected_notes->setText( "Notes ( " + notes + " )" );
	Sds->Set( sds_p->noteline_prefix.convention , (uint8_t)cb_value );
	Eventlog_p->add( SDS_ID,  UPDATE_NLP_KEY );
}

void File_Dialog_class::cB_NotesPerSec( int nps_id )
{
	uint8_t nps = ui->cb_nps->currentText().toInt();
	Sds->Set( sds_p->noteline_prefix.nps , nps );
	Eventlog_p->add( SDS_ID,  UPDATE_NLP_KEY );
}

void File_Dialog_class::Setup_widgets()
{
    uint8_t notestypeId = sds_p->NotestypeId;
	ui->cb_Notestype->setCurrentIndex( notestypeId );

    QString Instrument_name = QReadStr( Sds, INSTRUMENTSTR_KEY );
    ui->lE_Instrument->setText( Instrument_name );
    ui->cb_instrumentfiles->clear();
    ui->cb_instrumentfiles->addItems( Qread_filenames( EventINS.path ));
    ui->cb_instrumentfiles->setCurrentText( Instrument_name );

    QString Notes_name = QReadStr( Sds, NOTESSTR_KEY ) ;
    ui->lE_NotesFile->setText( Notes_name  );
    ui->cb_notefilenames->clear();
    ui->cb_notefilenames->addItems( Qread_filenames( EventVec[ notestypeId ].path ));
    ui->cb_notefilenames->setCurrentText( Notes_name );
    ui->cb_longnote->setChecked( sds_p->features[0].longplay );

    if ( sds_p->NotestypeId == XML_ID )
    	return;

    // nte section follows
    Note_class::Read( Notes_name.toStdString() );
    string Notesline =  Note_class::Get_note_line();
    bool verifyed = Verify_noteline( Noteline_prefix, Notesline );
    setButton( ui->pbNotesDone, verifyed );

    ui->lE_Notes->setText( Qstring( Notesline ) );

    string Rhythmline = Note_class::Get_rhythm_line();
    QString
    QStr = Qstring( Rhythmline );
    ui->lE_Rythm->setText( QStr );

    int convention_id = sds_p->noteline_prefix.convention;
    QStr = Qstring( convention_names[ convention_id ] );
    ui->cb_convention->setCurrentText( QStr );

    QStr = Qstring( convention_notes[convention_id ] );
    ui->lbl_selected_notes->setText("Notes ( " + QStr + " )" );

    QStr = QString::number( sds_p->noteline_prefix.nps );
    ui->cb_nps->setCurrentText( QStr );

	ui->sB_Octave->setValue( sds_p->noteline_prefix.octave_shift );
}





// ----------------------------------------------------------------------------- Notes
void File_Dialog_class::New_Notes()
{
	if ( sds_p->NotestypeId == XML_ID )
	{
		Eventlog_p->add( SDS_ID,  EventVec[ XML_ID ].event );
    	return;
	}

    QString QStr = ui->lE_Notes->text();
    string note_line = QStr.toStdString();
    bool verified = Verify_noteline( sds_p->noteline_prefix, note_line );
    setButton( ui->pbNotesDone, verified );
    if ( verified )
    {
        QStr = ui->lE_Rythm->text();
        Note_class::Set_rhythm_line( QStr.toStdString() );

        QStr = ui->lE_NotesFile->text();
        string notes_file = QStr.toStdString();

        Note_class::Save( notes_file, sds_p->noteline_prefix, note_line );

        // remote shall read and activate the new note line
        Sds->Write_str( NOTESSTR_KEY, notes_file);

        Eventlog_p->add( SDS_ID,  EventVec[NTE_ID].event );
        Info( "sync notes" );
    	sem->Release( SEMAPHORE_SYNCNOTES );

    }
}

void File_Dialog_class::Notes_Select(QString Note_name)
{
    if ( Note_name.length() == 0 ) return;

    ui->lE_NotesFile->setText( Note_name );

    string note_name = Note_name.toStdString();
    Sds->Write_str( NOTESSTR_KEY, note_name );
    uint8_t notestypeId = sds_p->NotestypeId;

    if ( notestypeId == XML_ID ) return;

	ui->lE_Notes->setText( Qstring( Note_class::Read( note_name ) ));

	ui->lE_Rythm->setText( Qstring( Note_class::Get_rhythm_line() ) );
}


void File_Dialog_class::Instrument_Select(QString QStr)
{
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( INSTRUMENTSTR_KEY, str );
        Eventlog_p->add( SDS_ID,  EventINS.event );
    }
    ui->lE_Instrument->setText( QStr );
}

void File_Dialog_class::Instrument_Save()
{
    const QString QStr 	= ui->lE_Instrument->text();
    string str			= QStr.toStdString();
    Sds->Write_str( INSTRUMENTSTR_KEY, str );

	Eventlog_p->add( SDS_ID, NEWINSTRUMENTKEY );
}
void File_Dialog_class::Notes_Done_clicked()
{
    New_Notes();
}


