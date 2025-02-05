#include "database.h"

#include "qsqlquery.h"
#include "qtablewidget.h"
#include "qsqlerror.h"
#include "qdebug.h"

Database::Database(const QString &dbName, QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if(db.open())
    {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS counters (id INTEGER, count INTEGER);");
    }
}

Database::~Database()
{
    if(db.isOpen())
        db.close();
}

bool Database::saveCounters(const QTableWidget *table)
{
    if(!table)
        return false;

    if(!db.isOpen())
        return false;

     db.transaction();

     QSqlQuery query;
     if(!query.exec("DELETE FROM counters;"))
     {
         db.rollback();
         return false;
     }
     else
     {
         for(int row = 0; row < table->rowCount(); row++)
         {
             int id = table->item(row, 0)->text().toInt();
             int count = table->item(row, 1)->text().toInt();

             query.prepare("INSERT INTO counters (id, count) VALUES (:id, :count);");
             query.bindValue(":id", id);
             query.bindValue(":count", count);
             if(db.isOpen())
             {
                 if(!query.exec())
                 {
                     qDebug() << "Не удалось выполнить запрос:" << query.lastError().text();
                     db.rollback();
                     return false;
                 }
             }
         }
     }
     db.commit();
     return true;
}

bool Database::loadCounters(QTableWidget *table)
{
    if(!db.isOpen())
        return false;

    QSqlQuery query("SELECT id, count FROM counters;");
    if(!query.exec())
        return false;

    table->clear();

    while(query.next())
    {
        int id = query.value(0).toInt();
        int count = query.value(1).toInt();

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
    }
    return true;
}
