/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef COMPOSITPROCESS_H
#define COMPOSITPROCESS_H

#include "abstractprocess.h"

#include <QObject>
#include <QList>

/*!
 * \brief The CompositProcess class.
 * It executes sequentially the AbstractProcess processes one by one.
 */
class CompositProcess : public AbstractProcess
{
public:
    explicit CompositProcess(QObject *parent = Q_NULLPTR);
    ~CompositProcess();

    /*!
     * \brief Add the process to this object.
     * \param process
     */
    void addProcess(AbstractProcess* process);
    /*!
     * \brief Remove the process from this object.
     * \param process
     */
    void removeProcess(AbstractProcess* process);

    virtual int exec() override { return 0; }
public Q_SLOTS:
    /*!
     * \brief Execute this object's process.
     * It executes a process of list sequencally.
     * If a process is failure, it cannot execute next process.
     */
    virtual void execute() override;
private:
    QList<AbstractProcess*> m_processList;
};

#endif // COMPOSITPROCESS_H
