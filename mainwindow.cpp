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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddItemCoun()
{
    int index = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(index);

    auto* l_pTableWidgetItem = new QTableWidgetItem(QString::number(index + 1));
    l_pTableWidgetItem->setFlags(l_pTableWidgetItem->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(index, 0, l_pTableWidgetItem);
}

void MainWindow::DelItemCoun()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::SaveAllCoun()
{

}
