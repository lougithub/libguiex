/****************************************************************************
** Meta object code from reading C++ file 'ReAnimEntityPanel.h'
**
** Created: Sun Jan 9 22:07:44 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimEntityPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimEntityPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimEntityPanel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      44,   38,   22,   22, 0x0a,
      77,   38,   22,   22, 0x0a,
     112,   22,   22,   22, 0x0a,
     146,  138,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimEntityPanel[] = {
    "RE::ReAnimEntityPanel\0\0UpdateLayout()\0"
    "_item\0OnItemAdded(ReAnimGraphicsItem*)\0"
    "OnItemDeleted(ReAnimGraphicsItem*)\0"
    "OnSceneSelectionChanged()\0_cursor\0"
    "OnRulerCursorChanged(int)\0"
};

const QMetaObject RE::ReAnimEntityPanel::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimEntityPanel,
      qt_meta_data_RE__ReAnimEntityPanel, 0 }
};

const QMetaObject *RE::ReAnimEntityPanel::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimEntityPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimEntityPanel))
        return static_cast<void*>(const_cast< ReAnimEntityPanel*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimEntityPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: UpdateLayout(); break;
        case 1: OnItemAdded((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 2: OnItemDeleted((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 3: OnSceneSelectionChanged(); break;
        case 4: OnRulerCursorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
