/********************************************************************************
** Form generated from reading UI file 'CutDesk_Dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUTDESK_DIALOG_H
#define UI_CUTDESK_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CutDesk_Dialog_class
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pb_back_right;
    QLCDNumber *lcdNumber_2;
    QLCDNumber *lcdNumber;
    QPushButton *pb_front_right;
    QPushButton *pB_rotate_left;
    QPushButton *pB_Cut;
    QPushButton *pB_back_left;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pB_goto_end;
    QLabel *lbl_wav_file;
    QPushButton *pb_front_left;
    QPushButton *pB_rotate_right;
    QPushButton *pB_Save;
    QComboBox *cB_Stepwidth;

    void setupUi(QDialog *CutDesk_Dialog_class)
    {
        if (CutDesk_Dialog_class->objectName().isEmpty())
            CutDesk_Dialog_class->setObjectName("CutDesk_Dialog_class");
        CutDesk_Dialog_class->resize(628, 78);
        gridLayoutWidget = new QWidget(CutDesk_Dialog_class);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 616, 60));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pb_back_right = new QPushButton(gridLayoutWidget);
        pb_back_right->setObjectName("pb_back_right");

        gridLayout->addWidget(pb_back_right, 0, 6, 1, 1);

        lcdNumber_2 = new QLCDNumber(gridLayoutWidget);
        lcdNumber_2->setObjectName("lcdNumber_2");

        gridLayout->addWidget(lcdNumber_2, 1, 5, 1, 1);

        lcdNumber = new QLCDNumber(gridLayoutWidget);
        lcdNumber->setObjectName("lcdNumber");

        gridLayout->addWidget(lcdNumber, 1, 3, 1, 1);

        pb_front_right = new QPushButton(gridLayoutWidget);
        pb_front_right->setObjectName("pb_front_right");

        gridLayout->addWidget(pb_front_right, 0, 3, 1, 1);

        pB_rotate_left = new QPushButton(gridLayoutWidget);
        pB_rotate_left->setObjectName("pB_rotate_left");

        gridLayout->addWidget(pB_rotate_left, 0, 8, 1, 1);

        pB_Cut = new QPushButton(gridLayoutWidget);
        pB_Cut->setObjectName("pB_Cut");

        gridLayout->addWidget(pB_Cut, 1, 1, 1, 1);

        pB_back_left = new QPushButton(gridLayoutWidget);
        pB_back_left->setObjectName("pB_back_left");

        gridLayout->addWidget(pB_back_left, 0, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 7, 1, 1);

        pB_goto_end = new QPushButton(gridLayoutWidget);
        pB_goto_end->setObjectName("pB_goto_end");

        gridLayout->addWidget(pB_goto_end, 1, 6, 1, 1);

        lbl_wav_file = new QLabel(gridLayoutWidget);
        lbl_wav_file->setObjectName("lbl_wav_file");

        gridLayout->addWidget(lbl_wav_file, 1, 9, 1, 1);

        pb_front_left = new QPushButton(gridLayoutWidget);
        pb_front_left->setObjectName("pb_front_left");

        gridLayout->addWidget(pb_front_left, 0, 1, 1, 1);

        pB_rotate_right = new QPushButton(gridLayoutWidget);
        pB_rotate_right->setObjectName("pB_rotate_right");

        gridLayout->addWidget(pB_rotate_right, 0, 9, 1, 1);

        pB_Save = new QPushButton(gridLayoutWidget);
        pB_Save->setObjectName("pB_Save");

        gridLayout->addWidget(pB_Save, 1, 8, 1, 1);

        cB_Stepwidth = new QComboBox(gridLayoutWidget);
        cB_Stepwidth->setObjectName("cB_Stepwidth");

        gridLayout->addWidget(cB_Stepwidth, 0, 4, 1, 1);


        retranslateUi(CutDesk_Dialog_class);

        QMetaObject::connectSlotsByName(CutDesk_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *CutDesk_Dialog_class)
    {
        CutDesk_Dialog_class->setWindowTitle(QCoreApplication::translate("CutDesk_Dialog_class", "Cut Desk", nullptr));
        pb_back_right->setText(QCoreApplication::translate("CutDesk_Dialog_class", "Step >", nullptr));
        pb_front_right->setText(QCoreApplication::translate("CutDesk_Dialog_class", "> Step", nullptr));
        pB_rotate_left->setText(QCoreApplication::translate("CutDesk_Dialog_class", "< Rotate", nullptr));
        pB_Cut->setText(QCoreApplication::translate("CutDesk_Dialog_class", "Cut", nullptr));
        pB_back_left->setText(QCoreApplication::translate("CutDesk_Dialog_class", "Step <", nullptr));
        pB_goto_end->setText(QCoreApplication::translate("CutDesk_Dialog_class", "End >>", nullptr));
        lbl_wav_file->setText(QCoreApplication::translate("CutDesk_Dialog_class", "None", nullptr));
        pb_front_left->setText(QCoreApplication::translate("CutDesk_Dialog_class", "< Step", nullptr));
        pB_rotate_right->setText(QCoreApplication::translate("CutDesk_Dialog_class", "Rotate >", nullptr));
        pB_Save->setText(QCoreApplication::translate("CutDesk_Dialog_class", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CutDesk_Dialog_class: public Ui_CutDesk_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUTDESK_DIALOG_H
