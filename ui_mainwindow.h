/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QCustomPlot *signalPlot;
    QCustomPlot *thermPlot;
    QCustomPlot *metricPlot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setStyleSheet(QStringLiteral("background-color:white;"));
        MainWindow->resize(1098, 786);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(11);
        font.setWeight(50);

        QApplication::setFont(font);

        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(9, 9, -1, -1);

        signalPlot = new QCustomPlot(centralWidget);
        signalPlot->setObjectName(QStringLiteral("signalPlot"));
        signalPlot->setMaximumSize(QSize(960, 960));
        gridLayout->addWidget(signalPlot, 0, 0, 1, 1);

        thermPlot = new QCustomPlot(centralWidget);
        thermPlot->setObjectName(QStringLiteral("thermPlot"));
        thermPlot->setMaximumSize(QSize(270, 540));
        gridLayout->addWidget(thermPlot, 0, 1, 1, 1);

        metricPlot = new QCustomPlot(centralWidget);
        metricPlot->setObjectName(QStringLiteral("metricPlot"));
        metricPlot->setMaximumSize(QSize(960, 540));
        gridLayout->addWidget(metricPlot, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BCIVibe demos", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
