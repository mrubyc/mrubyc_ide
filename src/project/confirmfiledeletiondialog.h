#ifndef CONFIRMFILEDELETIONDIALOG_H
#define CONFIRMFILEDELETIONDIALOG_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
QT_END_NAMESPACE

/*!
 * \brief The ConfirmFileDeletionDialog class.
 * It is confirmation dialog  to delete  the file from a mrbc project.
 */
class ConfirmFileDeletionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfirmFileDeletionDialog(const QString &filePath, QWidget *parent = Q_NULLPTR);
    ~ConfirmFileDeletionDialog();

    /*!
     * \brief Return the flag whether the file is removed from a file system.
     * \return If cheked deletion, true. Otherwise, false.
     */
    bool isPermanence();

protected Q_SLOTS:
    /*!
     * \brief Determine a file deletion.
     */
    void accept() override;

    /*!
     * \brief Cancel to delete a file
     */
    void reject() override;

private:
    QDialogButtonBox *m_buttonBox;
    QCheckBox *m_permanenceCheckBox;
};

#endif // CONFIRMFILEDELETIONDIALOG_H
