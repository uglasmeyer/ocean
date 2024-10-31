#include "notesdialog.h"
#include "ui_notesdialog.h"

NotesDialog::NotesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NotesDialog)
{
    ui->setupUi(this);
}

NotesDialog::~NotesDialog()
{
    delete ui;
}
