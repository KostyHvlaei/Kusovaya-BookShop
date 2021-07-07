#include "sell_form.h"
#include "ui_sell_form.h"

#include <QString>
#include <QErrorMessage>
#include <QSqlQuery>
#include <QTextStream>
#include <QDateTime>

//Sell Form

sell_form::sell_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sell_form)
{
    ui->setupUi(this);
    this->disableSellMaker();
    this->setWindowTitle("Sell");
    this->createUi();

}

void sell_form::setUserId(int ID){
    this->userid = ID;
    this->createUi();
}

void sell_form::createUi(){
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model = new SellModel(this);

    model->populateData(tm_id, tm_book_name, tm_count, tm_price);

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
}

void sell_form::enableSellMaker(){
    this->ui->bttn_addBook->setEnabled(true);
    this->ui->bttn_confirm->setEnabled(true);
    this->ui->tableView->setEnabled(true);
    this->ui->lnedit_bookId->setEnabled(true);
    this->ui->lnedit_bookCount->setEnabled(true);
    this->ui->bttn_cancel->setEnabled(true);
}

void sell_form::disableSellMaker(){
    this->ui->bttn_addBook->setEnabled(false);
    this->ui->bttn_confirm->setEnabled(false);
    this->ui->bttn_cancel->setEnabled(false);
    this->ui->tableView->setEnabled(false);
    this->ui->lnedit_bookId->setEnabled(false);
    this->ui->lnedit_bookCount->setEnabled(false);
}

sell_form::~sell_form()
{
    delete ui;
}

void sell_form::on_bttn_newSell_clicked()
{
    this->enableSellMaker();
    this->ui->bttn_newSell->setEnabled(false);
    this->createUi();
}

void sell_form::printErrMsg(QString errMsg){
    ui->label_ErrorMessage->setStyleSheet("color: rgb(255, 0, 0)");
    ui->label_ErrorMessage->setText(errMsg);
}
void sell_form::printOkMsg(QString okMsg){
    ui->label_ErrorMessage->setStyleSheet("color: rgb(28, 107, 24)");
    ui->label_ErrorMessage->setText(okMsg);
}

//id name count amount
void sell_form::on_bttn_addBook_clicked()
{
    QString inputId = this->ui->lnedit_bookId->text();
    QString inputCount = this->ui->lnedit_bookCount->text();

    bool match_id = true,
            match_count = true;

    int book_id, book_count;

    book_id = inputId.toInt(&match_id);
    book_count = inputCount.toInt(&match_count);

    if(!match_count && !match_id){
        (new QErrorMessage(this))->showMessage("Incorrect id and count input!");
    }
    else if(!match_id){
        (new QErrorMessage(this))->showMessage("Incorrect id input!");
    }
    else if(!match_count){
        (new QErrorMessage(this))->showMessage("Incorrect count input!");
    }
    else{
        QSqlQuery query;
        query.prepare("SELECT name,price FROM books WHERE id = :id;");
        query.bindValue(":id", book_id);
        query.exec();
        if(query.next() && book_count){
            QString book_name = query.value(0).toString();
            double book_price = query.value(1).toDouble();
            double amount = book_price*(double)book_count;

            tm_book_name.append(book_name);
            tm_id.append(book_id);
            tm_count.append(book_count);
            tm_price.append(amount);

            this->amount_confirm += amount;

            this->createUi();
            printOkMsg("Payment sum : " + QString::number(amount_confirm));
        }
        else if(!book_count){
            printErrMsg("Book count is 0!");
        }
        else {
            printErrMsg("Book with this id(" + QString::number(book_id) + ") doesn't exists!");
        }
    }
}


void sell_form::on_bttn_confirm_clicked()
{
    if(tm_id.size() > 0){
        QString sell_ids, buffer;
        QTextStream sstream(&buffer);

        bool first = true;

        for(int i = 0; i < tm_id.size(); i++){
            sstream << (first ? "" : " ");
            first = false;

            bool first_j = true;
            for(int j = 0; j < tm_count.at(i);j++){
                sstream << (first_j ? "" : " ");
                first_j = false;

                sstream << tm_id.at(i);
            }
        }
        sell_ids = *sstream.string();

        QSqlQuery query;
        query.prepare("INSERT INTO sells VALUES(:date, :empl_id,:amount, :ids);");
        query.bindValue(":date", QDateTime::currentDateTime().toString());
        query.bindValue(":empl_id", userid);
        query.bindValue(":amount", amount_confirm);
        query.bindValue(":ids", sell_ids);

        if(query.exec()){
            printOkMsg("SELLED for " + QString::number(amount_confirm));

            this->tm_id.clear();
            this->tm_book_name.clear();
            this->tm_count.clear();
            this->tm_price.clear();

            this->createUi();

            this->ui->lnedit_bookCount->setText("");
            this->ui->lnedit_bookId->setText("");

            this->disableSellMaker();

        }else{
            qDebug() << "Sell inserting error!";
            printErrMsg("Something gone wrong!");
        }
    }
    else{
        (new QErrorMessage(this))->showMessage("The filling is empty!");
        printErrMsg("The filling is empty!");
    }
}


void sell_form::on_bttn_cancel_clicked()
{
    this->disableSellMaker();

    this->tm_book_name.clear();
    this->tm_count.clear();
    this->tm_id.clear();
    this->tm_price.clear();

    this->amount_confirm = 0;

    this->ui->lnedit_bookCount->setText("");
    this->ui->lnedit_bookId->setText("");

    this->ui->bttn_newSell->setEnabled(true);
    this->createUi();
}

//Sell Model

SellModel::SellModel(QObject *parent) : QAbstractTableModel(parent)
{
}

void SellModel::populateData(const QList<int>& ID,
                 const QList<QString>& Book_name,
                 const QList<int>& Count,
                 const QList<double>& Price)
{
    tm_book_name.clear();
    tm_book_name = Book_name;

    tm_count.clear();
    tm_count = Count;

    tm_price.clear();
    tm_price = Price;

    tm_id.clear();
    tm_id = ID;
    return;
}

int SellModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tm_count.length();
}

int SellModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant SellModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return tm_id[index.row()];
    } else if (index.column() == 1) {
        return tm_book_name[index.row()];
    } else if(index.column() == 2){
        return tm_count[index.row()];
    } else if(index.column() == 3){
        return tm_price[index.row()];
    }
    return QVariant();
}

QVariant SellModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("id");
        } else if (section == 1) {
            return QString("name");
        } else if (section == 2) {
            return QString("count");
        } else if (section == 3) {
            return QString("amount");
        }
    }
    return QVariant();
}
