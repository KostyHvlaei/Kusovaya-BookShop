#include "auth_form.h"
#include "ui_auth_form.h"

auth_form::auth_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::auth_form)
{
    ui->setupUi(this);
}

auth_form::~auth_form()
{
    delete ui;
}

QString auth_form::getLogin(){
    return ui->edit_login->text();
}

QString auth_form::getPassword(){
    return ui->edit_password->text();
}

void auth_form::on_button_auth_clicked()
{
    emit button_auth_clicked();
}
