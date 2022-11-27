#include "windowconnect.h"
#include "ui_windowconnect.h"

windowConnect::windowConnect(QWidget *parent) : QWidget(parent), ui(new Ui::windowConnect)
{
    ui->setupUi(this);
    ioTextBrow.setFileName("ioTextBrowser.txt");
}

windowConnect::~windowConnect()
{
    if(ioTextBrow.isOpen())
        ioTextBrow.close();
    ioTextBrow.remove();
    ui->textBrowser->clear();
    delete ui;
}

void windowConnect::messageSlot(QByteArray array, int descriptor)
{
        QByteArray des;
        des.setNum(descriptor);
        len += des.size() + array.size() + 2;

        CounterStr++;
        browser.append(des + ":" + array + "\n");

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
            ui->textBrowser->append(des + ": " + array);
            len =  0;

        }
        else{
            ui->textBrowser->append(des + ": " + array);
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

void windowConnect::closeEvent(QCloseEvent *)
{
    emit closeWindow();
}
