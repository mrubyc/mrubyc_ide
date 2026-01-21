/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "mrcabstractfile.h"

const QString MrcAbstractFile::SUFFIX_MRCP(".mrcp");
const QString MrcAbstractFile::SUFFIX_RUBY(".rb");
const QString MrcAbstractFile::SUFFIX_MRB(".mrb");


MrcAbstractFile::MrcAbstractFile(QObject *parent)
    : QObject(parent)
    ,m_fileType(MrcFileType::OTHER)
{
}

MrcAbstractFile::~MrcAbstractFile()
{

}

QString MrcAbstractFile::name() const
{
    return m_name;
}

void MrcAbstractFile::setName(const QString &name)
{
    m_name = name;
}

QString MrcAbstractFile::path() const
{
    return m_path;
}

void MrcAbstractFile::setPath(const QString &path)
{
    m_path = path;
}

MrcAbstractFile::MrcFileType MrcAbstractFile::fileType() const
{
    return m_fileType;
}

void MrcAbstractFile::setFileType(const MrcFileType &fileType)
{
    m_fileType = fileType;
}

bool MrcAbstractFile::isNormalFile()
{
    return (m_fileType < FOLDER)? true : false;
}

bool MrcAbstractFile::isRubySourceFile()
{
    return (m_fileType == RUBY) ? true : false;
}

bool MrcAbstractFile::isMrcpFile()
{
    return (m_fileType == MRCP) ? true : false;
}

bool MrcAbstractFile::isSpecialFile()
{
    return (m_fileType == TMP_FILE)? true : false;
}

bool MrcAbstractFile::isFolder()
{
    return (m_fileType == FOLDER || m_fileType == PROJECT) ? true : false;
}

MrcAbstractFile::MrcFileType MrcAbstractFile::mrcFileType(const QString &filePath)
{
    if (filePath.endsWith(MrcAbstractFile::SUFFIX_RUBY))
        return MrcFileType::RUBY;
    else if (filePath.endsWith(MrcAbstractFile::SUFFIX_MRCP))
        return MrcFileType::MRCP;
    else if (filePath.endsWith(MrcAbstractFile::SUFFIX_MRB))
        return MrcFileType::MRB;
    else
        return MrcFileType::OTHER;
}
