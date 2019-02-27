#include "myurltablemodel.h"

static int count = 0;

MyUrlTableModel::MyUrlTableModel(QObject* parent) : QAbstractTableModel(parent), m_nColumns(2), m_urls()
{

}

QVariant MyUrlTableModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() and Qt::DisplayRole == role)
    {
        MyUrl url(m_urls.at(index.row()));

        if (0 == index.column())
        {
            return url.getAddress();
        }

        if (1 == index.column())
        {
            if (MyUrl::Status::MY_ERROR != url.getStatus())
            {
                if ((MyUrl::Status::NOT_FOUND == url.getStatus()))
                {
                    return QString("not found");
                }

                return QVariant::fromValue(url.getStatus()).toString().toLower();
            }
            else
            {
                return url.getErrorMessage();

            }
        }
    }

    return QVariant();
}

QVariant MyUrlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role)
    {
        if(Qt::Orientation::Horizontal == orientation)
        {
            if (0 == section)
            {
                return "Url";
            }

            if (1 == section)
            {
                return "Status";
            }
        }
        else
        {
            return section + 1;
        }
    }

    return QVariant();
}

void MyUrlTableModel::newUrl(const QString& address)
{
    if (rowCount() >= m_mRows)
    {
        if (0 == rowCount())
        {
            emit finished();
        }

        return;
    }

    if (MyUrl::regEx.match(address).hasMatch())
    {
        MyUrl url(address);

        if (not m_urls.contains(url))
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_urls.push_back(url);
            endInsertRows();

            emit newUrl(url);
        }
    }
}

void MyUrlTableModel::reset()
{
    beginResetModel();
    m_urls.clear();
    endResetModel();
    count = 0;
}

int MyUrlTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return m_nColumns;
}

int MyUrlTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_urls.size();
}

void MyUrlTableModel::setMaxRowsCount(const int value)
{
    m_mRows = value;
}

void MyUrlTableModel::updateUrl(const MyUrl& url)
{
    auto it = std::find(m_urls.begin(), m_urls.end(), url);

    if (it != m_urls.end())
    {
        *it = url;

        if (MyUrl::Status::NEW != url.getStatus() and MyUrl::Status::DOWNLOAD != url.getStatus())
        {
            ++count;
            emit updateProgress(count);
        }

        emit dataChanged(index(0, 0), index(m_urls.size() - 1, 1));
    }
}

void MyUrlTableModel::slotResult(const QPair<MyUrl, QStringList>& result)
{
    updateUrl(result.first);

    foreach(const QString& url, result.second)
    {
        newUrl(url);
    }

    if (not std::any_of(m_urls.rbegin(), m_urls.rend(), [] (const MyUrl url)
        {
            return MyUrl::Status::NEW == url.getStatus() or MyUrl::Status::DOWNLOAD == url.getStatus();
        }))
    {
        emit finished();
    }
}
