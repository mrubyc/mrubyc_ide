#ifndef MRCFILE_H
#define MRCFILE_H

#include "mrcabstractfile.h"
#include <QObject>
#include <QList>

class MrcTextDocument;
class ColorSchemeHighlighter;

/*!
 * \brief The MrcFile class.
    It is abstract file in a mrc project.
 */
class MrcFile : public MrcAbstractFile
{
    Q_OBJECT
public:
    explicit MrcFile(QObject *parent = Q_NULLPTR);
    explicit MrcFile(const QString &filePath, QObject *parent = Q_NULLPTR);
    ~MrcFile();

public:
    /*!
     * \brief Set the absolute file path.
     * \param filePath
     */
    void setFilePath(const QString &filePath);

    /*!
     * \brief Return MrcTextDocument.
     * \return if it does not set a document by setTextDocument, reurn null.
     *         Otherwise, return a document.
     */
    MrcTextDocument *textDocument() const;
    /*!
     * \brief Set TextDocument into this file in order to open.
     *  It enables be ready to edit.
     * \param textDocument text document to open.
     * \param editing default value is true.
     */
    void openTextDocument(MrcTextDocument *textDocument, bool editing = true);
    /*!
     * \brief Close a TextDocument if it has a text document.
     */
    void closeTextDocument();

    /*!
     * \brief Return a highlighter reference.
     * \return if it has a highlighter ref., return a highlighter reference.
     *  Otherwise, return null.
     */
    ColorSchemeHighlighter *highlighter() const;
    /*!
     * \brief Set a Highlighter.
     * \param highlighter the highligher reference.
     */
    void setHighlighter(ColorSchemeHighlighter *highlighter);

    /*!
     * \brief Is the file Editing?
     * \return If editting, true. otherwise, false.
     */
    bool isEditing();
    /*!
     * \brief Set a editing flag.
     * \param editing the editing for a file.
     */
    void setEditing(bool editing);

    /*!
     * \brief Return markList.
     * \return if it has a text document, return a mark's list of its document.
     *         Otherwise, empty list.
     */
    QList<int> markList() const;

    /*!
     * \brief Set a mark list that it will be used in openTextDocument()
     * \param markList the mark list.
     */
    void setMarkList(const QList<int> &markList);

private:
    //! it is flag whether this file is editing.
    bool m_editing;
    //! \brief It is in use temporarily.
    QList<int> m_markList;

    //! text document.
    MrcTextDocument* m_textDocument;

    //! syntax highlighter
    ColorSchemeHighlighter *m_highlighter;
};

#endif // MRCFILE_H
