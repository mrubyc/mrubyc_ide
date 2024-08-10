#ifndef BUILDSETTING_H
#define BUILDSETTING_H

#include "abstractsetting.h"

#include <QObject>
#include <QString>

/*!
 * \brief The BuildSetting class.
 * The setting class manages properties to build ruby's soruce programs.
 */
class BuildSetting : public AbstractSetting
{
    Q_OBJECT
    Q_DISABLE_COPY(BuildSetting)
public:
    explicit BuildSetting(QObject *parent = Q_NULLPTR);
    ~BuildSetting();

    /*!
     * \brief Set MrbcEnabled flag.
     * \param enable  if true, checked. otherwise, un-checked.
     */
    void setMrbcEnabled(bool enable);
    /*!
     * \brief Get MrbcEnabled flag.
     * \return if checked, true. otherwise, false.
     */
    bool mrbcEnabled() const;

    /*!
     * \brief Set Mrbc Command Path.
     * \param path the path for mrbc command
     */
    void setMrbcCommand(const QString &path);
    /*!
     * \brief Get Mrbc Command Path.
     * \return the path for mrbc command
     */
    QString mrbcCommand() const;

    /*!
     * \brief Set Mrbc Command Options
     * \param mrbcCommandOptions command options
     */
    void setMrbcCommandOptions(const QString &mrbcCommandOptions);
    /*!
     * \brief Get Mrbc Command Options.
     * \return command option
     */
    QString mrbcCommandOptions() const;

    /*!
     * \brief Set CloudEnabled flag
     * \param enable if true, checked. otherwise, un-checked.
     */
    void setCloudEnabled(bool enable);
    /*!
     * \brief Get cloudEnabled flag.
     * \return if checked, true. otherwise, false.
     */
    bool cloudEnabled() const;

    /*!
     * \brief Set CloudUrl
     * \param url the url for cloud url.
     */
    void setCloudUrl(const QString& url);
    /*!
     * \brief Get CloudUrl
     * \return the url for cloud url.
     */
    QString cloudUrl() const;

    /*!
     * \brief Set ClearConsoleEnabled flag
     * \param enable if true, checked. otherwise, un-checked.
     */
    void setClearConsoleEnabled(bool enable);
    /*!
     * \brief Get clearConsoleEnabled flag.
     * \return if checked, true. otherwise, false.
     */
    bool clearConsoleEnabled() const;

    /*!
     * \brief Set Mrbw Command path.
     * \param path the path for mrbwriter command
     */
    void setMrbwCommand(const QString& path);
    /*!
     * \brief Get Mrbw Command path.
     * \return the path for mrbwriter command
     */
    QString mrbwCommand() const;

    /*!
     * \brief Set MrbwCommandOptions
     * \param mrbwCommandOptions  command options for mrbwriter
     */
    void setMrbwCommandOptions(const QString &mrbwCommandOptions);
    /*!
     * \brief mrbwCommandOptions
     * \return mrbwriter's command options
     */
    QString mrbwCommandOptions() const;

    /*!
     * \brief Set PortName
     * \param portName the port name.
     */
    void setPortName(const QString &portName);
    /*!
     * \brief Get Port Name
     * \return the port name.
     */
    QString portName() const;

    /*!
     * \brief Set BaudRate
     * \param n baud rate
     */
    void setBaudRate(const QString &baud);
    /*!
     * \brief Get Port Name
     * \return baud rate.
     */
    QString baudRate() const;

    /*!
     * \brief Clear this object's properties.
     */
    void clear();

private:
    bool m_mrbcEnabled;
    QString m_mrbcCommand;
    QString m_mrbcCommandOptions;

    bool m_cloudEnabled;
    QString m_cloudUrl;

    bool m_clearConsoleEnabled;

    QString m_mrbwCommand;
    QString m_mrbwCommandOptions;

    QString m_portName;
    QString m_baudRate;
};

#endif // BUILDSETTING_H
