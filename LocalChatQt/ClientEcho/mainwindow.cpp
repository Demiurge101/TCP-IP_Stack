#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnect, Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    delete AutoTimer;
    delete socket;
    delete ui;
}

void MainWindow::slotReadyRead()
{
      QByteArray data, des;
      des = socket->read(4);
      data = socket->readAll();
      if(ui->label->text() == "-1"){
          ui->label->setText(des);
          QString str(data);
          ui->textBrowser->append(str);
      }
      else{
      QString str(des + ": " + data);
      ui->textBrowser->append(str);
      }
      data.clear();des.clear();
}

void MainWindow::slotDisconnect()
{
    ui->lineEdit->clear();
    ui->textBrowser->clear();
    ui->label->setNum(-1);

    if(AutoTimer != nullptr){
        if(AutoTimer->isActive()){
            AutoTimer->stop();
            ui->textBrowser->append("Auto mode stoped");
        }

    }
    ui->textBrowser->append("Disconnect");
    socket->close();
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->label->text() == "-1"){
        ui->lineEdit->clear();
        ui->textBrowser->clear();

        socket->connectToHost(ui->lineEdit_2->text(), ui->lineEdit_3->text().toInt());
        if(socket->waitForConnected(10)){
            ui->textBrowser->append("Client connect");
        }
        else{
            ui->textBrowser->append("Connection error");
        }
    }
    else
        ui->textBrowser->append("Connection is valid");
}

void MainWindow::sendToServer(QString str)
{
      QByteArray data;
      data = str.toLocal8Bit();
      socket->write(data);
}

void MainWindow::on_pushButton_2_clicked()
{

    sendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed()
{
    sendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}


void MainWindow::on_pushButton_3_clicked()
{
    if(ui->label->text() == "-1"){
        ui->lineEdit->clear();
        ui->textBrowser->clear();

        ui->textBrowser->append("Client is not connect");
    }
    else{
        if(AutoTimer == nullptr){
            AutoTimer = new QTimer(this);
            connect(AutoTimer, &QTimer::timeout, this, &MainWindow::AutoSendSlot, Qt::QueuedConnection);
        }
        else{
            if(AutoTimer->isActive()){
                AutoTimer->stop();
                ui->textBrowser->append("Auto mode stoped");
            }
            else{
            AutoTimer->start(1000);
            ui->textBrowser->append("Auto mode started");
            }
        }

    }

}

void MainWindow::AutoSendSlot()
{
    static unsigned int test = 0;
    QString t = QString::number(test);
    sendToServer("test " + t);
    test++;
}
