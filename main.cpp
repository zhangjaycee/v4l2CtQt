#include <stdio.h>
#include "mainwindow.h"
#include <QApplication>
#include "define.h"
#include "compressivetracker.h"

int imgWidth=IMAGEWIDTH;
int imgHeight=IMAGEHEIGHT;


Rect box[HANDNUM]; // tracking object
char c;
int first_flag=1;
int averInitCount=0;
int ctInitFlag=0;
int x0[HANDNUM];
int y00[HANDNUM];
int dx[HANDNUM];
int dy[HANDNUM];
int rgb_b[HANDNUM]={0},rgb_g[HANDNUM]={0},rgb_r[HANDNUM]={0};

int fpscount=1;

Mat first_frame(imgHeight, imgWidth, CV_8UC3);
Mat frame(imgHeight, imgWidth, CV_8UC3);
Mat fore_frame(imgHeight, imgWidth, CV_8UC3);
Mat current_gray(imgHeight, imgWidth, CV_8UC3);
CompressiveTracker ct[HANDNUM];

VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(IMAGEWIDTH, IMAGEHEIGHT));
VideoCapture reader("VideoTest.avi");


void boxInit()
{
    //box[0] = Rect(450, 170, 130, 150);//Rect(x,y,width,height)
    //box[1] = Rect(70, 170, 130, 150);//Rect(x,y,width,height)
    if(HANDNUM==1){
        box[0] = Rect(150, 70, 60, 70);
    }else if(HANDNUM>1){
        box[0] = Rect(230, 70, 60, 70);//Rect(x,y,width,height)
        box[1] = Rect(30, 70, 60, 70);//Rect(x,y,width,height)
        for(int i=2;i<HANDNUM;i++)
            box[i]=Rect(0,0,10,10);
    }
        for(int i=0;i<HANDNUM;i++){
            x0[i]=box[i].x;
            y00[i]=box[i].y;
            dx[i]=0;
            dy[i]=0;
            rgb_r[i]=255;
        }

}

int main(int argc, char *argv[])
{
    printf("main start\n");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
