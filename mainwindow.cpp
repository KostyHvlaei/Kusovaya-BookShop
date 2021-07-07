#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Employee.h"

#include <QErrorMessage>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _database()
    , ui_Sell()
    , ui_Stat()
    , ui_Settings()
    , ui(new Ui::MainWindow)
{

    connect(&ui_Auth, SIGNAL(button_auth_clicked()),
            this, SLOT(authorize()));
    connect(&ui_Auth, SIGNAL(destroyed()),
            this, SLOT(show()));

    connect(&ui_Book, &book_form::bookFormSignal, this, &MainWindow::show);
    connect(&ui_Empl, &empl_form::emplFormSignal, this, &MainWindow::show);
    connect(&ui_Sell, &sell_form::sellFormSignal, this, &MainWindow::show);
    connect(&ui_Stat, &statistics_form::statFormSignal, this, &MainWindow::show);
    connect(&ui_Settings, &settings_form::settFormSignal, this, &MainWindow::show);

    ui->setupUi(this);

    ui->centralwidget->setLayout(ui->gridLayout);
}

void MainWindow::update_stat_ui(){
    ui_Stat.update_selection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display(){
    ui_Auth.show();
    ui_Auth.setWindowTitle("Authorization");
}

void MainWindow::authorize(){
    QString tmp_username = ui_Auth.getLogin();
    QString tmp_user_password = ui_Auth.getPassword();

    QString sql = "SELECT * FROM employeers WHERE name = \'" + tmp_username + "\';";

    QSqlQuery query;
    QSqlRecord record;

    if(!query.exec(sql)){
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    record = query.record();
    query.next();


    int usid = query.value(record.indexOf("id")).toInt();
    int uspriority = query.value(record.indexOf("position")).toInt();
    QString usname = query.value(record.indexOf("name")).toString();
    QString uspass = query.value(record.indexOf("password")).toString();

    if(tmp_username == "" || tmp_user_password == ""){
        (new QErrorMessage(this))->showMessage("You haven't entered something");
    }
    else if(tmp_username != usname || tmp_user_password != uspass){
        (new QErrorMessage(this))->showMessage("Incorrect name or password");
        qDebug() << "Uncorrect authorization input!";
    }
    else{
        ui_Auth.close();

        this->show();
        this->_username = tmp_username;
        this->_user_password = tmp_user_password;
        this->priority = uspriority;
        this->_userid = usid;

        this->ui_Book.setUserPriority(this->priority);
        this->ui_Empl.setUserPriority(this->priority);
        this->ui_Settings.setUserId(this->_userid);
        this->ui_Sell.setUserId(this->_userid);


        if(this->priority >= 2){
            this->ui->buttonFindEmpl->setEnabled(false);
            this->ui->buttonStats->setEnabled(false);
        }

        this->ui->labelTimeDay->setText(QDate::currentDate().toString() + "\n" + _username + "\n" + getPostionName(uspriority));
    }
}

void MainWindow::on_buttonSell_clicked()
{
    ui_Sell.show();
    ui_Sell.activateWindow();
}

void MainWindow::on_buttonStats_clicked()
{
    ui_Stat.show();
    ui_Stat.update_selection();
    ui_Stat.activateWindow();
}

void MainWindow::on_buttonFindEmpl_clicked()
{
    ui_Empl.show();
    ui_Empl.activateWindow();
}

void MainWindow::on_buttonFindBook_clicked()
{
    ui_Book.show();
    ui_Book.update_ui();
    ui_Book.activateWindow();
}

void MainWindow::on_buttonSettings_clicked()
{
    ui_Settings.show();
    ui_Settings.updateLabels();
    ui_Settings.activateWindow();
}
