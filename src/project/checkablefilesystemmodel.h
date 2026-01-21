/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef CHECKABLEFILESYSTEMMODEL_H
#define CHECKABLEFILESYSTEMMODEL_H

#include <QObject>
#include <QFileSystemModel>
#include <QSet>

QT_BEGIN_NAMESPACE
class QFileSystemModel;
QT_END_NAMESPACE

/*!
 * \brief The CheckableFileSystemModel class.
 *          It is a file system model with enable checkbox.
 */
class CheckableFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit CheckableFileSystemModel(QObject *parent = Q_NULLPTR);
    ~CheckableFileSystemModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /*!
     * \brief Return QSet for a checked indexes.
     * \return checked indexes.
     */
    QSet<QModelIndex> checkedIndexes();

private:
    bool recursiveCheck(const QModelIndex &index, const QVariant &value);

    QSet<QModelIndex> m_checkedIndexes;
};

#endif // CHECKABLEFILESYSTEMMODEL_H
