/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef MRBCOMPILEPROCESS_H
#define MRBCOMPILEPROCESS_H

#include "abstractcommandprocess.h"

#include <QObject>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

/*!
 * \brief The MrcCompileProcess class.
 *  It executes sequentially some mrbc command as subprocess.
 */
class MrbCompileProcess : public AbstractCommandProcess
{
    Q_OBJECT
public:
    explicit MrbCompileProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent = Q_NULLPTR);

protected:
    /*!
     * \brief Execute a compile by mrbc.
     * \return if the process is normally finished, return 0. Otherwise, return non zero.
     */
    virtual int exec();
};

#endif // MRBCOMPILEPROCESS_H
