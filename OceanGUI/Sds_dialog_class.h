#ifndef SDS_DIALOG_CLASS_H
#define SDS_DIALOG_CLASS_H

#include <QDialog>
#include <data/Interface.h>

namespace Ui {
class SDS_Dialog_class;
}

class SDS_Dialog_class : public QDialog
{
    Q_OBJECT

public:
    Interface_class* Sds			= nullptr;

    explicit SDS_Dialog_class( QWidget *parent = nullptr,
    		Interface_class* Sds = nullptr
);
    virtual ~SDS_Dialog_class();

private:
    unique_ptr<Ui::SDS_Dialog_class>  ui;
};

#endif // SDS_DIALOG_CLASS_H
