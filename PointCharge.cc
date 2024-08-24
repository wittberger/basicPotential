#include "PointCharge.hh"

PointCharge::PointCharge(vec position, double charge)
{
    m_position = std::move(position);
    m_charge = charge;
    m_isValid = true;
}

PointCharge::PointCharge(const QList<QByteArray>& input2d)
{
    constexpr auto nrInputValues{3};
    if (input2d.size() == nrInputValues)
    {
        constexpr auto idx_q{0};
        constexpr auto idx_x{1};
        constexpr auto idx_y{2};

        m_charge = input2d[idx_q].toDouble();
        m_position = {input2d[idx_x].toDouble(), input2d[idx_y].toDouble()};
    }
}

vec PointCharge::position() const
{
    return m_position;
}

double PointCharge::charge() const
{
    return m_charge;
}

bool PointCharge::isValid() const
{
    return fabs(m_charge) > 0.0;
}

void PointCharge::position(vec newPosition)
{
    m_position = std::move(newPosition);
}

void PointCharge::charge(double newCharge)
{
    m_charge = newCharge;
}

// friend comparison operators
bool compareXcoordinate(PointCharge const& lhs, PointCharge const& rhs)
{
    return lhs.position().at(0) < rhs.position().at(0);
}

bool compareYcoordinate(PointCharge const& lhs, PointCharge const& rhs)
{
    return lhs.position().at(1) < rhs.position().at(1);
}
