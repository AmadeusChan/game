#ifndef DIALOGFORCLIENT_H
#define DIALOGFORCLIENT_H

#include <QDialog>
#include <QString>
#include <QHostAddress>

namespace Ui {
class dialogForClient;
}

class dialogForClient : public QDialog
{
    Q_OBJECT

public:
    explicit dialogForClient(QWidget *parent = 0);
    ~dialogForClient();

private:
    Ui::dialogForClient *ui;

private slots:
    void addText(QString string_);

    void on_pushButton_okay_clicked();

    void on_pushButton_delete_clicked();

signals:
    void addressGotten(QHostAddress,int);
};

#endif // DIALOGFORCLIENT_H
