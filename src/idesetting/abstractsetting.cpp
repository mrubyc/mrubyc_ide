#include "abstractsetting.h"

#include <QDebug>

AbstractSetting::AbstractSetting(QObject *parent)
    :QObject(parent)
{
    m_modified = false;
}

AbstractSetting::~AbstractSetting()
{
}

bool AbstractSetting::isModified()
{
    return m_modified;
}

void AbstractSetting::setModified(bool yes)
{
    if (m_modified != yes)
        m_modified = yes;
}
