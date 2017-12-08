#include "abstractcommandprocess.h"

#include "abstractprocess.h"

#include <QProcess>
#include <QDebug>

AbstractCommandProcess::AbstractCommandProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent)
    : AbstractProcess(parent)
    ,m_buildSetting(buildSetting)
    ,m_mrcProject(mrcProejct)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::started, this, &AbstractCommandProcess::start);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &AbstractCommandProcess::readStdout);
    connect(m_process, &QProcess::readyReadStandardError, this, &AbstractCommandProcess::readStderr);
}

AbstractCommandProcess::~AbstractCommandProcess()
{
    qDebug();
    delete m_process;
}


void AbstractCommandProcess::start()
{
    QString program = m_process->program();
    QStringList args = m_process->arguments();

    QString message;
    if (!program.isEmpty()) message.append(program);
    foreach (const QString arg, args) {
        if (!message.isEmpty()) message.append(" ");
        message.append(arg);
    }
    message.append("\r\n");
    emit messageSent(message);
}

void AbstractCommandProcess::readStdout()
{
//    qDebug();
    m_process->setReadChannel(QProcess::StandardOutput);
    if (!m_process->canReadLine()) {
        QByteArray array = m_process->readAllStandardOutput();
        QString message(array);
//        qDebug() << "Msg("<<QString::number(message.length())<<")="<<message;
        emit messageSent(message);
    } else {
        while (m_process->canReadLine()) {
            QString message = QString::fromLocal8Bit(m_process->readLine());
//            qDebug() << "Msg("<<QString::number(message.length())<<")="<<message;
            emit messageSent(message);
        }
    }
}

void AbstractCommandProcess::readStderr()
{
    m_process->setReadChannel(QProcess::StandardError);
    if (!m_process->canReadLine()) {
        QByteArray array = m_process->readAllStandardOutput();
        emit messageSent(QString(array));
    } else {
        while (m_process->canReadLine()) {
            QString message = QString::fromLocal8Bit(m_process->readLine());
            emit messageSent(message, AbstractProcess::ERROR);
        }
    }
}

void AbstractCommandProcess::kill()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
        if (!m_process->waitForFinished()) {
            QString message = m_process->errorString();
            emit messageSent(message, AbstractProcess::ERROR);
        }
    }
}
