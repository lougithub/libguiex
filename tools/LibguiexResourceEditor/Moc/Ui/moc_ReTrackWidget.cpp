/****************************************************************************
** Meta object code from reading C++ file 'ReTrackWidget.h'
**
** Created: Mon Nov 29 22:10:35 2010
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
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x0a,
      54,   47,   18,   18, 0x0a,
      76,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReTrackWidget[] = {
    "RE::ReTrackWidget\0\0_pos\0OnViewportChanged(int)\0"
    "_point\0OnContextMenu(QPoint)\0OnNewFrame()\0"
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
        case 0: OnViewportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: OnNewFrame(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
