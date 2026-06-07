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
