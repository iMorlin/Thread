#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void AddItemCoun();
    void DelItemCoun();
    void SaveAllCoun();

private:
    Ui::MainWindow *ui;
};
