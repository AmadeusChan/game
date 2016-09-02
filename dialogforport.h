#ifndef DIALOGFORPORT_H
#define DIALOGFORPORT_H

#include <QDialog>

namespace Ui {
class dialogForPort;
}

class dialogForPort : public QDialog
{
    Q_OBJECT

public:
    QString text();
    explicit dialogForPort(QWidget *parent = 0);
    ~dialogForPort();

private:
    Ui::dialogForPort *ui;

private slots:
    void addText(QString string_);
    void on_pushButton_delete_clicked();
    void on_pushButton_clicked();
};

#endif // DIALOGFORPORT_H
