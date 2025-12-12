#define APPLICATION_VERSION "1.3.0"

#include "mainwindow.h"

#include "mrubyc_ide.h"
#include "colorschemehighlighter.h"
#include "colorschemesetting.h"
#include "confirmfiledeletiondialog.h"
#include "fontcolorssetting.h"
#include "mrcfile.h"
#include "mrcproject.h"
#include "mrcprojectcontrol.h"
#include "mrcprojecttreeitem.h"
#include "mrcprojecttreemodel.h"
#include "mrctextdocument.h"
#include "savechangesdialog.h"
#include "idesettingcontrol.h"
#include "idesetting.h"
#include "buildsetting.h"
#include "fontcolorssetting.h"
#include "genericidesetting.h"
#include "projectsetting.h"
#include "colorschemehighlighter.h"
#include "setupaddnewfiledialog.h"
#include "idesetupdialog.h"
#include "setupimportfilesdialog.h"
#include "setupnewprojectdialog.h"
#include "abstractprocess.h"
#include "compositprocess.h"
#include "mrbcompileprocess.h"
#include "cloudcompileprocess.h"
#include "mrbwriteprocess.h"
#include "waitingspinner.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDateTime>
#include <QMovie>
#include <QThread>
#include <QScrollBar>
#include <QRegularExpression>

MainWindow::MainWindow(IdeSettingControl *settingControl, QWidget *parent) :
    QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,m_mrcProjectTreeModel(Q_NULLPTR)
    ,m_settingControl(settingControl)
    ,m_projectControl(new MrcProjectControl(this))
    ,m_currentProject(Q_NULLPTR)
    ,m_workerThread(this)
    ,m_consoleTimer(this)
    ,m_consoleSerialPort(this)
    ,m_stateSerialConsole(SERIALCONSOLE_CLOSED)
{
    ui->setupUi(this);

    setWindowTitle(tr("mruby/c IDE " APPLICATION_VERSION));
    m_waitingSpinner = new WaitingSpinner(this);

    ui->txtConsole->setContextMenuPolicy(Qt::NoContextMenu);
    //ui->txtConsole->setReadOnly(true);
    ui->txtConsole->setUndoRedoEnabled(false);

    m_errorFormat.setFontWeight(QFont::Bold);
    m_errorFormat.setForeground(Qt::red);
    m_infomationFormat.setFontWeight(QFont::Bold);
    m_infomationFormat.setForeground(Qt::darkCyan);
    m_normalFormat.setFontWeight(QFont::Normal);
    m_normalFormat.setForeground(Qt::black);

    // copy and paste setting.
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    connect(ui->actionCut, &QAction::triggered, ui->plainTextEdit, &MrcTextEditor::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->plainTextEdit, &MrcTextEditor::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->plainTextEdit, &MrcTextEditor::paste);
    connect(ui->plainTextEdit, &MrcTextEditor::copyAvailable, ui->actionCut, &QAction::setEnabled);
    connect(ui->plainTextEdit, &MrcTextEditor::copyAvailable, ui->actionCopy, &QAction::setEnabled);

    restoreIDESetting();
    initializeMrcProject();
    changeSetting();

    // for editor.
    connect(ui->plainTextEdit, &MrcTextEditor::documentChanged, this, &MainWindow::changeSetting);
    // signal/slot for combobox
    connect(ui->editingFileComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::changeCurrentDocument);
    // for close file button.
    connect(ui->editingFileCloseButton, &QPushButton::clicked, this, static_cast<void (MainWindow::*)()>(&MainWindow::closeFile));

    // Open a document
    connect(ui->projectTreeView, &QTreeView::doubleClicked, this, static_cast<void (MainWindow::*)(const QModelIndex&)>(&MainWindow::openFile));
    //
    connect(this, &MainWindow::fileOpened, this, static_cast<void (MainWindow::*)(MrcFile*)>(&MainWindow::openFile));

    // setting changed.
    connect(settingControl, &IdeSettingControl::settingChanged, this, &MainWindow::changeSetting);
    // for new file
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::on_actionNewFile_triggered);

    // Start/Stop for WaitingSpinner.
    connect(m_waitingSpinner, &WaitingSpinner::started, this, &MainWindow::startWaitingSpinner);
    connect(m_waitingSpinner, &WaitingSpinner::stopped, this, &MainWindow::stopWaitingSpinner);

    //connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::on_actionAbout_triggered);


    // start serial console
    connect(&m_consoleSerialPort, &QSerialPort::errorOccurred, this, &MainWindow::onConsoleSerialError_triggered);

    BuildSetting *buildSetting = m_settingControl->ideSetting()->buidSetting();
    if( !buildSetting->portName().isEmpty() ) {
	m_stateSerialConsole = SERIALCONSOLE_READY_OPEN;
    }
    connect(&m_consoleTimer, &QTimer::timeout, this, &MainWindow::onConsoleTimer_triggered);
    m_consoleTimer.start( 1000 );

    m_workerThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_projectControl;
    delete m_waitingSpinner;
    m_workerThread.quit();
    m_workerThread.wait();
}

/*============================================================================*/
/*==== file menu action ====*/
/*============================================================================*/
void MainWindow::on_actionCreateProject_triggered()
{
    qDebug();
    if (m_currentProject != Q_NULLPTR && !confirmCloseProject())
        return;

    on_actionClose_triggered();

    ProjectSetting *projectSetting = m_settingControl->ideSetting()->projectSetting();
    QString defaultProjectLocation = projectSetting->defaultLocation();

    // if default project location does not exist, use a directory of execution file.
    QDir projectDir(defaultProjectLocation);
    if (!projectDir.exists()) {
        QMessageBox::warning(this, tr("Create Project"), tr("The path \"%1\" does not exist.").arg(defaultProjectLocation), QMessageBox::Ok);
        defaultProjectLocation = IdeSettingControl::defaultProjectLocation();
    }

    SetupNewProjectDialog newProject(defaultProjectLocation, this);
    int result = newProject.exec();

    if (result == QDialog::Accepted) {
        QString projectName = newProject.projectName();
        QString projectPath = newProject.projectPath();
        m_currentProject = new MrcProject(this);
        bool result = m_projectControl->createNewProject(m_currentProject, projectPath, projectName);
        if (!result) {
            qWarning() << tr("Cannot create the project: %1").arg(projectName);
            delete m_currentProject;
            m_currentProject = Q_NULLPTR;
            return;
        }
        // Update a project file list.
        setupMrcProjectTreeModel(m_currentProject);

        // store project setting to a file.
        storeProjectSetting(m_currentProject->path(), m_currentProject->path());
    }
}

void MainWindow::on_actionImportProject_triggered()
{
    qDebug();
    if (m_currentProject != Q_NULLPTR && !confirmCloseProject())
        return;

    on_actionClose_triggered();

    ProjectSetting *projectSetting = m_settingControl->ideSetting()->projectSetting();
    QString defaultProjectLocation = projectSetting->defaultLocation();

    // if default project location does not exist, use a directory of execution file.
    QDir projectDir(defaultProjectLocation);
    if (!projectDir.exists()) {
        QMessageBox::warning(this, tr("Import Project"), tr("The path \"%1\" does not exist.").arg(defaultProjectLocation), QMessageBox::Ok);
        defaultProjectLocation = IdeSettingControl::defaultProjectLocation();
    }

    SetupImportFilesDialog importProject(this);
    importProject.setupLocation(defaultProjectLocation, false);
    int result = importProject.exec();

    if (result == QDialog::Accepted) {
        QString projectName = importProject.projectName();
        QString projectPath = importProject.projectPath();
        QList<QFileInfo> projectFileList = importProject.selectProjectFileList();
        m_currentProject = new MrcProject(this);
        bool result = m_projectControl->importExistingProject(m_currentProject, projectPath, projectName, projectFileList);
        if (!result) {
            qWarning() << tr("Cannot create the project: %1").arg(projectName);
            delete m_currentProject;
            m_currentProject = Q_NULLPTR;
            return;
        }
        // Update a project file list.
        setupMrcProjectTreeModel(m_currentProject);

        // store project setting to a file.
        storeProjectSetting(m_currentProject->path(), m_currentProject->path());
    }
}

void MainWindow::on_actionNewFile_triggered()
{
    qDebug();

    if (m_currentProject == Q_NULLPTR) {
        QMessageBox::information(this, tr("Add New File"), tr("Please open a project firstly."), QMessageBox::Ok);
        return;
    }

    QString projectPath = m_currentProject->path();

    SetupAddNewFileDialog dialog(projectPath, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
       QString newFilePath = dialog.filePath();
       MrcFile *mrcFile = m_projectControl->addNewFile(m_currentProject, newFilePath);
       if (mrcFile == Q_NULLPTR) {
           qWarning() << tr("Cannot create the new file: %1").arg(newFilePath);
           return;
       }

       //! Open to edit a file on the text editor.
       if (mrcFile->isEditing()) {
            openFile(mrcFile);
       }
       //! Add MRC file into the project tree model....
       addFileToProjectTreeModel(mrcFile);
    }
}

void MainWindow::on_actionSave_triggered()
{
    qDebug();
    if (m_currentProject == Q_NULLPTR) return;

    QList<MrcFile*> mrcFileList = m_projectControl->modifiedFiles(m_currentProject);
    if (mrcFileList.size() == 0) {
        return;
    }
    SaveChangesDialog saveChangeDialog(mrcFileList, this);
    int result = saveChangeDialog.exec();
    if (result == QDialog::Accepted) {
        m_projectControl->saveFiles(m_currentProject, mrcFileList);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug();

    if (m_currentProject != Q_NULLPTR && !confirmCloseProject())
        return;

    on_actionClose_triggered();

    ProjectSetting *projectSetting = m_settingControl->ideSetting()->projectSetting();
    QString lastProjectLocation = projectSetting->lastLocation();

    // if default project location does not exist, use a directory of execution file.
    QDir projectDir(lastProjectLocation);
    if (!projectDir.exists()) {
        QMessageBox::warning(this, tr("Open Project"), tr("The path \"%1\" does not exist.").arg(lastProjectLocation), QMessageBox::Ok);
        lastProjectLocation = IdeSettingControl::defaultProjectLocation();
    }

    QString fileName = QFileDialog::getOpenFileName(
                                                this,
                                                tr("Open Project"),
                                                lastProjectLocation,
                                                tr("mrubyc Project (*.mrcp)" ));
    if (!fileName.isEmpty()) {
        m_currentProject = new MrcProject(this);
        QString projectFileName = QDir::cleanPath(fileName);
        bool result = m_projectControl->openProject(m_currentProject, projectFileName);
        if (!result) {
            qWarning() << tr("Cannot create the project: %1").arg(projectFileName);
            delete m_currentProject;
            m_currentProject = Q_NULLPTR;
            return;
        }
        prepareMrcProject();

        // store project setting to a file.
        storeProjectSetting(m_currentProject->path(), QString());
    }
}

void MainWindow::on_actionClose_triggered()
{
    qDebug();
    if (m_currentProject == Q_NULLPTR) {
        // nothing.
        return;
    }

    // If the files are modified, save them
    on_actionSave_triggered();
    // Remove items from GUI and data model.
    putAwayMrcProject();

    m_projectControl->closeProject(m_currentProject);
    delete m_currentProject;
    m_currentProject = Q_NULLPTR;
}

void MainWindow::on_actionExit_triggered()
{
    qDebug();

    on_actionClose_triggered();

    storeIDESetting();

    QApplication::quit();
}

void MainWindow::on_actionSetup_triggered()
{
    IdeSetupDialog setupDialog(m_settingControl);
    connect(&setupDialog, &IdeSetupDialog::changed, this, &MainWindow::onConsoleParameter_changed);
    setupDialog.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,
	tr("About"),
	tr("mruby/c IDE Version " APPLICATION_VERSION "\n\n"
	   "Copyright (C) 2017- \n"
	   "  Kyushu Institute of Technology.\n"
	   "  Shimane IT Open-Innovation Center.\n"
	   "All rights reserved."
	   ),
        QMessageBox::Ok);
}

void MainWindow::setupMrcProjectTreeModel(MrcProject *project)
{
    if (m_mrcProjectTreeModel != Q_NULLPTR) {
        delete m_mrcProjectTreeModel;
    }
    m_mrcProjectTreeModel = new MrcProjectTreeModel(project, ui->projectTreeView);
    ui->projectTreeView->setModel(m_mrcProjectTreeModel);
    connect(m_mrcProjectTreeModel, &MrcProjectTreeModel::textChanged, this, &MainWindow::changeFileName);
}

void MainWindow::addFileToProjectTreeModel(MrcFile *mrcFile)
{
    // Add MRC file to Project Tree View.
    //! \todo parent's row is 0.
    //!  If it is changed to multiple parents, it should be derived a row from somewhere.
    QModelIndex projectIndex = m_mrcProjectTreeModel->index(0, 0);
    int rowCount = m_mrcProjectTreeModel->rowCount(projectIndex);
    // Add a line at the end of the tree.
    m_mrcProjectTreeModel->insertRows(rowCount, 1, projectIndex);

    // Set mrcfile at the end of the line.
    QModelIndex fileIndex = m_mrcProjectTreeModel->index(rowCount, 1, projectIndex);
    QVariant mrcFileVariant;
    mrcFileVariant.setValue(mrcFile);
    m_mrcProjectTreeModel->setData(fileIndex, mrcFileVariant);
}

void MainWindow::initializeMrcProject()
{
    qDebug();
    // Setup a project file list.
    ui->projectTreeView->setModel(Q_NULLPTR);

    // set up temporary MRC file.
    m_tmpMrcFile = new MrcFile(tr("<no document>"));
    m_tmpMrcFile->setFileType(MrcFile::TMP_FILE);
    m_tmpMrcFile->openTextDocument(new MrcTextDocument(m_tmpMrcFile),false);
    //// set up test document
    MrcTextDocument *tmpTextDocument = m_tmpMrcFile->textDocument();
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(tmpTextDocument);
    tmpTextDocument->setDocumentLayout(layout);
    layout->requestUpdate();

    // Set up clear list for editing combobox.
    ui->editingFileComboBox->clear();
    //// Set up an initial menu.
    openFile(m_tmpMrcFile);

    // Set up an initial text document for Code Editor.
    ui->plainTextEdit->setDocument(tmpTextDocument);
}

void MainWindow::prepareMrcProject()
{
    qDebug();

    // load editing files.
    QList<MrcFile*> editingFileList =  m_currentProject->editingMrcFileList();
    for (int i=0; i<editingFileList.size(); i++) {
        MrcFile* mrcFile = editingFileList.at(i);
        //! \todo if mrcfile is null, create document....
        bool editable = false;
        editable = m_projectControl->editFile(mrcFile);
        if (!editable) {
            qWarning() << "Cannot eidit a file:"<<mrcFile->name();
            continue;
        }
        MainWindow::openFile(mrcFile);
    }

    // load project files.
    setupMrcProjectTreeModel(m_currentProject);
}

void MainWindow::putAwayMrcProject()
{
    qDebug();

    // Setup a project file list.
    if (m_mrcProjectTreeModel != Q_NULLPTR) {
        delete m_mrcProjectTreeModel;
        m_mrcProjectTreeModel = Q_NULLPTR;
    }
    ui->projectTreeView->setModel(Q_NULLPTR);

    // Set up clear list for editing combobox.
    ui->editingFileComboBox->clear();
    //// Set up an initial menu.
    QVariant userData;
    userData.setValue(m_tmpMrcFile);
    ui->editingFileComboBox->addItem(m_tmpMrcFile->name(), userData);

    // Set up an initial text document for Code Editor.
    MrcTextDocument *document = m_tmpMrcFile->textDocument();
    ui->plainTextEdit->setDocument(document);
}

bool MainWindow::confirmCloseProject()
{
    QMessageBox confirmBox;
    confirmBox.setWindowTitle(tr("Close Project"));
    confirmBox.setText(tr("Do you want to close the project?"));
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    confirmBox.setDefaultButton(QMessageBox::Yes);
    confirmBox.setIcon(QMessageBox::Question);

    int result = confirmBox.exec();
    switch (result) {
    case QMessageBox::Yes:
        return true;
    case QMessageBox::No:
        return false;
    }
    return false;
}

bool MainWindow::closeFile(MrcFile *mrcFile, bool confirm)
{
    MrcTextDocument *document = mrcFile->textDocument();

    // check the file is opened. if not open, nothing this function.
    if (document == Q_NULLPTR) return true;

    bool storing = false;
    // If a file is modified and the confirmation is ture, pop up a confirmation dialog.
    // And if user want to save a file, save modification a file.
    if (confirm
        && document->isModified()) {

        QMessageBox::StandardButton replyButton =
            QMessageBox::question(this,
                                   tr("Save File"),
                                   tr("Do you want to save the file?"),
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                                  );
        if (replyButton == QMessageBox::Yes) {
            storing = true;
        } else if (replyButton == QMessageBox::Cancel) {
            return false;
        }
    }

    // find target file from combox and remove it.
    QVariant editingFileParam;
    editingFileParam.setValue(mrcFile);
    int index = ui->editingFileComboBox->findData(editingFileParam);
    ui->editingFileComboBox->removeItem(index);

    // Close the file in the project.
    m_projectControl->closeFile(m_currentProject, mrcFile, storing);

    return true;
}

void MainWindow::storeIDESetting()
{
    // store a IDE setting.
    qDebug() << "Max:" << isMaximized()
             << "Min:" << isMinimized();

    if (isMinimized()) {
        return;
    } else {
        GenericIDESetting *genericIDESetting = m_settingControl->ideSetting()->gnericIdeSetting();
        genericIDESetting->setGeometry(saveGeometry());
        genericIDESetting->setMaximized(isMaximized());
        m_settingControl->update(Setting::IDESettingGroup::GenericIDE);
    }
}

void MainWindow::restoreIDESetting()
{
    // store a IDE setting.
    GenericIDESetting *genericIDESetting = m_settingControl->ideSetting()->gnericIdeSetting();
    bool maximized = genericIDESetting->maximized();
    QByteArray geometry = genericIDESetting->geometry();
    if (maximized) {
        setWindowState(Qt::WindowMaximized);
    } else if (!maximized && !geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void MainWindow::storeProjectSetting(QString lastLocation, QString defaultLocation)
{
    bool modified = false;
    ProjectSetting *projectSetting = m_settingControl->ideSetting()->projectSetting();
    QString lastProjectLocation = projectSetting->lastLocation();

    if (!lastLocation.isNull() && lastProjectLocation != lastLocation) {
        qDebug() << "Update Project Location:" << lastLocation;
        projectSetting->setLastLocation(lastLocation);
        modified = true;
    }

    if (!defaultLocation.isNull()) {

        QString defaultProjectLocation = projectSetting->defaultLocation();

        QDir defaultProjectDir(defaultLocation);
        defaultProjectDir.cdUp();
        QString defaultProjectPath = defaultProjectDir.absolutePath();
        if (defaultProjectLocation != defaultProjectPath) {
            qDebug() << "Update Default Project Location:" << defaultProjectPath;
            projectSetting->setDefaultLocation(defaultProjectPath);
            modified = true;
        }
    }

    if (modified)
        m_settingControl->update(Setting::IDESettingGroup::Projects);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug();
    on_actionExit_triggered();
    event->accept();
}

void MainWindow::changeSetting()
{
    FontColorsSetting *fc = m_settingControl->ideSetting()->fontColorsSetting();
    QString fontFamily = fc->fontFamily();
    int fontSize = fc->fontSize();
    QFont newFont(fontFamily);
    newFont.setPointSize(fontSize);

    // Set font for Code Editor
    ui->plainTextEdit->setFont(newFont);

    // Set font for document.
    QTextDocument *document = ui->plainTextEdit->document();
    document->setDefaultFont(newFont);

    MrcTextDocument *mrcTextDocument = static_cast<MrcTextDocument*>(document);

    // Update color schema setting.
    QString colorSchemeName = fc->defaultColorSchemeName();
    ColorSchemeSetting *colorSchemeSetting = fc->findColorScheme(colorSchemeName);

    ColorSchemeHighlighter *highlighter = mrcTextDocument->mrcFile()->highlighter();
    if (colorSchemeSetting != Q_NULLPTR && highlighter != Q_NULLPTR) {
        highlighter->onColorSchemeChanged(colorSchemeSetting);
    }
}

void MainWindow::changeCurrentDocument(int index)
{
    qDebug() << index;
    if (index < 0) return;

    QVariant currentData = ui->editingFileComboBox->currentData();
    MrcFile *mrcFile = currentData.value<MrcFile*>();

    // setup active document.
    MrcTextDocument *mrcTextDocument = mrcFile->textDocument();
    ui->plainTextEdit->setDocument(mrcTextDocument);

    // Set up mark list.
    ui->plainTextEdit->clearMarkedNumList();
    QList<int> markList = mrcTextDocument->markList();
    ui->plainTextEdit->setMarkedNumberList(markList);
    // re-connect mark signal.
    disconnect(ui->plainTextEdit, &MrcTextEditor::marked, 0, 0);
    connect(ui->plainTextEdit, &MrcTextEditor::marked, mrcTextDocument, &MrcTextDocument::mark);
}

void MainWindow::changeFileName(const QModelIndex &index, const QString &name)
{

    qDebug() << index << name;
    MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
    if (item == Q_NULLPTR) return;

    MrcAbstractFile *mrcAbstractFile = item->data();
    MrcFile *mrcFile = static_cast<MrcFile*>(mrcAbstractFile);

    // Check to rename a file. If true,
    bool result = false;
    result = m_projectControl->renameFile(m_currentProject, mrcFile, name);
    if (!result) {
        QMessageBox::warning(this,
                           tr("Cannot Rename File"),
                           tr("The file %1 could not be removed %2.").arg(mrcFile->path()).arg(name),
                          QMessageBox::Ok
                          );
        // revert previous name....
        QVariant param;
        param.setValue(mrcFile);
        m_mrcProjectTreeModel->setData(index, param);
        return;
    }

    // Set up to display a text as new name on Comobox.
    QVariant param;
    param.setValue(mrcFile);
    int pos = ui->editingFileComboBox->findData(param);
    if (pos >= 0) {
        ui->editingFileComboBox->setItemText(pos, name);
    }

//    // Set up to change a text in the project file view.
//    m_mrcProjectTreeModel->setData(index, param, Qt::EditRole);
//    ui->projectTreeView->update(index);

    return;

}

void MainWindow::closeFile()
{
    qDebug();

    int currentIndex = ui->editingFileComboBox->currentIndex();
    if (currentIndex == 0) {
        return;
    } else {
        // close selected editor....
        QVariant currentData = ui->editingFileComboBox->currentData();
        MrcFile *mrcFile = currentData.value<MrcFile*>();
        closeFile(mrcFile);
    }
}

void MainWindow::openFile(MrcFile *mrcFile)
{
    qDebug() << "MainWindow::openFile(MrcFile *mrcFile)";

    QVariant editingFileParam;
    editingFileParam.setValue(mrcFile);
    int index = ui->editingFileComboBox->findData(editingFileParam);
    if (index < 0) {
        ui->editingFileComboBox->addItem(mrcFile->name(), editingFileParam);
        index = ui->editingFileComboBox->findData(editingFileParam);
    }
    ui->editingFileComboBox->setCurrentIndex(index);
}

void MainWindow::openFile()
{
    QModelIndex currentIndex = ui->projectTreeView->selectionModel()->currentIndex();
    openFile(currentIndex);
}

/*============================================================================*/
/*==== execution menu action ====*/
/*============================================================================*/
void MainWindow::on_actionCompile_triggered()
{
    BuildSetting *buildSetting = m_settingControl->ideSetting()->buidSetting();

    if (m_currentProject == Q_NULLPTR) {
            QMessageBox::information(this,
                                   tr("Compile"),
                                   tr("A project should be opened.\nCould you please open a project?"),
                                    QMessageBox::Ok
                                  );
            return;
    }

    // If the files are modified, save them
    on_actionSave_triggered();

    m_waitingSpinner->start();

    if( buildSetting->clearConsoleEnabled() ) {
	on_actionClearConsole_triggered();
    }

    CompositProcess *process = new CompositProcess;

    // Create local or cloud compile command.
    createCompileCommand(process);
    process->moveToThread(&m_workerThread);

    // Invoke a method on worker thread.
    QMetaObject::invokeMethod(process, "execute");
}

void MainWindow::on_actionWrite_triggered()
{
    BuildSetting *buildSetting = m_settingControl->ideSetting()->buidSetting();

    if (m_currentProject == Q_NULLPTR) {
            QMessageBox::information(this,
                                   tr("Compile"),
                                   tr("A project should be opened.\nCould you please open a project?"),
                                    QMessageBox::Ok
                                  );
            return;
    }

    // If the files are modified, save them
    on_actionSave_triggered();

    m_waitingSpinner->start();

    if( buildSetting->clearConsoleEnabled() ) {
	on_actionClearConsole_triggered();
    }

    if( m_stateSerialConsole == SERIALCONSOLE_OPENED ) {
	m_stateSerialConsole = SERIALCONSOLE_READY_CLOSE;
	while( m_stateSerialConsole != SERIALCONSOLE_CLOSED ) {
	    qApp->processEvents();
	}
    }

    CompositProcess *process = new CompositProcess;
    // Create local or cloud compile command.
    createCompileCommand(process);

    // Create Write Commaand
    MrbWriteProcess *writeProcess = new MrbWriteProcess(buildSetting, m_currentProject);
    writeProcess->setName(tr("Write"));

    process->addProcess(writeProcess);
    process->moveToThread(&m_workerThread);

    // invoke a process's method on worker thread.
    QMetaObject::invokeMethod(process, "execute");
}

void MainWindow::createCompileCommand(CompositProcess *process)
{
    BuildSetting *buildSetting = m_settingControl->ideSetting()->buidSetting();
    bool mrbcEnabled = buildSetting->mrbcEnabled();
    bool cloudEnabled = buildSetting->cloudEnabled();

    if (mrbcEnabled || !cloudEnabled) {
        AbstractProcess *p = new MrbCompileProcess(buildSetting, m_currentProject);
        p->setName(tr("Local Compile"));
        process->addProcess(p);
    } else if (cloudEnabled) {
        AbstractProcess *p = new CloudCompileProcess(buildSetting, m_currentProject);
        p->setName(tr("Cloud Compile"));
        process->addProcess(p);
    }
    connect(&m_workerThread, &QThread::finished, process, &QObject::deleteLater);
    connect(process, &AbstractProcess::finished, this, &MainWindow::finishProcess);
    qRegisterMetaType<AbstractProcess::MessageType>("AbstractProcess::MessageType");
    connect(process, &AbstractProcess::messageSent, this, &MainWindow::sendMessage);
}

QString MainWindow::currentTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString time = dt.toString("hh:mm:ss");
    return time;
}

void MainWindow::on_actionClearConsole_triggered()
{
    ui->txtConsole->clear();
}

/*============================================================================*/
/*==== Tree menu action ====*/
/*============================================================================*/
void MainWindow::on_projectTreeView_customContextMenuRequested(const QPoint &point)
{
    qDebug();

    QModelIndex index = ui->projectTreeView->currentIndex();
    MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
    if (item == Q_NULLPTR) {
        return;
    }
    MrcAbstractFile *abstractFile = item->data();
    if (abstractFile == Q_NULLPTR)
        return;

    qDebug() << "FileName:" << item->data()->name() << "FileType:" << item->data()->fileType();

    QMenu *menu = new QMenu(this);
    if (item->data()->isNormalFile()) {
        QAction *action;
        action = menu->addAction(QString(tr("Edit this file")));
        connect(action, &QAction::triggered, this, static_cast<void (MainWindow::*)()>(&MainWindow::openFile));
        menu->addAction(QString(tr("Remove File...")), this, &MainWindow::removeFile);
        menu->addAction(QString(tr("Duplicate File...")), this, &MainWindow::duplicateFile);
        menu->addAction(QString(tr("Rename File...")), this, &MainWindow::renameFile);
    } else if (item->data()->fileType() == MrcFile::PROJECT) {
        menu->addAction(QString(tr("Add New...")), this, &MainWindow::on_actionNewFile_triggered);
        menu->addAction(QString(tr("Add Existing File...")), this, &MainWindow::importExistingFiles);
        menu->addAction(QString(tr("Close")), this, &MainWindow::on_actionClose_triggered);
    }
    menu->exec(ui->projectTreeView->viewport()->mapToGlobal(point));
}

void MainWindow::importExistingFiles()
{
    qDebug();
    if (m_currentProject == Q_NULLPTR) return;

    SetupImportFilesDialog importFiles(this, SetupImportFilesDialog::ImportMode::File);
    QString projectPath = m_currentProject->path();
    importFiles.setupLocation(projectPath);
    int result = importFiles.exec();

    if (result != QDialog::Accepted) return;


    QList<QFileInfo> fileList = importFiles.selectProjectFileList();
    foreach(const QFileInfo info, fileList) {
        QString filePath = info.canonicalFilePath();
        bool adding = false;
        MrcFile *mrcFile = m_projectControl->addFile(m_currentProject, filePath, &adding);
        if (mrcFile == Q_NULLPTR) {
            qWarning() << tr("Cannot add the file: %1").arg(filePath);
            continue;
        } else if (adding == false && mrcFile != Q_NULLPTR) {
            // Check already registered file.
            continue;
        }

        //! Open to edit a file on the text editor.
        if (mrcFile->isEditing()) {
            openFile(mrcFile);
        }
        //! Add MRC file into the project tree model....
        addFileToProjectTreeModel(mrcFile);
    }
}

void MainWindow::removeFile()
{
    QModelIndex index = ui->projectTreeView->currentIndex();
    MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
    if (item == Q_NULLPTR) return;

    MrcAbstractFile *mrcAbstractFile = item->data();
    qDebug() << "FileName:" << mrcAbstractFile->name() << "FileType:" << mrcAbstractFile->fileType();
    QString removeFilePath = mrcAbstractFile->path();

    // Check a file deletion.
    ConfirmFileDeletionDialog confirmDialog(removeFilePath, this);
    int result = confirmDialog.exec();
    if (result != QDialog::Accepted) return;

    bool saveFile = false;
    bool permanence = confirmDialog.isPermanence();
    if (permanence) saveFile = !permanence;

    // Remove an item from GUI(Combobox and TextEdit)
    MrcFile* mrcFile = static_cast<MrcFile*>(mrcAbstractFile);
    bool finish = closeFile(mrcFile, saveFile);
    if (!finish) return;

    // Remove MRC file from Project Tree View.
    QModelIndex projectIndex = m_mrcProjectTreeModel->index(0, 0);
    int position = index.row();
    bool removeFileStatus = m_mrcProjectTreeModel->removeRows(position, 1, projectIndex);
    if (!removeFileStatus) qCritical() << "Cannot remove the file from TreeView:" << removeFilePath;

    // Remove an item from a project file.
    m_projectControl->removeFile(m_currentProject, mrcFile, permanence);
}

void MainWindow::duplicateFile()
{
    QModelIndex index = ui->projectTreeView->currentIndex();
    MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
    if (item == Q_NULLPTR) {
        return;
    }

    MrcAbstractFile *mrcAbstractFile = item->data();
    QString name = mrcAbstractFile->name() ;
    qDebug() << "FileName:" << name << "FileType:" << mrcAbstractFile->fileType();
    QString filePath = mrcAbstractFile->path();

    MrcFile *mrcFile = m_projectControl->duplicateFile(m_currentProject, filePath);
    if (mrcFile == Q_NULLPTR) {
        QMessageBox::warning(this, tr("Duplicate File"),
                             tr("A file '%1' can duplicate up to %2.").arg(name).arg(MRUBYC_IDE::MAX_DUP_FILES),
                             QMessageBox::Ok);
        qCritical() << tr("Cannot duplicate the file: %1").arg(filePath);
        return;
    }
    //! Add MRC file into the project tree model....
    addFileToProjectTreeModel(mrcFile);
}

void MainWindow::renameFile()
{
    QModelIndex index = ui->projectTreeView->currentIndex();
    MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
    if (item == Q_NULLPTR) return;

    // Makes the selected item editable.
    ui->projectTreeView->edit(index);
}

void MainWindow::openFile(const QModelIndex &index)
{
    qDebug() << "MainWindow::openFile()";
    if (index.isValid()) {
        MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
        if (item) {
            MrcAbstractFile *mrcAbstractFile = item->data();
            if (mrcAbstractFile->isNormalFile()) {
                MrcFile* mrcFile = static_cast<MrcFile *>(mrcAbstractFile);
                if (m_projectControl->editFile(mrcFile)) {
                    emit fileOpened(mrcFile);
                } else {
                    QString fileName = mrcAbstractFile->name();
                    QMessageBox::warning(this, tr(""), tr("Cannot open file: %1").arg(fileName), QMessageBox::Ok);
                }
            }
        }
    }
}


void MainWindow::sendMessage(const QString &message, AbstractProcess::MessageType type)
{
//    qDebug() << "M:" << message;
    ui->txtConsole->moveCursor(QTextCursor::End);
    if (type == AbstractProcess::MessageType::ERROR) {
        ui->txtConsole->textCursor().insertText(message, m_errorFormat);
    } else if (type == AbstractProcess::MessageType::INFOMATION) {
        ui->txtConsole->textCursor().insertText(QString("%1: %2").arg(currentTime()).arg(message), m_infomationFormat);
    } else {
        ui->txtConsole->textCursor().insertText(message, m_normalFormat);
    }

    QScrollBar *scrollBar = ui->txtConsole->verticalScrollBar();
    bool isBottom = (scrollBar->value() == scrollBar->maximum());
    if (!isBottom){
       scrollBar->setValue(scrollBar->maximum());
    }
}

void MainWindow::startWaitingSpinner()
{
    m_waitingSpinner->show();
    this->setEnabled(false);
}

void MainWindow::stopWaitingSpinner()
{
    m_waitingSpinner->hide();
    this->setEnabled(true);
}

void MainWindow::finishProcess(int result)
{
    Q_UNUSED(result);
    m_waitingSpinner->stop();

    if( m_stateSerialConsole == SERIALCONSOLE_CLOSED ) {
	m_stateSerialConsole = SERIALCONSOLE_READY_OPEN;
    }
}

void MainWindow::onConsoleParameter_changed()
{
    m_stateSerialConsole = SERIALCONSOLE_READY_OPEN;
    m_consoleTimer.setInterval( 1000 );
}

void MainWindow::onConsoleTimer_triggered()
{
    switch( m_stateSerialConsole ) {
    case SERIALCONSOLE_CLOSED: {
     } break;


    case SERIALCONSOLE_READY_OPEN: {
	BuildSetting *buildSetting = m_settingControl->ideSetting()->buidSetting();

	if( m_consoleSerialPort.isOpen() ) m_consoleSerialPort.close();
	if( buildSetting->portName().isEmpty() ) return;

	m_consoleSerialPort.setPortName( buildSetting->portName() );
	if( !buildSetting->baudRate().isEmpty() ) {
	    m_consoleSerialPort.setBaudRate( buildSetting->baudRate().toInt() );
	}
	if( !m_consoleSerialPort.open( QIODevice::ReadWrite ) ) return;	// port open error

	m_stateSerialConsole = SERIALCONSOLE_OPENED;
	m_consoleTimer.setInterval( 100 );
     } break;


    case SERIALCONSOLE_OPENED: {
	if( !m_consoleSerialPort.bytesAvailable() ) return;
	QByteArray read_data = m_consoleSerialPort.readAll();
	sendMessage(read_data);
     } break;


    case SERIALCONSOLE_READY_CLOSE: {
	m_consoleSerialPort.close();
	m_stateSerialConsole = SERIALCONSOLE_CLOSED;
	m_consoleTimer.setInterval( 1000 );
     } break;
    }
}

void MainWindow::onConsoleSerialError_triggered(QSerialPort::SerialPortError error_code)
{
    if( error_code == QSerialPort::NoError ) return;
    m_consoleSerialPort.clearError();

    if( m_stateSerialConsole == SERIALCONSOLE_OPENED ) {
	m_stateSerialConsole = SERIALCONSOLE_READY_CLOSE;
	while( m_stateSerialConsole != SERIALCONSOLE_CLOSED ) {
	    qApp->processEvents();
	}
	m_stateSerialConsole = SERIALCONSOLE_READY_OPEN;
    }
}
