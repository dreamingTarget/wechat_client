#include "usermanager.h"

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
