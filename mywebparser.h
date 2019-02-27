#ifndef MYWEBPARSER_H
#define MYWEBPARSER_H

#include <QEventLoop>
#include <QNetworkReply>
#include <QObject>
#include <QRegularExpression>
#include <myurl.h>

class MyWebParser : public QObject
{
    Q_OBJECT
public:
    MyWebParser(QObject* = nullptr);

    MyWebParser(const MyWebParser&)             = delete;
    MyWebParser(MyWebParser&&)                  = delete;
    MyWebParser& operator=(const MyWebParser&)  = delete;
    MyWebParser& operator=(MyWebParser&&)       = delete;

    bool                    findTextAndLinks(QStringList& links, QString& message, const QString& text, const QUrl& url);

private:
    QNetworkAccessManager   m_manager;

    QNetworkReply*          download(const QUrl& url);
    QStringList             findLinks(const QString& line);
    bool                    findText(const QString& line, const QString& text);
};

#endif // MYWEBPARSER_H
