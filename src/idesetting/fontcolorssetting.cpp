#include "fontcolorssetting.h"
#include "abstractsetting.h"
#include "colorschemesetting.h"

#include <QVariant>
#include <QDebug>

FontColorsSetting::FontColorsSetting(QObject *parent)
    : AbstractSetting(parent)
{
}

FontColorsSetting::~FontColorsSetting()
{
}

QString FontColorsSetting::fontFamily() const
{
    return m_fontFamily;
}

void FontColorsSetting::setFontFamily(const QString &fontFamily)
{
    if (!m_fontFamily.compare(fontFamily))
        return;

    m_fontFamily = fontFamily;
    setModified(true);
}

int FontColorsSetting::fontSize() const
{
    return m_fontSize;
}

void FontColorsSetting::setFontSize(int fontSize)
{
    if (m_fontSize == fontSize)
        return;

    m_fontSize = fontSize;
    setModified(true);
}

QString FontColorsSetting::defaultColorSchemeName() const
{
    return m_defaultColorSchemeName;
}

void FontColorsSetting::setDefaultColorSchemeName(const QString &defaultColorSchemeName)
{
    if (!m_defaultColorSchemeName.compare(defaultColorSchemeName))
        return;

    m_defaultColorSchemeName = defaultColorSchemeName;
    setModified(true);
}

QList<ColorSchemeSetting*> *FontColorsSetting::colorSchemeSettingList()
{
    return &m_colorSchemeSettingList;
}

ColorSchemeSetting *FontColorsSetting::findColorScheme(const QString &name)
{
    ColorSchemeSetting *colorSchemeSetting = Q_NULLPTR;
    int size = m_colorSchemeSettingList.size();
    for (int i=0; i<size; i++) {
        if(!m_colorSchemeSettingList.at(i)->name().compare(name)) {
            colorSchemeSetting = m_colorSchemeSettingList.at(i);
            break;
        }
    }
    return colorSchemeSetting;
}

void FontColorsSetting::clear()
{
    m_fontFamily = Q_NULLPTR;
    m_fontSize = 0;
    m_defaultColorSchemeName = Q_NULLPTR;

    m_colorSchemeSettingList.clear();
}

bool FontColorsSetting::isModified()
{
    bool modified = false;
    // check font/size
    modified = AbstractSetting::isModified();
    if (modified) return modified;

    // check color scheme
    for (int i=0; i<m_colorSchemeSettingList.size(); i++) {
        modified = m_colorSchemeSettingList.at(i)->isModified();
        if (modified) break;
    }
    return modified;

}

void FontColorsSetting::setModified(bool yes)
{
    AbstractSetting::setModified(yes);

    for (int i=0; i<m_colorSchemeSettingList.size(); i++) {
         m_colorSchemeSettingList.at(i)->setModified(yes);
    }
}

void FontColorsSetting::addColorSchemeSetting(ColorSchemeSetting *colorSchemeSetting)
{
    m_colorSchemeSettingList.append(colorSchemeSetting);
}

void FontColorsSetting::removeColorSchemeSetting(ColorSchemeSetting *colorSchemeSetting)
{
    m_colorSchemeSettingList.removeOne(colorSchemeSetting);
}


