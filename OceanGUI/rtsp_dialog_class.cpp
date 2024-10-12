
#include "Rtsp_dialog_class.h"
#include "ui_Rtsp_dialog_class.h"


Ui::Rtsp_Dialog_class UI_Rtsp_Dialog_Obj{};

Rtsp_Dialog_class::Rtsp_Dialog_class( 	QWidget* parent,
										Dataworld_class* data
)
    : Logfacility_class(Ui::ClassName),
	  Config_class( Ui::ClassName ),
	  QDialog(parent)
{
	ui = &UI_Rtsp_Dialog_Obj;
    ui->setupUi(this);
    this->DaTA		= data;

    connect( ui->rb_activate_S0	, SIGNAL(clicked()), this, SLOT(activate_S0()) );
    connect( ui->rb_activate_S1	, SIGNAL(clicked()), this, SLOT(activate_S1()) );
    connect( ui->rb_activate_S2	, SIGNAL(clicked()), this, SLOT(activate_S2()) );
    connect( ui->rb_activate_S3	, SIGNAL(clicked()), this, SLOT(activate_S3()) );
    connect( ui->pB_Exit_Rtsp	, SIGNAL(clicked()), this, SLOT( Exit_Rtsp()) );
    connect( ui->pB_Start_Rtsp	, SIGNAL(clicked()), this, SLOT( Start_Rtsp()) );

    red_color.setColor(QPalette::Button, Qt::red);
    green_color.setColor(QPalette::Button, Qt::green);

    Update_widgets();
}

auto proc_table = [](Rtsp_Dialog_class* C, uint row, uint col, string text)
{
	QVariant Text( QString( text.data() ));
	QTableWidgetItem* twItem = new QTableWidgetItem();
	twItem->setData(Qt::DisplayRole, Text );
	C->ui->process_table->setItem( row,col, twItem );
	;
};
void Rtsp_Dialog_class::Update_widgets()
{
	DaTA->Sds_master->config = SDS_ID; //comstack update
	DaTA->Sds_master->UpdateFlag = true;
	for( uint p = 0 ;p < MAXCONFIG+1; p++ )
	{
		process_t proc { DaTA->Sds_master->process_arr[ p ] };
		string text = DaTA->Sds.type_map[ proc.type ];

		proc_table( this, p, 0, text );
	}
	cout << boolalpha << DaTA->Sds_master->Rtsp << endl;
	DaTA->Sds_p->SHM.ShowDs( DaTA->SDS_vec[0].ds );
	if ( DaTA->Sds_master->Rtsp == RUNNING )
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
	Update_widgets();
	DaTA->Sds_master->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S1()
{
	SDS_ID=1;
	Update_widgets();
	DaTA->Sds_master->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S2()
{
	SDS_ID=2;
	Update_widgets();
	DaTA->Sds_master->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::activate_S3()
{
	SDS_ID=3;
	Update_widgets();
	DaTA->Sds_master->UserInterface = UPDATEGUI;
}
void Rtsp_Dialog_class::Exit_Rtsp()
{
	DaTA->Sds_master->Rtsp = EXITSERVER;
	DaTA->Sem.Release ( SYNTHESIZER_START );

	Update_widgets();
}
void Rtsp_Dialog_class::Start_Rtsp()
{
	string cmd = Server_cmd( DaTA->Cfg.Config.Term, file_structure().rtsp_bin, "" );
	system_execute( cmd );
	DaTA->Sem.Lock( RTSP_STARTED, 1 );

	Update_widgets();
}

Rtsp_Dialog_class::~Rtsp_Dialog_class()
{
//    if ( ui ) delete ui;
}
