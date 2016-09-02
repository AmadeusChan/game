#include "mygame.h"
#include <QDebug>
#include <QThread>

int myGame::BLACK_PLAYER=0;
int myGame::WHITE_PLAYER=1;

QChar myGame::BLACK_CHESS='B';
QChar myGame::WHITE_CHESS='W';
QChar myGame::WARNING='R';
QChar myGame::NOTHING='N';

myGame::myGame(int player_,QObject *parent):QObject(parent),player(player_){
    qDebug()<<"myGame()";
    width=height=15;
    initPointer();
    QString string_="";
    for (int i=0;i<height;++i) string_+=NOTHING;
    state->clear();
    for (int i=0;i<width;++i) state->append(string_);
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
}

myGame::~myGame(){
    qDebug()<<"~myGame()";
    deletePointer();
}

void myGame::initPointer(){
    qDebug()<<"myGame::initPointer";
    state=new QVector<QString>;
}

void myGame::deletePointer(){
    qDebug()<<"myGame::deletePointer";
    delete state;
}

void myGame::gameStart(){
    qDebug()<<"myGame::gameStart()";
    qDebug()<<"thread of game:"<<QThread::currentThreadId();
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            (*state)[i][j]=NOTHING;
        }
    }
    emit gameStateChange(*state);
    emit gameStarted();
}

void myGame::checkDirection(int x_, int y_, int dx_, int dy_,int &len_,int &cnt_){
    //qDebug()<<"myGame:checkDirection()";
    cnt_=0;
    len_=1;
    for (int i=1;;++i){
        int px_=x_+dx_*i;
        int py_=y_+dy_*i;
        if (px_<0 || px_>=width || py_<0 || py_>=height) break;
        if (state->at(px_).at(py_)!=state->at(x_).at(y_)) {
            if (state->at(px_).at(py_)==NOTHING || state->at(px_).at(py_)==WARNING) ++cnt_;
            break;
        }
        flag[px_][py_]=false;
        ++len_;
    }
    for (int i=1;;++i){
        int px_=x_-dx_*i;
        int py_=y_-dy_*i;
        if (px_<0 || px_>=width || py_<0 || py_>=height) break;
        if (state->at(px_).at(py_)!=state->at(x_).at(y_)) {
            if (state->at(px_).at(py_)==NOTHING || state->at(px_).at(py_)==WARNING) ++cnt_;
            break;
        }
        flag[px_][py_]=false;
        ++len_;
    }
}

bool myGame::checkWarningDirection(int x_, int y_, int dx_, int dy_){
    //qDebug()<<"myGame::checkWarningDirection()";
    int len_,cnt_;
    checkDirection(x_,y_,dx_,dy_,len_,cnt_);
    if (len_==3 && cnt_>=2) return true;
    if (len_==4 && cnt_>=1) return true;
    return false;
}

bool myGame::checkWarningState(QChar chess_to_check_){
    //qDebug()<<"myGame:checkWarningState()";
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            flag[i][j]=true;
        }
    }
    int cnt_=0;
    for (int x_=0;x_<width;++x_){
        for (int y_=0;y_<height;++y_){
            if (flag[x_][y_] && state->at(x_).at(y_)==chess_to_check_){
                flag[x_][y_]=false;
                if (checkWarningDirection(x_,y_,1,0)) ++cnt_;
                if (checkWarningDirection(x_,y_,0,1)) ++cnt_;
                if (checkWarningDirection(x_,y_,1,1)) ++cnt_;
                if (checkWarningDirection(x_,y_,1,-1)) ++cnt_;
            }
        }
    }
    return cnt_>=2;
}

void myGame::setWarningPoint(){
    //qDebug()<<"myGame::setWarningPoint()";
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            if (state->at(i).at(j)==WARNING){
                (*state)[i][j]=NOTHING;
            }
        }
    }
    QChar chess_to_check_=player==BLACK_PLAYER?BLACK_CHESS:WHITE_CHESS;
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            if (state->at(i).at(j)==NOTHING){
                (*state)[i][j]=chess_to_check_;
                if (checkWarningState(chess_to_check_)){
                    (*state)[i][j]=WARNING;
                } else (*state)[i][j]=NOTHING;
            }
        }
    }
}

bool myGame::checkResultDirection(int x_, int y_, int dx_, int dy_){
    qDebug()<<"myGame::checkResultDirection()";
    int len_,cnt_;
    checkDirection(x_,y_,dx_,dy_,len_,cnt_);
    if (len_>=5) return true;
    return false;
}

bool myGame::checkGameResult(int x_,int y_){
    qDebug()<<"myGame::checkGameResult";
    bool flag=false;
    if (checkResultDirection(x_,y_,1,0)) flag=true;
    if (checkResultDirection(x_,y_,0,1)) flag=true;
    if (checkResultDirection(x_,y_,1,1)) flag=true;
    if (checkResultDirection(x_,y_,1,-1)) flag=true;
    return flag;
    /*if (flag){
        emit gameEnd();
    }*/
}

void myGame::gameOperate(QVector<QString> strings_){
    qDebug()<<"myGame::gameOperator";
    int x_=strings_.at(0).toInt();
    int y_=strings_.at(1).toInt();
    if (x_<0 || x_>=width) return ;
    if (y_<0 || y_>=height) return ;
    QChar the_state_=state->at(x_).at(y_);
    if (the_state_==BLACK_CHESS || the_state_==WHITE_CHESS) return ;
    if (player==BLACK_PLAYER){
        (*state)[x_][y_]=BLACK_CHESS;
    } else {
        (*state)[x_][y_]=WHITE_CHESS;
    }
    setWarningPoint();
    emit gameStateChange(*state);
    emit gameOperated();
    if (checkGameResult(x_,y_)) {
        //QThread::msleep(300);
        emit gameEnd();
    }
}
/*
void myGame::getGameState(QVector<QString> &strings_){
    qDebug()<<"myGame::getGameState";
    strings_=*state;
    emit gameStateGotten();
}*/
/*
void myGame::getGameResult(QVector<QString> &strings_){

}*/

void myGame::setGameState(QVector<QString> strings_){
    qDebug()<<"myGame::setGameState()";
    (*state)=strings_;
    emit gameStateSet();
}
