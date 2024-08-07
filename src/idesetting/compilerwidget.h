#ifndef COMPILERWIDGET_H
#define COMPILERWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QNetworkReply>

class BuildSetting;

/*!
 * \brief Set up the compiler.
 * It is a setting widget to compile ruby source files.
 */
class CompilerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CompilerWidget(BuildSetting *buildSetting, QWidget *parent = Q_NULLPTR);
    ~CompilerWidget();

private Q_SLOTS:
    /*!
     * \brief Set up a compiler.
     */
    void setupCompiler();
    /*!
     * \brief Test a cloud compiler.
     */
    void testCloudCompiler();
    /*!
     * \brief replyFinished
     * \param reply
     */
    void finishReply(QNetworkReply* reply);

    /*!
     * \brief Set up a mrbc command enabled.
     * \param status
     */
    void setupMrbcEnabled(int status);

    /*!
     * \brief Apply a local compiler command to the setting when a mrbc is changed.
     */
    void changeMrbcComand();
    /*!
     * \brief Apply a local compiler command options to the setting when a mrbc is changed.
     */
    void changeMrbcComandOptions();

    /*!
     * \brief Set up a mrbc command enabled.
     * \param status
     */
    void setupCloudEnabled(int status);

    /*!
     * \brief Apply a cloud compiler url to the setting when an url is changed.
     */
    void cloudUrlChanged();

private:
    BuildSetting *m_buildSetting;

    QCheckBox *m_localCompilerCheckBox;
    QLineEdit *m_mrbcPathLineEdit;
    QLineEdit *m_mrbcOptionsLineEdit;

    QCheckBox *m_cloudCompilerCheckBox;
    QLineEdit *m_cloudLineEdit;
};

#endif // COMPILERWIDGET_H
