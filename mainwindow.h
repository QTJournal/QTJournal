#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "HttpRequestWorker.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QByteArray token;
    Ui::MainWindow *ui;

public slots:
    void updateText(const QString& text_);
    void getInfo();
    void postAutenticate();
    void getUserInfo();


    void handleResult(HttpRequestWorker* worker_);
    void handle_autentification(HttpRequestWorker* worker_);
    //void handle_userInfo(HttpRequestWorker* worker_);
private slots:
    void on_textEdit_destroyed();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
