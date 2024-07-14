/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLCDNumber *MainLCD_Amp;
    QSlider *Slider_Main_Hz;
    QSlider *Slider_Main_Vol;
    QSlider *Slider_VCO_Hz;
    QSlider *Slider_VCO_vol;
    QLCDNumber *VCOLCD_Hz;
    QLCDNumber *lcdNumber_2;
    QSlider *Slider_FMO_Hz;
    QSlider *Slider_FMO_vol;
    QLCDNumber *lcdNumber_3;
    QLCDNumber *FMOLCD_Hz;
    QLabel *labelVCO;
    QLabel *labelFMO;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QRadioButton *rb_reset;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QFrame *frame;
    QRadioButton *rb_bank1;
    QRadioButton *rb_bank2;
    QRadioButton *rb_bank3;
    QLabel *label_16;
    QRadioButton *rb_bank7;
    QRadioButton *rb_bank4;
    QRadioButton *rb_bank5;
    QFrame *frame_2;
    QSlider *Slider_mix_vol1;
    QSlider *Slider_mix_vol2;
    QSlider *Slider_mix_vol3;
    QSlider *Slider_mix_vol4;
    QSlider *Slider_mix_vol5;
    QSlider *Slider_mix_vol7;
    QSlider *Slider_mix_vol6;
    QFrame *frame_4;
    QCheckBox *cb_1;
    QCheckBox *cb_2;
    QCheckBox *cb_3;
    QCheckBox *cb_4;
    QCheckBox *cb_5;
    QCheckBox *cb_6;
    QCheckBox *cb_7;
    QPushButton *pB_Store;
    QPushButton *pB_Mute_StA;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pBAudioServer;
    QPushButton *pBAudioServerExit;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pBSynthesizer;
    QPushButton *pBSynthesizerExit;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pB_Save;
    QPushButton *pushButton_3;
    QDial *dial_PMW;
    QLabel *label_2;
    QPushButton *pB_Mute;
    QPushButton *pB_play_notes;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QPushButton *pBtoggleRecord;
    QLabel *label_4;
    QFrame *frame_5;
    QRadioButton *rb_melody_2;
    QRadioButton *rb_melody_3;
    QRadioButton *rb_melody_1;
    QRadioButton *rb_melody_4;
    QProgressBar *progressBar_record;
    QLCDNumber *MainLCD_Hz;
    QLabel *label_11;
    QDial *dial_soft_freq;
    QLabel *glidefrequency;
    QPushButton *pB_Wavedisplay;
    QGraphicsView *oscilloscope_view;
    QPushButton *pB_Debug;
    QComboBox *cb_external;
    QScrollBar *hs_hall_effect;
    QLabel *label_5;
    QScrollBar *hs_adsr_sustain;
    QScrollBar *hs_adsr_attack;
    QDial *dial_ramp_up_down;
    QPushButton *pB_Specrum;
    QSpinBox *sB_Duration;
    QFrame *frame_7;
    QLabel *wf_vco;
    QSpinBox *sB_VCO;
    QFrame *frame_8;
    QSpinBox *sB_Main;
    QLabel *wf_main;
    QFrame *frame_9;
    QLabel *wf_fmo;
    QSpinBox *sB_FMO;
    QPushButton *pb_clear;
    QLabel *label_3;
    QLabel *Bps;
    QLabel *label_6;
    QLabel *label_10;
    QMenuBar *menubar;
    QMenu *menuSound_Lab_GUI;
    QMenu *menuIO;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1207, 600);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(26, 95, 180, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(52, 143, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(39, 119, 217, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(13, 47, 90, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(17, 63, 120, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush6);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        QBrush brush7(QColor(140, 175, 217, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        QBrush brush9(QColor(0, 0, 0, 127));
        brush9.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush9);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        QBrush brush10(QColor(239, 239, 239, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        QBrush brush11(QColor(202, 202, 202, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush11);
        QBrush brush12(QColor(159, 159, 159, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush12);
        QBrush brush13(QColor(184, 184, 184, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush13);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        QBrush brush14(QColor(118, 118, 118, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush14);
        QBrush brush15(QColor(247, 247, 247, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush15);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        QBrush brush16(QColor(0, 0, 0, 128));
        brush16.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush16);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        QBrush brush17(QColor(177, 177, 177, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush17);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush16);
#endif
        MainWindow->setPalette(palette);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 10, 31, 17));
        MainLCD_Amp = new QLCDNumber(centralwidget);
        MainLCD_Amp->setObjectName(QString::fromUtf8("MainLCD_Amp"));
        MainLCD_Amp->setGeometry(QRect(330, 10, 111, 51));
        Slider_Main_Hz = new QSlider(centralwidget);
        Slider_Main_Hz->setObjectName(QString::fromUtf8("Slider_Main_Hz"));
        Slider_Main_Hz->setGeometry(QRect(250, 10, 16, 160));
        Slider_Main_Hz->setMinimum(1);
        Slider_Main_Hz->setMaximum(1000);
        Slider_Main_Hz->setPageStep(12);
        Slider_Main_Hz->setOrientation(Qt::Vertical);
        Slider_Main_Vol = new QSlider(centralwidget);
        Slider_Main_Vol->setObjectName(QString::fromUtf8("Slider_Main_Vol"));
        Slider_Main_Vol->setGeometry(QRect(290, 10, 16, 160));
        Slider_Main_Vol->setMaximum(100);
        Slider_Main_Vol->setOrientation(Qt::Vertical);
        Slider_VCO_Hz = new QSlider(centralwidget);
        Slider_VCO_Hz->setObjectName(QString::fromUtf8("Slider_VCO_Hz"));
        Slider_VCO_Hz->setGeometry(QRect(250, 190, 16, 160));
        Slider_VCO_Hz->setMaximum(400);
        Slider_VCO_Hz->setSingleStep(1);
        Slider_VCO_Hz->setOrientation(Qt::Vertical);
        Slider_VCO_vol = new QSlider(centralwidget);
        Slider_VCO_vol->setObjectName(QString::fromUtf8("Slider_VCO_vol"));
        Slider_VCO_vol->setGeometry(QRect(290, 190, 16, 160));
        Slider_VCO_vol->setMaximum(100);
        Slider_VCO_vol->setOrientation(Qt::Vertical);
        VCOLCD_Hz = new QLCDNumber(centralwidget);
        VCOLCD_Hz->setObjectName(QString::fromUtf8("VCOLCD_Hz"));
        VCOLCD_Hz->setGeometry(QRect(60, 190, 131, 51));
        lcdNumber_2 = new QLCDNumber(centralwidget);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(330, 380, 111, 51));
        Slider_FMO_Hz = new QSlider(centralwidget);
        Slider_FMO_Hz->setObjectName(QString::fromUtf8("Slider_FMO_Hz"));
        Slider_FMO_Hz->setGeometry(QRect(250, 380, 16, 160));
        Slider_FMO_Hz->setMaximum(800);
        Slider_FMO_Hz->setSingleStep(1);
        Slider_FMO_Hz->setOrientation(Qt::Vertical);
        Slider_FMO_vol = new QSlider(centralwidget);
        Slider_FMO_vol->setObjectName(QString::fromUtf8("Slider_FMO_vol"));
        Slider_FMO_vol->setGeometry(QRect(290, 380, 16, 160));
        Slider_FMO_vol->setOrientation(Qt::Vertical);
        lcdNumber_3 = new QLCDNumber(centralwidget);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(330, 190, 111, 51));
        FMOLCD_Hz = new QLCDNumber(centralwidget);
        FMOLCD_Hz->setObjectName(QString::fromUtf8("FMOLCD_Hz"));
        FMOLCD_Hz->setGeometry(QRect(60, 380, 131, 51));
        labelVCO = new QLabel(centralwidget);
        labelVCO->setObjectName(QString::fromUtf8("labelVCO"));
        labelVCO->setGeometry(QRect(260, 190, 31, 17));
        labelFMO = new QLabel(centralwidget);
        labelFMO->setObjectName(QString::fromUtf8("labelFMO"));
        labelFMO->setGeometry(QRect(260, 380, 31, 17));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(60, 430, 111, 20));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(60, 240, 111, 20));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(60, 60, 111, 20));
        rb_reset = new QRadioButton(centralwidget);
        rb_reset->setObjectName(QString::fromUtf8("rb_reset"));
        rb_reset->setGeometry(QRect(330, 150, 112, 23));
        radioButton_2 = new QRadioButton(centralwidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(330, 310, 112, 23));
        radioButton_3 = new QRadioButton(centralwidget);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(330, 500, 112, 23));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(960, 50, 131, 211));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        rb_bank1 = new QRadioButton(frame);
        rb_bank1->setObjectName(QString::fromUtf8("rb_bank1"));
        rb_bank1->setGeometry(QRect(0, 0, 121, 23));
        rb_bank2 = new QRadioButton(frame);
        rb_bank2->setObjectName(QString::fromUtf8("rb_bank2"));
        rb_bank2->setGeometry(QRect(0, 30, 121, 23));
        rb_bank3 = new QRadioButton(frame);
        rb_bank3->setObjectName(QString::fromUtf8("rb_bank3"));
        rb_bank3->setGeometry(QRect(0, 60, 121, 23));
        label_16 = new QLabel(frame);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 180, 66, 17));
        rb_bank7 = new QRadioButton(frame);
        rb_bank7->setObjectName(QString::fromUtf8("rb_bank7"));
        rb_bank7->setGeometry(QRect(0, 150, 121, 23));
        rb_bank4 = new QRadioButton(frame);
        rb_bank4->setObjectName(QString::fromUtf8("rb_bank4"));
        rb_bank4->setGeometry(QRect(0, 90, 121, 23));
        rb_bank5 = new QRadioButton(frame);
        rb_bank5->setObjectName(QString::fromUtf8("rb_bank5"));
        rb_bank5->setGeometry(QRect(0, 120, 121, 23));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(760, 50, 191, 211));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        Slider_mix_vol1 = new QSlider(frame_2);
        Slider_mix_vol1->setObjectName(QString::fromUtf8("Slider_mix_vol1"));
        Slider_mix_vol1->setGeometry(QRect(0, 0, 160, 16));
        Slider_mix_vol1->setOrientation(Qt::Horizontal);
        Slider_mix_vol2 = new QSlider(frame_2);
        Slider_mix_vol2->setObjectName(QString::fromUtf8("Slider_mix_vol2"));
        Slider_mix_vol2->setGeometry(QRect(0, 30, 160, 16));
        Slider_mix_vol2->setOrientation(Qt::Horizontal);
        Slider_mix_vol3 = new QSlider(frame_2);
        Slider_mix_vol3->setObjectName(QString::fromUtf8("Slider_mix_vol3"));
        Slider_mix_vol3->setGeometry(QRect(0, 60, 160, 16));
        Slider_mix_vol3->setOrientation(Qt::Horizontal);
        Slider_mix_vol4 = new QSlider(frame_2);
        Slider_mix_vol4->setObjectName(QString::fromUtf8("Slider_mix_vol4"));
        Slider_mix_vol4->setGeometry(QRect(0, 90, 160, 16));
        Slider_mix_vol4->setMaximum(120);
        Slider_mix_vol4->setOrientation(Qt::Horizontal);
        Slider_mix_vol5 = new QSlider(frame_2);
        Slider_mix_vol5->setObjectName(QString::fromUtf8("Slider_mix_vol5"));
        Slider_mix_vol5->setGeometry(QRect(0, 120, 160, 16));
        Slider_mix_vol5->setOrientation(Qt::Horizontal);
        Slider_mix_vol7 = new QSlider(frame_2);
        Slider_mix_vol7->setObjectName(QString::fromUtf8("Slider_mix_vol7"));
        Slider_mix_vol7->setGeometry(QRect(0, 150, 160, 16));
        Slider_mix_vol7->setOrientation(Qt::Horizontal);
        Slider_mix_vol6 = new QSlider(frame_2);
        Slider_mix_vol6->setObjectName(QString::fromUtf8("Slider_mix_vol6"));
        Slider_mix_vol6->setGeometry(QRect(0, 180, 160, 16));
        Slider_mix_vol6->setOrientation(Qt::Horizontal);
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(1100, 50, 91, 211));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        cb_1 = new QCheckBox(frame_4);
        cb_1->setObjectName(QString::fromUtf8("cb_1"));
        cb_1->setGeometry(QRect(0, 0, 92, 23));
        cb_2 = new QCheckBox(frame_4);
        cb_2->setObjectName(QString::fromUtf8("cb_2"));
        cb_2->setGeometry(QRect(0, 30, 92, 23));
        cb_3 = new QCheckBox(frame_4);
        cb_3->setObjectName(QString::fromUtf8("cb_3"));
        cb_3->setGeometry(QRect(0, 60, 92, 23));
        cb_4 = new QCheckBox(frame_4);
        cb_4->setObjectName(QString::fromUtf8("cb_4"));
        cb_4->setGeometry(QRect(0, 90, 92, 23));
        cb_5 = new QCheckBox(frame_4);
        cb_5->setObjectName(QString::fromUtf8("cb_5"));
        cb_5->setGeometry(QRect(0, 120, 92, 23));
        cb_6 = new QCheckBox(frame_4);
        cb_6->setObjectName(QString::fromUtf8("cb_6"));
        cb_6->setGeometry(QRect(0, 180, 92, 23));
        cb_7 = new QCheckBox(frame_4);
        cb_7->setObjectName(QString::fromUtf8("cb_7"));
        cb_7->setGeometry(QRect(0, 150, 92, 23));
        pB_Store = new QPushButton(centralwidget);
        pB_Store->setObjectName(QString::fromUtf8("pB_Store"));
        pB_Store->setGeometry(QRect(1000, 270, 89, 25));
        pB_Mute_StA = new QPushButton(centralwidget);
        pB_Mute_StA->setObjectName(QString::fromUtf8("pB_Mute_StA"));
        pB_Mute_StA->setGeometry(QRect(1100, 270, 89, 25));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(540, 470, 449, 80));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame_6 = new QFrame(layoutWidget);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pBAudioServer = new QPushButton(frame_6);
        pBAudioServer->setObjectName(QString::fromUtf8("pBAudioServer"));

        verticalLayout_3->addWidget(pBAudioServer);

        pBAudioServerExit = new QPushButton(frame_6);
        pBAudioServerExit->setObjectName(QString::fromUtf8("pBAudioServerExit"));

        verticalLayout_3->addWidget(pBAudioServerExit);


        horizontalLayout_3->addWidget(frame_6);

        frame_3 = new QFrame(layoutWidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pBSynthesizer = new QPushButton(frame_3);
        pBSynthesizer->setObjectName(QString::fromUtf8("pBSynthesizer"));

        verticalLayout->addWidget(pBSynthesizer);

        pBSynthesizerExit = new QPushButton(frame_3);
        pBSynthesizerExit->setObjectName(QString::fromUtf8("pBSynthesizerExit"));

        verticalLayout->addWidget(pBSynthesizerExit);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_3->addWidget(frame_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pB_Save = new QPushButton(layoutWidget);
        pB_Save->setObjectName(QString::fromUtf8("pB_Save"));

        verticalLayout_2->addWidget(pB_Save);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        verticalLayout_2->addWidget(pushButton_3);


        horizontalLayout_3->addLayout(verticalLayout_2);

        dial_PMW = new QDial(centralwidget);
        dial_PMW->setObjectName(QString::fromUtf8("dial_PMW"));
        dial_PMW->setGeometry(QRect(480, 170, 50, 64));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(490, 160, 81, 17));
        pB_Mute = new QPushButton(centralwidget);
        pB_Mute->setObjectName(QString::fromUtf8("pB_Mute"));
        pB_Mute->setGeometry(QRect(120, 140, 89, 25));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Button, brush10);
        QBrush brush18(QColor(143, 240, 164, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush18);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        pB_Mute->setPalette(palette1);
        pB_Mute->setAutoFillBackground(true);
        pB_play_notes = new QPushButton(centralwidget);
        pB_play_notes->setObjectName(QString::fromUtf8("pB_play_notes"));
        pB_play_notes->setGeometry(QRect(760, 10, 89, 25));
        radioButton_4 = new QRadioButton(centralwidget);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));
        radioButton_4->setGeometry(QRect(330, 330, 112, 23));
        radioButton_5 = new QRadioButton(centralwidget);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));
        radioButton_5->setGeometry(QRect(330, 520, 112, 23));
        pBtoggleRecord = new QPushButton(centralwidget);
        pBtoggleRecord->setObjectName(QString::fromUtf8("pBtoggleRecord"));
        pBtoggleRecord->setGeometry(QRect(480, 440, 89, 25));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(480, 50, 111, 17));
        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(1060, 380, 131, 111));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        rb_melody_2 = new QRadioButton(frame_5);
        rb_melody_2->setObjectName(QString::fromUtf8("rb_melody_2"));
        rb_melody_2->setGeometry(QRect(0, 30, 131, 23));
        rb_melody_3 = new QRadioButton(frame_5);
        rb_melody_3->setObjectName(QString::fromUtf8("rb_melody_3"));
        rb_melody_3->setGeometry(QRect(0, 60, 112, 23));
        rb_melody_1 = new QRadioButton(frame_5);
        rb_melody_1->setObjectName(QString::fromUtf8("rb_melody_1"));
        rb_melody_1->setGeometry(QRect(0, 0, 131, 23));
        rb_melody_4 = new QRadioButton(frame_5);
        rb_melody_4->setObjectName(QString::fromUtf8("rb_melody_4"));
        rb_melody_4->setGeometry(QRect(0, 90, 112, 23));
        progressBar_record = new QProgressBar(centralwidget);
        progressBar_record->setObjectName(QString::fromUtf8("progressBar_record"));
        progressBar_record->setGeometry(QRect(580, 440, 411, 23));
        progressBar_record->setValue(24);
        MainLCD_Hz = new QLCDNumber(centralwidget);
        MainLCD_Hz->setObjectName(QString::fromUtf8("MainLCD_Hz"));
        MainLCD_Hz->setGeometry(QRect(60, 10, 158, 49));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(480, 20, 51, 17));
        dial_soft_freq = new QDial(centralwidget);
        dial_soft_freq->setObjectName(QString::fromUtf8("dial_soft_freq"));
        dial_soft_freq->setGeometry(QRect(580, 170, 50, 64));
        dial_soft_freq->setMinimum(0);
        dial_soft_freq->setMaximum(100);
        glidefrequency = new QLabel(centralwidget);
        glidefrequency->setObjectName(QString::fromUtf8("glidefrequency"));
        glidefrequency->setGeometry(QRect(590, 146, 41, 31));
        glidefrequency->setWordWrap(true);
        pB_Wavedisplay = new QPushButton(centralwidget);
        pB_Wavedisplay->setObjectName(QString::fromUtf8("pB_Wavedisplay"));
        pB_Wavedisplay->setGeometry(QRect(480, 240, 131, 25));
        oscilloscope_view = new QGraphicsView(centralwidget);
        oscilloscope_view->setObjectName(QString::fromUtf8("oscilloscope_view"));
        oscilloscope_view->setGeometry(QRect(480, 270, 512, 166));
        oscilloscope_view->setFrameShape(QFrame::Box);
        oscilloscope_view->setLineWidth(2);
        pB_Debug = new QPushButton(centralwidget);
        pB_Debug->setObjectName(QString::fromUtf8("pB_Debug"));
        pB_Debug->setGeometry(QRect(620, 240, 88, 25));
        cb_external = new QComboBox(centralwidget);
        cb_external->setObjectName(QString::fromUtf8("cb_external"));
        cb_external->setGeometry(QRect(990, 10, 201, 25));
        cb_external->setEditable(false);
        hs_hall_effect = new QScrollBar(centralwidget);
        hs_hall_effect->setObjectName(QString::fromUtf8("hs_hall_effect"));
        hs_hall_effect->setGeometry(QRect(530, 80, 160, 16));
        hs_hall_effect->setMaximum(100);
        hs_hall_effect->setPageStep(1);
        hs_hall_effect->setOrientation(Qt::Horizontal);
        hs_hall_effect->setInvertedControls(false);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(480, 80, 66, 17));
        hs_adsr_sustain = new QScrollBar(centralwidget);
        hs_adsr_sustain->setObjectName(QString::fromUtf8("hs_adsr_sustain"));
        hs_adsr_sustain->setGeometry(QRect(530, 50, 160, 16));
        hs_adsr_sustain->setMaximum(100);
        hs_adsr_sustain->setPageStep(1);
        hs_adsr_sustain->setOrientation(Qt::Horizontal);
        hs_adsr_sustain->setInvertedControls(false);
        hs_adsr_attack = new QScrollBar(centralwidget);
        hs_adsr_attack->setObjectName(QString::fromUtf8("hs_adsr_attack"));
        hs_adsr_attack->setGeometry(QRect(530, 20, 160, 16));
        hs_adsr_attack->setMaximum(100);
        hs_adsr_attack->setPageStep(1);
        hs_adsr_attack->setOrientation(Qt::Horizontal);
        hs_adsr_attack->setInvertedControls(false);
        dial_ramp_up_down = new QDial(centralwidget);
        dial_ramp_up_down->setObjectName(QString::fromUtf8("dial_ramp_up_down"));
        dial_ramp_up_down->setGeometry(QRect(380, 60, 50, 64));
        dial_ramp_up_down->setMaximum(100);
        pB_Specrum = new QPushButton(centralwidget);
        pB_Specrum->setObjectName(QString::fromUtf8("pB_Specrum"));
        pB_Specrum->setGeometry(QRect(870, 10, 88, 25));
        sB_Duration = new QSpinBox(centralwidget);
        sB_Duration->setObjectName(QString::fromUtf8("sB_Duration"));
        sB_Duration->setGeometry(QRect(660, 190, 44, 26));
        sB_Duration->setMaximum(4);
        sB_Duration->setDisplayIntegerBase(10);
        frame_7 = new QFrame(centralwidget);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setGeometry(QRect(60, 270, 131, 41));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        wf_vco = new QLabel(frame_7);
        wf_vco->setObjectName(QString::fromUtf8("wf_vco"));
        wf_vco->setGeometry(QRect(0, 10, 66, 17));
        sB_VCO = new QSpinBox(frame_7);
        sB_VCO->setObjectName(QString::fromUtf8("sB_VCO"));
        sB_VCO->setGeometry(QRect(90, 0, 44, 41));
        sB_VCO->setMaximum(9);
        frame_8 = new QFrame(centralwidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(80, 90, 131, 41));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        sB_Main = new QSpinBox(frame_8);
        sB_Main->setObjectName(QString::fromUtf8("sB_Main"));
        sB_Main->setGeometry(QRect(90, 0, 44, 41));
        sB_Main->setMaximum(9);
        wf_main = new QLabel(frame_8);
        wf_main->setObjectName(QString::fromUtf8("wf_main"));
        wf_main->setGeometry(QRect(0, 10, 66, 17));
        frame_9 = new QFrame(centralwidget);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setGeometry(QRect(60, 460, 131, 41));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        wf_fmo = new QLabel(frame_9);
        wf_fmo->setObjectName(QString::fromUtf8("wf_fmo"));
        wf_fmo->setGeometry(QRect(0, 10, 66, 17));
        sB_FMO = new QSpinBox(frame_9);
        sB_FMO->setObjectName(QString::fromUtf8("sB_FMO"));
        sB_FMO->setGeometry(QRect(90, 0, 44, 41));
        sB_FMO->setMaximum(9);
        pb_clear = new QPushButton(centralwidget);
        pb_clear->setObjectName(QString::fromUtf8("pb_clear"));
        pb_clear->setGeometry(QRect(1000, 300, 88, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(660, 160, 66, 17));
        Bps = new QLabel(centralwidget);
        Bps->setObjectName(QString::fromUtf8("Bps"));
        Bps->setGeometry(QRect(720, 190, 31, 31));
        QFont font;
        font.setPointSize(13);
        Bps->setFont(font);
        Bps->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(710, 160, 31, 17));
        label_6->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(310, 20, 16, 131));
        label_10->setTextFormat(Qt::PlainText);
        label_10->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1207, 22));
        menuSound_Lab_GUI = new QMenu(menubar);
        menuSound_Lab_GUI->setObjectName(QString::fromUtf8("menuSound_Lab_GUI"));
        menuIO = new QMenu(menubar);
        menuIO->setObjectName(QString::fromUtf8("menuIO"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSound_Lab_GUI->menuAction());
        menubar->addAction(menuIO->menuAction());

        retranslateUi(MainWindow);
        QObject::connect(Slider_Main_Hz, SIGNAL(valueChanged(int)), MainLCD_Hz, SLOT(display(int)));
        QObject::connect(Slider_Main_Vol, SIGNAL(valueChanged(int)), MainLCD_Amp, SLOT(display(int)));
        QObject::connect(Slider_VCO_Hz, SIGNAL(valueChanged(int)), VCOLCD_Hz, SLOT(display(int)));
        QObject::connect(Slider_VCO_Hz, SIGNAL(valueChanged(int)), MainWindow, SLOT(VCO_slot_Hz()));
        QObject::connect(Slider_Main_Hz, SIGNAL(valueChanged(int)), MainWindow, SLOT(MAIN_slot_Hz()));
        QObject::connect(rb_reset, SIGNAL(clicked()), MainWindow, SLOT(set_mode_o()));
        QObject::connect(radioButton_2, SIGNAL(clicked()), MainWindow, SLOT(set_mode_v()));
        QObject::connect(radioButton_3, SIGNAL(clicked()), MainWindow, SLOT(set_mode_f()));
        QObject::connect(Slider_Main_Vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(MAIN_slot_volume()));
        QObject::connect(Slider_VCO_vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(VCO_slot_volume()));
        QObject::connect(Slider_VCO_vol, SIGNAL(valueChanged(int)), lcdNumber_3, SLOT(display(int)));
        QObject::connect(Slider_FMO_vol, SIGNAL(valueChanged(int)), lcdNumber_2, SLOT(display(int)));
        QObject::connect(Slider_FMO_vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(FMO_slot_volume()));
        QObject::connect(pB_Store, SIGNAL(clicked()), MainWindow, SLOT(Store()));
        QObject::connect(pB_Mute_StA, SIGNAL(clicked()), MainWindow, SLOT(Clear_Banks()));
        QObject::connect(cb_1, SIGNAL(clicked(bool)), MainWindow, SLOT(change_status1()));
        QObject::connect(cb_2, SIGNAL(clicked(bool)), MainWindow, SLOT(change_status2()));
        QObject::connect(cb_3, SIGNAL(clicked(bool)), MainWindow, SLOT(change_status3()));
        QObject::connect(Slider_mix_vol3, SIGNAL(valueChanged(int)), MainWindow, SLOT(Sl_mix3()));
        QObject::connect(Slider_mix_vol2, SIGNAL(valueChanged(int)), MainWindow, SLOT(Sl_mix2()));
        QObject::connect(Slider_mix_vol1, SIGNAL(valueChanged(int)), MainWindow, SLOT(Sl_mix1()));
        QObject::connect(pB_Mute, SIGNAL(clicked()), MainWindow, SLOT(toggle_Mute()));
        QObject::connect(radioButton_5, SIGNAL(clicked()), MainWindow, SLOT(connect_vco()));
        QObject::connect(radioButton_4, SIGNAL(clicked()), MainWindow, SLOT(connect_fmo()));
        QObject::connect(pBtoggleRecord, SIGNAL(clicked()), MainWindow, SLOT(toggle_Record()));
        QObject::connect(pB_Save, SIGNAL(clicked()), MainWindow, SLOT(Save_Config()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), MainWindow, SLOT(GUI_Exit()));
        QObject::connect(pBAudioServerExit, SIGNAL(clicked()), MainWindow, SLOT(Audio_Exit()));
        QObject::connect(pBSynthesizer, SIGNAL(clicked()), MainWindow, SLOT(start_synthesizer()));
        QObject::connect(pBAudioServer, SIGNAL(clicked()), MainWindow, SLOT(start_srv()));
        QObject::connect(pBSynthesizerExit, SIGNAL(clicked()), MainWindow, SLOT(Controller_Exit()));
        QObject::connect(cb_4, SIGNAL(clicked()), MainWindow, SLOT(change_status4()));
        QObject::connect(cb_5, SIGNAL(clicked()), MainWindow, SLOT(change_status5()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Sound Lab GUI", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "OSC", nullptr));
        labelVCO->setText(QCoreApplication::translate("MainWindow", "VCO", nullptr));
        labelFMO->setText(QCoreApplication::translate("MainWindow", "FMO", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        rb_reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        rb_bank1->setText(QCoreApplication::translate("MainWindow", "MemoryBank1", nullptr));
        rb_bank2->setText(QCoreApplication::translate("MainWindow", "MemoryBank2", nullptr));
        rb_bank3->setText(QCoreApplication::translate("MainWindow", "MemoryBank3", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        rb_bank7->setText(QCoreApplication::translate("MainWindow", "External", nullptr));
        rb_bank4->setText(QCoreApplication::translate("MainWindow", "MemoryBank4", nullptr));
        rb_bank5->setText(QCoreApplication::translate("MainWindow", "MemoryBank5", nullptr));
        cb_1->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_2->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_3->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_4->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_5->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_6->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        cb_7->setText(QCoreApplication::translate("MainWindow", "stored", nullptr));
        pB_Store->setText(QCoreApplication::translate("MainWindow", "Store", nullptr));
        pB_Mute_StA->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        pBAudioServer->setText(QCoreApplication::translate("MainWindow", "Audio Server", nullptr));
        pBAudioServerExit->setText(QCoreApplication::translate("MainWindow", "EXIT", nullptr));
        pBSynthesizer->setText(QCoreApplication::translate("MainWindow", "Synthesizer", nullptr));
        pBSynthesizerExit->setText(QCoreApplication::translate("MainWindow", "EXIT", nullptr));
        pB_Save->setText(QCoreApplication::translate("MainWindow", "Save Config", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "GUI EXIT", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "PMW", nullptr));
        pB_Mute->setText(QString());
        pB_play_notes->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        radioButton_4->setText(QCoreApplication::translate("MainWindow", "connect FMO", nullptr));
        radioButton_5->setText(QCoreApplication::translate("MainWindow", "connect VCO", nullptr));
        pBtoggleRecord->setText(QCoreApplication::translate("MainWindow", "Record", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Decay", nullptr));
        rb_melody_2->setText(QCoreApplication::translate("MainWindow", "-> MAN freq", nullptr));
        rb_melody_3->setText(QCoreApplication::translate("MainWindow", "-> VCO freq", nullptr));
        rb_melody_1->setText(QCoreApplication::translate("MainWindow", "-> MAIN volume", nullptr));
        rb_melody_4->setText(QCoreApplication::translate("MainWindow", "-> VCO freq", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Attack", nullptr));
        glidefrequency->setText(QCoreApplication::translate("MainWindow", "glide freq.", nullptr));
        pB_Wavedisplay->setText(QString());
        pB_Debug->setText(QCoreApplication::translate("MainWindow", "Details", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Hall", nullptr));
        pB_Specrum->setText(QCoreApplication::translate("MainWindow", "Spectrum", nullptr));
        wf_vco->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        wf_main->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        wf_fmo->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pb_clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Beat id", nullptr));
        Bps->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "  BPS", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "M a s   t e r", nullptr));
        menuSound_Lab_GUI->setTitle(QCoreApplication::translate("MainWindow", "Sound Lab GUI", nullptr));
        menuIO->setTitle(QCoreApplication::translate("MainWindow", "IO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
