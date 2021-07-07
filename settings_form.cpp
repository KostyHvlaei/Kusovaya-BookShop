#include "settings_form.h"
#include "ui_settings_form.h"

#include "Validating.h"
#include <QString>
#include <QSqlQuery>
#include <QErrorMessage>

settings_form::settings_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_form)
{
    ui->setupUi(this);
    ui->label_AdressSucces->hide();
    ui->label_PasswSucces->hide();
    ui->label_PhoneSucces->hide();

    this->setWindowTitle("Settings");
}

void settings_form::setUserId(int ID){
    this->userid = ID;
    QSqlQuery query;
    query.prepare("SELECT phone, adress FROM employeers WHERE id = :id;");
    query.bindValue(":id", ID);
    query.exec();
    query.next();
    QString userphone = query.value(0).toString();
    QString useradrees = query.value(1).toString();

    this->ui->lnedit_Phone->setText(userphone);
    this->ui->lnedit_Adress->setText(useradrees);
}

void settings_form::updateLabels(){
    this->ui->label_AdressSucces->hide();
    this->ui->label_PhoneSucces->hide();
    this->ui->label_PasswSucces->hide();

    QSqlQuery query;
    query.prepare("SELECT phone, adress FROM employeers WHERE id = :id;");
    query.bindValue(":id", this->userid);
    query.exec();
    query.next();
    QString userphone = query.value(0).toString();
    QString useradrees = query.value(1).toString();

    this->ui->lnedit_Phone->setText(userphone);
    this->ui->lnedit_Adress->setText(useradrees);
}

settings_form::~settings_form()
{
    delete ui;
}

void settings_form::on_bttn_Suggest_clicked()
{
    QString passw_old = this->ui->lnedit_oldPsw->text();
    QString passw_new = this->ui->lnedit_newPsw->text();
    QString passw_newSugg = this->ui->lnedit_newPswSug->text();


    QSqlQuery query;
    query.prepare("SELECT password FROM employeers WHERE id = :id;");
    query.bindValue(":id", this->userid);

    if(!query.exec()){
        qDebug() << "updating password statemnt wrong!" << this->userid;
    }
    query.next();
    if(query.value(0).toString() == passw_old){
        if(passw_new == passw_newSugg && validate_password(passw_new)){
            query.prepare("UPDATE employeers SET password = :passw WHERE id = :id");
            query.bindValue(":passw", passw_new);
            query.bindValue(":id", this->userid);

            query.exec();

            ui->lnedit_newPsw->setText("");
            ui->lnedit_newPswSug->setText("");
            ui->lnedit_oldPsw->setText("");

            ui->label_PasswSucces->show();
            qDebug() << "Updated";
        }
        else if(passw_new != passw_newSugg){
            (new QErrorMessage(this))->showMessage("Password mismatch!");
        }
        else if(!validate_password(passw_new)){
            (new QErrorMessage(this))->showMessage("Password must contain at least 7 characters, uppercase character and number");
        }
        else{
            (new QErrorMessage(this))->showMessage("Incorrect input!");
        }
    }else{
        (new QErrorMessage(this))->showMessage("Incorrect password!");
    }
}

void settings_form::on_bttn_SugPhone_clicked()
{
    QString new_phone = ui->lnedit_Phone->text();

    if(validate_phoneNumber(new_phone)){
        QSqlQuery query;
        query.prepare("UPDATE employeers SET phone = :phone WHERE id = :id;");
        query.bindValue(":phone", new_phone);
        query.bindValue(":id", this->userid);
        query.exec();

        this->ui->label_PhoneSucces->show();
    }
    else{
        (new QErrorMessage(this))->showMessage("Incorrect phone format!");
    }
}


void settings_form::on_bttn_SugAdress_clicked()
{
    QString new_adress = ui->lnedit_Adress->text();

    if(new_adress.size() >= 5){
        QSqlQuery query;
        query.prepare("UPDATE employeers SET adress = :adress WHERE id = :id;");
        query.bindValue(":adress", new_adress);
        query.bindValue(":id", this->userid);
        query.exec();

        this->ui->label_AdressSucces->show();
    }
    else{
        (new QErrorMessage(this))->showMessage("Short adress!");
    }
}
