#pragma once

#ifndef BOOK_FORM_H
#define BOOK_FORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QErrorMessage>

namespace Ui {
class book_form;
}

class book_form : public QDialog
{
    Q_OBJECT

public:
    explicit book_form(QWidget *parent = nullptr);
    ~book_form();

    void setUserPriority(int Priority);
    void update_ui();

signals:
    void bookFormSignal();
    void bookAddedSignal();

private slots:
    void on_edit_findBook_textEdited(const QString &arg1);

    void on_button_addBook_clicked();

    void on_button_remvBook_clicked();

private:
    void setup_model(const QString& tableName, const QStringList& headers);
    void create_ui();

    int user_priority = 2;

    Ui::book_form *ui;
    QSqlTableModel *model;
};

#endif // BOOK_FORM_H
