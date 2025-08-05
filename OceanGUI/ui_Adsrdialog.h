/********************************************************************************
** Form generated from reading UI file 'Adsrdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADSRDIALOG_H
#define UI_ADSRDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ADSRDialog_class
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QHBoxLayout *horizontalLayout;
    QSlider *verticalSlider_2;
    QSlider *verticalSlider;
    QSpacerItem *horizontalSpacer;
    QSlider *verticalSlider_3;
    QSlider *verticalSlider_4;
    QSpacerItem *horizontalSpacer_2;
    QSlider *verticalSlider_5;
    QSlider *verticalSlider_6;
    QSpacerItem *horizontalSpacer_3;
    QSlider *verticalSlider_7;
    QSlider *verticalSlider_8;
    QSpacerItem *horizontalSpacer_5;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLineEdit *lineEdit;

    void setupUi(QDialog *ADSRDialog_class)
    {
        if (ADSRDialog_class->objectName().isEmpty())
            ADSRDialog_class->setObjectName("ADSRDialog_class");
        ADSRDialog_class->resize(356, 171);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ADSRDialog_class->sizePolicy().hasHeightForWidth());
        ADSRDialog_class->setSizePolicy(sizePolicy);
        gridLayoutWidget = new QWidget(ADSRDialog_class);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(20, 10, 328, 154));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        radioButton = new QRadioButton(gridLayoutWidget);
        radioButton->setObjectName("radioButton");

        verticalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(gridLayoutWidget);
        radioButton_2->setObjectName("radioButton_2");

        verticalLayout->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(gridLayoutWidget);
        radioButton_3->setObjectName("radioButton_3");

        verticalLayout->addWidget(radioButton_3);


        gridLayout->addLayout(verticalLayout, 0, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalSlider_2 = new QSlider(gridLayoutWidget);
        verticalSlider_2->setObjectName("verticalSlider_2");
        verticalSlider_2->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_2);

        verticalSlider = new QSlider(gridLayoutWidget);
        verticalSlider->setObjectName("verticalSlider");
        verticalSlider->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalSlider_3 = new QSlider(gridLayoutWidget);
        verticalSlider_3->setObjectName("verticalSlider_3");
        verticalSlider_3->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_3);

        verticalSlider_4 = new QSlider(gridLayoutWidget);
        verticalSlider_4->setObjectName("verticalSlider_4");
        verticalSlider_4->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalSlider_5 = new QSlider(gridLayoutWidget);
        verticalSlider_5->setObjectName("verticalSlider_5");
        verticalSlider_5->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_5);

        verticalSlider_6 = new QSlider(gridLayoutWidget);
        verticalSlider_6->setObjectName("verticalSlider_6");
        verticalSlider_6->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_6);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        verticalSlider_7 = new QSlider(gridLayoutWidget);
        verticalSlider_7->setObjectName("verticalSlider_7");
        verticalSlider_7->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_7);

        verticalSlider_8 = new QSlider(gridLayoutWidget);
        verticalSlider_8->setObjectName("verticalSlider_8");
        verticalSlider_8->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout->addWidget(verticalSlider_8);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 1, 1, 1);

        spinBox = new QSpinBox(gridLayoutWidget);
        spinBox->setObjectName("spinBox");

        gridLayout->addWidget(spinBox, 1, 3, 1, 1);

        spinBox_2 = new QSpinBox(gridLayoutWidget);
        spinBox_2->setObjectName("spinBox_2");

        gridLayout->addWidget(spinBox_2, 2, 3, 1, 1);

        lineEdit = new QLineEdit(gridLayoutWidget);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 2, 0, 1, 1);


        retranslateUi(ADSRDialog_class);

        QMetaObject::connectSlotsByName(ADSRDialog_class);
    } // setupUi

    void retranslateUi(QDialog *ADSRDialog_class)
    {
        ADSRDialog_class->setWindowTitle(QCoreApplication::translate("ADSRDialog_class", "Dialog", nullptr));
        radioButton->setText(QCoreApplication::translate("ADSRDialog_class", "OSC", nullptr));
        radioButton_2->setText(QCoreApplication::translate("ADSRDialog_class", "VCO", nullptr));
        radioButton_3->setText(QCoreApplication::translate("ADSRDialog_class", "FMO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ADSRDialog_class: public Ui_ADSRDialog_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADSRDIALOG_H
