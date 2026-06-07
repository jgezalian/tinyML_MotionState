#include "model.h"
#include "rnd_clf.h"

#include "motion_meta.h"

static const char *MOTION_CLASS_NAMES[N_CLASSES] = {"bumps",          "clockwise_circle",
                                                    "constant_speed", "ctr_clockwise_circle",
                                                    "hard_accel",     "hard_decel",
                                                    "left_turn",      "left_u_turn",
                                                    "right_turn",     "right_u_turn",
                                                    "smooth_accel",   "smooth_decel",
                                                    "stationary",     "weave"};

int32_t MotionModel_Predict(const double features[N_FEATURES])
{
    return rnd_f_predict(features, N_FEATURES);
}

int32_t MotionModel_PredictProba(const double features[N_FEATURES], float probabilities[N_CLASSES])
{
    return rnd_f_predict_proba(features, N_FEATURES, probabilities, N_CLASSES);
}

const char *MotionModel_ClassName(int32_t class_id)
{
    if (class_id < 0 || class_id >= N_CLASSES)
    {
        return "unknown";
    }

    return MOTION_CLASS_NAMES[class_id];
}
