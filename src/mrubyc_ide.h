#ifndef MRUBYC_IDE_H
#define MRUBYC_IDE_H

namespace MRUBYC_IDE {
    //! \brief It is a maximun number of file to be able to duplicate.
    static const int MAX_DUP_FILES = 100;

    //! \brief indent as the amount of space to indent the XML node.
    static const int XML_INDENT = 2;


    //! \brief Define mark character number to be based on QFontMetrics of current font.
    static const int MARK_CHAR_NUM = 2;

    //! \brief  Define a margin for a width of the line number area.
    static const int LINE_NUM_AREA_MARGIN = 4;

    //! \brief  Default digit number to display.
    static const int DEFAULT_NUM_DIGITS = 4;

};

#define BOOL2STRING(b) (QString(b?"true":"false"))
#define STRING2BOOL(s) (s=="true":true:false)

#endif // MRUBYC_IDE_H
