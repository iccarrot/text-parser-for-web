#include "mygui.h"

MyGUI::MyGUI(QWidget* parent) : QWidget (parent), m_isWorking(false), m_model()
{
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
    {
        QString title = product + ". Version: " + version;
        setWindowTitle(title);
    }

    m_searchText->init("Search text:", "developer");
    m_scanningUrlCount->init("Maximum number of scanning urls:", "1");
    m_startUrl->init("Start url:", "https://developex.com.ua");
    m_threadCount->init("Maximum number of threads:", "1");
    m_button->setText("Start");

    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setModel(&m_model);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

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

void MyGUI::working(const bool value)
{
    m_isWorking = value;
    m_searchText->setReadOnly(value);
    m_scanningUrlCount->setReadOnly(value);
    m_startUrl->setReadOnly(value);
    m_threadCount->setReadOnly(value);
    m_button->setText(value ? "Stop" : "Start");
}

void MyGUI::slotNewUrl(MyUrl& url)
{
    m_progress->setMaximum(m_progress->maximum() + 1);
    url.setStatus(MyUrl::Status::DOWNLOAD);
    m_model.updateUrl(url);
    doWork(url);
}

void MyGUI::slotFinished()
{
    working(false);
    QMessageBox msgBox;
    msgBox.setText("Finished");
    msgBox.exec();
}

void MyGUI::slotUpdateProgress(const int value)
{
    m_progress->setValue(value);
}

void MyGUI::slotButtonClicked()
{
    if (m_isWorking)
    {
        slotFinished();
        m_tPool->clear();
    }
    else if (m_startUrl->getText().isEmpty() or m_searchText->getText().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Start url or searching text are empty");
        msgBox.exec();
    }
    else if (not MyUrl::regEx.match(m_startUrl->getText()).hasMatch())
    {
        QMessageBox msgBox;
        msgBox.setText("Wrong url");
        msgBox.exec();
    }
    else if (0 >= m_scanningUrlCount->getText().toInt() or 0 >= m_threadCount->getText().toInt())
    {
        QMessageBox msgBox;
        msgBox.setText("Maximum number of scanning urls and maximum number of threads must be more then one");
        msgBox.exec();
    }
    else
    {
        working(true);
        m_progress->reset();
        m_progress->setMaximum(0);
        m_model.reset();
        m_model.setMaxRowsCount(m_scanningUrlCount->getText().toInt());
        m_model.newUrl(m_startUrl->getText());
        m_tPool->setMaxThreadCount(m_threadCount->getText().toInt());
    }
}
