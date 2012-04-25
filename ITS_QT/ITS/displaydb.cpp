
#include "displaydb.h"
displaydb::displaydb(int id, QWidget *parent)
    : QDialog(parent)
    {
    vboxlayout = new QVBoxLayout(this);
 
    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    nameLineEdit->setAlignment(Qt::AlignTop);
    vboxlayout->addWidget(nameLineEdit);
 
    saveButton = new QPushButton("Save",this);
    saveButton->setFixedHeight(60);
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    vboxlayout->addWidget(saveButton);
 
    // 'person' table model
    tableModel = new QSqlTableModel(this);
    tableModel->setTable("Person");
    // Set where clause
    // QLineEdit shows person lastname that id is given parameter
    tableModel->setFilter(QString("id=%1").arg(id));
    tableModel->select();
 
    // Maps QLineEdit ui control into "person" table's lastname field
    dataMapper = new QDataWidgetMapper(this);
    dataMapper->setModel(tableModel);
    // We want that data is stored only if we call QDataWidgetMapper::submit()
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    dataMapper->setItemDelegate(new QSqlRelationalDelegate(dataMapper));
    dataMapper->addMapping(nameLineEdit, tableModel->fieldIndex("lastname"));
    dataMapper->toFirst();
 
    setLayout(vboxlayout);
    }
 
displaydb::~displaydb()
    {
    }
 
 
void displaydb::save()
   {
    // Stores data from QLineEdit to database using QDataWidgetMapper
    dataMapper->submit();
   }
	