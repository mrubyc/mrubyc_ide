/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef CLOUDCOMPILEPROCESS_H
#define CLOUDCOMPILEPROCESS_H

#include "abstractprocess.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class BuildSetting;
class MrcProject;

/*!
 * \brief The CloudCompileProcess class.
 *  It executes sequentially compile some sources  on a Cloud.
 */
class CloudCompileProcess : public AbstractProcess
{
    Q_OBJECT
public:
    explicit CloudCompileProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent = Q_NULLPTR);
    ~CloudCompileProcess();

protected:
    /*!
     * \brief Execute a process.
     * \return if the process is normally finished, return 0. Otherwise, return not zero.
     */
    virtual int exec();

protected:
    QNetworkAccessManager *m_manager;

    BuildSetting *m_buildSetting;
    MrcProject *m_mrcProject;
};

#endif // CLOUDCOMPILEPROCESS_H
