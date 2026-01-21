/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef COLORSCHEMESETTING_H
#define COLORSCHEMESETTING_H

#include "abstractsetting.h"
#include "colorschemeelementsetting.h"

#include <QObject>
#include <QString>
#include <QList>

/*!
 * \brief The ColorSchemeSetting class.
 * The setting class manages properties that it is to use a highlighter the color schemes.
 */
class ColorSchemeSetting : public AbstractSetting
{
    Q_OBJECT
public:
    enum Keys {
      Keyword = 0,
      Type,
      Number,
      String,
      Constants,
      Symbols,
      RegularExp,
      ShellCmdExp,
      Comment,
      Documents,
      MAX
    };
public:
    explicit ColorSchemeSetting(QObject *parent = Q_NULLPTR);
    explicit ColorSchemeSetting(ColorSchemeSetting &self);
    ~ColorSchemeSetting();

    /*!
     * \brief Get the name of Color Scheme.
     * \return the name of  Colro Scheme.
     */
    QString name() const;

    /*!
     * \brief Set the name of Color Scheme for this object.
     * \param name the name of Color Scheme.
     */
    void setName(const QString &name);

    /*!
     * \brief Is the editable Color Scheme.
     * \return if true, Edit the Color Scheme. Otherwise, diseditable.
     */
    bool editable() const;

    /*!
     * \brief Set editable Color Scheme.
     * \param editable if true, editable. otherwise, diseditable.
     */
    void setEditable(bool editable);

    /*!
     * \brief Get the list of Color Scheme Element.
     * \return
     */
    QList<ColorSchemeElementSetting*> *elementSettingList();

    /*!
     * \brief add ColorSchemeElementSetting to a list.
     * \param colorSchemeSetting
     */
    void addColorSchemeElementSetting(ColorSchemeElementSetting* colorSchemeElementSetting);

    /*!
     * \brief remove ColorSchemeElementSetting from a list.
     * \param colorSchemeElementSetting
     */
    void removeColorSchemeElementSetting(ColorSchemeElementSetting* colorSchemeElementSetting);

    /*!
     * \brief Return the modified flag.
     * \return
     */
    bool isModified() override;

    /*!
     * \brief Set the modified flag.
     * \param yes if the object is modified, true. Otherwise, fasle.
     */
    void setModified(bool yes) override;

private:
    QString m_name;
    bool m_editable;

    QList<ColorSchemeElementSetting*> m_elementSettingList;
};

#endif // COLORSCHEMESETTING_H
