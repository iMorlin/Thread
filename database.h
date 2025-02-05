#pragma once

#include <QObject>

#include "qsqldatabase.h"

class QTableWidget;

class Database: public QObject
{
    Q_OBJECT
public:
    Database(const QString &dbName, QObject *parent = nullptr);
    ~Database();

    bool saveCounters(const QTableWidget *table);
    bool loadCounters(QTableWidget *table);

private:
    QSqlDatabase db;
};
