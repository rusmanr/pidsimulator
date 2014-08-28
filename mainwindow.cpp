#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_math.h>

//#include "guihandler.h"
#include "sim.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->kpLowTb->setText("0");
    ui->kpHighTb->setText("1");
    ui->kpSlider->setMinimum(0);
    ui->kpSlider->setMaximum(1000);
    ui->kpLabel->setText("0.00");

    ui->kiLowTb->setText("0");
    ui->kiHighTb->setText("1");
    ui->kiSlider->setMinimum(0);
    ui->kiSlider->setMaximum(1000);
    ui->kiLabel->setText("0.00");

    ui->kdLowTb->setText("0");
    ui->kdHighTb->setText("1");
    ui->kdSlider->setMinimum(0);
    ui->kdSlider->setMaximum(1000);
    ui->kdLabel->setText("0.00");

    ui->qwtPlot->setAutoReplot(false);
    // axes
    ui->qwtPlot->setTitle("PID Plot");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "t -->");
//    setAxisTitle( xBottom, "x -->" );
    ui->qwtPlot->setAxisScale(QwtPlot:: xBottom, 0.0, 10.0 );

    ui->qwtPlot->setAxisTitle(QwtPlot:: yLeft, "M -->" );
    ui->qwtPlot->setAxisScale(QwtPlot:: yLeft, -10.0, 10.0 );

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    ui->qwtPlot->setCanvas( canvas );
    ui->qwtPlot->setAutoReplot(true);

//    // Insert new curves
//    QwtPlotCurve *cSin = new QwtPlotCurve( "y = sin(x)" );
//    cSin->setRenderHint( QwtPlotItem::RenderAntialiased );
//    cSin->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
//    cSin->setPen( Qt::red );
//    cSin->attach( ui->qwtPlot);

//    QwtPlotCurve *cCos = new QwtPlotCurve( "y = cos(x)" );
//    cCos->setRenderHint( QwtPlotItem::RenderAntialiased );
//    cCos->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
//    cCos->setPen( Qt::blue );
//    cCos->attach( ui->qwtPlot );

//    QPolygonF points;
//        points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
//            << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
//            << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );
//        cSin->setSamples( points );
//        cCos->setSamples( points );

//    guiHandler = new GuiHandler(ui->plotWidget);
    QObject::connect(ui->runPb, SIGNAL(clicked()), this, SLOT(run()));
    QObject::connect(ui->clearPb, SIGNAL(clicked()), this, SLOT(clearSimulation()));
    QObject::connect(ui->kpSlider, SIGNAL(valueChanged(int)), this, SLOT(setKp(int)));
    QObject::connect(ui->kiSlider, SIGNAL(valueChanged(int)), this, SLOT(setKi(int)));
    QObject::connect(ui->kdSlider, SIGNAL(valueChanged(int)), this, SLOT(setKd(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setKp(int slider)
{
    mKp = double(slider) / 1000.0;
//    guiHandler->runSimulation(mKp, mKi, mKd);
}

void MainWindow::setKi(int slider)
{
    mKi = double(slider) / 1000.0;
//    guiHandler->runSimulation(mKp, mKi, mKd);
}

void MainWindow::setKd(int slider)
{
    mKd = double(slider) / 1000.0;
//    guiHandler->runSimulation(mKp, mKi, mKd);
}

void MainWindow::run()
{
//    guiHandler->runSimulation(mKp, mKi, mKd);
    qDebug()<< "in run";
    runSimulation(mKp, mKi, mKd);
}

void MainWindow::runSimulation(double kp, double ki, double kd)
{
//    mPlotWidget->removeAllPlotObjects();
    qDebug()<< "in runSimulation";
    ui->qwtPlot->setAutoReplot(false);

    // Insert new curves
    QwtPlotCurve *cSin = new QwtPlotCurve( "y = sin(x)" );
    cSin->setRenderHint( QwtPlotItem::RenderAntialiased );
    cSin->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    cSin->setPen( Qt::red );
    cSin->attach( ui->qwtPlot);

    QwtPlotCurve *cCos = new QwtPlotCurve( "y = cos(x)" );
    cCos->setRenderHint( QwtPlotItem::RenderAntialiased );
    cCos->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    cCos->setPen( Qt::blue );
    cCos->attach( ui->qwtPlot );

    QPolygonF points;
        points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
            << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
            << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );
        cSin->setSamples( points );
        cCos->setSamples( points );

    // Create sin and cos data
//    cSin->setData( new FunctionData( ::sin ) );
//    cCos->setData( new FunctionData( ::cos ) );

    // Insert markers

    //  ...a horizontal line at y = 0...
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabel( QString::fromLatin1( "y = 0" ) );
    mY->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY->setLineStyle( QwtPlotMarker::HLine );
    mY->setYValue( 0.0 );
    mY->attach( ui->qwtPlot );

    //  ...a vertical line at x = 2 * pi
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel( QString::fromLatin1( "x = 2 pi" ) );
    mX->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    mX->setLabelOrientation( Qt::Vertical );
    mX->setLineStyle( QwtPlotMarker::VLine );
    mX->setLinePen( Qt::black, 0, Qt::DashDotLine );
    mX->setXValue( 2.0 * M_PI );
    mX->attach( ui->qwtPlot );

    const double x = 7.7;

    // an arrow at a specific position
    QwtPlotMarker *mPos = new QwtPlotMarker( "Marker" );
    mPos->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    mPos->setItemAttribute( QwtPlotItem::Legend, true );
//    mPos->setSymbol( new ArrowSymbol() );
    mPos->setValue( QPointF( x, ::sin( x ) ) );
    mPos->setLabel( QString( "x = %1" ).arg( x ) );
    mPos->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    mPos->attach( ui->qwtPlot );


    double minX, minY;
    minX = minY = std::numeric_limits<double>::max();
    double maxX, maxY;
    maxX = maxY = -1 * std::numeric_limits<double>::max();
    int colorIx = 0;

    QColor colors[] = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::cyan,
        Qt::magenta,
        Qt::yellow,
        Qt::darkRed,
        Qt::darkGreen,
        Qt::darkBlue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow,
        Qt::white,
        Qt::lightGray
    };

    points.clear();

    for (int setpoint = 2; setpoint < 10; setpoint += 2)
    {
        std::vector<SimPoint> simulation = controlResponse(300, setpoint, kp, ki, kd);

//        KPlotObject* plotData = new KPlotObject(colors[colorIx], KPlotObject::Lines, 2);
        if (++colorIx >= ARRAY_SIZE(colors)) colorIx = 0;
        for (SimPoint pt : simulation)
        {
//            plotData->addPoint(pt.first, pt.second);
            points.append(QPointF( pt.first, pt.second));
            minX = std::min(minX, pt.first);
            maxX = std::max(maxX, pt.first);
            minY = std::min(minY, pt.second);
            maxY = std::max(maxY, pt.second);
        }
//        mPlotWidget->addPlotObject(plotData);
        cSin->setSamples( points );
        cCos->setSamples( points );
    }
    if (minX == maxX) ++maxX;
    if (minY == maxY) ++maxY;

//    ui->qwtPlot->update();
//    ui->qwtPlot->repaint();
//    ui->qwtPlot->canvas()->repaint();
    ui->qwtPlot->setAutoReplot(true);
    ui->qwtPlot->replot();



//    mPlotWidget->setLimits(minX, maxX, minY, maxY);
//    mPlotWidget->update();
}

void MainWindow::clearSimulation()
{
//    mPlotWidget->removeAllPlotObjects();
    qDebug()<< "clear";
    ui->qwtPlot->detachItems(0);
    ui->qwtPlot->replot();
//    runSimulation(0.0, 0.0, 0.0);
}
