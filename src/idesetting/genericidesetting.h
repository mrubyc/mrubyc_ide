/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef GENERICIDESETTING_H
#define GENERICIDESETTING_H

#include "abstractsetting.h"

#include <QObject>

/*!
 * \brief The GenericIDESetting class.
 *  It is a generic setting properties for IDE.
 */
class GenericIDESetting : public AbstractSetting
{
    Q_OBJECT
public:
    explicit GenericIDESetting(QObject *parent = Q_NULLPTR);

    /*!
     * \brief Return the setting value of maximized window.
     * \return if maximized window, return true. otherwise, false.
     */
    bool maximized() const;
    /*!
     * \brief Set the value of maximized window.
     * \param maximized
     */
    void setMaximized(bool maximized);

    QByteArray geometry() const;
    void setGeometry(const QByteArray &geometry);

private:
    //! \brief This property holds whether IDE widget is maximized
    bool m_maximized;

    //! \brief geometory for test.
    QByteArray m_geometry;
};

#endif // GENERICIDESETTING_H
