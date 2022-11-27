#include "mconnectsocket.h"

mConnectSocket::mConnectSocket(QTcpSocket *sock, QObject *parent){
    socket = sock;
    socketId = sock->socketDescriptor();
    QByteArray des, mark_size;
    des.setNum(socketId);
    if(socketId < 1000)
        des.push_front("0");
    mark_size.setNum(20);
    mark_size.push_front("@LABEL@/");
    socket->write(mark_size + des + "Welcome to Echo!");

    timer = new QTimer(this);
    timer->start(30000);

    connect(socket, &QTcpSocket::readyRead, this, &mConnectSocket::SlotReadyRead, Qt::DirectConnection);

    connect(timer,&QTimer::timeout, this, &mConnectSocket::SlotTimeoutDisconnect, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &mConnectSocket::slotDisconnect, Qt::DirectConnection);
}

mConnectSocket::~mConnectSocket(){
    if(timer->isActive()){
        if(socket->isOpen())
        socket->close();
        timer->stop();
        qDebug() << "Client disconnect " << socketId;
    }
    socketId = 0;
    delete socket;
    if(timer != nullptr)
        delete timer;
}

void mConnectSocket::SlotReadyRead(){
    timer->stop();
    timer->start(30000);
    QByteArray array = socket->readAll();
    emit transMessage(array, socketId);
}

void mConnectSocket::SlotTimeoutDisconnect()
{
    socket->close();
    timer->stop();
    qDebug() << "Timeout disconnect " << socketId;
}

void mConnectSocket::slotDisconnect()
{
    emit disconnect(socketId);
}
