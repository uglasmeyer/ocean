
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
    this->Sds		= DaTA->GetSds( SDS_ID );
    this->Sds_master= DaTA->GetSdsAddr( 0 );

    connect( ui->rb_activate_S0	, SIGNAL(clicked()), this, SLOT(activate_S0()) );
    connect( ui->rb_activate_S1	, SIGNAL(clicked()), this, SLOT(activate_S1()) );
    connect( ui->rb_activate_S2	, SIGNAL(clicked()), this, SLOT(activate_S2()) );
    connect( ui->rb_activate_S3	, SIGNAL(clicked()), this, SLOT(activate_S3()) );

    red_color.setColor(QPalette::Button, Qt::red);
    green_color.setColor(QPalette::Button, Qt::green);

    proc_table_update_all();
    Update_widgets();
}

auto proc_table = [](Rtsp_Dialog_class* C, uint row, uint col, string text)
{
	QVariant 			Text( QString( text.data() ));
	QTableWidgetItem* 	twItem = new QTableWidgetItem();
	twItem->setData(Qt::DisplayRole, Text );
	C->ui->process_table->setItem( row,col, twItem );
	;
};

void Rtsp_Dialog_class::proc_table_update_row( uint row )
{
	if ( row > 0 )
	{
		Interface_class* SDS = DaTA->GetSds( row -1 );

		string 	text = SDS->Read_str( NOTESSTR_KEY );
		proc_table( this, row, 2, text );

				text = SDS->Read_str( INSTRUMENTSTR_KEY );
		proc_table( this, row, 1, text );
	}


	process_t proc { Sds_master->process_arr.at( row ) };
	string text = Type_map( proc.type );
	proc_table( this, row, 0, text );

}
void Rtsp_Dialog_class::proc_table_update_all( )
{
	for( uint row = 0 ; row < REGISTER_SIZE; row++ )
	{
		proc_table_update_row(row);
	}
}

void Rtsp_Dialog_class::Update_widgets()
{
	Sds_master->config = SDS_ID;
	Sds_master->UpdateFlag = true;	//comstack update
	proc_table_update_row( SDS_ID + 1 );

	DaTA->Sds_p->SHM.ShowDs( DaTA->SDS_vec[0].ds );
}

void Rtsp_Dialog_class::activate_S( uint sdsid)
{
	proc_table_update_row( sdsid + 1 );
	SDS_ID = sdsid;
	DaTA->Sds_master->config = sdsid;
	Update_widgets();
//	DaTA->Sds_master->UserInterface = UPDATEGUI;
};

void Rtsp_Dialog_class::activate_S0()
{
	activate_S( 0);
}
void Rtsp_Dialog_class::activate_S1()
{
	activate_S( 1);
}
void Rtsp_Dialog_class::activate_S2()
{
	activate_S( 2);
}
void Rtsp_Dialog_class::activate_S3()
{
	activate_S( 3);
}

Rtsp_Dialog_class::~Rtsp_Dialog_class()
{
//    if ( ui ) delete ui;
}
