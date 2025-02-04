/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
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
#include <QtWidgets/QGridLayout>
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
    QRadioButton *rb_sta1;
    QRadioButton *rb_sta2;
    QRadioButton *rb_sta3;
    QRadioButton *rb_sta7;
    QRadioButton *rb_sta4;
    QRadioButton *rb_sta5;
    QRadioButton *rb_sta0;
    QRadioButton *rb_sta6;
    QFrame *frame_2;
    QSlider *Slider_mix_vol1;
    QSlider *Slider_mix_vol2;
    QSlider *Slider_mix_vol3;
    QSlider *Slider_mix_vol4;
    QSlider *Slider_mix_vol5;
    QSlider *Slider_mix_vol7;
    QSlider *Slider_mix_vol6;
    QSlider *Slider_mix_vol0;
    QLabel *label_12;
    QFrame *frame_4;
    QCheckBox *cb_sta1;
    QCheckBox *cb_sta2;
    QCheckBox *cb_sta3;
    QCheckBox *cb_sta4;
    QCheckBox *cb_sta5;
    QCheckBox *cb_sta6;
    QCheckBox *cb_sta7;
    QCheckBox *cb_sta0;
    QPushButton *pB_Mute_StA;
    QDial *dial_PMW;
    QLabel *label_2;
    QPushButton *pB_play_notes;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QPushButton *pBtoggleRecord;
    QLabel *label_4;
    QFrame *frame_5;
    QRadioButton *rb_S0;
    QRadioButton *rb_S1;
    QRadioButton *rb_S2;
    QRadioButton *rb_S3;
    QProgressBar *progressBar_record;
    QLCDNumber *MainLCD_Hz;
    QLabel *label_11;
    QDial *dial_soft_freq;
    QLabel *glidefrequency;
    QPushButton *pB_Wavedisplay;
    QGraphicsView *oscilloscope_view;
    QPushButton *pB_wd_mode;
    QScrollBar *hs_hall_effect;
    QLabel *label_5;
    QScrollBar *hs_adsr_sustain;
    QScrollBar *hs_adsr_attack;
    QDial *dial_ramp_up_down;
    QPushButton *pB_Specrum;
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
    QLabel *label_10;
    QPushButton *pB_Mute;
    QComboBox *cb_external;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pBComposer;
    QPushButton *pBAudioServer;
    QPushButton *pushButton_3;
    QPushButton *pB_Save;
    QPushButton *pBAudioServerExit;
    QPushButton *pBSynthesizerExit;
    QPushButton *pBSynthesizer;
    QComboBox *cb_bps;
    QProgressBar *Pbar_telapsed;
    QPushButton *pB_Rtsp;
    QPushButton *pB_oscgroup;
    QPushButton *pb_fftmode;
    QLabel *lb_FMO_LFO;
    QLabel *lb_VCO_LFO;
    QPushButton *pb_SDSview;
    QMenuBar *menubar;
    QMenu *menuSound_Lab_GUI;
    QMenu *menuIO;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1207, 629);
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
        Slider_Main_Hz->setMaximum(3520);
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
        Slider_FMO_vol->setMaximum(100);
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
        radioButton_3->setGeometry(QRect(330, 500, 113, 23));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(960, 10, 131, 251));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        rb_sta1 = new QRadioButton(frame);
        rb_sta1->setObjectName(QString::fromUtf8("rb_sta1"));
        rb_sta1->setGeometry(QRect(0, 40, 121, 23));
        rb_sta2 = new QRadioButton(frame);
        rb_sta2->setObjectName(QString::fromUtf8("rb_sta2"));
        rb_sta2->setGeometry(QRect(0, 70, 121, 23));
        rb_sta3 = new QRadioButton(frame);
        rb_sta3->setObjectName(QString::fromUtf8("rb_sta3"));
        rb_sta3->setGeometry(QRect(0, 100, 121, 23));
        rb_sta7 = new QRadioButton(frame);
        rb_sta7->setObjectName(QString::fromUtf8("rb_sta7"));
        rb_sta7->setGeometry(QRect(0, 190, 121, 23));
        rb_sta4 = new QRadioButton(frame);
        rb_sta4->setObjectName(QString::fromUtf8("rb_sta4"));
        rb_sta4->setGeometry(QRect(0, 130, 121, 23));
        rb_sta5 = new QRadioButton(frame);
        rb_sta5->setObjectName(QString::fromUtf8("rb_sta5"));
        rb_sta5->setGeometry(QRect(0, 160, 121, 23));
        rb_sta0 = new QRadioButton(frame);
        rb_sta0->setObjectName(QString::fromUtf8("rb_sta0"));
        rb_sta0->setGeometry(QRect(0, 10, 121, 23));
        rb_sta6 = new QRadioButton(frame);
        rb_sta6->setObjectName(QString::fromUtf8("rb_sta6"));
        rb_sta6->setGeometry(QRect(0, 220, 121, 23));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(760, 10, 191, 251));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        Slider_mix_vol1 = new QSlider(frame_2);
        Slider_mix_vol1->setObjectName(QString::fromUtf8("Slider_mix_vol1"));
        Slider_mix_vol1->setGeometry(QRect(0, 40, 160, 16));
        Slider_mix_vol1->setMaximum(100);
        Slider_mix_vol1->setOrientation(Qt::Horizontal);
        Slider_mix_vol2 = new QSlider(frame_2);
        Slider_mix_vol2->setObjectName(QString::fromUtf8("Slider_mix_vol2"));
        Slider_mix_vol2->setGeometry(QRect(0, 70, 160, 16));
        Slider_mix_vol2->setMaximum(100);
        Slider_mix_vol2->setOrientation(Qt::Horizontal);
        Slider_mix_vol3 = new QSlider(frame_2);
        Slider_mix_vol3->setObjectName(QString::fromUtf8("Slider_mix_vol3"));
        Slider_mix_vol3->setGeometry(QRect(0, 100, 160, 16));
        Slider_mix_vol3->setMaximum(100);
        Slider_mix_vol3->setOrientation(Qt::Horizontal);
        Slider_mix_vol4 = new QSlider(frame_2);
        Slider_mix_vol4->setObjectName(QString::fromUtf8("Slider_mix_vol4"));
        Slider_mix_vol4->setGeometry(QRect(0, 130, 160, 16));
        Slider_mix_vol4->setMaximum(100);
        Slider_mix_vol4->setOrientation(Qt::Horizontal);
        Slider_mix_vol5 = new QSlider(frame_2);
        Slider_mix_vol5->setObjectName(QString::fromUtf8("Slider_mix_vol5"));
        Slider_mix_vol5->setGeometry(QRect(0, 160, 160, 16));
        Slider_mix_vol5->setMaximum(100);
        Slider_mix_vol5->setOrientation(Qt::Horizontal);
        Slider_mix_vol7 = new QSlider(frame_2);
        Slider_mix_vol7->setObjectName(QString::fromUtf8("Slider_mix_vol7"));
        Slider_mix_vol7->setGeometry(QRect(0, 190, 160, 16));
        Slider_mix_vol7->setMaximum(100);
        Slider_mix_vol7->setOrientation(Qt::Horizontal);
        Slider_mix_vol6 = new QSlider(frame_2);
        Slider_mix_vol6->setObjectName(QString::fromUtf8("Slider_mix_vol6"));
        Slider_mix_vol6->setGeometry(QRect(0, 220, 160, 16));
        Slider_mix_vol6->setMaximum(100);
        Slider_mix_vol6->setOrientation(Qt::Horizontal);
        Slider_mix_vol0 = new QSlider(frame_2);
        Slider_mix_vol0->setObjectName(QString::fromUtf8("Slider_mix_vol0"));
        Slider_mix_vol0->setGeometry(QRect(0, 10, 160, 16));
        Slider_mix_vol0->setMaximum(100);
        Slider_mix_vol0->setOrientation(Qt::Horizontal);
        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(0, 0, 161, 18));
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(1100, 10, 91, 251));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        cb_sta1 = new QCheckBox(frame_4);
        cb_sta1->setObjectName(QString::fromUtf8("cb_sta1"));
        cb_sta1->setGeometry(QRect(0, 40, 92, 23));
        cb_sta2 = new QCheckBox(frame_4);
        cb_sta2->setObjectName(QString::fromUtf8("cb_sta2"));
        cb_sta2->setGeometry(QRect(0, 70, 92, 23));
        cb_sta3 = new QCheckBox(frame_4);
        cb_sta3->setObjectName(QString::fromUtf8("cb_sta3"));
        cb_sta3->setGeometry(QRect(0, 100, 92, 23));
        cb_sta4 = new QCheckBox(frame_4);
        cb_sta4->setObjectName(QString::fromUtf8("cb_sta4"));
        cb_sta4->setGeometry(QRect(0, 130, 92, 23));
        cb_sta5 = new QCheckBox(frame_4);
        cb_sta5->setObjectName(QString::fromUtf8("cb_sta5"));
        cb_sta5->setGeometry(QRect(0, 160, 92, 23));
        cb_sta6 = new QCheckBox(frame_4);
        cb_sta6->setObjectName(QString::fromUtf8("cb_sta6"));
        cb_sta6->setGeometry(QRect(0, 220, 92, 23));
        cb_sta7 = new QCheckBox(frame_4);
        cb_sta7->setObjectName(QString::fromUtf8("cb_sta7"));
        cb_sta7->setGeometry(QRect(0, 190, 92, 23));
        cb_sta0 = new QCheckBox(frame_4);
        cb_sta0->setObjectName(QString::fromUtf8("cb_sta0"));
        cb_sta0->setGeometry(QRect(0, 10, 92, 23));
        pB_Mute_StA = new QPushButton(centralwidget);
        pB_Mute_StA->setObjectName(QString::fromUtf8("pB_Mute_StA"));
        pB_Mute_StA->setGeometry(QRect(1100, 260, 89, 25));
        dial_PMW = new QDial(centralwidget);
        dial_PMW->setObjectName(QString::fromUtf8("dial_PMW"));
        dial_PMW->setGeometry(QRect(480, 170, 50, 64));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(490, 160, 81, 17));
        pB_play_notes = new QPushButton(centralwidget);
        pB_play_notes->setObjectName(QString::fromUtf8("pB_play_notes"));
        pB_play_notes->setGeometry(QRect(480, 10, 89, 25));
        radioButton_4 = new QRadioButton(centralwidget);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));
        radioButton_4->setGeometry(QRect(330, 330, 112, 23));
        radioButton_5 = new QRadioButton(centralwidget);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));
        radioButton_5->setGeometry(QRect(330, 520, 113, 23));
        pBtoggleRecord = new QPushButton(centralwidget);
        pBtoggleRecord->setObjectName(QString::fromUtf8("pBtoggleRecord"));
        pBtoggleRecord->setGeometry(QRect(480, 470, 89, 25));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(480, 80, 111, 17));
        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(1000, 320, 91, 121));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        rb_S0 = new QRadioButton(frame_5);
        rb_S0->setObjectName(QString::fromUtf8("rb_S0"));
        rb_S0->setGeometry(QRect(20, 0, 61, 23));
        rb_S1 = new QRadioButton(frame_5);
        rb_S1->setObjectName(QString::fromUtf8("rb_S1"));
        rb_S1->setGeometry(QRect(20, 30, 61, 23));
        rb_S2 = new QRadioButton(frame_5);
        rb_S2->setObjectName(QString::fromUtf8("rb_S2"));
        rb_S2->setGeometry(QRect(20, 60, 61, 23));
        rb_S3 = new QRadioButton(frame_5);
        rb_S3->setObjectName(QString::fromUtf8("rb_S3"));
        rb_S3->setGeometry(QRect(20, 90, 61, 23));
        progressBar_record = new QProgressBar(centralwidget);
        progressBar_record->setObjectName(QString::fromUtf8("progressBar_record"));
        progressBar_record->setGeometry(QRect(580, 470, 411, 23));
        progressBar_record->setValue(24);
        MainLCD_Hz = new QLCDNumber(centralwidget);
        MainLCD_Hz->setObjectName(QString::fromUtf8("MainLCD_Hz"));
        MainLCD_Hz->setGeometry(QRect(60, 10, 158, 49));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(480, 50, 51, 17));
        dial_soft_freq = new QDial(centralwidget);
        dial_soft_freq->setObjectName(QString::fromUtf8("dial_soft_freq"));
        dial_soft_freq->setGeometry(QRect(540, 170, 50, 64));
        dial_soft_freq->setMinimum(0);
        dial_soft_freq->setMaximum(100);
        glidefrequency = new QLabel(centralwidget);
        glidefrequency->setObjectName(QString::fromUtf8("glidefrequency"));
        glidefrequency->setGeometry(QRect(550, 146, 41, 31));
        glidefrequency->setWordWrap(true);
        pB_Wavedisplay = new QPushButton(centralwidget);
        pB_Wavedisplay->setObjectName(QString::fromUtf8("pB_Wavedisplay"));
        pB_Wavedisplay->setGeometry(QRect(480, 260, 81, 25));
        oscilloscope_view = new QGraphicsView(centralwidget);
        oscilloscope_view->setObjectName(QString::fromUtf8("oscilloscope_view"));
        oscilloscope_view->setGeometry(QRect(480, 290, 512, 166));
        oscilloscope_view->setFrameShape(QFrame::Box);
        oscilloscope_view->setLineWidth(2);
        pB_wd_mode = new QPushButton(centralwidget);
        pB_wd_mode->setObjectName(QString::fromUtf8("pB_wd_mode"));
        pB_wd_mode->setGeometry(QRect(660, 260, 88, 25));
        hs_hall_effect = new QScrollBar(centralwidget);
        hs_hall_effect->setObjectName(QString::fromUtf8("hs_hall_effect"));
        hs_hall_effect->setGeometry(QRect(530, 110, 160, 16));
        hs_hall_effect->setMaximum(100);
        hs_hall_effect->setPageStep(1);
        hs_hall_effect->setOrientation(Qt::Horizontal);
        hs_hall_effect->setInvertedControls(false);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(480, 110, 66, 17));
        hs_adsr_sustain = new QScrollBar(centralwidget);
        hs_adsr_sustain->setObjectName(QString::fromUtf8("hs_adsr_sustain"));
        hs_adsr_sustain->setGeometry(QRect(530, 80, 160, 16));
        hs_adsr_sustain->setMaximum(100);
        hs_adsr_sustain->setPageStep(1);
        hs_adsr_sustain->setOrientation(Qt::Horizontal);
        hs_adsr_sustain->setInvertedControls(false);
        hs_adsr_attack = new QScrollBar(centralwidget);
        hs_adsr_attack->setObjectName(QString::fromUtf8("hs_adsr_attack"));
        hs_adsr_attack->setGeometry(QRect(530, 50, 160, 16));
        hs_adsr_attack->setMinimum(2);
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
        pB_Specrum->setGeometry(QRect(610, 10, 88, 25));
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
        sB_VCO->setWrapping(true);
        sB_VCO->setMaximum(9);
        frame_8 = new QFrame(centralwidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(80, 90, 131, 41));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        sB_Main = new QSpinBox(frame_8);
        sB_Main->setObjectName(QString::fromUtf8("sB_Main"));
        sB_Main->setGeometry(QRect(90, 0, 44, 41));
        sB_Main->setWrapping(true);
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
        sB_FMO->setWrapping(true);
        sB_FMO->setMaximum(9);
        pb_clear = new QPushButton(centralwidget);
        pb_clear->setObjectName(QString::fromUtf8("pb_clear"));
        pb_clear->setGeometry(QRect(1000, 260, 88, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(620, 146, 91, 31));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(310, 20, 16, 131));
        label_10->setTextFormat(Qt::PlainText);
        label_10->setWordWrap(true);
        pB_Mute = new QPushButton(centralwidget);
        pB_Mute->setObjectName(QString::fromUtf8("pB_Mute"));
        pB_Mute->setGeometry(QRect(330, 120, 88, 25));
        cb_external = new QComboBox(centralwidget);
        cb_external->setObjectName(QString::fromUtf8("cb_external"));
        cb_external->setGeometry(QRect(480, 500, 131, 25));
        cb_external->setEditable(false);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(630, 500, 462, 60));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pBComposer = new QPushButton(layoutWidget);
        pBComposer->setObjectName(QString::fromUtf8("pBComposer"));

        gridLayout->addWidget(pBComposer, 0, 3, 1, 1);

        pBAudioServer = new QPushButton(layoutWidget);
        pBAudioServer->setObjectName(QString::fromUtf8("pBAudioServer"));

        gridLayout->addWidget(pBAudioServer, 0, 0, 1, 1);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 1, 4, 1, 1);

        pB_Save = new QPushButton(layoutWidget);
        pB_Save->setObjectName(QString::fromUtf8("pB_Save"));

        gridLayout->addWidget(pB_Save, 0, 4, 1, 1);

        pBAudioServerExit = new QPushButton(layoutWidget);
        pBAudioServerExit->setObjectName(QString::fromUtf8("pBAudioServerExit"));

        gridLayout->addWidget(pBAudioServerExit, 1, 0, 1, 1);

        pBSynthesizerExit = new QPushButton(layoutWidget);
        pBSynthesizerExit->setObjectName(QString::fromUtf8("pBSynthesizerExit"));

        gridLayout->addWidget(pBSynthesizerExit, 1, 1, 1, 1);

        pBSynthesizer = new QPushButton(layoutWidget);
        pBSynthesizer->setObjectName(QString::fromUtf8("pBSynthesizer"));

        gridLayout->addWidget(pBSynthesizer, 0, 1, 1, 1);

        cb_bps = new QComboBox(centralwidget);
        cb_bps->setObjectName(QString::fromUtf8("cb_bps"));
        cb_bps->setGeometry(QRect(620, 190, 86, 25));
        Pbar_telapsed = new QProgressBar(centralwidget);
        Pbar_telapsed->setObjectName(QString::fromUtf8("Pbar_telapsed"));
        Pbar_telapsed->setGeometry(QRect(451, 290, 20, 161));
        Pbar_telapsed->setValue(24);
        Pbar_telapsed->setTextVisible(true);
        Pbar_telapsed->setOrientation(Qt::Vertical);
        pB_Rtsp = new QPushButton(centralwidget);
        pB_Rtsp->setObjectName(QString::fromUtf8("pB_Rtsp"));
        pB_Rtsp->setGeometry(QRect(1000, 290, 91, 25));
        pB_oscgroup = new QPushButton(centralwidget);
        pB_oscgroup->setObjectName(QString::fromUtf8("pB_oscgroup"));
        pB_oscgroup->setGeometry(QRect(570, 260, 81, 26));
        pb_fftmode = new QPushButton(centralwidget);
        pb_fftmode->setObjectName(QString::fromUtf8("pb_fftmode"));
        pb_fftmode->setGeometry(QRect(760, 260, 88, 26));
        lb_FMO_LFO = new QLabel(centralwidget);
        lb_FMO_LFO->setObjectName(QString::fromUtf8("lb_FMO_LFO"));
        lb_FMO_LFO->setGeometry(QRect(260, 400, 31, 18));
        lb_VCO_LFO = new QLabel(centralwidget);
        lb_VCO_LFO->setObjectName(QString::fromUtf8("lb_VCO_LFO"));
        lb_VCO_LFO->setGeometry(QRect(260, 210, 31, 18));
        pb_SDSview = new QPushButton(centralwidget);
        pb_SDSview->setObjectName(QString::fromUtf8("pb_SDSview"));
        pb_SDSview->setGeometry(QRect(1000, 450, 88, 26));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1207, 23));
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
        QObject::connect(Slider_Main_Hz, SIGNAL(valueChanged(int)), MainWindow, SLOT(MAIN_slot_Hz()));
        QObject::connect(rb_reset, SIGNAL(clicked()), MainWindow, SLOT(set_mode_o()));
        QObject::connect(radioButton_2, SIGNAL(clicked()), MainWindow, SLOT(set_mode_v()));
        QObject::connect(radioButton_3, SIGNAL(clicked()), MainWindow, SLOT(set_mode_f()));
        QObject::connect(Slider_Main_Vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(MAIN_slot_volume()));
        QObject::connect(Slider_VCO_vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(VCO_slot_volume()));
        QObject::connect(Slider_VCO_vol, SIGNAL(valueChanged(int)), lcdNumber_3, SLOT(display(int)));
        QObject::connect(Slider_FMO_vol, SIGNAL(valueChanged(int)), lcdNumber_2, SLOT(display(int)));
        QObject::connect(Slider_FMO_vol, SIGNAL(valueChanged(int)), MainWindow, SLOT(FMO_slot_volume()));
        QObject::connect(pB_Mute_StA, SIGNAL(clicked()), MainWindow, SLOT(Clear_Banks()));
        QObject::connect(radioButton_5, SIGNAL(clicked()), MainWindow, SLOT(set_mode_o()));
        QObject::connect(radioButton_4, SIGNAL(clicked()), MainWindow, SLOT(connect_fmo()));
        QObject::connect(pB_Save, SIGNAL(clicked()), MainWindow, SLOT(Save_Config()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), MainWindow, SLOT(GUI_Exit()));
        QObject::connect(pBSynthesizer, SIGNAL(clicked()), MainWindow, SLOT(start_synthesizer()));
        QObject::connect(pBAudioServerExit, SIGNAL(clicked()), MainWindow, SLOT(Audio_Exit()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Ocean", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "OSC", nullptr));
        labelVCO->setText(QCoreApplication::translate("MainWindow", "VCO", nullptr));
        labelFMO->setText(QCoreApplication::translate("MainWindow", "FMO", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        rb_reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        rb_sta1->setText(QCoreApplication::translate("MainWindow", "Storage Area 1", nullptr));
        rb_sta2->setText(QCoreApplication::translate("MainWindow", "Storage Area 2", nullptr));
        rb_sta3->setText(QCoreApplication::translate("MainWindow", "Storage Area 3", nullptr));
        rb_sta7->setText(QCoreApplication::translate("MainWindow", "External", nullptr));
        rb_sta4->setText(QCoreApplication::translate("MainWindow", "Instrument", nullptr));
        rb_sta5->setText(QCoreApplication::translate("MainWindow", "Keyboard", nullptr));
        rb_sta0->setText(QCoreApplication::translate("MainWindow", "Storage Area 0", nullptr));
        rb_sta6->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "0                                     100", nullptr));
        cb_sta1->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta2->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta3->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta4->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta5->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta6->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta7->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        cb_sta0->setText(QCoreApplication::translate("MainWindow", "play", nullptr));
        pB_Mute_StA->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "PMW", nullptr));
        pB_play_notes->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        radioButton_4->setText(QCoreApplication::translate("MainWindow", "connect FMO", nullptr));
        radioButton_5->setText(QCoreApplication::translate("MainWindow", "connect VCO", nullptr));
        pBtoggleRecord->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Decay", nullptr));
        rb_S0->setText(QCoreApplication::translate("MainWindow", "S0", nullptr));
        rb_S1->setText(QCoreApplication::translate("MainWindow", "S1", nullptr));
        rb_S2->setText(QCoreApplication::translate("MainWindow", "S2", nullptr));
        rb_S3->setText(QCoreApplication::translate("MainWindow", "S3", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Attack", nullptr));
        glidefrequency->setText(QCoreApplication::translate("MainWindow", "glide freq.", nullptr));
        pB_Wavedisplay->setText(QString());
        pB_wd_mode->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "Hall", nullptr));
        pB_Specrum->setText(QCoreApplication::translate("MainWindow", "Spectrum", nullptr));
        wf_vco->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        wf_main->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        wf_fmo->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pb_clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Beat per sec", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "M a s   t e r", nullptr));
        pB_Mute->setText(QString());
        pBComposer->setText(QCoreApplication::translate("MainWindow", "Composer", nullptr));
        pBAudioServer->setText(QCoreApplication::translate("MainWindow", "Audio Server", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "GUI EXIT", nullptr));
        pB_Save->setText(QCoreApplication::translate("MainWindow", "Save Config", nullptr));
        pBAudioServerExit->setText(QCoreApplication::translate("MainWindow", "EXIT", nullptr));
        pBSynthesizerExit->setText(QCoreApplication::translate("MainWindow", "EXIT", nullptr));
        pBSynthesizer->setText(QCoreApplication::translate("MainWindow", "Synthesizer", nullptr));
        pB_Rtsp->setText(QCoreApplication::translate("MainWindow", "RTSP", nullptr));
        pB_oscgroup->setText(QString());
        pb_fftmode->setText(QString());
        lb_FMO_LFO->setText(QCoreApplication::translate("MainWindow", "LFO", nullptr));
        lb_VCO_LFO->setText(QCoreApplication::translate("MainWindow", "LFO", nullptr));
        pb_SDSview->setText(QCoreApplication::translate("MainWindow", "view SDS", nullptr));
        menuSound_Lab_GUI->setTitle(QCoreApplication::translate("MainWindow", "Sound Lab GUI", nullptr));
        menuIO->setTitle(QCoreApplication::translate("MainWindow", "IO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
