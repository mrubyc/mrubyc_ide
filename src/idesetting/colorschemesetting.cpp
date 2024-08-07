#include "colorschemesetting.h"
#include <QDebug>

ColorSchemeSetting::ColorSchemeSetting(QObject *parent)
    :AbstractSetting(parent)
{

}

ColorSchemeSetting::ColorSchemeSetting(ColorSchemeSetting &self)
    : ColorSchemeSetting(Q_NULLPTR)
{
    qDebug() << __FUNCTION__ << "Name:" << self.name();
    setName(self.name());
    setEditable(self.editable());
    QList<ColorSchemeElementSetting*> *selflist = self.elementSettingList();
    for (int i=0; i<selflist->size(); i++) {
        ColorSchemeElementSetting* elem = new ColorSchemeElementSetting(*(selflist->at(i)));
        elem->setParent(this);
        addColorSchemeElementSetting(elem);
    }
}

ColorSchemeSetting::~ColorSchemeSetting()
{
    qDebug() << __FUNCTION__ << "Name:" << m_name;
    m_elementSettingList.clear();
}

QString ColorSchemeSetting::name() const
{
    return m_name;
}

void ColorSchemeSetting::setName(const QString &name)
{
    m_name = name;
}

bool ColorSchemeSetting::editable() const
{
    return m_editable;
}

void ColorSchemeSetting::setEditable(bool editable)
{
    m_editable = editable;
}

QList<ColorSchemeElementSetting*> *ColorSchemeSetting::elementSettingList()
{
    return &m_elementSettingList;
}

void ColorSchemeSetting::addColorSchemeElementSetting(ColorSchemeElementSetting *colorSchemeElementSetting)
{
    m_elementSettingList.append(colorSchemeElementSetting);
}

void ColorSchemeSetting::removeColorSchemeElementSetting(ColorSchemeElementSetting *colorSchemeElementSetting)
{
    m_elementSettingList.removeOne(colorSchemeElementSetting);
}

bool ColorSchemeSetting::isModified()
{
    bool modified = false;
    // check each elements for color scheme
    for (int i=0; i<m_elementSettingList.size(); i++) {
        modified = m_elementSettingList.at(i)->isModified();
        if (modified) break;
    }
    return modified;
}

void ColorSchemeSetting::setModified(bool yes)
{
    for (int i=0; i<m_elementSettingList.size(); i++) {
         m_elementSettingList.at(i)->setModified(yes);
    }
}
