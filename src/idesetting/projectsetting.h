#ifndef PROJECTSETTING_H
#define PROJECTSETTING_H

#include "abstractsetting.h"

#include <QObject>

/*!
 * \brief The ProjectSetting class.
 * It is setting properties for a generic project.
 */
class ProjectSetting : public AbstractSetting
{
    Q_OBJECT
public:
    explicit ProjectSetting(QObject *parent = Q_NULLPTR);

    /*!
     * \brief Return the last project location when the user opens a project's directory.
     * \return the project location
     */
    QString lastLocation() const;
    /*!
     * \brief Set the last location.
     * \param lastocation
     */
    void setLastLocation(const QString &lastLocation);

    /*!
     * \brief Return the default project location.
     * \return the default project location
     */
    QString defaultLocation() const;
    /*!
     * \brief Return the default project location
     *   when the user creates in a project's directory
     *    or the user imports from a project's directory.
     * \param defaultLocation the default project location
     */
    void setDefaultLocation(const QString &defaultLocation);

private:
    QString m_lastLocation;
    QString m_defaultLocation;
};

#endif // PROJECTSETTING_H
