#include "myworker.h"

MyWorker::MyWorker(const QString& text, const MyUrl& url, QObject* parent) : QObject(parent), m_text(std::move(text)), m_url(std::move(url))
{
    m_parser = nullptr;
}

MyWorker::~MyWorker()
{
    if (nullptr != m_parser)
    {
        delete m_parser;
    }
}

void MyWorker::run()
{
    m_parser = new MyWebParser();
    QStringList links;
    QString message;

    if (m_parser->findTextAndLinks(links, message, m_text, m_url.getAddress()))
    {
        m_url.setStatus(MyUrl::Status::FOUND);
    }
    else if (message.isEmpty())
    {
        m_url.setStatus(MyUrl::Status::NOT_FOUND);
    }
    else
    {
        m_url.setStatus(MyUrl::Status::MY_ERROR);
        m_url.setErrorMessage(message);
    }

    emit result(qMakePair(m_url, links));
}
