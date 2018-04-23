#ifndef PICTURE_H
#define PICTURE_H


class Picture
{
private:
    bool visit[801][801];
public:
    Picture();
    int getROI(Mat image, Rect rect, char *s);
    bool deNoise(Mat &src, int area);
    bool localThreshold(Mat src,Mat &local);
    bool colorReversal(Mat &src);
};

#endif // PICTURE_H
