#include "confirmfiledeletiondialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConfirmFileDeletionDialog::ConfirmFileDeletionDialog(const QString &filePath, QWidget *parent)
    :QDialog(parent)
{
    QLabel *removeLabel = new QLabel(tr("File to remove:"));
    QLineEdit *filePathLineEdit = new QLineEdit;
    filePathLineEdit->setText(filePath);
    filePathLineEdit->setReadOnly(true);

    m_permanenceCheckBox = new QCheckBox(tr("Delete file permanently"));
    m_permanenceCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(removeLabel);
    vBoxLayout->addWidget(filePathLineEdit);
    vBoxLayout->addWidget(m_permanenceCheckBox);
    vBoxLayout->addWidget(m_buttonBox);

    setLayout(vBoxLayout);
    setWindowTitle(tr("Remove File"));
    setMinimumWidth(500);

    // ok/cancel button for the dialog.
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ConfirmFileDeletionDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ConfirmFileDeletionDialog::reject);
}

ConfirmFileDeletionDialog::~ConfirmFileDeletionDialog()
{
}

bool ConfirmFileDeletionDialog::isPermanence()
{
    return (m_permanenceCheckBox->checkState()== Qt::CheckState::Checked)? true : false;
}

void ConfirmFileDeletionDialog::accept()
{

    QDialog::accept();
}

void ConfirmFileDeletionDialog::reject()
{
    QDialog::reject();
}
