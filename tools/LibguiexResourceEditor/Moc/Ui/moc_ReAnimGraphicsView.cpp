/****************************************************************************
** Meta object code from reading C++ file 'ReAnimGraphicsView.h'
**
** Created: Thu Jan 6 22:46:20 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimGraphicsView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimGraphicsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimGraphicsView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      27,   24,   23,   23, 0x05,
      55,   52,   23,   23, 0x05,
      95,   80,   23,   23, 0x05,
     118,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     136,   24,   23,   23, 0x0a,
     167,   52,   23,   23, 0x0a,
     198,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimGraphicsView[] = {
    "RE::ReAnimGraphicsView\0\0_x\0"
    "SceneOriginXChanged(int)\0_y\0"
    "SceneOriginYChanged(int)\0_itemPosInView\0"
    "SceneItemMoved(QPoint)\0EditModeChanged()\0"
    "RequestChangeSceneOriginX(int)\0"
    "RequestChangeSceneOriginY(int)\0"
    "OnSceneSelectionChanged()\0"
};

const QMetaObject RE::ReAnimGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_RE__ReAnimGraphicsView,
      qt_meta_data_RE__ReAnimGraphicsView, 0 }
};

const QMetaObject *RE::ReAnimGraphicsView::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimGraphicsView))
        return static_cast<void*>(const_cast< ReAnimGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int RE::ReAnimGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SceneOriginXChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: SceneOriginYChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: SceneItemMoved((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: EditModeChanged(); break;
        case 4: RequestChangeSceneOriginX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: RequestChangeSceneOriginY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: OnSceneSelectionChanged(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimGraphicsView::SceneOriginXChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReAnimGraphicsView::SceneOriginYChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RE::ReAnimGraphicsView::SceneItemMoved(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RE::ReAnimGraphicsView::EditModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
