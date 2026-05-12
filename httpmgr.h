#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include "singleton.h"
#include <QNetworkAccessManager>
#include "global.h"

class HttpMgr : public QObject, public Singleton<HttpMgr>,
public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
    friend class Singleton<HttpMgr>;

public:
    ~HttpMgr();

private:
    HttpMgr();
    void postHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

signals:
    void signal_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void signal_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

private:
    QNetworkAccessManager m_manager;

};

#endif // HTTPMGR_H
