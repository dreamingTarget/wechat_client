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
    return m_uid;
}

QString UserManager::getName()
{
    return m_name;
}

std::vector<std::shared_ptr<ApplyInfo> > UserManager::getApplyList()
{
    return m_apply_list;
}

UserManager::UserManager()
{

}

QString UserManager::icon() const
{
    return m_icon;
}

void UserManager::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
}
