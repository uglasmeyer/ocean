#include "Sds_dialog_class.h"
#include "ui_sds_dialog_class.h"

Ui::SDS_Dialog_class UI_SDS_dialog_obj {};

SDS_Dialog_class::SDS_Dialog_class(QWidget *parent,
		Interface_class* Sds ) :
     QDialog(parent)
{
	interface_t* sds_p = Sds->addr;
	ui 			= &UI_SDS_dialog_obj;

    ui->setupUi(this);

    ui->le_sdsid->setText( QString::number( sds_p->SDS_Id ) );
}

SDS_Dialog_class::~SDS_Dialog_class()
{
   // delete ui;
}

