/****************************************************************************
** Meta object code from reading C++ file 'ReAnimEntityWidget.h'
**
** Created: Thu Jan 6 22:39:33 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimEntityWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimEntityWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimEntityWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,
      48,   45,   23,   23, 0x05,
     119,   45,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     195,  188,   23,   23, 0x0a,
     228,  217,   23,   23, 0x0a,
     244,   23,   23,   23, 0x0a,
     268,   23,   23,   23, 0x0a,
     289,   23,   23,   23, 0x0a,
     307,   23,   23,   23, 0x0a,
     330,  325,   23,   23, 0x0a,
     364,  357,   23,   23, 0x0a,
     396,  389,   23,   23, 0x0a,
     420,  357,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimEntityWidget[] = {
    "RE::ReAnimEntityWidget\0\0TotalHeightChanged()\0"
    ",,\0"
    "EditStarted(ReAnimEntityWidget*,ReAnimTrackWidget*,ReAnimFrameWidget*)\0"
    "EditEnded(ReAnimEntityWidget*,ReAnimTrackWidget*,ReAnimFrameWidget*)\0"
    "_point\0OnContextMenu(QPoint)\0_isChecked\0"
    "OnToggled(bool)\0OnNewTranslationTrack()\0"
    "OnNewRotationTrack()\0OnNewScaleTrack()\0"
    "OnNewAlphaTrack()\0_pos\0"
    "OnPositionChanged(QPointF)\0_delta\0"
    "OnRotationChanged(qreal)\0_scale\0"
    "OnScaleChanged(QPointF)\0OnAlphaChanged(qreal)\0"
};

const QMetaObject RE::ReAnimEntityWidget::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReAnimEntityWidget,
      qt_meta_data_RE__ReAnimEntityWidget, 0 }
};

const QMetaObject *RE::ReAnimEntityWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimEntityWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimEntityWidget))
        return static_cast<void*>(const_cast< ReAnimEntityWidget*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReAnimEntityWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TotalHeightChanged(); break;
        case 1: EditStarted((*reinterpret_cast< ReAnimEntityWidget*(*)>(_a[1])),(*reinterpret_cast< ReAnimTrackWidget*(*)>(_a[2])),(*reinterpret_cast< ReAnimFrameWidget*(*)>(_a[3]))); break;
        case 2: EditEnded((*reinterpret_cast< ReAnimEntityWidget*(*)>(_a[1])),(*reinterpret_cast< ReAnimTrackWidget*(*)>(_a[2])),(*reinterpret_cast< ReAnimFrameWidget*(*)>(_a[3]))); break;
        case 3: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: OnToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: OnNewTranslationTrack(); break;
        case 6: OnNewRotationTrack(); break;
        case 7: OnNewScaleTrack(); break;
        case 8: OnNewAlphaTrack(); break;
        case 9: OnPositionChanged((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 10: OnRotationChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 11: OnScaleChanged((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 12: OnAlphaChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimEntityWidget::TotalHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void RE::ReAnimEntityWidget::EditStarted(ReAnimEntityWidget * _t1, ReAnimTrackWidget * _t2, ReAnimFrameWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RE::ReAnimEntityWidget::EditEnded(ReAnimEntityWidget * _t1, ReAnimTrackWidget * _t2, ReAnimFrameWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
