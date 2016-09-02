#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setText(QHostAddress address_,int port_);

private:
    Ui::Dialog *ui;

signals:
    void disconnectNetwork();
private slots:
    void on_pushButton_clicked();
};

#endif // DIALOG_H
