#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QMap>
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
    void appendApplyList(QJsonArray array);
    void addApplyList(std::shared_ptr<ApplyInfo> app);
    bool alreadyApply(int uid);
    void setUserInfo(std::shared_ptr<UserInfo> user_info);

    int getUid();
    QString getName();
    std::vector<std::shared_ptr<ApplyInfo>> getApplyList();

    QString icon() const;
    void setIcon(const QString &newIcon);

    bool checkFriendById(int uid);
    void addFriend(std::shared_ptr<AuthRsp> auth_rsp);
    void addFriend(std::shared_ptr<AuthInfo> auth_info);
    std::shared_ptr<FriendInfo> getFriendById(int uid);
    void appendFriendList(QJsonArray array);

    std::vector<std::shared_ptr<FriendInfo>> getChatListPerPage();
    bool isLoadChatFin();
    void updateChatLoadedCount();
    std::vector<std::shared_ptr<FriendInfo>> getConListPerPage();
    void updateContactLoadedCount();
    bool isLoadConFin();

private:
    UserManager();

signals:

public slots:
    void slot_add_friend_rsp(std::shared_ptr<AuthRsp> rsp);
    void slot_add_friend_auth(std::shared_ptr<AuthInfo> auth);

private:
    QString m_name;
    int m_uid;
    QString m_token;
    QString m_icon;
    std::vector<std::shared_ptr<ApplyInfo>> m_apply_list;
    std::shared_ptr<UserInfo> m_user_info;
    QMap<int, std::shared_ptr<FriendInfo>> m_friend_map;
    std::vector<std::shared_ptr<FriendInfo>> m_friend_list;
    int m_chat_loaded;
    int m_contact_loaded;

};

#endif // USERMANAGER_H
