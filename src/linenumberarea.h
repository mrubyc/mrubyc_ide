#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QSize>

class MrcTextEditor;

/*!
 * \brief LineNumber and Mark Area class
 *
 *  This class has two functions.
 *  One function is to display a line number.
 *  Other, to put a mark or an unmark.
 */
class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(MrcTextEditor *editor);

    QSize sizeHint() const;

protected:
    /*!
     * \brief Paint the area.
     * \param event
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Handle to put the mark and unmark when the mouse is pressed.
     * \param event
     */
    void mousePressEvent(QMouseEvent *event);

private:
    MrcTextEditor *mrcTextEditor;
};

#endif // LINENUMBERAREA_H
