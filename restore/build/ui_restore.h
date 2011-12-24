/********************************************************************************
** Form generated from reading UI file 'restore.ui'
**
** Created: Fri Dec 23 22:42:48 2011
**      by: Qt User Interface Compiler version 4.8.0
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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestoreWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *upButton;
    QLineEdit *fileRootPath;
    QPushButton *goButton;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *restoreButton;

    void setupUi(QWidget *RestoreWidget)
    {
        if (RestoreWidget->objectName().isEmpty())
            RestoreWidget->setObjectName(QString::fromUtf8("RestoreWidget"));
        RestoreWidget->resize(662, 394);
        horizontalLayout = new QHBoxLayout(RestoreWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, -1, -1);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, 0, -1);
        upButton = new QToolButton(RestoreWidget);
        upButton->setObjectName(QString::fromUtf8("upButton"));

        horizontalLayout_3->addWidget(upButton);

        fileRootPath = new QLineEdit(RestoreWidget);
        fileRootPath->setObjectName(QString::fromUtf8("fileRootPath"));

        horizontalLayout_3->addWidget(fileRootPath);

        goButton = new QPushButton(RestoreWidget);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout_3->addWidget(goButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        treeView = new QTreeView(RestoreWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        verticalLayout_2->addWidget(treeView);


        horizontalLayout->addLayout(verticalLayout_2);

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
        restoreButton = new QPushButton(RestoreWidget);
        restoreButton->setObjectName(QString::fromUtf8("restoreButton"));

        horizontalLayout_2->addWidget(restoreButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(RestoreWidget);
        QObject::connect(fileRootPath, SIGNAL(editingFinished()), goButton, SLOT(click()));

        QMetaObject::connectSlotsByName(RestoreWidget);
    } // setupUi

    void retranslateUi(QWidget *RestoreWidget)
    {
        RestoreWidget->setWindowTitle(QApplication::translate("RestoreWidget", "Restore", 0, QApplication::UnicodeUTF8));
        upButton->setText(QApplication::translate("RestoreWidget", "UP", 0, QApplication::UnicodeUTF8));
        goButton->setText(QApplication::translate("RestoreWidget", "Go", 0, QApplication::UnicodeUTF8));
        restoreButton->setText(QApplication::translate("RestoreWidget", "Restore", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RestoreWidget: public Ui_RestoreWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTORE_H
