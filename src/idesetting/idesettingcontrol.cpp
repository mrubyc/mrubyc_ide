#include "idesettingcontrol.h"

#include "setting.h"
#include "idesetting.h"
#include "buildsetting.h"
#include "fontcolorssetting.h"
#include "colorschemesetting.h"
#include "colorschemeelementsetting.h"
#include "projectsetting.h"
#include "genericidesetting.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

IdeSettingControl::IdeSettingControl(QSettings *config, QObject *parent)
    :QObject(parent)
    ,m_config(config)
{
    m_ideSetting = new IdeSetting(this);
}

IdeSettingControl::~IdeSettingControl()
{
}

void IdeSettingControl::loadColorSchemeSetting()
{
    FontColorsSetting *fontColorsSetting = m_ideSetting->fontColorsSetting();

    m_config->beginGroup(Setting::Constants::Key::COLOR_SCHEME_SETTING);
    {
        int size = m_config->beginReadArray(Setting::Constants::Key::COLOR_SCHEME);
        for (int i=0; i<size; i++) {
            ColorSchemeSetting *colorSchemeSetting = new ColorSchemeSetting(fontColorsSetting);
            m_config->setArrayIndex(i);
            QString colorSchemeName = m_config->value(Setting::Constants::Key::NAME).toString();
            bool colorSchemeEditable = m_config->value(Setting::Constants::Key::EDITABLE).toBool();
            colorSchemeSetting->setName(colorSchemeName);
            colorSchemeSetting->setEditable(colorSchemeEditable);

            int elementSize = m_config->beginReadArray(Setting::Constants::Key::COLOR_SCHEME_ELEMENT);
            for (int j=0; j<elementSize; j++) {
                ColorSchemeElementSetting *elementSetting = new ColorSchemeElementSetting(colorSchemeSetting);

                m_config->setArrayIndex(j);
                QString elementName = m_config->value(Setting::Constants::Key::NAME).toString();
                QString foregroundName = m_config->value(Setting::Constants::Key::FOREGROUND, "").toString();
                QString backgroundName = m_config->value(Setting::Constants::Key::BACKGROUND, "").toString();
                bool boldFlag = m_config->value(Setting::Constants::Key::BOLD, false).toBool();
                bool italicFlag = m_config->value(Setting::Constants::Key::ITALIC, false).toBool();
                elementSetting->setName(elementName);
                elementSetting->setForeground(foregroundName);
                elementSetting->setBackground(backgroundName);
                elementSetting->setBold(boldFlag);
                elementSetting->setItalic(italicFlag);

                colorSchemeSetting->addColorSchemeElementSetting(elementSetting);
            }
            m_config->endArray();
            fontColorsSetting->addColorSchemeSetting(colorSchemeSetting);
        }
        m_config->endArray();
    }
    m_config->endGroup();
}

void IdeSettingControl::storeColorSchemeSetting()
{
    FontColorsSetting *fontColorsSetting = m_ideSetting->fontColorsSetting();
    QList<ColorSchemeSetting*> *colorSchemeSettingList = fontColorsSetting->colorSchemeSettingList();

    m_config->beginGroup(Setting::Constants::Key::COLOR_SCHEME_SETTING);
    {
        m_config->beginWriteArray(Setting::Constants::Key::COLOR_SCHEME);
        for (int i=0; i<colorSchemeSettingList->size(); i++) {
            ColorSchemeSetting *colorSchemeSetting = colorSchemeSettingList->at(i);

            m_config->setArrayIndex(i);
            m_config->setValue(Setting::Constants::Key::NAME, colorSchemeSetting->name());
            m_config->setValue(Setting::Constants::Key::EDITABLE, colorSchemeSetting->editable());
            QList<ColorSchemeElementSetting*> *elementSettingList = colorSchemeSetting->elementSettingList();

            m_config->beginWriteArray(Setting::Constants::Key::COLOR_SCHEME_ELEMENT);
            for (int j=0; j<elementSettingList->size(); j++) {
                ColorSchemeElementSetting *elementSetting = elementSettingList->at(j);

                m_config->setArrayIndex(j);
                m_config->setValue(Setting::Constants::Key::NAME, elementSetting->name());
                QString foregroundName = elementSetting->foreground().isEmpty() ? "" : elementSetting->foreground();
                m_config->setValue(Setting::Constants::Key::FOREGROUND, foregroundName);
                QString backgroundName = elementSetting->background().isEmpty() ? "" : elementSetting->background();
                m_config->setValue(Setting::Constants::Key::BACKGROUND, backgroundName);
                m_config->setValue(Setting::Constants::Key::BOLD, elementSetting->bold());
                m_config->setValue(Setting::Constants::Key::ITALIC, elementSetting->italic());
            }
            m_config->endArray();
        }
        m_config->endArray();
    }
    m_config->endGroup();
}

void IdeSettingControl::save()
{
    qDebug();
    m_config->clear();

    ProjectSetting *projectSetting = m_ideSetting->projectSetting();
    m_config->setValue("Projects/LastLocation", projectSetting->lastLocation());
    m_config->setValue("Projects/DefaultLocation", projectSetting->defaultLocation());
    GenericIDESetting *genericIdeSetting = m_ideSetting->gnericIdeSetting();
    m_config->setValue("IDE/Window/Maximized", genericIdeSetting->maximized());
    m_config->setValue("IDE/Window/Geometry", genericIdeSetting->geometry());

    BuildSetting *buildSetting = m_ideSetting->buidSetting();
    m_config->beginGroup(Setting::Constants::Key::BUILD_AND_DEPLOY);
    m_config->beginGroup(Setting::Constants::Key::COMPILER);
    m_config->setValue(Setting::Constants::Key::MRBC_ENABLED, buildSetting->mrbcEnabled());
    m_config->setValue(Setting::Constants::Key::MRBC_COMMAND, buildSetting->mrbcCommand());
    m_config->setValue(Setting::Constants::Key::MRBC_COMMAND_OPTS, buildSetting->mrbcCommandOptions());
    m_config->setValue(Setting::Constants::Key::CLOUD_ENABLED, buildSetting->cloudEnabled());
    m_config->setValue(Setting::Constants::Key::CLOUD_URL, buildSetting->cloudUrl());
    m_config->setValue(Setting::Constants::Key::CLEAR_CONSOLE_ENABLED, buildSetting->clearConsoleEnabled());
    m_config->endGroup();
    m_config->beginGroup(Setting::Constants::Key::WRITER);
    m_config->setValue(Setting::Constants::Key::MRBW_COMMAND, buildSetting->mrbwCommand());
    m_config->setValue(Setting::Constants::Key::MRBW_COMMAND_OPTS, buildSetting->mrbwCommandOptions());
    m_config->setValue(Setting::Constants::Key::MRBW_PORT, buildSetting->portName());
    m_config->endGroup();
    m_config->endGroup();


    FontColorsSetting *fontColorsSetting = m_ideSetting->fontColorsSetting();
    m_config->beginGroup(Setting::Constants::Key::FONT_AND_COLORS);
    {
        m_config->beginGroup(Setting::Constants::Key::FONT);
        m_config->setValue(Setting::Constants::Key::FONT_FAMILY, fontColorsSetting->fontFamily());
        m_config->setValue(Setting::Constants::Key::FONT_SIZE, fontColorsSetting->fontSize());
        m_config->endGroup();
    }
    m_config->setValue(Setting::Constants::Key::COLOR_SCHEME_NAME,fontColorsSetting->defaultColorSchemeName());
    m_config->endGroup();

    storeColorSchemeSetting();

    m_config->sync();

    // The modify flag for all of settings is false
    // because all of those settings and a setting file are synchronized.
    m_ideSetting->setModified(false);

    emit settingChanged(Setting::ControlType::STORED);
}

void IdeSettingControl::load()
{
    qDebug();
    ProjectSetting *projectSetting = m_ideSetting->projectSetting();
    QString projectLocation = m_config->value("Projects/LastLocation").toString();
    if (projectLocation == Q_NULLPTR) {
        projectLocation = IdeSettingControl::defaultProjectLocation();
    }
    projectSetting->setLastLocation(projectLocation);

    QString defaultProjectLocation = m_config->value("Projects/DefaultLocation").toString();
    if (defaultProjectLocation == Q_NULLPTR) {
        defaultProjectLocation = projectLocation;
    }
    projectSetting->setDefaultLocation(defaultProjectLocation);

    GenericIDESetting *genericIdeSetting = m_ideSetting->gnericIdeSetting();
    bool maximum = m_config->value("IDE/Window/Maximized", false).toBool();
    genericIdeSetting->setMaximized(maximum);
    QByteArray geometry = m_config->value("IDE/Window/Geometry",QByteArray()).toByteArray();
    genericIdeSetting->setGeometry(geometry);

    BuildSetting *buildSetting = m_ideSetting->buidSetting();
    m_config->beginGroup(Setting::Constants::Key::BUILD_AND_DEPLOY);
    m_config->beginGroup(Setting::Constants::Key::COMPILER);
    buildSetting->setMrbcEnabled(m_config->value(Setting::Constants::Key::MRBC_ENABLED).toBool());
    buildSetting->setMrbcCommand(m_config->value(Setting::Constants::Key::MRBC_COMMAND).toString());
    buildSetting->setMrbcCommandOptions(m_config->value(Setting::Constants::Key::MRBC_COMMAND_OPTS).toString());
    buildSetting->setCloudEnabled(m_config->value(Setting::Constants::Key::CLOUD_ENABLED).toBool());
    buildSetting->setCloudUrl(m_config->value(Setting::Constants::Key::CLOUD_URL).toString());
    buildSetting->setClearConsoleEnabled(m_config->value(Setting::Constants::Key::CLEAR_CONSOLE_ENABLED).toBool());
    m_config->endGroup();
    m_config->beginGroup(Setting::Constants::Key::WRITER);
    buildSetting->setMrbwCommand(m_config->value(Setting::Constants::Key::MRBW_COMMAND).toString());
    buildSetting->setMrbwCommandOptions(m_config->value(Setting::Constants::Key::MRBW_COMMAND_OPTS).toString());
    buildSetting->setPortName(m_config->value(Setting::Constants::Key::MRBW_PORT).toString());
    m_config->endGroup();
    m_config->endGroup();


    FontColorsSetting *fontColorsSetting = m_ideSetting->fontColorsSetting();
    m_config->beginGroup(Setting::Constants::Key::FONT_AND_COLORS);
    {
        m_config->beginGroup(Setting::Constants::Key::FONT);
        QString fontFamily = m_config->value(Setting::Constants::Key::FONT_FAMILY).toString();
        int fontSize = m_config->value(Setting::Constants::Key::FONT_SIZE).toInt();
        fontColorsSetting->setFontFamily(fontFamily);
        fontColorsSetting->setFontSize(fontSize);
        m_config->endGroup();
    }
    fontColorsSetting->setDefaultColorSchemeName(m_config->value(Setting::Constants::Key::COLOR_SCHEME_NAME).toString());
    m_config->endGroup();

    loadColorSchemeSetting();

    // The modify flag for all of settings is false
    // because all of those settings and a setting file are synchronized.
    m_ideSetting->setModified(false);

    emit settingChanged(Setting::ControlType::LOADED);
}

void IdeSettingControl::reset()
{
    qDebug();
    m_ideSetting->clear();
    load();
}

bool IdeSettingControl::isModified()
{
    return m_ideSetting->isModified();
}

IdeSetting* IdeSettingControl::ideSetting()
{
    return m_ideSetting;
}

void IdeSettingControl::update(Setting::IDESettingGroup settingGroup)
{
    switch (settingGroup) {
    case Setting::IDESettingGroup::Projects: {
        ProjectSetting *projectSetting = m_ideSetting->projectSetting();
        m_config->setValue("Projects/LastLocation", projectSetting->lastLocation());
        m_config->setValue("Projects/DefaultLocation", projectSetting->defaultLocation());
        m_config->sync();
        break;
    }
    case Setting::IDESettingGroup::GenericIDE: {
        GenericIDESetting *genericIdeSetting = m_ideSetting->gnericIdeSetting();
        m_config->setValue("IDE/Window/Maximized", genericIdeSetting->maximized());
        m_config->setValue("IDE/Window/Geometry", genericIdeSetting->geometry());
        m_config->sync();
        break;
    }
    default:
        break;
    }
}

QString IdeSettingControl::defaultProjectLocation()
{
    QString projectLocation;
#if defined(Q_OS_MAC)
    QString applicationDirPath = QCoreApplication::applicationDirPath();
    QDir applicationDir(applicationDirPath);
    applicationDir.cdUp();
    applicationDir.cdUp();
    applicationDir.cdUp();
    QString applicationName = QCoreApplication::applicationName();
    QString bundleDirName = applicationName.append(".app");
    if (applicationDir.exists(bundleDirName)) {
        projectLocation = applicationDir.absolutePath();
    } else {
        projectLocation = applicationDirPath;
    }
#else // windows/linux
    projectLocation = QCoreApplication::applicationDirPath();
#endif
    return projectLocation;
}
