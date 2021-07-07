#include "empl_form.h"
#include "ui_empl_form.h"

#include "Employee.h"
#include "Validating.h"
#include "database.h"

empl_form::empl_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empl_form)
{
    ui->setupUi(this);
    this->setWindowTitle("Employeers");

    this->setup_model("employeers", QStringList() << "id"
        << "position" << "name" << "phone" << "adress" << "password" << "salary");

    users_count = getRowssCount("employeers");

    ui->cbox_EmplFilter->addItem("id");
    ui->cbox_EmplFilter->addItem("position");
    ui->cbox_EmplFilter->addItem("name");
    ui->cbox_EmplFilter->addItem("phone");
    ui->cbox_EmplFilter->addItem("adress");

    ui->cboxPosition->addItem("admin");
    ui->cboxPosition->addItem("manager");
    ui->cboxPosition->addItem("seller");

    ui->frame->hide();

    this->create_ui();
}

void empl_form::update_ui(){
    this->setup_model("employeers", QStringList() << "id"
        << "position" << "name" << "phone" << "adress" << "password" << "salary");

    this->create_ui();
}

void empl_form::setup_model(const QString& tableName , const QStringList& headers){
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i, Qt::Horizontal, headers[j]);
    }

    model->setSort(0, Qt::AscendingOrder);
}

void empl_form::create_ui(){
    ui->tableEmpls->setModel(model);

    ui->tableEmpls->setColumnHidden(5, true);

    ui->tableEmpls->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEmpls->setSelectionMode(QAbstractItemView::SingleSelection);


    if(user_priority != 0){
        ui->tableEmpls->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else{
        ui->tableEmpls->setEditTriggers(QAbstractItemView::DoubleClicked);
    }

    if(user_priority >= 1){
        ui->button_AddEmpl->setEnabled(false);
        ui->button_RemvEmpl->setEnabled(false);
    }
    else{
        ui->button_AddEmpl->setEnabled(true);
        ui->button_RemvEmpl->setEnabled(true);
    }

    ui->tableEmpls->horizontalHeader()->setStretchLastSection(true);

    model->select();
    ui->tableEmpls->resizeColumnsToContents();
}

void empl_form::setUserPriority(int Priority){
    this->user_priority = Priority;

    this->update_ui();
}

empl_form::~empl_form()
{
    delete ui;
}

void empl_form::on_button_RemvEmpl_clicked()
{
    QModelIndexList list = ui->tableEmpls->selectionModel()->selectedRows();

    if(users_count == 1){
        (new QErrorMessage(this))->showMessage("It's impossible to delete a single user!");
    }
    else if(!list.empty()){
        model->removeRow(list.at(0).row());

        this->update_ui();
    }
    else{
        (new QErrorMessage(this))->showMessage("Selection is empty!");
    }
}

void empl_form::on_button_AddEmpl_clicked()
{
    ui->frame->show();
}

void empl_form::on_lnedit_EmplFilter_textEdited(const QString &arg1)
{
    QString filter = ui->cbox_EmplFilter->currentText();

    if(arg1.isEmpty()){
        model->setFilter("");
    }
    else if(filter == "position" || filter == "id"){
        model->setFilter(filter + " = " + arg1);
    }
    else{
        model->setFilter(filter + " LIKE '%" + arg1 + "%'");
    }

    this->create_ui();
}

void empl_form::on_buttonAdd_clicked()
{
    QString Id       = ui->lineId->text();
    int     Position = getPositionIndex(ui->cboxPosition->currentText());
    QString Name     = ui->lineName->text();
    QString Phone    = ui->linePhone->text();
    QString Adress   = ui->lineAdress->text();
    QString Password = ui->linePassw->text();
    QString Salary   = ui->lineSalary->text();

    bool id_match = true;

    int id = Id.toInt(&id_match);

    bool name_match = Name.size() != 0;
    bool phone_match = validate_phoneNumber(Phone);
    bool adress_match = Adress.size() != 0;
    bool passw_match = validate_password(Password);

    bool sal_match;
    double salary = Salary.toDouble(&sal_match);

    if(name_match && phone_match && adress_match && passw_match && id_match && sal_match){
        QSqlQuery query;

        query.prepare("INSERT INTO employeers VALUES (:id, :position, :name, :phone, :adress, :password, :salary)");

        query.bindValue(":id", id);
        query.bindValue(":position", Position);
        query.bindValue(":name", Name);
        query.bindValue(":phone", Phone);
        query.bindValue(":adress", Adress);
        query.bindValue(":password", Password);
        query.bindValue(":salary", salary);

        if(!query.exec()){
            (new QErrorMessage(this))->showMessage("Adding error!");
        }
        else{
            this->update_ui();
            emit emplAddedSignal();
        }

        ui->frame->hide();
    }
    else{
        (new QErrorMessage(this))->showMessage("Сheck the correctness of the entered data!");
    }
}

void empl_form::on_buttonCancel_clicked()
{
    this->ui->frame->hide();
}
