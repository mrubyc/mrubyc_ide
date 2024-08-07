#ifndef IDESETTING_H
#define IDESETTING_H

#include "abstractsetting.h"

#include <QObject>

class BuildSetting;
class FontColorsSetting;
class GenericIDESetting;
class ProjectSetting;

/*!
 * \brief The IdeSetting class.
 *  It is a setting properties for all of IDE.
 */
class IdeSetting : public AbstractSetting
{
    Q_OBJECT
public:
    explicit IdeSetting(QObject *parent = Q_NULLPTR);
    ~IdeSetting();

    /*!
     * \brief Return the pointer of BuildSetting object
     * \return pointer of buildseting object
     */
    BuildSetting *buidSetting() const;
    /*!
     * \brief Return the font colors setting.
     * \return the reference of that objecct.
     */
    FontColorsSetting *fontColorsSetting() const;

    /*!
     * \brief Return gneric Ide Setting.
     * \return the reference of that objecct.
     */
    GenericIDESetting *gnericIdeSetting() const;

    /*!
     * \brief Return the project setting.
     * \return the reference of that objecct.
     */
    ProjectSetting *projectSetting() const;

    /*!
     * \brief Clear all of an attributes.
     */
    void clear();

    /*!
     * \brief Return the modification for this setting.
     * \return If nothing is modified, return false. otherwise, return true.
     */
    bool isModified() override;

    /*!
     * \brief Set the modify flag.
     * \param yes If modified, set true. otherwise, set false.
     */
    void setModified(bool yes) override;

private:
    BuildSetting *m_buildSetting;
    FontColorsSetting *m_fontColorsSetting;
    GenericIDESetting *m_gnericIdeSetting;
    ProjectSetting *m_projectSetting;
};

#endif // IDESETTING_H
