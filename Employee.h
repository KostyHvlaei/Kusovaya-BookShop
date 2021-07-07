#pragma once

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QObject>

QString getPostionName(int position);
int getPositionIndex(QString postion);

class Employee{
public:

    Employee(){
    }
    Employee(int Id, int Position, QString Name, QString Phone,
              QString Adress, double Salary) :
        id(Id), position(Position), name(Name), phone(Phone), adress(Adress), salary(Salary) {
    }

    int getId(){
        return this->id;
    }
    int getPosition(){
        return this->position;
    }
    QString getName(){
        return this->name;
    }
    QString getPhone(){
        return this->phone;
    }
    QString getAdress(){
        return this->adress;
    }
    double getSalary(){
        return this->salary;
    }

private:
    int id;
    int position;
    QString name;
    QString phone;
    QString adress;
    double salary;
};

#endif // EMPLOYEE_H
