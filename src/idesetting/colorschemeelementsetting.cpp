#include "colorschemeelementsetting.h"
#include <QDebug>

ColorSchemeElementSetting::ColorSchemeElementSetting(QObject *parent)
    : AbstractSetting(parent)
{

}

ColorSchemeElementSetting::ColorSchemeElementSetting(const ColorSchemeElementSetting &self)
    : AbstractSetting(Q_NULLPTR)
{
    setName(self.name());
    setBackground(self.background());
    setForeground(self.foreground());
    setBold(self.bold());
    setItalic(self.italic());
}

ColorSchemeElementSetting::~ColorSchemeElementSetting()
{
    qDebug() << __FUNCTION__ << "Name:" << m_name;
}

QString ColorSchemeElementSetting::name() const
{
    return m_name;
}

void ColorSchemeElementSetting::setName(const QString &name)
{
    if (!m_name.compare(name)) {
        return;
    }
    m_name = name;
    setModified(true);
}

QString ColorSchemeElementSetting::foreground() const
{
    return m_foreground;
}

void ColorSchemeElementSetting::setForeground(const QString &foreground)
{
    if (!m_foreground.compare(foreground))
        return;

    m_foreground = foreground;
    setModified(true);
}

QString ColorSchemeElementSetting::background() const
{
    return m_background;
}

void ColorSchemeElementSetting::setBackground(const QString &background)
{
    if (!m_background.compare(background))
        return;

    m_background = background;
    setModified(true);
}

bool ColorSchemeElementSetting::bold() const
{
    return m_bold;
}

void ColorSchemeElementSetting::setBold(bool bold)
{
    if (m_bold == bold)
        return;

    m_bold = bold;
    setModified(true);
}

bool ColorSchemeElementSetting::italic() const
{
    return m_italic;
}

void ColorSchemeElementSetting::setItalic(bool itaric)
{
    if (m_italic == itaric)
        return;

    m_italic = itaric;
    setModified(true);
}
