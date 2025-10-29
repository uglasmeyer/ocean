/********************************************************************************
** Form generated from reading UI file 'File_Dialog_class.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_DIALOG_CLASS_H
#define UI_FILE_DIALOG_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_File_Dialog_class
{
public:
    QGridLayout *gridLayout;
    QLabel *lbl_selected_notes;
    QComboBox *cb_nps;
    QPushButton *pbInstrumentDone;
    QLineEdit *lE_Rythm;
    QLabel *label;
    QComboBox *cb_convention;
    QLabel *label_4;
    QFrame *line_2;
    QLineEdit *lE_Instrument;
    QLineEdit *lE_Notes;
    QComboBox *cb_Notestype;
    QSpinBox *sB_Octave;
    QLabel *label_3;
    QPushButton *pbNotesDone;
    QLineEdit *lE_NotesFile;
    QLabel *label_2;
    QLabel *label_5;
    QComboBox *cb_notefilenames;
    QCheckBox *cb_longnote;
    QComboBox *cb_instrumentfiles;
    QFrame *line;
    QPushButton *pB_EditMusicxml;

    void setupUi(QDialog *File_Dialog_class)
    {
        if (File_Dialog_class->objectName().isEmpty())
            File_Dialog_class->setObjectName("File_Dialog_class");
        File_Dialog_class->resize(352, 341);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(File_Dialog_class->sizePolicy().hasHeightForWidth());
        File_Dialog_class->setSizePolicy(sizePolicy);
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
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush1);
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
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
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
        File_Dialog_class->setPalette(palette);
        gridLayout = new QGridLayout(File_Dialog_class);
        gridLayout->setObjectName("gridLayout");
        lbl_selected_notes = new QLabel(File_Dialog_class);
        lbl_selected_notes->setObjectName("lbl_selected_notes");

        gridLayout->addWidget(lbl_selected_notes, 5, 0, 1, 2);

        cb_nps = new QComboBox(File_Dialog_class);
        cb_nps->setObjectName("cb_nps");

        gridLayout->addWidget(cb_nps, 12, 1, 1, 1);

        pbInstrumentDone = new QPushButton(File_Dialog_class);
        pbInstrumentDone->setObjectName("pbInstrumentDone");
        pbInstrumentDone->setAutoDefault(false);

        gridLayout->addWidget(pbInstrumentDone, 0, 2, 1, 1);

        lE_Rythm = new QLineEdit(File_Dialog_class);
        lE_Rythm->setObjectName("lE_Rythm");

        gridLayout->addWidget(lE_Rythm, 8, 0, 1, 3);

        label = new QLabel(File_Dialog_class);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cb_convention = new QComboBox(File_Dialog_class);
        cb_convention->setObjectName("cb_convention");

        gridLayout->addWidget(cb_convention, 12, 2, 1, 1);

        label_4 = new QLabel(File_Dialog_class);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 11, 2, 1, 1);

        line_2 = new QFrame(File_Dialog_class);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_2, 9, 0, 1, 3);

        lE_Instrument = new QLineEdit(File_Dialog_class);
        lE_Instrument->setObjectName("lE_Instrument");
        lE_Instrument->setClearButtonEnabled(true);

        gridLayout->addWidget(lE_Instrument, 1, 2, 1, 1);

        lE_Notes = new QLineEdit(File_Dialog_class);
        lE_Notes->setObjectName("lE_Notes");

        gridLayout->addWidget(lE_Notes, 7, 0, 1, 3);

        cb_Notestype = new QComboBox(File_Dialog_class);
        cb_Notestype->setObjectName("cb_Notestype");

        gridLayout->addWidget(cb_Notestype, 3, 2, 1, 1);

        sB_Octave = new QSpinBox(File_Dialog_class);
        sB_Octave->setObjectName("sB_Octave");
        sB_Octave->setMinimum(0);
        sB_Octave->setMaximum(5);

        gridLayout->addWidget(sB_Octave, 12, 0, 1, 1);

        label_3 = new QLabel(File_Dialog_class);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 11, 1, 1, 1);

        pbNotesDone = new QPushButton(File_Dialog_class);
        pbNotesDone->setObjectName("pbNotesDone");

        gridLayout->addWidget(pbNotesDone, 5, 2, 1, 1);

        lE_NotesFile = new QLineEdit(File_Dialog_class);
        lE_NotesFile->setObjectName("lE_NotesFile");

        gridLayout->addWidget(lE_NotesFile, 4, 2, 1, 1);

        label_2 = new QLabel(File_Dialog_class);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 11, 0, 1, 1);

        label_5 = new QLabel(File_Dialog_class);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        cb_notefilenames = new QComboBox(File_Dialog_class);
        cb_notefilenames->setObjectName("cb_notefilenames");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cb_notefilenames->sizePolicy().hasHeightForWidth());
        cb_notefilenames->setSizePolicy(sizePolicy1);
        cb_notefilenames->setSizeIncrement(QSize(10, 0));

        gridLayout->addWidget(cb_notefilenames, 4, 0, 1, 2);

        cb_longnote = new QCheckBox(File_Dialog_class);
        cb_longnote->setObjectName("cb_longnote");

        gridLayout->addWidget(cb_longnote, 13, 0, 1, 1);

        cb_instrumentfiles = new QComboBox(File_Dialog_class);
        cb_instrumentfiles->setObjectName("cb_instrumentfiles");
        cb_instrumentfiles->setMaxVisibleItems(20);
        cb_instrumentfiles->setMaxCount(30);
        cb_instrumentfiles->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToMinimumContentsLengthWithIcon);

        gridLayout->addWidget(cb_instrumentfiles, 1, 0, 1, 2);

        line = new QFrame(File_Dialog_class);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 2, 0, 1, 3);

        pB_EditMusicxml = new QPushButton(File_Dialog_class);
        pB_EditMusicxml->setObjectName("pB_EditMusicxml");

        gridLayout->addWidget(pB_EditMusicxml, 6, 0, 1, 1);


        retranslateUi(File_Dialog_class);

        QMetaObject::connectSlotsByName(File_Dialog_class);
    } // setupUi

    void retranslateUi(QDialog *File_Dialog_class)
    {
        File_Dialog_class->setWindowTitle(QCoreApplication::translate("File_Dialog_class", "Notes&Instruments", nullptr));
        lbl_selected_notes->setText(QCoreApplication::translate("File_Dialog_class", "Notes: (CcDdEFfGgAaH)", nullptr));
        pbInstrumentDone->setText(QCoreApplication::translate("File_Dialog_class", "Instrument Done", nullptr));
        label->setText(QCoreApplication::translate("File_Dialog_class", "Instruments", nullptr));
        label_4->setText(QCoreApplication::translate("File_Dialog_class", "Note Convention", nullptr));
        label_3->setText(QCoreApplication::translate("File_Dialog_class", "NPS", nullptr));
        pbNotesDone->setText(QCoreApplication::translate("File_Dialog_class", "Notes Done", nullptr));
        label_2->setText(QCoreApplication::translate("File_Dialog_class", "Octave", nullptr));
        label_5->setText(QCoreApplication::translate("File_Dialog_class", "Notes type", nullptr));
        cb_longnote->setText(QCoreApplication::translate("File_Dialog_class", "Long note", nullptr));
        pB_EditMusicxml->setText(QCoreApplication::translate("File_Dialog_class", "Edit musicxml", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File_Dialog_class: public Ui_File_Dialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_DIALOG_CLASS_H
