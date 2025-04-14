/********************************************************************************
** Form generated from reading UI file 'sds_dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDS_DIALOG_CLASS_H
#define UI_SDS_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_SDS_Dialog_class
{
public:
    QLineEdit *le_sdsid;
    QLabel *label;

    void setupUi(QDialog *SDS_Dialog_class)
    {
        if (SDS_Dialog_class->objectName().isEmpty())
            SDS_Dialog_class->setObjectName("SDS_Dialog_class");
        SDS_Dialog_class->resize(400, 300);
        le_sdsid = new QLineEdit(SDS_Dialog_class);
        le_sdsid->setObjectName("le_sdsid");
        le_sdsid->setGeometry(QRect(80, 0, 113, 26));
        label = new QLabel(SDS_Dialog_class);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 66, 18));

        retranslateUi(SDS_Dialog_class);

        QMetaObject::connectSlotsByName(SDS_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *SDS_Dialog_class)
    {
        SDS_Dialog_class->setWindowTitle(QCoreApplication::translate("SDS_Dialog_class", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SDS_Dialog_class", "SDS Id", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SDS_Dialog_class: public Ui_SDS_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDS_DIALOG_CLASS_H
