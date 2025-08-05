#ifndef ADSRDIALOG_CLASS_H
#define ADSRDIALOG_CLASS_H
#include <Ocean.h>

#include <QDialog>

namespace Ui {
class ADSRDialog_class;
}

class ADSRDialog_class : public QDialog
{
    Q_OBJECT

public:
    explicit ADSRDialog_class(QWidget *parent = nullptr);
    ~ADSRDialog_class();

	unique_ptr<Ui::ADSRDialog_class>		ui;

	void Setup_widgets();
private:
//    Ui::ADSRDialog_class* 	ui;
};

#endif // ADSRDIALOG_CLASS_H
