#include "Cutterdialog.h"
#include "ui_CutDesk_Dialog.h"

CutterDialog_class::CutterDialog_class(QWidget *parent,
		Dataworld_class* data,
		EventLog_class* el)
    : QDialog(parent)
    , ui(new Ui::CutterDialog_class)
{
	this->DaTA			= data;
	this->sds_master	= DaTA->sds_master;
	this->Eventlog		= el;
	setSds( DaTA->Sds_p );

    ui->setupUi(this);
    connect(ui->pb_back_right	, SIGNAL(clicked() ),this, SLOT(Step_forward() ));
    connect(ui->pB_back_left	, SIGNAL(clicked() ),this, SLOT(Step_backward() ));
    connect(ui->pb_front_right	, SIGNAL(clicked() ),this, SLOT(Step_front_forward() ));
    connect(ui->pb_front_left	, SIGNAL(clicked() ),this, SLOT(Step_front_backward() ));
    connect(ui->pB_goto_end		, SIGNAL(clicked() ),this, SLOT(Step_to_end() ));
    connect(ui->pB_Cut			, SIGNAL(clicked() ),this, SLOT(Cut_tail() ));
    connect(ui->pB_Save			, SIGNAL(clicked() ),this, SLOT(Save() ));

	updateCutDesk();
}

CutterDialog_class::~CutterDialog_class()
{
    delete ui;
}

void CutterDialog_class::Step_forward()
{
	Sds->Set( sds->WD_state.direction, BACK_RIGHT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );

}
void CutterDialog_class::Step_backward()
{
	Sds->Set( sds->WD_state.direction, BACK_LEFT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutterDialog_class::Step_front_forward()
{
	Sds->Set( sds->WD_state.direction, FRONT_RIGHT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );

}
void CutterDialog_class::Step_front_backward()
{
	Sds->Set( sds->WD_state.direction, FRONT_LEFT );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutterDialog_class::Step_to_end()
{
	Sds->Set( sds->WD_state.direction, GOTO_END );
	Eventlog->add( SDS_ID, CUT_UPDATE_KEY );
}
void CutterDialog_class::updateCutDesk()
{
	ui->lcdNumber->display( sds->WD_state.cursor.min );
	ui->lcdNumber_2->display( sds->WD_state.cursor.max );
	coutf << "CutterDialog_class::updateCutDesk" << endl;
}

void CutterDialog_class::Cut_tail()
{
	Eventlog->add( SDS_ID, CUT_KEY );
}
void CutterDialog_class::Save()
{
	Eventlog->add( SDS_ID, CUT_SAVE );
}

void CutterDialog_class::Setup( Interface_class* Sds )
{
	setSds( Sds );
	Eventlog->add(SDS_ID, CUT_SETUP_KEY );
	updateCutDesk();
}

void CutterDialog_class::setSds( Interface_class* Sds )
{
	this->Sds 		= Sds;
	this->sds		= Sds->addr;
	this->SDS_ID	= sds->SDS_Id;
}


