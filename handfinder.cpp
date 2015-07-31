
//#include "handfinder.h"
#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "define.h"
using namespace cv;

int gotHand=0;	// got hand or not
int foundCount[HANDNUM]={0};
int foundCountFlag[HANDNUM]={0};
int allFoundFlag=0;
double averAver[HANDNUM]={0};
double aver[HANDNUM]={0};

void imgAver(Mat &img,int flag)
{
    int i,j,k,i0,j0,it,jt,width,height;//循环变量
    for(k=0;k<HANDNUM;k++){
        if(flag==1){
            it=box[k].y+box[k].height;
            jt=box[k].x+box[k].width;
            width=box[k].width;
            height=box[k].height;
            i0=box[k].y;
            j0=box[k].x;
        }else{
            it=height=img.rows;
            jt=width=img.cols;
            i0=0;
            j0=0;
        }
        //int step=img.widthStep/sizeof(uchar);
        uchar *data=(uchar*)img.data;

        aver[k]=0.0;

        for(i=i0;i<it;i++){
            for(j=j0;j<jt;j++){
               // printf("%d",data[i*])
               if(data[i*img.cols+j]>10){
                   aver[k]+=1;
               }
            }
        }
        aver[k]=1.0*aver[k]/(height*width);
    }
}

int getHand()
{
    for(int i=0;i<HANDNUM;i++)
    rectangle(frame, box[i], Scalar(rgb_b[i],rgb_g[i],rgb_r[i]));
    flip(frame, frame, 1);

    //imshow("kakka",frame);
    //waitKey();
    imgAver(fore_frame,1);

    if(averInitCount!=16){
        if(averInitCount<15){
            for(int i=0;i<HANDNUM;i++)
                averAver[i]+=aver[i];
            averInitCount++;
        }else if(averInitCount==15){
            for(int i=0;i<HANDNUM;i++)
                  averAver[i]/=15;
              averInitCount++;
              first_flag=1;
        }
        return 0;
    }
    for(int i=0;i<HANDNUM;i++){
        if(aver[i]>0.1&& aver[i]>averAver[i]){
            if(foundCountFlag[i]){
                foundCount[i]++;
            }
            foundCountFlag[i]=1;
        }else{
            foundCountFlag[i]=0;
            foundCount[i]=0;
        }
        if(foundCount[i]>0){
            rgb_g[i]=255;
        }else{
            rgb_g[i]=0;
        }
        allFoundFlag=1;
        for(int j=0;j<HANDNUM;j++){
            if((foundCount[j]<=10))
                allFoundFlag=0;
        }
        if(allFoundFlag){
            printf("i have got hands~!\n");
            gotHand=1;
            for(int j=0;j<HANDNUM;j++)
                rgb_r[j]=0;
        }
    }
    return 0;
}
