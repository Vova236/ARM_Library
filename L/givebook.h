#ifndef GIVEBOOK_H
#define GIVEBOOK_H

#include <QDialog>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDateTime>
namespace Ui {
class GiveBook;
}

class GiveBook : public QDialog
{
    Q_OBJECT

public:
    explicit GiveBook(QWidget *parent = nullptr);
    ~GiveBook();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_7_textEdited(const QString &arg1);

    void on_lineEdit_8_textEdited(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_back2_clicked();

    void on_back_clicked();

private:
    Ui::GiveBook *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    QSqlTableModel *model2;
    QSqlTableModel *model3;
};

#endif // GIVEBOOK_H
