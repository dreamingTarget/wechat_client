#include "MessageTextEdit.h"
#include <QDebug>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QImageWriter>

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : QTextEdit(parent)
{

    //this->setStyleSheet("border: none;");
    this->setMaximumHeight(60);

    //    connect(this,SIGNAL(textChanged()),this,SLOT(textEditChanged()));

}

MessageTextEdit::~MessageTextEdit()
{

}

QVector<MsgInfo> MessageTextEdit::getMsgList()
{
    mGetMsgList.clear();

    QString doc = this->document()->toPlainText();
    QString text="";//存储文本信息
    int indexUrl = 0;
    int count = mMsgList.size();

    for(int index=0; index<doc.size(); index++)
    {
        if(doc[index]==QChar::ObjectReplacementCharacter)
        {
            if(!text.isEmpty())
            {
                QPixmap pix;
                insertMsgList(mGetMsgList,"text",text,pix);
                text.clear();
            }
            while(indexUrl<count)
            {
                MsgInfo msg =  mMsgList[indexUrl];
                if(this->document()->toHtml().contains(msg.content,Qt::CaseSensitive))
                {
                    indexUrl++;
                    mGetMsgList.append(msg);
                    break;
                }
                indexUrl++;
            }
        }
        else
        {
            text.append(doc[index]);
        }
    }
    if(!text.isEmpty())
    {
        QPixmap pix;
        insertMsgList(mGetMsgList,"text",text,pix);
        text.clear();
    }
    mMsgList.clear();
    this->clear();
    return mGetMsgList;
}

void MessageTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == this) {
        event->ignore();
        return;
    }
    const QMimeData *mime = event->mimeData();
    // 检查是否包含可接受的数据类型：图片、文件URL、纯文本
    if (mime->hasImage() || mime->hasUrls() || mime->hasText()) {
        event->acceptProposedAction();   // 关键：接受拖放动作
    } else {
        event->ignore();
    }
}

void MessageTextEdit::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->source() == this) {
        event->ignore();
        return;
    }
    const QMimeData *mime = event->mimeData();
    if (mime->hasImage() || mime->hasUrls() || mime->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MessageTextEdit::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        event->ignore();
        return;
    }
    // 调用插入逻辑
    insertFromMimeData(event->mimeData());
    event->acceptProposedAction(); // 明确接受放下
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    if((e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return)&& !(e->modifiers() & Qt::ShiftModifier))
    {
        emit send();
        return;
    }
    QTextEdit::keyPressEvent(e);
}

void MessageTextEdit::insertFileFromUrl(const QStringList &urls)
{
    if(urls.isEmpty())
        return;

    foreach (QString url, urls){
        if(isImage(url))
            insertImages(url);
        else
            insertTextFile(url);
    }
}

void MessageTextEdit::insertImages(const QString &url)
{
    QImage image(url);
    // 使用更大的阈值
    const int maxWidth = 200;   // 可根据需要调整
    const int maxHeight = 150;
    if (image.width() > maxWidth || image.height() > maxHeight) {
        image = image.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QTextCursor cursor = this->textCursor();
    // QTextDocument *document = this->document();
    // document->addResource(QTextDocument::ImageResource, QUrl(url), QVariant(image));
    cursor.insertImage(image,url);

    insertMsgList(mMsgList,"image",url,QPixmap::fromImage(image));
}

void MessageTextEdit::insertTextFile(const QString &url)
{
    QFileInfo fileInfo(url);
    if(fileInfo.isDir())
    {
        QMessageBox::information(this,"提示","只允许拖拽单个文件!");
        return;
    }

    if(fileInfo.size()>100*1024*1024)
    {
        QMessageBox::information(this,"提示","发送的文件大小不能大于100M");
        return;
    }

    QPixmap pix = getFileIconPixmap(url);
    QTextCursor cursor = this->textCursor();
    cursor.insertImage(pix.toImage(),url);
    insertMsgList(mMsgList,"file",url,pix);
}

bool MessageTextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    return QTextEdit::canInsertFromMimeData(source);
}

void MessageTextEdit::insertFromMimeData(const QMimeData *source)
{
    // 1. 处理图片数据（从图片气泡或其它程序拖拽的图片）
    if (source->hasImage()) {
        QImage image = qvariant_cast<QImage>(source->imageData());
        if (!image.isNull()) {
            // 保存为临时文件
            QTemporaryFile tempFile;
            tempFile.setAutoRemove(false); // 保留到发送后清理
            if (tempFile.open()) {
                QString filePath = tempFile.fileName() + ".png";
                if (image.save(filePath, "PNG")) {
                    insertImages(filePath); // 复用现有插入逻辑
                    // 可添加至临时文件列表，后续统一删除
                    return;
                }
            }
        }
    }

    // 2. 处理文件拖放（URL）
    if (source->hasUrls()) {
        QList<QUrl> urls = source->urls();
        QStringList filePaths;
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                filePaths.append(url.toLocalFile());
            }
        }
        if (!filePaths.isEmpty()) {
            insertFileFromUrl(filePaths); // 您已有的方法
            return;
        }
    }

    // 3. 其他类型（文本、HTML等）交给基类
    QTextEdit::insertFromMimeData(source);
}

bool MessageTextEdit::isImage(QString url)
{
    QString imageFormat = "bmp,jpg,png,tif,gif,pcx,tga,exif,fpx,svg,psd,cdr,pcd,dxf,ufo,eps,ai,raw,wmf,webp";
    QStringList imageFormatList = imageFormat.split(",");
    QFileInfo fileInfo(url);
    QString suffix = fileInfo.suffix();
    if(imageFormatList.contains(suffix,Qt::CaseInsensitive)){
        return true;
    }
    return false;
}

void MessageTextEdit::insertMsgList(QVector<MsgInfo> &list, QString flag, QString text, QPixmap pix)
{
    MsgInfo msg;
    msg.msgFlag=flag;
    msg.content = text;
    msg.pixmap = pix;
    list.append(msg);
}

QStringList MessageTextEdit::getUrl(QString text)
{
    QStringList urls;
    if(text.isEmpty()) return urls;

    QStringList list = text.split("\n");
    foreach (QString url, list) {
        if(!url.isEmpty()){
            QStringList str = url.split("///");
            if(str.size()>=2)
                urls.append(str.at(1));
        }
    }
    return urls;
}

QPixmap MessageTextEdit::getFileIconPixmap(const QString &url)
{
    QFileIconProvider provder;
    QFileInfo fileinfo(url);
    QIcon icon = provder.icon(fileinfo);

    QString strFileSize = getFileSize(fileinfo.size());
    //qDebug() << "FileSize=" << fileinfo.size();

    QFont font(QString("宋体"),10,QFont::Normal,false);
    QFontMetrics fontMetrics(font);
    QSize textSize = fontMetrics.size(Qt::TextSingleLine, fileinfo.fileName());

    QSize FileSize = fontMetrics.size(Qt::TextSingleLine, strFileSize);
    int maxWidth = textSize.width() > FileSize.width() ? textSize.width() :FileSize.width();
    QPixmap pix(50 + maxWidth + 10, 50);
    pix.fill();

    QPainter painter;
    // painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setFont(font);
    painter.begin(&pix);
    // 文件图标
    QRect rect(0, 0, 50, 50);
    painter.drawPixmap(rect, icon.pixmap(40,40));
    painter.setPen(Qt::black);
    // 文件名称
    QRect rectText(50+10, 3, textSize.width(), textSize.height());
    painter.drawText(rectText, fileinfo.fileName());
    // 文件大小
    QRect rectFile(50+10, textSize.height()+5, FileSize.width(), FileSize.height());
    painter.drawText(rectFile, strFileSize);
    painter.end();
    return pix;
}

QString MessageTextEdit::getFileSize(qint64 size)
{
    QString Unit;
    double num;
    if(size < 1024){
        num = size;
        Unit = "B";
    }
    else if(size < 1024 * 1024){
        num = size / 1024.0;
        Unit = "KB";
    }
    else if(size <  1024 * 1024 * 1024){
        num = size / 1024.0 / 1024.0;
        Unit = "MB";
    }
    else{
        num = size / 1024.0 / 1024.0/ 1024.0;
        Unit = "GB";
    }
    return QString::number(num,'f',2) + " " + Unit;
}

void MessageTextEdit::textEditChanged()
{
    //qDebug() << "text changed!" << endl;
}

