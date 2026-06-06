#ifndef MOTION_FEATURES_H
#define MOTION_FEATURES_H
#include "motion_meta.h"
#include "motion_sample.h"
#include "sensors.h"

struct axis_accel_features;
struct axis_dps_features;

double *MotionFeatures_Extract(Motion_Sample *ordered_window);
void mean_min_max(Motion_Sample *ordered_window);
void std_dev(Motion_Sample *ordered_window);
void range(struct axis_accel_features aaf, struct axis_dps_features adf);
void abs_max(struct axis_accel_features aaf, struct axis_dps_features adf);
void delta(Motion_Sample *ordered_window);

#endif
