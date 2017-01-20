/****************************************************************************
** Meta object code from reading C++ file 'mwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MWindow_t {
    QByteArrayData data[16];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MWindow_t qt_meta_stringdata_MWindow = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MWindow"
QT_MOC_LITERAL(1, 8, 11), // "BOnePressed"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "BTwoPressed"
QT_MOC_LITERAL(4, 33, 13), // "BThreePressed"
QT_MOC_LITERAL(5, 47, 12), // "BFourPressed"
QT_MOC_LITERAL(6, 60, 12), // "BFivePressed"
QT_MOC_LITERAL(7, 73, 11), // "BSixPressed"
QT_MOC_LITERAL(8, 85, 13), // "BSevenPressed"
QT_MOC_LITERAL(9, 99, 13), // "BEightPressed"
QT_MOC_LITERAL(10, 113, 12), // "BNinePressed"
QT_MOC_LITERAL(11, 126, 11), // "BAddPressed"
QT_MOC_LITERAL(12, 138, 17), // "BSubstractPressed"
QT_MOC_LITERAL(13, 156, 16), // "BMultiplyPressed"
QT_MOC_LITERAL(14, 173, 14), // "BDividePressed"
QT_MOC_LITERAL(15, 188, 14) // "BEqualsPressed"

    },
    "MWindow\0BOnePressed\0\0BTwoPressed\0"
    "BThreePressed\0BFourPressed\0BFivePressed\0"
    "BSixPressed\0BSevenPressed\0BEightPressed\0"
    "BNinePressed\0BAddPressed\0BSubstractPressed\0"
    "BMultiplyPressed\0BDividePressed\0"
    "BEqualsPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MWindow *_t = static_cast<MWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->BOnePressed(); break;
        case 1: _t->BTwoPressed(); break;
        case 2: _t->BThreePressed(); break;
        case 3: _t->BFourPressed(); break;
        case 4: _t->BFivePressed(); break;
        case 5: _t->BSixPressed(); break;
        case 6: _t->BSevenPressed(); break;
        case 7: _t->BEightPressed(); break;
        case 8: _t->BNinePressed(); break;
        case 9: _t->BAddPressed(); break;
        case 10: _t->BSubstractPressed(); break;
        case 11: _t->BMultiplyPressed(); break;
        case 12: _t->BDividePressed(); break;
        case 13: _t->BEqualsPressed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MWindow.data,
      qt_meta_data_MWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MWindow.stringdata0))
        return static_cast<void*>(const_cast< MWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
