#pragma once

#ifndef SETTINGS_FORM_H
#define SETTINGS_FORM_H

#include <QDialog>
#include <QString>

namespace Ui {
class settings_form;
}

class settings_form : public QDialog
{
    Q_OBJECT

public:
    explicit settings_form(QWidget *parent = nullptr);
    ~settings_form();

    void setUserId(int ID);
    void updateLabels();

signals:
    void settFormSignal();

private slots:
    void on_bttn_Suggest_clicked();

    void on_bttn_SugPhone_clicked();

    void on_bttn_SugAdress_clicked();

private:

    int userid;
    Ui::settings_form *ui;
};

#endif // SETTINGS_FORM_H
