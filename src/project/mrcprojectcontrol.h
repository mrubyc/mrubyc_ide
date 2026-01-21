/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef MRCPROJECTCONTROL_H
#define MRCPROJECTCONTROL_H

#include <QObject>
#include <QList>
#include <QFileInfo>

class MrcProject;
class MrcFile;
class MrcTextDocument;

/*!
 * \brief The MrcProjectControl class.
 * It manages the project for mrubyc.
 */
class MrcProjectControl : public QObject
{
    Q_OBJECT
public:
    explicit MrcProjectControl(QObject *parent = Q_NULLPTR);
    ~MrcProjectControl();

    /*!
     * \brief Create new project.
     *  <ol>
     *   <li> Create the project file in the directory.
     *  </ol>
     * \param mrcProject [IN/OUT] project
     * \param projectPath the project directory
     * \param projectName the project name.
     * \return if project is created successfully, return true. Otherwise, false.
     */
    bool createNewProject(MrcProject *mrcProject, const QString &projectPath, const QString &projectName);

    /*!
     * \brief Import the directory as new proejct.
     * \param mrcProject [IN/OUT] project
     * \param projectPath a imported project directory
     * \param projectName the project name.
     * \param files the list of adding files in the project.
     * \return if project is created successfully, return true. Otherwise, false.
     */
    bool importExistingProject(MrcProject *mrcProject, const QString &projectPath, const QString &projectName, QList<QFileInfo> files);

    /*!
     * \brief Create a empty file as a parameter's file in this project directory.
     * \param mrcProject target MRC project
     * \param fullPath adding new file.
     * \param adding [OUT] adding the file.
     *        If a file is added in this method, out true. Otherwise, out false.
     * \param editing If true, open to edit the file in a project. Otherwise, do not open.
     * \param storing If yes, a file saves before closing the file.
     *         Otherwise, does not save the file.
     * \return MRC file. This is added MRC file, existing MRC file, or Q_NULLPTR.
     */
    MrcFile *addNewFile(MrcProject *mrcProject, const QString &fullPath, bool *adding = Q_NULLPTR, bool editing = true, bool storing = true);

    /*!
     * \brief Open a project from the project file.
     * \param mrcProject [INOUT] target MRC project
     * \param projectFileName the peoject file name.
     * \return if project is created successfully, return true. Otherwise, false.
     */
    bool openProject(MrcProject *mrcProject, const QString &projectFileName);

    /*!
     * \brief Close the current project.
     * \param mrcProject target MRC project
     */
    void closeProject(MrcProject *mrcProject);

    /*!
     * \brief Save the MrcFile to a file in a system.
     * \param mrcProject a project
     * \param mrcfileList saving mrc file list.
     */
    void saveFiles(MrcProject *mrcProject, QList<MrcFile*> mrcfileList);

    /*!
     * \brief Save MrcFile to a file system's file if it is modified.
     * \param mrcProject a project
     * \param mrcFile saving mrc file
     * \return if the file is saved successfully, return true. Otherwise, false.
     */
    bool saveFile(MrcProject *mrcProject, MrcFile *mrcFile);

    /*!
     * \brief Get modified file list.
     * \param mrcProject a project
     * \return modified file list.
     */
    QList<MrcFile*> modifiedFiles(MrcProject *mrcProject);

    /*!
     * \brief Remove the file from the project or the system and project.
     * \param mrcProject a prject
     * \param mrcFile a removal MRC file
     * \param permanence if yes, delete it from the system and proejct.
     *       otherwise, remove it from project only.
     */
    void removeFile(MrcProject *mrcProject, MrcFile *mrcFile, bool permanence);

    /*!
     * \brief Duplicate the file.
     * \param mrcFile a project
     * \param filePath a original file Path.
     * \return if duplicated file creates, return true. otherwise, false.
     */
    MrcFile *duplicateFile(MrcProject *mrcProject, const QString &filePath);

    /*!
     * \brief Rename the file from the name of mrcFile to the name.
     * \param mrcFile a project
     * \param mrcFile source MRC file.
     * \param name modified name.
     *      It is only a file name. But it is not included in a some path.
     * \return if it is done successfully, return true. otherwise, return false.
     *          For example, the renamed file alredy exists,
     *          or the renamed file does not create some reason.
     */
    bool renameFile(MrcProject *mrcProject, MrcFile *mrcFile, const QString &name);

    /*!
     * \brief Edit the file.
     *        Load a file from a file system if it hss not been editing.
     * \param mrcFile an adding file.
     * \return if open the file, true. otherwise, false.
     */
    bool editFile(MrcFile *mrcFile);

    /*!
     * \brief Add the existing file into a project.
     *        If editing, it is loaded from a file system.
     * \param mrcProject target project
     * \param name the adding file name. it is a full path.
     * \param adding [OUT] adding the file.
     *        If a file is added in this method, out true. Otherwise, out false.
     * \param editing If true, open to edit the file in a project. Otherwise, do not open.
     * \param storing If yes, a file saves before closing the file.
     *         Otherwise, does not save the file.
     * \return MRC file. This is added MRC file, existing MRC file, or Q_NULLPTR.
     */
    MrcFile *addFile(MrcProject *mrcProject, const QString &name, bool *adding = Q_NULLPTR, bool editing = true, bool storing = true);

    /*!
     * \brief Close a file in the project.
     * \param mrcProject target project
     * \param mrcFile closing file.
     * \param storing If yes, a file saves before closing the file.
     *         Otherwise, does not save the file.
     */
    void closeFile(MrcProject *mrcProject, MrcFile *mrcFile, bool storing = false);

protected:
    /*!
     * \brief Save MRC Project to a project file.
     * \param mrcProject target MRC Project
     */
    void saveToProject(MrcProject *mrcProject);
    /*!
     * \brief Load MRC Project from a project file.
     * \param mrcProject target MRC Project.
     * \return
     */
    bool loadFromProject(MrcProject *mrcProject);
};

#endif // MRCPROJECTCONTROL_H
