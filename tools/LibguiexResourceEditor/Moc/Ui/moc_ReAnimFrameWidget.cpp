/****************************************************************************
** Meta object code from reading C++ file 'ReAnimFrameWidget.h'
**
** Created: Wed Jan 5 21:23:23 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimFrameWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimFrameWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimFrameWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      29,   23,   22,   22, 0x05,
      61,   23,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimFrameWidget[] = {
    "RE::ReAnimFrameWidget\0\0_this\0"
    "EditStarted(ReAnimFrameWidget*)\0"
    "EditEnded(ReAnimFrameWidget*)\0"
};

const QMetaObject RE::ReAnimFrameWidget::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimFrameWidget,
      qt_meta_data_RE__ReAnimFrameWidget, 0 }
};

const QMetaObject *RE::ReAnimFrameWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimFrameWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimFrameWidget))
        return static_cast<void*>(const_cast< ReAnimFrameWidget*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimFrameWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: EditStarted((*reinterpret_cast< ReAnimFrameWidget*(*)>(_a[1]))); break;
        case 1: EditEnded((*reinterpret_cast< ReAnimFrameWidget*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimFrameWidget::EditStarted(ReAnimFrameWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReAnimFrameWidget::EditEnded(ReAnimFrameWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
