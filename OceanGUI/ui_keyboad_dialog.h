/********************************************************************************
** Form generated from reading UI file 'keyboad_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYBOAD_DIALOG_H
#define UI_KEYBOAD_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Keyboad_Dialog_class
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QCheckBox *cb_decay_mode;
    QLabel *label_2;
    QLabel *label_6;
    QSpinBox *sb_flats;
    QLabel *label_3;
    QLabel *label_12;
    QLabel *label_4;
    QLabel *label_5;
    QCheckBox *cb_sliding_mode;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *pB_Save;
    QLabel *label_13;
    QLabel *label_7;
    QSpinBox *sb_base_octave;
    QComboBox *cB_buffer_mode;
    QLabel *label_8;
    QLabel *label;
    QSpinBox *sb_sharps;
    QLabel *label_9;
    QLabel *lbl_key;
    QLabel *lbl_frq;
    QLabel *lbl_note;

    void setupUi(QDialog *Keyboad_Dialog_class)
    {
        if (Keyboad_Dialog_class->objectName().isEmpty())
            Keyboad_Dialog_class->setObjectName("Keyboad_Dialog_class");
        Keyboad_Dialog_class->resize(237, 239);
        gridLayoutWidget = new QWidget(Keyboad_Dialog_class);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 0, 221, 231));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        cb_decay_mode = new QCheckBox(gridLayoutWidget);
        cb_decay_mode->setObjectName("cb_decay_mode");

        gridLayout->addWidget(cb_decay_mode, 2, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 7, 0, 1, 1);

        sb_flats = new QSpinBox(gridLayoutWidget);
        sb_flats->setObjectName("sb_flats");
        sb_flats->setWrapping(true);
        sb_flats->setMaximum(3);

        gridLayout->addWidget(sb_flats, 4, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_12 = new QLabel(gridLayoutWidget);
        label_12->setObjectName("label_12");

        gridLayout->addWidget(label_12, 2, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 5, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 6, 0, 1, 1);

        cb_sliding_mode = new QCheckBox(gridLayoutWidget);
        cb_sliding_mode->setObjectName("cb_sliding_mode");

        gridLayout->addWidget(cb_sliding_mode, 5, 1, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName("label_10");

        gridLayout->addWidget(label_10, 4, 2, 1, 1);

        label_11 = new QLabel(gridLayoutWidget);
        label_11->setObjectName("label_11");

        gridLayout->addWidget(label_11, 6, 2, 1, 1);

        pB_Save = new QPushButton(gridLayoutWidget);
        pB_Save->setObjectName("pB_Save");

        gridLayout->addWidget(pB_Save, 7, 1, 1, 1);

        label_13 = new QLabel(gridLayoutWidget);
        label_13->setObjectName("label_13");

        gridLayout->addWidget(label_13, 5, 2, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        sb_base_octave = new QSpinBox(gridLayoutWidget);
        sb_base_octave->setObjectName("sb_base_octave");
        sb_base_octave->setWrapping(true);
        sb_base_octave->setMinimum(1);
        sb_base_octave->setMaximum(3);
        sb_base_octave->setValue(2);

        gridLayout->addWidget(sb_base_octave, 1, 1, 1, 1);

        cB_buffer_mode = new QComboBox(gridLayoutWidget);
        cB_buffer_mode->setObjectName("cB_buffer_mode");

        gridLayout->addWidget(cB_buffer_mode, 6, 1, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 1, 2, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 2, 0, 1, 1);

        sb_sharps = new QSpinBox(gridLayoutWidget);
        sb_sharps->setObjectName("sb_sharps");
        sb_sharps->setWrapping(true);
        sb_sharps->setMaximum(3);

        gridLayout->addWidget(sb_sharps, 3, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget);
        label_9->setObjectName("label_9");

        gridLayout->addWidget(label_9, 3, 2, 1, 1);

        lbl_key = new QLabel(gridLayoutWidget);
        lbl_key->setObjectName("lbl_key");

        gridLayout->addWidget(lbl_key, 0, 0, 1, 1);

        lbl_frq = new QLabel(gridLayoutWidget);
        lbl_frq->setObjectName("lbl_frq");

        gridLayout->addWidget(lbl_frq, 0, 2, 1, 1);

        lbl_note = new QLabel(gridLayoutWidget);
        lbl_note->setObjectName("lbl_note");

        gridLayout->addWidget(lbl_note, 0, 1, 1, 1);


        retranslateUi(Keyboad_Dialog_class);

        QMetaObject::connectSlotsByName(Keyboad_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Keyboad_Dialog_class)
    {
        Keyboad_Dialog_class->setWindowTitle(QCoreApplication::translate("Keyboad_Dialog_class", "Keyboard", nullptr));
        cb_decay_mode->setText(QString());
        label_2->setText(QCoreApplication::translate("Keyboad_Dialog_class", "# of sharps", nullptr));
        label_6->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Save Notes", nullptr));
        label_3->setText(QCoreApplication::translate("Keyboad_Dialog_class", "# of flats", nullptr));
        label_12->setText(QCoreApplication::translate("Keyboad_Dialog_class", "F4", nullptr));
        label_4->setText(QCoreApplication::translate("Keyboad_Dialog_class", "slide mode", nullptr));
        label_5->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Buffer mode", nullptr));
        cb_sliding_mode->setText(QString());
        label_10->setText(QCoreApplication::translate("Keyboad_Dialog_class", "F7/f8", nullptr));
        label_11->setText(QCoreApplication::translate("Keyboad_Dialog_class", "F6", nullptr));
        pB_Save->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Save", nullptr));
        label_13->setText(QCoreApplication::translate("Keyboad_Dialog_class", "F5", nullptr));
        label_7->setText(QCoreApplication::translate("Keyboad_Dialog_class", "base octave", nullptr));
        label_8->setText(QCoreApplication::translate("Keyboad_Dialog_class", "+/-", nullptr));
        label->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Decay mode", nullptr));
        label_9->setText(QCoreApplication::translate("Keyboad_Dialog_class", "F2/F3", nullptr));
        lbl_key->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Key", nullptr));
        lbl_frq->setText(QCoreApplication::translate("Keyboad_Dialog_class", "[Hz]", nullptr));
        lbl_note->setText(QCoreApplication::translate("Keyboad_Dialog_class", "Note", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Keyboad_Dialog_class: public Ui_Keyboad_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYBOAD_DIALOG_H
