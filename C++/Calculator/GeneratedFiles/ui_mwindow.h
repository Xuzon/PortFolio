/********************************************************************************
** Form generated from reading UI file 'mwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MWINDOW_H
#define UI_MWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MWindow
{
public:
    QWidget *centralWidget;
    QPushButton *BNumeric1;
    QPushButton *BNumeric2;
    QPushButton *BNumeric3;
    QPushButton *BNumeric6;
    QPushButton *BNumeric4;
    QPushButton *BNumeric5;
    QPushButton *BNumeric9;
    QPushButton *BNumeric8;
    QPushButton *BNumeric7;
    QPushButton *BEquals;
    QPushButton *BPlus;
    QPushButton *BSubstract;
    QPushButton *BMultiply;
    QPushButton *BDivide;
    QLabel *lcd;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MWindow)
    {
        if (MWindow->objectName().isEmpty())
            MWindow->setObjectName(QStringLiteral("MWindow"));
        MWindow->resize(223, 357);
        centralWidget = new QWidget(MWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BNumeric1 = new QPushButton(centralWidget);
        BNumeric1->setObjectName(QStringLiteral("BNumeric1"));
        BNumeric1->setGeometry(QRect(40, 210, 31, 31));
        BNumeric2 = new QPushButton(centralWidget);
        BNumeric2->setObjectName(QStringLiteral("BNumeric2"));
        BNumeric2->setGeometry(QRect(80, 210, 31, 31));
        BNumeric3 = new QPushButton(centralWidget);
        BNumeric3->setObjectName(QStringLiteral("BNumeric3"));
        BNumeric3->setGeometry(QRect(120, 210, 31, 31));
        BNumeric6 = new QPushButton(centralWidget);
        BNumeric6->setObjectName(QStringLiteral("BNumeric6"));
        BNumeric6->setGeometry(QRect(120, 170, 31, 31));
        BNumeric4 = new QPushButton(centralWidget);
        BNumeric4->setObjectName(QStringLiteral("BNumeric4"));
        BNumeric4->setGeometry(QRect(40, 170, 31, 31));
        BNumeric5 = new QPushButton(centralWidget);
        BNumeric5->setObjectName(QStringLiteral("BNumeric5"));
        BNumeric5->setGeometry(QRect(80, 170, 31, 31));
        BNumeric9 = new QPushButton(centralWidget);
        BNumeric9->setObjectName(QStringLiteral("BNumeric9"));
        BNumeric9->setGeometry(QRect(120, 130, 31, 31));
        BNumeric8 = new QPushButton(centralWidget);
        BNumeric8->setObjectName(QStringLiteral("BNumeric8"));
        BNumeric8->setGeometry(QRect(80, 130, 31, 31));
        BNumeric7 = new QPushButton(centralWidget);
        BNumeric7->setObjectName(QStringLiteral("BNumeric7"));
        BNumeric7->setGeometry(QRect(40, 130, 31, 31));
        BEquals = new QPushButton(centralWidget);
        BEquals->setObjectName(QStringLiteral("BEquals"));
        BEquals->setGeometry(QRect(170, 170, 31, 71));
        BPlus = new QPushButton(centralWidget);
        BPlus->setObjectName(QStringLiteral("BPlus"));
        BPlus->setGeometry(QRect(170, 130, 31, 31));
        BSubstract = new QPushButton(centralWidget);
        BSubstract->setObjectName(QStringLiteral("BSubstract"));
        BSubstract->setGeometry(QRect(170, 90, 31, 31));
        BMultiply = new QPushButton(centralWidget);
        BMultiply->setObjectName(QStringLiteral("BMultiply"));
        BMultiply->setGeometry(QRect(120, 90, 31, 31));
        BDivide = new QPushButton(centralWidget);
        BDivide->setObjectName(QStringLiteral("BDivide"));
        BDivide->setGeometry(QRect(80, 90, 31, 31));
        lcd = new QLabel(centralWidget);
        lcd->setObjectName(QStringLiteral("lcd"));
        lcd->setGeometry(QRect(46, 12, 141, 61));
        lcd->setScaledContents(false);
        lcd->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lcd->setWordWrap(false);
        MWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 223, 21));
        MWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MWindow->setStatusBar(statusBar);

        retranslateUi(MWindow);
        QObject::connect(BNumeric6, SIGNAL(clicked()), MWindow, SLOT(BSixPressed()));
        QObject::connect(BNumeric3, SIGNAL(clicked()), MWindow, SLOT(BThreePressed()));
        QObject::connect(BNumeric2, SIGNAL(clicked()), MWindow, SLOT(BTwoPressed()));
        QObject::connect(BNumeric1, SIGNAL(clicked()), MWindow, SLOT(BOnePressed()));
        QObject::connect(BNumeric7, SIGNAL(clicked()), MWindow, SLOT(BSevenPressed()));
        QObject::connect(BEquals, SIGNAL(clicked()), MWindow, SLOT(BEqualsPressed()));
        QObject::connect(BPlus, SIGNAL(clicked()), MWindow, SLOT(BAddPressed()));
        QObject::connect(BSubstract, SIGNAL(clicked()), MWindow, SLOT(BSubstractPressed()));
        QObject::connect(BDivide, SIGNAL(clicked()), MWindow, SLOT(BDividePressed()));
        QObject::connect(BNumeric8, SIGNAL(clicked()), MWindow, SLOT(BEightPressed()));
        QObject::connect(BNumeric9, SIGNAL(clicked()), MWindow, SLOT(BNinePressed()));
        QObject::connect(BMultiply, SIGNAL(clicked()), MWindow, SLOT(BMultiplyPressed()));
        QObject::connect(BNumeric5, SIGNAL(clicked()), MWindow, SLOT(BFivePressed()));
        QObject::connect(BNumeric4, SIGNAL(clicked()), MWindow, SLOT(BFourPressed()));

        QMetaObject::connectSlotsByName(MWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MWindow)
    {
        MWindow->setWindowTitle(QApplication::translate("MWindow", "MWindow", Q_NULLPTR));
        BNumeric1->setText(QApplication::translate("MWindow", "1", Q_NULLPTR));
        BNumeric2->setText(QApplication::translate("MWindow", "2", Q_NULLPTR));
        BNumeric3->setText(QApplication::translate("MWindow", "3", Q_NULLPTR));
        BNumeric6->setText(QApplication::translate("MWindow", "6", Q_NULLPTR));
        BNumeric4->setText(QApplication::translate("MWindow", "4", Q_NULLPTR));
        BNumeric5->setText(QApplication::translate("MWindow", "5", Q_NULLPTR));
        BNumeric9->setText(QApplication::translate("MWindow", "9", Q_NULLPTR));
        BNumeric8->setText(QApplication::translate("MWindow", "8", Q_NULLPTR));
        BNumeric7->setText(QApplication::translate("MWindow", "7", Q_NULLPTR));
        BEquals->setText(QApplication::translate("MWindow", "=", Q_NULLPTR));
        BPlus->setText(QApplication::translate("MWindow", "+", Q_NULLPTR));
        BSubstract->setText(QApplication::translate("MWindow", "-", Q_NULLPTR));
        BMultiply->setText(QApplication::translate("MWindow", "*", Q_NULLPTR));
        BDivide->setText(QApplication::translate("MWindow", "/", Q_NULLPTR));
        lcd->setText(QApplication::translate("MWindow", "0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MWindow: public Ui_MWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MWINDOW_H
