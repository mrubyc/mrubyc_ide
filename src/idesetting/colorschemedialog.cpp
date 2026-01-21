/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "colorschemedialog.h"
#include "fontcolorssetting.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

ColorSchemeDialog::ColorSchemeDialog(FontColorsSetting *fontColorsSetting, QWidget *parent)
    :QDialog(parent)
    ,m_fontColorsSetting(fontColorsSetting)
{
    QLabel *colorSchemeLabel = new QLabel(tr("Color Scheme Name:"));

    m_colorSchemeLineEdit = new QLineEdit;
    m_colorSchemeLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(colorSchemeLabel);
    vlayout->addWidget(m_colorSchemeLineEdit);
    vlayout->addWidget(m_buttonBox);
    setLayout(vlayout);

    setWindowTitle(tr("Copy Color Scheme"));

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ColorSchemeDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ColorSchemeDialog::reject);
}

ColorSchemeDialog::~ColorSchemeDialog()
{
}

void ColorSchemeDialog::setColorSchemeName(QString &name)
{
    m_colorSchemeLineEdit->setText(name);
}

QString ColorSchemeDialog::colorSchemeName() const
{
    return m_colorSchemeLineEdit->text();
}

void ColorSchemeDialog::accept()
{
        QDialog::accept();
}

void ColorSchemeDialog::reject()
{
    QDialog::reject();
}
