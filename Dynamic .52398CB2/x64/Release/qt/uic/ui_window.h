/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_windowClass
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *windowClass)
    {
        if (windowClass->objectName().isEmpty())
            windowClass->setObjectName("windowClass");
        windowClass->resize(707, 485);
        centralWidget = new QWidget(windowClass);
        centralWidget->setObjectName("centralWidget");
        windowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(windowClass);
        statusBar->setObjectName("statusBar");
        windowClass->setStatusBar(statusBar);

        retranslateUi(windowClass);

        QMetaObject::connectSlotsByName(windowClass);
    } // setupUi

    void retranslateUi(QMainWindow *windowClass)
    {
        windowClass->setWindowTitle(QCoreApplication::translate("windowClass", "window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class windowClass: public Ui_windowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
