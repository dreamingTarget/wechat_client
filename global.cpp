#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* widget) {
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
};

std::function<QString(QString)> xorString = [](QString input) ->QString {
    QString result = input;
    int length = result.length();
    length = length % 255;
    for (int i = 0; i < length; ++i) {
        result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(length)));
    }
    return result;
};

QString gate_url_prefix = "";

std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
    ":/picture/head_1.jpg",
    ":/picture/head_2.jpg",
    ":/picture/head_3.jpg",
    ":/picture/head_4.jpg",
    ":/picture/head_5.jpg",
    // ":/picture/RaceAgainstTime.jpg"
};

std::vector<QString> names = {
    "不信人间有秃头",
    "llfc",
    "zack",
    "c",
    "cpp",
    "golang",
    "java",
    "nodejs",
    "python",
    "rust"
};


