#include "picture.h"

Picture::Picture()
{
this->visit[801][801]={0};
}
/**
 * @brief Picture::getROI
 * @param image
 * @param rect
 * @param s
 * @return
 */
int Picture::getROI(Mat image, Rect rect, char *s)
{
    Mat img = image.clone();
        Mat roi;
        double cols = img.cols, rows = img.rows;
        //ROI越界，返回
        if (cols - rect.x < rect.width || rows - rect.y < rect.height)
            return -1;

        roi = img(Rect(rect.x, rect.y, rect.width, rect.height));

        rectangle(img, rect, Scalar(0, 0, 255), 2);
        //imshow("SignROI", img);
        image.copyTo(img);    //ROI和它的父图像指向同一块缓冲区，经次操作消除 标记ROI的矩形框
                              //string name = s;
                             // cout <<"111"<< s<< endl;
        imwrite(s, roi);
        //localThreshold(s);
        //imshow("ROI", roi);
        return 0;
}
/**
 * @brief Picture::deNoise
 * @param src
 * @param area
 * @return
 */
bool Picture::deNoise(Mat &src, int area)
{
    int width = src.cols*src.channels(), height = src.rows;
        //cout << "width " << width << " height " << height << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j <width; j++)
            {
                //cout << src.at<uchar>(i, j) << endl;
                if (src.at<uchar>(i, j) == 0 && !visit[j][i])
                {
               int x=j,y=i;

               queue<Point> Q;
                   //八个方向
                   int dir[][2] = { { 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };
                   list<Point> mList;
                   //Mat img = src.clone();
                   //imshow("src1", src);
                   int cols = src.cols*src.channels(), rows = src.rows;
                   int tempx, tempy, blackNum = 0;
                   Q.push(Point(x, y));
                   visit[x][y] = true;
                   mList.push_back(Point(x, y));
                   while (!Q.empty())
                   {

                       Point p = Q.front();
                       Q.pop();
                       for (int i = 0; i < 8; i++)
                       {
                           tempx = p.x + dir[i][0];
                           tempy = p.y + dir[i][1];
                           //cout << "tempx "<<tempx<<" tempy "<<tempy << endl;
                           if (tempx < cols&&tempx >= 0 && tempy < rows&&tempy >= 0)
                           {
                               if (src.at<uchar>(tempy, tempx) == 0 && !visit[tempx][tempy])
                               {
                                   blackNum++;
                                   Q.push(Point(tempx, tempy));
                                   visit[tempx][tempy] = true;
                                   mList.push_back(Point(tempx, tempy));

                               }
                           }
                       }
                   }

                   if (blackNum < area)
                   {
                       //cout << "blacknum " << blackNum << endl;
                       list<Point>::iterator k;
                       //cout << "size " << mList.size() << endl;
                       for (k = mList.begin(); k != mList.end(); ++k)
                       {
                           Point p = *k;
                           //cout << "x " << p.x << " y " << p.y << endl;
                           src.at<uchar>(p.y, p.x) = 255;
                       }
                   }

                }
            }
        }
        //imshow("de",src);

        return true;
}
bool Picture::localThreshold(Mat src,Mat &local)
{
       Mat gray;
        cvtColor(src, gray, COLOR_BGR2GRAY);
        //imshow("灰度图", gray);

        int blockSize = 25;
        int constValue = 10;
        cv::adaptiveThreshold(gray, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
        return true;
}
bool Picture::colorReversal(Mat &local)
{
    int height,width;
    height = local.rows;
    width = local.cols* local.channels();   // 列项要乘通道数
    //图像反转
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                local.at<uchar>(i, j) = 255 - local.at<uchar>(i, j);   // 每一个像素反转
            }
        }

     return true;
}
