#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "setting.h"
#include "mrcfile.h"
#include "mrbcompileprocess.h"

#include <QMainWindow>
#include <QString>
#include <QThread>
#include <QTextCharFormat>
#include <QTimer>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class WaitingSpinner;
class IdeSettingControl;
class ColorSchemeHighlighter;
class MrcProjectControl;
class ColorSchemeSetting;
class MrcProject;
class MrcProjectTreeModel;
class MrcFile;
class AbstractProcess;
class CompositProcess;

/*!
 * \brief The MainWindow class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(IdeSettingControl *m_settingControl, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

Q_SIGNALS:
    /*!
     * \brief This signal is emitted whenever the ColorScheme changes.
     * \param colorSchemeSetting a setting for ColorScheme
     */
    void colorSchemeChanged(ColorSchemeSetting *colorSchemeSetting);

    void fileOpened(MrcFile* mrcFile);

public Q_SLOTS:
    /*!
      \brief This signal is emitted whenever the IdeSettingDialog changes.
    */
    void onConsoleParameter_changed();

private Q_SLOTS:
    /*==== file menu action ====*/
    void on_actionCreateProject_triggered();

    void on_actionImportProject_triggered();

    void on_actionNewFile_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void on_actionSetup_triggered();

    void on_actionAbout_triggered();


    /*==== execution menu action ====*/
    void on_actionWrite_triggered();

    void on_actionCompile_triggered();

    void on_actionClearConsole_triggered();

    /*==== popup menu action ==*/
    void on_projectTreeView_customContextMenuRequested(const QPoint &point);

    /*==== tree view action ==*/
    /*!
     * \brief Import existing files.
     */
    void importExistingFiles();

    /*!
     * \brief Remove the file from a project.
     * \details If permanence is true, remove the file from the project and file system.
     *          Otherwise, remove the file from the project only.
     */
    void removeFile();

    /*!
     * \brief Duplicate a selected file.
     */
    void duplicateFile();

    /*!
     * \brief Rename a selected file
     */
    void renameFile();

    /*!
     * \brief Change TextDocument.
     *          Change font and color, and set up marks.
     */
    void changeSetting();

    /*!
     * \brief Change a text document with selected file name.
     * \param index an index of selected file
     */
    void changeCurrentDocument(int index);

    /*!
     * \brief Change model data in the view.
     * \brief This signal is send when the user changes a text in the view.
     * \param index It is an index of the view.
     * \param name change after a text.
     */
    void changeFileName(const QModelIndex &index, const QString &name);

    /*!
     * \brief Close the file.
     *        Remove it from a combobox and a text editor.
     */
    void closeFile();

    /*!
     * \brief Open the file in order to edit.
     *        It means that it becomes as selected file with a combobox.
     * \param mrcFile
     */
    void openFile(MrcFile* mrcFile);

    /*!
     * \brief Open the file in order to edit with a menu of a project tree.
     */
    void openFile();

    /*!
     * \brief Open the file in order to edit with a project tree
     * \param index
     */
    void openFile(const QModelIndex &index);

    /*==== other action ==*/
    /*!
     * \brief Receive message from a sub process.
     * \param message the message content.
     * \param type normal or error
     */
    void sendMessage(const QString &message, AbstractProcess::MessageType type = AbstractProcess::NORMAL);

    /*!
     * \brief Start waiting spinner.
     */
    void startWaitingSpinner();
    /*!
     * \brief Stop waiting spinner.
     */
    void stopWaitingSpinner();

    /*!
     * \brief Finish process result
     * \param result the return code for AbstractProcess.
     */
    void finishProcess(int result);

    /*!
      \brief for serial console
    */
    void onConsoleSerialError_triggered(QSerialPort::SerialPortError error_code);
    void onConsoleTimer_triggered();

private:
    void createCompileCommand(CompositProcess *process);
    /*!
     * \brief Return current time.
     * \return hh:mm:ss format.
     */
    QString currentTime();

    void setupMrcProjectTreeModel(MrcProject *project);

    void addFileToProjectTreeModel(MrcFile *mrcFile);

    /*!
     * \brief Initialize a default menu of combobox and a default document of textedit.
     */
    void initializeMrcProject();

    /*!
     * \brief Prepare a combobox and textedit for the MrcProject
     *      when the project will be opened  or be imported existing soruces.
     */
    void prepareMrcProject();

    /*!
     * \brief Put away a combobox and textedit for MrcProject
     *        when a project will be closed.
     */
    void putAwayMrcProject();

    /*!
     * \brief Confirm CloseProject
     * \return if the project is closed, true. otherwise, false.
     */
    bool confirmCloseProject();

    /*!
     * \brief Close the file from this project.
     * \param mrcFile
     * \param confirm if true and document is modified, show a confirm dialog to save a file.
     *        Otherwise, do not save a file.
     * \return If closing process is finished, return true. Otherwise, false.
     */
    bool closeFile(MrcFile *mrcFile, bool confirm = true);

    /*!
     * \brief Store setting when IDE is done.
     */
    void storeIDESetting();
    /*!
     * \brief Restore setting when IDE stats up.
     */
    void restoreIDESetting();

    /*!
     * \brief Store a project setting.
     */
    void storeProjectSetting(QString lastLocation, QString defaultLocation);

protected:
    /*!
     * \brief closeEvent
     * \param event
     */
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    WaitingSpinner *m_waitingSpinner;

    //! The model is utilized to display MRC files for a project.
    MrcProjectTreeModel *m_mrcProjectTreeModel;

    //! temporary file.
    MrcFile *m_tmpMrcFile;

    ColorSchemeHighlighter *m_highlighter;

    IdeSettingControl *m_settingControl;
    //! a proejct control
    MrcProjectControl *m_projectControl;

    //! currnet mrc project.
    MrcProject *m_currentProject;

    QTextCharFormat m_errorFormat;
    QTextCharFormat m_infomationFormat;
    QTextCharFormat m_normalFormat;

    //! the thread works a some process.
    QThread m_workerThread;

    //! for serial console.
    QTimer m_consoleTimer;
    QSerialPort m_consoleSerialPort;
    enum {
        SERIALCONSOLE_CLOSED = 0,
	SERIALCONSOLE_READY_OPEN = 1,
	SERIALCONSOLE_OPENED = 2,
	SERIALCONSOLE_READY_CLOSE = 3,
    } m_stateSerialConsole;
};

#endif // MAINWINDOW_H
