#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<QVector<int>>("QVector<int>");

    setWindowTitle("Таблица с бд и потоками");
    connect(ui->AddPushButton, SIGNAL(released()), this, SLOT(AddItemCoun()), Qt::QueuedConnection);
    connect(ui->DelPushButton, SIGNAL(released()), this, SLOT(DelItemCoun()), Qt::QueuedConnection);
    connect(ui->SavePushButton, SIGNAL(released()), this, SLOT(SaveAllCoun()), Qt::QueuedConnection);
    connect(this, SIGNAL(needUpdateTable()), this, SLOT(UpdateTable()), Qt::QueuedConnection);
    connect(this, SIGNAL(needUpdateLabel()), this, SLOT(UpdateLabel()), Qt::QueuedConnection);

    db = new Database("Couners.db", this);
    db->loadCounters(ui->tableWidget);

    StopThread = false;
    Thread = std::thread(&MainWindow::CounterIncriment, this);
}

MainWindow::~MainWindow()
{
    StopThread = true;
    if(Thread.joinable())
        Thread.join();
    delete ui;
}

void MainWindow::CounterIncriment()
{
    while(!StopThread)
    {
        Mutex.lock();
        for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            auto* ItemCounter = ui->tableWidget->item(i, 1);
            if(ItemCounter)
                ItemCounter->setText(QString::number(ItemCounter->text().toInt() + 1));
        }
        Mutex.unlock();
        emit needUpdateTable();
        emit needUpdateLabel();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void MainWindow::AddItemCoun()
{
    Mutex.lock();
    int index = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(index);

    auto* l_pTableWidgetItem = new QTableWidgetItem(QString::number(index + 1));
    l_pTableWidgetItem->setFlags(l_pTableWidgetItem->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(index, 0, l_pTableWidgetItem);
    ui->tableWidget->setItem(index, 1, new QTableWidgetItem("0"));
    Mutex.unlock();
}

void MainWindow::DelItemCoun()
{
    Mutex.lock();
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    Mutex.unlock();
}

void MainWindow::SaveAllCoun()
{
    std::lock_guard<std::mutex> guard(Mutex);
    db->saveCounters(ui->tableWidget);
}

void MainWindow::UpdateTable()
{
    ui->tableWidget->viewport()->update();
}

void MainWindow::UpdateLabel()
{
    int currentSum = 0;

    Mutex.lock();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        auto* ItemCounter = ui->tableWidget->item(i, 1);
        if(ItemCounter)
            currentSum += ItemCounter->text().toInt();
    }
    Mutex.unlock();

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = currentTime - Time;
    auto frequency = QString::number(static_cast<double>(currentSum - Sum) / elapsedSeconds.count());
    ui->label->setText(QString("Частота: %1").arg(frequency));

    Sum = currentSum;
    Time = currentTime;
}
