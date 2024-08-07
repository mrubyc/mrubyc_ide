#ifndef MRCTEXTDOCUMENT_H
#define MRCTEXTDOCUMENT_H

#include <QObject>
#include <QTextDocument>
#include <QList>

class QString;
class MrcFile;
class ColorSchemeSetting;

/*!
 * \brief The MrcTextDocument class.
 * It is a text document for mrc project.
 */
class MrcTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit MrcTextDocument(QObject *parent = Q_NULLPTR);
    ~MrcTextDocument();

    MrcFile *mrcFile() const;
    void setMrcFile(MrcFile *mrcFile);

    QString timeStamp() const;
    void setTimeStamp(const QString &timeStamp);

    QList<int> markList() const;
    void setMarkList(const QList<int> &markList);

public Q_SLOTS:
    /*!
     * \brief Put a mark.
     * \param yes if ture, when the mark is put, otherwise fasle
     * \param pos  marked line number(0 origin)
     */
    void mark(bool yes, int pos);
private:
    MrcFile *m_mrcFile;
    QString m_timeStamp;
    QList<int> m_markList;
};

#endif // MRCTEXTDOCUMENT_H
