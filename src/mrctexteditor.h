/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef MRCTEXTEDITOR_H
#define MRCTEXTEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QList>

/*!
 * \brief TextEditor class
 *
 *  This class has two additional functions to QPlainTextEdit.
 *  One function is to display a line number.
 *  Other, to put a mark or an unmark.
 */
class MrcTextEditor : public QPlainTextEdit
{
        Q_OBJECT
public:
    explicit MrcTextEditor(QWidget *parent = Q_NULLPTR);

    ~MrcTextEditor();

    /*!
     * \brief Calculate a with of the line number area.
     * \return with by pixels.
     */
    int lineNumberAreaWidth();
    /*!
     * \brief Paint the line numbers and the marks on the line number area.
     * \param event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    /*!
     * \brief Handle to put the mark and unmark when the mouse is pressed.
     * \param event
     */
    void lineNumberAreaMarkEvent(QMouseEvent *event);

    /*!
     * \brief Return a copied list of the marked number list.
     * \return the marked number list.
     */
    QList<int> markedNumberList() const;
    /*!
     * \brief Replace the marked number list by the parameter's list.
     * \param list new marked number list.
     */
    void setMarkedNumberList(QList<int> &list);
    /*!
     * \brief Remove all element from the marked number list.
     */
    void clearMarkedNumList();

    /*!
     * \brief Set the document as new one to the editor.
     *
     * If the document is changed, it emits changeDocument signal.
     * \param document new document
     */
    void setDocument(QTextDocument *document);

protected:
    /*! TODO
     * \brief resizeEvent
     * \param event
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Move a mark position when Return, Delete, Backspace key
     *  are pressed on the editor.
     * \param event
     */
    void keyPressEvent(QKeyEvent *event);

signals:

    /*!
     * \brief This signal is emitted when a mark is put or taken.
     * \param yes if ture, when the mark is put, otherwise fasle
     * \param pos  marked line number(0 origin)
     */
    void marked(bool yes, int pos);

    /*!
     * \brief This singal is emitted when the text document is changed.
     * \param document new textdocument.
     */
    void documentChanged(QTextDocument* document);

private slots:
    /*! TODO
     * \brief Set margins around the scrolling area when the block number is changed.
     * \param newBlockCount
     */
    void updateLineNumberAreaWidth(int newBlockCount);
    /*! TODO
     * \brief updateLineNumberArea
     */
    void updateLineNumberArea(const QRect &, int);

private:
    /*!
     * \brief lineNumberArea.
     *  This is displayed a line number and mark.
     */
    QWidget *lineNumberArea;

    /*!
     * \brief markedNumList
     * This is set a line number of a mark. Its number is zero origin.
     */
    QList<int> markedNumList;

};

#endif // MRCTEXTEDITOR_H
