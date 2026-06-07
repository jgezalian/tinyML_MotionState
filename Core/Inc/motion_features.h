#ifndef MOTION_FEATURES_H
#define MOTION_FEATURES_H
#include "motion_meta.h"
#include "motion_sample.h"
#include "sensors.h"

struct axis_accel_features;
struct axis_dps_features;
struct accel_global_features;
struct dps_global_features;

double *MotionFeatures_Extract(Motion_Sample *ordered_window);
double MotionFeatures_GetSample(Motion_Attributes att, Motion_Sample *ordered_window, uint32_t index);
void mean_min_max(Motion_Sample *ordered_window);
void std_dev(Motion_Sample *ordered_window);
void range(struct axis_accel_features *aaf, struct axis_dps_features *adf);
void abs_max(struct axis_accel_features *aaf, struct axis_dps_features *adf);
void delta(Motion_Sample *ordered_window);
void max_neighbor_diff(Motion_Sample *ordered_window);
void min_neighbor_diff(Motion_Sample *ordered_window);
void avg_neighbor_diff(Motion_Sample *ordered_window);
void avg_neighbor_diff(Motion_Sample *ordered_window);
double min_gradient(Motion_Sample *ordered_window, Motion_Attributes att);
double max_gradient(Motion_Sample *ordered_window, Motion_Attributes att);
void set_gradients(Motion_Sample *ordered_window);
void zero_crossings(Motion_Sample *ordered_window);
void sign_check(double *att_zero_cross, double cur, uint8_t *prev_pos, uint8_t *prev_neg,
                uint8_t att_type);
void mag_mean_min_max(Motion_Sample *ordered_window);
void mag_range(struct accel_global_features *agf, struct dps_global_features *dgf);
#endif
