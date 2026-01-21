/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef SAVECHANGESDIALOG_H
#define SAVECHANGESDIALOG_H

#include <QDialog>
#include <QList>

QT_BEGIN_NAMESPACE
class QListView;
class QDialogButtonBox;
QT_END_NAMESPACE

class MrcFile;

/*!
 * \brief The SaveChangesDialog class.
 *  It is a confirmation dialog of modification file list.
 */
class SaveChangesDialog : public QDialog
{
public:
    explicit SaveChangesDialog(const QList<MrcFile*> &mrcFileList, QWidget *parent = Q_NULLPTR);
protected Q_SLOTS:
    /*!
     * \brief Determine the selected saving files by a user.
     */
    void accept() override;

    /*!
     * \brief Cancel to save.
     */
    void reject() override;
private:
     QListView *m_unsaveChangesListView;
     QDialogButtonBox *m_buttonBox;
};

#endif // SAVECHANGESDIALOG_H
