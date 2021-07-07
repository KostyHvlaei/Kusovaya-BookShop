#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject(parent)
{
    this->connectToDataBase();
}

DataBase::~DataBase(){
    closeDataBase();
}

void DataBase::connectToDataBase()
{
    if(!QFile("ShopDb.db").exists()){
        QFile file("Shop.db");
        file.open(QIODevice::WriteOnly);
        file.close();
        this->openDataBase();

        QSqlQuery query;
        query.prepare("CREATE TABLE books(id INT PRIMARY KEY NOT NULL,author TEXT NOT NULL, name TEXT NOT NULL, genre TEXT NOT NULL, count INT NOT NULL, price REAL NOT NULL);");
        query.exec();
        query.prepare("CREATE TABLE employeers( id INT PRIMARY KEY NOT NULL, position INT NOT NULL,name TEXT NOT NULL, phone TEXT NOT NULL, adress TEXT NOT NULL, password TEXT NOT NULL,salary real NOT NULL);");
        query.exec();
        query.prepare("CREATE TABLE sells(date TEXT NOT NULL ,employee_id INT NOT NULL,amount real NOT NULL, ids TEXT NOT NULL,FOREIGN KEY(employee_id) REFERENCES employeers(id));");
        query.exec();
        query.prepare("INSERT INTO employeers VALUES(0, 0, \"admin\", \"+000000000000\", \"adress\", \"admin_password\", 0);");
        query.exec();
    } else {
        this->openDataBase();
    }
}
bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ShopDb.db");
    qDebug() << db.drivers();
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
}

int getRowssCount(QString table_name){
    QSqlQuery query("SELECT * FROM " + table_name);
    int initialPos = query.at();
    int pos = 0;
    if (query.last()){
        pos = query.at() + 1;
    }
    else{
        pos = 0;
    }
    query.seek(initialPos);
    return pos;
}
