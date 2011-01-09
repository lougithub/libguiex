/****************************************************************************
** Meta object code from reading C++ file 'ReAnimGraphicsScene.h'
**
** Created: Sun Jan 9 22:07:44 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimGraphicsScene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimGraphicsScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimGraphicsScene[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      31,   25,   24,   24, 0x05,
      62,   25,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      95,   24,   24,   24, 0x0a,
     107,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimGraphicsScene[] = {
    "RE::ReAnimGraphicsScene\0\0_item\0"
    "ItemAdded(ReAnimGraphicsItem*)\0"
    "ItemDeleted(ReAnimGraphicsItem*)\0"
    "OnAddItem()\0OnDeleteItem()\0"
};

const QMetaObject RE::ReAnimGraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_RE__ReAnimGraphicsScene,
      qt_meta_data_RE__ReAnimGraphicsScene, 0 }
};

const QMetaObject *RE::ReAnimGraphicsScene::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimGraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimGraphicsScene))
        return static_cast<void*>(const_cast< ReAnimGraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int RE::ReAnimGraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ItemAdded((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 1: ItemDeleted((*reinterpret_cast< ReAnimGraphicsItem*(*)>(_a[1]))); break;
        case 2: OnAddItem(); break;
        case 3: OnDeleteItem(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimGraphicsScene::ItemAdded(ReAnimGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReAnimGraphicsScene::ItemDeleted(ReAnimGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
