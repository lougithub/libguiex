/****************************************************************************
** Meta object code from reading C++ file 'ReAnimModel.h'
**
** Created: Tue Dec 21 20:44:50 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Core/ReAnimModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimModel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      35,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimModel[] = {
    "RE::ReAnimModel\0\0_trackType,_frame\0"
    "OnFrameChanged(eTrackType,ReAnimFrame*)\0"
};

const QMetaObject RE::ReAnimModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_RE__ReAnimModel,
      qt_meta_data_RE__ReAnimModel, 0 }
};

const QMetaObject *RE::ReAnimModel::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimModel))
        return static_cast<void*>(const_cast< ReAnimModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int RE::ReAnimModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnFrameChanged((*reinterpret_cast< eTrackType(*)>(_a[1])),(*reinterpret_cast< ReAnimFrame*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
