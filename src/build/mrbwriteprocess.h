/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef MRBWRITEPROCESS_H
#define MRBWRITEPROCESS_H

#include "abstractcommandprocess.h"

#include <QObject>

class BuildSetting;
class MrcProject;

/*!
 * \brief The MrbWriteProcess class.
 * It executes a mrbwrite command as subprocess.
 */
class MrbWriteProcess : public AbstractCommandProcess
{
public:
    explicit MrbWriteProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent = Q_NULLPTR);

protected:
    /*!
     * \brief Execute a writer by mrbwrite
     * \return if the process is normally finished, return 0. Otherwise, return non zero.
     */
    virtual int exec();
};

#endif // MRBWRITEPROCESS_H
