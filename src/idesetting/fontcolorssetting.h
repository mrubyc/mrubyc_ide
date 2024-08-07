#ifndef FONTCOLORSSETTING_H
#define FONTCOLORSSETTING_H

#include "abstractsetting.h"
#include "colorschemesetting.h"

#include <QObject>
#include <QString>
#include <QList>

/*!
 * \brief The FontColorsSetting class.
 * The setting class manages properties such as fonts, sizes and color schemes.
 */
class FontColorsSetting : public AbstractSetting
{
    Q_OBJECT
    Q_DISABLE_COPY(FontColorsSetting)
public:
    explicit FontColorsSetting(QObject *parent = Q_NULLPTR);
    ~FontColorsSetting();

    QString fontFamily() const;
    void setFontFamily(const QString &fontFamily);

    int fontSize() const;
    void setFontSize(int fontSize);

    QString defaultColorSchemeName() const;
    void setDefaultColorSchemeName(const QString &defaultColorSchemeName);

    QList<ColorSchemeSetting*> *colorSchemeSettingList();

    /*!
     * \brief This method returns ColorSchemeSetting object of the parameter name.
     *          if it does not exit, return null.
     * \param name Color Scheme Name.
     * \return find the target object.
     */
    ColorSchemeSetting* findColorScheme(const QString &name);

    /*!
     * \brief add ColorSchemeSetting to a list.
     * \param colorSchemeSetting
     */
    void addColorSchemeSetting(ColorSchemeSetting* colorSchemeSetting);

    /*!
     * \brief remove ColorSchemeSetting from a list.
     * \param colorSchemeSetting
     */
    void removeColorSchemeSetting(ColorSchemeSetting* colorSchemeSetting);

    /*!
     * \brief Clear all of element.
     */
    void clear();

    /*!
     * \brief Return the modified flag.
     * \return
     */
    bool isModified() override;

    /*!
     * \brief Set the modified flag.
     * \param yes if the object is modified, true. Otherwise, fasle.
     */
    void setModified(bool yes) override;

private:
    QString m_fontFamily;
    int m_fontSize;
    QString m_defaultColorSchemeName;

    QList<ColorSchemeSetting*> m_colorSchemeSettingList;
};

#endif // FONTCOLORSSETTING_H
