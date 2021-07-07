#pragma once

#ifndef SELL_FORM_H
#define SELL_FORM_H

#include <QDialog>
#include <QAbstractTableModel>
#include <QList>
#include <QString>

namespace Ui {
class sell_form;
}

//id name count amount
class SellModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SellModel(QObject *parent = 0);

    void populateData(const QList<int>& ID,
                      const QList<QString>& Book_name,
                      const QList<int>& Count,
                      const QList<double>& Price);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<int> tm_id;
    QList<QString> tm_book_name;
    QList<int> tm_count;
    QList<double> tm_price;
};


class sell_form : public QDialog
{
    Q_OBJECT

public:
    explicit sell_form(QWidget *parent = nullptr);
    ~sell_form();

    void disableSellMaker();
    void enableSellMaker();

    void printErrMsg(QString errMsg);
    void printOkMsg(QString okMsg);

    void setUserId(int ID);

signals:
    void sellFormSignal();

private slots:
    void on_bttn_newSell_clicked();

    void on_bttn_addBook_clicked();

    void on_bttn_confirm_clicked();

    void on_bttn_cancel_clicked();

private:
    SellModel* model;

    double amount_confirm = 0;

    QList<int> tm_id;
    QList<QString> tm_book_name;
    QList<int> tm_count;
    QList<double> tm_price;

    int userid;

    void createUi();
    Ui::sell_form *ui;
};
#endif // SELL_FORM_H
