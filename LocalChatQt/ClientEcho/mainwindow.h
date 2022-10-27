#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotReadyRead();
    void slotDisconnect();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButton_3_clicked();

    void AutoSendSlot();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QTimer* AutoTimer = nullptr;
    void sendToServer(QString str);
};

#endif // MAINWINDOW_H
