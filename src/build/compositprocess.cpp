/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "compositprocess.h"

CompositProcess::CompositProcess(QObject *parent)
    :AbstractProcess(parent)
{

}

CompositProcess::~CompositProcess()
{
    qDeleteAll(m_processList);
    m_processList.clear();
}

void CompositProcess::addProcess(AbstractProcess *process)
{
    m_processList.append(process);
    connect(process, &AbstractProcess::messageSent,
            this, [this](const QString &message, AbstractProcess::MessageType type){
                        emit messageSent(message, type);
                  });
    process->setParent(this);
}

void CompositProcess::removeProcess(AbstractProcess *process)
{
    m_processList.removeAll(process);
    disconnect(process, &AbstractProcess::messageSent,0,0);
}

void CompositProcess::execute()
{
    int result = 0;
    foreach (AbstractProcess* process, m_processList) {
        QString name = process->name();
        emit messageSent(tr("Starting: \"%1\"\n").arg(name), AbstractProcess::INFOMATION);
        int ret = process->exec();
        if (ret) {
            emit messageSent(tr("The process \"%1\" exited abnormally.\n\n").arg(name), AbstractProcess::INFOMATION);
            result = ret;
            break;
        }
        emit messageSent(tr("The process \"%1\" exited normally.\n\n").arg(name), AbstractProcess::INFOMATION);
    }
    emit finished(result);

    deleteLater();
}
