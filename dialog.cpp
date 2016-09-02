#include "dialog.h"
#include "ui_dialog.h"
#include <QString>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setText(QHostAddress address_, int port_){
    QString string_;
    string_.setNum(port_);
    string_="IP Address:"+address_.toString()+"  Port:"+string_;
    ui->label_2->setText(string_);
}

void Dialog::on_pushButton_clicked()
{
    emit disconnectNetwork();
}
