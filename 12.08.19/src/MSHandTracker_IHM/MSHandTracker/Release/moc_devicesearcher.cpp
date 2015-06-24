/****************************************************************************
** Meta object code from reading C++ file 'devicesearcher.h'
**
** Created: Sun 19. Aug 18:49:33 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../devicesearcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicesearcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeviceSearcher[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      27,   15,   15,   15, 0x05,
      41,   15,   15,   15, 0x05,
      59,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DeviceSearcher[] = {
    "DeviceSearcher\0\0noDevice()\0deviceFound()\0"
    "trackingStarted()\0trackingStopped()\0"
    "initiateSearch()\0"
};

void DeviceSearcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DeviceSearcher *_t = static_cast<DeviceSearcher *>(_o);
        switch (_id) {
        case 0: _t->noDevice(); break;
        case 1: _t->deviceFound(); break;
        case 2: _t->trackingStarted(); break;
        case 3: _t->trackingStopped(); break;
        case 4: _t->initiateSearch(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DeviceSearcher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DeviceSearcher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeviceSearcher,
      qt_meta_data_DeviceSearcher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeviceSearcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeviceSearcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeviceSearcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceSearcher))
        return static_cast<void*>(const_cast< DeviceSearcher*>(this));
    return QObject::qt_metacast(_clname);
}

int DeviceSearcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DeviceSearcher::noDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DeviceSearcher::deviceFound()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DeviceSearcher::trackingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DeviceSearcher::trackingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
