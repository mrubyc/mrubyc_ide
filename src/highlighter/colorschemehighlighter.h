/**
 * @file
 * @brief   ColorSchemeHighlighter クラス - ヘッダ
 */

#ifndef COLORSCHEMEHIGHLIGHTER_H
#define COLORSCHEMEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class ColorSchemeSetting;

/**
 * @brief カラースキームハイライター - クラス
 */
class ColorSchemeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    /**
     * @brief   コンストラクタ
     * @details
     * ColorSchemeHighlighter クラスのコンストラクタ．<br>
     * 引数の document を対象にハイライト処理をおこないます．
     *
     * @param   document    [in]    ハイライト対象の QTextDocument
     */
    ColorSchemeHighlighter( QTextDocument *document = Q_NULLPTR );
    /**
     * @brief   デストラクタ
     * @details
     * ColorSchemeHighlighter クラスのデストラクタ．<br>
     * クラス内で生成したインスタンスの解放をおこなう（クラス外で生成したインスタンスについては解放しない）
     */
    ~ColorSchemeHighlighter();

protected:
    void highlightBlock( const QString &text ) override;

private:
    void highlightTypeBlock( const QString &text, const int startOffset, QRegularExpressionMatchIterator &it, bool isClass = false );

public slots:
    /**
     * @brief   カラースキーム変更スロット
     * @param   settings    [in]    カラースキーム管理クラス
     */
    void onColorSchemeChanged( ColorSchemeSetting *settings );

private:
    class HighlightingRule;
    QVector<HighlightingRule *> _highlightRules;

    QVector<HighlightingRule *> _singleLineStartExp;
    QVector<HighlightingRule *> _singleLineEndExp;

    QRegularExpression _documentsStartExp;
    QRegularExpression _documentsEndExp;

    QList<QTextCharFormat *> _formatList;
};

#endif // COLORSCHEMEHIGHLIGHTER_H
