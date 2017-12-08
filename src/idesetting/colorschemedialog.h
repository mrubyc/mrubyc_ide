#ifndef COLORSCHEMEDIALOG_H
#define COLORSCHEMEDIALOG_H


#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QDialogButtonBox;
class QAbstractButton;
QT_END_NAMESPACE

class FontColorsSetting;

/*!
 * \brief The ColorSchemeDialog class.
 *  It is a small dialog to set a new color scheme name.
 */
class ColorSchemeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ColorSchemeDialog(FontColorsSetting *fontColorsSetting, QWidget *parent = Q_NULLPTR);
    ~ColorSchemeDialog();

    void setColorSchemeName(QString &name);
    QString colorSchemeName() const;

protected Q_SLOTS:
    /*!
     * \brief Save the settings if it is modified and close this window.
     */
    void accept();

    /*!
     * \brief Discard the modified setteings if an user accepts.
     */
    void reject();

private:
    QLineEdit *m_colorSchemeLineEdit;
    QDialogButtonBox *m_buttonBox;

    FontColorsSetting *m_fontColorsSetting;
};

#endif // COLORSCHEMEDIALOG_H
