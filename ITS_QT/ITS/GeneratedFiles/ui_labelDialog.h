/********************************************************************************
** Form generated from reading UI file 'labelDialog.ui'
**
** Created: Tue 24. Apr 20:24:14 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABELDIALOG_H
#define UI_LABELDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LableDialog
{
public:
    QComboBox *comboBox;
    QPushButton *pushButton;

    void setupUi(QDialog *LableDialog)
    {
        if (LableDialog->objectName().isEmpty())
            LableDialog->setObjectName(QString::fromUtf8("LableDialog"));
        LableDialog->resize(286, 100);
        LableDialog->setWindowOpacity(1);
        comboBox = new QComboBox(LableDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(0, 0, 130, 25));
        comboBox->setEditable(true);
        pushButton = new QPushButton(LableDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 0, 20, 25));

        retranslateUi(LableDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), LableDialog, SLOT(textChanged()));

        QMetaObject::connectSlotsByName(LableDialog);
    } // setupUi

    void retranslateUi(QDialog *LableDialog)
    {
        LableDialog->setWindowTitle(QApplication::translate("LableDialog", "Label", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("LableDialog", "ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LableDialog: public Ui_LableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABELDIALOG_H
