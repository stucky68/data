/****************************************************************************
** Meta object code from reading C++ file 'cstudent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QtStudent/cstudent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cstudent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CStudent_t {
    QByteArrayData data[9];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CStudent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CStudent_t qt_meta_stringdata_CStudent = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CStudent"
QT_MOC_LITERAL(1, 9, 17), // "ClassManage_Click"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 18), // "CourseManage_Click"
QT_MOC_LITERAL(4, 47, 19), // "StudentManage_Click"
QT_MOC_LITERAL(5, 67, 20), // "ChangePassword_Click"
QT_MOC_LITERAL(6, 88, 18), // "SystemManage_Click"
QT_MOC_LITERAL(7, 107, 25), // "StudentCourseManage_Click"
QT_MOC_LITERAL(8, 133, 13) // "ExitSys_Click"

    },
    "CStudent\0ClassManage_Click\0\0"
    "CourseManage_Click\0StudentManage_Click\0"
    "ChangePassword_Click\0SystemManage_Click\0"
    "StudentCourseManage_Click\0ExitSys_Click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CStudent[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CStudent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CStudent *_t = static_cast<CStudent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ClassManage_Click(); break;
        case 1: _t->CourseManage_Click(); break;
        case 2: _t->StudentManage_Click(); break;
        case 3: _t->ChangePassword_Click(); break;
        case 4: _t->SystemManage_Click(); break;
        case 5: _t->StudentCourseManage_Click(); break;
        case 6: _t->ExitSys_Click(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CStudent::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CStudent.data,
      qt_meta_data_CStudent,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CStudent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CStudent::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CStudent.stringdata0))
        return static_cast<void*>(const_cast< CStudent*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CStudent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
