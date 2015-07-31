#ifndef HANDFINDER_H
#define HANDFINDER_H

#include <opencv2/core/core.hpp>
#include "define.h"

using namespace cv;

extern int gotHand;	// got hand or not
extern int foundCount[HANDNUM];
extern int foundCountFlag[HANDNUM];
extern double averAver[HANDNUM];
extern double aver[HANDNUM];
extern int allFoundFlag;
void imgAver(Mat &img,int flag);
int getHand();

#endif // HANDFINDER_H

