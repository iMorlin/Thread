#pragma once

#include <QMainWindow>
#include <mutex>
#include <atomic>

class Database;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void needUpdateTable();
    void needUpdateLabel();

private:
    void CounterIncriment();

private slots:
    void AddItemCoun();
    void DelItemCoun();
    void SaveAllCoun();
    void UpdateTable();
    void UpdateLabel();

private:
    Ui::MainWindow *ui;

    Database* db;
    std::mutex Mutex;
    std::thread Thread;
    std::atomic<bool> StopThread;

    int Sum = 0;
    std::chrono::steady_clock::time_point Time = std::chrono::steady_clock::now();
};
