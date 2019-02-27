#ifndef MYGUI_H
#define MYGUI_H

#include <QtWidgets>
#include "mylabelwithlineedit.h"
#include "myurl.h"
#include "myurltablemodel.h"
#include "myworker.h"

class MyGUI : public QWidget
{
    Q_OBJECT
public:
    MyGUI(QWidget* = nullptr);

    MyGUI(const MyGUI&)             = delete;
    MyGUI(MyGUI&&)                  = delete;
    MyGUI& operator=(const MyGUI&)  = delete;
    MyGUI& operator=(MyGUI&&)       = delete;

    void                    makeConnections();
    void                    init(const QString& product,const QString& version);

private:
    bool                    m_isWorking;
    MyUrlTableModel         m_model;

    QPushButton*            m_button;
    QProgressBar*           m_progress;
    QTableView*             m_table;

    QHBoxLayout*            m_hbLayout;
    QVBoxLayout*            m_vbLayout;

    MyLabelWithLineEdit*    m_searchText;
    MyLabelWithLineEdit*    m_scanningUrlCount;
    MyLabelWithLineEdit*    m_startUrl;
    MyLabelWithLineEdit*    m_threadCount;

    QThreadPool*            m_tPool;

    void                    doWork(const MyUrl& url);
    void                    working(const bool value);

public slots:
    void                    slotNewUrl(MyUrl& url);
    void                    slotFinished();
    void                    slotUpdateProgress(const int value);

private slots:
    void                    slotButtonClicked();
};

#endif // MYGUI_H
