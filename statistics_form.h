#pragma once

#ifndef STATISTICS_FORM_H
#define STATISTICS_FORM_H

#include <QDialog>
#include <QList>
#include <QAbstractTableModel>
#include <QString>
#include <QSqlTableModel>
#include <QVector>
#include <QPair>

#include "Book.h"
#include "Employee.h"
#include "Sell.h"

class EmplModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    EmplModel(QObject *parent = 0);

    void populateData(const QList<int>& Tm_id,
                        const QList<QString>& Tm_empl_name,
                        const QList<double>& Tm_empl_salary,
                        const QList<double>& Tm_selled_amount,
                        const QList<int>& Tm_selled_count);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<int> tm_id;
    QList<QString> tm_empl_name;
    QList<double> tm_empl_salary;
    QList<double> tm_selled_amount;
    QList<int> tm_selled_count;
};

class BookSellModel : public QAbstractTableModel
 {
     Q_OBJECT

 public:
     BookSellModel(QObject *parent = 0);

     void populateData(const QList<int>& ID,
                       const QList<QString>& Book_name,
                       const QList<double>& Price,
                       const QList<int>& Selled_count
                       );

     int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
     int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
     QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

 private:
     QList<int> tm_id;
     QList<QString> tm_book_name;
     QList<double> tm_price;
     QList<int> tm_selled_count;
};

namespace Ui {
class statistics_form;
}

class statistics_form : public QDialog
{
    Q_OBJECT

public:
    explicit statistics_form(QWidget *parent = nullptr);
    ~statistics_form();

    void update_selection();

signals:
    void statFormSignal();

private slots:
    void on_bttn_Apply_clicked();

private:
    void setup_sells_model(const QString& tableName, const QStringList& headers);
    void createUi();

    QVector<Employee> empl_list;

    QList<int> tm_empl_id;
    QList<QString> tm_empl_name;
    QList<double> tm_empl_salary;
    QList<double> tm_empl_selled_amount;
    QList<int> tm_empl_selled_count;

    QMap<int, QPair<double, int>> get_empl_selled_amount_and_count();

    QVector<Book> book_list;

    QList<int> tm_book_id;
    QList<QString> tm_book_name;
    QList<double> tm_book_price;
    QList<int> tm_books_selled_count;

    QMap<int, int> get_books_selled_count();

    void complete_stat();

    bool flag_custom_stat_view = false;

    QDate date_from;
    QDate date_to;

    QVector<Sell> sell_list;

    QVector<Employee> read_adl_empls();
    QVector<Book> read_all_books();
    QVector<Sell> read_all_sells();

    double full_amount = 0;
    double avg_amount = 0;
    int    avg_count = 0;
    double full_count = 0;
    int    sells_count = 0;

    EmplModel* model_empl;
    BookSellModel* model_book_sell;
    QSqlTableModel* model_sells;

    Ui::statistics_form *ui;
};

#endif // STATISTICS_FORM_H
