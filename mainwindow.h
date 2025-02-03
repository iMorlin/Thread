#pragma once

#include <QMainWindow>
#include <mutex>
#include <atomic>

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

private:
    void CounterIncriment();

private slots:
    void AddItemCoun();
    void DelItemCoun();
    void SaveAllCoun();
    void UpdateTable();

private:
    Ui::MainWindow *ui;

    std::mutex Mutex;
    std::thread Thread;
    std::atomic<bool> StopThread;
};
