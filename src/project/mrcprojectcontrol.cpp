/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "mrcprojectcontrol.h"
#include "mrubyc_ide.h"
#include "mrcproject.h"
#include "mrcfile.h"
#include "mrctextdocument.h"
#include "mrcprojectfilecontrol.h"
#include "colorschemehighlighter.h"

#include <QStringList>
#include <QDateTime>
#include <QSettings>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDir>
#include <QPlainTextDocumentLayout>
#include <QDebug>

MrcProjectControl::MrcProjectControl(QObject *parent)
    :QObject(parent)
{
}

MrcProjectControl::~MrcProjectControl()
{
}

bool MrcProjectControl::createNewProject(MrcProject *mrcProject, const QString &projectPath, const QString &projectName)
{
    QDir projectPathDir(projectPath);
    if (!projectPathDir.mkpath(projectPath)) {
        qWarning() << tr("Cannot create the directory: %1").arg(projectPath);
        return false;
    }

    mrcProject->setName(projectName);
    mrcProject->setPath(projectPath);

    // save project file...
    saveToProject(mrcProject);
    return true;
}

bool MrcProjectControl::importExistingProject(MrcProject *mrcProject, const QString &projectPath, const QString &projectName, QList<QFileInfo> files)
{
    mrcProject->setName(projectName);
    mrcProject->setPath(projectPath);

    // Add selected files into a project.
    for (int i=0; i<files.size(); i++) {
        QFileInfo fileInfo = files.at(i);
        QString filePath = fileInfo.canonicalFilePath();
        bool adding = false;
        MrcFile *mrcFile = addFile(mrcProject, filePath, &adding, false, false);
        if (mrcFile == Q_NULLPTR) {
            qWarning() << tr("Cannot add the file: %1").arg(filePath);
            continue;
        } else if (adding == false && mrcFile != Q_NULLPTR) {
            // Check already registered file.
            continue;
        }
    }

    // save project file...
    saveToProject(mrcProject);
    return true;
}

MrcFile *MrcProjectControl::addNewFile(MrcProject *mrcProject, const QString &fullPath, bool *adding, bool editing, bool storing)
{
    QFileInfo fileInfo(fullPath);
    QDir parentDir = fileInfo.dir();
    if (!parentDir.exists()) {
        QString parentPath = parentDir.path();
        parentDir.mkpath(parentPath);
    }

    QFile newFile(fullPath);
    if (!newFile.open(QIODevice::ReadWrite|QIODevice::Text)) {
        if (adding) *adding = false;
        return Q_NULLPTR;
    }

    newFile.close();
    qDebug() << "Project:" << mrcProject << "Name:" << fullPath;
    MrcFile *mrcFile = addFile(mrcProject, fullPath, adding, editing, storing);
    return mrcFile;
}

bool MrcProjectControl::openProject(MrcProject *mrcProject, const QString &projectFileName)
{

    QFileInfo mrcpFileInfo(projectFileName);
    QString projectPath = mrcpFileInfo.absolutePath();
    QString projectName = mrcpFileInfo.completeBaseName();
    mrcProject->setName(projectName);
    mrcProject->setPath(projectPath);

    // load project file...
    loadFromProject(mrcProject);

    return true;
}

void MrcProjectControl::closeProject(MrcProject *mrcProject)
{
    // save the project to a file.
    saveToProject(mrcProject);
}

void MrcProjectControl::saveFiles(MrcProject *mrcProject, QList<MrcFile *> mrcfileList)
{
    for (int i=0; i<mrcfileList.size(); i++) {
        MrcFile *mrcFile = mrcfileList.at(i);
        saveFile(mrcProject, mrcFile);
    }
}

QList<MrcFile *> MrcProjectControl::modifiedFiles(MrcProject *mrcProject)
{
    QList<MrcFile*> mrcFileList;

    QList<MrcFile*> editingMrcFileList = mrcProject->editingMrcFileList();
    for (int i=0; i<editingMrcFileList.size(); i++) {
        MrcFile *mrcFile = editingMrcFileList.at(i);
        if (mrcFile->textDocument()->isModified()) {
            mrcFileList.append(mrcFile);
        }
    }
    return mrcFileList;
}

void MrcProjectControl::removeFile(MrcProject *mrcProject, MrcFile *mrcFile, bool permanence)
{
    // Remove a file from a project file.
    QString projectPath = mrcProject->path();
    QString projectName = mrcProject->name();
    QString filePath = mrcFile->path();
    MrcProjectFileControl projectFileControl(projectPath, projectName, this);
    bool removeProjectFlag = projectFileControl.remove(filePath);
    if (removeProjectFlag)
        qWarning() << tr("Somthing issue:") << tr("Do not remove %1 from a project file").arg(mrcFile->name());

    // Remove a file from a file system.
    if (permanence) {
        QFile file(filePath);
        if (!file.remove())
            qWarning() << tr("Cannot remove the file: %1").arg(mrcFile->name());
    }
    mrcProject->removeMrcFile(filePath);
    delete mrcFile;
}

MrcFile *MrcProjectControl::duplicateFile(MrcProject *mrcProject, const QString &filePath)
{
    QFileInfo origFileInfo(filePath);
    if (!origFileInfo.isFile()) return Q_NULLPTR;

    QString path = origFileInfo.path();
    QString baseName = origFileInfo.completeBaseName();
    QString completeSuffix = origFileInfo.suffix();

    // check new file does not exist in a file system
    // and copy from the file to a new file.
    int number = 0;
    QString newFileName;
    QFile origFile(filePath);
    while (number<MRUBYC_IDE::MAX_DUP_FILES) {
        QString incNum;
        if (number > 0) incNum = QString::number(number);
        newFileName += path + "/" + baseName + "_copy" + incNum + "." + completeSuffix;
        QFileInfo info(newFileName);
        if (!info.exists() && !info.isSymLink()) break;

        newFileName.clear();
        number++;
    }

    if (newFileName.isNull()) return Q_NULLPTR;

    // copy a file.
    if (!origFile.copy(newFileName)) return Q_NULLPTR;

    qDebug() << "Name:" << newFileName;
    bool adding = false;
    MrcFile *mrcFile = addFile(mrcProject, newFileName, &adding, false);
    // if adding is false, MRC file cannot be added. Thus, return NULL.
    if (!adding) mrcFile = Q_NULLPTR;
    return mrcFile;
}

bool MrcProjectControl::renameFile(MrcProject *mrcProject, MrcFile *mrcFile, const QString &name)
{

    QString oldFilePath = mrcFile->path();
    QFileInfo fileInfo(oldFilePath);
    QDir dir = fileInfo.absolutePath();

    // check that new file does not exist.
    QFileInfo newFileInfo(dir, name);
    if (newFileInfo.exists() || newFileInfo.isSymLink()) return false;

    QString newFilePath = newFileInfo.absoluteFilePath();
    mrcFile->setFilePath(newFilePath);

    // Rename the file.
    QFile oldFile(oldFilePath);
    bool renameStatus = false;
    renameStatus = oldFile.rename(newFilePath);
    if (!renameStatus) return false;

    // Remap a MRC file.
    mrcProject->removeMrcFile(oldFilePath);
    mrcProject->addMrcFile(newFilePath, mrcFile);

    // Rename a file from old name to new name in the project file.
    QString projectPath = mrcProject->path();
    QString projectName = mrcProject->name();
    MrcProjectFileControl projectFileControl(projectPath, projectName, this);
    renameStatus = false;
    renameStatus = projectFileControl.rename(oldFilePath, newFilePath);
    qDebug() << "Status:" << renameStatus << "Rename the file:"<< oldFilePath << "to" << newFilePath;
    return true;
}

bool MrcProjectControl::editFile(MrcFile *mrcFile)
{
    qDebug() << "IMPLEMENTED" << mrcFile->name();

    if (mrcFile->textDocument() != Q_NULLPTR) {
        // THe file is already editing.
        return true;
    }

    QString name = mrcFile->path();
    // check that a file exists.
    QFileInfo fileInfo(name);
    if (!fileInfo.isFile()
            || !fileInfo.isWritable()) {
        return false;
    }

    // A parent is set here temporally.
    // It should be set a MrcTextEdit.
    // load data from a file.
    QString filePath = fileInfo.filePath();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false; // FIMXE: if issue, return false?
    }
    MrcTextDocument *textDocument = new MrcTextDocument(this);

    textDocument->setPlainText(file.readAll());
    // setup layout
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(textDocument);
    textDocument->setDocumentLayout(layout);
    layout->requestUpdate();

    // The file has mark list in the project.
    mrcFile->openTextDocument(textDocument);
    // setup highlighter
    if (mrcFile->isRubySourceFile()) {
        mrcFile->setHighlighter(new ColorSchemeHighlighter(textDocument));
    }
    return true;
}

MrcFile *MrcProjectControl::addFile(MrcProject *mrcProject, const QString &name, bool *adding, bool editing, bool storing)
{
    MrcFile *mrcFile = mrcProject->findMrcFile(name);
    if (mrcFile != Q_NULLPTR) {
        // The file is alreday registered.
        if (adding) *adding = false;
        return mrcFile;
    }

    // check that a file exists.
    QFileInfo fileInfo(name);
    if (!fileInfo.isFile()
            || !fileInfo.isReadable()) {
        if (adding) *adding = false;
        return Q_NULLPTR;
    }
    //! The parent of MrcFile is MrcProject.
    mrcFile = new MrcFile(name, mrcProject);

    // Setup MRC Text document.
    MrcTextDocument *document = Q_NULLPTR;
    if (editing) {
        bool status = MrcProjectControl::editFile(mrcFile);
        if (status == false) {
            delete mrcFile;
            if (adding) *adding = false;
            return Q_NULLPTR;
        }
        document = mrcFile->textDocument();
        mrcFile->openTextDocument(document);
    }

    // Setup for MRC file
    mrcProject->addMrcFile(name, mrcFile);

    if (storing) {
        // Add a file into a project file.
        QString projectPath = mrcProject->path();
        QString projectName = mrcProject->name();
        MrcProjectFileControl projectFileControl(projectPath, projectName, this);
        projectFileControl.append(mrcFile);
    }

    if (adding) *adding = true;
    return mrcFile;
}

void MrcProjectControl::closeFile(MrcProject *mrcProject, MrcFile *mrcFile, bool storing)
{
    // if storing is true, save the file to file system.
    if (storing) saveFile(mrcProject, mrcFile);

    mrcFile->closeTextDocument();
}

bool MrcProjectControl::saveFile(MrcProject *mrcProject, MrcFile *mrcFile)
{
    MrcTextDocument *document = mrcFile->textDocument();

    if (document == Q_NULLPTR) {
        // nothing...
        return false;
    }
    if (!document->isModified()) {
        qDebug() << "No modified file:" << mrcFile->name() << "status:" << document->isModified();
        return false;
    }

    QString filePath = mrcFile->path();
    // check that a file exists.
    QFileInfo fileInfo(filePath);
    if (!fileInfo.isFile()
            || !fileInfo.isWritable()) {
        qCritical() << "No such file or No writable file:" << "File:" << filePath << " Writable:"<<fileInfo.isWritable();
        return false;
    }

    // save document to a file.
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Cannot open the file:" << "File:" << filePath;
        return false;
    }
    file.write(document->toPlainText().toUtf8());
    file.close();

    Q_UNUSED(mrcProject)
    // When save a file, the file stores only from a text document to a file system.
//    // Update a file information to a project file.
//    QString projectPath = mrcProject->path();
//    QString projectName = mrcProject->name();
//    MrcProjectFileControl projectFileControl(projectPath, projectName, this);
//    bool updateFlag = projectFileControl.update(mrcFile);
//    if (!updateFlag) qWarning() << "Cannot update a file:" << filePath;

    document->setModified(false);
    return true;
}

void MrcProjectControl::saveToProject(MrcProject *mrcProject)
{
    qDebug() << mrcProject;
    QString projectPath = mrcProject->path();
    QString projectName = mrcProject->name();
    MrcProjectFileControl projectFileControl(projectPath, projectName, this);
    projectFileControl.save(mrcProject);
}

bool MrcProjectControl::loadFromProject(MrcProject *mrcProject)
{
    qDebug() << mrcProject;

    QString projectPath = mrcProject->path();
    QString projectName = mrcProject->name();
    MrcProjectFileControl projectFileControl(projectPath, projectName, this);
    return projectFileControl.load(mrcProject);
}
