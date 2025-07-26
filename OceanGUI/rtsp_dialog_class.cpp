
#include <Rtsp_dialog_class.h>



Rtsp_Dialog_class::Rtsp_Dialog_class( 	QWidget* parent,
										Dataworld_class* data
)
	:  Config_class( Ui::ClassName ),
	  QDialog(parent),
	  ui( new Ui::Rtsp_Dialog_class {})
{
    ui->setupUi(this);
    this->DaTA		= data;
    this->Sds		= DaTA->SDS.GetSds( SDS_ID );
    this->sds_master= DaTA->sds_master;

    UpdateLog( "Init RTSP log" );
    proc_table_update_all();
}
Rtsp_Dialog_class::~Rtsp_Dialog_class()
{
	delete ( twItem_p );
//	delete(ui);
}

void Rtsp_Dialog_class::UpdateLog( const QString& logstr )
{
	ui->tB_log->append( logstr );
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
	register_process_t proc { sds_master->process_arr.at( row ) };
	string text = AppIdName( proc.type );
	proc_table( row, 0, text );

	if ( row > 0 )
	{
		Interface_class* SDS = DaTA->SDS.GetSds( row -1 );

		text = SDS->Read_str( INSTRUMENTSTR_KEY );
		proc_table( row, 1, text );

		text = SDS->Read_str( NOTESSTR_KEY );
		proc_table( row, 2, text );
	}
}

void Rtsp_Dialog_class::proc_table_update_all( )
{
	DaTA->Reg.Update_register();
	for( uint row = 0 ; row < REGISTER_SIZE; row++ )
	{
		proc_table_update_row(row);
	}
}




