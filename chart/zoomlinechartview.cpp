#include "zoomlinechartview.h"

ZoomLineChartView::ZoomLineChartView(QChart *parent) : QChartView(parent)
{

}


void ZoomLineChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        qDebug() << "zoomin";
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        qDebug() << "zoomout";
        chart()->zoomOut();
        break;
    }
}
