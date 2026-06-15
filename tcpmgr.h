#ifndef TCPMGR_H
#define TCPMGR_H

#include <QObject>
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"

class TcpMgr : public QObject, public Singleton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
    friend class Singleton<TcpMgr>;
public:
    ~TcpMgr();

private:
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);

signals:
    void sig_conn_success(bool);
    void sig_send_data(ReqId reqid, QString data);
    void sig_switch_chatdlg();
    void sig_login_failed(int);

    // void sig_user_search(std::shared_ptr<SearchInfo> si);

public slots:
    void slot_tcp_connect(ServerInfo info);
    void slot_send_data(ReqId reqid, QString data);

private:
    QTcpSocket m_socket;
    QString m_host;
    uint16_t m_port;
    QByteArray m_buffer;
    bool m_recv_pending;
    quint16 m_message_id;
    quint16 m_message_len;
    QMap<ReqId, std::function<void(ReqId, int, QByteArray)>> m_handlers;
};

#endif // TCPMGR_H
