#include "mrctexteditor.h"
#include "linenumberarea.h"
#include "mrubyc_ide.h"

#include <QtWidgets>
#include <QDebug>

MrcTextEditor::MrcTextEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &MrcTextEditor::blockCountChanged, this, &MrcTextEditor::updateLineNumberAreaWidth);;
    connect(this, &MrcTextEditor::updateRequest, this, &MrcTextEditor::updateLineNumberArea);

    updateLineNumberAreaWidth(0);
}

MrcTextEditor::~MrcTextEditor()
{
}

int MrcTextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    if (MRUBYC_IDE::DEFAULT_NUM_DIGITS > digits)
        digits = MRUBYC_IDE::DEFAULT_NUM_DIGITS;

    int space = MRUBYC_IDE::LINE_NUM_AREA_MARGIN + fontMetrics().width(QLatin1Char('8')) * ( digits + MRUBYC_IDE::MARK_CHAR_NUM );

    return space;
}

void MrcTextEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void MrcTextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void MrcTextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    updateLineNumberAreaWidth(0);
}


void MrcTextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
            // circle is always diplayed.
            if (!markedNumList.isEmpty() && markedNumList.contains(blockNumber)) {
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setPen(Qt::red);
                painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                painter.drawEllipse(2, top,
                                    fontMetrics().width(QLatin1Char('8'))*MRUBYC_IDE::MARK_CHAR_NUM-4,
                                    fontMetrics().height()-4);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void MrcTextEditor::lineNumberAreaMarkEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && block.isVisible() ) {
        if (top <= pos.y() && pos.y() <= bottom) {
            break;
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

    bool enableMarkFlag = false;
    if (markedNumList.contains(blockNumber)) {
        // If found, remove the mark from a list.
        markedNumList.removeOne(blockNumber);
        enableMarkFlag = false;
    } else {
        // If not found, add a mark to the list.
        int idx = 0;
        for (; idx < markedNumList.size(); idx++) {
            if (markedNumList.at(idx) > blockNumber) break;
        }
        markedNumList.insert(idx, blockNumber);
        enableMarkFlag = true;
    }
    emit marked(enableMarkFlag, blockNumber);
    update();
}

void MrcTextEditor::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    QTextCursor	cur = textCursor();
    QTextBlock block = cur.block();
    int lineNum = block.blockNumber(); // get the cursor line number.
    int curCol = cur.columnNumber();

    if (key == Qt::Key_Return) {
        bool updateFlag = false;
        // if a lineNum is rather than a value of element for list,
        // or lineNum is equal to a value and a cursor column is zero,
        // increment the value.
        QList<int>::iterator i = markedNumList.begin();
        while (i != markedNumList.end()) {
            if ( *i > lineNum || ( *i == lineNum && curCol == 0) ) {
                (*i)++;
                updateFlag = true;
            }
            i++;
        }
        // if some values are updated, perform the update method.
        if (updateFlag) {
            update();
        }
    } else if (key == Qt::Key_Delete) {
        QString txt = document()->characterAt(cur.position());
        if (txt == QChar::ParagraphSeparator) { // check return code...
            bool updateFlag = false;
            // if a value of element for list is greater than a lineNum,
            // or a value is equals to a lineNum and curCol is zero,
            // decrement the value.
            // NOTE: If the decreced value exists in the list, remove one of values from the list.
            QList<int>::iterator i = markedNumList.begin();
            while (i != markedNumList.end()) {
                if (*i > lineNum ) {
                    if (markedNumList.contains(*i - 1)) {
                        QList<int>::iterator itr = markedNumList.erase(i);
                        if (itr == markedNumList.end()) {
                            updateFlag = true;
                            break;
                        }
                    } else {
                        (*i)--;
                    }
                    updateFlag = true;
                }
                i++;
            }
            // if some values are updated, perform the update method.
            if (updateFlag) {
                update();
            }
        }
    } else if (key == Qt::Key_Backspace) {
        QString txt = document()->characterAt(cur.position() - 1);
        if (txt == QChar::ParagraphSeparator) { // check return code...
            bool updateFlag = false;
            // if a value of element for list is greater than a lineNum,
            // or a value is equals to a lineNum and curCol is zero,
            // decrement the value.
            // NOTE: If the decreced value exists in the list, remove one of values from the list.
            QList<int>::iterator i = markedNumList.begin();
            while (i != markedNumList.end()) {
                if (*i > lineNum || ( *i == lineNum && curCol == 0) ) {
                    if (markedNumList.contains(*i - 1)) {
                        QList<int>::iterator itr = markedNumList.erase(i);
                        if (itr == markedNumList.end()) {
                            updateFlag = true;
                            break;
                        }
                    } else {
                        (*i)--;
                    }
                    updateFlag = true;
                }
                i++;
            }
            // if some values are updated, perform the update method.
            if (updateFlag) {
                update();
            }
        }
    }

    QPlainTextEdit::keyPressEvent(event);
}

/*============================================================================*/
void MrcTextEditor::setDocument(QTextDocument *document)
{
    if (document != this->document()) {
        QPlainTextEdit::setDocument(document);
        emit MrcTextEditor::documentChanged(document);
    }
}

/*============================================================================*/
void MrcTextEditor::setMarkedNumberList(QList<int> &list)
{
    markedNumList = list;
}

QList<int> MrcTextEditor::markedNumberList() const
{
    QList<int> list;
    for (int i=0; i<markedNumList.size(); i++) {
        list.append(markedNumList.at(i));
    }
    return list;
}

void MrcTextEditor::clearMarkedNumList()
{
    markedNumList.clear();
}
