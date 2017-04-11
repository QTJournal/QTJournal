#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SUPER TJ Client");

    //api = new TJAPI();
    //connect(api, SIGNAL(responseIsHere(QString)), this, SLOT(updateText(QString)));
    //connect(api,SIGNAL(updateTextSignal(QString)),this,SLOT(updateText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(const QString& text_)
{
     ui->textEdit->append(QString("\r%1").arg(text_));
}

void MainWindow::on_textEdit_destroyed()
{

}

void MainWindow::on_pushButton_clicked()
{
    //api->getInfo();
    emit getInfoButtonClicked();
}

void MainWindow::on_pushButton_2_clicked()
{
    //api->verifyQR(ui->plainTextEdit->toPlainText());
    emit verifyQRButtonClicked(ui->plainTextEdit->toPlainText());
}

void MainWindow::on_pushButton_3_clicked()
{
    //api->getUserInfo();
    emit getUserInfoButtonClicked();
}
void MainWindow::on_pushButton_4_clicked()
{
    QFileDialog dialog(this);
    QStringList FileNames;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if (dialog.exec()){
        FileNames = dialog.selectedFiles();
    }
}
