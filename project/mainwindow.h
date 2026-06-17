#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_btnExit_clicked();

    void on_btnShow_clicked();

    void on_btnSearchMaterial_clicked();

    void on_btnSearchPrice_clicked();

    void on_btnAbout_clicked();

    void updateStatistics();



private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    int row;
};
#endif
