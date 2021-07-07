#ifndef SELL_H
#define SELL_H

#include <QVector>
#include <QString>
#include <QDateTime>
#include <QObject>

class Sell {
public:
    Sell(QDateTime DateTime, int EmplId, double Amount, QVector<int> BookIds) :
        date_time(DateTime), employee_id(EmplId), amount(Amount), book_ids(BookIds) {}
    virtual ~Sell() {}

    QDateTime get_date_time(){
        return this->date_time;
    }
    int get_empl_id(){
        return this->employee_id;
    }
    double get_amount(){
        return this->amount;
    }
    QVector<int> get_book_ids(){
        return this->book_ids;
    }

private:
    QDateTime date_time;
    int employee_id;
    double amount;
    QVector<int> book_ids;
};

#endif // SELL_H
