
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

    proc_table_update_all();
}

void Rtsp_Dialog_class::proc_table( uint row, uint col, string text)
{
	QVariant 			Text( QString( text.data() ));
	QTableWidgetItem* 	twItem_p = new QTableWidgetItem();
	twItem_p->setData(Qt::DisplayRole, Text );
	ui->process_table->setItem( row,col, twItem_p );

};

void Rtsp_Dialog_class::proc_table_update_row( uint row )
{
	process_t proc { Sds_master->process_arr.at( row ) };
	string text = Type_map( proc.type );
	proc_table( row, 0, text );

	if ( row > 0 )
	{
		Interface_class* SDS = DaTA->GetSds( row -1 );

		text = SDS->Read_str( INSTRUMENTSTR_KEY );
		proc_table( row, 1, text );

		text = SDS->Read_str( NOTESSTR_KEY );
		proc_table( row, 2, text );
	}
}

void Rtsp_Dialog_class::proc_table_update_all( )
{
	for( uint row = 0 ; row < REGISTER_SIZE; row++ )
	{
		proc_table_update_row(row);
	}
}




Rtsp_Dialog_class::~Rtsp_Dialog_class()
{

}
