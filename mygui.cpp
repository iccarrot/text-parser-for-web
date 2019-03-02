#include "mygui.h"

MyGUI::MyGUI(QWidget* parent) : QWidget(parent), m_isWorking(false), m_model(), m_threadPool()
{
    m_button                = new QPushButton();
    m_progress              = new QProgressBar();
    m_proxyModel            = new QSortFilterProxyModel();
    m_table                 = new QTableView();
    m_vbLayout              = new QVBoxLayout();

    m_searchBox             = new QGroupBox();
    m_searchLayout          = new QVBoxLayout();
    m_searchText            = new MyLabelWithLineEdit();
    m_searchUrl             = new MyLabelWithLineEdit();

    m_optionsBox            = new QGroupBox();
    m_optionsLayout         = new QHBoxLayout();
    m_optionsUrlCount       = new MyLabelWithLineEdit();
    m_optionsThreadCount    = new MyLabelWithLineEdit();
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

    m_button->setText("Start");

    m_proxyModel->setDynamicSortFilter(false);
    m_proxyModel->setSourceModel(&m_model);

    m_table->setModel(m_proxyModel);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->setSortingEnabled(true);

    m_searchText->init("Text:", "example");
    m_searchUrl->init("Start url:", "http://example.com/");
    m_searchLayout->addWidget(m_searchUrl);
    m_searchLayout->addWidget(m_searchText);
    m_searchBox->setTitle("Search");
    m_searchBox->setLayout(m_searchLayout);

    m_optionsThreadCount->init("Maximum number of threads:", "2");
    m_optionsUrlCount->init("Maximum number of scanning urls:", "10");
    m_optionsLayout->addWidget(m_optionsThreadCount);
    m_optionsLayout->addWidget(m_optionsUrlCount);
    m_optionsBox->setTitle("Options");
    m_optionsBox->setLayout(m_optionsLayout);

    m_vbLayout->addWidget(m_searchBox);
    m_vbLayout->addWidget(m_optionsBox);
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
    m_threadPool.start(worker);
}

void MyGUI::showMessageBox(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MyGUI::startWork()
{
    if (m_searchUrl->getText().isEmpty() or m_searchText->getText().isEmpty())
    {
        showMessageBox("Start url or searching text are empty");
    }
    else if (not MyUrl::regEx.match(m_searchUrl->getText()).hasMatch())
    {
        showMessageBox("Wrong url");
    }
    else if (0 >= m_optionsUrlCount->getText().toInt() or 0 >= m_optionsThreadCount->getText().toInt())
    {
        showMessageBox("Maximum number of scanning urls and maximum number of threads must be more or equal then one");
    }
    else
    {
        update(true);
        m_progress->reset();
        m_progress->setMaximum(0);
        m_model.reset();
        m_model.setMaxRowsCount(m_optionsUrlCount->getText().toInt());
        m_model.newUrl(m_searchUrl->getText());
        m_threadPool.setMaxThreadCount(m_optionsThreadCount->getText().toInt());
    }
}

void MyGUI::stopWork()
{
    m_threadPool.clear();
    slotFinished();
}

void MyGUI::update(const bool isWorking)
{
    m_isWorking = isWorking;
    m_searchText->setReadOnly(isWorking);
    m_optionsUrlCount->setReadOnly(isWorking);
    m_searchUrl->setReadOnly(isWorking);
    m_optionsThreadCount->setReadOnly(isWorking);
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
