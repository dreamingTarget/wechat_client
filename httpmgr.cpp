#include "httpmgr.h"
#include <QUrl>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::signal_http_finish, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::postHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    //QJsonObject json;json["username"] = "alice";json["password"] = "secret123";json["remember"] = true;
    //data{"password":"secret123","remember":true,"username":"alice"}
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    QNetworkReply* reply = m_manager.post(request, data);
    auto self = shared_from_this();
    connect(reply, &QNetworkReply::finished, this, [self, reply, req_id, mod]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "error: " << reply->errorString();
            emit self->signal_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }
        QByteArray response_data = reply->readAll();
        emit self->signal_http_finish(req_id, QString(response_data), ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
    });

}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if (mod == Modules::REGISTERMOD) {
        emit signal_reg_mod_finish(id, res, err);
    }
}
