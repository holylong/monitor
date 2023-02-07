#ifndef ZOOMLINECHART_H
#define ZOOMLINECHART_H

#include <QtCharts>
#include <QChart>

class ZoomLineChart : public QChart
{
    Q_OBJECT
public:
    explicit ZoomLineChart(QGraphicsItem *parent = nullptr);

protected:
    bool sceneEvent(QEvent *event);
    bool gestureEvent(QGestureEvent *event);

signals:

};

#endif // ZOOMLINECHART_H
