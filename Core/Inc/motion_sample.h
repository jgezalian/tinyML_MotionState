#ifndef MOTION_SAMPLE_H
#define MOTION_SAMPLE_H
#include "sensors.h"

typedef struct
{
    double a_x;
    double a_y;
    double a_z;
    double dps_x;
    double dps_y;
    double dps_z;
} Motion_Sample;

void MotionBuffer_AddSample(Motion_Sample sample);
bool MotionBuffer_ShouldClassify(void);
void MotionBuffer_MarkClassified(void);
Motion_Sample *MotionBuffer_OrderedWindow(void);

#endif
