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
