#include "buildsetting.h"

#include <QDebug>

BuildSetting::BuildSetting(QObject *parent)
    :AbstractSetting(parent)
    ,m_mrbcEnabled(false)
    ,m_cloudEnabled(false)
    ,m_clearConsoleEnabled(false)
{
}

BuildSetting::~BuildSetting()
{
}

void BuildSetting::setMrbcEnabled(bool enable)
{
    if (m_mrbcEnabled == enable)
        return;

    m_mrbcEnabled = enable;
    setModified(true);
}
bool BuildSetting::mrbcEnabled() const
{
    return m_mrbcEnabled;
}

void BuildSetting::setMrbcCommand(const QString &path)
{
    if (!m_mrbcCommand.compare(path))
        return;

    m_mrbcCommand = path;
    setModified(true);
}
QString BuildSetting::mrbcCommand() const
{
    return m_mrbcCommand;
}

void BuildSetting::setMrbcCommandOptions(const QString &mrbcCommandOptions)
{
    if (m_mrbcCommandOptions == mrbcCommandOptions)
        return;

    m_mrbcCommandOptions = mrbcCommandOptions;
    setModified(true);
}
QString BuildSetting::mrbcCommandOptions() const
{
    return m_mrbcCommandOptions;
}

void BuildSetting::setCloudEnabled(bool enable)
{
    if (m_cloudEnabled == enable)
        return;

    m_cloudEnabled = enable;
    setModified(true);
}
bool BuildSetting::cloudEnabled() const
{
    return m_cloudEnabled;
}

void BuildSetting::setCloudUrl(const QString &url)
{
    if (!m_cloudUrl.compare(url))
        return;

    m_cloudUrl = url;
    setModified(true);
}
QString BuildSetting::cloudUrl() const
{
    return m_cloudUrl;
}

void BuildSetting::setClearConsoleEnabled(bool enable)
{
    if (m_clearConsoleEnabled == enable)
        return;

    m_clearConsoleEnabled = enable;
    setModified(true);
}
bool BuildSetting::clearConsoleEnabled() const
{
    return m_clearConsoleEnabled;
}

void BuildSetting::setMrbwCommand(const QString &path)
{
    if (!m_mrbwCommand.compare(path))
        return;
    m_mrbwCommand = path;
    setModified(true);
}
QString BuildSetting::mrbwCommand() const
{
    return m_mrbwCommand;
}

void BuildSetting::setMrbwCommandOptions(const QString &mrbwCommandOptions)
{
    if (m_mrbwCommandOptions == mrbwCommandOptions)
        return;

    m_mrbwCommandOptions = mrbwCommandOptions;
    setModified(true);
}
QString BuildSetting::mrbwCommandOptions() const
{
    return m_mrbwCommandOptions;
}

void BuildSetting::setPortName(const QString &portName)
{
    if (m_portName == portName)
        return;

    m_portName = portName;
    setModified(true);
}
QString BuildSetting::portName() const
{
    return m_portName;
}

void BuildSetting::setBaudRate(const QString &baud)
{
    if( m_baudRate == baud ) return;

    m_baudRate = baud;
    setModified(true);
}
QString BuildSetting::baudRate() const
{
    return m_baudRate;
}


void BuildSetting::clear()
{
    m_mrbcEnabled = false;
    m_mrbcCommand = Q_NULLPTR;
    m_mrbcCommandOptions = Q_NULLPTR;
    m_cloudEnabled = false;
    m_cloudUrl = Q_NULLPTR;
    m_clearConsoleEnabled = false;
    m_mrbwCommand = Q_NULLPTR;
    m_mrbwCommandOptions = Q_NULLPTR;
    m_portName = Q_NULLPTR;
    m_baudRate = Q_NULLPTR;
}
