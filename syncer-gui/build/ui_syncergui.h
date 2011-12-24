/********************************************************************************
** Form generated from reading UI file 'syncergui.ui'
**
** Created: Sat Dec 24 00:50:04 2011
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNCERGUI_H
#define UI_SYNCERGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SyncerGui
{
public:

    void setupUi(QWidget *SyncerGui)
    {
        if (SyncerGui->objectName().isEmpty())
            SyncerGui->setObjectName(QString::fromUtf8("SyncerGui"));
        SyncerGui->setWindowModality(Qt::WindowModal);
        SyncerGui->resize(400, 300);

        retranslateUi(SyncerGui);

        QMetaObject::connectSlotsByName(SyncerGui);
    } // setupUi

    void retranslateUi(QWidget *SyncerGui)
    {
        SyncerGui->setWindowTitle(QApplication::translate("SyncerGui", "SyncerGui", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SyncerGui: public Ui_SyncerGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCERGUI_H
