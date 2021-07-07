#include "book_form.h"
#include "ui_book_form.h"

book_form::book_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::book_form)
{
    ui->setupUi(this);
    this->setWindowTitle("Books");

    this->setup_model("books", QStringList() << "id"
        << "author" << "name" << "genre" << "count" << "price");

    ui->cbox_findBook->addItem("id");
    ui->cbox_findBook->addItem("author");
    ui->cbox_findBook->addItem("name");
    ui->cbox_findBook->addItem("genre");

    this->create_ui();
}
void book_form::update_ui(){
    this->setup_model("books", QStringList() << "id"
        << "author" << "name" << "genre" << "count" << "price");

    this->create_ui();
}
void book_form::setup_model(const QString& tableName , const QStringList& headers){
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    model->setEditStrategy(QSqlTableModel::OnFieldChange);


    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i, Qt::Horizontal, headers[j]);
    }

    model->setSort(0, Qt::AscendingOrder);
}
void book_form::create_ui(){
    ui->tableBooks->setModel(model);

    ui->tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBooks->setSelectionMode(QAbstractItemView::SingleSelection);

    if(user_priority >= 2){
        ui->button_addBook->setEnabled(false);
        ui->button_remvBook->setEnabled(false);
        ui->tableBooks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else{
        ui->button_addBook->setEnabled(true);
        ui->button_remvBook->setEnabled(true);
        ui->tableBooks->setEditTriggers(QAbstractItemView::DoubleClicked);
    }

    ui->tableBooks->horizontalHeader()->setStretchLastSection(true);

    model->select();
    ui->tableBooks->resizeColumnsToContents();
}

void book_form::setUserPriority(int Priority){
    this->user_priority = Priority;
    this->update_ui();
}

void book_form::on_edit_findBook_textEdited(const QString &arg1)
{
    QString filter = ui->cbox_findBook->currentText();

    if(arg1.isEmpty()){
        model->setFilter("");
    }
    else if(filter == "id"){
        model->setFilter(filter + " = " + arg1);
    }
    else{
        model->setFilter(filter + " LIKE '%" + arg1 + "%'");
    }

    this->create_ui();
}

book_form::~book_form()
{
    delete ui;
}

void book_form::on_button_addBook_clicked()
{
    QSqlRecord record;
    record.setValue("id", model->rowCount());
    record.setValue("id", -1);
    record.setValue("author", "");
    record.setValue("name", "");
    record.setValue("genre", "");
    record.setValue("count", 0);
    record.setValue("price", 0);


    model->insertRows(model->rowCount(), 1);

    emit bookAddedSignal();
}

void book_form::on_button_remvBook_clicked()
{
    QModelIndexList list = ui->tableBooks->selectionModel()->selectedRows();
    if(!list.empty()){
        model->removeRow(list.at(0).row());
        this->update_ui();
    }
    else{
        (new QErrorMessage(this))->showMessage("Selection is empty!");
    }
}
