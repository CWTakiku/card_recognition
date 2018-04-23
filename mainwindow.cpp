#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "picture.h"
#include "service.h"
#include "standard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    mDialog.Save();
    sAnswer=mDialog.getInput();

    //qDebug()<<mDialog.rightAnswer;
    //mDialog.show();

    sensitivity=10;
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    vector<QString> ips;
    ui->label->setText(QString::number(sensitivity,10));
    //获取IP地址
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    //qDebug() << "IP Address:" << info.addresses();
    foreach(QHostAddress address, info.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
           ips.push_back(address.toString());
    }
    ui->label_11->setText("IP地址: "+ips[ips.size()-1]);
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{

  mDialog.exec();
  sAnswer=mDialog.getInput();
  //qDebug()<<"answer"<<rightAnswer;

}
void MainWindow::start() //开始监听
{
    ui->startButton->setEnabled(false);
    bytesReceived =0;
    if(!tcpServer.listen(QHostAddress::Any,6667 ))
    {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }

    ui->serverStatusLabel->setText(tr("监听"));
}

void MainWindow::acceptConnection()  //接受连接
{
    tcpServerConnection = new QTcpSocket;
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    ui->serverStatusLabel->setText(tr("接受连接"));
    tcpServer.close();



}

void MainWindow::updateServerProgress()  //更新进度条，接收数据
{

    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);

    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息

        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)&& (fileNameSize == 0))
        { //接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            qDebug()<<"totalBytes = "<<totalBytes<<"*******"<<";fileNameSize ="<<fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
            qDebug()<<"receiverBytes = "<<bytesReceived;
        }

        if((tcpServerConnection->bytesAvailable() >= fileNameSize)
               && (fileNameSize != 0))
        {  //接收文件名，并建立文件
            //in >> fileName;
            fileName="temp.png";
            ui->serverStatusLabel->setText(tr("接收文件 %1 _").arg(fileName));
            //bytesReceived += fileNameSize;
            qDebug()<<fileName;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
             qDebug()<<"filename = "<<fileName;
        }

        else return;
    }


    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件

        bytesReceived += tcpServerConnection->bytesAvailable();
        //qDebug()<<"sehenyu"<<tcpServerConnection->bytesAvailable()<<bytesReceived;
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);
    //qDebug()<<"byterec"<<bytesReceived;
    //更新进度条
    if(bytesReceived == totalBytes)
    { //接收数据完成时
        //qDebug()<<"wanc";
        ui->serverProgressBar->setValue(0);
        sensitivity=fileNameSize;
       ui->label->setText(QString::number(sensitivity,10));

       ui->pushButton_2->click();
        //qDebug()<<"send"<<sendContent;
       tcpServerConnection->write(sendContent.toStdString().c_str(),strlen(sendContent.toStdString().c_str()));
        tcpServerConnection->close();
        localFile->close();
        ui->startButton->setEnabled(true);
        ui->serverStatusLabel->setText(tr("receive%1 is success!").arg(fileName));
        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        ui->startButton->click();
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError) //错误处理
{
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
    ui->serverProgressBar->reset();
    ui->serverStatusLabel->setText(tr("Server is ready!"));
    ui->startButton->setEnabled(true);
}
//开始监听
void MainWindow::on_startButton_clicked()
{
        start();
}

//扫描阅卷
void MainWindow::on_pushButton_2_clicked()
{
   cout<<"read"<<sensitivity;
   ui->textEdit->setPlainText("");
    Service mService;
    Picture mPicture;
    Mat src,dt,matchMask;

    src=imread("temp.png");
    //imshow("src",src);
    //旋转
    Mat src1,src2;
    transpose(src, src1);
    flip(src1, src2, 1);
    //imshow("srcsrc",src2);
    matchMask=imread("moban.png");
    //imshow("src2",src2);
    cv::resize(src2, dt, Size(800, 1000));

    //imshow("dt",dt);
    vector<Point2i> findpoint= mService.templateMatching(dt,matchMask);
    Mat Perspective;
    mService.warpPersPective(dt,matchMask,Perspective,findpoint);
    Mat local;
    mPicture.localThreshold(Perspective,local);
    //imshow("local1",local);
    mPicture.colorReversal(local);
    //imshow("local",local);

    char topstr[]= "top.png",rightstr[]= "right.png",bottomstr[]="bottom.png";
    mPicture.getROI(local, Rect(0, 0, local.cols, 0.85*matchMask.rows),
                    topstr);
    mPicture.getROI(local, Rect(local.cols - 0.85*matchMask.cols,  0, 0.85*matchMask.cols, local.rows),
                    rightstr);
    mPicture.getROI(local, Rect(0, local.rows - 0.85*matchMask.rows,
                                local.cols, 0.85*matchMask.rows), bottomstr);
        Mat top = imread(topstr,0);
        Mat right = imread(rightstr, 0);
        Mat bottom = imread(bottomstr, 0);
           //去噪
            int blackArea = 30;//阀值
            mPicture.deNoise(top, blackArea);
            mPicture.deNoise(right, blackArea);
            mPicture.deNoise(bottom, blackArea);
            //旋转
                Mat dst, dst2, dst3;
                transpose(right, dst);
                flip(dst, dst2, 1);
                flip(dst, dst3, 0);
    vector<int> vroi021= mService.FetchMaxContour(top,1);
    vector<int> vroi022= mService.FetchMaxContour(top,2);
    vector<int> vroi031 = mService.FetchMaxContour(dst3, 1);
    vector<int> vroi032 = mService.FetchMaxContour(dst3, 2);
    vector<int> vroi041 = mService.FetchMaxContour(bottom, 1);
    vector<int> vroi042 = mService.FetchMaxContour(bottom, 2);
    Student student=mService.checkAnswer(vroi021,vroi022,vroi031,
                                         vroi032,vroi041,vroi042,local,sensitivity*0.5);
    //imshow("local",local);
    QString  temp;
    int k=1;
    if(student.titleNum==50)
    {
        ui->textEdit->append("  ");
        ui->textEdit->append("StudentID: ");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (student.id[i][j] !=0)
                   temp.append(QString::number(student.id[i][j]-1, 10));
            }
        }
         ui->textEdit->append(temp);
         temp="";



        for (int i = 1; i <=student.titleNum; i++)
            {
               if(i!=23&&i!=24&&i!=33&&i!=34)
               {
                  if(temp=="")
                    k=1;
                 temp.append(QString::number(k++, 10));
                 temp.append(": ");
                  if(i==1)
                  {
                   ui->textEdit->append("  ");
                   ui->textEdit->append("The listening test: ");
                  }
                  if(i==25)
                  {
                   ui->textEdit->append("  ");
                   ui->textEdit->append("Single option: ");

                  }
                  if(i==35)
                  {
                    ui->textEdit->append("  ");
                    ui->textEdit->append("Gestalt: ");

                  }
                for (int j = 0; j < 7; j++)
                {

                    if (student.selc[i][j] != 0)
                    {

                        switch (student.selc[i][j])
                        {
                        case 1: temp.append("A");
                            break;
                        case 2: temp.append("B");
                            break;
                        case 3: temp.append("C");
                            break;
                        case 4: temp.append("D");
                            break;
                        case 5: temp.append("E");
                            break;
                        case 6: temp.append("F");
                            break;
                        case 7: temp.append("G");
                            break;
                        }

                    }
                }
                temp.append(" ");
               if(i==22)
               {
                   ui->textEdit->append(temp);
                   temp="";
               }
               if(i==32)
               {
                   ui->textEdit->append(temp);
                   temp="";
               }
               if(i==49)
               {
                   ui->textEdit->append(temp);
                   temp="";
               }
            }
        }
    }
    else
    {
        for (int i = 1; i <=student.titleNum; i++)
            {
               if(i!=16)
                {
                   if(temp=="")
                     k=1;
                  temp.append(QString::number(k++, 10));
                  temp.append(": ");
                  if(i==1)
                  {   ui->textEdit->append("  ");
                      ui->textEdit->append("Reading comprehension ");

                  }
                  if(i==22)
                  {
                      ui->textEdit->append("  ");
                      ui->textEdit->append("Seven selected five: ");

                  }
                  for (int j = 0; j < 7; j++)
                  {

                      if (student.selc[i][j] != 0)
                      {

                          switch (student.selc[i][j])
                          {
                          case 1: temp.append("A");
                              break;
                          case 2: temp.append("B");
                              break;
                          case 3: temp.append("C");
                              break;
                          case 4: temp.append("D");
                              break;
                          case 5: temp.append("E");
                              break;
                          case 6: temp.append("F");
                              break;
                          case 7: temp.append("G");
                              break;
                          }

                      }
                  }
                  temp.append(" ");
                  if(i==21)
                  {
                      ui->textEdit->append(temp);
                      temp="";
                  }
                  if(i==26)
                  {
                      ui->textEdit->append(temp);
                      temp="";
                  }
                }
            }
    }
 ui->startButton->click();
 ui->pushButton_3->click();

}
//统计分数
void MainWindow::on_pushButton_3_clicked()
{
    int mode=0,num=0;
    float sumgrade=0;
    QString readstring=ui->textEdit->toPlainText();
   // qDebug()<<readstring;
    Handle handle;
    qDebug()<<handle.getAnswer(readstring);
   // qDebug()<<handle.getAnswer(readstring).length();
    if(handle.getAnswer(readstring).length()<=60)
        mode=2;
    else
        mode=1;
   if(handle.getGrade(sAnswer,mode))
   {

       QString gradestring= handle.getGradeString();
       ui->label_12->setText("未检出答案数量："+QString::number(handle.blankNum,10));
       sendContent=QString::number(handle.blankNum,10);
       sendContent+='@';
       qDebug()<<gradestring;
       QString tempgrade;
       for(int i=0;i<gradestring.length();i++)
       {
           if(mode==1)
           {
               if(gradestring[i]!='@')
               {
                   tempgrade.append(gradestring[i]);
               }
               else
               {
                   if(num==0)
                   {
                       ui->label_6->setText("听力分数: "+tempgrade);              

                   }
                   if(num==1)
                   {
                       ui->label_7->setText("单项选择分数: "+tempgrade);

                   }
                   if(num==2)
                   {
                       ui->label_8->setText("完形填空分数: "+tempgrade);
                    }
                   sendContent+=tempgrade;
                   sendContent+='@';
                   sumgrade+=tempgrade.toFloat();
                   tempgrade="";
                   num++;


               }

           }
           else
           {

                   if(gradestring[i]!='@')
                   {
                       tempgrade.append(gradestring[i]);
                   }
                   else
                   {
                       if(num==0)
                       {

                           ui->label_6->setText("阅读理解: "+tempgrade);
                           //qDebug()<<tempgrade;

                       }
                       if(num==1)
                       {
                           ui->label_7->setText("补全对话: "+tempgrade);
                            ui->label_8->setText("");

                       }
                       sendContent+=tempgrade;
                       sendContent+='@';
                       sumgrade+=tempgrade.toFloat();
                       tempgrade="";
                       num++;

                   }

           }
       }
       sendContent+=QString("%1").arg(sumgrade);
       sendContent+='@';
         ui->label_9->setText("该页面总分: "+QString("%1").arg(sumgrade));

      // GradeShow show;
       //show.exec();
   }

}
//减小
void MainWindow::on_pushButton_4_clicked()
{
    if(sensitivity>0)
    {
        sensitivity--;
        ui->label->setText(QString::number(sensitivity,10));
    }
}
//增加
void MainWindow::on_pushButton_5_clicked()
{
    if(sensitivity<19)
    {
        sensitivity++;
        ui->label->setText(QString::number(sensitivity,10));
    }
}
