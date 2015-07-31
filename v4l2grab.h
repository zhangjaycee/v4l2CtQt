#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<linux/videodev2.h>
#include<stdlib.h>

extern struct v4l2_fmtdesc fmtdesc;
extern struct v4l2_format fmt,fmtack;
extern struct v4l2_streamparm setfps;
extern struct v4l2_requestbuffers req;
extern struct v4l2_buffer buf;
extern enum   v4l2_buf_type type;
extern unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];//存RGB数据的buffer
extern int fd;
extern struct buffer{
    void * start;
    unsigned int length;
} * buffers;


int init_v4l2(void);//初始化
int v4l2_grab(void);//采集
int close_v4l2(void);



