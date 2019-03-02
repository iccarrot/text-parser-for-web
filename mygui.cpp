#include "mygui.h"

MyGUI::MyGUI(QWidget* parent) : QWidget(parent), m_isWorking(false), m_model()
{
    m_proxyModel        = new QSortFilterProxyModel();
    m_button            = new QPushButton();
    m_progress          = new QProgressBar();
    m_table             = new QTableView();
    m_hbLayout          = new QHBoxLayout();
    m_vbLayout          = new QVBoxLayout();
    m_searchText        = new MyLabelWithLineEdit();
    m_scanningUrlCount  = new MyLabelWithLineEdit();
    m_startUrl          = new MyLabelWithLineEdit();
    m_threadCount       = new MyLabelWithLineEdit();
    m_tPool             = new QThreadPool();
}

void MyGUI::makeConnections()
{
    connect(m_button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    connect(&m_model, SIGNAL(newUrl(MyUrl&)), this, SLOT(slotNewUrl(MyUrl&)));
    connect(&m_model, SIGNAL(finished()), this, SLOT(slotFinished()));
    connect(&m_model, SIGNAL(updateProgress(const int)), this, SLOT(slotUpdateProgress(const int)));
}

void MyGUI::init(const QString& product,const QString& version)
{
    setWindowTitle(product + ". Version: " + version);

    m_proxyModel->setDynamicSortFilter(false);
    m_proxyModel->setSourceModel(&m_model);

    m_searchText->init("Search text:", "example");
    m_scanningUrlCount->init("Maximum number of scanning urls:", "10");
    m_startUrl->init("Start url:", "http://example.com/");
    m_threadCount->init("Maximum number of threads:", "2");
    m_button->setText("Start");

    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setModel(m_proxyModel);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->setSortingEnabled(true);

    m_hbLayout->addWidget(m_threadCount);
    m_hbLayout->addWidget(m_scanningUrlCount);

    m_vbLayout->addWidget(m_startUrl);
    m_vbLayout->addWidget(m_searchText);
    m_vbLayout->addLayout(m_hbLayout);
    m_vbLayout->addWidget(m_button);
    m_vbLayout->addWidget(m_table);
    m_vbLayout->addWidget(m_progress);

    setLayout(m_vbLayout);

    qRegisterMetaType<QPair<MyUrl, QStringList>>();
}

void MyGUI::doWork(const MyUrl& url)
{
    auto worker = new MyWorker(m_searchText->getText(), url);
    connect(worker, SIGNAL(result(const QPair<MyUrl, QStringList>&)), &m_model, SLOT(slotResult(const QPair<MyUrl, QStringList>&)), Qt::QueuedConnection);
    m_tPool->start(worker);
}

void MyGUI::showMessageBox(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MyGUI::startWork()
{
    if (m_startUrl->getText().isEmpty() or m_searchText->getText().isEmpty())
    {
        showMessageBox("Start url or searching text are empty");
    }
    else if (not MyUrl::regEx.match(m_startUrl->getText()).hasMatch())
    {
        showMessageBox("Wrong url");
    }
    else if (0 >= m_scanningUrlCount->getText().toInt() or 0 >= m_threadCount->getText().toInt())
    {
        showMessageBox("Maximum number of scanning urls and maximum number of threads must be more or equal then one");
    }
    else
    {
        update(true);
        m_progress->reset();
        m_progress->setMaximum(0);
        m_model.reset();
        m_model.setMaxRowsCount(m_scanningUrlCount->getText().toInt());
        m_model.newUrl(m_startUrl->getText());
        m_tPool->setMaxThreadCount(m_threadCount->getText().toInt());
    }
}

void MyGUI::stopWork()
{
    m_tPool->clear();
    slotFinished();
}

void MyGUI::update(const bool isWorking)
{
    m_isWorking = isWorking;
    m_searchText->setReadOnly(isWorking);
    m_scanningUrlCount->setReadOnly(isWorking);
    m_startUrl->setReadOnly(isWorking);
    m_threadCount->setReadOnly(isWorking);
    m_button->setText(isWorking ? "Stop" : "Start");
}

void MyGUI::slotFinished()
{
    showMessageBox("Finished");
    update(false);
}

void MyGUI::slotNewUrl(MyUrl& url)
{
    m_progress->setMaximum(m_progress->maximum() + 1);
    url.setStatus(MyUrl::Status::DOWNLOAD);
    m_model.updateUrl(url);
    doWork(url);
}

void MyGUI::slotUpdateProgress(const int value)
{
    m_progress->setValue(value);
}

void MyGUI::slotButtonClicked()
{
    if (m_isWorking)
    {
        stopWork();
    }
    else
    {
        startWork();
    }
}
