#ifndef MYURL_H
#define MYURL_H

#include <QObject>
#include <QRegularExpression>
#include <QUrl>

class MyUrl : public QObject
{
    Q_OBJECT
public:
    enum class Status { NEW, DOWNLOAD, FOUND, NOT_FOUND, MY_ERROR };
    Q_ENUM(Status)

    static const QRegularExpression regEx;

    MyUrl(QObject*                  = nullptr);
    MyUrl(const MyUrl&, QObject*    = nullptr);
    MyUrl(const QUrl&, QObject*     = nullptr);
    MyUrl(MyUrl&&, QObject*         = nullptr);

    MyUrl&  operator=   (const MyUrl&);
    MyUrl&  operator=   (MyUrl&&);
    bool    operator==  (const MyUrl&) const;

    QUrl    getAddress      () const;
    QString getErrorMessage () const;
    Status  getStatus       () const;
    void    setErrorMessage (const QString& message);
    void    setStatus       (const Status status);

    friend void swap(MyUrl&, MyUrl&);

private:
    QUrl    m_address;
    QString m_errorMessage;
    Status  m_status;
};

inline uint qHash(const MyUrl& key, uint seed)
{
    return qHash(key.getAddress(), seed);
}

Q_DECLARE_METATYPE(MyUrl)

#endif // MYURL_H
