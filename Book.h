#pragma once

#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QObject>

class Book {
public:
    Book(){}
    Book(int Id,  QString Author, QString Name, QString Genre,  int Count, double Price) :
        id(Id), author(Author), name(Name), genre(Genre), count(Count), price(Price) {
    }

    int getId(){
        return this->id;
    }
    QString getAuthor(){
        return this->author;
    }
    QString getName(){
        return this->name;
    }
    QString getGenre(){
        return this->genre;
    }
    int getCount(){
        return this->count;
    }
    double getPrice(){
        return this->price;
    }

private:
    int id;
    QString author;
    QString name;
    QString genre;
    int count;
    double price;
};

#endif // BOOK_H
