/****************************************************************************
** Meta object code from reading C++ file 'MapWidget.h'
**
** Created: Tue Apr 14 21:07:27 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MapWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapWidget[] = {

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
      11,   10,   10,   10, 0x0a,
      40,   10,   10,   10, 0x0a,
      66,   10,   10,   10, 0x0a,
      89,   10,   10,   10, 0x0a,
     119,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapWidget[] = {
    "MapWidget\0\0actMapType_GoogleSatellate()\0"
    "actMapType_GoogleHybrid()\0"
    "actMapType_GoogleMap()\0"
    "actMapAccess_ServerAndCache()\0"
    "actMapAccess_Cache()\0"
};

void MapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MapWidget *_t = static_cast<MapWidget *>(_o);
        switch (_id) {
        case 0: _t->actMapType_GoogleSatellate(); break;
        case 1: _t->actMapType_GoogleHybrid(); break;
        case 2: _t->actMapType_GoogleMap(); break;
        case 3: _t->actMapAccess_ServerAndCache(); break;
        case 4: _t->actMapAccess_Cache(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MapWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MapWidget::staticMetaObject = {
    { &mapcontrol::OPMapWidget::staticMetaObject, qt_meta_stringdata_MapWidget,
      qt_meta_data_MapWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapWidget))
        return static_cast<void*>(const_cast< MapWidget*>(this));
    typedef mapcontrol::OPMapWidget QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int MapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef mapcontrol::OPMapWidget QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
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
