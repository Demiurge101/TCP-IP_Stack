#ifndef SERVERBASE_H
#define SERVERBASE_H
#include "mconnectsocket.h"
#include<QTcpServer>
#include<QVector>
class ServerBase : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerBase(QObject *parent = 0);
    ~ServerBase();
    void run();
    void delsocket(int descriptor);
    bool isEmpty();
    bool state();
    int getlastDescriptor();

private slots:
    void slotNewConnection();
    void slotClientHandler(QByteArray array, int descriptor);
    void slotRemoveClient();

signals:
    void connectClient();
    void disc(int descriptor);
    void tMessage(QByteArray array, int descriptor);

private:
    QTcpServer* mTcpServer;
    QVector <mConnectSocket*> Clients;
    int lastDescriptor = 0;
};

#endif // SERVERBASE_H
