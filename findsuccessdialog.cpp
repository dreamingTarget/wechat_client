#include "findsuccessdialog.h"
// #include "applyfriend.h"
#include "applyfriend.h"
#include "ui_findsuccessdialog.h"

#include <QDir>

FindSuccessDialog::FindSuccessDialog(QWidget *parent)
    : QDialog(parent)
    // , m_parent(parent)
    , ui(new Ui::FindSuccessDialog)
{
    ui->setupUi(this);

    //???
    m_parent = new QWidget(this);

    // 设置对话框标题
    setWindowTitle("添加");
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // 在构造函数中，设置完窗口标志后添加
    QPushButton *closeBtn = new QPushButton(this);
    closeBtn->setText("×");                        // 叉号
    closeBtn->setFixedSize(30, 30);               // 固定大小
    closeBtn->setObjectName("closeBtn");
    // 将按钮置于右上角（假设对话框宽 400，高 300，可动态调整）
    closeBtn->move(width() - 30 - 10, 10);        // 距右边 10px，上边 10px
    // 连接信号槽
    connect(closeBtn, &QPushButton::clicked, this, &QWidget::close);

    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path +
                                                QDir::separator() + "static"+QDir::separator()+"head_1.jpg");
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->label_head->size(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_head->setPixmap(head_pix);
    ui->pushButton_add_friend->setState("normal","hover","press");
    this->setModal(true);
}

FindSuccessDialog::~FindSuccessDialog()
{
    delete ui;
}

void FindSuccessDialog::setSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->label_name->setText(si->m_name);
    m_si = si;
}

void FindSuccessDialog::on_pushButton_add_friend_clicked()
{
    hide();
    auto applyFriend = new ApplyFriend(m_parent);
    applyFriend->setSearchInfo(m_si);
    applyFriend->setModal(true);
    applyFriend->show();
}

