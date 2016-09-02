#include "mynetwork.h"

#include <QChar>
#include <QByteArray>
#include <QDebug>
#include <QThread>

myNetwork::myNetwork(QObject *parent):QObject(parent){
    qDebug()<<"myNetwork::myNetwork()";
    connected_flag=false;
    server=NULL;
    socket=NULL;
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

QHostAddress myNetwork::getIpAddress(){
    qDebug()<<"myNetwork::getIpAddress()";
    QList<QHostAddress> list_=QNetworkInterface::allAddresses();
    for (int i=0;i<list_.size();++i){
        if (!list_.at(i).isLoopback()){
            if (list_.at(i).protocol()==QAbstractSocket::IPv4Protocol){
                return list_.at(i);
            }
        }
    }
}

void myNetwork::setUpServer(){
    qDebug()<<"void myNetwork::setUpServer()";
    qDebug()<<"thread of network:"<<QThread::currentThreadId();
    server=new QTcpServer(this);
    server->listen(getIpAddress(),8888);
    connect(server,SIGNAL(newConnection()),
            this,SLOT(actionOfConnectedServer()));
    qDebug()<<server->serverAddress()<<" "<<server->serverPort();
    emit serverSetUp(server->serverAddress(),server->serverPort());
}

void myNetwork::actionOfConnectedServer(){
    qDebug()<<"myNetwork::actionOfConnectedServer()";
        socket=server->nextPendingConnection();
        connected_flag=true;
        connect(socket,SIGNAL(readyRead()),
                this,SLOT(readData()));
        connect(socket,SIGNAL(disconnected()),
                this,SIGNAL(disconnected()));
        emit connected();
}

void myNetwork::setUpClient(QHostAddress address_,int port_){
    qDebug()<<"myNetwork::setUpClient()";
    qDebug()<<"thread of network:"<<QThread::currentThreadId();
    socket=new QTcpSocket(this);
    socket->connectToHost(address_,port_);
    connect(socket,SIGNAL(connected()),
            this,SLOT(actionOfConnectedClient()));
    emit clientSetUp(socket->localAddress(),socket->localPort());
}

void myNetwork::actionOfConnectedClient(){
    qDebug()<<"myNetwork::action0OfConnectedClient";
    qDebug()<<socket->localAddress()<<" "<<socket->localPort();
    connected_flag=true;
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(readData()));
    connect(socket,SIGNAL(disconnected()),
            this,SIGNAL(disconnected()));
    emit connected();
}

void myNetwork::readData(){
    qDebug()<<"myNetwork::readData()";
    QByteArray ba=socket->readAll();
    if (ba.isEmpty()) return ;
    char *data_=ba.data();
    QString string_="";
    while (*data_){
        string_+=QChar(*data_);
        ++data_;
    }
    if (string_.size()){
        qDebug()<<"size:"<<sizeof(string_);
        emit dataRead(string_);
    }
}

void myNetwork::writeData(QString data_){
    qDebug()<<"myNetwork::writeData()";
    qDebug()<<"SS:"<<&data_;
    qDebug()<<"to Write:"<<data_;
    ba.clear();
    ba.append(data_);
    socket->write(ba);
    qDebug()<<"HAHA";
    //emit dataWritten();
}

void myNetwork::disconnectNetwork(){
    qDebug()<<"disconnectNetwork()";
    if (server) delete server;
    server=NULL;
    if (socket) delete socket;
    socket=NULL;
    emit disconnected();
}
