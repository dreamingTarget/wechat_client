#include "tcpmgr.h"
#include "usermanager.h"
#include <QJsonArray>

UserManager::~UserManager()
{

}

void UserManager::setName(QString name)
{
    m_name = name;
}

void UserManager::setUid(int uid)
{
    m_uid = uid;
}

void UserManager::setToken(QString token)
{
    m_token = token;
}

void UserManager::appendApplyList(QJsonArray array)
{
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue &value : array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto status = value["status"].toInt();
        auto info = std::make_shared<ApplyInfo>(uid, name,
                                                desc, icon, nick, sex, status);
        m_apply_list.push_back(info);
    }
}

void UserManager::addApplyList(std::shared_ptr<ApplyInfo> app)
{
    m_apply_list.push_back(app);
}

bool UserManager::alreadyApply(int uid)
{
    for(auto& apply: m_apply_list){
        if(apply->m_uid == uid){
            return true;
        }
    }

    return false;
}

void UserManager::setUserInfo(std::shared_ptr<UserInfo> user_info)
{
    m_user_info = user_info;
}

int UserManager::getUid()
{
    // return m_uid;
    //???
    return m_user_info->m_uid;
}

QString UserManager::getName()
{
    // return m_name;
    //???
    return m_user_info->m_name;
}

std::vector<std::shared_ptr<ApplyInfo> > UserManager::getApplyList()
{
    return m_apply_list;
}

UserManager::UserManager() : m_user_info(nullptr), m_chat_loaded(0), m_contact_loaded(0)
{
    // connect(TcpMgr::getInstance().get(), &TcpMgr::sig_auth_rsp, this, &UserManager::slot_add_friend_rsp);
    // connect(TcpMgr::getInstance().get(), &TcpMgr::sig_add_auth_friend, this, &UserManager::slot_add_friend_auth);

}

void UserManager::slot_add_friend_rsp(std::shared_ptr<AuthRsp> rsp)
{
    addFriend(rsp);
}

void UserManager::slot_add_friend_auth(std::shared_ptr<AuthInfo> auth)
{
    addFriend(auth);
}

QString UserManager::icon() const
{
    return m_icon;
}

void UserManager::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
}

bool UserManager::checkFriendById(int uid)
{
    auto iter = m_friend_map.find(uid);
    if(iter == m_friend_map.end()){
        return false;
    }

    return true;
}

void UserManager::addFriend(std::shared_ptr<AuthRsp> auth_rsp)
{
    auto friend_info = std::make_shared<FriendInfo>(auth_rsp);
    m_friend_map[friend_info->m_uid] = friend_info;
}

void UserManager::addFriend(std::shared_ptr<AuthInfo> auth_info)
{
    auto friend_info = std::make_shared<FriendInfo>(auth_info);
    m_friend_map[friend_info->m_uid] = friend_info;
}

std::shared_ptr<FriendInfo> UserManager::getFriendById(int uid)
{
    auto find_it = m_friend_map.find(uid);
    if(find_it == m_friend_map.end()){
        return nullptr;
    }

    return *find_it;
}

void UserManager::appendFriendList(QJsonArray array)
{
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue& value : array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto back = value["back"].toString();

        auto info = std::make_shared<FriendInfo>(uid, name,
                                                 nick, icon, sex, desc, back);
        m_friend_list.push_back(info);
        m_friend_map.insert(uid, info);
    }
}

std::vector<std::shared_ptr<FriendInfo> > UserManager::getChatListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = m_chat_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= m_friend_list.size()) {
        return friend_list;
    }

    if (end > m_friend_list.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(m_friend_list.begin() + begin, m_friend_list.end());
        return friend_list;
    }


    friend_list = std::vector<std::shared_ptr<FriendInfo>>(m_friend_list.begin() + begin, m_friend_list.begin()+ end);
    return friend_list;
}

bool UserManager::isLoadChatFin()
{
    if (m_contact_loaded >= m_friend_list.size()) {
        return true;
    }

    return false;
}

void UserManager::updateChatLoadedCount()
{
    int begin = m_chat_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= m_friend_list.size()) {
        return ;
    }

    if (end > m_friend_list.size()) {
        m_chat_loaded = m_friend_list.size();
        return ;
    }

    m_chat_loaded = end;
}

std::vector<std::shared_ptr<FriendInfo> > UserManager::getConListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = m_contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= m_friend_list.size()) {
        return friend_list;
    }

    if (end > m_friend_list.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(m_friend_list.begin() + begin, m_friend_list.end());
        return friend_list;
    }


    friend_list = std::vector<std::shared_ptr<FriendInfo>>(m_friend_list.begin() + begin, m_friend_list.begin() + end);
    return friend_list;
}

void UserManager::updateContactLoadedCount()
{
    int begin = m_contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= m_friend_list.size()) {
        return;
    }

    if (end > m_friend_list.size()) {
        m_contact_loaded = m_friend_list.size();
        return;
    }

    m_contact_loaded = end;
}

bool UserManager::isLoadConFin()
{
    if (m_contact_loaded >= m_friend_list.size()) {
        return true;
    }

    return false;
}

