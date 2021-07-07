#include "statistics_form.h"
#include "ui_statistics_form.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <QDate>
#include <QErrorMessage>
#include <QMap>

#include "Book.h"
#include "Employee.h"

statistics_form::statistics_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statistics_form)
{
    ui->setupUi(this);
    this->setWindowTitle("Statistics");

    this->sell_list = read_all_sells();
    this->empl_list = read_adl_empls();
    this->book_list = read_all_books();

    this->createUi();
}

void statistics_form::setup_sells_model(const QString& tableName, const QStringList& headers){
    model_sells = new QSqlTableModel();

    model_sells->setTable(tableName);

    for(int i = 0, j = 0; i < model_sells->columnCount(); i++, j++){
        model_sells->setHeaderData(i, Qt::Horizontal, headers[j]);
    }

    model_sells->select();
}

void statistics_form::update_selection(){
    this->flag_custom_stat_view = false;
    this->createUi();
}

QMap<int, QPair<double, int>> statistics_form::get_empl_selled_amount_and_count(){
    QMap<int, QPair<double, int>> empls;

    for(int i = 0; i < sell_list.size();i++){
        QDate date = sell_list[i].get_date_time().date();
        int empl_id = sell_list[i].get_empl_id();
        if(flag_custom_stat_view
                && date >= date_from
                && date <= date_to)
        {
            empls[empl_id].first += sell_list[i].get_amount();
            empls[empl_id].second += sell_list[i].get_book_ids().size();
        }
        else if(!flag_custom_stat_view){
            empls[empl_id].first += sell_list[i].get_amount();
            empls[empl_id].second += sell_list[i].get_book_ids().size();
        }
    }
    return empls;
}

QMap<int, int> statistics_form::get_books_selled_count(){
    QMap<int, int> books;

    for(int i = 0; i < sell_list.size();i++){
        QDate date = sell_list[i].get_date_time().date();

        int BookId = sell_list[i].get_empl_id();

        if(flag_custom_stat_view
                && date >= date_from
                && date <= date_to)
        {
            books[BookId] += sell_list[i].get_book_ids().size();

            this->full_count += sell_list[i].get_book_ids().size();
            this->full_amount += sell_list[i].get_amount();
            this->sells_count++;
        }
        else if(sell_list[i].get_empl_id() == BookId && !flag_custom_stat_view){
            books[BookId] += sell_list[i].get_book_ids().size();

            this->full_count += sell_list[i].get_book_ids().size();
            this->full_amount += sell_list[i].get_amount();
            this->sells_count++;
        }
    }
    return books;
}

//BOOKS id name price selled count
void statistics_form::complete_stat(){
    tm_book_id.clear();
    tm_book_name.clear();
    tm_book_price.clear();
    tm_books_selled_count.clear();

    QMap<int, int> book_stat = get_books_selled_count();

    for(int i = 0; i < book_list.size();i++){
        if(!book_stat.contains(book_list[i].getId()) &&
                this->ui->checkBox_showEmptys->isChecked()){
            continue;
        }
        tm_book_id.append(book_list[i].getId());
        tm_book_name.append(book_list[i].getName());
        tm_book_price.append(book_list[i].getPrice());

        if(book_stat.contains(book_list[i].getId())){
            tm_books_selled_count.append(book_stat[book_list[i].getId()]);
        }
        else{
            tm_books_selled_count.append(0);
        }
    }

    tm_empl_id.clear();
    tm_empl_name.clear();
    tm_empl_salary.clear();
    tm_empl_selled_amount.clear();
    tm_empl_selled_count.clear();

    QMap<int, QPair<double, int>> empl_stat = get_empl_selled_amount_and_count();

    for(int i = 0; i < empl_list.size();i++){
        if(!empl_stat.contains(empl_list[i].getId()) &&
                this->ui->checkBox_showEmptys->isChecked()){
            continue;
        }
        tm_empl_id.append(empl_list[i].getId());
        tm_empl_name.append(empl_list[i].getName());
        tm_empl_salary.append(empl_list[i].getSalary());

        if(empl_stat.contains(empl_list[i].getId())){
            tm_empl_selled_amount.append(empl_stat[empl_list[i].getId()].first);
            tm_empl_selled_count.append(empl_stat[empl_list[i].getId()].second);

        }
        else{
            tm_empl_selled_amount.append(0);
            tm_empl_selled_count.append(0);
        }
    }
}

void statistics_form::createUi(){
    this->ui->tableView_bookSells->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView_empls->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView_sells->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->avg_amount = 0;
    this->avg_count = 0;
    this->full_amount = 0;
    this->full_count = 0;
    this->sells_count = 0;

    model_book_sell = new BookSellModel();
    model_empl = new EmplModel();
    this->setup_sells_model("sells", QStringList() << "date"
        << "employee_id" << "amount" << "ids");

    complete_stat();

    if(sells_count){
        this->avg_amount = this->full_amount/(double)this->sells_count;
        this->avg_count = this->full_count/(double)this->sells_count;
    }

    this->ui->label_AvgAmount->setText(QString::number(this->avg_amount));
    this->ui->label_AvgCount->setText(QString::number(this->avg_count));
    this->ui->label_FullAmount->setText(QString::number(this->full_amount));
    this->ui->label_FullCount->setText(QString::number(this->full_count));
    this->ui->label_SellsCount->setText(QString::number(this->sells_count));

    model_empl->populateData(tm_empl_id, tm_empl_name, tm_empl_salary, tm_empl_selled_amount, tm_empl_selled_count);
    model_book_sell->populateData(tm_book_id, tm_book_name, tm_book_price, tm_books_selled_count);

    this->ui->tableView_bookSells->setModel(model_book_sell);
    this->ui->tableView_empls->setModel(model_empl);
    this->ui->tableView_sells->setModel(model_sells);

    this->ui->tableView_bookSells->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableView_empls->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableView_sells->horizontalHeader()->setStretchLastSection(true);

    this->ui->tableView_bookSells->resizeColumnsToContents();
    this->ui->tableView_empls->resizeColumnsToContents();
    this->ui->tableView_sells->resizeColumnsToContents();
}

statistics_form::~statistics_form()
{
    delete ui;
}

void statistics_form::on_bttn_Apply_clicked()
{
    QDate _date_from = ui->dateEditFrom->date(),
            _date_to = ui->dateTEdito->date();
    if(_date_from > _date_to){
        (new QErrorMessage(this))->showMessage("Incorrect filling of fields!");
    }
    else{
        this->date_from = _date_from;
        this->date_to = _date_to;
        this->flag_custom_stat_view = true;
    }
    this->createUi();
}

QVector<Employee> statistics_form::read_adl_empls(){
    QVector<Employee> empls;
    QSqlQuery query("SELECT * FROM employeers;");
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        int position = query.value(1).toInt();
        QString name = query.value(2).toString();
        QString phone = query.value(3).toString();
        QString adress = query.value(4).toString();
        double salary = query.value(6).toDouble();

        empls.push_back(Employee(id, position, name, phone, adress, salary));
    }

    return empls;
}

QVector<Book> statistics_form::read_all_books(){
    QVector<Book> books;
    QSqlQuery query("SELECT * FROM books;");
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString author = query.value(1).toString();
        QString name = query.value(2).toString();
        QString genre = query.value(3).toString();
        int count = query.value(4).toInt();
        double price = query.value(5).toDouble();

        books.push_back(Book(id, author, name, genre, count, price));
    }

    return books;
}

QVector<Sell> statistics_form::read_all_sells(){
    QVector<Sell> sells;
    QSqlQuery query("SELECT * FROM sells;");
    query.exec();

    while (query.next()) {
        QString date_time_str = query.value(0).toString();
        QDateTime date_time = QDateTime::fromString(date_time_str);
        int empl_id = query.value(1).toInt();
        double amount = query.value(2).toDouble();
        QVector<int> book_ids;

        QString ids = query.value(3).toString();
        QTextStream ss(&ids);
        while (!ss.atEnd()) {
            int tmp;
            ss >> tmp;
            book_ids.push_back(tmp);
        }

        sells.push_back(Sell(date_time, empl_id, amount, book_ids));
    }

    return sells;
}


//Empl Model
EmplModel::EmplModel(QObject *parent) : QAbstractTableModel(parent)
{
}

void EmplModel::populateData(const QList<int>& Tm_id,
                    const QList<QString>& Tm_empl_name,
                    const QList<double>& Tm_empl_salary,
                    const QList<double>& Tm_selled_amount,
                    const QList<int>& Tm_selled_count)
{
    tm_id.clear();
    tm_id = Tm_id;

    tm_empl_name.clear();
    tm_empl_name = Tm_empl_name;

    tm_empl_salary.clear();
    tm_empl_salary = Tm_empl_salary;

    tm_selled_count.clear();
    tm_selled_count = Tm_selled_count;

    tm_selled_amount.clear();
    tm_selled_amount = Tm_selled_amount;

    return;
}

int EmplModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tm_id.length();
}

int EmplModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant EmplModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return tm_id[index.row()];
    } else if (index.column() == 1) {
        return tm_empl_name[index.row()];
    } else if(index.column() == 2){
        return tm_empl_salary[index.row()];
    } else if(index.column() == 3){
        return tm_selled_count[index.row()];
    } else if (index.column() == 4) {
        return tm_selled_amount[index.row()];
    }
    return QVariant();
}

QVariant EmplModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("id");
        } else if (section == 1) {
            return QString("name");
        } else if (section == 2) {
            return QString("salary");
        } else if (section == 3) {
            return QString("books selled");
        } else if (section == 4) {
            return QString("selled amount");
        }
    }
    return QVariant();
}

//Book Sell Model
BookSellModel::BookSellModel(QObject *parent) : QAbstractTableModel(parent)
{
}

void BookSellModel::populateData(const QList<int>& ID,
                  const QList<QString>& Book_name,
                  const QList<double>& Price,
                  const QList<int>& Selled_count)
{
    tm_id.clear();
    tm_id = ID;

    tm_book_name.clear();
    tm_book_name = Book_name;

    tm_price.clear();
    tm_price = Price;

    tm_selled_count.clear();
    tm_selled_count = Selled_count;
    return;
}

int BookSellModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tm_id.length();
}

int BookSellModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant BookSellModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return tm_id[index.row()];
    } else if (index.column() == 1) {
        return tm_book_name[index.row()];
    } else if(index.column() == 2){
        return tm_price[index.row()];
    } else if(index.column() == 3){
        return tm_selled_count[index.row()];
    }
    return QVariant();
}

QVariant BookSellModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("id");
        } else if (section == 1) {
            return QString("name");
        } else if (section == 2) {
            return QString("price");
        } else if (section == 3) {
            return QString("selled count");
        }
    }
    return QVariant();
}
