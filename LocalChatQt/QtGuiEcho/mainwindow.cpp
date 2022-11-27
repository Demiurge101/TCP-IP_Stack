#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    echo = new ServerBase(this);
    newcon = new windowConnect;
}

MainWindow::~MainWindow()
{
    if(newcon != nullptr)
        delete newcon;
    delete echo;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    echo->listen();
    if(echo->state() && IsNotStarted){
        echo->run();
        IsNotStarted = false;
        ui->textBrowser->append("Server is started! ");

        connect(echo,&ServerBase::connectClient,this, &MainWindow::connected_Client,Qt::DirectConnection);
        connect(echo,&ServerBase::disc,this, &MainWindow::disconnect_Client,Qt::DirectConnection);

        connect(echo,&ServerBase::tMessage,newcon, &windowConnect::messageSlot, Qt::QueuedConnection);
    }
    else if(!echo->state()){
        ui->textBrowser->append("Server is not started! ");
    }
    else{
        ui->textBrowser->append("Server is worked! ");
    }
}

void MainWindow::connected_Client()
{
    QString str;
    str = QString::number(echo->getlastDescriptor());
    ui->textBrowser->append("Connect client " + str);
    descriptors.push_back(echo->getlastDescriptor());

    if(!echo->isEmpty() && !newcon->isVisible()){
        newcon->show();
    }

    connect(newcon,&windowConnect::closeWindow,this, &MainWindow::Close_window,Qt::QueuedConnection);
}

void MainWindow::disconnect_Client(int descriptor)
{
    QString str;
    str = QString::number(descriptor);
    ui->textBrowser->append("Disconnect client " + str);
    descriptors.removeAll(str.toInt());

    if(echo->isEmpty()){
        newcon->close();
    }
}

void MainWindow::Close_window()
{
    for(int i = 0; i < descriptors.size(); i++){
        echo->delsocket(descriptors[i]);
        descriptors.remove(i);
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
        newcon->close();
}



