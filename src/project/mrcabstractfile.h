/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef MRCABSTRACTFILE_H
#define MRCABSTRACTFILE_H

#include <QObject>

/*!
 * \brief The MrcAbstractFile class.
 *  This is abstract file class for a file or a project.
 */
class MrcAbstractFile : public QObject
{
    Q_OBJECT
public:
    //! MRC file type to manage in a project
    enum MrcFileType {
        //! other file.
        OTHER = 0,
        //! MRCP file
        MRCP = 1,
        //! ruby source file.
        RUBY = 2,
        //! ruby compiled file
        MRB = 100,
        //! normal folder.
        FOLDER = 253,
        //! temporary file
        TMP_FILE = 254,
        //! project folder.
        PROJECT = 255
    };
    //! suffix definition for a mrcp file.
    static const QString SUFFIX_MRCP;
    //! suffix definition for a ruby source file.
    static const QString SUFFIX_RUBY;
    //! suffix definition for a ruby compiled file.
    static const QString SUFFIX_MRB;

public:
    explicit MrcAbstractFile(QObject *parent = Q_NULLPTR);
    ~MrcAbstractFile();

    /*!
     * \brief Return the name.
     * \return the name.
     */
    QString name() const;
    /*!
     * \brief Set the name.
     * \param name the name
     */
    void setName(const QString &name);

    /*!
     * \brief Return the absolute path.
     * \return path.
     */
    QString path() const;
    /*!
     * \brief Set the absolute path.
     * \param filePath
     */
    void setPath(const QString &path);


    MrcFileType fileType() const;
    void setFileType(const MrcFileType &fileType);

    /*!
     * \brief Check a kind of file whether it is normal.
     * \return if nomal, true. otherwise, false.
     */
    bool isNormalFile();

    /*!
     * \brief Check a kine of file whether it is ruby source file.
     * \return Return true if a ruby source file. Otherwise, return false.
     */
    bool isRubySourceFile();

    /*!
     * \brief Check a kine of file whether it is ruby source file.
     * \return Return true if a mrcp project file. Otherwise, return false.
     */
    bool isMrcpFile();

    /*!
     * \brief Check a kind of file whether it is special.
     * \return if nomal, true. otherwise, false.
     */
    bool isSpecialFile();

    /*!
     * \brief Check a kind of file whether it is a foler.
     * \return if nomal, true. otherwise, false.
     */
    bool isFolder();

    /*!
     * \brief Get the file type from the filePath.
     * \param filePath the filePath.
     * \return file type.
     */
    static MrcFileType mrcFileType(const QString &filePath);
private:
    //! name for object.
    QString m_name;
    //! path for object. It is include in the name.
    QString m_path;

    //! file type.
    MrcFileType m_fileType;
};

#endif // MRCABSTRACTFILE_H
