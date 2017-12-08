#ifndef ABSTRACTPROCESS_H
#define ABSTRACTPROCESS_H

#include <QObject>

/*!
 * \brief The AbstractProcess class.
 * It is an abstract class for executing the build process.
 */
class AbstractProcess : public QObject
{
    Q_OBJECT
    Q_ENUMS(MessageType)
public:
    /*!
     * \brief The MessageType enum
     */
    enum MessageType {
        //! when normal message.
        NORMAL,
        //! when information message.
        INFOMATION,
        //! when warning message.
        WARNING,
        //! when a error message.
        ERROR
    };

public:
    explicit AbstractProcess(QObject *parent = Q_NULLPTR);
    virtual ~AbstractProcess();

    /*!
     * \brief Return the name of this process.
     * \return process's name
     */
    QString name() const;
    /*!
     * \brief Set the name of this process.
     * \param name process's name.
     */
    void setName(const QString &name);

protected:
    /*!
     * \brief Execute a process.
     * \return if the process is normally finished, return 0. Otherwise, return not zero.
     */
    virtual int exec() = 0;

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when a message is sent by the process.
     *        The specified error describes the type of error that occurred.
     * \param message the content of a message.
     * \param type the message type. The default value is NORMAL.
     */
    void messageSent(const QString &message, AbstractProcess::MessageType type = AbstractProcess::MessageType::NORMAL);

    /*!
     * \brief This signal is emitted when an abstract process is finished.
     * \param returnCode
     */
    void finished(int returnCode);

public Q_SLOTS:
    /*!
     * \brief Execute this process.
     *  If it is finished, the resultReady() signal is emitted.
     */
    virtual void execute();

private:
    QString m_name;

    friend class CompositProcess;
};

#endif // ABSTRACTPROCESS_H
