/********************************************************************************
** Form generated from reading UI file 'DBdisplay.ui'
**
** Created: Tue 24. Apr 20:24:14 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBDISPLAY_H
#define UI_DBDISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBdisplay
{
public:
    QTableView *tableView;

    void setupUi(QWidget *DBdisplay)
    {
        if (DBdisplay->objectName().isEmpty())
            DBdisplay->setObjectName(QString::fromUtf8("DBdisplay"));
        DBdisplay->resize(389, 298);
        tableView = new QTableView(DBdisplay);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(30, 10, 256, 192));

        retranslateUi(DBdisplay);

        QMetaObject::connectSlotsByName(DBdisplay);
    } // setupUi

    void retranslateUi(QWidget *DBdisplay)
    {
        DBdisplay->setWindowTitle(QApplication::translate("DBdisplay", "DBdisplay", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DBdisplay: public Ui_DBdisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBDISPLAY_H
