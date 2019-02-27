#include "mywebparser.h"

MyWebParser::MyWebParser(QObject* parent) : QObject(parent), m_manager()
{

}

bool MyWebParser::findTextAndLinks(QStringList& links, QString& message, const QString& text, const QUrl& url)
{
    auto response = download(url);
    auto result = false;

    if (QNetworkReply::NoError == response->error())
    {
        while(response->canReadLine())
        {
            QString line = response->readLine();

            if (findText(line, text))
            {
                result = true;
            }

            links << findLinks(line);
        }
    }
    else
    {
        message = QVariant::fromValue(response->error()).toString();
    }

    return result;
}

QNetworkReply* MyWebParser::download(const QUrl& url)
{
    auto response = m_manager.get(QNetworkRequest(url));
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();

    return response;
}

QStringList MyWebParser::findLinks(const QString& line)
{
    QStringList result;
    auto it = MyUrl::regEx.globalMatch(line);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        if (match.hasMatch())
        {
            result << match.captured(0);
        }
    }

    return result;
}

bool MyWebParser::findText(const QString& line, const QString& text)
{
    QString temp(line);
    temp.remove(QRegularExpression("(<|</)(b|em|i|small|strong|sub|sup|ins|del|mark)>"));

    return line.contains(text, Qt::CaseInsensitive);
}
