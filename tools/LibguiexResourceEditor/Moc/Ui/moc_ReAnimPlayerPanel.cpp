/****************************************************************************
** Meta object code from reading C++ file 'ReAnimPlayerPanel.h'
**
** Created: Tue Jan 18 22:23:34 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimPlayerPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimPlayerPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimPlayerPanel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      38,   22,   22,   22, 0x0a,
      72,   64,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimPlayerPanel[] = {
    "RE::ReAnimPlayerPanel\0\0OnTogglePlay()\0"
    "OnSceneSelectionChanged()\0_cursor\0"
    "OnRulerCursorChanged(int)\0"
};

const QMetaObject RE::ReAnimPlayerPanel::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimPlayerPanel,
      qt_meta_data_RE__ReAnimPlayerPanel, 0 }
};

const QMetaObject *RE::ReAnimPlayerPanel::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimPlayerPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimPlayerPanel))
        return static_cast<void*>(const_cast< ReAnimPlayerPanel*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimPlayerPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnTogglePlay(); break;
        case 1: OnSceneSelectionChanged(); break;
        case 2: OnRulerCursorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
