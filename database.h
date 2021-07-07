#pragma once

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QString>
#include <QFile>
#include <QDate>
#include <QDebug>


int getRowssCount(QString table_name);

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();

    void connectToDataBase();

private:
    QSqlDatabase db;

private:

    bool openDataBase();
    void closeDataBase();
};

#endif // DATABASE_H
