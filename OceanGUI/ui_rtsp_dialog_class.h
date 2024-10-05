/********************************************************************************
** Form generated from reading UI file 'rtsp_dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTSP_DIALOG_CLASS_H
#define UI_RTSP_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_Rtsp_Dialog_class
{
public:

    void setupUi(QDialog *Rtsp_Dialog_class)
    {
        if (Rtsp_Dialog_class->objectName().isEmpty())
            Rtsp_Dialog_class->setObjectName(QString::fromUtf8("Rtsp_Dialog_class"));
        Rtsp_Dialog_class->resize(400, 300);

        retranslateUi(Rtsp_Dialog_class);

        QMetaObject::connectSlotsByName(Rtsp_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Rtsp_Dialog_class)
    {
        Rtsp_Dialog_class->setWindowTitle(QCoreApplication::translate("Rtsp_Dialog_class", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rtsp_Dialog_class: public Ui_Rtsp_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSP_DIALOG_CLASS_H
