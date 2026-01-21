/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "linenumberarea.h"
#include "mrctexteditor.h"

LineNumberArea::LineNumberArea(MrcTextEditor *editor)
    : QWidget(editor) {
    mrcTextEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(mrcTextEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    mrcTextEditor->lineNumberAreaPaintEvent(event);
}

void LineNumberArea::mousePressEvent(QMouseEvent *event)
{
    mrcTextEditor->lineNumberAreaMarkEvent(event);
}
