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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Spectrum_Dialog_class
{
public:
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QSpinBox *sb_spwf2;
    QSpinBox *sb_spwf4;
    QGridLayout *gridLayout_3;
    QRadioButton *rb_spec_fmo;
    QRadioButton *rb_spec_main;
    QRadioButton *rb_spec_vco;
    QHBoxLayout *horizontalLayout_5;
    QSlider *fS_7;
    QSlider *vS_8;
    QRadioButton *rb_reset;
    QHBoxLayout *horizontalLayout_3;
    QSlider *fS_3;
    QSlider *vS_4;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSlider *fS_1;
    QSlider *vS_2;
    QHBoxLayout *horizontalLayout_4;
    QSlider *fS_5;
    QSlider *vS_6;
    QSpinBox *sb_spwf1;
    QSpinBox *sb_spwf3;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *cb_adsr;
    QLabel *label_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QSlider *hs_attack;
    QSlider *hs_decay;
    QSpinBox *sb_adsrwf;
    QLabel *lbl_adsrwf;
    QGridLayout *gridLayout_2;
    QLabel *lbl_instrument;
    QPushButton *pB_save_spectrum;
    QLabel *lbl_spectrumDisplay;
    QLCDNumber *lcd_spectrumDisplay;
    QLabel *lbl_waveform;
    QLabel *label;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Spectrum_Dialog_class)
    {
        if (Spectrum_Dialog_class->objectName().isEmpty())
            Spectrum_Dialog_class->setObjectName("Spectrum_Dialog_class");
        Spectrum_Dialog_class->resize(467, 213);
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
        gridLayout_4 = new QGridLayout(Spectrum_Dialog_class);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        sb_spwf2 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf2->setObjectName("sb_spwf2");
        sb_spwf2->setWrapping(true);

        gridLayout->addWidget(sb_spwf2, 1, 1, 1, 1);

        sb_spwf4 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf4->setObjectName("sb_spwf4");
        sb_spwf4->setWrapping(true);

        gridLayout->addWidget(sb_spwf4, 1, 5, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        rb_spec_fmo = new QRadioButton(Spectrum_Dialog_class);
        rb_spec_fmo->setObjectName("rb_spec_fmo");

        gridLayout_3->addWidget(rb_spec_fmo, 2, 0, 1, 1);

        rb_spec_main = new QRadioButton(Spectrum_Dialog_class);
        rb_spec_main->setObjectName("rb_spec_main");

        gridLayout_3->addWidget(rb_spec_main, 0, 0, 1, 1);

        rb_spec_vco = new QRadioButton(Spectrum_Dialog_class);
        rb_spec_vco->setObjectName("rb_spec_vco");

        gridLayout_3->addWidget(rb_spec_vco, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout_3, 0, 6, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        fS_7 = new QSlider(Spectrum_Dialog_class);
        fS_7->setObjectName("fS_7");
        fS_7->setMinimum(-50);
        fS_7->setMaximum(50);
        fS_7->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_5->addWidget(fS_7);

        vS_8 = new QSlider(Spectrum_Dialog_class);
        vS_8->setObjectName("vS_8");
        vS_8->setMaximum(100);
        vS_8->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_5->addWidget(vS_8);


        gridLayout->addLayout(horizontalLayout_5, 0, 5, 1, 1);

        rb_reset = new QRadioButton(Spectrum_Dialog_class);
        rb_reset->setObjectName("rb_reset");

        gridLayout->addWidget(rb_reset, 1, 6, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        fS_3 = new QSlider(Spectrum_Dialog_class);
        fS_3->setObjectName("fS_3");
        fS_3->setMinimum(-50);
        fS_3->setMaximum(50);
        fS_3->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_3->addWidget(fS_3);

        vS_4 = new QSlider(Spectrum_Dialog_class);
        vS_4->setObjectName("vS_4");
        vS_4->setMaximum(100);
        vS_4->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_3->addWidget(vS_4);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        fS_1 = new QSlider(Spectrum_Dialog_class);
        fS_1->setObjectName("fS_1");
        fS_1->setMinimum(-50);
        fS_1->setMaximum(50);
        fS_1->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_2->addWidget(fS_1);

        vS_2 = new QSlider(Spectrum_Dialog_class);
        vS_2->setObjectName("vS_2");
        vS_2->setMaximum(100);
        vS_2->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_2->addWidget(vS_2);


        horizontalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        fS_5 = new QSlider(Spectrum_Dialog_class);
        fS_5->setObjectName("fS_5");
        fS_5->setMinimum(-50);
        fS_5->setMaximum(50);
        fS_5->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_4->addWidget(fS_5);

        vS_6 = new QSlider(Spectrum_Dialog_class);
        vS_6->setObjectName("vS_6");
        vS_6->setMaximum(100);
        vS_6->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_4->addWidget(vS_6);


        gridLayout->addLayout(horizontalLayout_4, 0, 4, 1, 1);

        sb_spwf1 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf1->setObjectName("sb_spwf1");
        sb_spwf1->setWrapping(true);

        gridLayout->addWidget(sb_spwf1, 1, 0, 1, 1);

        sb_spwf3 = new QSpinBox(Spectrum_Dialog_class);
        sb_spwf3->setObjectName("sb_spwf3");
        sb_spwf3->setWrapping(true);

        gridLayout->addWidget(sb_spwf3, 1, 4, 1, 1);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        cb_adsr = new QCheckBox(Spectrum_Dialog_class);
        cb_adsr->setObjectName("cb_adsr");

        verticalLayout_2->addWidget(cb_adsr);

        label_2 = new QLabel(Spectrum_Dialog_class);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        frame_2 = new QFrame(Spectrum_Dialog_class);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName("verticalLayout");
        hs_attack = new QSlider(frame_2);
        hs_attack->setObjectName("hs_attack");
        hs_attack->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_attack);

        hs_decay = new QSlider(frame_2);
        hs_decay->setObjectName("hs_decay");
        hs_decay->setMaximum(100);
        hs_decay->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_decay);

        sb_adsrwf = new QSpinBox(frame_2);
        sb_adsrwf->setObjectName("sb_adsrwf");
        sb_adsrwf->setWrapping(true);
        sb_adsrwf->setMaximum(8);

        verticalLayout->addWidget(sb_adsrwf);

        lbl_adsrwf = new QLabel(frame_2);
        lbl_adsrwf->setObjectName("lbl_adsrwf");
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
        lbl_adsrwf->setPalette(palette1);
        lbl_adsrwf->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));

        verticalLayout->addWidget(lbl_adsrwf);


        verticalLayout_2->addWidget(frame_2);


        gridLayout_4->addLayout(verticalLayout_2, 0, 1, 2, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        lbl_instrument = new QLabel(Spectrum_Dialog_class);
        lbl_instrument->setObjectName("lbl_instrument");
        lbl_instrument->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));

        gridLayout_2->addWidget(lbl_instrument, 2, 1, 1, 1);

        pB_save_spectrum = new QPushButton(Spectrum_Dialog_class);
        pB_save_spectrum->setObjectName("pB_save_spectrum");

        gridLayout_2->addWidget(pB_save_spectrum, 2, 2, 1, 1);

        lbl_spectrumDisplay = new QLabel(Spectrum_Dialog_class);
        lbl_spectrumDisplay->setObjectName("lbl_spectrumDisplay");

        gridLayout_2->addWidget(lbl_spectrumDisplay, 1, 1, 1, 1);

        lcd_spectrumDisplay = new QLCDNumber(Spectrum_Dialog_class);
        lcd_spectrumDisplay->setObjectName("lcd_spectrumDisplay");

        gridLayout_2->addWidget(lcd_spectrumDisplay, 1, 2, 1, 1);

        lbl_waveform = new QLabel(Spectrum_Dialog_class);
        lbl_waveform->setObjectName("lbl_waveform");
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Active, QPalette::PlaceholderText, brush7);
#endif
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush14);
#endif
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush14);
#endif
        lbl_waveform->setPalette(palette2);
        lbl_waveform->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));

        gridLayout_2->addWidget(lbl_waveform, 1, 0, 1, 1);

        label = new QLabel(Spectrum_Dialog_class);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Ubuntu Sans\";"));

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(40, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 1, 0, 1, 1);


        retranslateUi(Spectrum_Dialog_class);

        QMetaObject::connectSlotsByName(Spectrum_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Spectrum_Dialog_class)
    {
        Spectrum_Dialog_class->setWindowTitle(QCoreApplication::translate("Spectrum_Dialog_class", "Spectrum", nullptr));
        rb_spec_fmo->setText(QCoreApplication::translate("Spectrum_Dialog_class", "FMO", nullptr));
        rb_spec_main->setText(QCoreApplication::translate("Spectrum_Dialog_class", "OSC", nullptr));
        rb_spec_vco->setText(QCoreApplication::translate("Spectrum_Dialog_class", "VCO", nullptr));
        rb_reset->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Reset", nullptr));
        cb_adsr->setText(QCoreApplication::translate("Spectrum_Dialog_class", "ADSR", nullptr));
        label_2->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Attack/Delay", nullptr));
        lbl_adsrwf->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Wafeform", nullptr));
        lbl_instrument->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Instrument", nullptr));
        pB_save_spectrum->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Save", nullptr));
        lbl_spectrumDisplay->setText(QCoreApplication::translate("Spectrum_Dialog_class", "TextLabel", nullptr));
        lbl_waveform->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Wafeform", nullptr));
        label->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Instrument:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Spectrum_Dialog_class: public Ui_Spectrum_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECTRUM_DIALOG_CLASS_H
