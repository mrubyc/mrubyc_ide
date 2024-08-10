#include "mrbwriteprocess.h"

#include "abstractprocess.h"
#include "mrcfile.h"
#include "buildsetting.h"
#include "mrcproject.h"

#include <QList>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>

MrbWriteProcess::MrbWriteProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent)
    :AbstractCommandProcess(buildSetting, mrcProejct, parent)
{
}

int MrbWriteProcess::exec()
{
    QString command = m_buildSetting->mrbwCommand();
    QString commandOptions = m_buildSetting->mrbwCommandOptions();
    QList<MrcFile*> mrcFileList = m_mrcProject->projectMrcFileList();
    QString projectPath = m_mrcProject->path();

    int result = 0;

    m_process->setWorkingDirectory(projectPath);

    // Setup the command arguments.
    QStringList args;
    //// 1. Add inputed command option to a command option.
    if (!commandOptions.isEmpty())
        args = commandOptions.split(" ");
    //// 2. Add the port name and baud rate.
    QString portName = m_buildSetting->portName();
    if (!portName.isEmpty()) {
        args << "-l" << portName;
    }
    QString baud = m_buildSetting->baudRate();
    if( !baud.isEmpty() ) {
	args << "-s" << baud;
    }

    //// 3. Add the files to write.
    foreach (MrcFile *mrcFile, mrcFileList) {
        // The target is mrb file only.
        if (!mrcFile->isRubySourceFile()) {
            continue;
        }
        QString filePath = mrcFile->path();
        QString mrbFilePath = MrcProject::mrbFilePath(filePath);
        // Check mrb file.
        // If the file does not exit, return 1 and emit error message.
        QFileInfo mrbFileInfo(mrbFilePath);
        if (!mrbFileInfo.exists()) {
            emit messageSent(tr("No such file: %1\n").arg(mrbFilePath), AbstractProcess::ERROR);
            return 1;
        }
        // Populate relative path for a mrb file.
        QString relativeFilePath = MrcProject::relativePath(projectPath, mrbFilePath);

        args << relativeFilePath;
    }

    // Execute the command
    m_process->start(command, args);
    if (!m_process->waitForFinished()) {
        result = 2;
        emit messageSent(m_process->errorString() + "\n", AbstractProcess::ERROR);
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
    // kill process if it is running.
    kill();
    return result;
}
