#include "service.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
Service::Service()
{

}
/**
 * @brief Service::FetchMaxContour
 * @param src
 * @param mode
 * @return
 */
vector<int> Service::FetchMaxContour(Mat src, int mode)
{
    //读取图像
        Mat testmat = src.clone();
        Mat testclone = src.clone();
        Mat matcanny;
        //用于寻找轮廓
        Mat threshold_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        int imax = 0; int maxsize = 0;
        RotatedRect theMinRect;
        RotatedRect theMinEllipse;
        //imshow("原始图像",testmat);
        //大津法找到敏感区域
        threshold(testmat, testmat, 0, 255, cv::THRESH_OTSU);
        //imshow("大津法",testmat);
        //为什么要转换，因为白色是有数据的区域,轮廓是围绕白色区域的
        threshold(testmat, testmat, 0, 255, THRESH_BINARY_INV);
        //imshow("二值", testmat);
        //计算纵向投影
        vector<int> vcol; int itmp = 0;
        for (int i = 0; i < testmat.cols; i++)
        {
            for (int j = 0; j < testmat.rows; j++)
            {
                if (testmat.at<uchar>(j, i))
                {
                    itmp = itmp + 1;
                }
            }

            vcol.push_back(itmp);
            itmp = 0;
        }
        ////对得到的结果进行处理,计算波峰
        //int isum = 0;//一共多少个波峰
        vector<int> vrise;
        if (mode == 1)
        {
            for (int i = 1; i < vcol.size(); i++)
            {
                if (vcol[i - 1] == 0 && vcol[i] > 0)
                {
                    vrise.push_back(i);
                    //cout << "ndoe1 " << i <<"itmp "<<vcol[i]<< endl;
                    //isum = isum+1;
                }
            }
        }
        if (mode == 2)
        {
            for (int i = 1; i < vcol.size() - 1; i++)
            {
                if (vcol[i + 1] == 0 && vcol[i] > 0)
                {
                    vrise.push_back(i);
                    //cout << "ndoe2 " << i << endl;
                    //isum = isum+1;
                }
            }
        }
        cout << "size" << vrise.size() << endl;
        return vrise;
}
/**
 * @brief Service::CheckDir
 * @param Dir
 * @return
 */
int Service::CheckDir(char* Dir)
{
    FILE *fp = NULL;
        char TempDir[200];
        memset(TempDir, '\0', sizeof(TempDir));
        sprintf(TempDir, Dir);
        strcat(TempDir, "\\");
        fp = fopen(TempDir, "w");
        if (!fp)
        {
            if (_mkdir(Dir) == 0)
            {
                return 1;//文件夹创建成功
            }
            else
            {
                return -1;//can not make a dir;
            }
        }
        else
        {
            fclose(fp);
        }
        return 0;
}
/**
 * @brief Service::templateMatching
 * @param dt
 * @param matchMask
 * @return
 */
vector<Point2i> Service::templateMatching(Mat dt,Mat matchMask)
{
       Mat matchResult;
        matchResult.create(Size(dt.cols - matchMask.cols, dt.rows - matchMask.rows), CV_16SC1);
        //----模板匹配找四个定位点，同时得归一化（初始数据范围太大，自己通过image watch 查看）
        matchTemplate(dt, matchMask, matchResult, TM_CCOEFF_NORMED, Mat());
        normalize(matchResult, matchResult, 0, 1, NORM_MINMAX);


        double minValue[4] = { 0 }, maxValue[4]{ 0 };
        vector<Point2i> maxPoint(4), minPoint(4);

        //matchResult.create(Size(dt.cols - matchMask.cols, dt.rows - matchMask.rows), CV_16SC1);
        Mat topleftMask = Mat::zeros(matchResult.size(), CV_8UC1);
        Mat toprightMask = Mat::zeros(matchResult.size(), CV_8UC1);
        Mat botleftMask = Mat::zeros(matchResult.size(), CV_8UC1);
        Mat botrightMask = Mat::zeros(matchResult.size(), CV_8UC1);

        topleftMask(Rect(Point(0, 0), Point(matchResult.cols / 2, matchResult.rows / 2))).setTo(255);
        toprightMask(Rect(Point(matchResult.cols / 2, 0), Point(matchResult.cols, matchResult.rows / 2))).setTo(255);
        botleftMask(Rect(Point(0, matchResult.rows / 2), Point(matchResult.cols / 2, matchResult.rows))).setTo(255);
        botrightMask(Rect(Point(matchResult.cols / 2, matchResult.rows / 2), Point(matchResult.cols, matchResult.rows))).setTo(255);

        Mat topleft = matchResult(Rect(Point(0, 0), Point(matchResult.cols / 2, matchResult.rows / 2)));
        minMaxLoc(topleft, &minValue[0], &maxValue[0], &minPoint[0], &maxPoint[0]);
        //rectangle(dt, maxPoint[0], Point(maxPoint[0].x + matchMask.cols, maxPoint[0].y + matchMask.rows), cvScalar(0, 0, 255));


        vector<Mat> vectorMask;//注意此处如果用vector<Mat> vectorMask(4);对应的下面写法是vectorMask[0]=topleftMask;
        vectorMask.push_back(topleftMask);
        vectorMask.push_back(toprightMask);
        vectorMask.push_back(botleftMask);
        vectorMask.push_back(botrightMask);

        for (size_t i = 0; i < vectorMask.size(); i++)
        {
            cout << i << endl;
            minMaxLoc(matchResult, &minValue[i], &maxValue[i], &minPoint[i], &maxPoint[i], vectorMask[i]);
            //rectangle(dt, maxPoint[i], Point(maxPoint[i].x + matchMask.cols, maxPoint[i].y + matchMask.rows), cvScalar(0, 0, 255));
        }
       // imshow("原图", dt);
        return maxPoint;
}
/**
 * @brief Service::warpPersPective
 * @param in
 * @param matchMask
 * @param out
 * @param maxPoint
 * @return
 */
bool Service::warpPersPective(Mat in,Mat matchMask,Mat &out,vector<Point2i> maxPoint)
{
    cv::Point2f objectivePoints[4], imagePoints[4];

    struct points {
        int x;
        int y;
    }point[5];

    point[0].x = maxPoint[0].x;
        point[0].y = maxPoint[0].y;
        point[1].x = maxPoint[2].x;
        point[1].y = maxPoint[2].y + matchMask.rows;
        point[2].x = maxPoint[1].x + matchMask.cols;
        point[2].y = maxPoint[1].y;
        point[3].x = maxPoint[3].x + matchMask.cols;
        point[3].y = maxPoint[3].y + matchMask.rows;

        double rect_height = sqrt(abs(point[0].x - point[1].x)*abs(point[0].x - point[1].x) +
                abs(point[0].y - point[1].y)*abs(point[0].y - point[1].y));
        double rect_width = sqrt(abs(point[0].x - point[2].x)*abs(point[0].x - point[2].x) +
                abs(point[0].y - point[2].y)*abs(point[0].y - point[2].y));

            imagePoints[0].x = point[0].x; imagePoints[0].y = point[0].y;// top_left
            imagePoints[1].x = point[1].x; imagePoints[1].y = point[1].y;// bottom_Left
            imagePoints[2].x = point[2].x; imagePoints[2].y = point[2].y;// top_Right
            imagePoints[3].x = point[3].x; imagePoints[3].y = point[3].y;// bottom_Right
            double moveValueX = 0.0;
                double moveValueY = 0.0;

                objectivePoints[0].x = 0.0 + moveValueX; objectivePoints[0].y = 0 + moveValueY;
                objectivePoints[1].x = 0.0 + moveValueX; objectivePoints[1].y = rect_height + moveValueY;
                objectivePoints[2].x = rect_width + moveValueX; objectivePoints[2].y = 0.0 + moveValueY;
                objectivePoints[3].x = rect_width + moveValueX; objectivePoints[3].y = rect_height + moveValueY;

                cv::Mat transform = cv::getPerspectiveTransform(objectivePoints, imagePoints);

                cv::warpPerspective(in,
                    out,
                    transform,
                    cv::Size(rect_width, rect_height),
                    cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);
               // cv::imshow("透视变换", out);
                return true;
}
/**
 * @brief Service::checkAnswer
 * @param vroi021
 * @param vroi022
 * @param vroi031
 * @param vroi032
 * @param vroi041
 * @param vroi042
 * @return
 */
Student Service::checkAnswer(vector<int> vroi021,vector<int> vroi022,
                             vector<int> vroi031, vector<int> vroi032,
                             vector<int> vroi041, vector<int> vroi042,Mat local,float accuracy)
{
    Student student;
    double h = ((vroi032[2] - vroi031[2]) + (vroi032[3] - vroi031[3])
            + (vroi032[4] - vroi031[4])) / 3;
        double w = ((vroi042[2] - vroi041[2])+ (vroi042[3] - vroi041[3])
            + (vroi042[4] - vroi041[4]))/3;
        int  titlNum = 0,temp=0,temp1=0,tl=0,dx=0;
            double area = (h+1)*w, blackNum = 0;
            double acc = area*(1-accuracy*0.1);
   cout<<"w "<<w<<" h "<<h<<" area"<<area<<" acc "<<acc<<endl;
            if (vroi031.size() == 27)
                {
                    for (int j = 1; j < vroi031.size() - 1; j++)
                        for (int i = 1; i < vroi041.size() - 1; i++)
                        {
                            //学号部分
                            if (1 <= j&&j <= 10 && 13 <= i&&i <= 16)
                            {
                                //判断是否为涂了
                                blackNum = 0;
                                for (int k = vroi031[j] - 1; k < vroi031[j] + h + 1; k++)
                                {
                                    for (int o = vroi021[vroi021.size()-(18-i)]-1; o < (vroi021[vroi021.size() - (18 - i)] + w+1); o++)
                                    {
                                        if (local.at<uchar>(k, o) == 0)
                                            blackNum++;

                                    }
                                }

                                if (blackNum >= acc)//挑精确度 所涂卡占答案块的比率
                                {
                                    student.id[i - 13][j - 1] = j ;
                                }
                                rectangle(local, Rect(vroi021[vroi021.size() - (18 - i)], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                            }
                            //答案部分
                            else
                            {
                                //听力部分
                                if (10<j&&j <= 16)
                                {
                                    //判断是否为涂了
                                    blackNum = 0;
                                    for (int k = vroi031[j] - 1; k < vroi031[j] + h + 1; k++)
                                    {
                                        for (int o = vroi041[i]-1; o < (vroi041[i] + w+1); o++)
                                        {
                                            if (local.at<uchar>(k, o) == 0)
                                                blackNum++;

                                        }
                                    }

                                    titlNum = (i - 1) / 4 * 6 + j - 10;
                                    cout <<"titl "<<titlNum<<" blacknum " << blackNum << endl;
                                    if (blackNum >= acc)//挑精确度 所涂卡占答案块的比率
                                    {
                                        student.selc[titlNum][(i - 1) % 4]= (i - 1) % 4+1;
                                        //cout << "*******" << (i - 1) % 4 << endl;
                                    }
                                    rectangle(local, Rect(vroi041[i], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                                    tl = titlNum;
                                   // cout << "tl" << tl << endl;
                                }
                                else
                                {       //单项选择
                                    if (16<j&&j <= 21)
                                    {
                                        //判断是否为涂了
                                        blackNum = 0;
                                        for (int k = vroi031[j] - 1; k < vroi031[j] + h + 1; k++)
                                        {
                                            for (int o = vroi041[i]-1; o < (vroi041[i] + w+1); o++)
                                            {
                                                if (local.at<uchar>(k, o) == 0)
                                                    blackNum++;

                                            }
                                        }
                                        titlNum = (i - 1) / 8 * 5 + j - 16 + tl;
                                        cout << "titl " << titlNum << " blacknum " << blackNum << endl;
                                        if (blackNum >= acc)//挑精确度 所涂卡占答案块的比率
                                        {
                                            student.selc[titlNum][(i - 1) % 4] = (i - 1) % 4+1;
                                        }
                                        rectangle(local, Rect(vroi041[i], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                                        dx = titlNum;
                                        //cout << "dx" << dx << endl;
                                    }
                                    //完形填空
                                    if (22 <= j&&j <= 25)
                                    {
                                        //判断是否为涂了
                                        blackNum = 0;
                                        for (int k = vroi031[j] - 1; k < vroi031[j] + h + 1; k++)
                                        {
                                            for (int o = vroi041[i]-1; o < (vroi041[i] + w+1); o++)
                                            {
                                                if (local.at<uchar>(k, o) == 0)
                                                    blackNum++;

                                            }
                                        }
                                        titlNum = (i - 1) / 4 * 4 + j - 21 + dx;
                                        cout << "titl " << titlNum << " blacknum " << blackNum << endl;
                                        if (blackNum >=acc)//挑精确度 所涂卡占答案块的比率
                                        {
                                            student.selc[titlNum][(i - 1) % 4] = (i - 1) % 4+1;
                                        }
                                        rectangle(local, Rect(vroi041[i], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                                    }
                                }
                            }
                        }
                }
                if (vroi031.size() == 16)
                {

                    for (int j = 1; j < vroi031.size() - 1; j++)
                        for (int i = 1; i < vroi041.size() - 1; i++)
                        {
                            //阅读理解
                            if (1 <= j&&j <= 4)
                            {
                                blackNum = 0;
                                for (int k = vroi031[j]-1 ; k < vroi031[j] + h + 1; k++)
                                {
                                    for (int o = vroi041[i]-1; o < (vroi041[i] + w+1); o++)
                                    {
                                        if (local.at<uchar>(k, o) == 0)
                                            blackNum++;

                                    }
                                }
                                cout << "black "<<blackNum << endl;
                                if (i <= 4)
                                    titlNum= (i - 1) /4 * 4 + j;
                                if (7 < i)
                                    titlNum = (i - 4) / 4 * 4 +j;

                                if (blackNum >=acc)//挑精确度 所涂卡占答案块的比率
                                {
                                    if (i <= 4)
                                    {

                                        student.selc[titlNum][(i - 1) % 4] = (i - 1) % 4+1;
                                    }
                                    if (7 < i)
                                    {

                                        student.selc[titlNum][(i - 8) % 4] = (i - 8) % 4+1;
                                    }
                                    cout << "titlnum " << titlNum <<" "<< (i - 1) % 4<<" " <<blackNum<< endl;

                                }
                                cout << "ti " << titlNum << endl;
                                rectangle(local, Rect(vroi041[i], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                                temp = titlNum;
                            }

                            //七选五
                            if (5 <= j&&j <= 14 && i <= 7)
                            {
                                blackNum = 0;
                                for (int k = vroi031[j] - 1; k < vroi031[j] + h + 1; k++)
                                {
                                    for (int o = vroi041[i]-1; o < (vroi041[i] + w+1); o++)
                                    {
                                        if (local.at<uchar>(k, o) == 0)
                                            blackNum++;

                                    }
                                }

                                titlNum = (i - 1) / 7 * 5 + j - 4 + temp;
                                cout <<"titlnum "<< titlNum<< " black " << blackNum << endl;
                                if (blackNum >= acc)//挑精确度 所涂卡占答案块的比率
                                {

                                    student.selc[titlNum][(i - 1) % 7] = (i - 1) % 7+1;

                                }
                                rectangle(local, Rect(vroi041[i], vroi031[j], w, h), Scalar(0, 0, 255), 1);
                            }
                        }
                }
                student.titleNum=titlNum;
                return student;
}

