#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "import/store.h"

Receipt receipt;



// UI
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

void MainWindow::on_title_TextHere_textChanged()
{
    QString textValue = ui->title_TextHere->toPlainText();
    receipt.setTitle(textValue.toStdString());
    ui->textBrowser->setText(receipt.printReceipt());
}

void MainWindow::on_branch_TextHere_textChanged()
{
    QString textValue = ui->branch_TextHere->toPlainText();
    receipt.setBranch(textValue.toStdString());
    ui->textBrowser->setText(receipt.printReceipt());
}

void MainWindow::on_city_TextHere_textChanged()
{
    QString textValue = ui->city_TextHere->toPlainText();
    receipt.setCity(textValue.toStdString());
    ui->textBrowser->setText(receipt.printReceipt());
}
