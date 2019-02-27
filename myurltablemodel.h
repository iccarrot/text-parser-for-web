#ifndef MYURLTABLEMODEL_H
#define MYURLTABLEMODEL_H

#include <QAbstractTableModel>
#include "myurl.h"

class MyUrlTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyUrlTableModel(QObject* = nullptr);

    MyUrlTableModel(const MyUrlTableModel&)             = delete;
    MyUrlTableModel(MyUrlTableModel&&)                  = delete;
    MyUrlTableModel& operator=(const MyUrlTableModel&)  = delete;
    MyUrlTableModel& operator=(MyUrlTableModel&&)       = delete;

    int             columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant        data(const QModelIndex&, int = Qt::DisplayRole) const override;
    QVariant        headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override;
    void            newUrl(const QString& address);
    void            reset();
    int             rowCount(const QModelIndex& = QModelIndex()) const override;
    void            setMaxRowsCount(const int value);
    void            updateUrl(const MyUrl&);

signals:
    void            finished();
    void            newUrl(MyUrl&);
    void            updateProgress(const int);

public slots:
    void            slotResult(const QPair<MyUrl, QStringList>& result);

private:
    int             m_mRows;
    int             m_nColumns;
    QVector<MyUrl>  m_urls;
};

#endif // MYURLTABLEMODEL_H
