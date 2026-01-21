/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "savechangesdialog.h"
#include "savechangesmodel.h"

#include <QLabel>
#include <QDialog>
#include <QListView>
#include <QDialogButtonBox>
#include <QVBoxLayout>

SaveChangesDialog::SaveChangesDialog(const QList<MrcFile*> &mrcFileList, QWidget *parent)
    :QDialog(parent)
{
    QLabel *messageLabel = new QLabel(tr("The following files have unsaved changes:"));

    m_unsaveChangesListView = new QListView;
    m_unsaveChangesListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    m_unsaveChangesListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    SaveChangesModel *saveChangesModel = new SaveChangesModel(mrcFileList);
    m_unsaveChangesListView->setModel(saveChangesModel);

    // button.
    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::SaveAll|QDialogButtonBox::Cancel);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(messageLabel);
    vboxLayout->addWidget(m_unsaveChangesListView);
    vboxLayout->addWidget(m_buttonBox);

    setLayout(vboxLayout);
    setWindowTitle(tr("Save Changes"));

    // ok/cancel button for the dialog.
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SaveChangesDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SaveChangesDialog::reject);
}

void SaveChangesDialog::accept()
{
    QDialog::accept();
}

void SaveChangesDialog::reject()
{
    QDialog::reject();
}
