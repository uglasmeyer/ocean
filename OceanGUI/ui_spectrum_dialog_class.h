/********************************************************************************
** Form generated from reading UI file 'spectrum_dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECTRUM_DIALOG_CLASS_H
#define UI_SPECTRUM_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Spectrum_Dialog_class
{
public:
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QSlider *vS_4;
    QSpacerItem *horizontalSpacer;
    QSlider *fS_1;
    QSlider *fS_5;
    QSlider *fS_7;
    QSlider *vS_2;
    QSlider *fS_3;
    QSlider *vS_8;
    QSlider *vS_6;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pB_save_spectrum;
    QLabel *lbl_instrument;
    QLabel *label;
    QFrame *frame_rb_osc;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer;
    QRadioButton *rb_spec_fmo;
    QRadioButton *rb_spec_main;
    QRadioButton *rb_spec_vco;
    QSpacerItem *verticalSpacer_2;
    QLCDNumber *lcd_spectrumDisplay;
    QLabel *lbl_spectrumDisplay;
    QRadioButton *rb_reset;
    QLabel *lbl_waveform;
    QSpinBox *sb_spwf1;
    QSpinBox *sb_spwf2;
    QSpinBox *sb_spwf3;
    QSpinBox *sb_spwf4;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QSlider *hs_osc_attack;
    QSlider *hs_osc_decay;
    QSpacerItem *verticalSpacer_3;
    QSlider *hs_vco_attack;
    QSlider *hs_vco_decay;
    QSpacerItem *verticalSpacer_4;
    QSlider *hs_fmo_attack;
    QSlider *hs_fmo_decay;
    QLabel *label_2;

    void setupUi(QDialog *Spectrum_Dialog_class)
    {
        if (Spectrum_Dialog_class->objectName().isEmpty())
            Spectrum_Dialog_class->setObjectName("Spectrum_Dialog_class");
        Spectrum_Dialog_class->resize(496, 272);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(153, 193, 241, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(204, 224, 248, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(76, 96, 120, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(102, 129, 161, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        QBrush brush7(QColor(0, 0, 0, 127));
        brush7.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush7);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush8(QColor(239, 239, 239, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        QBrush brush9(QColor(202, 202, 202, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        QBrush brush10(QColor(159, 159, 159, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        QBrush brush11(QColor(184, 184, 184, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        QBrush brush12(QColor(118, 118, 118, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush12);
        QBrush brush13(QColor(247, 247, 247, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        QBrush brush14(QColor(0, 0, 0, 128));
        brush14.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush14);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        QBrush brush15(QColor(177, 177, 177, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush14);
#endif
        Spectrum_Dialog_class->setPalette(palette);
        frame = new QFrame(Spectrum_Dialog_class);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(21, 11, 231, 131));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName("gridLayout_2");
        vS_4 = new QSlider(frame);
        vS_4->setObjectName("vS_4");
        vS_4->setMaximum(100);
        vS_4->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(vS_4, 0, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        fS_1 = new QSlider(frame);
        fS_1->setObjectName("fS_1");
        fS_1->setMinimum(-50);
        fS_1->setMaximum(50);
        fS_1->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(fS_1, 0, 0, 1, 1);

        fS_5 = new QSlider(frame);
        fS_5->setObjectName("fS_5");
        fS_5->setMinimum(-50);
        fS_5->setMaximum(50);
        fS_5->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(fS_5, 0, 6, 1, 1);

        fS_7 = new QSlider(frame);
        fS_7->setObjectName("fS_7");
        fS_7->setMinimum(-50);
        fS_7->setMaximum(50);
        fS_7->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(fS_7, 0, 9, 1, 1);

        vS_2 = new QSlider(frame);
        vS_2->setObjectName("vS_2");
        vS_2->setMaximum(100);
        vS_2->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(vS_2, 0, 1, 1, 1);

        fS_3 = new QSlider(frame);
        fS_3->setObjectName("fS_3");
        fS_3->setMinimum(-50);
        fS_3->setMaximum(50);
        fS_3->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(fS_3, 0, 3, 1, 1);

        vS_8 = new QSlider(frame);
        vS_8->setObjectName("vS_8");
        vS_8->setMaximum(100);
        vS_8->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(vS_8, 0, 10, 1, 1);

        vS_6 = new QSlider(frame);
        vS_6->setObjectName("vS_6");
        vS_6->setMaximum(100);
        vS_6->setOrientation(Qt::Orientation::Vertical);

        gridLayout_2->addWidget(vS_6, 0, 7, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 5, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 8, 1, 1);

        pB_save_spectrum = new QPushButton(Spectrum_Dialog_class);
        pB_save_spectrum->setObjectName("pB_save_spectrum");
        pB_save_spectrum->setGeometry(QRect(20, 220, 80, 25));
        lbl_instrument = new QLabel(Spectrum_Dialog_class);
        lbl_instrument->setObjectName("lbl_instrument");
        lbl_instrument->setGeometry(QRect(110, 220, 91, 21));
        lbl_instrument->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));
        label = new QLabel(Spectrum_Dialog_class);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 200, 91, 20));
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Ubuntu Sans\";"));
        frame_rb_osc = new QFrame(Spectrum_Dialog_class);
        frame_rb_osc->setObjectName("frame_rb_osc");
        frame_rb_osc->setGeometry(QRect(270, 10, 80, 151));
        frame_rb_osc->setFrameShape(QFrame::Shape::StyledPanel);
        frame_rb_osc->setFrameShadow(QFrame::Shadow::Raised);
        layoutWidget = new QWidget(frame_rb_osc);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 66, 131));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        rb_spec_fmo = new QRadioButton(layoutWidget);
        rb_spec_fmo->setObjectName("rb_spec_fmo");

        gridLayout_3->addWidget(rb_spec_fmo, 4, 0, 1, 1);

        rb_spec_main = new QRadioButton(layoutWidget);
        rb_spec_main->setObjectName("rb_spec_main");

        gridLayout_3->addWidget(rb_spec_main, 0, 0, 1, 1);

        rb_spec_vco = new QRadioButton(layoutWidget);
        rb_spec_vco->setObjectName("rb_spec_vco");

        gridLayout_3->addWidget(rb_spec_vco, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 3, 0, 1, 1);

        lcd_spectrumDisplay = new QLCDNumber(Spectrum_Dialog_class);
        lcd_spectrumDisplay->setObjectName("lcd_spectrumDisplay");
        lcd_spectrumDisplay->setGeometry(QRect(270, 210, 81, 51));
        lbl_spectrumDisplay = new QLabel(Spectrum_Dialog_class);
        lbl_spectrumDisplay->setObjectName("lbl_spectrumDisplay");
        lbl_spectrumDisplay->setGeometry(QRect(200, 230, 66, 18));
        rb_reset = new QRadioButton(Spectrum_Dialog_class);
        rb_reset->setObjectName("rb_reset");
        rb_reset->setGeometry(QRect(20, 170, 61, 24));
        lbl_waveform = new QLabel(Spectrum_Dialog_class);
        lbl_waveform->setObjectName("lbl_waveform");
        lbl_waveform->setGeometry(QRect(270, 170, 81, 21));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Active, QPalette::PlaceholderText, brush7);
#endif
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush12);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush14);
#endif
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush15);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush14);
#endif
        lbl_waveform->setPalette(palette1);
        lbl_waveform->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));
        sb_spwf1 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf1->setObjectName("sb_spwf1");
        sb_spwf1->setGeometry(QRect(30, 140, 44, 27));
        sb_spwf2 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf2->setObjectName("sb_spwf2");
        sb_spwf2->setGeometry(QRect(90, 140, 44, 27));
        sb_spwf3 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf3->setObjectName("sb_spwf3");
        sb_spwf3->setGeometry(QRect(150, 140, 44, 27));
        sb_spwf4 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf4->setObjectName("sb_spwf4");
        sb_spwf4->setGeometry(QRect(210, 140, 44, 27));
        frame_2 = new QFrame(Spectrum_Dialog_class);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(370, 10, 104, 151));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName("verticalLayout");
        hs_osc_attack = new QSlider(frame_2);
        hs_osc_attack->setObjectName("hs_osc_attack");
        hs_osc_attack->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_osc_attack);

        hs_osc_decay = new QSlider(frame_2);
        hs_osc_decay->setObjectName("hs_osc_decay");
        hs_osc_decay->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_osc_decay);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        hs_vco_attack = new QSlider(frame_2);
        hs_vco_attack->setObjectName("hs_vco_attack");
        hs_vco_attack->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_vco_attack);

        hs_vco_decay = new QSlider(frame_2);
        hs_vco_decay->setObjectName("hs_vco_decay");
        hs_vco_decay->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_vco_decay);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        hs_fmo_attack = new QSlider(frame_2);
        hs_fmo_attack->setObjectName("hs_fmo_attack");
        hs_fmo_attack->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_fmo_attack);

        hs_fmo_decay = new QSlider(frame_2);
        hs_fmo_decay->setObjectName("hs_fmo_decay");
        hs_fmo_decay->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_fmo_decay);

        label_2 = new QLabel(Spectrum_Dialog_class);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(390, 170, 84, 12));

        retranslateUi(Spectrum_Dialog_class);

        QMetaObject::connectSlotsByName(Spectrum_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Spectrum_Dialog_class)
    {
        Spectrum_Dialog_class->setWindowTitle(QCoreApplication::translate("Spectrum_Dialog_class", "Spectrum", nullptr));
        pB_save_spectrum->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Save", nullptr));
        lbl_instrument->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Instrument", nullptr));
        label->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Instrument:", nullptr));
        rb_spec_fmo->setText(QCoreApplication::translate("Spectrum_Dialog_class", "FMO", nullptr));
        rb_spec_main->setText(QCoreApplication::translate("Spectrum_Dialog_class", "OSC", nullptr));
        rb_spec_vco->setText(QCoreApplication::translate("Spectrum_Dialog_class", "VCO", nullptr));
        lbl_spectrumDisplay->setText(QCoreApplication::translate("Spectrum_Dialog_class", "TextLabel", nullptr));
        rb_reset->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Reset", nullptr));
        lbl_waveform->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Wafeform", nullptr));
        label_2->setText(QCoreApplication::translate("Spectrum_Dialog_class", "ADSR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Spectrum_Dialog_class: public Ui_Spectrum_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECTRUM_DIALOG_CLASS_H
