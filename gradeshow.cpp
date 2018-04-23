#include "gradeshow.h"
#include "ui_gradeshow.h"

GradeShow::GradeShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradeShow)
{
    ui->setupUi(this);
}

GradeShow::~GradeShow()
{
    delete ui;
}
