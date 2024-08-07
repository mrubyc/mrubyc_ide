#ifndef FONTCOLORSWIDGET_H
#define FONTCOLORSWIDGET_H

#include <QWidget>
#include <QFontComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class FontColorsSetting;
class ColorSchemeSetting;
class ColorSchemeElementSetting;

/*!
 * \brief The setting class for a font and color.
 * The FontColorsWidget is configured a font and color for Text Editor.
 */
class FontColorsWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(DecorationType)
public:
    explicit FontColorsWidget(FontColorsSetting *fontColorSetting, QWidget *parent = Q_NULLPTR);
    ~FontColorsWidget();

    /*!
     * \brief This enum specified what the preview area should be decorated with a type.
     */
    enum DecorationType {
        //! It will be changed the font style of Bold.
        BOLD = 0x01,
        //! It will be changed the font style of Itatic.
        ITALIC = 0x2,
        //! It will be changed the charactors' foreground color.
        FOREGROUND = 0x4,
        //!  It will be changed the charactors' background color.
        BACKGROUND = 0x8,
        //! It will be changed the font family or size.
        FONT = 0x16
    };

private:
    /*!
     * \brief Change a current Color Scheme to a selected one with CoboBox.
     * \param index
     */
    void changeCurrentColorScheme(int index);

    /*!
     * \brief Return a current ColorSchemeElementSetting.
     * \param index index for a list of ColorSchemeElementSetting
     * \return current ColorSchemeElementSetting.
     *         If it does not get an object for an index, return Q_NULLPTR.
     */
    ColorSchemeElementSetting *currentColorSchemeElementSetting(int index);

    /*!
     * \brief Decolate a preview area with Color Scheme Element.
     * \param type decolation type.
     * \param elementSetting
     */
    void decolatePreviewArea(const DecorationType &type, ColorSchemeElementSetting *elementSetting);

private Q_SLOTS:
    /*!
     * \brief This property sets size list of the currently selected font.
     * \param font
     */
    void createFontSizeComboBox(const QFont& font);

    /*!
     * \brief Change current font family.
     * \param font the selected font.
     */
    void changeCurrentFont(const QFont &font);
    /*!
     * \brief Change the current font size.
     * \param index changed index for combobox.
     */
    void changeCurrentFontSize(int index);

    /*!
     * \brief Copy a selected Color Scheme.
     *        If the inputted name has not been registered,
     *        this method copies a selected Color Scheme in memory.
     */
    void copyColorScheme();

    /*!
     * \brief Delete a selected ColorScheme from memory.
     */
    void deleteColorScheme();

    /*!
     * \brief Change a current Color Scheme to a selected one with CoboBox.
     * \param text selected Color Schme name.
     */
    void changeCurrentColorScheme(const QString &text);

    /*!
     * \brief Change a current Color Scheme to a selected one with List.
     * \param index
     */
    void changeCurrentColorSchemeElement(int index);

    /*!
     * \brief Set up a bold style for the selected Color Scheme Element(disply item) with CheckBox.
     * \param checked
     */
    void setupBoldStyle(int status);

    /*!
     * \brief Set up an italic style for the selected Color Scheme Element(display item) with CheckBox.
     * \param checked
     */
    void setupItaticStyle(int status);

    /*!
     * \brief Set up a foreground color for the Color Scheme Element(selected disply) item on ColorDialog.
     * \param checked
     */
    void setupForegroundColor(bool checked);

    /*!
     * \brief Set up a background color for the selected Color Scheme Element(disply item) on ColorDialog.
     * \param checked
     */
    void setupBackgroundColor(bool);

    /*!
     * \brief Clear a foreground color for the selected Color Scheme Element(disply item).
     *        The color becomes black.
     * \param checked
     */
    void clearForegroundColor(bool);

    /*!
     * \brief Clear a background color for the selected Color Scheme Element(disply item) on ColorDialog.
     * \param checked
     */
    void clearBackgroundColor(bool checked);

private:
    FontColorsSetting *m_fontColorsSetting;
    ColorSchemeSetting* m_currentColorSchemeSetting;

    QFontComboBox *m_fontFamilyComboBox;
    QComboBox *m_fontSizeComboBox;

    QPushButton *m_copySettingButton;
    QPushButton *m_deleteSettingButton;

    QComboBox *m_settingCombo;
    QListWidget *m_colorSchemeItemList;
    QCheckBox *m_boldCheckBox;
    QCheckBox *m_italicCheckBox;

    QPushButton *m_foregroundSetupButton;
    QPushButton *m_foregroundClearButton;
    QPushButton *m_backgroundSetupButton;
    QPushButton *m_backgroundClearButton;

    QLabel *m_previewAreaLabel;
};

#endif // FONTCOLORSWIDGET_H
