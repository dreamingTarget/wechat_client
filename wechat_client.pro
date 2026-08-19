QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    applyfrienditem.cpp \
    applyfriendlist.cpp \
    applyfriendpage.cpp \
    authenfriend.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatitembase.cpp \
    chatuserlist.cpp \
    chatuserwidget.cpp \
    chatview.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    clickedoncelabel.cpp \
    contactuserlist.cpp \
    conuseritem.cpp \
    customizeedit.cpp \
    findfaildlg.cpp \
    findsuccessdialog.cpp \
    friendlabel.cpp \
    global.cpp \
    grouptipitem.cpp \
    httpmgr.cpp \
    listitembase.cpp \
    loadingdia.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagetextedit.cpp \
    pagechat.cpp \
    picturebubble.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    searchlist.cpp \
    statewidget.cpp \
    tcpmgr.cpp \
    textbubble.cpp \
    timerbtn.cpp \
    userdata.cpp \
    usermanager.cpp

HEADERS += \
    adduseritem.h \
    applyfriend.h \
    applyfrienditem.h \
    applyfriendlist.h \
    applyfriendpage.h \
    authenfriend.h \
    bubbleframe.h \
    chatdialog.h \
    chatitembase.h \
    chatuserlist.h \
    chatuserwidget.h \
    chatview.h \
    clickedbtn.h \
    clickedlabel.h \
    clickedoncelabel.h \
    contactuserlist.h \
    conuseritem.h \
    customizeedit.h \
    findfaildlg.h \
    findsuccessdialog.h \
    friendlabel.h \
    global.h \
    grouptipitem.h \
    httpmgr.h \
    listitembase.h \
    loadingdia.h \
    logindialog.h \
    mainwindow.h \
    messagetextedit.h \
    pagechat.h \
    picturebubble.h \
    registerdialog.h \
    resetdialog.h \
    searchlist.h \
    singleton.h \
    statewidget.h \
    tcpmgr.h \
    textbubble.h \
    timerbtn.h \
    userdata.h \
    usermanager.h

FORMS += \
    adduseritem.ui \
    applyfriend.ui \
    applyfrienditem.ui \
    applyfriendpage.ui \
    authenfriend.ui \
    chatdialog.ui \
    chatuserwidget.ui \
    conuseritem.ui \
    findfaildlg.ui \
    findsuccessdialog.ui \
    friendlabel.ui \
    grouptipitem.ui \
    loadingdia.ui \
    logindialog.ui \
    mainwindow.ui \
    pagechat.ui \
    registerdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = icon.ico
DESTDIR = ./bin

RESOURCES += \
    res.qrc

DISTFILES += \
    config.ini

# 自动复制配置文件
CONFIG += file_copies

# 1. 定义要拷贝的文件（工程根目录的config.ini）
config_copy.files = $${PWD}/config.ini
new_file_copy.files = $${PWD}/static

# 2. 按编译模式+Windows平台，指定拷贝目标路径（和DESTDIR一致，即exe所在的bin目录）
win32 { # 仅Windows平台执行
CONFIG(debug, debug | release) {
    # Debug模式：拷贝到 编译目录/bin（和exe同目录）
    config_copy.path = $${OUT_PWD}/bin
    new_file_copy.path = $${OUT_PWD}/bin
} else {
    # Release模式：同样拷贝到 编译目录/bin
    config_copy.path = $${OUT_PWD}/bin
    new_file_copy.path = $${OUT_PWD}/bin
}
}

# 3. 启用拷贝规则
COPIES += config_copy
COPIES += new_file_copy

#打印路径（验证用，构建时看编译输出）
message("config_copy: src: " $$config_copy.files)
message("config_copy: dest: " $$config_copy.path)

win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" /utf-8


