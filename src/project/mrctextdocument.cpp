#include "mrctextdocument.h"
#include "fontcolorssetting.h"
#include <QDebug>

MrcTextDocument::MrcTextDocument(QObject *parent)
    :QTextDocument(parent)
{

}

MrcTextDocument::~MrcTextDocument()
{

}

MrcFile *MrcTextDocument::mrcFile() const
{
    return m_mrcFile;
}

void MrcTextDocument::setMrcFile(MrcFile *mrcFile)
{
    m_mrcFile = mrcFile;
}

QString MrcTextDocument::timeStamp() const
{
    return m_timeStamp;
}

void MrcTextDocument::setTimeStamp(const QString &timeStamp)
{
    m_timeStamp = timeStamp;
}

QList<int> MrcTextDocument::markList() const
{
    return m_markList;
}

void MrcTextDocument::setMarkList(const QList<int> &markList)
{
    m_markList = markList;
}

void MrcTextDocument::mark(bool yes, int pos)
{
    qDebug() << yes << pos;
    if (yes) {
        int index = 0;
        for (; index < m_markList.size(); index++) {
            if (m_markList.at(index) > pos) break;
        }
        m_markList.insert(index, pos);
    } else {
        m_markList.removeAll(pos);
    }
}

