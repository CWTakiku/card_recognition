#ifndef GRADESHOW_H
#define GRADESHOW_H

#include <QDialog>

namespace Ui {
class GradeShow;
}

class GradeShow : public QDialog
{
    Q_OBJECT

public:
    explicit GradeShow(QWidget *parent = 0);
    ~GradeShow();

private:
    Ui::GradeShow *ui;
};

#endif // GRADESHOW_H
