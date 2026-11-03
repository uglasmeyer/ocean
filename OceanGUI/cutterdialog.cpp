/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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


#include <Cutterdialog.h>
#include <GUIcommon.h>

CutDesk_Dialog_class::CutDesk_Dialog_class(	QWidget* 			parent,
											Dataworld_class* 	data,
											EventLog_class* 	el )
    : Interface_base( data )
	, QDialog(parent)
    , ui(new Ui::CutDesk_Dialog_class)
{
	this->Eventlog		= el;

    ui->setupUi(this);
	QStringList Qbps_str_lst = Qstringlist( bps_struct().Bps_lst );
    ui->cB_Stepwidth->addItems	( Qbps_str_lst );

    connect(ui->pb_back_right	, SIGNAL(clicked() )	,this, SLOT(Step_forward() ));
    connect(ui->pB_back_left	, SIGNAL(clicked() )	,this, SLOT(Step_backward() ));
    connect(ui->pb_front_right	, SIGNAL(clicked() )	,this, SLOT(Step_front_forward() ));
    connect(ui->pb_front_left	, SIGNAL(clicked() )	,this, SLOT(Step_front_backward() ));
    connect(ui->pB_goto_end		, SIGNAL(clicked() )	,this, SLOT(Step_to_end() ));
    connect(ui->pB_Cut			, SIGNAL(clicked() )	,this, SLOT(Cut_tail() ));
    connect(ui->pB_Save			, SIGNAL(clicked() )	,this, SLOT(Save() ));
    connect(ui->pB_rotate_left	, SIGNAL(clicked() )	,this, SLOT(Rotate_left() ));
    connect(ui->pB_rotate_right	, SIGNAL(clicked() )	,this, SLOT(Rotate_right() ));
    connect(ui->cB_Stepwidth	, SIGNAL(activated(int)),this, SLOT(Stepwidth(int) ));

    move( 0,0 );
	updateCutDesk();
}

CutDesk_Dialog_class::~CutDesk_Dialog_class()
{
    delete ui;
}

void CutDesk_Dialog_class::Dialog()
{
	if( not this->isVisible() )
	{
	    if( DaTA_p->Appstate.IsRunning( sds_p, SYNTHID ) )
	    {
		    Setup();
		    DaTA_p->Sem_p->Lock( PROCESSOR_WAIT );
			if( sds_p->WD_state.mode == RANGE_WD )
			{
			    StAId_e staid = GetStaid( sds_p->WD_state.roleId );
			    if ( not CutIds.contains( staid ) )
			    	return;
			    QString qTitle = "CutDesk ( " + Qstring( GetName( staid ) ) + " )";
			    this->setWindowTitle( qTitle );
				this->show();
			}
	    }
	}
	else
	{
		Eventlog->add( SDS_ID, CUT_RESTORE_KEY );
    	this->hide();
	}

}
void CutDesk_Dialog_class::Stepwidth( int idx )
{
	uint8_t width = bps_struct().Bps_vec[ idx ];
	Sds->Set( sds_p->WD_state.record_steps, width );
    Eventlog->add( SDS_ID, CUT_STEPWIDTH_KEY );
}
void CutDesk_Dialog_class::Rotate_left()
{
	Sds->Set( sds_p->WD_state.direction, ROTATE_LEFT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::Rotate_right()
{
	Sds->Set( sds_p->WD_state.direction, ROTATE_RIGHT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::Step_forward()
{
	Sds->Set( sds_p->WD_state.direction, BACK_RIGHT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::Step_backward()
{
	Sds->Set( sds_p->WD_state.direction, BACK_LEFT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::Step_front_forward()
{
	Sds->Set( sds_p->WD_state.direction, FRONT_RIGHT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );

}
void CutDesk_Dialog_class::Step_front_backward()
{
	Sds->Set( sds_p->WD_state.direction, FRONT_LEFT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::Step_to_end()
{
	Sds->Set( sds_p->WD_state.direction, GOTO_END );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutDesk_Dialog_class::updateCutDesk()
{
	string	str = sds_master->WAV_filename;
	ui->lbl_wav_file->setText( Qstring( str ) );
	ui->lcdNumber->display( sds_p->WD_state.cursor.min );
	ui->lcdNumber_2->display( sds_p->WD_state.cursor.max );
//	coutf << "CutDesk_Dialog_class::updateCutDesk" << endl;
}

void CutDesk_Dialog_class::Cut_tail()
{
	Eventlog->add( SDS_ID, CUT_KEY );
}
void CutDesk_Dialog_class::Save()
{
	Eventlog->add( SDS_ID, CUT_SAVE_KEY );
}

void CutDesk_Dialog_class::Setup()
{
	Eventlog->add(SDS_ID, CUT_SETUP_KEY );
	updateCutDesk();
}

void CutDesk_Dialog_class::SetSds()
{
	Interface_base::Set_Sds();
	updateCutDesk();
}



