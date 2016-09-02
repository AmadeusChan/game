#include "dialogforport.h"
#include "ui_dialogforport.h"
#include <QSignalMapper>

dialogForPort::dialogForPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogForPort)
{
    ui->setupUi(this);
    QSignalMapper *mapper_=new QSignalMapper(this);
    connect(ui->pushButton_0,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_1,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_6,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_7,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_8,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    connect(ui->pushButton_9,SIGNAL(clicked(bool)),
            mapper_,SLOT(map()));
    mapper_->setMapping(ui->pushButton_0,"0");
    mapper_->setMapping(ui->pushButton_1,"1");
    mapper_->setMapping(ui->pushButton_2,"2");
    mapper_->setMapping(ui->pushButton_3,"3");
    mapper_->setMapping(ui->pushButton_4,"4");
    mapper_->setMapping(ui->pushButton_5,"5");
    mapper_->setMapping(ui->pushButton_6,"6");
    mapper_->setMapping(ui->pushButton_7,"7");
    mapper_->setMapping(ui->pushButton_8,"8");
    mapper_->setMapping(ui->pushButton_9,"9");
    connect(mapper_,SIGNAL(mapped(QString)),
            this,SLOT(addText(QString)));
    ui->lineEdit->setText("8888");
}

dialogForPort::~dialogForPort()
{
    delete ui;
}

void dialogForPort::addText(QString string_){
    ui->lineEdit->setText(ui->lineEdit->text()+string_);
}

QString dialogForPort::text(){
    return ui->lineEdit->text();
}

void dialogForPort::on_pushButton_delete_clicked()
{
    QString string_=ui->lineEdit->text();
    if (string_.size()){
        string_.remove(string_.size()-1,1);
        ui->lineEdit->setText(string_);
    }
}

void dialogForPort::on_pushButton_clicked()
{
    close();
}
