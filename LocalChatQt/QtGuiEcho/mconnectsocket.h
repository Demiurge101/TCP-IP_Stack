#ifndef MCONNECTSOKCET_H
#define MCONNECTSOKCET_H
#include<QTcpSocket>
#include<QTimer>

class mConnectSocket : public QObject{
    Q_OBJECT
public:
    mConnectSocket(QTcpSocket* sock, QObject* parent = 0);
    ~mConnectSocket();
    int getsocketId(){ return socketId; }
    void write(QByteArray data){ socket->write(data); }
private:
    QTcpSocket* socket;
    QTimer* timer;
    int socketId;
signals:
    void disconnect(int descriptor);
    void transMessage(QByteArray array, int descriptor);

private slots:
    void SlotReadyRead();
    void SlotTimeoutDisconnect();
    void slotDisconnect();
};

#endif // MCONNECTSOKCET_H
