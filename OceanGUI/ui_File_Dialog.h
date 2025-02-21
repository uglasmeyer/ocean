/********************************************************************************
** Form generated from reading UI file 'File_Dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_DIALOG_H
#define UI_FILE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_File_Dialog
{
public:
    QGridLayout *gridLayout;
    QComboBox *cb_convention;
    QLabel *label_3;
    QSpinBox *sB_Octave;
    QLineEdit *lE_NotesFile;
    QComboBox *cb_nps;
    QLabel *label;
    QPushButton *pbInstrumentDone;
    QFrame *line_2;
    QComboBox *cb_notefilenames;
    QLineEdit *lE_Notes;
    QLineEdit *lE_Instrument;
    QFrame *line;
    QLabel *lbl_selected_notes;
    QLabel *label_2;
    QPushButton *pbNotesDone;
    QLabel *label_4;
    QComboBox *cb_instrumentfiles;
    QComboBox *cb_Notestype;
    QLabel *label_5;
    QLineEdit *lE_Rythm;

    void setupUi(QDialog *File_Dialog)
    {
        if (File_Dialog->objectName().isEmpty())
            File_Dialog->setObjectName(QString::fromUtf8("File_Dialog"));
        File_Dialog->resize(344, 311);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(File_Dialog->sizePolicy().hasHeightForWidth());
        File_Dialog->setSizePolicy(sizePolicy);
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
        File_Dialog->setPalette(palette);
        gridLayout = new QGridLayout(File_Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cb_convention = new QComboBox(File_Dialog);
        cb_convention->setObjectName(QString::fromUtf8("cb_convention"));

        gridLayout->addWidget(cb_convention, 10, 2, 1, 1);

        label_3 = new QLabel(File_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 9, 1, 1, 1);

        sB_Octave = new QSpinBox(File_Dialog);
        sB_Octave->setObjectName(QString::fromUtf8("sB_Octave"));
        sB_Octave->setMinimum(0);
        sB_Octave->setMaximum(5);

        gridLayout->addWidget(sB_Octave, 10, 0, 1, 1);

        lE_NotesFile = new QLineEdit(File_Dialog);
        lE_NotesFile->setObjectName(QString::fromUtf8("lE_NotesFile"));

        gridLayout->addWidget(lE_NotesFile, 4, 2, 1, 1);

        cb_nps = new QComboBox(File_Dialog);
        cb_nps->setObjectName(QString::fromUtf8("cb_nps"));

        gridLayout->addWidget(cb_nps, 10, 1, 1, 1);

        label = new QLabel(File_Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pbInstrumentDone = new QPushButton(File_Dialog);
        pbInstrumentDone->setObjectName(QString::fromUtf8("pbInstrumentDone"));
        pbInstrumentDone->setAutoDefault(false);

        gridLayout->addWidget(pbInstrumentDone, 0, 2, 1, 1);

        line_2 = new QFrame(File_Dialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 8, 0, 1, 3);

        cb_notefilenames = new QComboBox(File_Dialog);
        cb_notefilenames->setObjectName(QString::fromUtf8("cb_notefilenames"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cb_notefilenames->sizePolicy().hasHeightForWidth());
        cb_notefilenames->setSizePolicy(sizePolicy1);
        cb_notefilenames->setSizeIncrement(QSize(10, 0));

        gridLayout->addWidget(cb_notefilenames, 4, 0, 1, 2);

        lE_Notes = new QLineEdit(File_Dialog);
        lE_Notes->setObjectName(QString::fromUtf8("lE_Notes"));

        gridLayout->addWidget(lE_Notes, 6, 0, 1, 3);

        lE_Instrument = new QLineEdit(File_Dialog);
        lE_Instrument->setObjectName(QString::fromUtf8("lE_Instrument"));
        lE_Instrument->setClearButtonEnabled(true);

        gridLayout->addWidget(lE_Instrument, 1, 2, 1, 1);

        line = new QFrame(File_Dialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 2, 0, 1, 3);

        lbl_selected_notes = new QLabel(File_Dialog);
        lbl_selected_notes->setObjectName(QString::fromUtf8("lbl_selected_notes"));

        gridLayout->addWidget(lbl_selected_notes, 5, 0, 1, 2);

        label_2 = new QLabel(File_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 9, 0, 1, 1);

        pbNotesDone = new QPushButton(File_Dialog);
        pbNotesDone->setObjectName(QString::fromUtf8("pbNotesDone"));

        gridLayout->addWidget(pbNotesDone, 5, 2, 1, 1);

        label_4 = new QLabel(File_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 9, 2, 1, 1);

        cb_instrumentfiles = new QComboBox(File_Dialog);
        cb_instrumentfiles->setObjectName(QString::fromUtf8("cb_instrumentfiles"));
        cb_instrumentfiles->setMaxVisibleItems(20);
        cb_instrumentfiles->setMaxCount(30);
        cb_instrumentfiles->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

        gridLayout->addWidget(cb_instrumentfiles, 1, 0, 1, 2);

        cb_Notestype = new QComboBox(File_Dialog);
        cb_Notestype->setObjectName(QString::fromUtf8("cb_Notestype"));

        gridLayout->addWidget(cb_Notestype, 3, 2, 1, 1);

        label_5 = new QLabel(File_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        lE_Rythm = new QLineEdit(File_Dialog);
        lE_Rythm->setObjectName(QString::fromUtf8("lE_Rythm"));

        gridLayout->addWidget(lE_Rythm, 7, 0, 1, 3);


        retranslateUi(File_Dialog);

        QMetaObject::connectSlotsByName(File_Dialog);
    } // setupUi

    void retranslateUi(QDialog *File_Dialog)
    {
        File_Dialog->setWindowTitle(QCoreApplication::translate("File_Dialog", "Notes&Instruments", nullptr));
        label_3->setText(QCoreApplication::translate("File_Dialog", "NPS", nullptr));
        label->setText(QCoreApplication::translate("File_Dialog", "Instruments", nullptr));
        pbInstrumentDone->setText(QCoreApplication::translate("File_Dialog", "Instrument Done", nullptr));
        lbl_selected_notes->setText(QCoreApplication::translate("File_Dialog", "Notes: (CcDdEFfGgAaH)", nullptr));
        label_2->setText(QCoreApplication::translate("File_Dialog", "Octave", nullptr));
        pbNotesDone->setText(QCoreApplication::translate("File_Dialog", "Notes Done", nullptr));
        label_4->setText(QCoreApplication::translate("File_Dialog", "Note Convention", nullptr));
        label_5->setText(QCoreApplication::translate("File_Dialog", "Notes type", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File_Dialog: public Ui_File_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_DIALOG_H
