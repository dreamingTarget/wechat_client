#include "applyfriend.h"
#include "ui_applyfriend.h"
#include "friendlabel.h"
#include <QApplication>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QPushButton>
#include <QScrollBar>
#include <QTimer>
#include <algorithm>
#include "usermanager.h"
#include "global.h"

namespace {
const int kFriendTagAreaPadding = 10;
const int kFriendTagHorizontalGap = 6;
const int kFriendTagVerticalGap = 4;
const int kTipListMinHeight = 50;
}

ApplyFriend::ApplyFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setObjectName("ApplyFriend");
    setModal(true);
    setWindowTitle(QStringLiteral("申请添加好友"));

    ui->verticalLayout_2->setContentsMargins(18, 12, 18, 8);
    ui->verticalLayout_2->setSpacing(12);
    ui->verticalLayout_3->setSpacing(8);
    ui->horizontalLayout->setContentsMargins(30, 10, 30, 6);
    ui->horizontalLayout->setSpacing(20);
    ui->label_apply->setFixedHeight(34);

    ui->lineEdit_apply_name->setPlaceholderText(tr("恋恋风辰"));
    ui->lineEdit_label->setPlaceholderText("搜索、添加标签");
    ui->lineEdit_notes->setPlaceholderText("燃烧的胸毛");
    ui->lineEdit_label->setMaxLength(30);
    ui->widget_input->hide();

    ui->horizontalLayout_3->removeWidget(ui->lineEdit_label);
    ui->lineEdit_label->setParent(ui->widget_grid);
    ui->lineEdit_label->setFixedHeight(24);
    ui->widget_grid->setMaximumHeight(QWIDGETSIZE_MAX);
    ui->widget_group->setMaximumHeight(QWIDGETSIZE_MAX);
    ui->widget->setMaximumHeight(QWIDGETSIZE_MAX);
    ui->widget_label_list->setMaximumHeight(QWIDGETSIZE_MAX);

    m_tip_data = { "同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                   "父与子学Python","nodejs开发指南","go 语言开发指南",
                   "游戏伙伴","金融投资","微信读书","拼多多拼友" };

    connect(ui->label_more, &ClickedOnceLabel::clicked, this, &ApplyFriend::slot_show_more_label);
    connect(ui->lineEdit_label, &CustomizeEdit::returnPressed, this, &ApplyFriend::slot_label_enter);
    connect(ui->lineEdit_label, &CustomizeEdit::sig_live_text_changed, this, &ApplyFriend::slot_label_text_change);
    connect(ui->lineEdit_label, &CustomizeEdit::editingFinished, this, &ApplyFriend::slot_label_edit_finished);
    connect(ui->label_tip, &ClickedOnceLabel::clicked, this, &ApplyFriend::slot_add_firend_label_by_click_tip);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->widget_grid->installEventFilter(this);
    ui->widget_label_list->installEventFilter(this);
    ui->lineEdit_label->installEventFilter(this);

    auto* closeBtn = new QPushButton(this);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setText(QStringLiteral("×"));
    closeBtn->setFixedSize(28, 28);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->move(width() - closeBtn->width() - 14, 12);
    closeBtn->raise();
    //关闭按钮不能是默认按钮，否则回车会触发它导致对话框直接退出
    closeBtn->setAutoDefault(false);
    closeBtn->setDefault(false);
    connect(closeBtn, &QPushButton::clicked, this, &ApplyFriend::slot_apply_cancel);

    ui->pushButton_sure->setState("normal","hover","press");
    ui->pushButton_cancel->setState("normal","hover","press");
    ui->pushButton_sure->setAutoDefault(false);
    ui->pushButton_sure->setDefault(false);
    ui->pushButton_cancel->setAutoDefault(false);
    ui->pushButton_cancel->setDefault(false);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &ApplyFriend::slot_apply_cancel);
    connect(ui->pushButton_sure, &QPushButton::clicked, this, &ApplyFriend::slot_apply_sure);

    initTipLbs();
    QTimer::singleShot(0, this, [this]() {
        relayoutFriendLabels();
        relayoutTipLabels(2);
        refreshLabelArea();
    });
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

QSize ApplyFriend::measureTipSize(const QString& text) const
{
    QFontMetrics fm(QFont("Microsoft YaHei", 13));
    const int textWidth = fm.horizontalAdvance(text);
    const int textHeight = fm.height();
    return QSize(textWidth + TIP_LABEL_PAD_H, textHeight + TIP_LABEL_PAD_V);
}

ClickedLabel* ApplyFriend::createTipLabel(const QString& text)
{
    auto* lb = new ClickedLabel(ui->widget_label_list);
    lb->setState("normal", "hover", "pressed", "selected_normal",
                 "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setFont(QFont("Microsoft YaHei", 13));
    lb->setAlignment(Qt::AlignCenter);
    lb->setText(text);
    lb->setFixedSize(measureTipSize(text));
    connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::slot_change_friend_label_by_tip);
    return lb;
}

void ApplyFriend::relayoutTipLabels(int maxLines)
{
    const int containerWidth = ui->widget_label_list->width();
    QPoint cursor(tip_offset, tip_offset);
    int rowHeight = 0;
    int lines = 1;
    int maxBottom = tip_offset;

    for (const auto& key : m_add_label_keys) {
        auto* lb = m_add_labels.value(key, nullptr);
        if (!lb) {
            continue;
        }

        const int labelWidth = lb->width();
        const int labelHeight = lb->height();

        if (cursor.x() > tip_offset && cursor.x() + labelWidth + tip_offset > containerWidth) {
            ++lines;
            cursor.setX(tip_offset);
            cursor.setY(cursor.y() + rowHeight + TIP_V_GAP);
            rowHeight = 0;
        }

        if (maxLines > 0 && lines > maxLines) {
            lb->hide();
            continue;
        }

        lb->move(cursor);
        lb->show();
        rowHeight = qMax(rowHeight, labelHeight);
        maxBottom = qMax(maxBottom, cursor.y() + labelHeight);
        cursor.setX(cursor.x() + labelWidth + TIP_H_GAP);
    }

    const int contentHeight = qMax(kTipListMinHeight, maxBottom + tip_offset);
    ui->widget_label_list->setFixedHeight(contentHeight);
    ui->widget->setFixedHeight(contentHeight);
    ui->widget->updateGeometry();
}

void ApplyFriend::relayoutFriendLabels()
{
    const int containerWidth = ui->widget_grid->width();
    const int leftPadding = kFriendTagAreaPadding;
    const int rightLimit = qMax(leftPadding, containerWidth - kFriendTagAreaPadding);
    const int fullRowWidth = qMax(MIN_APPLY_LINEEDIT_LABEL_LEN, rightLimit - leftPadding);
    QPoint cursor(leftPadding, kFriendTagAreaPadding);
    int rowHeight = 0;

    for (const auto& key : m_friend_label_keys) {
        auto* label = m_friend_labels.value(key, nullptr);
        if (!label) {
            continue;
        }

        const QSize labelSize = label->size();
        const int labelWidth = labelSize.width();
        const int labelHeight = labelSize.height();
        if (cursor.x() > leftPadding && cursor.x() + labelWidth > rightLimit) {
            cursor.setX(leftPadding);
            cursor.setY(cursor.y() + rowHeight + kFriendTagVerticalGap);
            rowHeight = 0;
        }

        label->move(cursor);
        label->show();
        rowHeight = qMax(rowHeight, labelHeight);
        cursor.setX(cursor.x() + labelWidth + kFriendTagHorizontalGap);
    }

    //输入框固定独占一整行，永远排在所有标签下方，保证不会被标签挤没
    int inputY = kFriendTagAreaPadding;
    if (!m_friend_label_keys.empty()) {
        inputY = cursor.y() + rowHeight + kFriendTagVerticalGap;
    }

    ui->lineEdit_label->setFixedWidth(fullRowWidth);
    ui->lineEdit_label->move(leftPadding, inputY);
    ui->lineEdit_label->raise();
    ui->lineEdit_label->show();

    //高度全部基于本地计算值，避免读取尚未生效的旧几何尺寸
    const int gridHeight = qMax(40, inputY + ui->lineEdit_label->height() + kFriendTagAreaPadding);
    ui->widget_grid->setFixedHeight(gridHeight);
    const int inputTipHeight = ui->widget_input->isVisible() ? ui->widget_input->height() : 0;
    ui->widget_group->setFixedHeight(gridHeight + inputTipHeight);
}

void ApplyFriend::addLabel(QString name)
{
    name = normalizedLabelText(name);
    if (name.isEmpty() || m_friend_labels.contains(name)) {
        return;
    }

    auto* label = new FriendLabel(ui->widget_grid);
    label->setText(name);
    connect(label, &FriendLabel::sig_close, this, &ApplyFriend::slot_remove_friend_label);
    connect(label, &FriendLabel::sig_clicked, this, [this]() {
        ui->lineEdit_label->setFocus();
        ui->lineEdit_label->setCursorPosition(ui->lineEdit_label->text().size());
    });

    m_friend_labels.insert(name, label);
    m_friend_label_keys.push_back(name);
    relayoutFriendLabels();
    refreshLabelArea();
}

void ApplyFriend::addTipLabelToList(const QString& text, bool selected)
{
    const QString value = normalizedLabelText(text);
    if (value.isEmpty()) {
        return;
    }

    if (std::find(m_tip_data.begin(), m_tip_data.end(), value) == m_tip_data.end()) {
        m_tip_data.push_back(value);
    }

    auto it = m_add_labels.find(value);
    if (it == m_add_labels.end()) {
        auto* lb = createTipLabel(value);
        m_add_labels.insert(value, lb);
        m_add_label_keys.push_back(value);
    }

    syncTipLabelState(value, selected);
    relayoutTipLabels(m_tip_expanded ? -1 : 2);
    refreshLabelArea();
}

void ApplyFriend::clearLabelInput()
{
    m_live_label_text.clear();
    ui->lineEdit_label->clear();
    ui->label_tip->clear();
    ui->widget_input->hide();
    relayoutFriendLabels();
    refreshLabelArea();
    ensureInputVisible();
    ui->lineEdit_label->setFocus();
}

QString ApplyFriend::normalizedLabelText(const QString& text) const
{
    return text.simplified();
}

void ApplyFriend::syncTipLabelState(const QString& text, bool selected)
{
    auto it = m_add_labels.find(text);
    if (it == m_add_labels.end()) {
        return;
    }

    if (selected) {
        it.value()->setCurState(ClickLbState::Selected);
        return;
    }

    it.value()->resetNormalState();
}

void ApplyFriend::updateInputTip(const QString& text)
{
    const QString value = normalizedLabelText(text);
    if (value.isEmpty()) {
        ui->label_tip->clear();
        ui->widget_input->hide();
        relayoutFriendLabels();
        refreshLabelArea();
        ensureInputVisible();
        return;
    }

    ui->label_tip->setText(QString("%1：%2").arg(add_prefix, value));
    ui->widget_input->show();
    relayoutFriendLabels();
    refreshLabelArea();
    ensureInputVisible();
}

void ApplyFriend::refreshLabelArea()
{
    if (ui->verticalLayout_2) {
        ui->verticalLayout_2->invalidate();
        ui->verticalLayout_2->activate();
    }
    ui->scrollAreaWidgetContents->adjustSize();
    const int contentHeight = ui->verticalLayout_2->sizeHint().height();
    ui->scrollAreaWidgetContents->setMinimumHeight(contentHeight);
    ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->width(), contentHeight);
}

void ApplyFriend::ensureInputVisible()
{
    QTimer::singleShot(0, this, [this]() {
        QWidget* target = ui->widget_input->isVisible()
                              ? static_cast<QWidget*>(ui->widget_input)
                              : static_cast<QWidget*>(ui->lineEdit_label);
        ui->scrollArea->ensureWidgetVisible(target, 0, 24);
    });
}

void ApplyFriend::initTipLbs()
{
    for (const auto& text : m_tip_data) {
        if (m_add_labels.contains(text)) {
            continue;
        }
        auto* lb = createTipLabel(text);
        m_add_labels.insert(text, lb);
        m_add_label_keys.push_back(text);
    }
    relayoutTipLabels(2);
}

void ApplyFriend::keyPressEvent(QKeyEvent *event)
{
    //回车键统一兜底：聚焦在标签输入框时添加标签，其余情况一律吞掉，绝不触发关闭
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (ui->lineEdit_label->hasFocus()) {
            slot_label_enter();
        }
        event->accept();
        return;
    }
    QDialog::keyPressEvent(event);
}

bool ApplyFriend::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->scrollArea && event->type() == QEvent::Enter) {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    } else if (obj == ui->scrollArea && event->type() == QEvent::Leave) {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    } else if (obj == ui->lineEdit_label && event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            slot_label_enter();
            return true;
        }
    } else if (obj == ui->widget_grid && event->type() == QEvent::MouseButtonPress) {
        ui->lineEdit_label->setFocus();
        ui->lineEdit_label->setCursorPosition(ui->lineEdit_label->text().size());
        ensureInputVisible();
        return true;
    } else if (obj == ui->widget_grid && event->type() == QEvent::Resize) {
        relayoutFriendLabels();
    } else if (obj == ui->widget_label_list && event->type() == QEvent::Resize) {
        relayoutTipLabels(m_tip_expanded ? -1 : 2);
    }
    return QObject::eventFilter(obj, event);
}

void ApplyFriend::setSearchInfo(std::shared_ptr<SearchInfo> si)
{
    m_si = si;
    ui->lineEdit_apply_name->setText(UserManager::getInstance()->getName());
    if (si) {
        ui->lineEdit_notes->setText(si->m_name);
    }
}

void ApplyFriend::slot_show_more_label()
{
    if (m_tip_expanded) {
        return;
    }

    m_tip_expanded = true;
    ui->widget_more_label->hide();
    ui->widget_label_list->setFixedWidth(325);
    relayoutTipLabels(-1);
    refreshLabelArea();
}

void ApplyFriend::slot_label_enter()
{
    const QString text = normalizedLabelText(ui->lineEdit_label->text());
    if (text.isEmpty()) {
        return;
    }

    addLabel(text);
    addTipLabelToList(text, true);
    clearLabelInput();
}

void ApplyFriend::slot_remove_friend_label(QString name)
{
    auto it = m_friend_labels.find(name);
    if (it == m_friend_labels.end()) {
        return;
    }

    auto keyIt = std::find(m_friend_label_keys.begin(), m_friend_label_keys.end(), name);
    if (keyIt != m_friend_label_keys.end()) {
        m_friend_label_keys.erase(keyIt);
    }

    auto* label = it.value();
    m_friend_labels.erase(it);
    if (label) {
        label->hide();
        label->deleteLater();
    }

    relayoutFriendLabels();
    syncTipLabelState(name, false);
    refreshLabelArea();
}

void ApplyFriend::slot_change_friend_label_by_tip(QString lbtext, ClickLbState state)
{
    if (!m_add_labels.contains(lbtext)) {
        return;
    }

    if (state == ClickLbState::Selected) {
        addLabel(lbtext);
        return;
    }

    slot_remove_friend_label(lbtext);
}

void ApplyFriend::slot_label_text_change(const QString& text)
{
    m_live_label_text = text;
    updateInputTip(text);
}

void ApplyFriend::slot_label_edit_finished()
{
    QTimer::singleShot(100, this, [this]() {
        auto* focusWidget = QApplication::focusWidget();
        if (focusWidget == ui->label_tip || ui->widget_input->isAncestorOf(focusWidget)) {
            return;
        }
        ui->widget_input->hide();
        relayoutFriendLabels();
        refreshLabelArea();
    });
}

void ApplyFriend::slot_add_firend_label_by_click_tip(QString text)
{
    Q_UNUSED(text);
    slot_label_enter();
}

void ApplyFriend::slot_apply_cancel()
{
    hide();
    deleteLater();
}

void ApplyFriend::slot_apply_sure()
{
    hide();
    deleteLater();
}


