#include "MultipolePlot.hh"
#include <QDebug>
#include <qcustomplot/qcustomplot.h>

MultipolePlot::MultipolePlot(QQuickItem* parent) : QQuickPaintedItem{parent}, m_customPlot{nullptr}
{
    setFlag(QQuickItem::ItemHasContents, true);
    // setRenderTarget(QQuickPaintedItem::FramebufferObject);
    // setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    connect(this, &QQuickPaintedItem::widthChanged, this, &MultipolePlot::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &MultipolePlot::updateCustomPlotSize);
}

MultipolePlot::~MultipolePlot()
{
    delete m_customPlot;
    m_customPlot = nullptr;
}

void MultipolePlot::registerMultipoleField(MultipoleField* multipoleField)
{
    m_multipoleField = multipoleField;

    m_customPlot = new QCustomPlot();

    updateCustomPlotSize();
    setupPotential(m_customPlot);
    connect(m_customPlot, &QCustomPlot::afterReplot, this, &MultipolePlot::onCustomReplot);

    m_customPlot->replot();
}

void MultipolePlot::paint(QPainter* painter)
{
    if (m_customPlot)
    {
        QPixmap picture(boundingRect().size().toSize());
        QCPPainter qcpPainter(&picture);

        // m_CustomPlot->replot();
        m_customPlot->toPainter(&qcpPainter);

        painter->drawPixmap(QPoint(), picture);
    }
}

void MultipolePlot::mousePressEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void MultipolePlot::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void MultipolePlot::mouseMoveEvent(QMouseEvent* event)
{
    routeMouseEvents(event);
}

void MultipolePlot::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

// void MultipolePlot::graphClicked(QCPAbstractPlottable* plottable)
//{
//    qDebug() << Q_FUNC_INFO << QString("Clicked on graph '%1 ").arg(plottable->name());
//}

void MultipolePlot::routeMouseEvents(QMouseEvent* event)
{
    if (m_customPlot)
    {
        //        QMouseEvent* newEvent = new QMouseEvent(event->type(), event->position(), event->button(),
        //                                                event->buttons(), event->modifiers());
        //        // QCoreApplication::sendEvent( m_CustomPlot, newEvent );
        //        QCoreApplication::postEvent(m_customPlot, newEvent);
    }
}

void MultipolePlot::updateCustomPlotSize()
{
    if (m_customPlot)
    {
        m_customPlot->setGeometry(0, 0, width(), height());
    }
}

void MultipolePlot::onCustomReplot()
{
    qDebug() << Q_FUNC_INFO;
    update();
}

void MultipolePlot::setupPotential(QCustomPlot* customPlot)
{
    // configure axis rect:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    constexpr auto nx{500};
    constexpr auto ny{500};
    constexpr auto maxrange{4};
    QCPColorMap* colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    colorMap->data()->setSize(nx, ny);
    QRectF boundingbox = m_multipoleField->boundingBox();
    double width = boundingbox.width();
    double height = boundingbox.height();
    constexpr auto factor{1.5};
    double left = boundingbox.right() - factor * width;
    double right = boundingbox.left() + factor * width;
    double top = boundingbox.bottom() - factor * height;
    double bottom = boundingbox.top() + factor * height;
    colorMap->data()->setRange(QCPRange(left, right), QCPRange(top, bottom));

    constexpr auto threshold{220.0};
    double x, y, z; // NOLINT
    for (int xIndex = 0; xIndex < nx; ++xIndex)
    {
        for (int yIndex = 0; yIndex < ny; ++yIndex)
        {
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            z = m_multipoleField->potential(vec{x, y}, threshold);
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }

    // add a color scale:
    QCPColorScale* colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right
                                           // (actually atRight is already the default)
    colorMap->setColorScale(colorScale);   // associate the color map with the color scale
    colorScale->axis()->setLabel("Electric potential");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpPolar);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color
    // gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup* marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();
}
