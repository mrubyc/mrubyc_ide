#include "cloudcompileprocess.h"

#include "buildsetting.h"
#include "mrcproject.h"
#include "mrcfile.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileInfo>
#include <QList>
#include <QEventLoop>

CloudCompileProcess::CloudCompileProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent)
    :AbstractProcess(parent)
    ,m_buildSetting(buildSetting)
    ,m_mrcProject(mrcProejct)
{
    m_manager = new QNetworkAccessManager(this);
}

CloudCompileProcess::~CloudCompileProcess()
{
    delete m_manager;
}

int CloudCompileProcess::exec()
{
    QString cloudUrl = m_buildSetting->cloudUrl();
    QList<MrcFile*> mrcFileList = m_mrcProject->projectMrcFileList();

    int result = 0;
    foreach (MrcFile *mrcFile, mrcFileList) {
        // The target is rb file only.
        if (!mrcFile->isRubySourceFile()) {
            continue;
        }

        // Get absolute path for a ruby file.
        QString path = mrcFile->path();
        // Check rb file exists.
        // If the file does not exit, set return code as 1 and emit error message.
        QFileInfo fileInfo(path);
        if (!fileInfo.isFile()) {
            result = 1;
            emit messageSent(tr("No such file: %1\n").arg(path), AbstractProcess::ERROR);
            continue;
        }
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            result = 2;
            emit messageSent(tr("Permission denied: %1\n").arg(path), AbstractProcess::ERROR);
            continue;
        }

        // Create a parameter.
        QByteArray postData;
        postData.append("s=" + file.readAll());
        postData.append("key=swest");
        file.close();

        // Execute the compile.
        QEventLoop eventLoop;
        connect(m_manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        QNetworkReply *reply = m_manager->post(QNetworkRequest(QUrl(cloudUrl+"/compile")), postData);
        int exitCode = eventLoop.exec();
        if (exitCode) {
            result = 3;
            emit messageSent(tr("It was abnormally finished: %1\n").arg(path), AbstractProcess::ERROR);
            continue;
        }
        if (reply->error() == QNetworkReply::NoError){
            QString mrbFilePath = MrcProject::mrbFilePath(path);
            QFile mrbFile(mrbFilePath);
            if (!file.open(QIODevice::WriteOnly)) {
                result = 4;
                emit messageSent(tr("Permission denied: %1\n").arg(mrbFilePath), AbstractProcess::ERROR);
                continue;
            }
            mrbFile.write(reply->readAll());
            mrbFile.close();
        } else {
            result = 5;
            emit messageSent(reply->errorString() +"\n", AbstractProcess::ERROR);
        }
    }
    return result;
}
