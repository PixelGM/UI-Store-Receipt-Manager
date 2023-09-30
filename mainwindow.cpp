#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "import/store.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_clicked(bool checked)
{
    Receipt receipt;
    if (checked) {
        receipt.setCity("Vancouver");
        ui->textBrowser->setText(QString::fromStdString(receipt.getCity()));
    } else {
        receipt.setCity("Burnaby");
        ui->textBrowser->setText(QString::fromStdString(receipt.getCity()));
    }
}

