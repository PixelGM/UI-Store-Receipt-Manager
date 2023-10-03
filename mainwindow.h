#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void hideSettingsUI();

    void showSettingsUI();

    void updateTextBrowser();

    void on_city_TextHere_textChanged();

    void on_branch_TextHere_textChanged();

    void on_title_TextHere_textChanged();

    void on_street_TextHere_textChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
