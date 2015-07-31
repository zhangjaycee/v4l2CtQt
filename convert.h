
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtGui>
#include <QDebug>

using namespace cv;
QImage MatToQImage(const Mat&);

int yuyv_2_rgb888(void);

bool YV12ToBGR24_OpenCV(unsigned char* pYUV,unsigned char* pBGR24,int width,int height);

void yuv2Mat(void *pYUV, int width, int height);
