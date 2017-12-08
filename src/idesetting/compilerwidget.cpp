#include "compilerwidget.h"
#include "setting.h"
#include "buildsetting.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>


CompilerWidget::CompilerWidget(BuildSetting *buildSetting, QWidget *parent)
    : QWidget(parent)
    ,m_buildSetting(buildSetting)
{
    // all of parts for QWidget.
    m_localCompilerCheckBox = new QCheckBox(tr("Local Compiler"));

    QLabel *mrbcLabel = new QLabel(tr("mrbc"));
    m_mrbcPathLineEdit = new QLineEdit;
    QPushButton *buttonSelectCompiler = new QPushButton(tr("..."));
    QLabel *mrbcOptionsLabel = new QLabel(tr("Options"));
    m_mrbcOptionsLineEdit = new QLineEdit;

    m_cloudCompilerCheckBox = new QCheckBox(tr("Cloud Compiler"));

    QLabel *cloudLabel = new QLabel(tr("URL"));
    m_cloudLineEdit = new QLineEdit;
    QPushButton *buttonServerTest = new QPushButton(tr("Test"));

    // layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(mrbcLabel, 0, 0);
    gridLayout->addWidget(m_mrbcPathLineEdit, 0, 1);
    gridLayout->addWidget(buttonSelectCompiler, 0,2);
    gridLayout->addWidget(mrbcOptionsLabel, 1, 0);
    gridLayout->addWidget(m_mrbcOptionsLineEdit, 1, 1, 1, 2);

    QHBoxLayout *cloudHLayout = new QHBoxLayout;
    cloudHLayout->addWidget(cloudLabel);
    cloudHLayout->addWidget(m_cloudLineEdit);
    cloudHLayout->addWidget(buttonServerTest);


    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(m_localCompilerCheckBox);
    verticalLayout->addLayout(gridLayout);
    verticalLayout->addStretch();
    verticalLayout->addWidget(m_cloudCompilerCheckBox);
    verticalLayout->addLayout(cloudHLayout);
    verticalLayout->addStretch();

    setLayout(verticalLayout);

    // Set a value on an each element.
    m_localCompilerCheckBox->setCheckState(CONV_BOOL_TO_CHK_STS(m_buildSetting->mrbcEnabled()));
    m_mrbcPathLineEdit->setText(m_buildSetting->mrbcCommand());
    m_mrbcOptionsLineEdit->setText(m_buildSetting->mrbcCommandOptions());
    m_cloudCompilerCheckBox->setCheckState(CONV_BOOL_TO_CHK_STS(m_buildSetting->cloudEnabled()));
    m_cloudLineEdit->setText(m_buildSetting->cloudUrl());


    // set a button on a relative slot.
    connect(buttonSelectCompiler, &QPushButton::clicked, this, &CompilerWidget::setupCompiler);
    connect(buttonServerTest, &QPushButton::clicked, this, &CompilerWidget::testCloudCompiler);
    // The value for a checkbox and a textfield is changed.
    connect(m_localCompilerCheckBox, &QCheckBox::stateChanged, this, &CompilerWidget::setupMrbcEnabled);
    connect(m_mrbcPathLineEdit, &QLineEdit::editingFinished, this, &CompilerWidget::changeMrbcComand);
    connect(m_mrbcOptionsLineEdit, &QLineEdit::editingFinished, this, &CompilerWidget::changeMrbcComandOptions);
    connect(m_cloudCompilerCheckBox, &QCheckBox::stateChanged, this, &CompilerWidget::setupCloudEnabled);
    connect(m_cloudLineEdit, &QLineEdit::editingFinished, this, &CompilerWidget::cloudUrlChanged);
}

CompilerWidget::~CompilerWidget()
{

}

void CompilerWidget::setupCompiler()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Choose Compiler"), "", "");
    if (fileName.isEmpty())
        return;

    m_mrbcPathLineEdit->setText(fileName);

    // send signal because it does not catch a modification for the text.
    changeMrbcComand();
}

void CompilerWidget::testCloudCompiler()
{
    // Connect testing to the server.
    // get verion.
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishReply(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(m_cloudLineEdit->text()+"/version")));
}

void CompilerWidget::finishReply(QNetworkReply *reply)
{
    QString str;
    if(reply->error() == QNetworkReply::NoError){
        str = reply->readAll();
    } else {
        str = reply->errorString();
    }
    QMessageBox msgBox(this);
    msgBox.setText(str);
    msgBox.exec();
}

void CompilerWidget::setupMrbcEnabled(int status)
{
//    qDebug() << Q_FUNC_INFO << ", status:" << status;
    m_buildSetting->setMrbcEnabled(CONV_CHK_STS_TO_BOOL(static_cast<Qt::CheckState>(status)));
}

void CompilerWidget::changeMrbcComand()
{
    m_buildSetting->setMrbcCommand(m_mrbcPathLineEdit->text());
}

void CompilerWidget::changeMrbcComandOptions()
{
    m_buildSetting->setMrbcCommandOptions(m_mrbcOptionsLineEdit->text());
}

void CompilerWidget::setupCloudEnabled(int status)
{
    m_buildSetting->setCloudEnabled(static_cast<Qt::CheckState>(status));
}

void CompilerWidget::cloudUrlChanged()
{
    m_buildSetting->setCloudUrl(m_cloudLineEdit->text());
}
