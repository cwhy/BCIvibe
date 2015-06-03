#ifndef UI_SIGNALPLOT_H
#define UI_SIGNALPLOT_H

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

class Ui_SignalPlot
{
public:
    QWidget *centralWidget;
    QCustomPlot *signalPlot;
    QGridLayout *gridLayout;

    void setupUi(QMainWindow *SignalPlotWindow)
    {
        if (SignalPlotWindow->objectName().isEmpty())
            SignalPlotWindow->setObjectName(QStringLiteral("SignalPlotWindow"));
        // SignalPlotWindow->setStyleSheet(QStringLiteral("background-color:white;"));

        SignalPlotWindow->setGeometry(0,0,960,540);
        SignalPlotWindow->setWindowFlags( Qt::FramelessWindowHint );

        centralWidget = new QWidget(SignalPlotWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));

        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        signalPlot = new QCustomPlot(centralWidget);
        signalPlot->setObjectName(QStringLiteral("signalPlot"));
        gridLayout->addWidget(signalPlot);

        SignalPlotWindow->setCentralWidget(centralWidget);
        retranslateUi(SignalPlotWindow);

        QMetaObject::connectSlotsByName(SignalPlotWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SignalPlotWindow)
    {
        SignalPlotWindow->setWindowTitle(QApplication::translate("SignalPlotWindow", "BCIVibe demos", 0));
    } // retranslateUi

};

namespace Ui {
    class SignalPlotWindow: public Ui_SignalPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
