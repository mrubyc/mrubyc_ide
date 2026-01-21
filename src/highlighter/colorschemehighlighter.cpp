/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.

  @file    colorschemehighlighter.cpp
  @brief   ColorSchemeHighlighter クラス - ソース
*/

#include "colorschemehighlighter.h"
#include "colorschemesetting.h"

class ColorSchemeHighlighter::HighlightingRule : public QObject
{
public:
    enum States
    {
        None = 0,
        SingleNormal,
        SingleRegExp,
        SinglePercent,
        SingleComment,
    };

    States state;
    QRegularExpression pattern;
    QTextCharFormat *format;

    HighlightingRule( const States state_, const QRegularExpression &pattern_, QTextCharFormat *format_, QObject *parent = Q_NULLPTR )
        : QObject( parent )
        , state( state_ )
        , pattern( pattern_ )
        , format( format_ )
    {
    }
};

ColorSchemeHighlighter::ColorSchemeHighlighter( QTextDocument *document )
    : QSyntaxHighlighter( document )
{
    for ( int i = 0; i < ColorSchemeSetting::MAX; ++i ) {
        _formatList.append( new QTextCharFormat() );
    }

    // Number
    _highlightRules.append( new HighlightingRule( HighlightingRule::None , QRegularExpression( "(?<!\\$)\\b0o{0,1}([0-7]_{0,1}){0,}[0-7](r|i|ri){0,1}\\b|(?<!\\$)\\b0b([0-1]_{0,1}){0,}[0-1](r|i|ri){0,1}\\b|(?<!\\$)\\b0x([0-9a-fA-F]_{0,1}){0,}[0-9a-fA-F](r|i|ri){0,1}\\b|(?<!\\$)\\b0d([0-9]_{0,1}){0,}[0-9](r|i|ri){0,1}\\b|(?<!\\$)\\b(([0-9]_{0,1}){0,}[0-9](\\.([0-9]_{0,1}){0,}[0-9]){0,1})([eE][+-]{0,1}([0-9]_{0,1}){0,}[0-9]){0,1}(r|i|ri){0,1}\\b" ), _formatList.at( ColorSchemeSetting::Number ), this ) );
    // Constants - Upper String
    _highlightRules.append( new HighlightingRule( HighlightingRule::None, QRegularExpression( "(?!\\bBEGIN\\b)(?!\\bEND\\b)\\b([A-Z_]{1}[a-zA-z0-9_]*)\\b" ), _formatList.at( ColorSchemeSetting::Constants ), this ) );
    // Keyword
    QStringList keywordPatterns;
    keywordPatterns << "\\bBEGIN\\b" << "\\bEND\\b"    << "\\balias\\b"
                    << "\\band\\b"   << "\\bbegin\\b"  << "\\bbreak\\b"
                    << "\\bcase\\b"  << "\\bdef\\b"    << "\\bdefined\\?"
                    << "\\bdo\\b"    << "\\belse\\b"   << "\\belsif\\b"
                    << "\\bend\\b"   << "\\bensure\\b" << "\\bfalse\\b"
                    << "\\bfor\\b"   << "\\bif\\b"     << "\\bin\\b"
                    << "\\bnext\\b"  << "\\bnil\\b"    << "\\bnot\\b"
                    << "\\bor\\b"    << "\\bredo\\b"   << "\\brescue\\b"
                    << "\\bretry\\b" << "\\breturn\\b" << "\\bself\\b"
                    << "\\bsuper\\b" << "\\bthen\\b"   << "\\btrue\\b"
                    << "\\bundef\\b" << "\\bunless\\b" << "\\buntil\\b"
                    << "\\bwhen\\b"  << "\\bwhile\\b"  << "\\byield\\b"
                    << "(?<!@@)(?<!@)(?<!\\$)(?<!:)(\\bclass\\b|\\bmodule\\b)";
    foreach( const QString &pattern, keywordPatterns ) {
        _highlightRules.append( new HighlightingRule( HighlightingRule::None, QRegularExpression( pattern ), _formatList.at( ColorSchemeSetting::Keyword ), this ) );
    }
    // Type
    // class & modul の横にある文字列（Type）は highlightBlock() 内にて探索をおこなう
    // Constants - (@@classVar)|(@instanceVar)|($globalVar)
    _highlightRules.append( new HighlightingRule( HighlightingRule::None, QRegularExpression( "((?<!:[+\\-])@{2}[a-zA-Z_]{1}[a-zA-Z0-9_]*)|((?<!:[+\\-])@{1}[a-zA-Z_]{1}[a-zA-Z0-9_]*)|(\\${1}([a-zA-Z_]{1}[a-zA-Z0-9_]*|[!\"$&'*+,./:;<=>?@\\\\`~_]{1}|[0-9]{1,}|(\\-[0adFiIKlpvwW]{1})))" ), _formatList.at( ColorSchemeSetting::Constants ), this ) );
    // Symbols
    _highlightRules.append( new HighlightingRule( HighlightingRule::None, QRegularExpression( "(?<!(:|\\$|[a-zA-Z_])):{1}(?!:)([a-zA-Z_]{1}[a-zA-Z0-9_]{0,}[!?=]{0,1}\\b|(@{2}[a-zA-Z_]{1}[a-zA-Z0-9_]{0,})\\b|(@{1}[a-zA-Z_]{1}[a-zA-Z0-9_]{0,})\\b|(\\${1}[a-zA-Z_]{1}[a-zA-Z0-9_]{0,})\\b|(<=>)|(===)|(==)|(=~)|(>=)|(<=)|(<<)|(>>)|(\\*\\*)|(\\+@)|(-@)|(\\[\\]=)|(\\[\\])|(!=)|(!~)|([|^&><+\\-*/%~`!]{1}))" ), _formatList.at( ColorSchemeSetting::Symbols ), this ) );

    // String & RegExp & ShellCmdExp & Comment
    // :"symbol-d-quot"
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!(:|\\$|[a-zA-Z_])):{1}(?!:)\"" ), _formatList.at( ColorSchemeSetting::Symbols ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!\\\\)\"" ), _formatList.at( ColorSchemeSetting::Symbols ), this ) );
    // :"symbol-s-quot"
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!(:|\\$|[a-zA-Z_])):{1}(?!:)'" ), _formatList.at( ColorSchemeSetting::Symbols ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!\\\\)'" ), _formatList.at( ColorSchemeSetting::Symbols ), this ) );
    // "d-quot-string"
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!(\\$|(?<!(:|\\$|[a-zA-Z_])):))\"" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!\\\\)\"" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    // 's-quot-string'
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!(\\$|(?<!(:|\\$|[a-zA-Z_])):))'" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!\\\\)'" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    // `ShellCmdExp`
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!(\\$|(?<!(:|\\$|[a-zA-Z_])):))`" ), _formatList.at( ColorSchemeSetting::ShellCmdExp ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleNormal, QRegularExpression( "(?<!\\\\)`" ), _formatList.at( ColorSchemeSetting::ShellCmdExp ), this ) );
    // /RegExp/
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleRegExp, QRegularExpression( "(?<!([a-zA-Z0-9]|\\$|:|_))/(?!=)" ), _formatList.at( ColorSchemeSetting::RegularExp ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SingleRegExp, QRegularExpression( "(?<!\\\\)/[iomxuesn]*" ), _formatList.at( ColorSchemeSetting::RegularExp ), this ) );
    // %{d-quot-string}
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!:)%{" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!\\\\)}" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    // %Q{d-quot-string}
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!:)%Q{" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!\\\\)}" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    // %q{s-quot-string}
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!:)%q{" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!\\\\)}" ), _formatList.at( ColorSchemeSetting::String ), this ) );
    // %r{RegExp}
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!:)%r{" ), _formatList.at( ColorSchemeSetting::RegularExp ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!\\\\)}[iomxuesn]*" ), _formatList.at( ColorSchemeSetting::RegularExp ), this ) );
    // %x{ShellCmdExp}
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!:)%x{" ), _formatList.at( ColorSchemeSetting::ShellCmdExp ), this ) );
    _singleLineEndExp.append( new HighlightingRule( HighlightingRule::SinglePercent, QRegularExpression( "(?<!\\\\)}" ), _formatList.at( ColorSchemeSetting::ShellCmdExp ), this ) );
    // # comment
    _singleLineStartExp.append( new HighlightingRule( HighlightingRule::SingleComment, QRegularExpression( "#.*$" ), _formatList.at( ColorSchemeSetting::Comment ), this ) );

    // Documents( multi-line )
    _documentsStartExp = QRegularExpression( "^=begin" );
    _documentsEndExp = QRegularExpression( "^=end" );
}

ColorSchemeHighlighter::~ColorSchemeHighlighter()
{
    qDeleteAll( _formatList );
}

void ColorSchemeHighlighter::highlightBlock( const QString &text )
{
    // Keyword & Constants & Number
    foreach ( const HighlightingRule *rule, _highlightRules ) {
        QRegularExpressionMatchIterator matchIterator = rule->pattern.globalMatch( text );
        while ( matchIterator.hasNext() ) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat( match.capturedStart(), match.capturedLength(), *rule->format );

            // Type 用探索
            if ( match.captured() == "class" ) {
                highlightTypeBlock( text, match.capturedEnd(), matchIterator, true );
            }
            else if ( match.captured() == "module" ) {
                highlightTypeBlock( text, match.capturedEnd(), matchIterator );
            }
        }
    }

    // single-line highlight
    int offset = 0;
    // 文中の「\\（\のエスケープ）」をスペースに置換
    QString tmpStr = QString( text ).replace( QRegularExpression( "\\\\\\\\" ), "  " );
    // 文末まで探索が完了するまでループ
    while( offset < tmpStr.length() ) {
        int minOffset = tmpStr.length();
        HighlightingRule *targetRule = Q_NULLPTR;
        QRegularExpressionMatch sMatch;
        for ( int i = 0; i < _singleLineStartExp.count(); ++i ) {
            HighlightingRule *rule = _singleLineStartExp.at( i );
            QRegularExpressionMatch m = rule->pattern.match( tmpStr, offset );
            if ( m.hasMatch() && m.capturedStart() < minOffset  ) {
                sMatch = m;
                minOffset = sMatch.capturedStart();
                targetRule = rule;
            }
        }
        int index = _singleLineStartExp.indexOf( targetRule );
        if ( index < 0 ) {
            break;
        }

        // 一行コメントの場合、開始から文末までハイライトして終了
        if ( targetRule->state == HighlightingRule::SingleComment ) {
            setFormat( minOffset, tmpStr.length() - minOffset, *targetRule->format );
            break;
        }

        QRegularExpressionMatch eMatch = _singleLineEndExp.at( index )->pattern.match( tmpStr, sMatch.capturedEnd() );
        // 終了条件が見つかった場合
        if ( eMatch.hasMatch() ) {
            // % 記法
            if ( targetRule->state == HighlightingRule::SinglePercent ) {
                int index = tmpStr.indexOf( QRegularExpression( "(?<!\\\\){" ), sMatch.capturedEnd() );
                if ( sMatch.capturedEnd() <= index && index < eMatch.capturedStart() ) {
                    tmpStr.replace( index, 1, ' ' ).replace( eMatch.capturedStart(), 1, ' ' );
                    continue;
                }
            }

            // RegExp( /.../ ) の場合
            if ( targetRule->state == HighlightingRule::SingleRegExp ) {
                QRegularExpression finishReg( "^\\s*($|[)}\\],.;:#]|(!~)|(=~))" );
                QRegularExpressionMatch m = finishReg.match( text.mid( eMatch.capturedEnd() ) );
                if ( !m.hasMatch() ) {
                    ++offset;
                    continue;
                }
                setFormat( minOffset, eMatch.capturedEnd() - minOffset, *targetRule->format );
            }
            // その他の単行ハイライトの場合
            else {
                setFormat( minOffset, eMatch.capturedEnd() - minOffset, *targetRule->format );
            }
            offset = eMatch.capturedEnd();
        }
        // 終了条件が見つからなかった場合
        else {
            if ( targetRule->state != HighlightingRule::SingleRegExp ) {
                setFormat( minOffset, text.length() - minOffset, *targetRule->format );
            }
            else {
                ++offset;
                continue;
            }
            break;
        }
    }
    setCurrentBlockState( 0 );

    // multi-line highlight( Documents Only )
    int startPos = 0;
    // 1 つ上の行が複数行ハイライト状態でない場合は、開始文字列を探索
    if ( previousBlockState() < ColorSchemeSetting::Documents ) {
        startPos = text.indexOf( _documentsStartExp );
    }
    while ( 0 <= startPos ) {
        QRegularExpressionMatch match = _documentsEndExp.match( text, startPos );
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if ( endIndex == -1 ) {
            // 終了条件が見つからない場合は、ハイライト範囲を開始から文末までとし、行の状態を変更する
            setCurrentBlockState( ColorSchemeSetting::Documents );
            commentLength = text.length() - startPos;
        }
        else {
            // 終了条件が見つかった場合は、ハイライト範囲を開始から終了までとする
            commentLength = endIndex - startPos + match.capturedLength();
        }
        setFormat( startPos, commentLength, ( *_formatList.at( ColorSchemeSetting::Documents ) ) );
        startPos = text.indexOf( _documentsStartExp, startPos + commentLength );
    }
}

void ColorSchemeHighlighter::highlightTypeBlock( const QString &text, const int startOffset, QRegularExpressionMatchIterator &it, bool isClass )
{
    int keywordCount = 0;

    // class & module highlight
    QRegularExpressionMatch m = QRegularExpression( "\\S" ).match( text, startOffset );
    int offset = m.capturedStart();
    m = QRegularExpression( "[a-zA-Z_]" ).match( m.captured() );
    if ( !m.hasMatch() ) {
        goto BREAK_TYPE_HIGHLIGHT;
    }
    m = QRegularExpression( "[a-zA-Z0-9_]+" ).match( text, offset );
    setFormat( m.capturedStart(), m.capturedLength(), *( _formatList[ColorSchemeSetting::Type] ) );
    if ( m.captured() == "class" || m.captured() == "module" ) {
        ++keywordCount;
    }
    // base class highlight
    if ( isClass ) {
        m = QRegularExpression( "\\S" ).match( text, m.capturedEnd() );
        offset = m.capturedEnd();
        m = QRegularExpression( "<" ).match( m.captured() );
        if ( !m.hasMatch() ) {
            goto BREAK_TYPE_HIGHLIGHT;
        }
        m = QRegularExpression( "\\S" ).match( text, offset );
        offset = m.capturedStart();
        m = QRegularExpression( "[a-zA-Z_]" ).match( m.captured() );
        if ( !m.hasMatch() ) {
            goto BREAK_TYPE_HIGHLIGHT;
        }
        m = QRegularExpression( "[a-zA-Z0-9_]+" ).match( text, offset );
        setFormat( m.capturedStart(), m.capturedLength(), *( _formatList[ColorSchemeSetting::Type] ) );
        if ( m.captured() == "class" || m.captured() == "module" ) {
            ++keywordCount;
        }
    }

BREAK_TYPE_HIGHLIGHT:
    for ( int i = 0; i < keywordCount; ++i ) {
        if ( it.hasNext() ) {
            it.next();
        } else {
            break;
        }
    }
}

void ColorSchemeHighlighter::onColorSchemeChanged(ColorSchemeSetting *settings)
{
    for ( int i = 0; i < _formatList.count(); ++i ) {
        ColorSchemeElementSetting *scheme = settings->elementSettingList()->at( i );
        _formatList[i]->clearForeground();
        if ( !scheme->foreground().isEmpty() ) {
            _formatList[i]->setForeground( QBrush( QColor( scheme->foreground() ) ) );
        }
        _formatList[i]->clearBackground();
        if ( !scheme->background().isEmpty() ) {
            _formatList[i]->setBackground( QBrush( QColor( scheme->background() ) ) );
        }
        _formatList[i]->setFontWeight( ( scheme->bold() ? QFont::Bold : QFont::Normal ) );
        _formatList[i]->setFontItalic( scheme->italic() );
    }
    rehighlight();
}
