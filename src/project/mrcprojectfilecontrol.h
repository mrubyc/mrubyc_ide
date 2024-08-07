#ifndef MRCPROJECTFILECONTROL_H
#define MRCPROJECTFILECONTROL_H

#include <QObject>
#include <QDomDocument>

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE

class MrcFile;
class MrcProject;

/*!
 * \brief The MrcProjectFileControl class.
 * It is the class that manages project file.
 */
class MrcProjectFileControl : public QObject
{
    Q_OBJECT
public:
    explicit MrcProjectFileControl(QString projectPath, QString projectName, QObject *parent = Q_NULLPTR);
    ~MrcProjectFileControl();
public:
    /*!
     * \brief Return size of files in project.
     * \return files size.
     */
    int size();

    /*!
     * \brief Append the file in the project.
     *  Is is only appended a file to a project file.
     * \param mrcFile
     * \return if appened, true. Otherwise, false.
     */
    bool append(MrcFile *mrcFile);

    /*!
     * \brief Remove the file from the project.
     * *  Is is only removed a file from a project file.
     * \param filePath removing file name.  It must be absolute path.
     * \return If removed, true. Otherwise, false.
     */
    bool remove(const QString &filePath);

    /*!
     * \brief Rename a registered file to a renaming name.
     * \param fromFilePath resigtered file name. It must be absolute path.
     * \param toFilePath renaming file name. It must be absolute path.
     * \return If renamed, true. Otherwise, false.
     */
    bool rename(const QString &fromFilePath, const QString &toFilePath);

    /*!
     * \brief Uppdate a file infomatin by mrcFile.
     *       The updating file should be registered in a project.
     * \param mrcFile updating file information.
     * \return If updated, true. Otherwise, false.
     */
    bool update(MrcFile *mrcFile);

    /*!
     * \brief Save MRC Project to a project file.
     * \param mrcProject target MRC Project
     */
    void save(MrcProject *mrcProject);
    /*!
     * \brief Load MRC Project from a project file.
     * \param mrcProject target MRC Project.
     * \return
     */
    bool load(MrcProject *mrcProject);
protected:

    static QString toMarkString(MrcFile *mrcFile);

    MrcFile *addFile(const QString &name);

    /*!
     * \brief Find a dom node that an attribute of name for a file tag is filePath(relative).
     * \param document the searching target dom document.
     * \param filePath path name.
     * \param ok [OUT] return false if something issue. Otherwise, true.
     * \return found dom node. If it is not found or something issue, return dom node which is null.
     */
    QDomNode find(QDomDocument *document, const QString &filePath, bool *ok = Q_NULLPTR);

    /*!
     * \brief Append MrcFile as dom node into a files dom node on the dom document.
     * \param document
     * \param mrcFile
     * \return true if dom node is added normally. Otherwise, false.
     */
    bool appendDomNode(QDomDocument *document, MrcFile *mrcFile);

    /*!
     * \brief Write Xml Document to the devince.
     * \param device writing device
     * \param document writing dom document
     */
    void writeXmlDocument(QIODevice *device, QDomDocument *document);

    /*!
     * \brief Read from XML document from the device.
     * \param device the device for a xml file.
     * \param document [OUT] dom document for a xml.
     * \param mrcProject [OUT] MRC project for a dom document.
     * \return false if any issues have been occurred. Otherwise, true.
     */
    bool readXmlDocument(QIODevice *device, QDomDocument *document, MrcProject* mrcProject = Q_NULLPTR);

private:
    QString m_projectPath;
    QString m_projectName;
    QString m_mrcpFilePath;
};

#endif // MRCPROJECTFILECONTROL_H
