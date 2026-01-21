/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "waitingspinner.h"

#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QDebug>

WaitingSpinner::WaitingSpinner(QWidget *parent) : QLabel(parent)
{
    m_movie = new QMovie("://icons/spinners.gif");
    setMovie(m_movie);
    setWindowOpacity(0.5);
    hide();
}

WaitingSpinner::~WaitingSpinner()
{
    delete m_movie;
}

void WaitingSpinner::start()
{
    if (isVisible()) {
        return;
    }
    QWidget *parent = parentWidget();

    QPoint pos = this->pos();
    pos.setX((parent->width() - width())/2);
    pos.setY((parent->height() - height())/2);
    move(pos);
//    Qt::WindowFlags flags = 0;
//    flags |= Qt::X11BypassWindowManagerHint;
//    this->setWindowFlags(flags);

    m_movie->start();
    emit started();
}

void WaitingSpinner::stop()
{
    if (!isVisible()) {
        return;
    }
    m_movie->stop();
    emit stopped();
}
