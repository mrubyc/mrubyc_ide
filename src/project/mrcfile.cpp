/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "mrcfile.h"
#include "mrctextdocument.h"

#include <QFileInfo>
#include <QDebug>

MrcFile::MrcFile(QObject *parent)
    :MrcAbstractFile(parent)
    ,m_editing(false)
    ,m_textDocument(Q_NULLPTR)
    ,m_highlighter(Q_NULLPTR)
{

}
MrcFile::MrcFile(const QString &filePath, QObject *parent)
    :MrcAbstractFile(parent)
    ,m_editing(false)
    ,m_textDocument(Q_NULLPTR)
    ,m_highlighter(Q_NULLPTR)
{
    setFilePath(filePath);
}

MrcFile::~MrcFile()
{
    qDebug() << path();
}

void MrcFile::setFilePath(const QString &filePath)
{
    setFileType(mrcFileType(filePath));
    setPath(filePath);

    QFileInfo fileInfo(filePath);
    QString fileName = QString(fileInfo.fileName());
    setName(fileName);
}

MrcTextDocument *MrcFile::textDocument() const
{
    return m_textDocument;
}

void MrcFile::openTextDocument(MrcTextDocument *textDocument, bool editing)
{
    if (textDocument == Q_NULLPTR) return;

    textDocument->setMrcFile(this);
    textDocument->setModified(false);

    if (!m_markList.isEmpty()) {
        // Remove a mark more than a line number of file.
        // a mark is zero origin.
        int lineCount = textDocument->lineCount();
        while (!m_markList.isEmpty() && lineCount < m_markList.last() + 1) {
            m_markList.removeLast();
        }
    }
    textDocument->setMarkList(m_markList);

    m_markList.clear();
    m_editing = editing;
    m_textDocument = textDocument;
}

void MrcFile::closeTextDocument()
{
    if (m_textDocument) {
        // Set null for mrcfile reference.
        m_textDocument->setMrcFile(Q_NULLPTR);
        m_textDocument = Q_NULLPTR;
    }
    m_markList.clear();
    m_editing = false;
}

ColorSchemeHighlighter *MrcFile::highlighter() const
{
    return m_highlighter;
}

void MrcFile::setHighlighter(ColorSchemeHighlighter *highlighter)
{
    m_highlighter = highlighter;
}

bool MrcFile::isEditing()
{
    return m_editing;
}

void MrcFile::setEditing(bool editing)
{
    m_editing = editing;
}

QList<int> MrcFile::markList() const
{
    if (m_textDocument) {
        return m_textDocument->markList();
    }
    QList <int> empty;
    return empty;
}

void MrcFile::setMarkList(const QList<int> &markList)
{
    m_markList = markList;
}

