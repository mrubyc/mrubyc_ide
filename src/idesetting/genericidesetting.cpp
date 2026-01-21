/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

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
