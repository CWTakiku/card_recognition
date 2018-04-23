#ifndef SERVICE_H
#define SERVICE_H
#include "student.h"
class Service
{

public:
    Service();
    vector<int> FetchMaxContour(Mat src, int mode);
    int CheckDir(char* Dir);
   vector<Point2i> templateMatching(Mat dt,Mat matchMask);
   bool warpPersPective(Mat in,Mat matchMask,Mat &out,vector<Point2i> maxPoint);
   Student checkAnswer(vector<int> vroi021,vector<int> vroi022,
                    vector<int> vroi031,vector<int> vroi032,
                    vector<int> vroi041,vector<int> vroi042,Mat local,float accuracy);

};

#endif // SERVICE_H
