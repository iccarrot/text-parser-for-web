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
    QThreadPool             m_threadPool;

    QPushButton*            m_button;
    QProgressBar*           m_progress;
    QSortFilterProxyModel*  m_proxyModel;
    QTableView*             m_table;
    QVBoxLayout*            m_vbLayout;

    QGroupBox*              m_searchBox;
    QVBoxLayout*            m_searchLayout;
    MyLabelWithLineEdit*    m_searchText;
    MyLabelWithLineEdit*    m_searchUrl;

    QGroupBox*              m_optionsBox;
    QHBoxLayout*            m_optionsLayout;
    MyLabelWithLineEdit*    m_optionsUrlCount;
    MyLabelWithLineEdit*    m_optionsThreadCount;

    void                    doWork(const MyUrl& url);
    void                    showMessageBox(const QString& message);
    void                    startWork();
    void                    stopWork();
    void                    update(const bool isWorking);

public slots:
    void                    slotFinished();
    void                    slotNewUrl(MyUrl& url);
    void                    slotUpdateProgress(const int value);

private slots:
    void                    slotButtonClicked();
};

#endif // MYGUI_H
