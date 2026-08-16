#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
// #include <memory>
#include "singleton.h"
#include "userdata.h"
#include <vector>

class UserManager : public QObject, public Singleton<UserManager>
{
    Q_OBJECT
    friend class Singleton<UserManager>;

public:    
    ~UserManager();
    void setName(QString name);
    void setUid(int uid);
    void setToken(QString token);
    // void AppendApplyList(QJsonArray array);

    int getUid();
    QString getName();
    std::vector<std::shared_ptr<ApplyInfo>> getApplyList();

    QString icon() const;
    void setIcon(const QString &newIcon);

private:
    UserManager();

signals:

private:
    QString m_name;
    int m_uid;
    QString m_token;
    QString m_icon;
    std::vector<std::shared_ptr<ApplyInfo>> m_apply_list;

};

#endif // USERMANAGER_H
