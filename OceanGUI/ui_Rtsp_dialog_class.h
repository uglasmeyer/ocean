/********************************************************************************
** Form generated from reading UI file 'Rtsp_dialog_class.ui'
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
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_Rtsp_Dialog_class
{
public:
    QPushButton *pB_Exit_Rtsp;
    QFrame *frame;
    QRadioButton *rb_activate_S0;
    QRadioButton *rb_activate_S2;
    QRadioButton *rb_activate_S1;
    QRadioButton *rb_activate_S3;
    QPushButton *pB_Start_Rtsp;

    void setupUi(QDialog *Rtsp_Dialog_class)
    {
        if (Rtsp_Dialog_class->objectName().isEmpty())
            Rtsp_Dialog_class->setObjectName(QString::fromUtf8("Rtsp_Dialog_class"));
        Rtsp_Dialog_class->resize(372, 281);
        Rtsp_Dialog_class->setStyleSheet(QString::fromUtf8("background-color: rgb(153, 193, 241);"));
        pB_Exit_Rtsp = new QPushButton(Rtsp_Dialog_class);
        pB_Exit_Rtsp->setObjectName(QString::fromUtf8("pB_Exit_Rtsp"));
        pB_Exit_Rtsp->setGeometry(QRect(240, 230, 88, 25));
        frame = new QFrame(Rtsp_Dialog_class);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(90, 130, 120, 131));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        rb_activate_S0 = new QRadioButton(frame);
        rb_activate_S0->setObjectName(QString::fromUtf8("rb_activate_S0"));
        rb_activate_S0->setGeometry(QRect(10, 10, 111, 23));
        rb_activate_S2 = new QRadioButton(frame);
        rb_activate_S2->setObjectName(QString::fromUtf8("rb_activate_S2"));
        rb_activate_S2->setGeometry(QRect(10, 70, 111, 23));
        rb_activate_S1 = new QRadioButton(frame);
        rb_activate_S1->setObjectName(QString::fromUtf8("rb_activate_S1"));
        rb_activate_S1->setGeometry(QRect(10, 40, 111, 23));
        rb_activate_S3 = new QRadioButton(frame);
        rb_activate_S3->setObjectName(QString::fromUtf8("rb_activate_S3"));
        rb_activate_S3->setGeometry(QRect(10, 100, 111, 23));
        pB_Start_Rtsp = new QPushButton(Rtsp_Dialog_class);
        pB_Start_Rtsp->setObjectName(QString::fromUtf8("pB_Start_Rtsp"));
        pB_Start_Rtsp->setGeometry(QRect(240, 190, 88, 25));

        retranslateUi(Rtsp_Dialog_class);

        QMetaObject::connectSlotsByName(Rtsp_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Rtsp_Dialog_class)
    {
        Rtsp_Dialog_class->setWindowTitle(QCoreApplication::translate("Rtsp_Dialog_class", "Dialog", nullptr));
        pB_Exit_Rtsp->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Exit Rtsp", nullptr));
        rb_activate_S0->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S0", nullptr));
        rb_activate_S2->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S2", nullptr));
        rb_activate_S1->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S1", nullptr));
        rb_activate_S3->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S3", nullptr));
        pB_Start_Rtsp->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Start Rtsp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rtsp_Dialog_class: public Ui_Rtsp_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSP_DIALOG_CLASS_H
