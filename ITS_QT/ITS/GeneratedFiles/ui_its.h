/********************************************************************************
** Form generated from reading UI file 'its.ui'
**
** Created: Sun 6. May 21:49:23 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITS_H
#define UI_ITS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ITSClass
{
public:
    QAction *actionImport;
    QAction *actionKlasor;
    QAction *actionWeb;
    QAction *actionExport;
    QAction *actionExport_Photos;
    QAction *actionSearch_HardDisk;
    QAction *actionAbout_ITS;
    QWidget *centralWidget;
    QMdiArea *mdiArea;
    QTreeWidget *treeWidget;
    QMdiArea *mdiDownSapmle;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuImport_Photos;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ITSClass)
    {
        if (ITSClass->objectName().isEmpty())
            ITSClass->setObjectName(QString::fromUtf8("ITSClass"));
        ITSClass->resize(902, 573);
        actionImport = new QAction(ITSClass);
        actionImport->setObjectName(QString::fromUtf8("actionImport"));
        actionKlasor = new QAction(ITSClass);
        actionKlasor->setObjectName(QString::fromUtf8("actionKlasor"));
        actionWeb = new QAction(ITSClass);
        actionWeb->setObjectName(QString::fromUtf8("actionWeb"));
        actionExport = new QAction(ITSClass);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        actionExport_Photos = new QAction(ITSClass);
        actionExport_Photos->setObjectName(QString::fromUtf8("actionExport_Photos"));
        actionSearch_HardDisk = new QAction(ITSClass);
        actionSearch_HardDisk->setObjectName(QString::fromUtf8("actionSearch_HardDisk"));
        actionAbout_ITS = new QAction(ITSClass);
        actionAbout_ITS->setObjectName(QString::fromUtf8("actionAbout_ITS"));
        centralWidget = new QWidget(ITSClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setGeometry(QRect(190, 10, 700, 400));
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setViewMode(QMdiArea::SubWindowView);
        treeWidget = new QTreeWidget(centralWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 170, 510));
        mdiDownSapmle = new QMdiArea(centralWidget);
        mdiDownSapmle->setObjectName(QString::fromUtf8("mdiDownSapmle"));
        mdiDownSapmle->setGeometry(QRect(190, 410, 700, 110));
        mdiDownSapmle->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ITSClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ITSClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 902, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuImport_Photos = new QMenu(menuFile);
        menuImport_Photos->setObjectName(QString::fromUtf8("menuImport_Photos"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        ITSClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ITSClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ITSClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ITSClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ITSClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(menuImport_Photos->menuAction());
        menuImport_Photos->addAction(actionKlasor);
        menuImport_Photos->addAction(actionSearch_HardDisk);
        menuHelp->addAction(actionAbout_ITS);
        mainToolBar->addSeparator();

        retranslateUi(ITSClass);
        QObject::connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), ITSClass, SLOT(treeWidgetSelectionChange()));
        QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), ITSClass, SLOT(updateFace()));

        QMetaObject::connectSlotsByName(ITSClass);
    } // setupUi

    void retranslateUi(QMainWindow *ITSClass)
    {
        ITSClass->setWindowTitle(QApplication::translate("ITSClass", "ITS", 0, QApplication::UnicodeUTF8));
        actionImport->setText(QApplication::translate("ITSClass", "Import", 0, QApplication::UnicodeUTF8));
        actionKlasor->setText(QApplication::translate("ITSClass", "Folder", 0, QApplication::UnicodeUTF8));
        actionWeb->setText(QApplication::translate("ITSClass", "Web", 0, QApplication::UnicodeUTF8));
        actionExport->setText(QApplication::translate("ITSClass", "Export", 0, QApplication::UnicodeUTF8));
        actionExport_Photos->setText(QApplication::translate("ITSClass", "Export Photos", 0, QApplication::UnicodeUTF8));
        actionSearch_HardDisk->setText(QApplication::translate("ITSClass", "Search HardDisk", 0, QApplication::UnicodeUTF8));
        actionAbout_ITS->setText(QApplication::translate("ITSClass", "About ITS", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("ITSClass", "Menu", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("ITSClass", "File", 0, QApplication::UnicodeUTF8));
        menuImport_Photos->setTitle(QApplication::translate("ITSClass", "Import Photos", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("ITSClass", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ITSClass: public Ui_ITSClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITS_H
