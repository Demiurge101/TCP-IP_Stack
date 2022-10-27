#include "windowconnect.h"
#include "ui_windowconnect.h"

windowConnect::windowConnect(QWidget *parent) : QWidget(parent), ui(new Ui::windowConnect)
{
    ui->setupUi(this);
}

windowConnect::~windowConnect()
{
    delete ui;
}

void windowConnect::messageSlot(QByteArray array, int descriptor)
{
        QByteArray des;
        des.setNum(descriptor);
        ui->textBrowser->append(des + ": " + array);
}

void windowConnect::closeEvent(QCloseEvent *)
{
    emit closeWindow();
}
