#include "mainwindow.h"
#include "idesettingcontrol.h"

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication apps(argc, argv);

    /*
     * Change the init file's directory the same directory with this application location.
     */
    QString currentPath = QCoreApplication::applicationDirPath();
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, currentPath);
    qDebug() << "ConfigPath:" << currentPath;

    // load ide setgings.
    QString appsName = QCoreApplication::applicationName();
    QSettings config(QSettings::IniFormat, QSettings::UserScope, appsName);

    QString fileName = config.fileName();
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists()) {
        QMessageBox::information(Q_NULLPTR,
                                 QObject::tr("mrubyc ide"),
                                 QObject::tr("No such file: %1\nPlease put it on above directory.").arg(fileName),
                                 QMessageBox::Ok);
        qWarning() << QObject::tr("No such file: %1\nPlease put it on above directory.").arg(fileName);
        exit(1);
    }
    IdeSettingControl *settingControl = new IdeSettingControl(&config);
    settingControl->load();

    MainWindow mainWin(settingControl);
    mainWin.show();

    return apps.exec();
}
