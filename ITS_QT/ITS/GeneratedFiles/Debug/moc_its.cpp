/****************************************************************************
** Meta object code from reading C++ file 'its.h'
**
** Created: Tue 24. Apr 20:24:10 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../its.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'its.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ITS[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      33,    4,    4,    4, 0x08,
      70,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ITS[] = {
    "ITS\0\0on_actionKlasor_triggered()\0"
    "on_actionSearch_HardDisk_triggered()\0"
    "controlList()\0"
};

const QMetaObject ITS::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ITS,
      qt_meta_data_ITS, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ITS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ITS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ITS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ITS))
        return static_cast<void*>(const_cast< ITS*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ITS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_actionKlasor_triggered(); break;
        case 1: on_actionSearch_HardDisk_triggered(); break;
        case 2: controlList(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
