/********************************************************************************
** Form generated from reading UI file 'restore.ui'
**
** Created: Wed Dec 21 19:21:53 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTORE_H
#define UI_RESTORE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestoreWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;

    void setupUi(QWidget *RestoreWidget)
    {
        if (RestoreWidget->objectName().isEmpty())
            RestoreWidget->setObjectName(QString::fromUtf8("RestoreWidget"));
        RestoreWidget->resize(807, 609);
        horizontalLayout = new QHBoxLayout(RestoreWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView = new QTreeView(RestoreWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        horizontalLayout->addWidget(treeView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, -1, 0);
        treeWidget = new QTreeWidget(RestoreWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        pushButton = new QPushButton(RestoreWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(RestoreWidget);

        QMetaObject::connectSlotsByName(RestoreWidget);
    } // setupUi

    void retranslateUi(QWidget *RestoreWidget)
    {
        RestoreWidget->setWindowTitle(QApplication::translate("RestoreWidget", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("RestoreWidget", "Restore", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RestoreWidget: public Ui_RestoreWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTORE_H
