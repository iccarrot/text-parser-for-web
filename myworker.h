#ifndef MYWORKER_H
#define MYWORKER_H

#include <QObject>
#include <QRunnable>
#include "myurl.h"
#include "mywebparser.h"

class MyWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MyWorker(const QString& text, const MyUrl& url, QObject* parent = nullptr);
    ~MyWorker() override;

    MyWorker(const MyWorker&)               = delete;
    MyWorker(MyWorker&&)                    = delete;
    MyWorker& operator=(const MyWorker&)    = delete;
    MyWorker& operator=(MyWorker&&)         = delete;

    void            run() override;

signals:
    void            result(const QPair<MyUrl, QStringList>&);

private:
    MyWebParser*    m_parser;
    const QString   m_text;
    MyUrl           m_url;
};

#endif // MYWORKER_H
