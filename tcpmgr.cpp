#include "tcpmgr.h"
#include "usermanager.h"
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonObject>
// #include "usermanager.h"

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr() : m_host(""), m_port(0), m_recv_pending(false), m_message_id(0),
    m_message_len(0)
{
    connect(&m_socket, &QTcpSocket::connected, this, [&](){
        qDebug() << "connect to server";
        //连接建立后发送消息
        emit sig_conn_success(true);
    });

    connect(&m_socket, &QTcpSocket::readyRead, this, [&](){
        //当有数据可读时，读取所有数据
        //读取所有数据并追加到缓冲区
        m_buffer.append(m_socket.readAll());
        // stream.setByteOrder(QDataStream::LittleEndian);

        forever {
            QDataStream stream(&m_buffer, QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            //先解析头部
            if (!m_recv_pending) {
                //检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
                if (m_buffer.size() < static_cast<int>(sizeof(uint16_t) * 2)) {
                    return;//数据不够，等待更多数据
                }
                //预读取消息ID和消息长度，但不从缓冲区中移除
                stream >> m_message_id >> m_message_len;
                //将buffer 中的前四个字节移除
                m_buffer = m_buffer.mid(sizeof(uint16_t) * 2);
                //输出读取的数据
                qDebug() << "message id: " << m_message_id << ", message len: " << m_message_len;

            }
            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if (m_buffer.size() < m_message_len) {
                m_recv_pending = true;
                return;
            }
            m_recv_pending = false;
            //读取消息体
            QByteArray messagebody = m_buffer.mid(0, m_message_len);
            qDebug() << "receive body msg is " << messagebody;
            m_buffer = m_buffer.mid(m_message_len);
            handleMsg(ReqId(m_message_id), m_message_len, messagebody);
        }
    });

    //5.15 之后版本
    //       QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
    //           Q_UNUSED(socketError)
    //           qDebug() << "Error:" << _socket.errorString();
    //       });
    // 处理错误（适用于Qt 5.15之前的版本）
    QObject::connect(&m_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error), this,
                     [&](QTcpSocket::SocketError socketError) {
                         qDebug() << "Error:" << m_socket.errorString() ;
                         switch (socketError) {
                         case QTcpSocket::ConnectionRefusedError:
                             qDebug() << "Connection Refused!";
                             emit sig_conn_success(false);
                             break;
                         case QTcpSocket::RemoteHostClosedError:
                             qDebug() << "Remote Host Closed Connection!";
                             break;
                         case QTcpSocket::HostNotFoundError:
                             qDebug() << "Host Not Found!";
                             emit sig_conn_success(false);
                             break;
                         case QTcpSocket::SocketTimeoutError:
                             qDebug() << "Connection Timeout!";
                             emit sig_conn_success(false);
                             break;
                         case QTcpSocket::NetworkError:
                             qDebug() << "Network Error!";
                             break;
                         default:
                             qDebug() << "Other Error!";
                             break;
                         }
                     });
    // 处理连接断开
    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });

    //连接发送信号用来发送数据
    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    //注册消息
    initHandlers();
}

void TcpMgr::initHandlers()
{
    m_handlers.insert(ID_CHAT_LOGIN_RES, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle is: " << id << " data is: " << data;
        //将QByteArray转换为QJsonDocument
        QJsonDocument doc = QJsonDocument::fromJson(data);
        //bytearry -> doc
        if (doc.isNull()) {
            qDebug() << "failed to create doc";
            return;
        }
        QJsonObject obj = doc.object();
        if (!obj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "login failed, err is json parse err: " << err;
            emit sig_login_failed(err);
            return;
        }

        int err = obj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "login failed, err is " << err;
            emit sig_login_failed(err);
            return;
        }

        //UserManager -->
        UserManager::getInstance()->setName(obj["name"].toString());
        UserManager::getInstance()->setUid(obj["uid"].toInt());
        UserManager::getInstance()->setToken(obj["token"].toString());
        emit sig_switch_chatdlg();
        qDebug() << "sig_switch_chatdlg()";
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto it = m_handlers.find(id);
    if (it == m_handlers.end()) {
        qDebug() << "not found id [" << id << "] to handle";
        return;
    }
    it.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo info)
{
    qDebug() << "receive tcp connect signal";
    //尝试连接到服务器
    qDebug() << "connecting to server......";
    m_host = info.host;
    m_port = static_cast<uint16_t>(info.port.toUShort());
    m_socket.connectToHost(info.host, m_port);
}

void TcpMgr::slot_send_data(ReqId reqid, QString data)
{
    uint16_t id = reqid;
    //将字符串转换为UTF-8编码的字节数组
    QByteArray databytes = data.toUtf8();
    //计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(databytes.size());
    //创建一个QByteArray用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);
    //写入ID和长度
    out << id << len;
    //添加字符串数据
    block.append(databytes);
    //发送数据
    m_socket.write(block);

}
