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

/*
 * keyboard_dialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <include/Rtsp_dialog.h>



Rtsp_Dialog_class::Rtsp_Dialog_class( 	QWidget* parent,
										Dataworld_class* data
)
	: Logfacility_class( "Rtsp_Dialog_class" ),
	  QDialog(parent),
	  ui( new Ui::Rtsp_Dialog_class {})
{
	className		= Logfacility_class::className;
    this->DaTA		= data;

    ui->setupUi(this);
    ui->process_table->setRowCount( max_table_rows );

    UpdateLog( "Init RTSP log" );
    Proc_table_update_all();
}

Rtsp_Dialog_class::~Rtsp_Dialog_class()
{
//	delete ( twItem_p );
//	delete(ui);
}

void Rtsp_Dialog_class::UpdateLog( const QString& logstr )
{
	ui->tB_log->append( logstr );
}
void Rtsp_Dialog_class::proc_table( uint row, uint col, string text)
{
	QVariant 			Text( QString( text.data() ));
	QTableWidgetItem*	twItem_p 			= new QTableWidgetItem() ;
	twItem_p->setData(Qt::DisplayRole, Text );
	ui->process_table->setItem( row, col, twItem_p );
};

void Rtsp_Dialog_class::Proc_table_update(  interface_t* sds, APPID appid  )
{
	//  consider table layout
	//	row0 : audioserver running on sdsid 0
	//	1234 : synthesizer running on sdsid 0123
	//	processs name | Instrument | Notes

	uint 				table_row	= sds->SDS_Id + appid;

	bool 				state		= DaTA->Appstate.IsRunning( sds, appid);
	string 				appname 	= state ? AppIdName( appid ) : "no process";

	proc_table( table_row, 0, appname );

	if ( appid == SYNTHID )
	{
		string instrname{ sds->Instrument }; //Sds->Read_str( INSTRUMENTSTR_KEY );
		proc_table( table_row, 1, instrname );

		string notesname{ sds->Notes };// = Sds->Read_str( NOTESSTR_KEY );
		proc_table( table_row, 2, notesname );
	}
}

void Rtsp_Dialog_class::Proc_table_update_all( )
{
	interface_t* sds = this->DaTA->SDS.GetSdsAddr( 0 );
	Proc_table_update( sds, AUDIOID );

	for( interface_t* sds : DaTA->SDS.vec )
	{
		Proc_table_update( sds, SYNTHID );
	}
}




