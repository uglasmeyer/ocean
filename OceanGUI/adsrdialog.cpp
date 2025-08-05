#include <Adsrdialog.h>
#include "ui_Adsrdialog.h"

ADSRDialog_class::ADSRDialog_class(QWidget *parent)
    : QDialog(parent)
    , ui( new Ui::ADSRDialog_class )
{
    ui->setupUi(this);
}

ADSRDialog_class::~ADSRDialog_class()
{
//    delete ui;
}
void ADSRDialog_class::Setup_widgets()
{
	;
}
