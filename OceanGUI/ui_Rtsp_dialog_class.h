/********************************************************************************
** Form generated from reading UI file 'Rtsp_dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTSP_DIALOG_CLASS_H
#define UI_RTSP_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Rtsp_Dialog_class
{
public:
    QTableWidget *process_table;
    QTextBrowser *tB_log;

    void setupUi(QDialog *Rtsp_Dialog_class)
    {
        if (Rtsp_Dialog_class->objectName().isEmpty())
            Rtsp_Dialog_class->setObjectName("Rtsp_Dialog_class");
        Rtsp_Dialog_class->resize(619, 302);
        Rtsp_Dialog_class->setStyleSheet(QString::fromUtf8("background-color: rgb(153, 193, 241);"));
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
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        process_table->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        process_table->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        process_table->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        process_table->setVerticalHeaderItem(3, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        process_table->setVerticalHeaderItem(4, __qtablewidgetitem7);
        process_table->setObjectName("process_table");
        process_table->setGeometry(QRect(10, 9, 331, 192));
        process_table->setRowCount(5);
        process_table->setColumnCount(3);
        tB_log = new QTextBrowser(Rtsp_Dialog_class);
        tB_log->setObjectName("tB_log");
        tB_log->setGeometry(QRect(10, 190, 601, 101));
        tB_log->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tB_log->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        retranslateUi(Rtsp_Dialog_class);

        QMetaObject::connectSlotsByName(Rtsp_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Rtsp_Dialog_class)
    {
        Rtsp_Dialog_class->setWindowTitle(QCoreApplication::translate("Rtsp_Dialog_class", "Server Control", nullptr));
        QTableWidgetItem *___qtablewidgetitem = process_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Process", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = process_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Instrument", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = process_table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Rtsp_Dialog_class", "Notes", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = process_table->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Rtsp_Dialog_class", "A", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = process_table->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Rtsp_Dialog_class", "S0", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = process_table->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Rtsp_Dialog_class", "S1", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = process_table->verticalHeaderItem(3);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Rtsp_Dialog_class", "S2", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = process_table->verticalHeaderItem(4);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Rtsp_Dialog_class", "S3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rtsp_Dialog_class: public Ui_Rtsp_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSP_DIALOG_CLASS_H
