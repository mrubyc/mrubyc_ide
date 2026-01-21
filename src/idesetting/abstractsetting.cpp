/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

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
