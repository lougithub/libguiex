/****************************************************************************
** Meta object code from reading C++ file 'ReEditorPanelWidget.h'
**
** Created: Tue Dec 21 20:44:50 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReEditorPanelWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReEditorPanelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReEditorPanelWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x0a,
      43,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReEditorPanelWidget[] = {
    "RE::ReEditorPanelWidget\0\0OnToggleSetting()\0"
    "OnSettingsChanged()\0"
};

const QMetaObject RE::ReEditorPanelWidget::staticMetaObject = {
    { &ReBaseWidget<QWidget>::staticMetaObject, qt_meta_stringdata_RE__ReEditorPanelWidget,
      qt_meta_data_RE__ReEditorPanelWidget, 0 }
};

const QMetaObject *RE::ReEditorPanelWidget::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReEditorPanelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReEditorPanelWidget))
        return static_cast<void*>(const_cast< ReEditorPanelWidget*>(this));
    return ReBaseWidget<QWidget>::qt_metacast(_clname);
}

int RE::ReEditorPanelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ReBaseWidget<QWidget>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnToggleSetting(); break;
        case 1: OnSettingsChanged(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
