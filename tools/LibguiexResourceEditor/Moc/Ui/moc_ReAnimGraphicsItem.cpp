/****************************************************************************
** Meta object code from reading C++ file 'ReAnimGraphicsItem.h'
**
** Created: Thu Jan 6 22:16:22 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimGraphicsItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimGraphicsItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimGraphicsItem[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      29,   24,   23,   23, 0x05,
      61,   54,   23,   23, 0x05,
      91,   84,   23,   23, 0x05,
     113,   54,   23,   23, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimGraphicsItem[] = {
    "RE::ReAnimGraphicsItem\0\0_pos\0"
    "PositionChanged(QPointF)\0_delta\0"
    "RotationChanged(qreal)\0_scale\0"
    "ScaleChanged(QPointF)\0AlphaChanged(qreal)\0"
};

const QMetaObject RE::ReAnimGraphicsItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RE__ReAnimGraphicsItem,
      qt_meta_data_RE__ReAnimGraphicsItem, 0 }
};

const QMetaObject *RE::ReAnimGraphicsItem::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimGraphicsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimGraphicsItem))
        return static_cast<void*>(const_cast< ReAnimGraphicsItem*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< ReAnimGraphicsItem*>(this));
    return QObject::qt_metacast(_clname);
}

int RE::ReAnimGraphicsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PositionChanged((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 1: RotationChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: ScaleChanged((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 3: AlphaChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimGraphicsItem::PositionChanged(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReAnimGraphicsItem::RotationChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RE::ReAnimGraphicsItem::ScaleChanged(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RE::ReAnimGraphicsItem::AlphaChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
