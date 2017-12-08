#ifndef SETUPIMPORTFILESDIALOG_H
#define SETUPIMPORTFILESDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QWidget;
class QDialog;
class QLineEdit;
class QLabel;
class QTreeView;
class QFileInfo;
class QDialogButtonBox;
QT_END_NAMESPACE

class CheckableFileSystemModel;

/*!
 * \brief The SetupImportFilesDialog class.
 * The dialog is for an user to import project or file.
 */
class SetupImportFilesDialog : public QDialog
{
    Q_OBJECT
public:
    enum ImportMode {
        //! \brief Project mode. It means that it can import a folder in a project directory.
        Project
        //! \brief File mode. It can import a file under a project directory.
        ,File
    };
public:
    explicit SetupImportFilesDialog(QWidget *parent = Q_NULLPTR, ImportMode mode = Project);
    ~SetupImportFilesDialog();

    /*!
     * \brief Return the inputed project name.
     * \return the proejct name.
     */
    QString projectName() const;
    /*! \todo The QString is the type of return value. It is better...
     * \brief Return the project path.
     *      It is included in a directory of a project name.
     * \return a path
     */
    QString projectPath() const;

    /*!
     * \brief Return selected files to import into the project.
     * \return selected files. It is not included in a directory.
     */
    QList<QFileInfo> selectProjectFileList() const;

    /*!
     * \brief Set up project location.
     *          It is included in a project name.
     * \param location the project locatoin
     * \param displayName if true, display the project name.
     * \return if false, the location is not directry. Otherwise, true.
     */
    bool setupLocation(const QString &location, bool displayName = true);

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

    /*!
     * \brief This slot is when an user finished to input the location on a location field.
     */
    void finishInputLocation();

private:
    void initialize();

private:
    ImportMode m_importMode;

    //! project name.
    QLineEdit *m_nameLineEdit;
    //! location in a project directory.
    QLineEdit *m_pathLineEdit;
    //! treeview to select files
    QTreeView *m_fileTreeView;
    CheckableFileSystemModel *m_fileSystemModel;

    //! status label
    QLabel *m_statusLabel;

    //! for dialog buttons
    QDialogButtonBox *m_buttonBox;
};

#endif // SETUPIMPORTFILESDIALOG_H
