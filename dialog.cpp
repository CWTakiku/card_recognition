#include "dialog.h"
#include "ui_dialog.h"
#include<QFileInfo>
#include<QTextStream>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QFile file("Answer.txt");
    if(file.open(QIODevice::ReadOnly))
    {
         QTextStream in(&file);
         QString line = in.readLine();
         /*
         while (!line.isNull()) {

             process_line(line);
             line = in.readLine();
         }*/
         this->Set(line);
    }

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_3_clicked()
{

     reject();
}
StandardAnswer Dialog::getInput()
{
    return this->sdAnswer;
}



void Dialog::on_pushButton_2_clicked()
{

    if(this->Save())
    {
        ui->pushButton_2->setText("保存成功");
    }
}
bool Dialog::Save()
{

    QString rightAnswer=ui->lineEdit->text(); 
    rightAnswer=rightAnswer+ui->lineEdit_6->text();  
    rightAnswer=rightAnswer+'@';
    rightAnswer+=ui->lineEdit_2->text();
    rightAnswer=rightAnswer+ui->lineEdit_7->text();
    rightAnswer=rightAnswer+'@';
    rightAnswer+=ui->lineEdit_3->text();
    rightAnswer=rightAnswer+ui->lineEdit_8->text();
    //qDebug()<<ui->lineEdit_8->text();
    rightAnswer=rightAnswer+'@';
    rightAnswer+=ui->lineEdit_4->text();
    rightAnswer=rightAnswer+ui->lineEdit_9->text();
    rightAnswer=rightAnswer+'@';
    rightAnswer+=ui->lineEdit_5->text();
    rightAnswer=rightAnswer+ui->lineEdit_10->text();
    rightAnswer=rightAnswer+'@';

    this->sdAnswer.listening=ui->lineEdit->text();
    this->sdAnswer.listening_grade=ui->lineEdit_6->text().toFloat();
    this->sdAnswer.single=ui->lineEdit_2->text();
    this->sdAnswer.single_grade=ui->lineEdit_7->text().toFloat();
    this->sdAnswer.gestalt=ui->lineEdit_3->text();
    this->sdAnswer.gestalt_grade=ui->lineEdit_8->text().toFloat();
    this->sdAnswer.read=ui->lineEdit_4->text();
    this->sdAnswer.read_grade=ui->lineEdit_9->text().toFloat();
    this->sdAnswer.completion=ui->lineEdit_5->text();
    this->sdAnswer.completion_grade=ui->lineEdit_10->text().toFloat();
    this->rightAnswer=rightAnswer;

    QFile f("Answer.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //cout << "Open failed." << endl;
        return false;
    }

   QTextStream out(&f);
   out<<this->rightAnswer;
   // QString s1(this->rightAnswer);
    //txtOutput << s1 << endl;
    f.close();
    return true;
}
bool Dialog::Set(QString buffer)
{
    int flag=0,num=0,i;
     QString temp;
    for( i=0;i<buffer.length()-1;i++)
    {
        if('0'<=buffer[i]&&buffer[i]<='9')
        {
            if(num==0)
            {
                ui->lineEdit->setText(buffer.mid(flag,i-flag));
                 temp=buffer[i];
                for(int j=i+1;j<buffer.length()-1;j++)
                {
                    if(buffer[j]!='@')
                        temp+=buffer[j];
                    else
                    {
                        i=j;
                        break;
                    }
                }
                ui->lineEdit_6->setText(temp);
            }
            if(num==1)
            {
                ui->lineEdit_2->setText(buffer.mid(flag,i-flag));
                temp=buffer[i];
                for(int j=i+1;j<buffer.length()-1;j++)
                {
                    if(buffer[j]!='@')
                        temp+=buffer[j];
                    else
                    {
                        i=j;
                        break;
                    }
                }
                ui->lineEdit_7->setText(temp);
            }
            if(num==2)
            {
                ui->lineEdit_3->setText(buffer.mid(flag,i-flag));
                temp=buffer[i];
                for(int j=i+1;j<buffer.length()-1;j++)
                {
                    if(buffer[j]!='@')
                        temp+=buffer[j];
                    else
                    {
                     i=j;
                        break;
                    }
                }
                ui->lineEdit_8->setText(temp);
            }
            if(num==3)
            {
                ui->lineEdit_4->setText(buffer.mid(flag,i-flag));
               temp=buffer[i];
                for(int j=i+1;j<buffer.length()-1;j++)
                {
                    if(buffer[j]!='@')
                        temp+=buffer[j];
                    else
                    {
                        i=j;
                        break;
                    }
                }
                ui->lineEdit_9->setText(temp);
            }
            if(num==4)
            {
                ui->lineEdit_5->setText(buffer.mid(flag,i-flag));
                temp=buffer[i];
                for(int j=i+1;j<buffer.length()-1;j++)
                {
                    if(buffer[j]!='@')
                        temp+=buffer[j];
                    else
                    {
                        i=j;
                        break;
                    }
                }
                ui->lineEdit_10->setText(temp);
            }
             num++;
            flag=i+1;
        }
    }
    return true;
}

void Dialog::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
     ui->label_11->setText("数量:"+QString::number(ui->lineEdit->text().length(),10));
}

void Dialog::on_lineEdit_2_cursorPositionChanged(int arg1, int arg2)
{
     ui->label_12->setText("数量:"+QString::number(ui->lineEdit_2->text().length(),10));
}

void Dialog::on_lineEdit_3_cursorPositionChanged(int arg1, int arg2)
{
     ui->label_13->setText("数量:"+QString::number(ui->lineEdit_3->text().length(),10));
}

void Dialog::on_lineEdit_4_cursorPositionChanged(int arg1, int arg2)
{
     ui->label_14->setText("数量:"+QString::number(ui->lineEdit_4->text().length(),10));
}

void Dialog::on_lineEdit_5_cursorPositionChanged(int arg1, int arg2)
{
     ui->label_15->setText("数量:"+QString::number(ui->lineEdit_5->text().length(),10));
}
