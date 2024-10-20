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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Rtsp_Dialog_class
{
public:
    QFrame *frame;
    QRadioButton *rb_activate_S1;
    QRadioButton *rb_activate_S0;
    QRadioButton *rb_activate_S2;
    QRadioButton *rb_activate_S3;
    QTableWidget *process_table;

    void setupUi(QDialog *Rtsp_Dialog_class)
    {
        if (Rtsp_Dialog_class->objectName().isEmpty())
            Rtsp_Dialog_class->setObjectName(QString::fromUtf8("Rtsp_Dialog_class"));
        Rtsp_Dialog_class->resize(493, 227);
        Rtsp_Dialog_class->setStyleSheet(QString::fromUtf8("background-color: rgb(153, 193, 241);"));
        frame = new QFrame(Rtsp_Dialog_class);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(9, 9, 121, 191));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        rb_activate_S1 = new QRadioButton(frame);
        rb_activate_S1->setObjectName(QString::fromUtf8("rb_activate_S1"));
        rb_activate_S1->setGeometry(QRect(10, 90, 111, 23));
        rb_activate_S0 = new QRadioButton(frame);
        rb_activate_S0->setObjectName(QString::fromUtf8("rb_activate_S0"));
        rb_activate_S0->setGeometry(QRect(10, 60, 111, 23));
        rb_activate_S2 = new QRadioButton(frame);
        rb_activate_S2->setObjectName(QString::fromUtf8("rb_activate_S2"));
        rb_activate_S2->setGeometry(QRect(10, 120, 111, 23));
        rb_activate_S3 = new QRadioButton(frame);
        rb_activate_S3->setObjectName(QString::fromUtf8("rb_activate_S3"));
        rb_activate_S3->setGeometry(QRect(10, 150, 111, 23));
        process_table = new QTableWidget(Rtsp_Dialog_class);
        if (process_table->columnCount() < 3)
            process_table->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        process_table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        process_table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        process_table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (process_table->rowCount() < 5)
            process_table->setRowCount(5);
        process_table->setObjectName(QString::fromUtf8("process_table"));
        process_table->setGeometry(QRect(150, 9, 331, 192));
        process_table->setRowCount(5);
        process_table->setColumnCount(3);

        retranslateUi(Rtsp_Dialog_class);

        QMetaObject::connectSlotsByName(Rtsp_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Rtsp_Dialog_class)
    {
        Rtsp_Dialog_class->setWindowTitle(QCoreApplication::translate("Rtsp_Dialog_class", "Server Control", nullptr));
        rb_activate_S1->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S1", nullptr));
        rb_activate_S0->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S0", nullptr));
        rb_activate_S2->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S2", nullptr));
        rb_activate_S3->setText(QCoreApplication::translate("Rtsp_Dialog_class", "activate S3", nullptr));
        QTableWidgetItem *___qtablewidgetitem = process_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Process", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = process_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Instrument", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = process_table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Notes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rtsp_Dialog_class: public Ui_Rtsp_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSP_DIALOG_CLASS_H
