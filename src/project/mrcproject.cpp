/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "mrcproject.h"
#include "mrctextdocument.h"
#include "mrcfile.h"
#include "colorschemehighlighter.h"

#include <QMap>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>

MrcProject::MrcProject(QObject *parent)
    :MrcAbstractFile(parent)
{
    setFileType(MrcFileType::PROJECT);
}

MrcProject::~MrcProject()
{
}

QString MrcProject::name() const
{
    return MrcAbstractFile::name();
}

void MrcProject::setName(const QString &name)
{
    MrcAbstractFile::setName(name);
}

QString MrcProject::version() const
{
    return m_version;
}

void MrcProject::setVersion(const QString &version)
{
    m_version = version;
}

QString MrcProject::timeStamp() const
{
    return m_timeStamp;
}

void MrcProject::setTimeStamp(const QString &timeStamp)
{
    m_timeStamp = timeStamp;
}


bool MrcProject::hasFile(const QString &name)
{
    return m_mrcFileMap.contains(name);
}

bool MrcProject::hasOpeningFile(QString name)
{
    QMap<QString,MrcFile*>::iterator mrcFileItr = m_mrcFileMap.find(name);

    Q_ASSERT(mrcFileItr == m_mrcFileMap.end());

    return  mrcFileItr.value()->isEditing();
}

QList<MrcFile*> MrcProject::projectMrcFileList()
{
    QList<MrcFile*> mrcFileList;

    QList<MrcFile*> valueList = m_mrcFileMap.values();
    for (int i=0; i<valueList.size(); i++) {
        MrcFile *mrcFile = valueList.at(i);
        mrcFileList.append(mrcFile);
    }
    return mrcFileList;
}

QList<MrcFile*> MrcProject::editingMrcFileList()
{
    QList<MrcFile*> mrcFileList;

    QList<MrcFile*> valueList = m_mrcFileMap.values();
    for (int i=0; i<valueList.size(); i++) {
        MrcFile *mrcFile = valueList.at(i);
        if (mrcFile->isEditing()) {
            mrcFileList.append(mrcFile);
        }
    }
    return mrcFileList;
}

MrcFile *MrcProject::findMrcFile(QString fullPath)
{
    QMap<QString,MrcFile*>::iterator mrcFileItr = m_mrcFileMap.find(fullPath);

    if (mrcFileItr == m_mrcFileMap.end()) {
        return Q_NULLPTR;
    } else {
        return mrcFileItr.value();
    }
}

void MrcProject::addMrcFile(const QString &fullPath, MrcFile *mrcFile)
{
    m_mrcFileMap.insert(fullPath, mrcFile);
}

bool MrcProject::removeMrcFile(const QString &fullPath)
{
    bool removal = false;
    QMap<QString,MrcFile*>::iterator mrcFileItr = m_mrcFileMap.find(fullPath);
    if (mrcFileItr != m_mrcFileMap.end()) {
        // remove an mrc file form a project.
        m_mrcFileMap.erase(mrcFileItr);
        removal = true;
    }
    return removal;
}


QString MrcProject::mrcpFilePath(const QString &path, const QString &name)
{
    QString prjPath = path;
    prjPath.remove(QRegularExpression("/+$")); // remove last '/' characters.

    QString mrcpFilePath = prjPath + "/" + name + MrcFile::SUFFIX_MRCP;
    return mrcpFilePath;
}

QString MrcProject::relativePath(const QString &projectPath, const QString &filePath)
{
    QString prjPath = projectPath;
    prjPath.remove(QRegularExpression("/+$")); // remove last '/' characters.

    QString relativeFilePath = filePath;
    relativeFilePath.remove(QRegularExpression("^"+prjPath));
    relativeFilePath.remove(QRegularExpression("^/+"));  // When a first char is '/', remove it.
    return relativeFilePath;
}

QString MrcProject::absolutePath(const QString &projectPath, const QString &fileRelativePath)
{
    QString prjPath = projectPath;
    prjPath.remove(QRegularExpression("/+$")); // remove last '/' characters.

    QString absolutePath = prjPath + "/" + fileRelativePath;
    return absolutePath;
}

QString MrcProject::mrbFilePath(const QString &filePath)
{
    QString mrbPath;
    QFileInfo info(filePath);
    QString absolutePath = info.absolutePath();
    QString baseName = info.completeBaseName();
    mrbPath = absolutePath.append("/").append(baseName).append(MrcAbstractFile::SUFFIX_MRB);
    return mrbPath;
}

