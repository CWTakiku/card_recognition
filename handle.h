#ifndef HANDLE_H
#define HANDLE_H
#include<QString>
#include<standard.h>
#include<QDebug>
class Handle
{
public:
    Handle();
    QString getAnswer(QString aswer);
    bool isNumber(QChar c);
    bool isAnswer(QChar c);
    bool getGrade(StandardAnswer sAnswer,int mode);
    QString  countGrade(QString s,QString portionAnswer,float value);
    QString getGradeString();
    int blankNum=0;
private :
    QString formatAnswer;
    QString grade;

};

#endif // HANDLE_H
