#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPoint>
#include <QLine>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QRect>
#include <QDesktopWidget>
#include <algorithm>
#include <QStringList>
#include <QInputDialog>
#include <QThread>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hint_flag=false;
    QRect screen_=(QApplication::desktop())->screenGeometry();
    this->resize(screen_.width()/2,screen_.width()/2);
    state.clear();
    for (int i=0;i<15;++i){
        QString string_="";
        for (int j=0;j<15;++j) string_+="N";
        state.append(string_);
    }
    /*state[1][2]=QChar('B');
    state[1][3]=QChar('W');
    state[2][3]=QChar('R');*/
    network=new myNetwork;
    qDebug()<<"main thread:"<<QThread::currentThreadId();
    /*thread_for_network=new QThread;
    network->moveToThread(thread_for_network);
    thread_for_network->start();*/

    connect(this,SIGNAL(toSetUpClient(QHostAddress,int)),
            network,SLOT(setUpClient(QHostAddress,int)));
    connect(this,SIGNAL(toSetUpServer()),
            network,SLOT(setUpServer()));

    //emit toSetUpServer();

    connect(network,SIGNAL(clientSetUp(QHostAddress,int)),
            this,SLOT(clientSetUpAction(QHostAddress,int)));
    connect(network,SIGNAL(serverSetUp(QHostAddress,int)),
            this,SLOT(serverSetUpAction(QHostAddress,int)));


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

    connect(&dialog_for_network,SIGNAL(disconnectNetwork()),
            network,SLOT(disconnectNetwork()));
    connect(&dialog_for_network,SIGNAL(disconnectNetwork()),
            this,SLOT(stopServerListening()));

    //connect(network,&myNetwork::connected,game,&myGame::gameStart);
    connect(network,&myNetwork::connected,[=](){
        qDebug()<<"connected!";
        dialog_for_network.close();
        QMessageBox::information(this,tr("CONNECTED!"),tr("connected!"));
        qDebug()<<"GAME:"<<game;
        //game->gameStart();
    });

    connect(&dialog_for_client,&dialogForClient::addressGotten,[=](QHostAddress address_,int port_){
        qDebug()<<address_;
    });
    connect(&dialog_for_client,SIGNAL(addressGotten(QHostAddress,int)),
            network,SLOT(setUpClient(QHostAddress,int)));
    connect(network,SIGNAL(clientSetUp(QHostAddress,int)),
            this,SLOT(clientSetUpAction(QHostAddress,int)));
    /*connect(this,SIGNAL(toWriteDate(QString)),
            network,SLOT(writeData(QString)));*/
    connect(this,SIGNAL(toWriteDate(QString)),
            network,SLOT(writeData(QString)));
    connect(network,SIGNAL(dataRead(QString)),
            this,SLOT(readData(QString)));


    /*timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),
            this,SLOT(update()));
    timer->start(300);*/
    boom.load(":/boom/boom");
}

void MainWindow::setUpGameConnection(){
    //thread_for_game=new QThread;
    //game->moveToThread(thread_for_game);
    //thread_for_game->start();
    connect(game,SIGNAL(gameStateChange(QVector<QString>)),
            this,SLOT(refresh(QVector<QString>)));
    /*connect(network,SIGNAL(connected()),
            game,SLOT(gameStart()));*/
    connect(this,SIGNAL(toOperatorGame(QVector<QString>)),
            game,SLOT(gameOperate(QVector<QString>)));
    connect(game,&myGame::gameStateChange,[=](QVector<QString> state_){
        state=state_;
        QString string_="GAMESTATE-";
        for (int i=0;i<15;++i) string_+=(state.at(i)+"-");
        qDebug()<<"TT"<<&string_;
        emit toWriteDate(string_);
        update();
    });
    connect(this,SIGNAL(toSetGameState(QVector<QString>)),
            game,SLOT(setGameState(QVector<QString>)));
    connect(game,&myGame::gameEnd,[=](){
        operation_flag=false;
        QString string_="END-";
        emit toWriteDate(string_);
        qDebug()<<"!!!";
        QMessageBox::information(this,tr("YOU WIN!"),tr("Congratulation! You WIN the game!"));
        qDebug()<<"???";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    /*delete game;
    delete network;
    delete thread_for_game;
    delete thread_for_network;*/
}

void MainWindow::init(){
    QStringList items_;
    items_<<tr("Server Mode")<<tr("Client Mode");
    bool okay_;
    QString item_;
    item_=QInputDialog::getItem(this,tr("SELECT MODE"),tr("please select a mode:"),items_,0,false,&okay_);
    if (!( okay_ && item_.size() )) {
        //qApp->quit();
        /*this->close();
        thread_for_network->quit();
        qApp->quit();*/
        qDebug()<<"close()";
    } else {
        qDebug()<<item_;
        game=new myGame((item_==tr("Server Mode"))?0:1);
        qDebug()<<"game:"<<game;
        setUpGameConnection();
        if (item_==tr("Server Mode")){
            operation_flag=true;
            player_at_present=1;
            emit toSetUpServer();
        } else {
            operation_flag=false;
            player_at_present=1;
            dialog_for_client.exec();
        }
        refreshTitle();
    }
}

void MainWindow::paint(){
    qDebug()<<"MainWindow:aint:p";
    int len_=std::min(this->height(),this->width())/16;
    /*QPoint left_top(len_,len_);
    QPoint right_bottom(len_*15,len_*15);
    QPoint right_top(len_*15,len_);
    QPoint left_bottom(len_,len_*15);*/
    QPainter painter_(this);
    QVector<QRect> vector_;
    int delta_=len_/8;
    cells.clear();
    for (int i=0;i<15;++i){
        vector_.clear();
        for (int j=0;j<15;++j){
            vector_.append(QRect(len_/2+len_*j+delta_,len_/2+len_*i+delta_,len_-delta_*2,len_-delta_*2));
        }
        cells.append(vector_);
    }
    qDebug()<<cells.size()<<" "<<cells.at(0).size();
    painter_.setPen(QPen(Qt::white));
    painter_.setBrush(QBrush(Qt::white));
    painter_.drawRect(QRect(0,0,this->width(),this->height()));
    /*for (int i=0;i<15;++i){
        for (int j=0;j<15;++j){
            painter_.drawRect(cells.at(i).at(j));
        }
    }*/
    painter_.setPen(QPen(Qt::black));
    for (int i=1;i<=15;++i){
        painter_.drawLine(QPoint(len_,len_*i),QPoint(len_*15,len_*i));
        painter_.drawLine(QPoint(len_*i,len_),QPoint(len_*i,len_*15));
    }
    for (int i=0;i<15;++i){
        for (int j=0;j<15;++j){
            if (state.at(i).at(j)==QChar('B')){
                painter_.setPen(QPen(Qt::black));
                painter_.setBrush(QBrush(Qt::black));
                painter_.drawEllipse(cells[i][j]);
            }
            if (state.at(i).at(j)==QChar('W')){
                painter_.setPen(QPen(Qt::black));
                painter_.setBrush(QBrush(Qt::white));
                painter_.drawEllipse(cells[i][j]);
            }
            if (state.at(i).at(j)==QChar('R') && operation_flag && hint_flag){
                painter_.setPen(QPen(Qt::red));
                painter_.setBrush(QBrush(Qt::transparent));
                painter_.drawImage(cells[i][j],boom);
                //painter_.drawEllipse(cells[i][j]);
            }
        }
    }
    /*if (high_rect.x()!=-1){
        painter_.setPen(QPen(Qt::black));
        if (player_at_present==0){
            painter_.setBrush(QBrush(Qt::white));
        } else {
            painter_.setBrush(QBrush(Qt::black));
        }
        painter_.drawEllipse(high_rect);
    }*/
}

void MainWindow::paintEvent(QPaintEvent *event){
    paint();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button()==Qt::LeftButton){
        int x_=-1,y_=-1;
        bool flag_=false;
        for (int i=0;i<15;++i){
            for (int j=0;j<15;++j){
                if (cells[i][j].contains(event->pos())){
                    x_=i,y_=j;
                    flag_=true;
                    break;
                }
            }
            if (flag_) break;
        }
        qDebug()<<x_<<" "<<y_;
        if (x_!=-1 && y_!=-1 && state.at(x_).at(y_)!=QChar('W') && state.at(x_).at(y_)!=QChar('B')){
            if (operation_flag){
                qDebug()<<"gogo";
                strings.clear();
                QString string_;
                string_.setNum(x_);
                strings.append(string_);
                string_.setNum(y_);
                strings.append(string_);
                operation_flag=false;
                player_at_present^=1;
                refreshTitle();
                hint_flag=false;
                emit toOperatorGame(strings);
            }
        }
    }
    QMainWindow::mousePressEvent(event);
}
/*
void MainWindow::on_actionQuit_triggered()
{
    this->close();
}*/

void MainWindow::serverSetUpAction(QHostAddress address_, int port_){
    qDebug()<<"serverSetUpAction()";
    qDebug()<<address_;
    qDebug()<<port_;
    dialog_for_network.setText(address_,port_);
    dialog_for_network.show();
    connect(&dialog_for_network,SIGNAL(disconnectNetwork()),
            this,SLOT(stopServerListening()));
}

void MainWindow::stopServerListening(){
    dialog_for_network.close();
}

void MainWindow::clientSetUpAction(QHostAddress address_, int port_){
    qDebug()<<"MainWindow::clientSetUpAction";
    qDebug()<<address_;
    qDebug()<<port_;
}

void MainWindow::refresh(QVector<QString> state_){
    state=state_;
    update();
}

void MainWindow::readData(QString string_){
    qDebug()<<"readData";
    qDebug()<<string_;
    strings.clear();
    QString the_string_="";
    for (int i=0;i<string_.size();++i){
        if (string_.at(i)==QChar('-')){
            strings.append(the_string_);
            the_string_="";
        } else {
            the_string_+=string_.at(i);
        }
    }
    qDebug()<<strings.size();
    for (int i=0;i<strings.size();++i){
        qDebug()<<strings.at(i);
    }
    if (strings.at(0)=="GAMESTATE"){
        for (int i=1;i<16;++i){
            state[i-1]=strings.at(i);
            qDebug()<<"#@$%@#$@#:"<<strings.at(i).size();
        }
        /*for (int i=0;i<15;++i){
            for (int j=0;j<15;++j){
                if (state[i][j]==QChar('R')){
                    state[i][j]=QChar('N');
                }
            }
        }*/
        emit toSetGameState(state);
        update();
        operation_flag=true;
        player_at_present^=1;
        refreshTitle();
    }
    if (string_.contains("END")){
        operation_flag=false;
        QMessageBox::information(this,tr("YOU LOSE!"),tr("Such a pity.You lose the game."));
    }
}

void MainWindow::on_actionHint_triggered()
{
    hint_flag=true;
    update();
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
/*
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (!high_rect.contains(event->pos())){
        high_rect.setX(-1);
        for (int i=0;i<15;++i){
            for (int j=0;j<15;++j){
                if (cells.at(i).at(j).contains(event->pos())){
                    high_rect=cells.at(i).at(j);
                }
            }
        }
        paint();
    }/*
    high_rect.setX(-1);
    for (int i=0;i<15;++i){
        for (int j=0;j<15;++j){
            if (cells.at(i).at(j).contains(event->pos())){
                high_rect=cells.at(i).at(j);
            }
        }
    }
    QMainWindow::mouseMoveEvent(event);
    QPaintEvent event_(high_rect);
    if (high_rect.x()!=-1) paintEvent(&event_);*/
//}

void MainWindow::refreshTitle(){
    QString title_="(";
    if (player_at_present==0) title_+="WHITE"; else title_+="BLACK";
    title_+=")";
    this->setWindowTitle(title_);
}
