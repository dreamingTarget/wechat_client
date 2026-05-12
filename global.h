#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QStyle>

extern std::function<void(QWidget*)> repolish;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001,
    ID_REG_USER = 1002,
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER = 1004
};

enum Modules {
    REGISTERMOD = 0
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,
    ERR_NETWORK = 2,
    ERR_UNKNOWN = 3
};

#endif // GLOBAL_H
