/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLCDNumber *OSCLCD_Amp;
    QSlider *Slider_VCO_Hz;
    QSlider *Slider_VCO_vol;
    QLCDNumber *VCOLCD_Hz;
    QLCDNumber *FMOLCD_Amp;
    QSlider *Slider_FMO_Hz;
    QSlider *Slider_FMO_vol;
    QLCDNumber *VCOLCD_Amp;
    QLCDNumber *FMOLCD_Hz;
    QLabel *labelVCO;
    QLabel *labelFMO;
    QLabel *label_7;
    QLabel *label_8;
    QFrame *frame;
    QCheckBox *cb_ssta1;
    QCheckBox *cb_ssta2;
    QCheckBox *cb_ssta3;
    QCheckBox *cb_ssta7;
    QCheckBox *cb_ssta4;
    QCheckBox *cb_ssta5;
    QCheckBox *cb_ssta0;
    QCheckBox *cb_ssta6;
    QFrame *frame_2;
    QSlider *Slider_mix_vol1;
    QSlider *Slider_mix_vol2;
    QSlider *Slider_mix_vol3;
    QSlider *Slider_mix_vol4;
    QSlider *Slider_mix_vol5;
    QSlider *Slider_mix_vol7;
    QSlider *Slider_mix_vol6;
    QSlider *Slider_mix_vol0;
    QPushButton *pB_Mute_StA;
    QLabel *label_2;
    QPushButton *pB_play_notes;
    QPushButton *pBtoggleRecord;
    QFrame *frame_5;
    QRadioButton *rb_S0;
    QRadioButton *rb_S1;
    QRadioButton *rb_S2;
    QRadioButton *rb_S3;
    QProgressBar *progressBar_record;
    QPushButton *pB_WdRole;
    QGraphicsView *oscilloscope_view;
    QPushButton *pB_wd_mode;
    QLabel *label_5;
    QPushButton *pB_Spectrum;
    QPushButton *pb_clear;
    QLabel *label_3;
    QPushButton *pB_Mute;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pBGuiExit;
    QPushButton *pb_Keyboard;
    QPushButton *pBAudioServer;
    QPushButton *pBComposer;
    QPushButton *pB_Save;
    QPushButton *pB_Rtsp;
    QPushButton *pB_Cutter;
    QComboBox *cB_external;
    QPushButton *pB_Capture;
    QPushButton *pb_SDSview;
    QCheckBox *cb_overwrite;
    QToolButton *tB_logState;
    QComboBox *CB_kbd_bps;
    QProgressBar *Pbar_telapsed;
    QPushButton *pB_oscgroup;
    QLabel *lb_FMO_LFO;
    QLabel *lb_VCO_LFO;
    QSlider *SliderVCOadjust;
    QSlider *SliderFMOadjust;
    QLabel *label_6;
    QLabel *label_13;
    QScrollBar *hs_hall_effect;
    QScrollBar *hs_balance;
    QLabel *label_14;
    QCheckBox *cb_connect_vcov;
    QCheckBox *cb_connect_fmov;
    QCheckBox *cb_connect_oscv;
    QCheckBox *cb_connect_oscf;
    QLabel *label_15;
    QLabel *label_16;
    QPushButton *pBSynthesizer;
    QLabel *label_12;
    QFrame *frame_3;
    QCheckBox *cb_fsta0;
    QCheckBox *cb_fsta1;
    QCheckBox *cb_fsta2;
    QCheckBox *cb_fsta3;
    QCheckBox *cb_fsta4;
    QCheckBox *cb_fsta5;
    QCheckBox *cb_fsta7;
    QCheckBox *cb_fsta6;
    QLabel *label_17;
    QScrollBar *hs_chord_delay;
    QScrollBar *hs_pmw;
    QPushButton *pB_ADSR;
    QLabel *label_4;
    QComboBox *CB_inst_bps;
    QLabel *label_11;
    QLabel *label_18;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QSlider *Slider_OSC_Vol;
    QSpacerItem *horizontalSpacer;
    QSlider *Slider_slideFrq;
    QGridLayout *gridLayout_2;
    QPushButton *pB_wfOSC;
    QLCDNumber *OSCLCD_Hz;
    QLabel *lbl_frqglide_sec;
    QLabel *label_19;
    QSlider *Slider_slideVol;
    QSlider *Slider_OSC_Hz;
    QLabel *glidefrequency;
    QCheckBox *cB_Combine_vco;
    QCheckBox *cB_Combine_fmo;
    QPushButton *pB_wfVCO;
    QPushButton *pB_wfFMO;
    QToolButton *tB_play_0;
    QToolButton *tB_play_1;
    QToolButton *tB_play_2;
    QToolButton *tB_play_3;
    QToolButton *tB_play_4;
    QToolButton *tB_play_5;
    QToolButton *tB_play_6;
    QToolButton *tB_play_7;
    QStatusBar *statusbar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(1221, 650);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(153, 193, 241, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush3(QColor(204, 224, 248, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush3);
        QBrush brush4(QColor(76, 96, 120, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush4);
        QBrush brush5(QColor(102, 129, 161, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush7(QColor(0, 0, 0, 127));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush8(QColor(76, 96, 120, 127));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush8);
#endif
        QBrush brush9(QColor(220, 236, 255, 255));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush9);
#endif
        MainWindow->setPalette(palette);
        MainWindow->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        MainWindow->setAcceptDrops(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        OSCLCD_Amp = new QLCDNumber(centralwidget);
        OSCLCD_Amp->setObjectName("OSCLCD_Amp");
        OSCLCD_Amp->setGeometry(QRect(330, 10, 111, 51));
        Slider_VCO_Hz = new QSlider(centralwidget);
        Slider_VCO_Hz->setObjectName("Slider_VCO_Hz");
        Slider_VCO_Hz->setGeometry(QRect(210, 190, 16, 160));
        Slider_VCO_Hz->setMinimum(1);
        Slider_VCO_Hz->setMaximum(72);
        Slider_VCO_Hz->setSingleStep(1);
        Slider_VCO_Hz->setPageStep(12);
        Slider_VCO_Hz->setOrientation(Qt::Orientation::Vertical);
        Slider_VCO_vol = new QSlider(centralwidget);
        Slider_VCO_vol->setObjectName("Slider_VCO_vol");
        Slider_VCO_vol->setGeometry(QRect(290, 190, 16, 160));
        Slider_VCO_vol->setMaximum(100);
        Slider_VCO_vol->setOrientation(Qt::Orientation::Vertical);
        VCOLCD_Hz = new QLCDNumber(centralwidget);
        VCOLCD_Hz->setObjectName("VCOLCD_Hz");
        VCOLCD_Hz->setGeometry(QRect(60, 190, 131, 51));
        VCOLCD_Hz->setDigitCount(5);
        FMOLCD_Amp = new QLCDNumber(centralwidget);
        FMOLCD_Amp->setObjectName("FMOLCD_Amp");
        FMOLCD_Amp->setGeometry(QRect(330, 380, 111, 51));
        Slider_FMO_Hz = new QSlider(centralwidget);
        Slider_FMO_Hz->setObjectName("Slider_FMO_Hz");
        Slider_FMO_Hz->setGeometry(QRect(210, 380, 16, 160));
        Slider_FMO_Hz->setMinimum(1);
        Slider_FMO_Hz->setMaximum(72);
        Slider_FMO_Hz->setSingleStep(1);
        Slider_FMO_Hz->setPageStep(12);
        Slider_FMO_Hz->setOrientation(Qt::Orientation::Vertical);
        Slider_FMO_vol = new QSlider(centralwidget);
        Slider_FMO_vol->setObjectName("Slider_FMO_vol");
        Slider_FMO_vol->setGeometry(QRect(290, 380, 16, 160));
        Slider_FMO_vol->setMaximum(100);
        Slider_FMO_vol->setOrientation(Qt::Orientation::Vertical);
        VCOLCD_Amp = new QLCDNumber(centralwidget);
        VCOLCD_Amp->setObjectName("VCOLCD_Amp");
        VCOLCD_Amp->setGeometry(QRect(330, 190, 111, 51));
        FMOLCD_Hz = new QLCDNumber(centralwidget);
        FMOLCD_Hz->setObjectName("FMOLCD_Hz");
        FMOLCD_Hz->setGeometry(QRect(60, 380, 131, 51));
        FMOLCD_Hz->setDigitCount(5);
        labelVCO = new QLabel(centralwidget);
        labelVCO->setObjectName("labelVCO");
        labelVCO->setGeometry(QRect(240, 190, 31, 17));
        labelFMO = new QLabel(centralwidget);
        labelFMO->setObjectName("labelFMO");
        labelFMO->setGeometry(QRect(240, 380, 31, 17));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(60, 430, 111, 20));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(60, 240, 111, 20));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(960, 40, 90, 251));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        cb_ssta1 = new QCheckBox(frame);
        cb_ssta1->setObjectName("cb_ssta1");
        cb_ssta1->setGeometry(QRect(0, 40, 80, 23));
        cb_ssta2 = new QCheckBox(frame);
        cb_ssta2->setObjectName("cb_ssta2");
        cb_ssta2->setGeometry(QRect(0, 70, 80, 23));
        cb_ssta3 = new QCheckBox(frame);
        cb_ssta3->setObjectName("cb_ssta3");
        cb_ssta3->setGeometry(QRect(0, 100, 80, 23));
        cb_ssta7 = new QCheckBox(frame);
        cb_ssta7->setObjectName("cb_ssta7");
        cb_ssta7->setGeometry(QRect(0, 220, 80, 23));
        cb_ssta4 = new QCheckBox(frame);
        cb_ssta4->setObjectName("cb_ssta4");
        cb_ssta4->setEnabled(true);
        cb_ssta4->setGeometry(QRect(0, 130, 80, 23));
        QFont font;
        font.setFamilies({QString::fromUtf8("Ubuntu Sans")});
        font.setItalic(false);
        cb_ssta4->setFont(font);
        cb_ssta5 = new QCheckBox(frame);
        cb_ssta5->setObjectName("cb_ssta5");
        cb_ssta5->setEnabled(true);
        cb_ssta5->setGeometry(QRect(0, 160, 80, 23));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Ubuntu Mono")});
        font1.setItalic(true);
        cb_ssta5->setFont(font1);
        cb_ssta0 = new QCheckBox(frame);
        cb_ssta0->setObjectName("cb_ssta0");
        cb_ssta0->setGeometry(QRect(0, 10, 80, 23));
        cb_ssta6 = new QCheckBox(frame);
        cb_ssta6->setObjectName("cb_ssta6");
        cb_ssta6->setEnabled(true);
        cb_ssta6->setGeometry(QRect(0, 190, 80, 23));
        cb_ssta6->setFont(font1);
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(760, 40, 191, 251));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        Slider_mix_vol1 = new QSlider(frame_2);
        Slider_mix_vol1->setObjectName("Slider_mix_vol1");
        Slider_mix_vol1->setGeometry(QRect(0, 40, 160, 16));
        Slider_mix_vol1->setMaximum(100);
        Slider_mix_vol1->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol2 = new QSlider(frame_2);
        Slider_mix_vol2->setObjectName("Slider_mix_vol2");
        Slider_mix_vol2->setGeometry(QRect(0, 70, 160, 16));
        Slider_mix_vol2->setMaximum(100);
        Slider_mix_vol2->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol3 = new QSlider(frame_2);
        Slider_mix_vol3->setObjectName("Slider_mix_vol3");
        Slider_mix_vol3->setGeometry(QRect(0, 100, 160, 16));
        Slider_mix_vol3->setMaximum(100);
        Slider_mix_vol3->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol4 = new QSlider(frame_2);
        Slider_mix_vol4->setObjectName("Slider_mix_vol4");
        Slider_mix_vol4->setGeometry(QRect(0, 130, 160, 16));
        Slider_mix_vol4->setMaximum(100);
        Slider_mix_vol4->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol5 = new QSlider(frame_2);
        Slider_mix_vol5->setObjectName("Slider_mix_vol5");
        Slider_mix_vol5->setGeometry(QRect(0, 160, 160, 16));
        Slider_mix_vol5->setMaximum(100);
        Slider_mix_vol5->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol7 = new QSlider(frame_2);
        Slider_mix_vol7->setObjectName("Slider_mix_vol7");
        Slider_mix_vol7->setGeometry(QRect(0, 220, 160, 16));
        Slider_mix_vol7->setMaximum(100);
        Slider_mix_vol7->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol6 = new QSlider(frame_2);
        Slider_mix_vol6->setObjectName("Slider_mix_vol6");
        Slider_mix_vol6->setGeometry(QRect(0, 190, 160, 16));
        Slider_mix_vol6->setMaximum(100);
        Slider_mix_vol6->setOrientation(Qt::Orientation::Horizontal);
        Slider_mix_vol0 = new QSlider(frame_2);
        Slider_mix_vol0->setObjectName("Slider_mix_vol0");
        Slider_mix_vol0->setGeometry(QRect(0, 10, 160, 16));
        Slider_mix_vol0->setMaximum(100);
        Slider_mix_vol0->setOrientation(Qt::Orientation::Horizontal);
        pB_Mute_StA = new QPushButton(centralwidget);
        pB_Mute_StA->setObjectName("pB_Mute_StA");
        pB_Mute_StA->setGeometry(QRect(1120, 290, 75, 25));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(480, 110, 50, 17));
        pB_play_notes = new QPushButton(centralwidget);
        pB_play_notes->setObjectName("pB_play_notes");
        pB_play_notes->setGeometry(QRect(480, 10, 89, 41));
        pBtoggleRecord = new QPushButton(centralwidget);
        pBtoggleRecord->setObjectName("pBtoggleRecord");
        pBtoggleRecord->setGeometry(QRect(470, 470, 89, 25));
        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(1000, 370, 151, 121));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        rb_S0 = new QRadioButton(frame_5);
        rb_S0->setObjectName("rb_S0");
        rb_S0->setGeometry(QRect(20, 0, 111, 23));
        rb_S1 = new QRadioButton(frame_5);
        rb_S1->setObjectName("rb_S1");
        rb_S1->setGeometry(QRect(20, 30, 111, 23));
        rb_S2 = new QRadioButton(frame_5);
        rb_S2->setObjectName("rb_S2");
        rb_S2->setGeometry(QRect(20, 60, 111, 23));
        rb_S3 = new QRadioButton(frame_5);
        rb_S3->setObjectName("rb_S3");
        rb_S3->setGeometry(QRect(20, 90, 121, 23));
        progressBar_record = new QProgressBar(centralwidget);
        progressBar_record->setObjectName("progressBar_record");
        progressBar_record->setGeometry(QRect(580, 470, 411, 23));
        progressBar_record->setValue(0);
        pB_WdRole = new QPushButton(centralwidget);
        pB_WdRole->setObjectName("pB_WdRole");
        pB_WdRole->setGeometry(QRect(480, 260, 81, 25));
        oscilloscope_view = new QGraphicsView(centralwidget);
        oscilloscope_view->setObjectName("oscilloscope_view");
        oscilloscope_view->setGeometry(QRect(480, 290, 512, 166));
        oscilloscope_view->setFrameShape(QFrame::Shape::Box);
        oscilloscope_view->setLineWidth(2);
        pB_wd_mode = new QPushButton(centralwidget);
        pB_wd_mode->setObjectName("pB_wd_mode");
        pB_wd_mode->setGeometry(QRect(570, 260, 81, 25));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(480, 50, 50, 17));
        pB_Spectrum = new QPushButton(centralwidget);
        pB_Spectrum->setObjectName("pB_Spectrum");
        pB_Spectrum->setGeometry(QRect(570, 10, 88, 25));
        pb_clear = new QPushButton(centralwidget);
        pb_clear->setObjectName("pb_clear");
        pb_clear->setGeometry(QRect(1040, 290, 75, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(610, 170, 91, 31));
        label_3->setWordWrap(true);
        pB_Mute = new QPushButton(centralwidget);
        pB_Mute->setObjectName("pB_Mute");
        pB_Mute->setGeometry(QRect(330, 80, 88, 25));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(470, 500, 661, 71));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        gridLayout->setHorizontalSpacing(15);
        gridLayout->setVerticalSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pBGuiExit = new QPushButton(layoutWidget);
        pBGuiExit->setObjectName("pBGuiExit");

        gridLayout->addWidget(pBGuiExit, 1, 4, 1, 1);

        pb_Keyboard = new QPushButton(layoutWidget);
        pb_Keyboard->setObjectName("pb_Keyboard");

        gridLayout->addWidget(pb_Keyboard, 0, 5, 1, 1);

        pBAudioServer = new QPushButton(layoutWidget);
        pBAudioServer->setObjectName("pBAudioServer");

        gridLayout->addWidget(pBAudioServer, 1, 5, 1, 1);

        pBComposer = new QPushButton(layoutWidget);
        pBComposer->setObjectName("pBComposer");

        gridLayout->addWidget(pBComposer, 0, 3, 1, 1);

        pB_Save = new QPushButton(layoutWidget);
        pB_Save->setObjectName("pB_Save");

        gridLayout->addWidget(pB_Save, 0, 4, 1, 1);

        pB_Rtsp = new QPushButton(layoutWidget);
        pB_Rtsp->setObjectName("pB_Rtsp");

        gridLayout->addWidget(pB_Rtsp, 0, 1, 1, 1);

        pB_Cutter = new QPushButton(layoutWidget);
        pB_Cutter->setObjectName("pB_Cutter");

        gridLayout->addWidget(pB_Cutter, 0, 2, 1, 1);

        cB_external = new QComboBox(layoutWidget);
        cB_external->setObjectName("cB_external");
        cB_external->setEditable(false);

        gridLayout->addWidget(cB_external, 1, 0, 1, 1);

        pB_Capture = new QPushButton(layoutWidget);
        pB_Capture->setObjectName("pB_Capture");

        gridLayout->addWidget(pB_Capture, 0, 0, 1, 1);

        pb_SDSview = new QPushButton(layoutWidget);
        pb_SDSview->setObjectName("pb_SDSview");

        gridLayout->addWidget(pb_SDSview, 1, 2, 1, 1);

        cb_overwrite = new QCheckBox(layoutWidget);
        cb_overwrite->setObjectName("cb_overwrite");

        gridLayout->addWidget(cb_overwrite, 1, 1, 1, 1);

        tB_logState = new QToolButton(layoutWidget);
        tB_logState->setObjectName("tB_logState");

        gridLayout->addWidget(tB_logState, 1, 3, 1, 1);

        CB_kbd_bps = new QComboBox(centralwidget);
        CB_kbd_bps->setObjectName("CB_kbd_bps");
        CB_kbd_bps->setGeometry(QRect(700, 200, 41, 25));
        Pbar_telapsed = new QProgressBar(centralwidget);
        Pbar_telapsed->setObjectName("Pbar_telapsed");
        Pbar_telapsed->setGeometry(QRect(451, 290, 20, 161));
        Pbar_telapsed->setValue(24);
        Pbar_telapsed->setTextVisible(true);
        Pbar_telapsed->setOrientation(Qt::Orientation::Vertical);
        pB_oscgroup = new QPushButton(centralwidget);
        pB_oscgroup->setObjectName("pB_oscgroup");
        pB_oscgroup->setGeometry(QRect(660, 260, 81, 26));
        lb_FMO_LFO = new QLabel(centralwidget);
        lb_FMO_LFO->setObjectName("lb_FMO_LFO");
        lb_FMO_LFO->setGeometry(QRect(220, 400, 31, 18));
        lb_VCO_LFO = new QLabel(centralwidget);
        lb_VCO_LFO->setObjectName("lb_VCO_LFO");
        lb_VCO_LFO->setGeometry(QRect(220, 210, 31, 18));
        SliderVCOadjust = new QSlider(centralwidget);
        SliderVCOadjust->setObjectName("SliderVCOadjust");
        SliderVCOadjust->setGeometry(QRect(40, 190, 16, 160));
        SliderVCOadjust->setMaximum(50);
        SliderVCOadjust->setOrientation(Qt::Orientation::Vertical);
        SliderFMOadjust = new QSlider(centralwidget);
        SliderFMOadjust->setObjectName("SliderFMOadjust");
        SliderFMOadjust->setGeometry(QRect(40, 380, 16, 160));
        SliderFMOadjust->setMaximum(50);
        SliderFMOadjust->setOrientation(Qt::Orientation::Vertical);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 540, 66, 18));
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(30, 350, 66, 18));
        hs_hall_effect = new QScrollBar(centralwidget);
        hs_hall_effect->setObjectName("hs_hall_effect");
        hs_hall_effect->setGeometry(QRect(580, 50, 160, 16));
        hs_hall_effect->setMaximum(100);
        hs_hall_effect->setPageStep(5);
        hs_hall_effect->setOrientation(Qt::Orientation::Horizontal);
        hs_hall_effect->setInvertedControls(false);
        hs_balance = new QScrollBar(centralwidget);
        hs_balance->setObjectName("hs_balance");
        hs_balance->setGeometry(QRect(580, 80, 160, 16));
        hs_balance->setMinimum(-100);
        hs_balance->setMaximum(100);
        hs_balance->setSingleStep(5);
        hs_balance->setPageStep(5);
        hs_balance->setOrientation(Qt::Orientation::Horizontal);
        hs_balance->setInvertedControls(false);
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(480, 80, 50, 17));
        cb_connect_vcov = new QCheckBox(centralwidget);
        cb_connect_vcov->setObjectName("cb_connect_vcov");
        cb_connect_vcov->setEnabled(true);
        cb_connect_vcov->setGeometry(QRect(330, 340, 111, 31));
        cb_connect_vcov->setCheckable(true);
        cb_connect_fmov = new QCheckBox(centralwidget);
        cb_connect_fmov->setObjectName("cb_connect_fmov");
        cb_connect_fmov->setGeometry(QRect(330, 303, 111, 31));
        cb_connect_oscv = new QCheckBox(centralwidget);
        cb_connect_oscv->setObjectName("cb_connect_oscv");
        cb_connect_oscv->setGeometry(QRect(330, 150, 111, 24));
        cb_connect_oscf = new QCheckBox(centralwidget);
        cb_connect_oscf->setObjectName("cb_connect_oscf");
        cb_connect_oscf->setGeometry(QRect(330, 120, 111, 24));
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(330, 240, 111, 20));
        label_16 = new QLabel(centralwidget);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(330, 430, 111, 20));
        pBSynthesizer = new QPushButton(centralwidget);
        pBSynthesizer->setObjectName("pBSynthesizer");
        pBSynthesizer->setGeometry(QRect(1010, 330, 121, 26));
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(765, 10, 421, 20));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(1070, 40, 40, 251));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        cb_fsta0 = new QCheckBox(frame_3);
        cb_fsta0->setObjectName("cb_fsta0");
        cb_fsta0->setGeometry(QRect(10, 10, 40, 24));
        cb_fsta0->setCheckable(true);
        cb_fsta1 = new QCheckBox(frame_3);
        cb_fsta1->setObjectName("cb_fsta1");
        cb_fsta1->setGeometry(QRect(10, 40, 40, 24));
        cb_fsta1->setCheckable(true);
        cb_fsta2 = new QCheckBox(frame_3);
        cb_fsta2->setObjectName("cb_fsta2");
        cb_fsta2->setGeometry(QRect(10, 70, 40, 24));
        cb_fsta2->setCheckable(true);
        cb_fsta3 = new QCheckBox(frame_3);
        cb_fsta3->setObjectName("cb_fsta3");
        cb_fsta3->setGeometry(QRect(10, 100, 40, 24));
        cb_fsta3->setCheckable(true);
        cb_fsta4 = new QCheckBox(frame_3);
        cb_fsta4->setObjectName("cb_fsta4");
        cb_fsta4->setGeometry(QRect(10, 130, 40, 24));
        cb_fsta4->setCheckable(true);
        cb_fsta5 = new QCheckBox(frame_3);
        cb_fsta5->setObjectName("cb_fsta5");
        cb_fsta5->setGeometry(QRect(10, 160, 40, 24));
        cb_fsta5->setCheckable(true);
        cb_fsta7 = new QCheckBox(frame_3);
        cb_fsta7->setObjectName("cb_fsta7");
        cb_fsta7->setGeometry(QRect(10, 220, 40, 24));
        cb_fsta7->setCheckable(true);
        cb_fsta6 = new QCheckBox(frame_3);
        cb_fsta6->setObjectName("cb_fsta6");
        cb_fsta6->setGeometry(QRect(10, 190, 40, 24));
        cb_fsta6->setCheckable(true);
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(480, 140, 50, 17));
        hs_chord_delay = new QScrollBar(centralwidget);
        hs_chord_delay->setObjectName("hs_chord_delay");
        hs_chord_delay->setGeometry(QRect(580, 140, 160, 16));
        hs_chord_delay->setMinimum(0);
        hs_chord_delay->setMaximum(200);
        hs_chord_delay->setSingleStep(10);
        hs_chord_delay->setPageStep(10);
        hs_chord_delay->setOrientation(Qt::Orientation::Horizontal);
        hs_chord_delay->setInvertedControls(false);
        hs_pmw = new QScrollBar(centralwidget);
        hs_pmw->setObjectName("hs_pmw");
        hs_pmw->setGeometry(QRect(580, 110, 160, 16));
        hs_pmw->setMinimum(0);
        hs_pmw->setMaximum(100);
        hs_pmw->setPageStep(5);
        hs_pmw->setOrientation(Qt::Orientation::Horizontal);
        hs_pmw->setInvertedControls(false);
        pB_ADSR = new QPushButton(centralwidget);
        pB_ADSR->setObjectName("pB_ADSR");
        pB_ADSR->setGeometry(QRect(660, 10, 88, 25));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(610, 200, 91, 31));
        label_4->setWordWrap(true);
        CB_inst_bps = new QComboBox(centralwidget);
        CB_inst_bps->setObjectName("CB_inst_bps");
        CB_inst_bps->setGeometry(QRect(700, 170, 41, 25));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(330, 280, 151, 18));
        label_18 = new QLabel(centralwidget);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(410, 540, 66, 18));
        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(20, 10, 281, 151));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        gridLayout_3->setContentsMargins(0, 10, 0, 10);
        label_10 = new QLabel(gridLayoutWidget_2);
        label_10->setObjectName("label_10");
        label_10->setTextFormat(Qt::TextFormat::PlainText);
        label_10->setWordWrap(true);

        gridLayout_3->addWidget(label_10, 0, 8, 1, 1);

        Slider_OSC_Vol = new QSlider(gridLayoutWidget_2);
        Slider_OSC_Vol->setObjectName("Slider_OSC_Vol");
        Slider_OSC_Vol->setMaximum(100);
        Slider_OSC_Vol->setOrientation(Qt::Orientation::Vertical);

        gridLayout_3->addWidget(Slider_OSC_Vol, 0, 9, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 4, 1, 1);

        Slider_slideFrq = new QSlider(gridLayoutWidget_2);
        Slider_slideFrq->setObjectName("Slider_slideFrq");
        Slider_slideFrq->setMinimum(0);
        Slider_slideFrq->setMaximum(100);
        Slider_slideFrq->setSingleStep(4);
        Slider_slideFrq->setPageStep(4);
        Slider_slideFrq->setValue(0);
        Slider_slideFrq->setOrientation(Qt::Orientation::Vertical);

        gridLayout_3->addWidget(Slider_slideFrq, 0, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        pB_wfOSC = new QPushButton(gridLayoutWidget_2);
        pB_wfOSC->setObjectName("pB_wfOSC");
        pB_wfOSC->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pB_wfOSC->sizePolicy().hasHeightForWidth());
        pB_wfOSC->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setBold(true);
        pB_wfOSC->setFont(font2);

        gridLayout_2->addWidget(pB_wfOSC, 3, 1, 1, 1);

        OSCLCD_Hz = new QLCDNumber(gridLayoutWidget_2);
        OSCLCD_Hz->setObjectName("OSCLCD_Hz");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(OSCLCD_Hz->sizePolicy().hasHeightForWidth());
        OSCLCD_Hz->setSizePolicy(sizePolicy1);
        OSCLCD_Hz->setDigitCount(5);
        OSCLCD_Hz->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(OSCLCD_Hz, 0, 1, 1, 1);

        lbl_frqglide_sec = new QLabel(gridLayoutWidget_2);
        lbl_frqglide_sec->setObjectName("lbl_frqglide_sec");

        gridLayout_2->addWidget(lbl_frqglide_sec, 2, 0, 1, 2);

        label_19 = new QLabel(gridLayoutWidget_2);
        label_19->setObjectName("label_19");

        gridLayout_2->addWidget(label_19, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 2, 1, 1);

        Slider_slideVol = new QSlider(gridLayoutWidget_2);
        Slider_slideVol->setObjectName("Slider_slideVol");
        Slider_slideVol->setMaximum(100);
        Slider_slideVol->setSingleStep(10);
        Slider_slideVol->setOrientation(Qt::Orientation::Vertical);
        Slider_slideVol->setTickPosition(QSlider::TickPosition::TicksBothSides);

        gridLayout_3->addWidget(Slider_slideVol, 0, 7, 1, 1);

        Slider_OSC_Hz = new QSlider(gridLayoutWidget_2);
        Slider_OSC_Hz->setObjectName("Slider_OSC_Hz");
        Slider_OSC_Hz->setMinimum(1);
        Slider_OSC_Hz->setMaximum(72);
        Slider_OSC_Hz->setPageStep(12);
        Slider_OSC_Hz->setOrientation(Qt::Orientation::Vertical);
        Slider_OSC_Hz->setTickPosition(QSlider::TickPosition::TicksAbove);
        Slider_OSC_Hz->setTickInterval(12);

        gridLayout_3->addWidget(Slider_OSC_Hz, 0, 3, 1, 1);

        glidefrequency = new QLabel(centralwidget);
        glidefrequency->setObjectName("glidefrequency");
        glidefrequency->setGeometry(QRect(20, 150, 39, 36));
        glidefrequency->setWordWrap(true);
        cB_Combine_vco = new QCheckBox(centralwidget);
        cB_Combine_vco->setObjectName("cB_Combine_vco");
        cB_Combine_vco->setGeometry(QRect(110, 330, 85, 23));
        cB_Combine_vco->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        cB_Combine_fmo = new QCheckBox(centralwidget);
        cB_Combine_fmo->setObjectName("cB_Combine_fmo");
        cB_Combine_fmo->setGeometry(QRect(110, 520, 85, 23));
        cB_Combine_fmo->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        pB_wfVCO = new QPushButton(centralwidget);
        pB_wfVCO->setObjectName("pB_wfVCO");
        pB_wfVCO->setGeometry(QRect(60, 270, 94, 26));
        sizePolicy.setHeightForWidth(pB_wfVCO->sizePolicy().hasHeightForWidth());
        pB_wfVCO->setSizePolicy(sizePolicy);
        pB_wfVCO->setFont(font2);
        pB_wfFMO = new QPushButton(centralwidget);
        pB_wfFMO->setObjectName("pB_wfFMO");
        pB_wfFMO->setGeometry(QRect(60, 460, 94, 26));
        sizePolicy.setHeightForWidth(pB_wfFMO->sizePolicy().hasHeightForWidth());
        pB_wfFMO->setSizePolicy(sizePolicy);
        pB_wfFMO->setFont(font2);
        tB_play_0 = new QToolButton(centralwidget);
        tB_play_0->setObjectName("tB_play_0");
        tB_play_0->setGeometry(QRect(1140, 50, 26, 25));
        tB_play_1 = new QToolButton(centralwidget);
        tB_play_1->setObjectName("tB_play_1");
        tB_play_1->setGeometry(QRect(1140, 80, 26, 25));
        tB_play_2 = new QToolButton(centralwidget);
        tB_play_2->setObjectName("tB_play_2");
        tB_play_2->setGeometry(QRect(1140, 110, 26, 25));
        tB_play_3 = new QToolButton(centralwidget);
        tB_play_3->setObjectName("tB_play_3");
        tB_play_3->setGeometry(QRect(1140, 140, 26, 25));
        tB_play_4 = new QToolButton(centralwidget);
        tB_play_4->setObjectName("tB_play_4");
        tB_play_4->setGeometry(QRect(1140, 170, 26, 25));
        tB_play_5 = new QToolButton(centralwidget);
        tB_play_5->setObjectName("tB_play_5");
        tB_play_5->setGeometry(QRect(1140, 200, 26, 25));
        tB_play_6 = new QToolButton(centralwidget);
        tB_play_6->setObjectName("tB_play_6");
        tB_play_6->setGeometry(QRect(1140, 230, 26, 25));
        tB_play_7 = new QToolButton(centralwidget);
        tB_play_7->setObjectName("tB_play_7");
        tB_play_7->setGeometry(QRect(1140, 260, 26, 25));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setEnabled(true);
        QFont font3;
        font3.setPointSize(13);
        statusbar->setFont(font3);
        MainWindow->setStatusBar(statusbar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setEnabled(false);
        menuBar->setGeometry(QRect(0, 0, 1221, 23));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "OceanGUI", nullptr));
        labelVCO->setText(QCoreApplication::translate("MainWindow", "VCO", nullptr));
        labelFMO->setText(QCoreApplication::translate("MainWindow", "FMO", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        cb_ssta1->setText(QString());
        cb_ssta2->setText(QString());
        cb_ssta3->setText(QString());
        cb_ssta7->setText(QString());
        cb_ssta4->setText(QString());
        cb_ssta5->setText(QString());
        cb_ssta0->setText(QString());
        cb_ssta6->setText(QString());
        pB_Mute_StA->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        pB_play_notes->setText(QCoreApplication::translate("MainWindow", "Notes,  \n"
"Instruments", nullptr));
        pBtoggleRecord->setText(QCoreApplication::translate("MainWindow", "Record", nullptr));
        rb_S0->setText(QCoreApplication::translate("MainWindow", "Instrument 0", nullptr));
        rb_S1->setText(QCoreApplication::translate("MainWindow", "Instrument 1", nullptr));
        rb_S2->setText(QCoreApplication::translate("MainWindow", "Instrument 2", nullptr));
        rb_S3->setText(QCoreApplication::translate("MainWindow", "Instrument 3", nullptr));
        pB_WdRole->setText(QString());
        pB_wd_mode->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "Hall", nullptr));
        pB_Spectrum->setText(QCoreApplication::translate("MainWindow", "Spectrum", nullptr));
        pb_clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Instrument Bps", nullptr));
        pB_Mute->setText(QCoreApplication::translate("MainWindow", "Mute", nullptr));
        pBGuiExit->setText(QCoreApplication::translate("MainWindow", "GUI EXIT (ESC)", nullptr));
        pb_Keyboard->setText(QCoreApplication::translate("MainWindow", "Keyboard", nullptr));
        pBAudioServer->setText(QCoreApplication::translate("MainWindow", "Audio Server", nullptr));
        pBComposer->setText(QCoreApplication::translate("MainWindow", "Composer", nullptr));
        pB_Save->setText(QCoreApplication::translate("MainWindow", "Save Default", nullptr));
        pB_Rtsp->setText(QCoreApplication::translate("MainWindow", "RTSP", nullptr));
        pB_Cutter->setText(QCoreApplication::translate("MainWindow", "CutDesk", nullptr));
        pB_Capture->setText(QCoreApplication::translate("MainWindow", "Capture", nullptr));
        pb_SDSview->setText(QCoreApplication::translate("MainWindow", "view SDS", nullptr));
        cb_overwrite->setText(QCoreApplication::translate("MainWindow", "Overwrite", nullptr));
        tB_logState->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        pB_oscgroup->setText(QString());
        lb_FMO_LFO->setText(QCoreApplication::translate("MainWindow", "LFO", nullptr));
        lb_VCO_LFO->setText(QCoreApplication::translate("MainWindow", "LFO", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "adjust", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "adjust", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Balance", nullptr));
        cb_connect_vcov->setText(QCoreApplication::translate("MainWindow", "FMO frq->\n"
"VCO vol", nullptr));
        cb_connect_fmov->setText(QCoreApplication::translate("MainWindow", "VCO frq ->\n"
"FMO vol", nullptr));
        cb_connect_oscv->setText(QCoreApplication::translate("MainWindow", "connect VCO", nullptr));
        cb_connect_oscf->setText(QCoreApplication::translate("MainWindow", "connect FMO", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Volume [%]", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Volume [%]", nullptr));
        pBSynthesizer->setText(QCoreApplication::translate("MainWindow", "Instrument", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "0              Volume        100        |   storing\342\226\240           stored    playing\342\226\272", nullptr));
        cb_fsta0->setText(QString());
        cb_fsta1->setText(QString());
        cb_fsta2->setText(QString());
        cb_fsta3->setText(QString());
        cb_fsta4->setText(QString());
        cb_fsta5->setText(QString());
        cb_fsta7->setText(QString());
        cb_fsta6->setText(QString());
        label_17->setText(QCoreApplication::translate("MainWindow", "Delay", nullptr));
        pB_ADSR->setText(QCoreApplication::translate("MainWindow", "ADSR", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Keyboard Bps", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "sub connection", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "WAV file", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "M\n"
"a\n"
"s\n"
"t\n"
"e\n"
"r", nullptr));
        pB_wfOSC->setText(QString());
        lbl_frqglide_sec->setText(QCoreApplication::translate("MainWindow", "txt", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Frequency [Hz]", nullptr));
        glidefrequency->setText(QCoreApplication::translate("MainWindow", "glide frq.", nullptr));
        cB_Combine_vco->setText(QCoreApplication::translate("MainWindow", "Combine", nullptr));
        cB_Combine_fmo->setText(QCoreApplication::translate("MainWindow", "Combine", nullptr));
        pB_wfVCO->setText(QString());
        pB_wfFMO->setText(QString());
        tB_play_0->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_1->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_2->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_3->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_4->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_5->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_6->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tB_play_7->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
