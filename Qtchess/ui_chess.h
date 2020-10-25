/********************************************************************************
** Form generated from reading UI file 'chess.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESS_H
#define UI_CHESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chess
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Chess)
    {
        if (Chess->objectName().isEmpty())
            Chess->setObjectName(QString::fromUtf8("Chess"));
        Chess->resize(400, 300);
        centralWidget = new QWidget(Chess);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Chess->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Chess);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 26));
        Chess->setMenuBar(menuBar);
        statusBar = new QStatusBar(Chess);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Chess->setStatusBar(statusBar);

        retranslateUi(Chess);

        QMetaObject::connectSlotsByName(Chess);
    } // setupUi

    void retranslateUi(QMainWindow *Chess)
    {
        Chess->setWindowTitle(QApplication::translate("Chess", "Chess", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chess: public Ui_Chess {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESS_H
