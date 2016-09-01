#include "mygame.h"

int myGame::BLACK_PLAYER=0;
int myGame::WHITE_PLAYER=1;

QChar myGame::BLACK_CHESS='B';
QChar myGame::WHITE_CHESS='W';
QChar myGame::WARNING='R';
QChar myGame::NOTHING='N';

myGame::myGame(int player_,QObject *parent):abstractGame(parent),player(player_){
    width=height=15;
    initPointer();
    QString string_="";
    for (int i=0;i<height;++i) string_+=NOTHING;
    state->clear();
    for (int i=0;i<width;++i) state->append(string_);
}

myGame::~myGame(){
    deletePointer();
}

void myGame::initPointer(){
    state=new QVector<QString>;
}

void myGame::deletePointer(){
    delete state;
}

void myGame::gameStart(const QVector<QString> &strings_){
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            (*state)[i][j]=NOTHING;
        }
    }
    emit gameStateChange(*state);
}

bool myGame::checkWarningDirection(int x_, int y_, int dx_, int dy_){

}

bool myGame::checkWarningState(QChar chess_to_check_){
    bool flag_[width][height];
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            flag_[i][j]=true;
        }
    }
    int cnt_=0;
    for (int x_=0;x_<width;++x_){
        for (int y_=0;y_<height;++y_){
            if (flag_[x_][y_] && state->at(x_).at(y_)==chess_to_check_){
                if (checkWarningDirection(x_,y_,1,0)) ++cnt;
                if (checkWarningDirection(x_,y_,0,1)) ++cnt;
                if (checkWarningDirection(x_,y_,1,1)) ++cnt;
                if (checkWarningDirection(x_,y_,1,-1)) ++cnt;
            }
        }
    }
    return cnt_>=2;
}

void myGame::setWarningPoint(){
    for (int i=0;i<width;++i){
        for (int j=0;j<height;++j){
            if (state->at(i).at(j)==WARNING){
                (*state)[i][j]=NOTHING;
            }
        }
    }
    QChar chess_to_check_=player==WHITE_PLAYER?BLACK_CHESS:WHITE_CHESS;
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

void myGame::checkGameResult(int x_,int y_){

}

void myGame::gameOperate(const QVector<QString> &strings_){
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
    checkGameResult(x_,y_);
    emit gameStateChange(*state);
}
