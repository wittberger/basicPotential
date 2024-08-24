#pragma once

// todo
// - use command line option to read file in
// - write charges to file

#include "PointCharge.hh"
#include <QList>
#include <QRectF>
#include <optional>

using namespace arma;

class MultipoleField
{
  public:
    MultipoleField();
    explicit MultipoleField(const QString& filePath);

  public:
    QRectF boundingBox() const;
    void appendPointCharge(const PointCharge& pointCharge);

    // Phi = 1/(4pi e_0) Sum (Q_i / |R-r_i|)
    double potential(const vec& position, std::optional<double> threshold = std::nullopt) const;

  private:
    QList<PointCharge> m_charges;
};

using MultipoleFieldPtr = std::shared_ptr<MultipoleField>;
