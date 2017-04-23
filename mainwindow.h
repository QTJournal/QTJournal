#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#include <QFileDialog>

#include <QStringListModel>
#include <QDebug>

#include <QStandardItemModel>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QListWidgetItem>
#include <model/post.h>

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
    void updateQRString(const QString& str_);
    void updatePostsList(QList<Post*>*posts);

private slots:
    void on_textEdit_destroyed();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

signals:
    void getInfoButtonClicked();
    void verifyQRButtonClicked(QString);
    void getUserInfoButtonClicked();
    void QRtoDecode(QStringList);
};

#endif // MAINWINDOW_H
