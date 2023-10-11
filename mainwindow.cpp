#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "import/store.h"

// Note for Coder:
// Object is got by using pointer (example: ui->title_TextHere)
// title_TextHere is the object. You can call a function (e.x. ui->title_TextHere->setText("bla"))

Receipt receipt;

void MainWindow::updateTextBrowser()
{
    ui->textBrowser->setText(receipt.printReceipt());
}



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

void MainWindow::hideSettingsUI()
{
    ui->branch_TextHere->hide();
    ui->branch_label->hide();
    ui->city_TextHere->hide();
    ui->city_label->hide();
    ui->street_TextHere->hide();
    ui->street_label->hide();
    ui->textBrowser->hide();
    ui->title_TextHere->hide();
    ui->title_label->hide();
}

void MainWindow::showSettingsUI()
{
    ui->branch_TextHere->show();
    ui->branch_label->show();
    ui->city_TextHere->show();
    ui->city_label->show();
    ui->street_TextHere->show();
    ui->street_label->show();
    ui->textBrowser->show();
    ui->title_TextHere->show();
    ui->title_label->show();
}


void MainWindow::on_title_TextHere_textChanged()
{
    QString textValue = ui->title_TextHere->toPlainText();
    receipt.setTitle(textValue.toStdString());
    updateTextBrowser();
}

void MainWindow::on_street_TextHere_textChanged()
{
    QString textValue = ui->street_TextHere->toPlainText();
    receipt.setStreet(textValue.toStdString());
    updateTextBrowser();
}

void MainWindow::on_branch_TextHere_textChanged()
{
    QString textValue = ui->branch_TextHere->toPlainText();
    receipt.setBranch(textValue.toStdString());
    updateTextBrowser();
}

void MainWindow::on_city_TextHere_textChanged()
{
    QString textValue = ui->city_TextHere->toPlainText();
    receipt.setCity(textValue.toStdString());
    updateTextBrowser();
}

void MainWindow::on_pushButton_clicked()
{
    // I can make 'hidden' a static variable to persist its state between function calls
    static bool hidden = false;

    // Check the current state of 'hidden'
    if (hidden == false)
    {
        hideSettingsUI();
        ui->pushButton->setText("Show");
        hidden = true;
    }
    else
    {
        showSettingsUI();
        ui->pushButton->setText("Hide");
        hidden = false;
    }
}


