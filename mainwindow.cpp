#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //test_server;
    //myNetwork *network_=new myNetwork(this);
    /*connect(ui->pushButton,SIGNAL(clicked(bool)),
            network_,SLOT(setUpServer()));*/
    //connect(ui->pushButton,&QPushButton::clicked,&network_,&myNetwork::setUpServer);
    /*connect(ui->pushButton,SIGNAL(clicked()),
            &network_,SLOT(setUpServer()));*/
    /*connect(ui->pushButton,&QPushButton::clicked,[=](){
        qDebug()<<"clicked";
    });*/
    /*connect(network_,&myNetwork::serverSetUp,[=](QHostAddress address_,int port_){
        qDebug()<<address_<<" "<<port_;
    });
    connect(network_,	&myNetwork::connected,[=](){
        qDebug()<<"connected!";
    });
    connect(network_,&myNetwork::dataRead,[=](QString string_){
        qDebug()<<string_;
    });*/

    //end_test_server

    //test_client
    /*connect(this,SIGNAL(test_client(QHostAddress,int)),
            network_,SLOT(setUpClient(QHostAddress,int)));
    emit test_client(QHostAddress("127.0.0.1"),45500);
    connect(this,SIGNAL(test_client_write(QString)),
            network_,SLOT(writeData(QString)));
    connect(network_,&myNetwork::connected,[=](){
        qDebug()<<"connected";
        emit test_client_write("Hello GO!");
    });*/

    //end_test_client
}

MainWindow::~MainWindow()
{
    delete ui;
}
