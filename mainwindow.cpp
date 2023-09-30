#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    if (checked) {
        ui->textBrowser->setText("The radio button is checked.");
    } else {
        ui->textBrowser->setText("The radio button is not checked.");
    }
}

