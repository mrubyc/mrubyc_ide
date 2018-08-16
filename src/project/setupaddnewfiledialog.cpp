#include "setupaddnewfiledialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QFileDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QDebug>

SetupAddNewFileDialog::SetupAddNewFileDialog(const QString &projectPath, QWidget *parent)
    :QDialog(parent)
    ,m_projectPath(projectPath)
{
        m_nameLineEdit = new QLineEdit;
        m_nameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_locationLineEdit = new QLineEdit;
        m_locationLineEdit->setText(m_projectPath);
        m_locationLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        formLayout->addRow(tr("File Name"), m_nameLineEdit);
        formLayout->addRow(tr("Create In"), pathHLayout);

        QVBoxLayout *setupLayout = new QVBoxLayout;
        setupLayout->addLayout(formLayout);
        setupLayout->addWidget(m_statusLabel);
        setupLayout->addWidget(m_buttonBox);

        setLayout(setupLayout);
        setWindowTitle(tr("Setup Add New File"));
        setMinimumWidth(500);

        // for project Location Button
        connect(projectPathButton, &QPushButton::clicked, this, &SetupAddNewFileDialog::selectFileCreationLocation);
        // ok/cancel button for the dialog.
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SetupAddNewFileDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SetupAddNewFileDialog::reject);

}

SetupAddNewFileDialog::~SetupAddNewFileDialog()
{

}

QString SetupAddNewFileDialog::fileName() const
{
    return m_nameLineEdit->text();
}

QString SetupAddNewFileDialog::filePath() const
{
     return m_locationLineEdit->text() + "/" + m_nameLineEdit->text();
}

void SetupAddNewFileDialog::accept()
{
    if (m_nameLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("Name is empty."));
        return;
    }
    if (m_locationLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("The path must not be empty."));
        return;
    }
    QFileInfo finfo( m_nameLineEdit->text() );
    if (finfo.suffix() == "") {
      m_nameLineEdit->setText( m_nameLineEdit->text() + ".rb" );
    }

    QFile file(filePath());
    if (file.exists()) {
        m_statusLabel->setText(tr("The file already exists."));
        return;
    }
    if (!filePath().startsWith(m_projectPath)) {
        m_statusLabel->setText(tr("The file must be created under a project."));
        return;
    }
    QDialog::accept();
}

void SetupAddNewFileDialog::reject()
{
    QDialog::reject();
}

void SetupAddNewFileDialog::selectFileCreationLocation()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Select Directory"),
                                                    m_projectPath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isNull()) {
        m_locationLineEdit->setText(dir);
    }
}
