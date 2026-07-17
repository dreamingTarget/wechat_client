#include "textbubble.h"

#include <QEvent>
#include <QTextBlock>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent)
    : BubbleFrame(role, parent) {
    m_text = new QTextEdit;
    m_text->setReadOnly(true);
    m_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    m_text->setFont(font);
    setPlainText(text);
    setWidget(m_text);
    initStyleSheet();
    m_text->installEventFilter(this);
}

bool TextBubble::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_text && e->type() == QEvent::Paint) {
        adjustHeight();
    }
    if (obj == m_text && e->type() == QEvent::FocusOut) {
        // 清除选中状态
        QTextCursor cursor = m_text->textCursor();
        cursor.clearSelection();
        m_text->setTextCursor(cursor);
        // 可选：将光标移到末尾，消除视觉残留
        // m_text->moveCursor(QTextCursor::End);
        return false; // 继续传递事件
    }
    return BubbleFrame::eventFilter(obj, e);
}

void TextBubble::adjustHeight()
{
    qreal doc_margin = m_text->document()->documentMargin();
    QTextDocument* doc = m_text->document();
    qreal text_height = 0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        QTextLayout* layout = it.layout();
        QRectF text_rect = layout->boundingRect();
        text_height += text_rect.height();
    }
    int VMargin = layout()->contentsMargins().top();
    setFixedHeight(text_height + doc_margin * 2 + VMargin * 2);
}

void TextBubble::setPlainText(const QString &text)
{
    // m_text->setText(text);
    // //找到段落中最大宽度
    // qreal doc_margin = m_text->document()->documentMargin();
    // int margin_left = this->layout()->contentsMargins().left();
    // int margin_right = this->layout()->contentsMargins().right();
    // QFontMetricsF fm(m_text->font());
    // QTextDocument *doc = m_text->document();
    // int max_width = 0;
    // //遍历每一段找到 最宽的那一段
    // for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    //字体总长
    // {
    //     int txtW = int(fm.width(it.text()));
    //     max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
    // }
    // //设置这个气泡的最大宽度 只需要设置一次
    // setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));

    m_text->setPlainText(text);               // 纯文本用 setPlainText，避免富文本解析
    qreal doc_margin = m_text->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();

    QFontMetricsF fm(m_text->font());
    QTextDocument *doc = m_text->document();
    doc->adjustSize();                       // 刷新文档布局，确保宽度计算准确

    qreal max_width = 0.0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        qreal txtW = fm.horizontalAdvance(it.text());  // 推荐用法，精度更高
        max_width = qMax(max_width, txtW);
    }

    // 计算气泡所需总宽度 + 5 像素余量
    int total_max_width = qRound(max_width + doc_margin * 2 + margin_left + margin_right) + 5;

    // 微信风格：限制最大宽度（例如父容器宽度的 60%），超出则换行
    int max_bubble_width = parentWidget() ? parentWidget()->width() * 0.6 : 400;
    total_max_width = qMin(total_max_width, max_bubble_width);

    setMaximumWidth(total_max_width);
}

void TextBubble::initStyleSheet()
{
    m_text->setStyleSheet("QTextEdit{background: transparent; border: none}");
    QPalette pal = m_text->palette();
    pal.setColor(QPalette::Highlight, QColorConstants::Svg::lightblue);       // 选中背景
    pal.setColor(QPalette::HighlightedText, Qt::black);    // 选中文字
    // 重要：也设置 Inactive 状态
    pal.setColor(QPalette::Inactive, QPalette::Highlight, QColorConstants::Svg::lightblue);
    pal.setColor(QPalette::Inactive, QPalette::HighlightedText, Qt::black);
    m_text->setPalette(pal);
}

