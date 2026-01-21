/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "idesetting.h"
#include "abstractsetting.h"
#include "buildsetting.h"
#include "fontcolorssetting.h"
#include "genericidesetting.h"
#include "projectsetting.h"

#include <QDebug>

IdeSetting::IdeSetting(QObject *parent)
    :AbstractSetting(parent)
{
    m_buildSetting = new BuildSetting(this);
    m_fontColorsSetting = new FontColorsSetting(this);
    m_gnericIdeSetting = new GenericIDESetting(this);
    m_projectSetting = new ProjectSetting(this);
}

IdeSetting::~IdeSetting()
{
}

BuildSetting *IdeSetting::buidSetting() const
{
    return m_buildSetting;
}

FontColorsSetting *IdeSetting::fontColorsSetting() const
{
    return m_fontColorsSetting;
}

GenericIDESetting *IdeSetting::gnericIdeSetting() const
{
    return m_gnericIdeSetting;
}

ProjectSetting *IdeSetting::projectSetting() const
{
    return m_projectSetting;
}

void IdeSetting::clear()
{
    m_buildSetting->clear();
    m_fontColorsSetting->clear();
}

bool IdeSetting::isModified()
{
    bool modify = false;
    modify = m_buildSetting->isModified();
    if (modify) return modify;

    modify = m_fontColorsSetting->isModified();
    if (modify) return modify;
    return modify;
}

void IdeSetting::setModified(bool yes)
{
    m_buildSetting->setModified(yes);
    m_fontColorsSetting->setModified(yes);
}
