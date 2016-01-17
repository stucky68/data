/****************************************************************************
** Meta object code from reading C++ file 'CStudentManage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QtStudent/CStudentManage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CStudentManage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CStudentManage_t {
    QByteArrayData data[9];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CStudentManage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CStudentManage_t qt_meta_stringdata_CStudentManage = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CStudentManage"
QT_MOC_LITERAL(1, 15, 21), // "slotAddStudentClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 21), // "slotDelStudentClicked"
QT_MOC_LITERAL(4, 60, 21), // "slotChgStudentClicked"
QT_MOC_LITERAL(5, 82, 21), // "slotExitManageClicked"
QT_MOC_LITERAL(6, 104, 20), // "slotModelDataChanged"
QT_MOC_LITERAL(7, 125, 12), // "QVector<int>"
QT_MOC_LITERAL(8, 138, 19) // "slotEditTextChanged"

    },
    "CStudentManage\0slotAddStudentClicked\0"
    "\0slotDelStudentClicked\0slotChgStudentClicked\0"
    "slotExitManageClicked\0slotModelDataChanged\0"
    "QVector<int>\0slotEditTextChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CStudentManage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    3,   48,    2, 0x0a /* Public */,
       8,    1,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex, 0x80000000 | 7,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void CStudentManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CStudentManage *_t = static_cast<CStudentManage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotAddStudentClicked(); break;
        case 1: _t->slotDelStudentClicked(); break;
        case 2: _t->slotChgStudentClicked(); break;
        case 3: _t->slotExitManageClicked(); break;
        case 4: _t->slotModelDataChanged((*reinterpret_cast< QModelIndex(*)>(_a[1])),(*reinterpret_cast< QModelIndex(*)>(_a[2])),(*reinterpret_cast< QVector<int>(*)>(_a[3]))); break;
        case 5: _t->slotEditTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        }
    }
}

const QMetaObject CStudentManage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStudentManage.data,
      qt_meta_data_CStudentManage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CStudentManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CStudentManage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CStudentManage.stringdata0))
        return static_cast<void*>(const_cast< CStudentManage*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStudentManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
