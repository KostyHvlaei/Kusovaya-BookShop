#pragma once

#ifndef EMPL_FORM_H
#define EMPL_FORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QErrorMessage>

#include <database.h>

namespace Ui {
class empl_form;
}

class empl_form : public QDialog
{
    Q_OBJECT

public:
    explicit empl_form(QWidget *parent = nullptr);
    ~empl_form();

    void setUserPriority(int Priority);
    void update_ui();

signals:
    void emplFormSignal();
    void emplAddedSignal();

private slots:
    void on_button_RemvEmpl_clicked();

    void on_button_AddEmpl_clicked();

    void on_lnedit_EmplFilter_textEdited(const QString &arg1);

    void on_buttonAdd_clicked();

    void on_buttonCancel_clicked();

private:
    void setup_model(const QString& tableName, const QStringList& headers);
    void create_ui();

    int user_priority = 2;

    int users_count = 0;

    QSqlTableModel *model;
    Ui::empl_form *ui;
};

#endif // EMPL_FORM_H
