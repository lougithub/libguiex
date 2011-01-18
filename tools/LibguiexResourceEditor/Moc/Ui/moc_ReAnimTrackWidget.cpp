/****************************************************************************
** Meta object code from reading C++ file 'ReAnimTrackWidget.h'
**
** Created: Tue Jan 18 22:23:32 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReAnimTrackWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReAnimTrackWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReAnimTrackWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      31,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   50,   22,   22, 0x0a,
      79,   22,   22,   22, 0x0a,
      95,   22,   22,   22, 0x0a,
     116,  111,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReAnimTrackWidget[] = {
    "RE::ReAnimTrackWidget\0\0_cursor\0"
    "DataChangedAt(int)\0_point\0"
    "OnContextMenu(QPoint)\0OnCreateFrame()\0"
    "OnDeleteFrame()\0_pos\0OnViewportChanged(int)\0"
};

const QMetaObject RE::ReAnimTrackWidget::staticMetaObject = {
    { &ReRulerWidget::staticMetaObject, qt_meta_stringdata_RE__ReAnimTrackWidget,
      qt_meta_data_RE__ReAnimTrackWidget, 0 }
};

const QMetaObject *RE::ReAnimTrackWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReAnimTrackWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReAnimTrackWidget))
        return static_cast<void*>(const_cast< ReAnimTrackWidget*>(this));
    if (!strcmp(_clname, "ReModelBase<ReAnimFrameWidget>"))
        return static_cast< ReModelBase<ReAnimFrameWidget>*>(const_cast< ReAnimTrackWidget*>(this));
    return ReRulerWidget::qt_metacast(_clname);
}

int RE::ReAnimTrackWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReRulerWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DataChangedAt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: OnCreateFrame(); break;
        case 3: OnDeleteFrame(); break;
        case 4: OnViewportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RE::ReAnimTrackWidget::DataChangedAt(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
