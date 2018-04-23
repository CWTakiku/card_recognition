#ifndef DIALOG_H
#define DIALOG_H
#include<standard.h>
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    StandardAnswer getInput();
    bool Save();
    bool Set(QString);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();



    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_2_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_3_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_4_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_5_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::Dialog *ui;
    QString rightAnswer;
    StandardAnswer sdAnswer;



};

#endif // DIALOG_H
