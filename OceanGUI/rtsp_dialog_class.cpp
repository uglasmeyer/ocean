//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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




