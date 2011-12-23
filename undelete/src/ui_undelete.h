/********************************************************************************
** Form generated from reading UI file 'undelete.ui'
**
** Created: Thu Dec 22 17:42:38 2011
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNDELETE_H
#define UI_UNDELETE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UndeleteWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;

    void setupUi(QWidget *UndeleteWidget)
    {
        if (UndeleteWidget->objectName().isEmpty())
            UndeleteWidget->setObjectName(QString::fromUtf8("UndeleteWidget"));
        UndeleteWidget->resize(662, 368);
        horizontalLayout = new QHBoxLayout(UndeleteWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(UndeleteWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidget = new QTreeWidget(UndeleteWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(UndeleteWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(UndeleteWidget);

        QMetaObject::connectSlotsByName(UndeleteWidget);
    } // setupUi

    void retranslateUi(QWidget *UndeleteWidget)
    {
        UndeleteWidget->setWindowTitle(QApplication::translate("UndeleteWidget", "Undelete", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("UndeleteWidget", "Undelete", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UndeleteWidget: public Ui_UndeleteWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNDELETE_H
