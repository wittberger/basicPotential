#pragma once

#include "MultipoleField.hh"
#include <QQuickPaintedItem>

class QCustomPlot;
class QCPAbstractPlottable;

class MultipolePlot : public QQuickPaintedItem
{
    Q_OBJECT

  public:
    MultipolePlot(QQuickItem* parent = nullptr);
    ~MultipolePlot() override;

    void paint(QPainter* painter) override;

    Q_INVOKABLE void registerMultipoleField(MultipoleField* multipoleField);

  protected:
    void routeMouseEvents(QMouseEvent* event);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    void setupPotential(QCustomPlot* customPlot);

  private:
    QCustomPlot* m_customPlot{nullptr};
    MultipoleField* m_multipoleField{nullptr};

  private slots:
    // void graphClicked(QCPAbstractPlottable* plottable);
    void onCustomReplot();
    void updateCustomPlotSize();
};
