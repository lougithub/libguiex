/****************************************************************************
** Meta object code from reading C++ file 'ReRulerWidget.h'
**
** Created: Thu Dec 16 21:15:39 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReRulerWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReRulerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReRulerWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x05,
      43,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   19,   18,   18, 0x0a,
      85,   19,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReRulerWidget[] = {
    "RE::ReRulerWidget\0\0_pos\0CursorChanged(int)\0"
    "ViewportChanged(int)\0OnCursorChanged(int)\0"
    "OnViewportChanged(int)\0"
};

const QMetaObject RE::ReRulerWidget::staticMetaObject = {
    { &ReBaseWidget<QLabel>::staticMetaObject, qt_meta_stringdata_RE__ReRulerWidget,
      qt_meta_data_RE__ReRulerWidget, 0 }
};

const QMetaObject *RE::ReRulerWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReRulerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReRulerWidget))
        return static_cast<void*>(const_cast< ReRulerWidget*>(this));
    return ReBaseWidget<QLabel>::qt_metacast(_clname);
}

int RE::ReRulerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QLabel>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CursorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: ViewportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnCursorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: OnViewportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void RE::ReRulerWidget::CursorChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReRulerWidget::ViewportChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
