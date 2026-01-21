/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "setupnewprojectdialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QFileDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QDebug>


SetupNewProjectDialog::SetupNewProjectDialog(QString defaultPath, QWidget *parent)
    :QDialog(parent)
{
//    QLabel *nameLabel = new QLabel(tr("Project Name"));
    m_nameLineEdit = new QLineEdit;
    m_nameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    QLabel *pathLabel = new QLabel(tr("Create in"));
    m_locationLineEdit = new QLineEdit;
    m_locationLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_locationLineEdit->setText(defaultPath); // default project path.
    QPushButton *projectPathButton = new QPushButton(tr("..."));
    projectPathButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_statusLabel = new QLabel;
    m_statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QHBoxLayout *pathHLayout = new QHBoxLayout;
    pathHLayout->addWidget(m_locationLineEdit);
    pathHLayout->addWidget(projectPathButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    formLayout->addRow(tr("Project Name"), m_nameLineEdit);
    formLayout->addRow(tr("Create In"), pathHLayout);

    QVBoxLayout *setupLayout = new QVBoxLayout;
    setupLayout->addLayout(formLayout);
    setupLayout->addWidget(m_statusLabel);
    setupLayout->addWidget(m_buttonBox);

    setLayout(setupLayout);
    setWindowTitle(tr("Setup New Project"));
    setMinimumWidth(500);

    // for project Location Button
    connect(projectPathButton, &QPushButton::clicked, this, &SetupNewProjectDialog::selectProjectLocation);
    // ok/cancel button for the dialog.
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SetupNewProjectDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SetupNewProjectDialog::reject);
}

SetupNewProjectDialog::~SetupNewProjectDialog()
{

}

QString SetupNewProjectDialog::projectName() const
{
    return m_nameLineEdit->text();
}

QString SetupNewProjectDialog::projectLocation() const
{
    return m_locationLineEdit->text();
}

QString SetupNewProjectDialog::projectPath() const
{
    QString path = projectLocation() + "/" + projectName();
    QString projectPath = QDir::cleanPath(path);
    return projectPath;
}


void SetupNewProjectDialog::accept()
{
    if (m_nameLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("Name is empty."));
    } else if (m_locationLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("The path should be inputed."));
    } else {
        QDir dir(projectPath());
        if (dir.exists()) {
            m_statusLabel->setText(tr("The project already exists."));
        } else {
            QDialog::accept();
        }
    }
}

void SetupNewProjectDialog::reject()
{
    QDialog::reject();
}

void SetupNewProjectDialog::selectProjectLocation()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    m_locationLineEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isNull()) {
        m_locationLineEdit->setText(dir);
    }
}
