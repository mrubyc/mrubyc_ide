#include "projectsetting.h"

ProjectSetting::ProjectSetting(QObject *parent)
    :AbstractSetting(parent)
{
}

QString ProjectSetting::lastLocation() const
{
    return m_lastLocation;
}

void ProjectSetting::setLastLocation(const QString &lastLocation)
{
    m_lastLocation = lastLocation;
}

QString ProjectSetting::defaultLocation() const
{
    return m_defaultLocation;
}

void ProjectSetting::setDefaultLocation(const QString &defaultLocation)
{
    m_defaultLocation = defaultLocation;
}
