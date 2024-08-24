#pragma once

#include <QByteArray>
#include <QList>
#include <armadillo>

using namespace arma;

class PointCharge
{
  public:
    PointCharge() = delete;
    PointCharge(vec position, double charge);
    PointCharge(const QList<QByteArray>& input2d);

    vec position() const;
    double charge() const;
    bool isValid() const;
    void position(vec newPosition);
    void charge(double newCharge);

    friend bool compareXcoordinate(PointCharge const& lhs, PointCharge const& rhs);
    friend bool compareYcoordinate(PointCharge const& lhs, PointCharge const& rhs);

  private:
    vec m_position;
    double m_charge;
    bool m_isValid{false};
};

// friend comparison operators
bool compareXcoordinate(PointCharge const& lhs, PointCharge const& rhs);
bool compareYcoordinate(PointCharge const& lhs, PointCharge const& rhs);
