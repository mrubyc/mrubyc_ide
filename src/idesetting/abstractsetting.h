#ifndef ABSTRACTSETTING_H
#define ABSTRACTSETTING_H

#include <QObject>

/*!
 * \brief The AbstractSetting class.
 *  It is abstract class for all of the setting.
 */
class AbstractSetting : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSetting(QObject *parent = Q_NULLPTR);
    ~AbstractSetting();

    /*!
     * \brief Return the modified flag.
     * \return
     */
    virtual bool isModified();

    /*!
     * \brief Set the modified flag.
     * \param yes if the object is modified, true. Otherwise, fasle.
     */
    virtual void setModified(bool yes);

private:
    //! modify flag for setting object.
    bool m_modified;
};

#endif // ABSTRACTSETTING_H
