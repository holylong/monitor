#include "chartwidget.h"
#include <QtCharts>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QToolBar>
#include <chart/rainfall/rainfallgraph.h>
#include <chart/tablewidget.h>
#include <chart/boxdatareader.h>


ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent)
{
    InitAllChartView();
    _toolBar = new QToolBar();
    _toolBar->setOrientation(Qt::Vertical);

    QAction *actionAreaChart = new QAction();
    actionAreaChart->setText("AreaChart");
    connect(actionAreaChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(0);
    });
    _toolBar->addAction(actionAreaChart);

    QAction *actionSplineChart = new QAction();
    actionSplineChart->setText("SplineChart");
    connect(actionSplineChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(1);
    });
    _toolBar->addAction(actionSplineChart);

    QAction *actionZoomLineChart = new QAction();
    actionZoomLineChart->setText("ZoomLineChart");
    connect(actionZoomLineChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(2);
    });
    _toolBar->addAction(actionZoomLineChart);

    QAction *actionDateTimeChart = new QAction();
    actionDateTimeChart->setText("DateTimeChart");
    connect(actionDateTimeChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(3);
    });
    _toolBar->addAction(actionDateTimeChart);

    QAction *actionModelDataChart = new QAction();
    actionModelDataChart->setText("ModelDataChart");
    connect(actionModelDataChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(4);
    });
    _toolBar->addAction(actionModelDataChart);

    QAction *actionBoxPlotChart = new QAction();
    actionBoxPlotChart->setText("BoxPlotChart");
    connect(actionBoxPlotChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(5);
    });
    _toolBar->addAction(actionBoxPlotChart);

    QAction *actionTemperatureChart = new QAction();
    actionTemperatureChart->setText("TemperatureChartChart");
    connect(actionTemperatureChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(6);
    });
    _toolBar->addAction(actionTemperatureChart);

#ifdef OPENGL_3D
    QAction *actionProxyChart = new QAction();
    actionProxyChart->setText("ProxyChartChart");
    connect(actionProxyChart, &QAction::triggered, [&]{
        _stackedLayout->setCurrentIndex(7);
    });
    _toolBar->addAction(actionProxyChart);
#endif

    QAction *actionQuit = new QAction();
    actionQuit->setText("Quit");
    connect(actionQuit, &QAction::triggered, [&]{
        this->hide();
    });
    _toolBar->addAction(actionQuit);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    mainLayout->addWidget(_toolBar);
    mainLayout->addLayout(_stackedLayout);
    setLayout(mainLayout);
}

void ChartWidget::InitAllChartView()
{
    _stackedLayout = new QStackedLayout();
    CreateAreaChart();
    CreateSplineChart();
    CreateZoomLineChart();
    CreateDateTimeChart();
    CreateModelDataChart();
    CreateBoxPlotChart();
    CreateTemperatureChartView();
#ifdef OPENGL_3D
    CreateProxyChart();
#endif
}

void ChartWidget::closeEvent(QCloseEvent *event)
{
    hide();
    qDebug() << "ChartWidget closeEvent";
    event->ignore();
}

/**
 * @brief ChartWidget::CreateKeyMouseChart
 */
void ChartWidget::CreateKeyMouseChart()
{
#if 0
    //解析json文件
    //创建chart
    QLineSeries *series = new QLineSeries();

    while (!stream.atEnd()) {
        QStringList values = line.split(" ", QString::SkipEmptyParts);
        QDateTime momentInTime;
        momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt() , 15));
        series->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
    }
    sunSpots.close();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("DataTime Chart");

    QDateTimeAxis *axisX =new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%1");
    axisY->setTitleText("Hellow owltf");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    _dateTimeChartView = new QChartView(chart);
    _dateTimeChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_dateTimeChartView);
#endif
}

void ChartWidget::CreateProxyChart()
{
    Q3DBars *rainfall = new Q3DBars;

    if (!rainfall->hasContext()) {
        qWarning() << QStringLiteral("Couldn't initialize the OpenGL context.") ;
        return;
    }

    rainfall->setFlags(rainfall->flags() ^ Qt::FramelessWindowHint);
    rainfall->resize(1000, 800);
    rainfall->setPosition(QPoint(10, 30));
    rainfall->show();

    RainfallGraph rainfallgraph(rainfall);
    rainfallgraph.start();
}

void ChartWidget::CreateCustomChart()
{
    QLineSeries *series = new QLineSeries();
    *series << QPointF(0,6) << QPointF(9,4) << QPointF(15,20)
            << QPointF(25,12) <<QPointF(29, 26);
    QChart *chart = new QChart;
    chart->legend()->hide();
    chart->addSeries(series);

    QPen pen(QRgb(0xfdb157));
    pen.setWidth(5);
    series->setPen(pen);

    QFont font;
    font.setPixelSize(5);
    chart->setFont(font);
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setTitle("Custom Chart");

    QLinearGradient backgroundGradient;
}

void ChartWidget::CreateModelDataChart()
{
    _modelDataWidget = new TableWidget();
    _stackedLayout->addWidget(_modelDataWidget);
}

/**
 * @brief ChartWidget::CreateBoxPlotChart
 */
void ChartWidget::CreateBoxPlotChart(){
    //! [1]
    QBoxPlotSeries *acmeSeries = new QBoxPlotSeries();
    acmeSeries->setName("Acme Ltd");

    QBoxPlotSeries *boxWhiskSeries = new QBoxPlotSeries();
    boxWhiskSeries->setName("BoxWhisk Inc");
    //! [1]

    //! [2]
    QFile acmeData(":/datas/acme_data.txt");
    if (!acmeData.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    BoxDataReader dataReader(&acmeData);
    while (!dataReader.atEnd()) {
        QBoxSet *set = dataReader.readBox();
        if (set)
            acmeSeries->append(set);
    }
    //! [2]

    //! [3]
    QFile boxwhiskData(":/datas/boxwhisk_data.txt");
    if (!boxwhiskData.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    dataReader.readFile(&boxwhiskData);
    while (!dataReader.atEnd()) {
        QBoxSet *set = dataReader.readBox();
        if (set)
            boxWhiskSeries->append(set);
    }
    //! [3]

    //! [4]
    QChart *chart = new QChart();
    chart->addSeries(acmeSeries);
    chart->addSeries(boxWhiskSeries);
    chart->setTitle("Acme Ltd and BoxWhisk Inc share deviation in 2012");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //! [4]

    //! [5]
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setMin(15.0);
    chart->axes(Qt::Horizontal).first()->setMax(34.0);
    //! [5]

    //! [6]
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    //! [6]

    //! [7]
    _boxPlotChartView = new QChartView(chart);
    _boxPlotChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_boxPlotChartView);
}

/**
 * @brief ChartWidget::CreateTemperatureChartView
 */
void ChartWidget::CreateTemperatureChartView()
{
    //![1]
        QBarSet *low = new QBarSet("Min");
        QBarSet *high = new QBarSet("Max");

        *low << -52 << -50 << -45.3 << -37.0 << -25.6 << -8.0
             << -6.0 << -11.8 << -19.7 << -32.8 << -43.0 << -48.0;
        *high << 11.9 << 12.8 << 18.5 << 26.5 << 32.0 << 34.8
              << 38.2 << 34.8 << 29.8 << 20.4 << 15.1 << 11.8;
    //![1]

    //![2]
        QStackedBarSeries *series = new QStackedBarSeries();
        series->append(low);
        series->append(high);
    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Temperature records in celcius");
        chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
        QStringList categories = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setTitleText("Month");
        chart->addAxis(axisX, Qt::AlignBottom);
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(-52, 52);
        axisY->setTitleText("Temperature [&deg;C]");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    //![4]

    //![5]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]

    //![6]
        _temperatureChartView = new QChartView(chart);
        _temperatureChartView->setRenderHint(QPainter::Antialiasing);
    //![6]
        _stackedLayout->addWidget(_temperatureChartView);
}

/**
 * @brief ChartWidget::CreateDateTimeChart
 * DataTimeChart
 */
void ChartWidget::CreateDateTimeChart()
{
    QLineSeries *series = new QLineSeries();

    QFile sunSpots(":/datas/sun_spots.txt");
    if (!sunSpots.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&sunSpots);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.startsWith("#") || line.startsWith(":"))
            continue;
        QStringList values = line.split(" ", QString::SkipEmptyParts);
        QDateTime momentInTime;
        momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt() , 15));
        series->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
    }
    sunSpots.close();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("DataTime Chart");

    QDateTimeAxis *axisX =new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%1");
    axisY->setTitleText("Hellow owltf");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    _dateTimeChartView = new QChartView(chart);
    _dateTimeChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_dateTimeChartView);
}

void ChartWidget::CreateZoomLineChart()
{
    QLineSeries *series = new QLineSeries();
    for (int i = 0; i < 500; i++) {
       QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
       p.ry() += QRandomGenerator::global()->bounded(20);
       *series << p;
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple ZoomLine chart example");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0,500);
    chart->axes(Qt::Vertical).first()->setRange(0, 120);

    _zoomLineChartView = new ZoomLineChartView(chart);
    _zoomLineChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_zoomLineChartView);
}

void ChartWidget::CreateSplineChart()
{
    QSplineSeries *series = new QSplineSeries();
    series->setName("Spline Chart");
    series->append(0,6);
    series->append(2,4);
    series->append(3,8);
    series->append(7,4);
    series->append(10,5);

    *series << QPointF(11,1) << QPointF(13,3)
            << QPointF(17,6) << QPointF(18,3)
            << QPointF(20,3);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple spline chart example");
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0,10);

    _splineChartView = new QChartView(chart);
    _splineChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_splineChartView);
}

void ChartWidget::CreateAreaChart()
{
    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();

    *series0 << QPointF(1,5) << QPointF(3,7) << QPointF(7,6) << QPointF(9,7) <<QPointF(12,6)
             << QPointF(16,7) << QPointF(18,5);
    *series1 << QPointF(1,3) << QPointF(3,4) << QPointF(7,3) << QPointF(8,2) <<QPointF(12,3)
             << QPointF(16,4) << QPointF(18,3);

    QAreaSeries *series = new QAreaSeries(series0, series1);
    series->setName("Latin");
    QPen pen(0x059605);
    pen.setWidth(3);
    series->setPen(pen);

    QLinearGradient gradient(QPointF(0,0), QPointF(0,1));
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    series->setBrush(gradient);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple areachart example");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0,20);
    chart->axes(Qt::Vertical).first()->setRange(0,20);

    _areaChartView = new QChartView(chart);
    _areaChartView->setRenderHint(QPainter::Antialiasing);
    _stackedLayout->addWidget(_areaChartView);
}
