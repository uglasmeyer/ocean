/********************************************************************************
** Form generated from reading UI file 'spectrum_dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECTRUM_DIALOG_CLASS_H
#define UI_SPECTRUM_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Spectrum_Dialog_class
{
public:
    QPushButton *pB_save_spectrum;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *lbl_waveform;
    QLabel *lbl_spectrumDisplay;
    QLabel *lbl_waveform_txt;
    QSpacerItem *verticalSpacer;
    QLCDNumber *lcd_spectrumDisplay;
    QSpacerItem *verticalSpacer_2;
    QRadioButton *rb_high_low;
    QWidget *layoutWidget1;
    QGridLayout *DialoggridLayout;
    QSpinBox *sb_spwf2;
    QSpinBox *sb_spwf4;
    QGridLayout *gridLayout_3;
    QRadioButton *rb_spec_fmo;
    QRadioButton *rb_spec_main;
    QRadioButton *rb_spec_vco;
    QHBoxLayout *horizontalLayout_5;
    QSlider *fS_4;
    QSlider *vS_4;
    QRadioButton *rb_reset;
    QHBoxLayout *horizontalLayout_3;
    QSlider *fS_2;
    QSlider *vS_2;
    QHBoxLayout *horizontalLayout_4;
    QSlider *fS_3;
    QSlider *vS_3;
    QSpinBox *sb_spwf1;
    QSpinBox *sb_spwf3;
    QHBoxLayout *horizontalLayout_2;
    QSlider *fS_1;
    QSlider *vS_1;
    QLabel *label_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QSlider *hs_attack;
    QLabel *label_5;
    QSlider *hs_decay;
    QComboBox *cb_bps;
    QCheckBox *cb_adsr;
    QGraphicsView *fft_dataview;
    QGraphicsView *adsr_dataview;
    QPushButton *pB_Fourier;
    QComboBox *cB_resolution;
    QLabel *label;
    QLabel *label_4;
    QPushButton *pB_fourier_input;
    QSlider *sl_shift_frequency;
    QComboBox *cB_fourierfilter;

    void setupUi(QDialog *Spectrum_Dialog_class)
    {
        if (Spectrum_Dialog_class->objectName().isEmpty())
            Spectrum_Dialog_class->setObjectName("Spectrum_Dialog_class");
        Spectrum_Dialog_class->resize(705, 269);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Spectrum_Dialog_class->sizePolicy().hasHeightForWidth());
        Spectrum_Dialog_class->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(254, 254, 253, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush);
        QBrush brush1(QColor(153, 193, 241, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        Spectrum_Dialog_class->setPalette(palette);
        pB_save_spectrum = new QPushButton(Spectrum_Dialog_class);
        pB_save_spectrum->setObjectName("pB_save_spectrum");
        pB_save_spectrum->setGeometry(QRect(10, 220, 124, 26));
        layoutWidget = new QWidget(Spectrum_Dialog_class);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(9, 130, 262, 71));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lbl_waveform = new QLabel(layoutWidget);
        lbl_waveform->setObjectName("lbl_waveform");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbl_waveform->sizePolicy().hasHeightForWidth());
        lbl_waveform->setSizePolicy(sizePolicy1);
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush2);
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush3);
        QBrush brush4(QColor(204, 224, 248, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush4);
        QBrush brush5(QColor(76, 96, 120, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush5);
        QBrush brush6(QColor(102, 129, 161, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush6);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush2);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush2);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush7);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush2);
        QBrush brush8(QColor(0, 0, 0, 127));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush8);
#endif
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush3);
        QBrush brush9(QColor(202, 202, 202, 255));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush9);
        QBrush brush10(QColor(159, 159, 159, 255));
        brush10.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush10);
        QBrush brush11(QColor(184, 184, 184, 255));
        brush11.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush11);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush2);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush3);
        QBrush brush12(QColor(118, 118, 118, 255));
        brush12.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush12);
        QBrush brush13(QColor(247, 247, 247, 255));
        brush13.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush13);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush7);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush2);
        QBrush brush14(QColor(0, 0, 0, 128));
        brush14.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush14);
#endif
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush5);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush4);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush5);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush6);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush5);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush5);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush3);
        QBrush brush15(QColor(177, 177, 177, 255));
        brush15.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush15);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush13);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush7);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush2);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush14);
#endif
        lbl_waveform->setPalette(palette1);
        lbl_waveform->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 11pt \"Ubuntu\";"));

        gridLayout_2->addWidget(lbl_waveform, 3, 0, 1, 1);

        lbl_spectrumDisplay = new QLabel(layoutWidget);
        lbl_spectrumDisplay->setObjectName("lbl_spectrumDisplay");

        gridLayout_2->addWidget(lbl_spectrumDisplay, 2, 2, 1, 1);

        lbl_waveform_txt = new QLabel(layoutWidget);
        lbl_waveform_txt->setObjectName("lbl_waveform_txt");
        lbl_waveform_txt->setStyleSheet(QString::fromUtf8("font: 600 11pt \"Ubuntu Sans\";"));

        gridLayout_2->addWidget(lbl_waveform_txt, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(40, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 0, 1, 1, 1);

        lcd_spectrumDisplay = new QLCDNumber(layoutWidget);
        lcd_spectrumDisplay->setObjectName("lcd_spectrumDisplay");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lcd_spectrumDisplay->sizePolicy().hasHeightForWidth());
        lcd_spectrumDisplay->setSizePolicy(sizePolicy2);
        lcd_spectrumDisplay->setSizeIncrement(QSize(0, 0));
        QFont font;
        font.setBold(true);
        lcd_spectrumDisplay->setFont(font);
        lcd_spectrumDisplay->setStyleSheet(QString::fromUtf8("gridline-color: rgb(0, 0, 0);"));

        gridLayout_2->addWidget(lcd_spectrumDisplay, 3, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 4, 0, 1, 1);

        rb_high_low = new QRadioButton(layoutWidget);
        rb_high_low->setObjectName("rb_high_low");

        gridLayout_2->addWidget(rb_high_low, 2, 1, 1, 1);

        layoutWidget1 = new QWidget(Spectrum_Dialog_class);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(0, 0, 252, 127));
        DialoggridLayout = new QGridLayout(layoutWidget1);
        DialoggridLayout->setObjectName("DialoggridLayout");
        DialoggridLayout->setContentsMargins(10, 10, 0, 0);
        sb_spwf2 = new QSpinBox(layoutWidget1);
        sb_spwf2->setObjectName("sb_spwf2");
        sb_spwf2->setWrapping(true);
        sb_spwf2->setMinimum(0);
        sb_spwf2->setMaximum(9);

        DialoggridLayout->addWidget(sb_spwf2, 1, 1, 1, 1);

        sb_spwf4 = new QSpinBox(layoutWidget1);
        sb_spwf4->setObjectName("sb_spwf4");
        sb_spwf4->setWrapping(true);
        sb_spwf4->setMinimum(0);
        sb_spwf4->setMaximum(9);

        DialoggridLayout->addWidget(sb_spwf4, 1, 5, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        rb_spec_fmo = new QRadioButton(layoutWidget1);
        rb_spec_fmo->setObjectName("rb_spec_fmo");

        gridLayout_3->addWidget(rb_spec_fmo, 2, 0, 1, 1);

        rb_spec_main = new QRadioButton(layoutWidget1);
        rb_spec_main->setObjectName("rb_spec_main");

        gridLayout_3->addWidget(rb_spec_main, 0, 0, 1, 1);

        rb_spec_vco = new QRadioButton(layoutWidget1);
        rb_spec_vco->setObjectName("rb_spec_vco");

        gridLayout_3->addWidget(rb_spec_vco, 1, 0, 1, 1);


        DialoggridLayout->addLayout(gridLayout_3, 0, 6, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        fS_4 = new QSlider(layoutWidget1);
        fS_4->setObjectName("fS_4");
        fS_4->setMinimum(0);
        fS_4->setMaximum(50);
        fS_4->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_5->addWidget(fS_4);

        vS_4 = new QSlider(layoutWidget1);
        vS_4->setObjectName("vS_4");
        vS_4->setMaximum(100);
        vS_4->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_5->addWidget(vS_4);


        DialoggridLayout->addLayout(horizontalLayout_5, 0, 5, 1, 1);

        rb_reset = new QRadioButton(layoutWidget1);
        rb_reset->setObjectName("rb_reset");

        DialoggridLayout->addWidget(rb_reset, 1, 6, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        fS_2 = new QSlider(layoutWidget1);
        fS_2->setObjectName("fS_2");
        fS_2->setMinimum(0);
        fS_2->setMaximum(50);
        fS_2->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_3->addWidget(fS_2);

        vS_2 = new QSlider(layoutWidget1);
        vS_2->setObjectName("vS_2");
        vS_2->setMaximum(100);
        vS_2->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_3->addWidget(vS_2);


        DialoggridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        fS_3 = new QSlider(layoutWidget1);
        fS_3->setObjectName("fS_3");
        fS_3->setMinimum(0);
        fS_3->setMaximum(50);
        fS_3->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_4->addWidget(fS_3);

        vS_3 = new QSlider(layoutWidget1);
        vS_3->setObjectName("vS_3");
        vS_3->setMaximum(100);
        vS_3->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_4->addWidget(vS_3);


        DialoggridLayout->addLayout(horizontalLayout_4, 0, 4, 1, 1);

        sb_spwf1 = new QSpinBox(layoutWidget1);
        sb_spwf1->setObjectName("sb_spwf1");
        sb_spwf1->setWrapping(true);
        sb_spwf1->setMinimum(0);
        sb_spwf1->setMaximum(9);
        sb_spwf1->setStepType(QAbstractSpinBox::StepType::DefaultStepType);
        sb_spwf1->setDisplayIntegerBase(10);

        DialoggridLayout->addWidget(sb_spwf1, 1, 0, 1, 1);

        sb_spwf3 = new QSpinBox(layoutWidget1);
        sb_spwf3->setObjectName("sb_spwf3");
        sb_spwf3->setWrapping(true);
        sb_spwf3->setMinimum(0);
        sb_spwf3->setMaximum(9);

        DialoggridLayout->addWidget(sb_spwf3, 1, 4, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        fS_1 = new QSlider(layoutWidget1);
        fS_1->setObjectName("fS_1");
        fS_1->setMinimum(0);
        fS_1->setMaximum(50);
        fS_1->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_2->addWidget(fS_1);

        vS_1 = new QSlider(layoutWidget1);
        vS_1->setObjectName("vS_1");
        vS_1->setMaximum(100);
        vS_1->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_2->addWidget(vS_1);


        DialoggridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        label_3 = new QLabel(Spectrum_Dialog_class);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(259, 39, 129, 18));
        label_3->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);
        label_3->setWordWrap(true);
        frame_2 = new QFrame(Spectrum_Dialog_class);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(259, 95, 104, 104));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName("verticalLayout");
        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        verticalLayout->addWidget(label_2);

        hs_attack = new QSlider(frame_2);
        hs_attack->setObjectName("hs_attack");
        hs_attack->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_attack);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName("label_5");

        verticalLayout->addWidget(label_5);

        hs_decay = new QSlider(frame_2);
        hs_decay->setObjectName("hs_decay");
        hs_decay->setMaximum(100);
        hs_decay->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(hs_decay);

        cb_bps = new QComboBox(Spectrum_Dialog_class);
        cb_bps->setObjectName("cb_bps");
        cb_bps->setGeometry(QRect(259, 63, 86, 26));
        cb_adsr = new QCheckBox(Spectrum_Dialog_class);
        cb_adsr->setObjectName("cb_adsr");
        cb_adsr->setGeometry(QRect(259, 10, 62, 23));
        fft_dataview = new QGraphicsView(Spectrum_Dialog_class);
        fft_dataview->setObjectName("fft_dataview");
        fft_dataview->setGeometry(QRect(420, 110, 260, 91));
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(fft_dataview->sizePolicy().hasHeightForWidth());
        fft_dataview->setSizePolicy(sizePolicy3);
        fft_dataview->setMinimumSize(QSize(260, 64));
        adsr_dataview = new QGraphicsView(Spectrum_Dialog_class);
        adsr_dataview->setObjectName("adsr_dataview");
        adsr_dataview->setGeometry(QRect(420, 10, 260, 94));
        sizePolicy.setHeightForWidth(adsr_dataview->sizePolicy().hasHeightForWidth());
        adsr_dataview->setSizePolicy(sizePolicy);
        adsr_dataview->setMinimumSize(QSize(260, 64));
        adsr_dataview->setMaximumSize(QSize(260, 128));
        pB_Fourier = new QPushButton(Spectrum_Dialog_class);
        pB_Fourier->setObjectName("pB_Fourier");
        pB_Fourier->setGeometry(QRect(140, 220, 71, 26));
        cB_resolution = new QComboBox(Spectrum_Dialog_class);
        cB_resolution->setObjectName("cB_resolution");
        cB_resolution->setGeometry(QRect(320, 220, 61, 26));
        label = new QLabel(Spectrum_Dialog_class);
        label->setObjectName("label");
        label->setGeometry(QRect(390, 20, 31, 20));
        label_4 = new QLabel(Spectrum_Dialog_class);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(390, 110, 31, 20));
        pB_fourier_input = new QPushButton(Spectrum_Dialog_class);
        pB_fourier_input->setObjectName("pB_fourier_input");
        pB_fourier_input->setGeometry(QRect(220, 220, 94, 26));
        sl_shift_frequency = new QSlider(Spectrum_Dialog_class);
        sl_shift_frequency->setObjectName("sl_shift_frequency");
        sl_shift_frequency->setGeometry(QRect(579, 210, 101, 20));
        sl_shift_frequency->setMinimum(-128);
        sl_shift_frequency->setMaximum(128);
        sl_shift_frequency->setSliderPosition(0);
        sl_shift_frequency->setOrientation(Qt::Orientation::Horizontal);
        sl_shift_frequency->setTickPosition(QSlider::TickPosition::TicksBelow);
        sl_shift_frequency->setTickInterval(4);
        cB_fourierfilter = new QComboBox(Spectrum_Dialog_class);
        cB_fourierfilter->setObjectName("cB_fourierfilter");
        cB_fourierfilter->setGeometry(QRect(390, 220, 71, 26));

        retranslateUi(Spectrum_Dialog_class);

        QMetaObject::connectSlotsByName(Spectrum_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *Spectrum_Dialog_class)
    {
        Spectrum_Dialog_class->setWindowTitle(QCoreApplication::translate("Spectrum_Dialog_class", "Spectrum", nullptr));
        pB_save_spectrum->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Save Instrument", nullptr));
        lbl_waveform->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Wafeform", nullptr));
        lbl_spectrumDisplay->setText(QCoreApplication::translate("Spectrum_Dialog_class", "TextLabel", nullptr));
        lbl_waveform_txt->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Waveform", nullptr));
        rb_high_low->setText(QCoreApplication::translate("Spectrum_Dialog_class", "High/low", nullptr));
        rb_spec_fmo->setText(QCoreApplication::translate("Spectrum_Dialog_class", "FMO", nullptr));
        rb_spec_main->setText(QCoreApplication::translate("Spectrum_Dialog_class", "OSC", nullptr));
        rb_spec_vco->setText(QCoreApplication::translate("Spectrum_Dialog_class", "VCO", nullptr));
        rb_reset->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Reset", nullptr));
        label_3->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Beats per measure:", nullptr));
        label_2->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Attack", nullptr));
        label_5->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Decay", nullptr));
        cb_adsr->setText(QCoreApplication::translate("Spectrum_Dialog_class", "ADSR", nullptr));
        pB_Fourier->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Fourier", nullptr));
        label->setText(QCoreApplication::translate("Spectrum_Dialog_class", "A(\317\204)", nullptr));
        label_4->setText(QCoreApplication::translate("Spectrum_Dialog_class", "A(\317\211)", nullptr));
        pB_fourier_input->setText(QCoreApplication::translate("Spectrum_Dialog_class", "Fourier Input", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Spectrum_Dialog_class: public Ui_Spectrum_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECTRUM_DIALOG_CLASS_H
