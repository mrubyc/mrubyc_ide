#ifndef WAITINGSPINNER_H
#define WAITINGSPINNER_H

#include <QWidget>
#include <QLabel>
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

/*!
 * \brief The WaitingSpinner class.
 */
class WaitingSpinner : public QLabel
{
    Q_OBJECT
public:
    explicit WaitingSpinner(QWidget *parent);
    ~WaitingSpinner();

    /*!
     * \brief start waiting spinner.
     */
    void start();
    /*!
     * \brief stop waiting spinner.
     */
    void stop();
signals:
    /*!
     * \brief The signal is emited when start a waiting spinner.
     */
    void started();
    /*!
     * \brief The signal is emited when stop a waiting spinner.
     */
    void stopped();
private:
    QMovie *m_movie;
};

#endif // WAITINGSPINNER_H
