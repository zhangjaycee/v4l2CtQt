
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<linux/videodev2.h>
#include<stdlib.h>
#include"define.h"


int fd;//设备号
static struct   v4l2_capability cap;//视频设备支持的操作模式
struct v4l2_fmtdesc fmtdesc;
struct v4l2_format fmt,fmtack;
struct v4l2_streamparm setfps;
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
enum v4l2_buf_type type;

struct buffer{
    void * start;
    unsigned int length;
} * buffers;

unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];//存RGB数据的buffer



int init_v4l2(void)
{
    int i;
    int ret = 0;
    char * FILE_VIDEO="/dev/video4";
    printf("%s\n",FILE_VIDEO);
    //打开设备
    if ((fd = open(FILE_VIDEO, O_RDWR)) == -1){
        printf("Error opening V4L interface\n");
        return (FALSE);
    }

    //查询capability
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1){//是否能查询
        printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
        return (FALSE);
    }else{//打印查询的摄像头信息
        printf("driver:\t\t%s\n",cap.driver);
        printf("card:\t\t%s\n",cap.card);
        printf("bus_info:\t%s\n",cap.bus_info);
        printf("version:\t%d\n",cap.version);
        printf("capabilities:\t%x\n",cap.capabilities);

        if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE){
             printf("Device %s: supports capture.\n",FILE_VIDEO);
        }
        if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING){
            printf("Device %s: supports streaming.\n",FILE_VIDEO);
        }
    }

    //列举摄像头所支持像素格式
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format:\n");
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1){
        printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }

    //设置像素格式。一般的USB摄像头都会支持YUYV，有些还支持其他的格式。
    //通过前一步对摄像头所支持像素格式查询，下面需要对格式进行设置。
    //命令为VIDIOC_S_FMT，通过结构体v4l2_format把图像的像素格式设置为V4L2_PIX_FMT_YUYV，
    //高度和宽度设置为IMAGEHEIGHT和IMAGEWIDTH。
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.height = IMAGEHEIGHT;
    fmt.fmt.pix.width = IMAGEWIDTH;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1){
        printf("Unable to set format\n");
        return FALSE;
    }

    //为了确保设置的格式作用到摄像头上，再通过命令VIDIOC_G_FMT将摄像头设置读取回来。
    if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
    {
        printf("Unable to get format\n");
        return FALSE;
    }else{
        printf("fmt.type:\t\t%d\n",fmt.type);
        printf("pix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
        printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
        printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
        printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);
        imgHeight=fmt.fmt.pix.height;
        imgWidth=fmt.fmt.pix.width;
    }

     //set fps
     setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
     setfps.parm.capture.timeperframe.numerator = 1;
     setfps.parm.capture.timeperframe.denominator = 15;
     /////////////////////
     if(ioctl(fd, VIDIOC_S_PARM, &setfps) == -1)
     {
         printf("Unable to set frame rate\n");
         return FALSE;
     }
     else
     {
         printf("set fps OK!\n");
     }

     if(ioctl(fd, VIDIOC_G_PARM, &setfps) == -1)
     {
         printf("Unable to get frame rate\n");
         return FALSE;
     }
     else
     {
         printf("get fps OK!\n");
         printf("timeperframe.numerator:\t%d\n",setfps.parm.capture.timeperframe.numerator);
         printf("timeperframe.denominator:\t%d\n",setfps.parm.capture.timeperframe.denominator);

     }
     ///////////////////
     printf("init %s \t[OK]\n",FILE_VIDEO);

     return TRUE;
}


int v4l2_grab(void)
{
    unsigned int n_buffers;

    //request for 4 buffers
    //申请4个buffer
    req.count=2;
    req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory=V4L2_MEMORY_MMAP;
    if(ioctl(fd,VIDIOC_REQBUFS,&req)==-1)
    {
        printf("request for buffers error\n");
    }

    //mmap for buffers
    //映射到用户空间
    buffers = (struct buffer *)malloc(req.count*sizeof(*buffers) );
    if (!buffers)
    {
        printf ("Out of memory\n");
        return(FALSE);
    }

    for (n_buffers = 0; n_buffers < req.count; n_buffers++)
    {
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        //query buffers
        if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
        {
            printf("query buffer error\n");
            return(FALSE);
        }

        buffers[n_buffers].length = buf.length;
        //map
        buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ |PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (buffers[n_buffers].start == MAP_FAILED)
        {
            printf("buffer map error\n");
            return(FALSE);
        }
    }

    //queue
    for (n_buffers = 0; n_buffers < req.count; n_buffers++)
    {
        buf.index = n_buffers;
        ioctl(fd, VIDIOC_QBUF, &buf);
    }
    //开始采集
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl (fd, VIDIOC_STREAMON, &type);
    //
    //   ioctl(fd, VIDIOC_DQBUF, &buf);
    //
    printf("grab yuyv OK\n");
    return(TRUE);
}



int close_v4l2(void)
{
    ioctl(fd, VIDIOC_STREAMOFF, &type);
    if(fd != -1){
        close(fd);
        return (TRUE);
    }
    return (FALSE);
}
