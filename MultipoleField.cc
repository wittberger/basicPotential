#include "MultipoleField.hh"
#include "PointCharge.hh"
#include <QDebug>
#include <QFile>
#include <QPointF>
#include <algorithm>

template <typename T>
int signum(T val)
{
    return (T(0) < val) - (val < T(0));
}

MultipoleField::MultipoleField()
{
    constexpr auto two{2.0};
    constexpr auto three{3.0};
    constexpr auto four{4.0};
    constexpr auto five{5.0};
    constexpr auto six{6.0};

    appendPointCharge({vec{three, four}, three});
    appendPointCharge({vec{five, six}, four});
    appendPointCharge({vec{six, four}, -four});
}

MultipoleField::MultipoleField(const QString& filePath)
{
    QFile file(filePath);

    if (!file.exists())
    {
        qCritical() << "File not found.";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << file.errorString();
        return;
    }

    // read file header
    if (!file.atEnd())
    {
        file.readLine();
    }
    // read point charges
    while (!file.atEnd())
    {
        PointCharge pointCharge{file.readLine().replace('\n', "").split('\t')};

        if (pointCharge.isValid())
        {
            m_charges.append(pointCharge);
        }
    }
    file.close();
}

void MultipoleField::appendPointCharge(const PointCharge& pointCharge)
{
    m_charges.append(pointCharge);
}

QRectF MultipoleField::boundingBox() const
{
    auto [xMin, xMax] = std::minmax_element(m_charges.begin(), m_charges.end(), compareXcoordinate);
    auto [yMin, yMax] = std::minmax_element(m_charges.begin(), m_charges.end(), compareYcoordinate);
    QPointF upperLeft{xMin->position().at(0), yMin->position().at(1)};
    QPointF lowerRight{xMax->position().at(0), yMax->position().at(1)};

    return QRectF(upperLeft, lowerRight);
}

double MultipoleField::potential(const vec& position, std::optional<double> threshold) const
{
    double Phi = 0.0;

    for (auto& pointCharge : m_charges)
    {
        Phi += pointCharge.charge() / norm(position - pointCharge.position());
    }

    // cutting off potential
    if (threshold.has_value() && fabs(Phi) > threshold.value())
    {
        Phi = signum(Phi) * threshold.value();
    }

    return Phi;
}
