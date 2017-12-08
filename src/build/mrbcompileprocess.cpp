#include "mrbcompileprocess.h"

#include "abstractprocess.h"
#include "mrcfile.h"
#include "buildsetting.h"
#include "mrcproject.h"

#include <QList>
#include <QProcess>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

MrbCompileProcess::MrbCompileProcess(BuildSetting* buildSetting, MrcProject *mrcProejct, QObject *parent)
    :AbstractCommandProcess(buildSetting, mrcProejct, parent)
{
}

int MrbCompileProcess::exec()
{
    QString command = m_buildSetting->mrbcCommand();
    QString commandOptions = m_buildSetting->mrbcCommandOptions();
    QList<MrcFile*> mrcFileList = m_mrcProject->projectMrcFileList();
    QString projectPath = m_mrcProject->path();

    m_process->setWorkingDirectory(projectPath);

    int result = 0;
    foreach (MrcFile *mrcFile, mrcFileList) {
        // The target is rb file only.
        if (!mrcFile->isRubySourceFile()) {
            continue;
        }
        // kill process if it is running.
        kill();

        // Get absolute path for a ruby file.
        QString filePath = mrcFile->path();
        // Check rb file exists.
        // If the file does not exit, set return code as 1 and emit error message.
        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists()) {
            result = 1;
            emit messageSent(tr("No such file: %1\n").arg(filePath), AbstractProcess::ERROR);
            continue;
        }
        // Populate relative path for a mrb file.
        QString relativeFilePath = MrcProject::relativePath(projectPath, filePath);

        // Setup the command arguments.
        //// 1. Add inputed command option to a command option.
        QStringList args;
        if (!commandOptions.isEmpty()) {
            args << commandOptions;
        }
        //// 2. Add a comples file.
        args << relativeFilePath;

        // Execute the command
        m_process->start(command, args);
        if (!m_process->waitForFinished()) {
            result = 2;
            emit messageSent(m_process->errorString()+"\n", AbstractProcess::ERROR);
        } else {
            if (m_process->exitStatus() == QProcess::NormalExit) {
                if (m_process->exitCode()) {
                    result = m_process->exitCode();
                }
            } else {
                result = 3;
                emit messageSent(tr("The program was abnormally finished: %1\n").arg(command), AbstractProcess::ERROR);
            }
        }
    }
    // kill process if it is running.
    kill();
    return result;

}
