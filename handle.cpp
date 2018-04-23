#include "handle.h"

Handle::Handle()
{

}
QString Handle::getAnswer(QString answer)
{
    QString temp;
    int flag=0,flag1=0,num;
    for(int i=1;i<answer.length();i++)
    {
       num=0;
       if(this->isNumber(answer[i])&&!this->isNumber(answer[i-1]))
       {
           if(flag==0)
               formatAnswer.append(" ");
           temp=answer[i];
           if(this->isNumber(answer[i+1]))
           {
               temp+=answer[i+1];
           }

            num=temp.toInt();
           flag=0;flag1=1;
       }
       if(num==1)
           formatAnswer+='@';
       if(this->isAnswer(answer[i])&&!this->isAnswer(answer[i-1])&&!this->isAnswer(answer[i+1])&&flag1!=0)
       {
           formatAnswer+=answer[i];
           flag=1;
       }

    }
     formatAnswer+='@';
    return formatAnswer;
}
bool Handle::isNumber(QChar c)
{
    if('0'<=c&&c<='9')
    {
        return true;
    }
    return false;
}
bool Handle::isAnswer(QChar c)
{
    if('A'<=c&&c<='Z'||'a'<=c&&c<='z')
    {
        return true;
    }
    return false;
}
bool Handle::getGrade(StandardAnswer sAnswer,int mode)
{

    if(mode==1)
    {
        int flag=-1; int num=0;
    for(int i=1;i<formatAnswer.length()-1;i++)
    {
       // qDebug()<<formatAnswer[49];
        if(formatAnswer[i]=='@')
        {
            flag=i+1;
            num++;

        }
        if(formatAnswer[i+1]=='@')
        {
            if(num==1)
            {
           grade=this->countGrade(formatAnswer.mid(flag,i+1-flag),sAnswer.listening,sAnswer.listening_grade);
            //grade=QString("%1").arg(value);
            grade+='@';
            }
            if(num==2)
            {

           grade+=this->countGrade(formatAnswer.mid(flag,i+1-flag),sAnswer.single,sAnswer.single_grade);
           // grade+=QString("%1").arg(value);
            grade+='@';
            }
            if(num==3)
            {
               // qDebug()<<formatAnswer.mid(flag,i+2-flag)<<flag<<i;
           grade+=this->countGrade(formatAnswer.mid(flag,i+1-flag),sAnswer.gestalt,sAnswer.gestalt_grade);
            //grade+=QString("%1").arg(value);
            grade+='@';
            }
            //
            //qDebug()<<grade;

        }
    }
    }
    else
    {

        int flag=-1; int num=0;
    for(int i=1;i<formatAnswer.length()-1;i++)
    {
        if(formatAnswer[i]=='@')
        {
            flag=i+1;
            num++;

        }
        if(formatAnswer[i+1]=='@')
        {

            if(num==1)
            {
           grade=this->countGrade(formatAnswer.mid(flag,i+1-flag),sAnswer.read,sAnswer.read_grade);
            //grade=QString("%1").arg(value);
            qDebug()<<grade;
            grade+='@';
            }
            if(num==2)
            {
               // qDebug()<<formatAnswer.mid(flag,i+1-flag);
           grade+=this->countGrade(formatAnswer.mid(flag,i+1-flag),sAnswer.completion,sAnswer.completion_grade);
            //grade+=QString("%1").arg(value);
            grade+='@';
            }

            /*if(num==3)
            {
           value=this->countGrade(formatAnswer.mid(flag,i+2-flag),sAnswer.gestalt,sAnswer.gestalt_grade);
            grade+=QString::number(value, 10);
            grade+='@';
            }
            */

            //qDebug()<<grade;

        }
    }
    }

   return true;;
}
QString Handle::countGrade(QString s,QString portionAnswer,float value)
{
    float rightgrade=0;
    if(portionAnswer.length()!=s.length())
    {
        qDebug()<<"标准答案数量与检测出的答案数量不符合"<<s.length()<<portionAnswer.length();
        return -1;
    }
    for(int i=0;i<portionAnswer.length();i++)
    {
        if(portionAnswer[i]==s[i])
            rightgrade+=value;
        if(s[i]==' ')
            blankNum++;
    }

    //qDebug()<<QString("%1").arg(rightgrade);
    return QString("%1").arg(rightgrade);
}
QString Handle::getGradeString()
{
    return this->grade;
}
