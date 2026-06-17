#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QSqlTableModel>
#include <QInputDialog>
#include <QMessageBox>
#include "jewelry.h"
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if (Database::connectToDatabase()) {
        Database::createTable();
    }


    model = new QSqlTableModel(this);
    model->setTable("Prykrasy");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Назва");
    model->setHeaderData(2, Qt::Horizontal, "Матеріал");
    model->setHeaderData(3, Qt::Horizontal, "Вага (г)");
    model->setHeaderData(4, Qt::Horizontal, "Ціна (грн)");
    model->setHeaderData(5, Qt::Horizontal, "Фірма");
    model->select();
    updateStatistics();


    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);



    ui->tableView->verticalHeader()->setVisible(false);

    row = -1;
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_btnAdd_clicked()
{
    int rowCount = model->rowCount();
    if (model->insertRow(rowCount)) {
        model->setData(model->index(rowCount, 1), "Нова прикраса");
        model->submitAll();
        updateStatistics();
    }
    QModelIndex newIndex = model->index(rowCount, 1);
    ui->tableView->scrollTo(newIndex, QAbstractItemView::EnsureVisible);
    ui->tableView->selectRow(rowCount);
    ui->tableView->edit(newIndex);

}


void MainWindow::on_btnDelete_clicked()
{
    if (row < 0) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, оберіть рядок для видалення.");
        return;

    }
    QMessageBox confirmBox(this);
    confirmBox.setWindowTitle("Підтвердження");


    QString name = model->data(model->index(row, 1)).toString();

    confirmBox.setText("Ви впевнені, що бажаєте видалити: \n\"" + name + "\"?");
    confirmBox.setIcon(QMessageBox::Question);
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setButtonText(QMessageBox::Yes, "Так, видалити");
    confirmBox.setButtonText(QMessageBox::No, "Скасувати");


    confirmBox.setStyleSheet(
        "QMessageBox { background-color: rgb(242, 235, 226); }"
        "QLabel { color: rgb(118, 64, 74); font-family: 'Times New Roman'; font-size: 13pt; }"
        "QPushButton { background-color: rgb(118, 64, 74); color: white; border-radius: 5px; padding: 5px 15px; font-family: 'Times New Roman'; min-width: 100px; }"
        );



    if (confirmBox.exec() == QMessageBox::Yes) {
        model->removeRow(row);
        model->submitAll();
        model->select();
        updateStatistics();
        row = -1;
    }

}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();

    QSqlRecord record = model->record(row);

    int id           = record.value("id").toInt();
    QString name     = record.value("name").toString();
    QString material = record.value("material").toString();
    double weight    = record.value("weight").toDouble();
    double price     = record.value("price").toDouble();
    QString brand    = record.value("brand").toString();



    Jewelry* item = new Jewelry(id, name, material, weight, price, brand);


    ui->labelType->setText(item->getName());
    ui->labelMaterial->setText(item->getMaterial());
    ui->labelWeight->setText(QString::number(item->getWeight()) + " г");
    ui->labelPrice->setText(QString::number(item->getPrice()) + " грн");
    ui->labelBrand->setText(item->getBrand());


    delete item;
}

void MainWindow::on_btnExit_clicked()
{
    this->close();
}

void MainWindow::on_btnShow_clicked()
{
    model->setFilter("");
    model->select();
    updateStatistics();
}


void MainWindow::on_btnSearchMaterial_clicked()
{
    QStringList items;
    items << "Усі" << "Золото" << "Срібло" << "Біле золото"<< "Рожеве золото" << "Платина";


    QInputDialog dialog(this);
    dialog.setWindowTitle("Пошук");
    dialog.setLabelText("Оберіть матеріал:");
    dialog.setComboBoxItems(items);
    dialog.setOkButtonText("Обрати");
    dialog.setCancelButtonText("Скасувати");


    dialog.setStyleSheet(
        "QComboBox QAbstractItemView { "
        "  background-color: rgb(242, 235, 226); "
        "   color: rgb(118, 64, 74); "
        "   selection-color:rgb(242, 235, 226); "
        "}"

        "QInputDialog { background-color: rgb(242, 235, 226); }"
        "QLabel { color: rgb(118, 64, 74); font-family: 'Times New Roman'; font-size: 13pt; }"
        "QComboBox { background-color:rgb(242, 235, 226);color: rgb(118, 64, 74); border: 1px solid rgb(118, 64, 74); padding: 5px; font-family: 'Times New Roman'; }"        "QPushButton { background-color: rgb(118, 64, 74); color: rgb(242, 235, 226); border-radius: 5px; padding: 5px 15px; font-family: 'Times New Roman'; min-width: 80px; }"
        );
    if (dialog.exec() == QDialog::Accepted) {
        QString item = dialog.textValue();

        if (!item.isEmpty()) {
            if (item == "Усі") {
                model->setFilter("");
            } else {

                model->setFilter(QString("LOWER(material) = LOWER('%1')").arg(item));
            }
            model->select();
            updateStatistics();
        }
    }
}


void MainWindow::on_btnSearchPrice_clicked()
{
    QInputDialog dialog(this);
    dialog.setWindowTitle("Пошук за ціною");
    dialog.setInputMode(QInputDialog::DoubleInput);
    dialog.setDoubleDecimals(2);
    dialog.setDoubleRange(0.0, 1000000.0);


    dialog.setStyleSheet(


        "QInputDialog { background-color: rgb(242, 235, 226); }"
        "QLabel { color: rgb(118, 64, 74); font-family: 'Times New Roman'; font-size: 13pt; }"
        "QPushButton { background-color: rgb(118, 64, 74); color: white; border-radius: 5px; padding: 5px 15px; min-width: 80px; }"
        "QDoubleSpinBox { "
        "   background-color:  rgb(242, 235, 226); "
        "   color: rgb(118, 64, 74); "
        "   border: 1px solid rgb(118, 64, 74); "
        "   padding: 5px; "
        "   selection-background-color: rgb(118, 64, 74); "
        "   selection-color:  rgb(242, 235, 226); "
        "}"        );


    dialog.setLabelText("Ціна ВІД:");
    if (dialog.exec() != QDialog::Accepted) return;
    double minPrice = dialog.doubleValue();


    dialog.setLabelText("Ціна ДО:");
    dialog.setDoubleValue(minPrice + 100);
    dialog.setDoubleMinimum(minPrice);
    if (dialog.exec() != QDialog::Accepted) return;
    double maxPrice = dialog.doubleValue();

    model->setFilter(QString("price >= %1 AND price <= %2").arg(minPrice).arg(maxPrice));
    model->select();
    updateStatistics();
}

void MainWindow::on_btnAbout_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Про програму");


    QString text = "<div style='font-family: \"Times New Roman\"; font-size: 14pt;'>"
                   "<h3 style='color: #76404a;'>«Магазин прикрас»</h3>"
                   "<p><b>Розробник:</b> Влад Марина Євгенівна, студентка групи 31-КН</p>"
                   "<p>Програма розроблена для курсового проєкту.</p>"
                   "<hr>"
                   "<p>Програма дозволяє вести облік ювелірних виробів, здійснювати пошук за ціною та матеріалом, "
                   "а також редагувати базу даних у реальному часі.</p>"
                   "</div>";

    msgBox.setText(text);


    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color:  rgb(242, 235, 226);"
        "}"
        "QLabel {"
        "   color:  rgb(118, 64, 74);"
        "   font-family: 'Times New Roman';"
        "}"
        "QPushButton {"
        "   background-color:  rgb(118, 64, 74);"
        "   color:rgb(242, 235, 226);"
        "   border-radius: 5px;"
        "   padding: 5px 15px;"
        "   font-family: 'Times New Roman';"
        "}"
        );

    msgBox.exec();}

void MainWindow::updateStatistics() {
    int count = model->rowCount();
    ui->labelCount->setText("Всього товарів: " + QString::number(count));
    ui->labelCount->setStyleSheet(
        "color: rgb(118, 64, 74); "
        "font-family: 'Times New Roman'; "
        "font-weight: bold; "
        "font-size: 14pt;");
}



