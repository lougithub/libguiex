/****************************************************************************
** Meta object code from reading C++ file 'ReClipWorkshop.h'
**
** Created: Tue Jan 18 22:34:27 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReClipWorkshop.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReClipWorkshop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReClipWorkshop[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      27,   20,   19,   19, 0x0a,
      49,   19,   19,   19, 0x0a,
      63,   19,   19,   19, 0x0a,
      74,   19,   19,   19, 0x0a,
      83,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReClipWorkshop[] = {
    "RE::ReClipWorkshop\0\0_point\0"
    "OnContextMenu(QPoint)\0OnLoadImage()\0"
    "OnImport()\0OnSave()\0OnSaveAs()\0"
};

const QMetaObject RE::ReClipWorkshop::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReClipWorkshop,
      qt_meta_data_RE__ReClipWorkshop, 0 }
};

const QMetaObject *RE::ReClipWorkshop::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReClipWorkshop::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReClipWorkshop))
        return static_cast<void*>(const_cast< ReClipWorkshop*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReClipWorkshop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: OnLoadImage(); break;
        case 2: OnImport(); break;
        case 3: OnSave(); break;
        case 4: OnSaveAs(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
