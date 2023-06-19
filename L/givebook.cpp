#include "givebook.h"
#include "ui_givebook.h"
int idReader;
GiveBook::GiveBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GiveBook)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Ldb.db");
    db.open();
    query = new QSqlQuery(db);
    model = new QSqlTableModel(this,db);
    model->setTable("readers");
    model->select();
    ui->tableView->setModel(model);
    model2 = new QSqlTableModel(this,db);
    model2->setTable("books");
    model2->setFilter(" existence>0");
    model2->select();
    ui->tableView_2->setModel(model2);
    model3 = new QSqlTableModel(this,db);
    model3->setTable("taken_books");
    model3->select();
    ui->tableView_3->setModel(model3);
    ui->books->hide();
    ui->choose->hide();
    ui->returnBook->hide();
    model2->setHeaderData(1, Qt::Horizontal, "Назва");
    model2->setHeaderData(2, Qt::Horizontal, "Автор");
    model2->setHeaderData(3, Qt::Horizontal, "Рік");
    model2->setHeaderData(4, Qt::Horizontal, "Кількість");
    model2->setHeaderData(5, Qt::Horizontal, "Жанр");
    model2->setHeaderData(6, Qt::Horizontal, "Видавництво");
    model->setHeaderData(1, Qt::Horizontal, "Ім'я");
    model->setHeaderData(2, Qt::Horizontal, "Прізвище");
    model->setHeaderData(3, Qt::Horizontal, "Номер телефону");
    model->setHeaderData(4, Qt::Horizontal, "Дата народження");
    ui->tableView->setColumnWidth(4, 125);
    model3->setHeaderData(1, Qt::Horizontal, "id читача");
    model3->setHeaderData(2, Qt::Horizontal, "id книжки");
    model3->setHeaderData(3, Qt::Horizontal, "Статус");
    model3->setHeaderData(4, Qt::Horizontal, "Дата видачі");
}
GiveBook::~GiveBook()
{
    delete ui;
}
void GiveBook::on_pushButton_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();

    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    int id;
    foreach(const QModelIndex& index, selectedIndexes) {
        id = index.sibling(index.row(), 0).data().toInt();
    }
        QSqlQuery query;
        query.prepare("Select * FROM readers WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
            ui->readers->hide();
            ui->choose->show();
            idReader=id;
            model3->setFilter(" reader_id="+QString::number(idReader)+" AND status=1");
            model3->select();
}
void GiveBook::on_pushButton_2_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView_2->selectionModel();

    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    int id;
    foreach(const QModelIndex& index, selectedIndexes) {
        id = index.sibling(index.row(), 0).data().toInt();
    }
    QSqlQuery q1,q2;
    QDate currentDate = QDate::currentDate();
    q1.prepare("INSERT INTO taken_books VALUES (NULL,:reader_id,:book_id,1,:date)");
    q1.bindValue(":reader_id", idReader);
    q1.bindValue(":book_id", id);
    q1.bindValue(":date", currentDate);
    q1.exec();
    q2.prepare("UPDATE books SET existence=existence-1 WHERE id=:book_id");
    q2.bindValue(":book_id", id);
    q2.exec();

    model2->select();
    model3->select();
}
void GiveBook::on_pushButton_5_clicked()
{
    accept();
}
void GiveBook::on_pushButton_3_clicked()
{
    ui->choose->hide();
    ui->books->show();
}
void GiveBook::on_pushButton_4_clicked()
{
    ui->choose->hide();
    ui->returnBook->show();
}
void GiveBook::on_lineEdit_7_textEdited(const QString &arg1)
{
    QString columnName;
    switch (ui->comboBox_3->currentIndex()) {
    case 0:
        columnName="first_name";
        break;
    case 1:
        columnName="last_name";
        break;
    case 2:
        columnName="phone_num";
        break;
    case 3:
        columnName="birthday";
        break;
    default:
        break;
    }
    model->setFilter(""+columnName+" LIKE '%"+ui->lineEdit_7->text()+"%'");
}
void GiveBook::on_lineEdit_8_textEdited(const QString &arg1)
{
    QString columnName;
    switch (ui->comboBox_2->currentIndex()) {
    case 0:
        columnName="name";
        break;
    case 1:
        columnName="author";
        break;
    case 2:
        columnName="year";
        break;
    case 3:
        columnName="existence";
        break;
    case 4:
        columnName="genre";
        break;
    case 5:
        columnName="edition";
        break;
    default:
        break;
    }
    model2->setFilter(""+columnName+" LIKE '%"+ui->lineEdit_8->text()+"%' AND existence>0");
}
void GiveBook::on_pushButton_6_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView_3->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QList<int> selectedIds;
    foreach(const QModelIndex& index, selectedIndexes) {
        int id = index.sibling(index.row(), 0).data().toInt();
        selectedIds.append(id);
    }
    foreach(int id, selectedIds) {
        QSqlQuery query;
        query.prepare("UPDATE taken_books SET status=0 WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
    }
    foreach (QModelIndex i, selectedIndexes) {
        QVariant value = model3->data(model3->index(i.row(), 2));
        QString id = value.toString();
        QSqlQuery query;
        query.prepare("UPDATE books SET existence=existence+1 WHERE id=:book_id");
        query.bindValue(":book_id", id);
        query.exec();
    }
    model3->select();
    model2->select();
}
void GiveBook::on_back2_clicked()
{
    ui->returnBook->hide();
    ui->choose->show();
}
void GiveBook::on_back_clicked()
{
    ui->books->hide();
    ui->choose->show();
}
