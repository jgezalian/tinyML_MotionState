#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>
#include "motion_meta.h"

int32_t MotionModel_Predict(const double features[N_FEATURES]);

int32_t MotionModel_PredictProba(
    const double features[N_FEATURES],
    float probabilities[N_CLASSES]
);

const char *MotionModel_ClassName(int32_t class_id);

#endif
