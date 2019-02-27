#include "myurl.h"

const QRegularExpression MyUrl::regEx = QRegularExpression("(?:https?|ftp)://[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\\-._~:/?#\\[\\]@!$&'()*+,;=]+");

MyUrl::MyUrl(QObject* parent) : QObject(parent), m_address(), m_errorMessage(), m_status(Status::NEW)
{

}

MyUrl::MyUrl(const MyUrl& other, QObject* parent) : QObject(parent), m_address(std::move(other.m_address)), m_errorMessage(std::move(other.m_errorMessage)), m_status(other.m_status)
{

}

MyUrl::MyUrl(const QUrl& url, QObject* parent) : QObject (parent), m_address(std::move(url)), m_errorMessage(), m_status(Status::NEW)
{

}

MyUrl::MyUrl(MyUrl&& other, QObject* parent) : QObject(parent)
{
    swap(*this, other);
}

MyUrl& MyUrl::operator=(const MyUrl& other)
{
    MyUrl temp(other);
    swap(*this, temp);

    return *this;
}

MyUrl &MyUrl::operator=(MyUrl&& other)
{
    MyUrl temp(other);
    swap(*this, temp);

    return *this;
}

bool MyUrl::operator==(const MyUrl& url) const
{
    return m_address == url.m_address;
}

QUrl MyUrl::getAddress() const
{
    return m_address;
}

QString MyUrl::getErrorMessage() const
{
    return m_errorMessage;
}

MyUrl::Status MyUrl::getStatus() const
{
    return m_status;
}

void MyUrl::setErrorMessage(const QString& message)
{
    if (m_errorMessage != message)
    {
        m_errorMessage = message;
    }
}

void MyUrl::setStatus(Status status)
{
    if (m_status != status)
    {
        m_status = status;
    }
}

void swap(MyUrl& left, MyUrl& right)
{
    using std::swap;

    swap(left.m_address, right.m_address);
    swap(left.m_errorMessage, right.m_errorMessage);
    swap(left.m_status, right.m_status);
}
