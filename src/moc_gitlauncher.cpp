/****************************************************************************
** Meta object code from reading C++ file 'gitlauncher.h'
**
** Created: Thu Dec 15 20:45:57 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gitlauncher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gitlauncher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GitLauncher[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x0a,
      39,   13,   12,   12, 0x0a,
      65,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GitLauncher[] = {
    "GitLauncher\0\0path\0fileChanged(QString)\0"
    "directoryChanged(QString)\0checkForUpdate()\0"
};

const QMetaObject GitLauncher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GitLauncher,
      qt_meta_data_GitLauncher, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GitLauncher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GitLauncher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GitLauncher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GitLauncher))
        return static_cast<void*>(const_cast< GitLauncher*>(this));
    return QObject::qt_metacast(_clname);
}

int GitLauncher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fileChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: directoryChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: checkForUpdate(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
