#include "abstractprocess.h"

#include <QDebug>

AbstractProcess::AbstractProcess(QObject *parent) : QObject(parent)
{

}

AbstractProcess::~AbstractProcess()
{
     qDebug();
}

void AbstractProcess::setName(const QString &name)
{
    m_name = name;
}

QString AbstractProcess::name() const
{
    return m_name;
}

void AbstractProcess::execute()
{
    int ret = exec();
    emit finished(ret);
}
