#ifndef SETUPNEWPROJECTDIALOG_H
#define SETUPNEWPROJECTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QWidget;
class QLineEdit;
class QLabel;
class QDir;
class QDialogButtonBox;
QT_END_NAMESPACE


/*!
 * \brief The SetupNewProjectDialog class.
 *  It is for new project to set up information.
 */
class SetupNewProjectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SetupNewProjectDialog(QString defaultPath, QWidget *parent = Q_NULLPTR);
    ~SetupNewProjectDialog();

    /*!
     * \brief Return the inputed project name.
     * \return the proejct name.
     */
    QString projectName() const;
    /*!
     * \brief Return the project's absolute path a createing the project.
     *          It is not included in a directory of a project name.
     * \return a project location.
     */
    QString projectLocation() const;

    /*!
     * \brief Return the project path.
     *      It is included in a directory of a project name.
     * \return a path
     */
    QString projectPath() const;

protected Q_SLOTS:
    /*!
     * \brief Determine the inputed information for new project by a user.
     */
    void accept() override;

    /*!
     * \brief Cancel the new project creation.
     */
    void reject() override;

    /*!
     * \brief Select the direcotry to include the project in a dialog.
     */
    void selectProjectLocation();

private:
    //! project name.
    QLineEdit *m_nameLineEdit;
    //! location in a project directory.
    QLineEdit *m_locationLineEdit;

    //! status label
    QLabel *m_statusLabel;

    //! for dialog buttons
    QDialogButtonBox *m_buttonBox;
};

#endif // SETUPNEWPROJECTDIALOG_H
