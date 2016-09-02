#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QHostAddress>
#include <QVector>
#include <QString>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QThread>
#include <QImage>

#include "mynetwork.h"
#include "mygame.h"
#include "dialog.h"
#include "dialogforclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();

private:
    Ui::MainWindow *ui;
    QVector<QString> state;
    QVector<QVector<QRect>> cells;
    myGame *game;
    myNetwork *network;
    QThread *thread_for_network,*thread_for_game;
    Dialog dialog_for_network;
    dialogForClient dialog_for_client;
    bool operation_flag;
    QVector<QString> strings;
    QTimer *timer;
    bool hint_flag;
    int player_at_present;
    QRect high_rect;
    QImage boom;

    void setUpGameConnection();
    void paint();
    void refreshTitle();

signals:
    //void test_client(QHostAddress,int);
    //void test_client_write(QString);
    void toSetUpServer();
    void toSetUpClient(QHostAddress,int);
    void toOperatorGame(QVector<QString>);
    void toWriteDate(QString string_);
    void toSetGameState(QVector<QString>);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

private slots:
    //void on_actionQuit_triggered();
    void serverSetUpAction(QHostAddress address_,int port_);
    void clientSetUpAction(QHostAddress address_,int port_);
    void stopServerListening();
    void refresh(QVector<QString> state_);
    void readData(QString string_);

    void on_actionHint_triggered();
    void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
