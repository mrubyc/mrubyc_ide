#ifndef COLORSCHEMEELEMENTSETTING_H
#define COLORSCHEMEELEMENTSETTING_H

#include <QObject>
#include <QString>

#include "abstractsetting.h"

/*!
 * \brief The ColorSchemeElementSetting class.
 * The setting class manages properties that it is a color scheme element for a highlighter.
 */
class ColorSchemeElementSetting : public AbstractSetting
{
    Q_OBJECT
public:
    explicit ColorSchemeElementSetting(QObject *parent = Q_NULLPTR);
    explicit ColorSchemeElementSetting(const ColorSchemeElementSetting & self);
    ~ColorSchemeElementSetting();

    QString name() const;
    void setName(const QString &name);

    QString foreground() const;
    void setForeground(const QString &foreground);

    QString background() const;
    void setBackground(const QString &background);

    bool bold() const;
    void setBold(bool bold);

    bool italic() const;
    void setItalic(bool italic);

private:
    QString m_name;
    QString m_foreground;
    QString m_background;
    bool m_bold;
    bool m_italic;
};

#endif // COLORSCHEMEELEMENTSETTING_H
