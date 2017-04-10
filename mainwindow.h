#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "TJAPI.h"
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
private slots:
    void on_textEdit_destroyed();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
signals:
    void getInfoButtonClicked();
    void verifyQRButtonClicked(QString);
    void getUserInfoButtonClicked();
};

#endif // MAINWINDOW_H
