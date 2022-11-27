#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QFile>

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
    QFile ioTextBrow;
    QString browser{ };
    int CounterStr = 0;    
    int len_mess = 0, len = 0, len_half = 0;
    void sendToServer(QString str);
    int findPosSize(QByteArray array);
};

#endif // MAINWINDOW_H
