/****************************************************************************
** Meta object code from reading C++ file 'ReAnimSheet.h'
**
** Created: Sun Jan 16 16:12:09 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimSheet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimSheet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimSheet[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimSheet[] = {
    "RE::ReAnimSheet\0"
};

const QMetaObject RE::ReAnimSheet::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimSheet,
      qt_meta_data_RE__ReAnimSheet, 0 }
};

const QMetaObject *RE::ReAnimSheet::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimSheet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimSheet))
        return static_cast<void*>(const_cast< ReAnimSheet*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimSheet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
