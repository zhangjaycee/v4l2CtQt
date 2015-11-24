#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QPainter>
//#include <image.h>
#include "define.h"
#include "v4l2grab.h"
#include "convert.h"
#include "compressivetracker.h"
#include "handfinder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    if(init_v4l2() == FALSE)
    {
        printf("init error\n");
    }

    boxInit();
    v4l2_grab();
    grabAndShow();
}


void MainWindow::grabAndShow(){

    yuv2Mat(buffers[0].start,imgWidth,imgHeight);
    //new frame grabed,process start
    writer << frame;
    while(1){
        if(first_flag){
            cvtColor(frame, first_frame, CV_RGB2GRAY);
            first_flag=0;
        }
        cvtColor(frame, current_gray, CV_RGB2GRAY);
        absdiff(first_frame,current_gray,fore_frame);
        if(!gotHand){
            getHand();
            if(gotHand){
                ctInitFlag=1;
            }
            break;
        }
        // CT initialization
        if(ctInitFlag){
            for(int i=0;i<HANDNUM;i++)
                ct[i].init(fore_frame, box[i]);
            ctInitFlag=0;
        }
        for(int i=0;i<HANDNUM;i++){
            ct[i].processFrame(fore_frame, box[i]);
            rectangle(frame, box[i], Scalar(rgb_b[i],rgb_g[i],rgb_r[i]));
        }
        flip(frame, frame, 1);
        break;
    }
    //imshow("ok?",frame);
    //waitKey();

    qImg=MatToQImage(frame);
    ui->label->setPixmap(QPixmap::fromImage(qImg));
    ui->label->resize(ui->label->pixmap()->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{
    //printf("framecount:%d\n",fpscount++);
    ioctl(fd, VIDIOC_DQBUF, &buf);
    grabAndShow();
    ioctl(fd, VIDIOC_QBUF, &buf);
}

