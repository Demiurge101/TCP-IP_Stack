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

    ioTextBrow.setFileName("ioTextBrowser.txt");

}

MainWindow::~MainWindow()
{
    if(AutoTimer != nullptr)
        delete AutoTimer;
    if(ioTextBrow.isOpen())
        ioTextBrow.close();

    ioTextBrow.remove();
    ui->textBrowser->clear();
    delete socket;
    delete ui;
}

void MainWindow::slotReadyRead()
{    
    QByteArray data, des, temp;
    temp = socket->readAll();
    int pos = findPosSize(temp);

    while(pos != -1){
    des.append(temp.mid(pos, 2));
    len_mess = des.toInt();
    des.clear();

    temp.remove(0, pos + 2);
    des.append(temp.left(4));
    temp.remove(0, 4);
    data.append(temp.left(len_mess - 4));
    len += len_mess;
    len_mess = 0;

        if(ui->label->text() == "-1"){
            ui->label->setText(des);
            QString str(data);
            ui->textBrowser->append(str);
        }
        else{

            CounterStr++;
            browser.append(des + ":" + data + "\n");

            if(CounterStr % 100 == 0){
                len_half = len;
                len = 0;
            }

            if(CounterStr % 200 == 0){
                browser.remove(0, len_half);
                len_half = 0;
                ui->textBrowser->clear();
                ui->textBrowser->append(browser);
                browser.clear();
                ui->textBrowser->append(/*browser*/des + ": " + data);
                len = 0;
            }
            else{
                ui->textBrowser->append(des + ": " + data);
            }

            if(CounterStr % 100 == 0){
                if(ioTextBrow.open(QIODevice::WriteOnly | QIODevice::Text) || ioTextBrow.isOpen()){
                    des.clear();
                    des.append(ui->textBrowser->toPlainText());
                    ioTextBrow.write(des);
                    ioTextBrow.flush();
                }
            }
        }
        data.clear();des.clear();temp.clear();
        pos = findPosSize(temp);
    }
    data.clear();des.clear();temp.clear();
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

int MainWindow::findPosSize(QByteArray array)
{
    int pos = array.indexOf("@LABEL@/");
    if(pos == -1)
        return pos;
    return pos + 8;
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
            connect(AutoTimer, &QTimer::timeout, this, &MainWindow::AutoSendSlot, Qt::DirectConnection);
            AutoTimer->start(1000);
            ui->textBrowser->append("Auto mode started");
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
