/****************************************************************************
** Meta object code from reading C++ file 'ReMainWindow.h'
**
** Created: Wed Dec 29 19:18:20 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ui/ReMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RE__ReMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   18,   17,   17, 0x0a,
      37,   17,   17,   17, 0x2a,
      44,   17,   17,   17, 0x09,
      63,   17,   17,   17, 0x09,
      80,   17,   17,   17, 0x09,
     100,   17,   17,   17, 0x09,
     118,   17,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_RE__ReMainWindow[] = {
    "RE::ReMainWindow\0\0_delta\0Tick(qreal)\0"
    "Tick()\0OnGotoClipEditor()\0OnGotoAsEditor()\0"
    "OnGotoImageEditor()\0OnAboutSoftware()\0"
    "OnAboutAuthor()\0"
};

const QMetaObject RE::ReMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RE__ReMainWindow,
      qt_meta_data_RE__ReMainWindow, 0 }
};

const QMetaObject *RE::ReMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *RE::ReMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RE__ReMainWindow))
        return static_cast<void*>(const_cast< ReMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RE::ReMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Tick((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: Tick(); break;
        case 2: OnGotoClipEditor(); break;
        case 3: OnGotoAsEditor(); break;
        case 4: OnGotoImageEditor(); break;
        case 5: OnAboutSoftware(); break;
        case 6: OnAboutAuthor(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
