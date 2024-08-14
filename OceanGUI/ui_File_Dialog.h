/********************************************************************************
** Form generated from reading UI file 'File_Dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
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

QT_BEGIN_NAMESPACE

class Ui_File_Dialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *pbPlayNotes;
    QLabel *label;
    QComboBox *cb_notefilenames;
    QLineEdit *lE_Instrument;
    QLineEdit *lE_NotesFile;
    QComboBox *cb_instrumentfiles;
    QPushButton *pbInstrumentDone;
    QLabel *lbl_selected_notes;
    QPushButton *pbNotesDone;
    QLineEdit *lE_Rythm;
    QLineEdit *lE_Notes;
    QFrame *line;

    void setupUi(QDialog *File_Dialog)
    {
        if (File_Dialog->objectName().isEmpty())
            File_Dialog->setObjectName(QString::fromUtf8("File_Dialog"));
        File_Dialog->resize(400, 207);
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
        pbPlayNotes = new QPushButton(File_Dialog);
        pbPlayNotes->setObjectName(QString::fromUtf8("pbPlayNotes"));
        pbPlayNotes->setAutoDefault(false);

        gridLayout->addWidget(pbPlayNotes, 10, 1, 1, 1);

        label = new QLabel(File_Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        cb_notefilenames = new QComboBox(File_Dialog);
        cb_notefilenames->setObjectName(QString::fromUtf8("cb_notefilenames"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cb_notefilenames->sizePolicy().hasHeightForWidth());
        cb_notefilenames->setSizePolicy(sizePolicy1);
        cb_notefilenames->setSizeIncrement(QSize(10, 0));

        gridLayout->addWidget(cb_notefilenames, 6, 1, 1, 1);

        lE_Instrument = new QLineEdit(File_Dialog);
        lE_Instrument->setObjectName(QString::fromUtf8("lE_Instrument"));
        lE_Instrument->setClearButtonEnabled(true);

        gridLayout->addWidget(lE_Instrument, 1, 2, 1, 2);

        lE_NotesFile = new QLineEdit(File_Dialog);
        lE_NotesFile->setObjectName(QString::fromUtf8("lE_NotesFile"));

        gridLayout->addWidget(lE_NotesFile, 6, 2, 1, 2);

        cb_instrumentfiles = new QComboBox(File_Dialog);
        cb_instrumentfiles->setObjectName(QString::fromUtf8("cb_instrumentfiles"));
        cb_instrumentfiles->setMaxVisibleItems(20);
        cb_instrumentfiles->setMaxCount(30);
        cb_instrumentfiles->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

        gridLayout->addWidget(cb_instrumentfiles, 1, 1, 1, 1);

        pbInstrumentDone = new QPushButton(File_Dialog);
        pbInstrumentDone->setObjectName(QString::fromUtf8("pbInstrumentDone"));
        pbInstrumentDone->setAutoDefault(false);

        gridLayout->addWidget(pbInstrumentDone, 0, 2, 1, 1);

        lbl_selected_notes = new QLabel(File_Dialog);
        lbl_selected_notes->setObjectName(QString::fromUtf8("lbl_selected_notes"));

        gridLayout->addWidget(lbl_selected_notes, 8, 1, 1, 1);

        pbNotesDone = new QPushButton(File_Dialog);
        pbNotesDone->setObjectName(QString::fromUtf8("pbNotesDone"));

        gridLayout->addWidget(pbNotesDone, 8, 2, 1, 1);

        lE_Rythm = new QLineEdit(File_Dialog);
        lE_Rythm->setObjectName(QString::fromUtf8("lE_Rythm"));

        gridLayout->addWidget(lE_Rythm, 10, 2, 1, 1);

        lE_Notes = new QLineEdit(File_Dialog);
        lE_Notes->setObjectName(QString::fromUtf8("lE_Notes"));

        gridLayout->addWidget(lE_Notes, 9, 1, 1, 3);

        line = new QFrame(File_Dialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 2, 1, 1, 2);


        retranslateUi(File_Dialog);

        QMetaObject::connectSlotsByName(File_Dialog);
    } // setupUi

    void retranslateUi(QDialog *File_Dialog)
    {
        File_Dialog->setWindowTitle(QCoreApplication::translate("File_Dialog", "File Director", nullptr));
        pbPlayNotes->setText(QString());
        label->setText(QCoreApplication::translate("File_Dialog", "Instruments", nullptr));
        pbInstrumentDone->setText(QCoreApplication::translate("File_Dialog", "Instrument Done", nullptr));
        lbl_selected_notes->setText(QCoreApplication::translate("File_Dialog", "Notes: (CcDdEFfGgAaH)", nullptr));
        pbNotesDone->setText(QCoreApplication::translate("File_Dialog", "Notes Done", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File_Dialog: public Ui_File_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_DIALOG_H
