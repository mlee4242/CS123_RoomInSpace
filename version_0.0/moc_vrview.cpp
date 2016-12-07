/****************************************************************************
** Meta object code from reading C++ file 'vrview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/vrview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vrview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VRView_t {
    QByteArrayData data[11];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VRView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VRView_t qt_meta_stringdata_VRView = {
    {
QT_MOC_LITERAL(0, 0, 6), // "VRView"
QT_MOC_LITERAL(1, 7, 15), // "framesPerSecond"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "deviceIdentifier"
QT_MOC_LITERAL(4, 41, 9), // "frameSwap"
QT_MOC_LITERAL(5, 51, 13), // "statusMessage"
QT_MOC_LITERAL(6, 65, 15), // "updateFramerate"
QT_MOC_LITERAL(7, 81, 8), // "shutdown"
QT_MOC_LITERAL(8, 90, 12), // "debugMessage"
QT_MOC_LITERAL(9, 103, 19), // "QOpenGLDebugMessage"
QT_MOC_LITERAL(10, 123, 7) // "message"

    },
    "VRView\0framesPerSecond\0\0deviceIdentifier\0"
    "frameSwap\0statusMessage\0updateFramerate\0"
    "shutdown\0debugMessage\0QOpenGLDebugMessage\0"
    "message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VRView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    1,   52,    2, 0x06 /* Public */,
       4,    0,   55,    2, 0x06 /* Public */,
       5,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   59,    2, 0x09 /* Protected */,
       7,    0,   60,    2, 0x09 /* Protected */,
       8,    1,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void VRView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VRView *_t = static_cast<VRView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->framesPerSecond((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->deviceIdentifier((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->frameSwap(); break;
        case 3: _t->statusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->updateFramerate(); break;
        case 5: _t->shutdown(); break;
        case 6: _t->debugMessage((*reinterpret_cast< QOpenGLDebugMessage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QOpenGLDebugMessage >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VRView::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VRView::framesPerSecond)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VRView::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VRView::deviceIdentifier)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VRView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VRView::frameSwap)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VRView::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VRView::statusMessage)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject VRView::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_VRView.data,
      qt_meta_data_VRView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VRView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VRView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VRView.stringdata0))
        return static_cast<void*>(const_cast< VRView*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_4_1_Core"))
        return static_cast< QOpenGLFunctions_4_1_Core*>(const_cast< VRView*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int VRView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void VRView::framesPerSecond(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VRView::deviceIdentifier(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VRView::frameSwap()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void VRView::statusMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
