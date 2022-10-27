#include "serverbase.h"

ServerBase::ServerBase(QObject *parent) : QTcpServer(parent){

    mTcpServer = new QTcpServer(this);

    if(!mTcpServer->listen(QHostAddress :: LocalHost, 23)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

ServerBase::~ServerBase(){
    for(int i = 0; i < Clients.size(); i++)
        delete Clients[i];
    Clients.clear();
    delete mTcpServer;
}

void ServerBase::run(){
    connect(mTcpServer, &QTcpServer::newConnection, this, &ServerBase::slotNewConnection, Qt::DirectConnection);
    connect(mTcpServer, &QTcpServer::newConnection, this, &ServerBase::connectClient, Qt::DirectConnection);
}

void ServerBase::delsocket(int descriptor)
{
    for(int i = 0; i < Clients.size(); i++){
        if(Clients[i]->getsocketId() == descriptor){
            delete Clients[i];
            Clients.remove(i);
        }
    }
}

bool ServerBase::isEmpty()
{
    return Clients.isEmpty();
}

bool ServerBase::state()
{
    return mTcpServer->isListening();
}

int ServerBase::getlastDescriptor()
{
    return lastDescriptor;
}


void ServerBase::slotNewConnection(){

    QTcpSocket* mTcpSocket = mTcpServer->nextPendingConnection();
    mConnectSocket* mSocket = new mConnectSocket(mTcpSocket);

    connect(mSocket,&mConnectSocket::disconnect, this, &ServerBase::slotRemoveClient, Qt::QueuedConnection);
    connect(mSocket,&mConnectSocket::disconnect, this, &ServerBase::disc, Qt::QueuedConnection);

    connect(mSocket,&mConnectSocket::transMessage, this, &ServerBase::tMessage, Qt::QueuedConnection);
    connect(mSocket,&mConnectSocket::transMessage, this, &ServerBase::slotClientHandler, Qt::QueuedConnection);

    Clients.push_back(mSocket);
    qDebug() << "Client connect " << mSocket->getsocketId();
    lastDescriptor = mSocket->getsocketId();
}

void ServerBase::slotClientHandler(QByteArray array, int descriptor)
{
    QByteArray des;
    des.setNum(descriptor);
    for(int i = 0; i < Clients.size(); i++){
        Clients[i]->write(des + array);
    }
}

void ServerBase::slotRemoveClient()
{
    mConnectSocket* mSocket = (mConnectSocket*) sender();
    if(mSocket != nullptr){
        lastDescriptor = mSocket->getsocketId();
        delete mSocket;
        Clients.removeAll(mSocket);
    }
}
