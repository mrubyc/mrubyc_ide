#ifndef WRITERWIDGET_H
#define WRITERWIDGET_H

#include <QWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

class BuildSetting;

/*!
 * \brief Set up the writer.
 * The class is set up a writer which deploys a ruby's compiled file to a device.
 */
class WriterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WriterWidget(BuildSetting *buildSetting, QWidget *parent = Q_NULLPTR);
    ~WriterWidget();

private Q_SLOTS:
    /*!
     * \brief Set up a wirter.
     */
    void setupWriterCommand();

    /*!
     * \brief Set a writer command to the setting.
     */
    void changeMrbwComand();
    /*!
     * \brief Set a writer command options to the setting.
     */
    void changeMrbwComandOptions();

    /*!
     * \brief Change serial port by an user.
     */
    void changeSerialPort(int index);

    /*!
     * \brief Update sirial port list.
     * \details Update the list of combobox wtih the available serial ports.
     *          If set port is correct, change the index of the current item in the combobox.
     *          Otherwise, change the first index for the current item.
     */
    void updateSirialPortList();

private:
    BuildSetting *m_buildSetting;

    QLineEdit *m_mrbWriterLineEdit;
    QLineEdit *m_mrbWriterOptionsLineEdit;
    QComboBox *m_portComboBox;
};

#endif // WRITERWIDGET_H
