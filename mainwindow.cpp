#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->AddPushButton, SIGNAL(released()), this, SLOT(AddItemCoun()));
    connect(ui->DelPushButton, SIGNAL(released()), this, SLOT(DelItemCoun()));
    connect(ui->SavePushButton, SIGNAL(released()), this, SLOT(SaveAllCoun()));    
    connect(this, SIGNAL(needUpdateTable()), this, SLOT(UpdateTable()));

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
        std::lock_guard<std::mutex> lock(Mutex);

        for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            auto* ItemCounter = ui->tableWidget->item(i, 2);
            if(ItemCounter)
                ItemCounter->setText(QString::number(ItemCounter->text().toInt() + 1));
        }
        emit needUpdateTable();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void MainWindow::AddItemCoun()
{
    std::lock_guard<std::mutex> lock(Mutex);
    int index = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(index);

    auto* l_pTableWidgetItem = new QTableWidgetItem(QString::number(index + 1));
    l_pTableWidgetItem->setFlags(l_pTableWidgetItem->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(index, 0, l_pTableWidgetItem);
    ui->tableWidget->setItem(index, 1, new QTableWidgetItem("0"));
}

void MainWindow::DelItemCoun()
{
    std::lock_guard<std::mutex> lock(Mutex);
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::SaveAllCoun()
{
    std::lock_guard<std::mutex> lock(Mutex);
}

void MainWindow::UpdateTable()
{
    ui->tableWidget->viewport()->update();
}
