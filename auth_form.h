#pragma once

#ifndef AUTH_FORM_H
#define AUTH_FORM_H

#include <QDialog>

namespace Ui {
class auth_form;
}

class auth_form : public QDialog
{
    Q_OBJECT

public:
    explicit auth_form(QWidget *parent = nullptr);
    ~auth_form();

    QString getLogin();
    QString getPassword();

signals:
    void button_auth_clicked();

private slots:
    void on_button_auth_clicked();

private:
    Ui::auth_form* ui;
};

#endif // AUTH_FORM_H
