/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef IDESETTINGCONTROL_H
#define IDESETTINGCONTROL_H

#include "setting.h"

#include <QObject>
#include <QVariant>
#include <QSettings>

class IdeSetting;
class BuildSetting;

/*!
 * \brief The IdeSettingControl class.
 * It is in order to control IdeSetting.
 */
class IdeSettingControl : public QObject
{
    Q_OBJECT
public:
    explicit IdeSettingControl(QSettings *config, QObject *parent = Q_NULLPTR);
    ~IdeSettingControl();

    /*!
     * \brief The save method stores IdeSetting to the setting data.
     *      After that it emits sattingStored signal.
     */
    void save();

    /*!
     * \brief The load method loads from the setting data to IdeSetting.
     *        After that it emits settingLoaded signal.
     */
    void load();

    /*!
     * \brief The reset method resets IdeSetting by the setting data.
     *       It is same as the load method.
     */
    void reset();

    /*!
     * \brief Is IdeSetting modified.
     * \return return ture if IdeSetting is modified, otherwise false.
     */
    bool isModified();

    /*!
     * \brief Return a current setting data.
     * \return setting data
     */
    IdeSetting* ideSetting();


    /*!
     * \brief Update a setting group with a parameter.
     * \param settingGroup the updating group.
     */
    void update(Setting::IDESettingGroup settingGroup);

    /*!
     * \brief Return a default project location.
     * \return the project location.
     */
    static QString defaultProjectLocation();
private:
    /*!
     * \brief Load only color scheme data from a setting file.
     */
    void loadColorSchemeSetting();
    /*!
     * \brief Store only color scheme data to a setting file.
     */
    void storeColorSchemeSetting();

Q_SIGNALS:
    /*!
     * \brief The signal is sent when IdeSetting is changed.
     * \param type
     *  If the setting is loaded from a file, the type is set LOADED
     *  Or if it is stored to a file, the type is set STORED
     */
    void settingChanged(Setting::ControlType status);

private:
    IdeSetting* m_ideSetting;
    QSettings* m_config;
};

#endif // IDESETTINGCONTROL_H
