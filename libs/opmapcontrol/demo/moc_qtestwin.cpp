/****************************************************************************
** Meta object code from reading C++ file 'qtestwin.h'
**
** Created: Tue Apr 14 21:07:28 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtestwin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtestwin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTestWin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      30,   24,    9,    9, 0x0a,
      58,   50,    9,    9, 0x0a,
      81,   75,    9,    9, 0x0a,
     120,   75,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTestWin[] = {
    "QTestWin\0\0actFatchMap()\0index\0"
    "maptypeChanged(int)\0newZoom\0"
    "zoomChanged(int)\0event\0"
    "mapWidget_mouseMoveEvent(QMouseEvent*)\0"
    "timerEvent(QTimerEvent*)\0"
};

void QTestWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTestWin *_t = static_cast<QTestWin *>(_o);
        switch (_id) {
        case 0: _t->actFatchMap(); break;
        case 1: _t->maptypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zoomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->mapWidget_mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTestWin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTestWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QTestWin,
      qt_meta_data_QTestWin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTestWin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTestWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTestWin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTestWin))
        return static_cast<void*>(const_cast< QTestWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int QTestWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
