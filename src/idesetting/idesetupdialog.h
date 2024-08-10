#ifndef IDESETUPDIALOG_H
#define IDESETUPDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QListWidget>

class IdeSettingControl;

/*!
 * \brief The IdeSetupDialog class.
 * It is a dialog to configure properties all of ide.
 */
class IdeSetupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IdeSetupDialog(IdeSettingControl *settingControl, QWidget *parent = Q_NULLPTR);
    ~IdeSetupDialog();

Q_SIGNALS:
    void changed();

public Q_SLOTS:
    /*!
     * \brief Save the settings if it is modified and close this window.
     */
    void accept();

    /*!
     * \brief Discard the modified setteings if an user accepts.
     */
    void reject();

private Q_SLOTS:
    /*!
     * \brief Chaange the setting tab when an user clicks a setting option.
     * \param current clicked item
     * \param previous previous item
     */
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

    /*!
     * \brief all of pushed button are controled.
     * \param button
     */
    void clickButton(QAbstractButton *button);

private:
    void createSettingOptions();
private:
    //! settingOptionList
    QListWidget *m_settingOptionList;

    QStackedWidget *m_configWidget;

    QWidget *m_textEditorWidget;
    QWidget *m_compilerWidget;
    QWidget *m_writerWidget;

    //! button box for dialog
    QDialogButtonBox *m_buttons;

    IdeSettingControl *m_settingControl;
};

#endif // IDESETUPDIALOG_H
