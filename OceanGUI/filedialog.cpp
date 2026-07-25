/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

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
#include <GUIcommon.h>
#include <include/File_Dialog.h>

File_Dialog_class::File_Dialog_class( 	QWidget*			parent,
										Dataworld_class* 	_data,
										EventLog_class*		_log,
										WfDisplay_que_struct*	WfD)
	: Interface_base	( _data )
	, QDialog			( parent )
	, Musicxml			( _data->Cfg_p->fs )
	, ui				( new Ui::File_Dialog_class )
	, Noteline			( &sds_p->Noteline, SDSLONG_SIZE )
	, Rythmline			( &sds_p->Rythmline, SDSLONG_SIZE )
	, Noteline_prefix	( &sds_p->Noteline_prefix )
{
	this->Eventlog_p	= _log ;
	this->sem			= DaTA_p->Sem_p;
	this->WfDisplay_que = WfD;

	ui->setupUi			(this);
	SetSds				();

	init_Direvent		();
	init_Combobox		();

	ui->sB_Octave->setMinimum( octave_range.min );
	ui->sB_Octave->setMaximum( octave_range.max );

	updateWidgets();

	connect(ui->cb_instrumentfiles	, SIGNAL( textActivated(QString))		,
			this					, SLOT	( Instrument_Select(QString)) )	;
    connect(ui->cb_notefilenames	, SIGNAL( textActivated(QString))		,
    		this					, SLOT	( Notes_Select(QString)) )		;
    connect(ui->pbInstrumentDone	, SIGNAL( clicked())					,
    		this					, SLOT	( Instrument_Save()) )			;
	connect(ui->cb_Notestype		, SIGNAL( activated( int ))				,
			this					, SLOT	( cb_Notestype( int )))			;
    connect(ui->sB_Octave			, SIGNAL( valueChanged( int ))			,
    		this					, SLOT	( sB_Octave( int )))			;
    connect(ui->cb_nps				, SIGNAL( activated( int ))				,
    		this					, SLOT	( cB_NotesPerSec( int )))		;
    connect(ui->cb_convention		, SIGNAL( activated( int ))				,
    		this					, SLOT	( cB_Convention( int ) ))		;
    connect(ui->pbNotesDone			, SIGNAL( clicked())					,
    		this					, SLOT	( Notes_Done_clicked()) )		;
    connect(ui->pB_EditMusicxml		, SIGNAL( clicked())					,
    		this					, SLOT	( EditMusicXML()) )				;
    connect(ui->cb_longnote			, SIGNAL( clicked( bool ))				,
    		this					, SLOT	( Longnote( bool ) ))			;
    connect(ui->cb_score			, SIGNAL( textActivated( QString ))		,
    		this					, SLOT	( Scorename( QString ) ))		;

    move( 0, 100 );
    Comment( INFO," File_Dialog initialized");
}

File_Dialog_class::~File_Dialog_class()
{
	delete ui;
}
void File_Dialog_class::Dialog()
{
	if( this->isVisible() )
	{
		this->hide();
	}
	else
	{
	    updateWidgets();
		this->show();
	}

}

void File_Dialog_class::init_Direvent()
{
	EventXML		= direvent_struct( XML_ID, ui->cb_notefilenames, UPDATENOTESKEY,
										&sds_p->Notes, fs->xmldir, fs->xml_type );
	EventNTE		= direvent_struct( NTE_ID, ui->cb_notefilenames, UPDATENOTESKEY,
										&sds_p->Notes, fs->notesdir		, fs->nte_type );
	EventINS		= direvent_struct( DIRINS_ID, ui->cb_instrumentfiles, SETINSTRUMENTKEY	,
										&sds_p->Instrument, fs->instrumentdir	, fs->snd_type );
	EventSES		= direvent_struct( DIRSES_ID, ui->cb_score, SETSESSIONDIR_KEY	,
										&sds_master->Session, fs->includedir	, fs->synth_type );
	EventVec		= { &EventXML, &EventNTE, &EventINS, &EventSES };
	Scorename		( sds_master->Session );

}
void File_Dialog_class::init_Combobox()
{
	ui->cb_Notestype->clear();
    ui->cb_Notestype->addItems( QStringList{ EventXML.path.Qext, EventNTE.path.Qext } );

    ui->cb_convention->clear();
    ui->cb_convention->addItems( Qstringvector( convention_names ) );

    ui->cb_nps->clear();
    ui->cb_nps->addItems( Qstringvector( Noteline_prefix.Bps_str_vec ) );
}
void File_Dialog_class::EditMusicXML()
{
	const 	string	notename 			= ui->lE_NotesFile->text().toStdString();
	const 	string 	filename 			= fs->xmldir + notename + fs->xml_type;
 	const	string 	cmd					= string(XMLEDITOR) + " " + filename;

	if( filesystem::exists( filename ) )
	{
		int	process_id	= System_execute_bg( cmd, "/tmp/nohup.out" );
		if( process_id > 0 )
		{
			DaTA_p->Appstate.SetRunning( sds_p, XMLEDITORID );
			DaTA_p->Appstate.SetPid( sds_p, XMLEDITORID, process_id );
			return;
		}
	}
	Comment( ERROR, "cannot execute", cmd );
}

void File_Dialog_class::Longnote( bool value )
{
	if( sds_master->mixer_state.composer ) return;
	Sds->Set( sds_p->features[0].longplay , value );
}

void File_Dialog_class::change_dirs( string subdir )
{
	for(direvent_t* event : EventVec )
	{
		event->SetSubdir( subdir );
	}
	EventVec[sds_p->NotesTypeId]->Update_cb();
	EventVec[DIRINS_ID]->Update_cb();
	EventVec[DIRSES_ID]->Update_cb();

}
void File_Dialog_class::Scorename( QString qname )
{
	string scorename 	= qname.toStdString();
	auto nothing_todo = [ this, scorename ](  )
	{
		return false;//strEqual( scorename, string( sds_master->Session ) );
	};

	if( nothing_todo() )
		return;
	if( sds_master->mixer_state.composer )
	{
		scorename = sds_master->Session;
	}
	else
	{
		sdsCommand( SAVESESSION_KEY );
		Sds_master->Write_str	( SESSIONSTR, scorename );
		sdsCommand( SETSESSIONDIR_KEY );
	}
	change_dirs		( scorename );
}

void File_Dialog_class::sdsCommand( EVENTKEY_e key )
{
	if( sds_master->mixer_state.composer ) return;
	Eventlog_p->add( SDS_ID, key );
}
void File_Dialog_class::SetSds()
{
	Interface_base::Set_Sds		();
	Noteline.cstr_p				( &sds_p->Noteline );
	Rythmline.cstr_p			( &sds_p->Rythmline );
	Noteline_prefix.Nlp_data_p	( &sds_p->Noteline_prefix );
}

void File_Dialog_class::cb_Notestype( int cb_value )
{
	Sds->Set( sds_p->NotesTypeId, (NOTETYPE_e)cb_value );
	EventVec[ cb_value ]->Update_cb();
}

void File_Dialog_class::sB_Octave( int value )
{	// dynamically set during sound generation. see Notes->gen_chord_data

	uint8_t oct = uint8_t(value);
	Sds->Set( sds_p->Noteline_prefix.Octave , oct );
	Sds->Set( sds_p->Noteline_prefix.origin, NOTE::GUI );
	sdsCommand( GENERATENOTES_KEY );
}

void File_Dialog_class::cB_Convention( int cb_value )
{
	QString notes = Qstring( convention_notes[ cb_value ] );
	ui->lbl_selected_notes->setText( "Notes ( " + notes + " )" );
	Sds->Set( sds_p->Noteline_prefix.convention , (uint8_t)cb_value );
	Sds->Set( sds_p->Noteline_prefix.origin, NOTE::GUI );
	sdsCommand( GENERATENOTES_KEY );
}

void File_Dialog_class::cB_NotesPerSec( int nps_id )
{
	uint8_t nps = ui->cb_nps->currentText().toInt();
	if( nps == 0 )
		nps = 1;
	Sds->Set( sds_p->Noteline_prefix.nps , nps );
	Sds->Set( sds_p->Noteline_prefix.origin, NOTE::GUI );
	sdsCommand( GENERATENOTES_KEY );
}

void File_Dialog_class::update_notesWidgets( 	const string& nl,
												const string& rl,
												const string& sn)
{
    ui->lE_Notes->setText( Qstring( nl ) );
    ui->lE_Rythm->setText( Qstring( rl ) );
    ui->lbl_selected_notes->setText( Qstring( sn ) );
}

void File_Dialog_class::updateWidgets()
{
	QString score_qstr 					= Qstring( string(sds_master->Session) );
	ui->cb_score->setCurrentText		( score_qstr );

    ui->lE_Instrument->setText			( Qstring( sds_p->Instrument ) );
    ui->lE_NotesFile->setText			( Qstring( sds_p->Notes ));
    ui->cb_longnote->setChecked			( sds_p->features[0].longplay );
	ui->cb_Notestype->setCurrentIndex	( sds_p->NotesTypeId );

	string note_line 		= "";
	string rhythm_line		= "";
	string selected_notes 	= "";
    if ( sds_p->NotesTypeId == NTE_ID )
    {

		nlp_data_t nlp 	= Noteline_prefix.Read();
		ui->cb_nps->setCurrentText	( QString::number( nlp.nps ) );
		ui->sB_Octave->setValue		( nlp.Octave );
		ui->cb_convention->setCurrentText( Qstring( convention_names[ nlp.convention ] ) );
		selected_notes 	= "Notes ( " + convention_notes[ nlp.convention ] + " )";

		rhythm_line		= Rythmline.read();
		note_line 		= Noteline.read();
    }
    setButton			( ui->pbNotesDone, 3*(int)sds_p->NotesVerified );

    update_notesWidgets	( note_line, rhythm_line, selected_notes );
 }




// ----------------------------------------------------------------------------- Notes

void File_Dialog_class::Notes_Done_clicked()
{
	if( sds_master->mixer_state.composer )
		return;
	WfDisplay_que->Add( NOTESROLE );
    sdsCommand( SETWAVEDISPLAYKEY );

	if ( sds_p->NotesTypeId == XML_ID )
	{
		sdsCommand(  EventXML.event );
    	sem->Release( SEMAPHORE_SYNCNOTES );

    	return;
	}

	Noteline_prefix.data.origin = NOTE::GUI;
	Noteline_prefix.Save();
    QString QStr = ui->lE_Notes->text();
    Noteline.save( QStr.toStdString() );

    QStr = ui->lE_Rythm->text();
	Rythmline.save( QStr.toStdString() );

	QStr = ui->lE_NotesFile->text();
	Sds->Write_str( NOTENAMESTR, QStr.toStdString() );

	sdsCommand( UPDATENOTESKEY );
	sdsCommand( SAVENOTES_KEY );

}

void File_Dialog_class::Notes_Select(QString Qnote_filename)
{	// update
	// dialog fields: le_notesfile, le_notes, le_rhythm and
	// string sds notes from
	// combo box cb_notefilenames
	// while the content of cb_notefilenames is updated by dircontent cb_Update

    if ( Qnote_filename.length() == 0 )
    	return;

    ui->lE_NotesFile->setText( Qnote_filename );

    string notefile_name = Qnote_filename.toStdString() ;
    Sds->Write_str( NOTENAMESTR, notefile_name );

    if ( sds_p->NotesTypeId == NTE_ID )
    {
    	sdsCommand( READNOTESFILE_KEY );
    }
    else
    {
    	ui->lE_Notes->setText( "" );
    	ui->lE_Rythm->setText( "" );
    }
}

void File_Dialog_class::Instrument_Select(QString QStr)
{
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( INSTRUMENTSTR, str );
        sdsCommand( EventINS.event );
    }
    ui->lE_Instrument->setText( QStr );
}

void File_Dialog_class::Instrument_Save()
{
    const QString QStr 	= ui->lE_Instrument->text();
    string str			= QStr.toStdString();
    Sds->Write_str( INSTRUMENTSTR, str );

	sdsCommand( SAVEINSTRUMENTKEY );
}


