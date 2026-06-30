#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QStyle>

extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;
extern QString gate_url_prefix;
extern std::vector<QString>  strs;
extern std::vector<QString> heads;
extern std::vector<QString> names;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001,
    ID_REG_USER = 1002,
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER = 1004,
    ID_CHAT_LOGIN = 1005,
    ID_CHAT_LOGIN_RES = 1006,
};

enum Modules {
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,
    ERR_NETWORK = 2,
    ERR_UNKNOWN = 3
};

enum TipErr {
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};

enum ChatUIMode {
    SEARCH_MODE,
    CHAT_MODE,
    CONTACT_MODE,
};

enum ListItemType {
    CHAT_USER_ITEM,
    CONTACT_USER_ITEM,
    SEARCH_USER_ITEM,
    ADD_USER_TIP_ITEM,
    INVALID_ITEM,
    GROUP_TIP_ITEM,
};

struct ServerInfo {
    QString host;
    QString port;
    QString token;
    int uid;
};

#endif // GLOBAL_H
