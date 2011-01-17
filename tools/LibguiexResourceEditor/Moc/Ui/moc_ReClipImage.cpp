/****************************************************************************
** Meta object code from reading C++ file 'ReClipImage.h'
**
** Created: Mon Jan 17 22:39:13 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReClipImage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReClipImage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReClipImage[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x0a,
      37,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReClipImage[] = {
    "RE::ReClipImage\0\0_scalar\0OnZoom(int)\0"
    "OnDelete()\0"
};

const QMetaObject RE::ReClipImage::staticMetaObject = {
    { &ReBaseWidget<QLabel>::staticMetaObject, qt_meta_stringdata_RE__ReClipImage,
      qt_meta_data_RE__ReClipImage, 0 }
};

const QMetaObject *RE::ReClipImage::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReClipImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReClipImage))
        return static_cast<void*>(const_cast< ReClipImage*>(this));
    return ReBaseWidget<QLabel>::qt_metacast(_clname);
}

int RE::ReClipImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QLabel>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnDelete(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
