#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
// #include <memory>
#include "singleton.h"

class UserManager : public QObject, public Singleton<UserManager>
{
    Q_OBJECT
    friend class Singleton<UserManager>;

public:    
    ~UserManager();
    void setName(QString name);
    void setUid(int uid);
    void setToken(QString token);

    int getUid();
    QString getName();

private:
    UserManager();

signals:

private:
    QString m_name;
    int m_uid;
    QString m_token;

};

#endif // USERMANAGER_H
