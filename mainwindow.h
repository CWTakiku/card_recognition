#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<direct.h>
#include  "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include<dialog.h>
#include <QTcpServer>
#include <QFile>
#include <QTcpSocket>
#include<vector>
#include<QHostInfo>
#include<QHostAddress>
#include "handle.h"
#include "gradeshow.h"
using namespace std;
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void start();   //开始监听
    void acceptConnection();  //建立连接
    void updateServerProgress();  //更新进度条，接收数据
    void displayError(QAbstractSocket::SocketError socketError);
    void on_startButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString sendContent; //要发送的内容
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区
    StandardAnswer sAnswer;//标准答案
    qint64 sensitivity;
    Dialog mDialog;
};

#endif // MAINWINDOW_H
