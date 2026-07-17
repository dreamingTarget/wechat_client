#include "picturebubble.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QLabel>
#include <QApplication>

#define PIC_MAX_WIDTH 160
#define PIC_MAX_HEIGHT 90

PictureBubble::PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent)
    : BubbleFrame(role, parent)
{
    lb = new QLabel();
    // 不要 setScaledContents(true)，让QLabel固定大小，pixmap按固定尺寸缩放后设置

    // 计算目标尺寸（保持宽高比，且不超过最大尺寸，同时不放大原图）
    QSize originalSize = picture.size();
    QSize targetSize = originalSize;
    if (targetSize.width() > PIC_MAX_WIDTH || targetSize.height() > PIC_MAX_HEIGHT) {
        targetSize.scale(PIC_MAX_WIDTH, PIC_MAX_HEIGHT, Qt::KeepAspectRatio);
    }
    // 如果原图小于最大尺寸，就保持原尺寸，避免放大模糊

    // 使用平滑变换缩放
    QPixmap scaledPix = picture.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lb->setPixmap(scaledPix);
    lb->setFixedSize(targetSize);   // 固定大小与pixmap一致，防止拉伸

    this->setWidget(lb);

    // 计算气泡整体尺寸（加上边距）
    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int v_margin = this->layout()->contentsMargins().bottom();
    setFixedSize(targetSize.width() + left_margin + right_margin,
                 targetSize.height() + v_margin * 2);
}

void PictureBubble::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
    }
    BubbleFrame::mousePressEvent(event); // 保持原有行为（如点击选中）
}

void PictureBubble::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance())
        return;

    // 获取当前显示的图片
    QLabel *label = qobject_cast<QLabel*>(lb);
    if (!label) return;
    QPixmap pix = label->pixmap(Qt::ReturnByValue);
    if (pix.isNull()) return;

    // 构造拖拽数据
    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData(pix);   // 图片数据

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    QPixmap preview = pix.scaled(160, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    drag->setPixmap(preview);

    drag->exec(Qt::CopyAction);
}

