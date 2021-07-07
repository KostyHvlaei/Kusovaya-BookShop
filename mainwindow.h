#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtSql/QtSql>

#include <auth_form.h>
#include <book_form.h>
#include <sell_form.h>
#include <empl_form.h>
#include <statistics_form.h>
#include <settings_form.h>
#include <database.h>

#include "Book.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void display();

private slots:
    void authorize();

    void on_buttonSell_clicked();

    void on_buttonStats_clicked();

    void on_buttonFindEmpl_clicked();

    void on_buttonFindBook_clicked();

    void on_buttonSettings_clicked();

private:
    DataBase _database;

    auth_form ui_Auth;

    sell_form ui_Sell;
    book_form ui_Book;
    empl_form ui_Empl;
    statistics_form ui_Stat;
    settings_form   ui_Settings;

    void update_stat_ui();

    Ui::MainWindow* ui;

    QString _username;
    QString _user_password;
    int priority, _userid;
};
#endif // MAINWINDOW_H
