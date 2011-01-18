/****************************************************************************
** Meta object code from reading C++ file 'ReZoomInfo.h'
**
** Created: Tue Jan 18 22:58:00 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Core/ReZoomInfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReZoomInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReZoomInfo[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReZoomInfo[] = {
    "RE::ReZoomInfo\0\0_scalar\0ScalarChanged(int)\0"
};

const QMetaObject RE::ReZoomInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RE__ReZoomInfo,
      qt_meta_data_RE__ReZoomInfo, 0 }
};

const QMetaObject *RE::ReZoomInfo::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReZoomInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReZoomInfo))
        return static_cast<void*>(const_cast< ReZoomInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int RE::ReZoomInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ScalarChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void RE::ReZoomInfo::ScalarChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
