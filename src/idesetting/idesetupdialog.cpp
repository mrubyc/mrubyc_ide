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
    m_compilerWidget = new CompilerWidget(m_settingControl->ideSetting()->buidSetting());
    m_writerWidget = new WriterWidget(m_settingControl->ideSetting()->buidSetting());
    m_textEditorWidget = new FontColorsWidget(m_settingControl->ideSetting()->fontColorsSetting());

    m_configWidget->addWidget(m_compilerWidget);
    m_configWidget->addWidget(m_writerWidget);
    m_configWidget->addWidget(m_textEditorWidget);

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
    qDebug() << "IdeSetupDialog::accept()";
    if (m_settingControl->isModified()) {
        m_settingControl->save();
	emit changed();
    }
    QDialog::accept();
}

void IdeSetupDialog::reject()
{
    qDebug() << "IdeSetupDialog::reject()";
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
    QListWidgetItem *compilerOption = new QListWidgetItem(m_settingOptionList);
    compilerOption->setText(tr("Compiler"));
    compilerOption->setSelected(true);

    QListWidgetItem *writerOption = new QListWidgetItem(m_settingOptionList);
    writerOption->setText(tr("Writer and Console"));

    QListWidgetItem *textEditorOption = new QListWidgetItem(m_settingOptionList);
    textEditorOption->setText(tr("Text Editor"));
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
