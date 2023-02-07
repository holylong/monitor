#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <chart/zoomlinechartview.h>

//https://doc.qt.io/qt-5/qtcharts-customchart-example.html chart
//https://blog.csdn.net/birenxiaofeigg/article/details/95614311 chart
//https://blog.csdn.net/weixin_40774605/article/details/108419887 chart
//https://www.jianshu.com/p/c7d104ae164b?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation mvvm
//https://blog.csdn.net/qq_45662588/article/details/117668702  mvvm

class QToolBar;
class QStackedLayout;
class TableWidget;
namespace QtCharts {
    class QChartView;
}

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);

    void CreateAreaChart();
    void CreateSplineChart();
    void CreateZoomLineChart();
    void CreateDateTimeChart();

    void CreateModelDataChart();
    void CreateCustomChart();

    void CreateBoxPlotChart();
    void CreateTemperatureChartView();
    void CreateProxyChart();
    void CreateKeyMouseChart();

    void InitAllChartView();

private:
    QToolBar             *_toolBar;

    QtCharts::QChartView *_areaChartView;
    QtCharts::QChartView *_splineChartView;
    ZoomLineChartView    *_zoomLineChartView;
    QtCharts::QChartView *_dateTimeChartView;
    QtCharts::QChartView *_boxPlotChartView;
    QtCharts::QChartView *_proxyChartView;
    QtCharts::QChartView *_temperatureChartView;
    TableWidget          *_modelDataWidget;

    QStackedLayout       *_stackedLayout;

signals:

};

#endif // CHARTWIDGET_H
