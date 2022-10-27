#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serverbase.h"
#include "windowconnect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void connected_Client();
    void disconnect_Client(int descriptor);
    void Close_window();

private:
    void closeEvent(QCloseEvent *);
    Ui::MainWindow* ui;
    windowConnect* newcon;
    QVector<int> descriptors;
    ServerBase* echo;
    bool IsNotStarted = true;
};
#endif // MAINWINDOW_H
