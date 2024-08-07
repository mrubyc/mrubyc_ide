#include "setupimportfilesdialog.h"
#include "checkablefilesystemmodel.h"
#include "mrcproject.h"

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QFormLayout>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QDebug>

SetupImportFilesDialog::SetupImportFilesDialog(QWidget *parent, ImportMode mode)
    :QDialog(parent)
    ,m_importMode(mode)
{
    m_nameLineEdit = new QLineEdit;
    m_nameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pathLineEdit = new QLineEdit;
    m_pathLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QPushButton *projectPathButton = new QPushButton(tr("..."));
    projectPathButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    m_fileTreeView = new QTreeView;
    m_fileTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_fileTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_fileSystemModel = new CheckableFileSystemModel(m_fileTreeView);
    m_fileTreeView->setModel(m_fileSystemModel);

    // message....
    m_statusLabel = new QLabel;
    m_statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // button.
    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QHBoxLayout *pathHLayout = new QHBoxLayout;
    pathHLayout->addWidget(m_pathLineEdit);
    pathHLayout->addWidget(projectPathButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    formLayout->addRow(tr("Project Name"), m_nameLineEdit);
    formLayout->addRow(tr("Location"), pathHLayout);

    QVBoxLayout *setupLayout = new QVBoxLayout;
    setupLayout->addLayout(formLayout);
    setupLayout->addWidget(m_fileTreeView);
    setupLayout->addWidget(m_statusLabel);
    setupLayout->addWidget(m_buttonBox);

    setLayout(setupLayout);
    if (m_importMode == ImportMode::Project) {
        setWindowTitle(tr("Import Project"));
    } else if (m_importMode == ImportMode::File) {
        setWindowTitle(tr("Import Files"));
        m_nameLineEdit->setReadOnly(true);
        m_pathLineEdit->setReadOnly(true);
        projectPathButton->setEnabled(false);
    }
    setMinimumSize(500,450);

    // for project Location Button
    connect(projectPathButton, &QPushButton::clicked, this, &SetupImportFilesDialog::selectProjectLocation);
    // ok/cancel button for the dialog.
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SetupImportFilesDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SetupImportFilesDialog::reject);

    connect(m_pathLineEdit, &QLineEdit::editingFinished, this, &SetupImportFilesDialog::finishInputLocation);
}

SetupImportFilesDialog::~SetupImportFilesDialog()
{
}

QString SetupImportFilesDialog::projectName() const
{
    return m_nameLineEdit->text();
}

QString SetupImportFilesDialog::projectPath() const
{
    QString path = m_pathLineEdit->text();
    QString projectPath = QDir::cleanPath(path);
    return projectPath;
}

QList<QFileInfo> SetupImportFilesDialog::selectProjectFileList() const
{
    QList<QFileInfo> fileInfoList;

    QSet<QModelIndex> indexes = m_fileSystemModel->checkedIndexes();
    QSet<QModelIndex>::const_iterator indexItr = indexes.begin();
    while (indexItr != indexes.end()) {
        QFileInfo info = m_fileSystemModel->fileInfo(*indexItr);
        if (info.isFile())
            fileInfoList.append(info);
        ++indexItr;
    }
    return fileInfoList;
}

void SetupImportFilesDialog::accept()
{
    m_statusLabel->clear();

    if (m_nameLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("Name is empty."));
        return;
    }

    if (m_pathLineEdit->text().isEmpty()) {
        m_statusLabel->setText(tr("The location should be inputed."));
        return;
    }

    QSet<QModelIndex> indexes = m_fileSystemModel->checkedIndexes();
    if (indexes.size() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Unable to Proceed"));
        msgBox.setText(tr("Nothing found to import"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Ok:
            break;
        default:
            break;
        }
        return;
    }

    if (m_importMode == ImportMode::Project) {
        QString projectPath = SetupImportFilesDialog::projectPath();
        QString projectName = SetupImportFilesDialog::projectName();
        QString mrcpFilePath = MrcProject::mrcpFilePath(projectPath, projectName);
        QFile mrcpFile(mrcpFilePath);
        if (mrcpFile.exists()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("File Already Exists"));
            msgBox.setText(tr("The project file already exists:\n'%1'\nWould you like override it?").arg(mrcpFilePath));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Warning);
            int ret = msgBox.exec();

            switch (ret) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Cancel:
                return;
            default:
                break;
            }
        }
    }

    QDialog::accept();
}

void SetupImportFilesDialog::reject()
{
    QDialog::reject();
}

void SetupImportFilesDialog::selectProjectLocation()
{
    QString inputedLocation = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    m_pathLineEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    initialize();
    setupLocation(inputedLocation, false);
}

void SetupImportFilesDialog::finishInputLocation()
{
    initialize();
    QString inputedLocation = m_pathLineEdit->text();
    setupLocation(inputedLocation, false);
}

void SetupImportFilesDialog::initialize()
{
    m_statusLabel->clear();
    m_fileTreeView->setModel(Q_NULLPTR);
}

bool SetupImportFilesDialog::setupLocation(const QString &location, bool displayName)
{
    // Check input location.
    QFileInfo dirInfo(location);
    if (!dirInfo.isDir()) {
        m_statusLabel->setText(tr("Please input the correct location."));
        return false;
    }

    if (displayName) {
        QString projectName = dirInfo.completeBaseName();
        m_nameLineEdit->setText(projectName);
    }

    m_pathLineEdit->setText(location);

    m_fileTreeView->setModel(m_fileSystemModel);
    QModelIndex rootModelIndex = m_fileSystemModel->setRootPath(location);
    m_fileTreeView->setRootIndex(rootModelIndex);

    return true;
}
