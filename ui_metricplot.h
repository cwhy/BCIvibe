#ifndef UI_METRICPLOT_H
#define UI_METRICPLOT_H

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

class Ui_MetricPlot
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QCustomPlot *metricPlot;
    QCustomPlot *thermPlot;

    void setupUi(QMainWindow *MetricPlotWindow)
    {
        if (MetricPlotWindow->objectName().isEmpty())
            MetricPlotWindow->setObjectName(QStringLiteral("MetricPlotWindow"));
        // MetricPlotWindow->setStyleSheet(QStringLiteral("background-color:white;"));
        MetricPlotWindow->setGeometry(0,540,1920,540);
        MetricPlotWindow->setWindowFlags( Qt::FramelessWindowHint );
        centralWidget = new QWidget(MetricPlotWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));

        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        thermPlot = new QCustomPlot(centralWidget);
        thermPlot->setObjectName(QStringLiteral("thermPlot"));
        thermPlot->setMaximumWidth(270);
        gridLayout->addWidget(thermPlot, 0, 0, 1, 1);

        metricPlot = new QCustomPlot(centralWidget);
        metricPlot->setObjectName(QStringLiteral("metricPlot"));
        // metricPlot->setMaximumSize(QSize(960, 540));
        gridLayout->addWidget(metricPlot, 0, 1, 1, 1);

        MetricPlotWindow->setCentralWidget(centralWidget);
        retranslateUi(MetricPlotWindow);

        QMetaObject::connectSlotsByName(MetricPlotWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MetricPlotWindow)
    {
        MetricPlotWindow->setWindowTitle(QApplication::translate("MetricPlotWindow", "BCIVibe demos", 0));
    } // retranslateUi

};

namespace Ui {
    class MetricPlotWindow: public Ui_MetricPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METRICPLOT_H
