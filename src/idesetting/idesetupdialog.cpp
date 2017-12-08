#include "idesetupdialog.h"
#include "idesettingcontrol.h"
#include "idesetting.h"
#include "buildsetting.h"
#include "fontcolorssetting.h"
#include "fontcolorswidget.h"
#include "compilerwidget.h"
#include "writerwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QDebug>

IdeSetupDialog::IdeSetupDialog(IdeSettingControl *settingControl, QWidget *parent)
    :QDialog(parent)
    ,m_settingControl(settingControl)
{
    m_settingOptionList = new QListWidget;
    m_settingOptionList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    createSettingOptions();

    m_configWidget = new QStackedWidget;
    m_textEditorWidget = new FontColorsWidget(m_settingControl->ideSetting()->fontColorsSetting());
    m_compilerWidget = new CompilerWidget(m_settingControl->ideSetting()->buidSetting());
    m_writerWidget = new WriterWidget(m_settingControl->ideSetting()->buidSetting());
    QTabWidget *textEditorTabWidget = new QTabWidget;
    textEditorTabWidget->addTab(m_textEditorWidget, tr("Font and Colors"));
    textEditorTabWidget->setTabsClosable(false);
    QTabWidget *programTabWidget = new QTabWidget;
    programTabWidget->addTab(m_compilerWidget, tr("Compiler"));
    programTabWidget->addTab(m_writerWidget, tr("Writer"));
    programTabWidget->setTabsClosable(false);
    m_configWidget->addWidget(textEditorTabWidget);
    m_configWidget->addWidget(programTabWidget);


    m_buttons = new QDialogButtonBox;
    m_buttons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Apply);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_settingOptionList);
    horizontalLayout->addWidget(m_configWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addWidget(m_buttons);
    setLayout(mainLayout);

    setWindowTitle(tr("mruby/c IDE Setting"));


    connect(m_settingOptionList, &QListWidget::currentItemChanged,
            this, &IdeSetupDialog::changePage);
    connect(m_buttons, &QDialogButtonBox::clicked, this, &IdeSetupDialog::clickButton);
}
IdeSetupDialog::~IdeSetupDialog()
{
}

void IdeSetupDialog::accept()
{
    qDebug() << "okButtonClicked";
    if (m_settingControl->isModified()) {
        m_settingControl->save();
    }
    QDialog::accept();
}

void IdeSetupDialog::reject()
{
    qDebug() << "cancelButtonClicked";
    if (m_settingControl->isModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm"), tr("Are you sure to cancel the modification?"),
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }
    m_settingControl->reset();
    QDialog::reject();
}

void IdeSetupDialog::createSettingOptions()
{
    QListWidgetItem *textEditorOption = new QListWidgetItem(m_settingOptionList);
    textEditorOption->setText(tr("Text Editor"));
    textEditorOption->setSelected(true);

    QListWidgetItem *programOption = new QListWidgetItem(m_settingOptionList);
    programOption->setText(tr("Build and Deploy"));
}

void IdeSetupDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{

    if (!current)
        current = previous;

    m_configWidget->setCurrentIndex(m_settingOptionList->row(current));

}

void IdeSetupDialog::clickButton(QAbstractButton *button)
{
    if (static_cast<QPushButton*>(button) == m_buttons->button(QDialogButtonBox::Apply)) {
        qDebug() << "Apply buttonClicked:" << button->text();
        if (m_settingControl->isModified()) {
            m_settingControl->save();
        }
    } else if(static_cast<QPushButton*>(button) == m_buttons->button(QDialogButtonBox::Ok)) {
        qDebug() << "OK Button clicked:" << button->text();
        accept();
    } else if(static_cast<QPushButton*>(button) == m_buttons->button(QDialogButtonBox::Cancel)) {
        qDebug() << "Cancel Button clicked:" << button->text();
        reject();
    } else {
        qDebug() << "other button clicked:" << button->text();
    }
}
