#ifndef MRCPROJECT_H
#define MRCPROJECT_H

#include "mrcabstractfile.h"

#include <QObject>
#include <QMap>

class MrcFile;
class MrcTextDocument;

/*!
 * \brief The MrcProject class.
 * This is a mrc project file for mrubyc_ide.
 */
class MrcProject : public MrcAbstractFile
{
    Q_OBJECT
public:
    explicit MrcProject(QObject *parent = Q_NULLPTR);
    ~MrcProject();

    /*!
     * \brief Return the project name.
     * \return the project name.
     */
    QString name() const;
    /*!
     * \brief Set the project name.
     * \param name the project name
     */
    void setName(const QString &name);

    /*!
     * \brief Return the version for a MRCP format.
     * \return version.
     */
    QString version() const;
    /*!
     * \brief Set the version.
     * \param version setting version.
     */
    void setVersion(const QString &version);

    /*!
     * \brief Return the saved timeStamp for a MRCP project file.
     * \return timestamp.
     */
    QString timeStamp() const;
    /*!
     * \brief Set the saving timeStamp.
     * \param timeStamp
     */
    void setTimeStamp(const QString &timeStamp);

    /*!
     * \brief Return whether the file is included in a project.
     * \param name the checking file.
     * \return If a project has the file, return ture. Otherwise, false.
     */
    bool hasFile(const QString &name);

    /*!
     * \brief Return whether the file is opening on a project.
     * \param name the file name.
     * \return if openging file, ture. otherwise, false.
     */
    bool hasOpeningFile(QString name);

    /*!
     * \brief Return the MRC file list in a project.
     * \return file list.
     */
    QList<MrcFile*> projectMrcFileList();
    /*!
     * \brief Return the MRC editing file in a project.
     * \return editing file list.
     */
    QList<MrcFile*> editingMrcFileList();

    /*!
     * \brief Find the MRC file from a project.
     * \param fullPath fullpath for file
     * \return If a file is found, the MRC file. Otherwise,
     */
    MrcFile* findMrcFile(QString fullPath);

    /*!
     * \brief Add MrcFile to manage this project.
     * \param fullPath full path for an adding file.
     * \param mrcFile mrcFile for an adding file.
     */
    void addMrcFile(const QString &fullPath, MrcFile* mrcFile);

    /*!
     * \brief Remove MrcFile from this project.
     * \param fullPath full path for a removing file.
     * \return if removed MrcFile, return true. otherwise, false.
     */
    bool removeMrcFile(const QString &fullPath);

public:
    /*!
     * \brief Get the MRCP file path.
     * \param path the project path
     * \param name the project name
     * \return MRCP file path
     */
    static QString mrcpFilePath(const QString &path, const QString &name);

    /*!
     * \brief Return the relative file name.
     * \param projectPath the project file path.
     * \param filePath the file path.
     * \return
     */
    static QString relativePath(const QString &projectPath, const QString &filePath);

    /*!
     * \brief Return the absolute file name.
     * \param projectPath the project file path.
     * \param fileRelativePath the file relative path.
     * \return
     */
    static QString absolutePath(const QString &projectPath, const QString &fileRelativePath);

    /*!
     * \brief Return mrb filePath.
     * \param filePath the ruby source file path.
     * \return mrb file path.
     */
    static QString mrbFilePath(const QString& filePath);
private:
    //! file version for MRCP
    QString m_version;
    //! timestamp in MRCP file
    QString m_timeStamp;

    //! The key is fullpath for a file. And the value is MRC file.
    QMap<QString,MrcFile*> m_mrcFileMap;

};

#endif // MRCPROJECT_H
