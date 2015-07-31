#include <opencv2/core/core.hpp>
#include "compressivetracker.h"

using namespace cv;

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#define TRUE 1
#define FALSE 0


#define YUV            "/home/jaycee/Desktop/v4l2Yuv.yuv"

#define  IMAGEWIDTH  320
#define  IMAGEHEIGHT   240
#define HANDNUM 1

extern int fpscount;
extern int imgWidth;
extern int imgHeight;


extern Rect box[HANDNUM]; // tracking object
extern string video;

extern char c;
extern int first_flag;
extern int averInitCount;
extern int ctInitFlag;
extern int x0[HANDNUM];
extern int y00[HANDNUM];
extern int dx[HANDNUM];
extern int dy[HANDNUM];
extern int rgb_b[HANDNUM],rgb_g[HANDNUM],rgb_r[HANDNUM];

extern Mat first_frame;
extern Mat frame;
extern Mat skinmask;
extern Mat fore_frame;
extern Mat current_gray;
extern CompressiveTracker ct[HANDNUM];


void boxInit();

/*
INCLUDEPATH += /usr/local/include\
/usr/local/include/opencv\
/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so\
/usr/local/lib/libopencv_core.so\
/usr/local/lib/libopencv_imgproc.so

INCLUDEPATH += /usr/local/arm/opencv/include\
/usr/local/arm/opencv/include/opencv\
/usr/local/arm/opencv/include/opencv2

LIBS += /usr/local/arm/opencv/lib/libopencv_highgui.so\
/usr/local/arm/opencv/lib/libopencv_core.so\
/usr/local/arm/opencv/lib/libopencv_imgproc.so
*/
