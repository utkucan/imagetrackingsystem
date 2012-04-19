
#ifndef DISPLAYDB_H
#define DISPLAYDB_H
#include <QWidget>
#include "ui_DBdisplay.h"

class displaydb : public QWidget
{
public:
	displaydb(QWidget *parent = 0, Qt::WFlags flags = 0);
	~displaydb(void);
private:
	Ui::DBdisplay uidb;
};
#endif
/*

#include <QtGui>
#include <QtSql>
 
class displaydb : public QDialog
    {
    Q_OBJECT
 
    public:
        displaydb(int id = 0, QWidget *parent = 0);
        ~displaydb();
 
    public slots:
        void save();
 
    private:
        // "person" table mode class
        QSqlTableModel*             tableModel;
 
        // Maps QLineEdit ui control into "person"
        // table's lastname field
        QDataWidgetMapper*          dataMapper;
 
        QVBoxLayout*                vboxlayout;
        QLineEdit*                  nameLineEdit;
        QPushButton*                saveButton;
    };
	*/