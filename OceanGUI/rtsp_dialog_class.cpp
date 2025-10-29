
#include <include/Rtsp_dialog_class.h>



Rtsp_Dialog_class::Rtsp_Dialog_class( 	QWidget* parent,
										Dataworld_class* data
)
	: QDialog(parent),
	  ui( new Ui::Rtsp_Dialog_class {})
{
    ui->setupUi(this);
    this->DaTA		= data;

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
	uint8_t				sdsid	= ( row == 0 ) ? 0 : row-1;
	Interface_class* 	SDS 	= DaTA->SDS.GetSds( sdsid );
	APPID 				appid 	= ( row == 0 ) ? AUDIOID : SYNTHID;
	bool 				state	= DaTA->Appstate.IsRunning( SDS->addr, appid);
	string 				appname = state ? AppIdName( appid ) : "no process";
	proc_table( row, 0, appname );
	if ( row > 0 ) // no additional comment for the audio server row 0
	{
		string instrname = SDS->Read_str( INSTRUMENTSTR_KEY );
		proc_table( row, 1, instrname );

		string notesname = SDS->Read_str( NOTESSTR_KEY );
		proc_table( row, 2, notesname );
	}
}

void Rtsp_Dialog_class::proc_table_update_all( )
{
	for( uint row = 0 ; row < ( MAXCONFIG+1 ); row++ )
	{
		proc_table_update_row(row);
	}
}




