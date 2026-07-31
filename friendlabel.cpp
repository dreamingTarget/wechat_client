#include "friendlabel.h"
#include "ui_friendlabel.h"
#include "global.h"
#include <QFontMetrics>
#include <QMouseEvent>

FriendLabel::FriendLabel(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FriendLabel)
{
    ui->setupUi(this);
    setObjectName("FriendLabel");
    ui->horizontalLayout->setContentsMargins(10, 4, 8, 4);
    ui->horizontalLayout->setSpacing(4);
    ui->widget_close->setFixedSize(18, 18);
    ui->label_close->setFixedSize(14, 14);
    ui->label_tip->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ui->label_tip->setFont(QFont("Microsoft YaHei", 13));
    ui->label_tip->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    ui->label_close->setState("normal", "hover", "pressed",
                              "selected_normal", "selected_hover", "selected_pressed");
    connect(ui->label_close, &ClickedLabel::clicked, this, [this](QString, ClickLbState) {
        ui->label_close->resetNormalState();
        emit sig_close(m_text);
    });
}

FriendLabel::~FriendLabel()
{
    delete ui;
}

void FriendLabel::setText(QString text)
{
    m_text = text;
    ui->label_tip->setText(text);
    ensurePolished();
    ui->label_tip->ensurePolished();

    QFontMetrics fm(ui->label_tip->font());
    const int textWidth = fm.horizontalAdvance(text) + 2;
    const int textHeight = fm.height();
    const QMargins margins = ui->horizontalLayout->contentsMargins();
    const int spacing = ui->horizontalLayout->spacing();
    const int closeWidth = ui->widget_close->width();
    const int contentWidth = margins.left() + textWidth + spacing + closeWidth + margins.right();
    const int contentHeight = margins.top() +
                              qMax(textHeight, ui->widget_close->height()) +
                              margins.bottom();

    ui->label_tip->setFixedWidth(textWidth);
    setFixedSize(contentWidth, contentHeight);
    m_width = contentWidth;
    m_height = contentHeight;
}

int FriendLabel::width()
{
    return m_width;
}

int FriendLabel::height()
{
    return m_height;
}

QString FriendLabel::text()
{
    return m_text;
}

void FriendLabel::slot_close()
{
    emit sig_close(m_text);
}

void FriendLabel::mousePressEvent(QMouseEvent *event)
{
    if (event && event->button() == Qt::LeftButton) {
        emit sig_clicked();
    }
    QFrame::mousePressEvent(event);
}
