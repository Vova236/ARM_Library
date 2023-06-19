#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "givebook.h"
int id;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Ldb.db");
    db.open();
    model = new QSqlTableModel(this,db);
    model->setTable("books");
    model->select();
ui->readersWidget->hide();
    ui->TakenBooks->hide();
    ui->tableView->setModel(model);
model2 = new QSqlTableModel(this,db);
model2->setTable("readers");
model2->select();
ui->tableView_2->setModel(model2);
model3 = new QSqlTableModel(this,db);
model3->setTable("taken_books");
model3->select();
ui->tableView_3->setModel(model3);
ui->pushButton_8->hide();
ui->pushButton_9->hide();
ui->pushButton_10->hide();
ui->AcceptResuctReader->hide();
model->setHeaderData(1, Qt::Horizontal, "Назва");
model->setHeaderData(2, Qt::Horizontal, "Автор");
model->setHeaderData(3, Qt::Horizontal, "Рік");
model->setHeaderData(4, Qt::Horizontal, "Кількість");
model->setHeaderData(5, Qt::Horizontal, "Жанр");
model->setHeaderData(6, Qt::Horizontal, "Видавництво");
model2->setHeaderData(1, Qt::Horizontal, "Ім'я");
model2->setHeaderData(2, Qt::Horizontal, "Прізвище");
//model2->setHeaderData(3, Qt::Horizontal, "Номер телефону");
model2->setHeaderData(4, Qt::Horizontal, "Дата народження");
ui->tableView_2->setColumnWidth(4, 125);
model3->setHeaderData(1, Qt::Horizontal, "id читача");
model3->setHeaderData(2, Qt::Horizontal, "id книжки");
model3->setHeaderData(3, Qt::Horizontal, "Статус");
model3->setHeaderData(4, Qt::Horizontal, "Дата видачі");
QString pattern = "^(0?[1-9]|[12][0-9]|3[01])\\.(0?[1-9]|1[012])\\.\\d{4}$";
QRegularExpressionValidator* validator = new QRegularExpressionValidator(QRegularExpression(pattern), this);
ui->birthday->setValidator(validator);
pattern = "^\\+?\\d{1,3}?[-.\\s]?\\(?\\d{3}\\)?[-.\\s]?\\d{3}[-.\\s]?\\d{3}$";
QRegularExpressionValidator* validator2 = new QRegularExpressionValidator(QRegularExpression(pattern), this);
ui->phone_num->setValidator(validator2);
QIntValidator* validatorYear = new QIntValidator(1900, 2100, this);
ui->lineEdit_2->setValidator(validatorYear);
QIntValidator* validatorInt = new QIntValidator(1, std::numeric_limits<int>::max(), this);
ui->lineEdit_3->setValidator(validatorInt);

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_4_clicked()
{
    QString name1 = ui->lineEdit->text();
    int year = (ui->lineEdit_2->text()).toInt();
    QString author=ui->lineEdit_8->text();
    int existence = ui->lineEdit_3->text().toInt();
    QString genre = ui->lineEdit_4->text();
    QString edition = ui->lineEdit_5->text();
    QSqlQuery q1;
    q1.prepare("INSERT INTO books (name,author, year, existence, genre, edition) VALUES (:name,:author, :year, :existence, :genre, :edition)");
    q1.bindValue(":name", name1);
    q1.bindValue(":year", year);
    q1.bindValue(":author",author);
    q1.bindValue(":existence", existence);
    q1.bindValue(":genre", genre);
    q1.bindValue(":edition", edition);
    q1.exec();
    model->select();
}
void MainWindow::on_pushButton_7_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QList<int> selectedIds;
    foreach(const QModelIndex& index, selectedIndexes) {
        int id = index.sibling(index.row(), 0).data().toInt();
        selectedIds.append(id);
    }
    foreach(int id, selectedIds) {
        QSqlQuery query;
        query.prepare("DELETE FROM books WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
    }
    model->select();
}
void MainWindow::on_pushButton_6_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    foreach(const QModelIndex& index, selectedIndexes) {
         id = index.sibling(index.row(), 0).data().toInt();
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
        if (query.next()) {
         ui->lineEdit->setText(query.value("name").toString());
         ui->lineEdit_2->setText(query.value("year").toString());
         ui->lineEdit_8->setText(query.value("author").toString());
        ui->lineEdit_3->setText(query.value("existence").toString());
        ui->lineEdit_4->setText(query.value("genre").toString());
        ui->lineEdit_5->setText(query.value("edition").toString());
        ui->pushButton_8->show();
        ui->pushButton_9->show();
        ui->pushButton_4->hide();
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
    }
}
void MainWindow::on_pushButton_8_clicked()
{
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET name = :name,author=:author,year=:year,existence=:existence,genre=:genre,edition=:edition WHERE ID = :id");
    updateQuery.bindValue(":id", id);
    updateQuery.bindValue(":name", ui->lineEdit->text());
    updateQuery.bindValue(":author", ui->lineEdit_8->text());
    updateQuery.bindValue(":year", ui->lineEdit_2->text());
    updateQuery.bindValue(":existence", ui->lineEdit_3->text());
    updateQuery.bindValue(":genre", ui->lineEdit_4->text());
    updateQuery.bindValue(":edition", ui->lineEdit_5->text());
    updateQuery.exec();
    model->select();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    ui->pushButton_4->show();
    ui->pushButton_6->show();
    ui->pushButton_7->show();
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    id=-1;
}
void MainWindow::on_pushButton_9_clicked()
{
    id=-1;
     ui->pushButton_8->hide();
     ui->pushButton_9->hide();
     ui->lineEdit->setText("");
     ui->lineEdit_2->setText("");
     ui->lineEdit_8->setText("");
     ui->lineEdit_3->setText("");
     ui->lineEdit_4->setText("");
     ui->lineEdit_5->setText("");
     ui->pushButton_4->show();
     ui->pushButton_6->show();
     ui->pushButton_7->show();
     model->select();
}
void MainWindow::on_pushButton_2_clicked()
{
    ui->readersWidget->show();
    ui->booksWidget->hide();
    ui->TakenBooks->hide();
}
void MainWindow::on_pushButton_clicked()
{
    ui->readersWidget->hide();
    ui->booksWidget->show();
    ui->TakenBooks->hide();
}
void MainWindow::on_pushButton_3_clicked()
{
    ui->TakenBooks->show();
    ui->readersWidget->hide();
    ui->booksWidget->hide();
}
void MainWindow::on_pushButton_5_clicked()
{
    QString first_name = ui->first_name->text();
    QString last_name = ui->last_name->text();
    QString phone_num = (ui->phone_num->text());
    QString birthday = ui->birthday->text();
    QSqlQuery q1;
    q1.prepare("INSERT INTO readers VALUES (NULL,:first_name, :last_name,:phone_num,:birthday)");
    q1.bindValue(":first_name", first_name);
    q1.bindValue(":last_name", last_name);
    q1.bindValue(":phone_num", phone_num);
    q1.bindValue(":birthday", birthday);
    q1.exec();
    model2->select();
}
void MainWindow::on_lineEdit_6_textEdited(const QString &arg1)
{
    QString columnName;
    switch (ui->comboBox_findBook->currentIndex()) {
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
    model->setFilter(""+columnName+" LIKE '%"+ui->lineEdit_6->text()+"%'");
}
void MainWindow::on_deleteReader_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView_2->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QList<int> selectedIds;
    foreach(const QModelIndex& index, selectedIndexes) {
        int id = index.sibling(index.row(), 0).data().toInt();
        selectedIds.append(id);
    }
    foreach(int id, selectedIds) {
        QSqlQuery query;
        query.prepare("DELETE FROM readers WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
    }
    model2->select();
}
void MainWindow::on_reductReader_clicked()
{
    QItemSelectionModel* selectionModel = ui->tableView_2->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    foreach(const QModelIndex& index, selectedIndexes) {
        id = index.sibling(index.row(), 0).data().toInt();
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM readers WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.next()) {
        ui->first_name->setText(query.value("first_name").toString());
        ui->last_name->setText(query.value("last_name").toString());
        ui->birthday->setText(query.value("birthday").toString());
        ui->phone_num->setText(query.value("phone_num").toString());
        ui->AcceptResuctReader->show();
        ui->pushButton_10->show();
        ui->pushButton_5->hide();
        ui->deleteReader->hide();
        ui->reductReader->hide();
    }
}
void MainWindow::on_AcceptResuctReader_clicked()
{
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE readers SET first_name = :first_name,last_name=:last_name,birthday=:birthday,phone_num=:phone_num WHERE id = :id");
    updateQuery.bindValue(":id", id);
    updateQuery.bindValue(":first_name", ui->first_name->text());
    updateQuery.bindValue(":last_name", ui->last_name->text());
    updateQuery.bindValue(":birthday", ui->birthday->text());
    updateQuery.bindValue(":phone_num", ui->phone_num->text());
    updateQuery.exec();
    model2->select();
    ui->AcceptResuctReader->hide();
    ui->pushButton_10->hide();
    ui->pushButton_5->show();
    ui->deleteReader->show();
    ui->reductReader->show();
    ui->first_name->setText("");
    ui->last_name->setText("");
    ui->birthday->setText("");
    ui->phone_num->setText("");
    id=-1;
}
void MainWindow::on_pushButton_10_clicked()
{
    id=-1;
    model2->select();
    ui->AcceptResuctReader->hide();
    ui->pushButton_10->hide();
    ui->pushButton_5->show();
    ui->deleteReader->show();
    ui->reductReader->show();
    ui->first_name->setText("");
    ui->last_name->setText("");
    ui->birthday->setText("");
    ui->phone_num->setText("");
}
void MainWindow::on_lineEdit_7_textEdited(const QString &arg1)
{
    QString columnName;
    switch (ui->comboBox_2->currentIndex()) {
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
    model2->setFilter(""+columnName+" LIKE '%"+ui->lineEdit_7->text()+"%'");
}
void MainWindow::on_giveBook_clicked()
{
    GiveBook w;
    w.setModal(true);
    w.setWindowTitle("Видача/Повернення книг");
    w.exec();
    model->select();
    model3->select();
}
void MainWindow::on_lineEdit_10_textEdited(const QString &arg1)
{
    QString columnName;
    switch (ui->comboBox_4->currentIndex()) {
    case 0:
        columnName="reader_id";
        break;
    case 1:
        columnName="book_id";
        break;
    case 2:
        columnName="status";
        break;
    case 3:
        columnName="date";
        break;
    default:
        break;
    }
    model3->setFilter(""+columnName+" LIKE '%"+ui->lineEdit_10->text()+"%'");
}

