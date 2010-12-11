/****************************************************************************
** Meta object code from reading C++ file 'ReTrackWidget.h'
**
** Created: Sat Dec 11 19:33:36 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReTrackWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReTrackWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReTrackWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      26,   19,   18,   18, 0x05,
      63,   19,   18,   18, 0x05,
     120,  100,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     178,  173,   18,   18, 0x0a,
     208,  201,   18,   18, 0x0a,
     230,   18,   18,   18, 0x0a,
     246,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReTrackWidget[] = {
    "RE::ReTrackWidget\0\0_track\0"
    "CreateFrameRequested(ReTrackWidget*)\0"
    "DeleteFrameRequested(ReTrackWidget*)\0"
    "_track,_frame,_time\0"
    "FrameMoved(ReTrackWidget*,ReTrackFrameWidget*,qreal)\0"
    "_pos\0OnViewportChanged(int)\0_point\0"
    "OnContextMenu(QPoint)\0OnCreateFrame()\0"
    "OnDeleteFrame()\0"
};

const QMetaObject RE::ReTrackWidget::staticMetaObject = {
    { &ReRulerWidget::staticMetaObject, qt_meta_stringdata_RE__ReTrackWidget,
      qt_meta_data_RE__ReTrackWidget, 0 }
};

const QMetaObject *RE::ReTrackWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReTrackWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReTrackWidget))
        return static_cast<void*>(const_cast< ReTrackWidget*>(this));
    if (!strcmp(_clname, "ReModelBase<ReTrackFrameWidget>"))
        return static_cast< ReModelBase<ReTrackFrameWidget>*>(const_cast< ReTrackWidget*>(this));
    return ReRulerWidget::qt_metacast(_clname);
}

int RE::ReTrackWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReRulerWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CreateFrameRequested((*reinterpret_cast< ReTrackWidget*(*)>(_a[1]))); break;
        case 1: DeleteFrameRequested((*reinterpret_cast< ReTrackWidget*(*)>(_a[1]))); break;
        case 2: FrameMoved((*reinterpret_cast< ReTrackWidget*(*)>(_a[1])),(*reinterpret_cast< ReTrackFrameWidget*(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3]))); break;
        case 3: OnViewportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: OnCreateFrame(); break;
        case 6: OnDeleteFrame(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RE::ReTrackWidget::CreateFrameRequested(ReTrackWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RE::ReTrackWidget::DeleteFrameRequested(ReTrackWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RE::ReTrackWidget::FrameMoved(ReTrackWidget * _t1, ReTrackFrameWidget * _t2, qreal _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
