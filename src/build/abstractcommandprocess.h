#ifndef ABSTRACTCOMMANDPROCESS_H
#define ABSTRACTCOMMANDPROCESS_H

#include "abstractprocess.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

class BuildSetting;
class MrcProject;

/*!
 * \brief The AbstractCommandProcess class.
 * It is an abstract class for executing the build process as a command.
 */
class AbstractCommandProcess : public AbstractProcess
{
    Q_OBJECT
public:
    explicit AbstractCommandProcess(BuildSetting *buildSetting, MrcProject *mrcProejct, QObject *parent = Q_NULLPTR);
    virtual ~AbstractCommandProcess();

protected:
    /*!
     * \brief Execute a process.
     * \return if the process is normally finished, return 0. Otherwise, return not zero.
     */
    virtual int exec() = 0;

    /*!
     * \brief kill proecess.
     *          if the status is running, kill a process and send a message.
     */
    void kill();

protected Q_SLOTS:
    /*!
     * \brief Start
     */
    void start();

    /*!
     * \brief Read stdout when ready to go.
     */
    void readStdout();
    /*!
     * \brief Read stderr when ready to go.
     */
    void readStderr();

protected:
    QProcess *m_process;

    BuildSetting *m_buildSetting;
    MrcProject *m_mrcProject;
};

#endif // ABSTRACTCOMMANDPROCESS_H
