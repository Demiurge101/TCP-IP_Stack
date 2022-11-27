#ifndef WINDOWCONNECT_H
#define WINDOWCONNECT_H

#include <QWidget>
#include <QFile>

namespace Ui {
class windowConnect;
}

class windowConnect : public QWidget
{
    Q_OBJECT

public:
    explicit windowConnect(QWidget *parent = 0);
    ~windowConnect();

public slots:
    void messageSlot(QByteArray array, int descriptor);

signals:
    void closeWindow();

private:
    void closeEvent(QCloseEvent *);
    Ui::windowConnect *ui;
    QFile ioTextBrow;
    QString browser{ };
    int CounterStr = 0;
    int len = 0;
    int len_half = 0;
};

#endif // WINDOWCONNECT_H
