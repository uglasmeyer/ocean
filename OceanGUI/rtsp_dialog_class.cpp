
#include "Rtsp_dialog_class.h"
#include "ui_Rtsp_dialog_class.h"


Ui::Rtsp_Dialog_class UI_Rtsp_Dialog_Obj{};

Rtsp_Dialog_class::Rtsp_Dialog_class( 	QWidget* parent,
										Dataworld_class* data,
										Semaphore_class* sem )
    : Logfacility_class(Ui::ClassName),
	  Config_class( Ui::ClassName ),
	  QDialog(parent)
{
	ui = &UI_Rtsp_Dialog_Obj;
    ui->setupUi(this);
    this->sem 		= sem;
    this->sds		= data->Sds.addr;
    this->daTA		= data;

    connect( ui->rb_activate_S0	, SIGNAL(clicked()), this, SLOT(activate_S0()) );
    connect( ui->rb_activate_S1	, SIGNAL(clicked()), this, SLOT(activate_S1()) );
    connect( ui->rb_activate_S2	, SIGNAL(clicked()), this, SLOT(activate_S2()) );
    connect( ui->rb_activate_S3	, SIGNAL(clicked()), this, SLOT(activate_S3()) );
    connect( ui->pB_Exit_Rtsp	, SIGNAL(clicked()), this, SLOT( Exit_Rtsp()) );
    connect( ui->pB_Start_Rtsp	, SIGNAL(clicked()), this, SLOT( Start_Rtsp()) );

    red_color.setColor(QPalette::Button, Qt::red);
    green_color.setColor(QPalette::Button, Qt::green);

    update_widgets();
}

void Rtsp_Dialog_class::update_widgets()
{
	if ( sds->Rtsp == RUNNING )
	{
		ui->pB_Exit_Rtsp->setEnabled( true );
		ui->pB_Start_Rtsp->setEnabled( false );
	}
	else
	{
		ui->pB_Exit_Rtsp->setEnabled( false );
		ui->pB_Start_Rtsp->setEnabled( true );
	}
}

void Rtsp_Dialog_class::activate_S0()
{
	SDS_ID=0;
	update_widgets();
	sds->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S1()
{
	SDS_ID=1;
	update_widgets();
	sds->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S2()
{
	SDS_ID=2;
	update_widgets();
	sds->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S3()
{
	SDS_ID=3;
	update_widgets();
	sds->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::Exit_Rtsp()
{
	sds->Rtsp = EXITSERVER;
	sem->Release ( SYNTHESIZER_START );

	update_widgets();
}
void Rtsp_Dialog_class::Start_Rtsp()
{
	string cmd = Server_cmd( "", file_structure().rtsp_bin, "" );
	system_execute( cmd );
	sem->Lock( RTSP_STARTED, 1 );

	update_widgets();
}

Rtsp_Dialog_class::~Rtsp_Dialog_class()
{
//    if ( ui ) delete ui;
}
