/****************************************************************************
** Meta object code from reading C++ file 'ReTrackPanelWidget.h'
**
** Created: Sat Dec 11 19:32:37 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReTrackPanelWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReTrackPanelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReTrackPanelWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      38,   23,   24,   23, 0x0a,
      62,   23,   24,   23, 0x0a,
      83,   23,   24,   23, 0x0a,
     101,   23,   24,   23, 0x0a,
     125,  119,   24,   23, 0x0a,
     155,  148,   23,   23, 0x0a,
     184,  177,   23,   23, 0x0a,
     223,  177,   23,   23, 0x0a,
     282,  262,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReTrackPanelWidget[] = {
    "RE::ReTrackPanelWidget\0\0ReTrackSuite*\0"
    "OnNewTranslationTrack()\0OnNewRotationTrack()\0"
    "OnNewScaleTrack()\0OnNewAlphaTrack()\0"
    "_type\0OnNewTrack(eTrackType)\0_point\0"
    "OnContextMenu(QPoint)\0_track\0"
    "OnCreateFrameRequested(ReTrackWidget*)\0"
    "OnDeleteFrameRequested(ReTrackWidget*)\0"
    "_track,_frame,_time\0"
    "OnFrameMoved(ReTrackWidget*,ReTrackFrameWidget*,qreal)\0"
};

const QMetaObject RE::ReTrackPanelWidget::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReTrackPanelWidget,
      qt_meta_data_RE__ReTrackPanelWidget, 0 }
};

const QMetaObject *RE::ReTrackPanelWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReTrackPanelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReTrackPanelWidget))
        return static_cast<void*>(const_cast< ReTrackPanelWidget*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReTrackPanelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { ReTrackSuite* _r = OnNewTranslationTrack();
            if (_a[0]) *reinterpret_cast< ReTrackSuite**>(_a[0]) = _r; }  break;
        case 1: { ReTrackSuite* _r = OnNewRotationTrack();
            if (_a[0]) *reinterpret_cast< ReTrackSuite**>(_a[0]) = _r; }  break;
        case 2: { ReTrackSuite* _r = OnNewScaleTrack();
            if (_a[0]) *reinterpret_cast< ReTrackSuite**>(_a[0]) = _r; }  break;
        case 3: { ReTrackSuite* _r = OnNewAlphaTrack();
            if (_a[0]) *reinterpret_cast< ReTrackSuite**>(_a[0]) = _r; }  break;
        case 4: { ReTrackSuite* _r = OnNewTrack((*reinterpret_cast< eTrackType(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< ReTrackSuite**>(_a[0]) = _r; }  break;
        case 5: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: OnCreateFrameRequested((*reinterpret_cast< ReTrackWidget*(*)>(_a[1]))); break;
        case 7: OnDeleteFrameRequested((*reinterpret_cast< ReTrackWidget*(*)>(_a[1]))); break;
        case 8: OnFrameMoved((*reinterpret_cast< ReTrackWidget*(*)>(_a[1])),(*reinterpret_cast< ReTrackFrameWidget*(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
