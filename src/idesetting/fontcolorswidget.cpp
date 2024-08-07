#include "fontcolorswidget.h"

#include "setting.h"
#include "fontcolorssetting.h"
#include "colorschemedialog.h"
#include "colorschemeelementsetting.h"

#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QColor>
#include <QColorDialog>
#include <QMap>
#include <QDebug>

FontColorsWidget::FontColorsWidget(FontColorsSetting *fontColorSetting, QWidget *parent)
    : QWidget(parent)
     ,m_fontColorsSetting(fontColorSetting)
{
    // Font Group
    QGroupBox *fontGroup = new QGroupBox(tr("Font"));

    QLabel *fontFamilyLabel = new QLabel(tr("Family"));
    m_fontFamilyComboBox = new QFontComboBox;

    QLabel *fontSizeLabel = new QLabel(tr("Size"));
    m_fontSizeComboBox = new QComboBox;

    QHBoxLayout *fontLayout = new QHBoxLayout;
    fontLayout->addWidget(fontFamilyLabel);
    fontLayout->addWidget(m_fontFamilyComboBox);
    fontLayout->addWidget(fontSizeLabel);
    fontLayout->addWidget(m_fontSizeComboBox);
    fontLayout->addStretch();
    fontGroup->setLayout(fontLayout);

    // Show Setting...
    QGroupBox *showSettingGroup = new QGroupBox(tr("Show Setting for"));

    //// Copy and Delete Control.
    m_settingCombo = new QComboBox;
    m_settingCombo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_copySettingButton = new QPushButton(tr("Copy..."));
    m_copySettingButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    m_deleteSettingButton = new QPushButton(tr("Delete"));
    m_deleteSettingButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    QHBoxLayout *settingLayout = new QHBoxLayout;
    settingLayout->addWidget(m_settingCombo);
    //settingLayout->addStretch();
    settingLayout->addWidget(m_copySettingButton);
    settingLayout->addWidget(m_deleteSettingButton);

    //// Color Scheme
    QGroupBox *colorSchemeSettingGroup = new QGroupBox(tr("Color Scheme"));

    QLabel *colorSchemeItemLabel = new QLabel(tr("Display Item"));
    m_colorSchemeItemList = new QListWidget;
    m_colorSchemeItemList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    m_boldCheckBox = new QCheckBox(tr("Bold"));
    m_italicCheckBox = new QCheckBox(tr("Italic"));

    QLabel *foregroundLabel = new QLabel(tr("Foreground"));
    m_foregroundSetupButton = new QPushButton(tr("..."));
    m_foregroundSetupButton->setMaximumWidth(30);
    m_foregroundClearButton = new QPushButton(tr("X"));
    m_foregroundClearButton->setMaximumWidth(30);
    QLabel *backgroundLabel = new QLabel(tr("Background"));
    m_backgroundSetupButton = new QPushButton(tr("..."));
    m_backgroundSetupButton->setMaximumWidth(30);
    m_backgroundClearButton = new QPushButton(tr("X"));
    m_backgroundClearButton->setMaximumWidth(30);
    m_previewAreaLabel = new QLabel(tr("AaBbCcZzYyZz"));
    m_previewAreaLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
    m_previewAreaLabel->setAlignment(Qt::AlignCenter);

    ////// Left layout for Color Scheme.
    QVBoxLayout *colorSchemeItemVLayout = new QVBoxLayout;
    colorSchemeItemVLayout->addWidget(colorSchemeItemLabel);
    colorSchemeItemVLayout->addWidget(m_colorSchemeItemList);

    ////// Right layout for Color Scheme.
    QHBoxLayout *itemStyleHLayout = new QHBoxLayout;
    itemStyleHLayout->addWidget(m_boldCheckBox);
    itemStyleHLayout->addWidget(m_italicCheckBox);

    QGridLayout *colorGLayout = new QGridLayout;
    colorGLayout->setColumnStretch(0,1);
    colorGLayout->addWidget(foregroundLabel,0,0,Qt::AlignLeft);
    colorGLayout->addWidget(m_foregroundSetupButton,0,1);
    colorGLayout->addWidget(m_foregroundClearButton,0,2);
    colorGLayout->addWidget(backgroundLabel,1,0,Qt::AlignLeft);
    colorGLayout->addWidget(m_backgroundSetupButton,1,1);
    colorGLayout->addWidget(m_backgroundClearButton,1,2);

    QVBoxLayout *itemSettingVLayout = new QVBoxLayout;
    itemSettingVLayout->addLayout(itemStyleHLayout);
    itemSettingVLayout->addLayout(colorGLayout);
    itemSettingVLayout->addWidget(m_previewAreaLabel);

    //// layout for Color Scheme.
    QHBoxLayout *colorSchemeHLayout = new QHBoxLayout;
    colorSchemeHLayout->addLayout(colorSchemeItemVLayout);
    colorSchemeHLayout->addLayout(itemSettingVLayout);
    colorSchemeSettingGroup->setLayout(colorSchemeHLayout);

    // Layout for Show Setting...
    QVBoxLayout *settingVLayout = new QVBoxLayout;
    settingVLayout->addLayout(settingLayout);
    settingVLayout->addWidget(colorSchemeSettingGroup);
    showSettingGroup->setLayout(settingVLayout);

    // layout
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(fontGroup);
    vLayout->addWidget(showSettingGroup);

    setLayout(vLayout);


    // Set a value on an each element.
    QString fontFamily = m_fontColorsSetting->fontFamily();
    int fontSize = m_fontColorsSetting->fontSize();
    if (fontFamily.isEmpty()) {
        QFont font;
        fontFamily = font.defaultFamily();
        qWarning() << "System needs the fontFamily. It uses" << fontFamily << ".";
    }

    QFont font(fontFamily);
    m_fontFamilyComboBox->setCurrentFont(font);
    // set up a font size for combobox.
    createFontSizeComboBox(font);

    QString param;
    param.setNum(fontSize);
    int fontSizeIndex = m_fontSizeComboBox->findText(param);
    if (fontSizeIndex < 0) fontSizeIndex = 0;
    m_fontSizeComboBox->setCurrentIndex(fontSizeIndex);
    qDebug() << "Font:"<<font << ", Index:" << fontSizeIndex;

    // Setup a combobox for the color scheme.
    QList<ColorSchemeSetting*> *colorSchemeSettingList = m_fontColorsSetting->colorSchemeSettingList();
    for (int i=0; i<colorSchemeSettingList->size(); i++) {
        ColorSchemeSetting* colorSchemeSetting = colorSchemeSettingList->at(i);
        m_settingCombo->addItem(colorSchemeSetting->name());
    }

    QString colorSchemeName = m_fontColorsSetting->defaultColorSchemeName();
    changeCurrentColorScheme(colorSchemeName);


    // set a button on a relative slot.
    connect(m_fontFamilyComboBox, &QFontComboBox::currentFontChanged, this, &FontColorsWidget::changeCurrentFont);
    connect(m_fontFamilyComboBox, &QFontComboBox::currentFontChanged, this, &FontColorsWidget::createFontSizeComboBox);
    connect(m_fontSizeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FontColorsWidget::changeCurrentFontSize);
    // Manage a color scheme.
    connect(m_settingCombo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, static_cast<void (FontColorsWidget::*)(const QString&)>(&FontColorsWidget::changeCurrentColorScheme));
    connect(m_copySettingButton, &QPushButton::clicked, this, &FontColorsWidget::copyColorScheme);
    connect(m_deleteSettingButton, &QPushButton::clicked, this, &FontColorsWidget::deleteColorScheme);
    // Control for each color scheme.
    connect(m_colorSchemeItemList, &QListWidget::currentRowChanged, this, &FontColorsWidget::changeCurrentColorSchemeElement);
    connect(m_boldCheckBox, &QCheckBox::stateChanged, this, &FontColorsWidget::setupBoldStyle);
    connect(m_italicCheckBox, &QCheckBox::stateChanged, this, &FontColorsWidget::setupItaticStyle);
    connect(m_foregroundSetupButton, &QPushButton::clicked, this, &FontColorsWidget::setupForegroundColor);
    connect(m_foregroundClearButton, &QPushButton::clicked, this, &FontColorsWidget::clearForegroundColor);
    connect(m_backgroundSetupButton, &QPushButton::clicked, this, &FontColorsWidget::setupBackgroundColor);

    connect(m_backgroundClearButton, &QPushButton::clicked, this, &FontColorsWidget::clearBackgroundColor);
}

FontColorsWidget::~FontColorsWidget()
{

}

void FontColorsWidget::createFontSizeComboBox(const QFont &font)
{
    QFontDatabase database;
    m_fontSizeComboBox->clear();

    // If the values of specified family name and style name are not set correctly,
    // smoothsizes() may be returned empty list.
    // Firstly, the method retrives a style string from QFontDatabase.
    // It uses to set them a parameter of that's method.
    // If it gets empty list, it uses standardsizes(), insted of that's method.
    QStringList familyList = database.styles(font.family());
    QString style;
    if (familyList.size()) {
        style = familyList.at(0);
    }

    QList<int> fontSizeList;

    fontSizeList = database.smoothSizes(font.family(), style);
    if (fontSizeList.size() <= 0) {
        fontSizeList = QFontDatabase::standardSizes();
    }

    foreach (int points, fontSizeList) {
        QString point;
        point.setNum(points);
        m_fontSizeComboBox->addItem(point);
    }
}

void FontColorsWidget::changeCurrentFont(const QFont &font)
{
    qDebug() << "Font:" << font;
    m_fontColorsSetting->setFontFamily(font.family());

    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::FONT;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::changeCurrentFontSize(int index)
{

    QString fontSize = m_fontSizeComboBox->currentText();
    qDebug() << "Current Index:" << index
             << "FontSize:" << fontSize;
    m_fontColorsSetting->setFontSize(fontSize.toInt());

    int colorSchemeIndex = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(colorSchemeIndex);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << colorSchemeIndex;
        return;
    }

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::FONT;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::copyColorScheme()
{
    QList<ColorSchemeSetting*> *colorSchemeSettingList = m_fontColorsSetting->colorSchemeSettingList();

    int colorSchemeCount = 10;
    if (colorSchemeSettingList->size() >= colorSchemeCount) {
        QMessageBox::warning(this, tr("Color Scheme"), tr("You can only register colors sceme less than %1.").arg(colorSchemeCount), QMessageBox::Ok);
        return;
    }

    int index = m_settingCombo->currentIndex();

    ColorSchemeSetting* colorSchemeSetting = colorSchemeSettingList->at(index);

    QString currentColorSchemeName = colorSchemeSetting->name();
    ColorSchemeDialog colorSchemeDialog(m_fontColorsSetting, this);
    colorSchemeDialog.setColorSchemeName(currentColorSchemeName);

    // 1. popup name modification dialog
    // 2. if same name is registered, show message and re-enter name.
    do {
        int resultCode = colorSchemeDialog.exec();

        if (resultCode != QDialog::Accepted) {
            break;
        } else {
            QString inputedName = colorSchemeDialog.colorSchemeName();

            // If inputed name exists in ColorSetting, try to input a name again.
            // Otherwise, copy from selected ColorSetting to new one as inputed name.
            ColorSchemeSetting *findColorSchemeSetting = m_fontColorsSetting->findColorScheme(inputedName);
            if (findColorSchemeSetting != Q_NULLPTR) {
                QMessageBox mb(tr("Color Scheme"),
                              tr("The Color Scheme is already registered.\nPlease input a name again."),
                              QMessageBox::NoIcon,
                              QMessageBox::Ok | QMessageBox::Default,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
                mb.exec();
            } else {
                // Copy new ColorScheme to a setting data.
                findColorSchemeSetting = m_fontColorsSetting->findColorScheme(currentColorSchemeName);
                ColorSchemeSetting *newColorSchemeSetting = new ColorSchemeSetting(*findColorSchemeSetting);
                newColorSchemeSetting->setName(inputedName);
                newColorSchemeSetting->setEditable(true);
                newColorSchemeSetting->setParent(m_fontColorsSetting);

                m_fontColorsSetting->addColorSchemeSetting(newColorSchemeSetting);

                // Active new ColorScheme in combobox.
                m_settingCombo->addItem(inputedName);
                changeCurrentColorScheme(inputedName);

                break;
            }
        }
    } while(true);
}

void FontColorsWidget::deleteColorScheme()
{
    QList<ColorSchemeSetting*> *colorSchemeSettingList = m_fontColorsSetting->colorSchemeSettingList();
    int index = m_settingCombo->currentIndex();

    ColorSchemeSetting* colorSchemeSetting = colorSchemeSettingList->at(index);
    QString currentColorSchemeSettingName = colorSchemeSetting->name();
    if (!colorSchemeSetting->editable()) {
        qWarning() << currentColorSchemeSettingName << "cannot be removed because it is diseditable.";
        return;
    } else {
        QMessageBox mb("Delete Color Scheme",
                      "Are you sure you want to delete this color scheme permanently?",
                      QMessageBox::Question,
                      QMessageBox::Yes | QMessageBox::Default,
                      QMessageBox::Cancel | QMessageBox::Escape,
                      QMessageBox::NoButton);
       if (mb.exec() == QMessageBox::Yes) {
            colorSchemeSettingList->removeAt(index);
//            delete colorSchemeSetting; // FIXME:XXXXX need???

            // for combobox.
            m_settingCombo->removeItem(index);
       }
    }
}

void FontColorsWidget::changeCurrentColorScheme(int index)
{
    QList<ColorSchemeSetting*> *colorSchemeSettingList = m_fontColorsSetting->colorSchemeSettingList();
    int size = colorSchemeSettingList->size();
    if (!((size > 0) && (size > index))) {
        setEnabled(false);
        qCritical() << "The index "<<index<<" is invalid.";
        return;
    } else {
        setEnabled(true);
    }

    m_colorSchemeItemList->clear();
    m_currentColorSchemeSetting = colorSchemeSettingList->at(index);

    QList<ColorSchemeElementSetting*> *elementSettingList = m_currentColorSchemeSetting->elementSettingList();
    for (int i=0; i<elementSettingList->size(); i++) {
        m_colorSchemeItemList->addItem(elementSettingList->value(i)->name());
    }

    // when a color schemed is selected, the element is selected a first row.
    m_colorSchemeItemList->setCurrentRow(0);
    changeCurrentColorSchemeElement(0);

    // update an index by a parameter value.
    m_fontColorsSetting->setDefaultColorSchemeName(m_settingCombo->currentText());

    // Settup editable Color Scheme
    bool editableColorScheme = m_currentColorSchemeSetting->editable();
    m_deleteSettingButton->setEnabled(editableColorScheme);
    //    m_copySettingButton->setEnabled(editableColorScheme);
    m_boldCheckBox->setEnabled(editableColorScheme);
    m_italicCheckBox->setEnabled(editableColorScheme);
    m_foregroundSetupButton->setEnabled(editableColorScheme);
    m_foregroundClearButton->setEnabled(editableColorScheme);
    m_backgroundSetupButton->setEnabled(editableColorScheme);
    m_backgroundClearButton->setEnabled(editableColorScheme);
}

ColorSchemeElementSetting *FontColorsWidget::currentColorSchemeElementSetting(int index)
{
    ColorSchemeElementSetting *elementSetting = Q_NULLPTR;

    QList<ColorSchemeElementSetting*> *elementSettingList = m_currentColorSchemeSetting->elementSettingList();
    if (elementSettingList->size() > index && index >= 0)
        elementSetting = elementSettingList->at(index);

    return elementSetting;
}

void FontColorsWidget::decolatePreviewArea(const FontColorsWidget::DecorationType &type, ColorSchemeElementSetting *elementSetting)
{
    if ((type & FontColorsWidget::FOREGROUND) || (type & FontColorsWidget::BACKGROUND) ) {
        QPalette labelPalette = m_previewAreaLabel->palette();

        QString foreground = elementSetting->foreground();
        if (!foreground.isEmpty()) {
           labelPalette.setColor(QPalette::Foreground, QColor(foreground));
        } else {
           labelPalette.setColor(QPalette::Foreground, QColor("black"));
        }

        // If the background color is not set in a setting, auto fill background is false.
        // But the color is set, the method sets the palet the cofingured color,
        // and auto fill background is true.
        bool autoFillBackground = false;
        QString background = elementSetting->background();
        if (!background.isEmpty()) {
            labelPalette.setColor(QPalette::Background, QColor(background));
            autoFillBackground = true;
        } else {
            autoFillBackground = false;
        }

        m_previewAreaLabel->setAutoFillBackground(autoFillBackground);
        m_previewAreaLabel->setPalette(labelPalette);
    }

    if ((type & FontColorsWidget::BOLD)
         || (type & FontColorsWidget::ITALIC)
         || (type & FontColorsWidget::FONT)) {
        bool boldFlag = false;
        bool italicFlag = false;

        QString fontFamily = m_fontColorsSetting->fontFamily();
        int fontSize = m_fontColorsSetting->fontSize();
        QFont font(fontFamily, fontSize);
        boldFlag = elementSetting->bold();
        italicFlag = elementSetting->italic();

        font.setBold(boldFlag);
        font.setItalic(italicFlag);

        m_previewAreaLabel->setFont(font);
    }

}

void FontColorsWidget::changeCurrentColorScheme(const QString &text)
{
    int index = m_settingCombo->findText(text);
    if (index < 0) {
        qWarning() << text << " does not exit in the color scheme list. So that the method sets first item.";
        index = 0;
    }

    m_settingCombo->setCurrentIndex(index);
    changeCurrentColorScheme(index);
}

void FontColorsWidget::changeCurrentColorSchemeElement(int index)
{
    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;//FIXME: index is -1 when contents of a list is chenged. this case should not be error.
        return;
    }

    Qt::CheckState status;
    status = CONV_BOOL_TO_CHK_STS(elementSetting->bold());
    m_boldCheckBox->setCheckState(status);

    status = CONV_BOOL_TO_CHK_STS(elementSetting->italic());
    m_italicCheckBox->setCheckState(status);

    FontColorsWidget::DecorationType type;
    type = static_cast<DecorationType>(FontColorsWidget::BACKGROUND | FontColorsWidget::FOREGROUND | FontColorsWidget::BOLD | FontColorsWidget::ITALIC);
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::setupBoldStyle(int status)
{
    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }

    elementSetting->setBold(CONV_CHK_STS_TO_BOOL(status));

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::BOLD;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::setupItaticStyle(int status)
{
    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }

    elementSetting->setItalic(CONV_CHK_STS_TO_BOOL(status));

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::ITALIC;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::setupForegroundColor(bool checked)
{
    Q_UNUSED(checked)

    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }
    QString currentColor = elementSetting->foreground();

    QColor color = QColorDialog::getColor(currentColor, this);
    if (!color.isValid()) return;

    QString newColor = color.name();

    elementSetting->setForeground(newColor);

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::FOREGROUND;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::setupBackgroundColor(bool /*checked*/)
{
    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }
    QString currnentColor = elementSetting->background();

    QColor color = QColorDialog::getColor(currnentColor, this);
    if (!color.isValid()) return;

    QString newColor = color.name();

    elementSetting->setBackground(newColor);

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::BACKGROUND;
    decolatePreviewArea(type, elementSetting);
}

void FontColorsWidget::clearForegroundColor(bool /*checked*/)
{
    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }
    elementSetting->setForeground("");

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::FOREGROUND;
    decolatePreviewArea(type, elementSetting);

}

void FontColorsWidget::clearBackgroundColor(bool /*checked*/)
{
    int index = m_colorSchemeItemList->currentRow();

    ColorSchemeElementSetting *elementSetting = currentColorSchemeElementSetting(index);
    if (elementSetting == Q_NULLPTR) {
        qCritical() << "Invalid Color Scheme Element. index:" << index;
        return;
    }
    elementSetting->setBackground("");

    FontColorsWidget::DecorationType type;
    type = FontColorsWidget::BACKGROUND;
    decolatePreviewArea(type, elementSetting);
}
