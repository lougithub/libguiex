/****************************************************************************
** Meta object code from reading C++ file 'ReAnimConsoleWidget.h'
**
** Created: Sun Jan 16 16:12:10 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimConsoleWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimConsoleWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimConsoleWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      31,   25,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   25,   24,   24, 0x0a,
      91,   25,   24,   24, 0x0a,
     133,  126,   24,   24, 0x0a,
     155,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimConsoleWidget[] = {
    "RE::ReAnimConsoleWidget\0\0_item\0"
    "Focus(ReAnimGraphicsItem*)\0"
    "OnItemAdded(ReAnimGraphicsItem*)\0"
    "OnItemDeleted(ReAnimGraphicsItem*)\0"
    "_point\0OnContextMenu(QPoint)\0OnExport()\0"
};

const QMetaObject RE::ReAnimConsoleWidget::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimConsoleWidget,
      qt_meta_data_RE__ReAnimConsoleWidget, 0 }
};

const QMetaObject *RE::ReAnimConsoleWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimConsoleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimConsoleWidget))
        return static_cast<void*>(const_cast< ReAnimConsoleWidget*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimConsoleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Focus((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 1: OnItemAdded((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 2: OnItemDeleted((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 3: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: OnExport(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimConsoleWidget::Focus(ReAnimGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
