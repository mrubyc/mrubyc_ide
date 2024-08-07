#include "genericidesetting.h"
#include "abstractsetting.h"

GenericIDESetting::GenericIDESetting(QObject *parent)
    :AbstractSetting(parent)
{
}

bool GenericIDESetting::maximized() const
{
    return m_maximized;
}

void GenericIDESetting::setMaximized(bool maximized)
{
    m_maximized = maximized;
}

QByteArray GenericIDESetting::geometry() const
{
    return m_geometry;
}

void GenericIDESetting::setGeometry(const QByteArray &geometry)
{
    m_geometry = geometry;
}
