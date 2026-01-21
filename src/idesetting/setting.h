/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#ifndef SETTING_H
#define SETTING_H

#include <QString>

namespace Setting {
namespace Constants {
namespace Key {
    // Group key for Build and Deploy
    static const QString BUILD_AND_DEPLOY       = "BuildAndDeploy";
    static const QString COMPILER               = "Compiler";
    static const QString WRITER                 = "Writer";

    // Key for Build and Deploy
    static const QString MRBC_ENABLED           = "MrbcEnabled";
    static const QString MRBC_COMMAND           = "MrbcCommand";
    static const QString MRBC_COMMAND_OPTS      = "MrbcCommandOptions";
    static const QString CLOUD_ENABLED          = "CloudEnabled";
    static const QString CLOUD_URL              = "CloudUrl";
    static const QString CLEAR_CONSOLE_ENABLED  = "ClearConsoleEnabled";

    static const QString MRBW_COMMAND           = "MrbwCommand";
    static const QString MRBW_COMMAND_OPTS      = "MrbwCommandOptions";
    static const QString MRBW_PORT              = "Port";
    static const QString BAUD_RATE              = "BaudRate";

    // Group key for Font and Colors
    static const QString FONT_AND_COLORS        = "FontAndColors";
    static const QString FONT                   = "Font";
    static const QString COLOR_SCHEME_SETTING   = "ColorSchemeSetting";
    static const QString COLOR_SCHEME           = "ColorScheme";
    static const QString COLOR_SCHEME_ELEMENT   = "Element";

    // Key for Font and Colors
    static const QString NAME                   = "Name";
    static const QString EDITABLE               = "Editable";
    static const QString SIZE                   = "size";

    static const QString KEYWORD                = "Keyword";
    static const QString TYPE                   = "Type";
    static const QString Number                 = "Number";
    static const QString STRING                 = "String";
    static const QString CONSTANTS              = "Constants";
    static const QString SYMBOLS                = "Symbols";
    static const QString REGULAR_EXP            = "RegularExp";
    static const QString SHELL_CMD_EXP          = "ShellCmdExp";
    static const QString COMMENT                = "Comment";
    static const QString DOCUMENTS              = "Documents";

    static const QString FOREGROUND             = "Foreground";
    static const QString BACKGROUND             = "Background";
    static const QString BOLD                   = "Bold";
    static const QString ITALIC                 = "Italic";

    // key for Font and Colors
    static const QString FONT_FAMILY            = "Family";
    static const QString FONT_SIZE              = "Size";
    static const QString COLOR_SCHEME_NAME      = "DefaultColorSchemeName";
} // namespace Constants
} // namespace Key

     /*!
     * \brief The Setting Data Status enum
     */
    enum ControlType {
        //! the config has been loaded.
        LOADED = 0x1,
        //! the config has been stored.
        STORED = 0x2
    };

    /*!
     * \brief The IDESettingGroup enum.
     */
    enum IDESettingGroup {
        //! Projects Group
        Projects,
        //! IDE Group
        GenericIDE
    };

} // namespace Setting

#include <qnamespace.h>

#define CONV_CHK_STS_TO_BOOL(sts) \
    ((sts==Qt::CheckState::Checked)?true:false)
#define CONV_BOOL_TO_CHK_STS(bol) \
    ((bol==true)?Qt::CheckState::Checked:Qt::CheckState::Unchecked)

#endif // SETTING_H
