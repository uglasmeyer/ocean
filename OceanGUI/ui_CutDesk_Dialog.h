/********************************************************************************
** Form generated from reading UI file 'CutDesk_Dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUTDESK_DIALOG_H
#define UI_CUTDESK_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CutterDialog_class
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pb_back_right;
    QPushButton *pb_front_right;
    QPushButton *pB_back_left;
    QPushButton *pb_front_left;
    QPushButton *pB_Cut;
    QPushButton *pB_goto_end;
    QPushButton *pB_Save;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;

    void setupUi(QDialog *CutterDialog_class)
    {
        if (CutterDialog_class->objectName().isEmpty())
            CutterDialog_class->setObjectName("CutterDialog_class");
        CutterDialog_class->resize(538, 78);
        gridLayoutWidget = new QWidget(CutterDialog_class);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 526, 60));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pb_back_right = new QPushButton(gridLayoutWidget);
        pb_back_right->setObjectName("pb_back_right");

        gridLayout->addWidget(pb_back_right, 0, 4, 1, 1);

        pb_front_right = new QPushButton(gridLayoutWidget);
        pb_front_right->setObjectName("pb_front_right");

        gridLayout->addWidget(pb_front_right, 0, 2, 1, 1);

        pB_back_left = new QPushButton(gridLayoutWidget);
        pB_back_left->setObjectName("pB_back_left");

        gridLayout->addWidget(pB_back_left, 0, 3, 1, 1);

        pb_front_left = new QPushButton(gridLayoutWidget);
        pb_front_left->setObjectName("pb_front_left");

        gridLayout->addWidget(pb_front_left, 0, 0, 1, 1);

        pB_Cut = new QPushButton(gridLayoutWidget);
        pB_Cut->setObjectName("pB_Cut");

        gridLayout->addWidget(pB_Cut, 1, 0, 1, 1);

        pB_goto_end = new QPushButton(gridLayoutWidget);
        pB_goto_end->setObjectName("pB_goto_end");

        gridLayout->addWidget(pB_goto_end, 0, 5, 1, 1);

        pB_Save = new QPushButton(gridLayoutWidget);
        pB_Save->setObjectName("pB_Save");

        gridLayout->addWidget(pB_Save, 1, 5, 1, 1);

        lcdNumber = new QLCDNumber(gridLayoutWidget);
        lcdNumber->setObjectName("lcdNumber");

        gridLayout->addWidget(lcdNumber, 1, 2, 1, 1);

        lcdNumber_2 = new QLCDNumber(gridLayoutWidget);
        lcdNumber_2->setObjectName("lcdNumber_2");

        gridLayout->addWidget(lcdNumber_2, 1, 3, 1, 1);


        retranslateUi(CutterDialog_class);

        QMetaObject::connectSlotsByName(CutterDialog_class);
    } // setupUi

    void retranslateUi(QDialog *CutterDialog_class)
    {
        CutterDialog_class->setWindowTitle(QCoreApplication::translate("CutterDialog_class", "Cut Desk", nullptr));
        pb_back_right->setText(QCoreApplication::translate("CutterDialog_class", "Step >", nullptr));
        pb_front_right->setText(QCoreApplication::translate("CutterDialog_class", "> Step", nullptr));
        pB_back_left->setText(QCoreApplication::translate("CutterDialog_class", "Step <", nullptr));
        pb_front_left->setText(QCoreApplication::translate("CutterDialog_class", "< Step", nullptr));
        pB_Cut->setText(QCoreApplication::translate("CutterDialog_class", "Cut", nullptr));
        pB_goto_end->setText(QCoreApplication::translate("CutterDialog_class", "End >>", nullptr));
        pB_Save->setText(QCoreApplication::translate("CutterDialog_class", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CutterDialog_class: public Ui_CutterDialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUTDESK_DIALOG_H
