#ifndef SETUPADDNEWFILEDIALOG_H
#define SETUPADDNEWFILEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QWidget;
class QLineEdit;
class QLabel;
class QDir;
class QDialogButtonBox;
QT_END_NAMESPACE

/*!
 * \brief The SetupAddNewFileDialog class.
 * It is a small dialog to input a file name.
 */
class SetupAddNewFileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SetupAddNewFileDialog(const QString &projectPath, QWidget *parent = Q_NULLPTR);
    ~SetupAddNewFileDialog();

    /*!
     * \brief Return the inputed file name.
     * \return the file name.
     */
    QString fileName() const;
    /*!
     * \brief Return the file's absolute path in a file system.
     * \return a file location.
     */
    QString filePath() const;

protected Q_SLOTS:
    /*!
     * \brief Determine the inputed information for new file by a user.
     */
    void accept() override;

    /*!
     * \brief Cancel the new project creation.
     */
    void reject() override;

    /*!
     * \brief Select the direcotry to include the project in a dialog.
     */
    void selectFileCreationLocation();

private:
    //! project name.
    QLineEdit *m_nameLineEdit;
    //! location in a project directory.
    QLineEdit *m_locationLineEdit;

    //! status label
    QLabel *m_statusLabel;

    //! for dialog buttons
    QDialogButtonBox *m_buttonBox;

    //! project path.
    QString m_projectPath;
};

#endif // SETUPADDNEWFILEDIALOG_H
