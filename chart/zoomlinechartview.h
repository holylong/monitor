#ifndef ZOOMLINECHARTVIEW_H
#define ZOOMLINECHARTVIEW_H

#include <QtCharts>
#include <QChartView>

class ZoomLineChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ZoomLineChartView(QChart *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event);

signals:

};

#endif // ZOOMLINECHART_H
