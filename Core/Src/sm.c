#include "sm.h"
#include "main.h"
#include "motion_sample.h"
#include "model.h"
#include "motion_features.h"
#include "sensors.h"
#include "uart_cmd.h"

typedef enum
{
    NO_STATE,
    BOOT,
    SENSOR_INIT,
    DATA_READ,
    CLASSIFY,
    DEGRADED,
    RECOVERY
} States;

const char *const EventsStr[] = {[NO_EVENT] = "NO_EVENT",
                                 [EVENT_BOOT] = "EVENT_BOOT",
                                 [EVENT_BOOT_SUCCESS] = "EVENT_BOOT_SUCESS",
                                 [EVENT_SENSOR_INIT] = "EVENT_SENSOR_INIT",
                                 [EVENT_DATA_READ] = "EVENT_DATA_READ",
                                 [EVENT_CHECK_BUFFER] = "EVENT_CHECK_BUFFER",
                                 [EVENT_CLASSIFY] = "EVENT_CLASSIFY",
                                 [EVENT_ERROR] = "ERROR"};

static States cur_state = NO_STATE;
SensorReadStatus read_status = TO_READ;

void sm_dispatch(Events event)
{
    switch (cur_state)
    {
    case NO_STATE:
        if (event != EVENT_BOOT)
        {
            UartCmd_PrintGeneral("Unexpected event during NO_STATE: ");
            UartCmd_PrintGeneral(EventsStr[event]);
            cur_state = DEGRADED;
            break;
        }
        cur_state = BOOT;
        break;

    case BOOT:
        if (event == EVENT_ERROR)
        {
            cur_state = DEGRADED;
            break;
        }

        if (event == EVENT_BOOT_SUCCESS)
        {
            cur_state = SENSOR_INIT;
        }

        break;

    case SENSOR_INIT:
        if (event == EVENT_ERROR)
        {
            cur_state = DEGRADED;
            break;
        }

        if (event == EVENT_SENSOR_INIT)
        {
            Attempt_Sensors_Init();
            break;
        }

        if (event == EVENT_SENSOR_INIT_SUCCESS)
        {
            cur_state = DATA_READ;
        }
        break;

    case DATA_READ:
        if (event == EVENT_ERROR)
        {
            cur_state = DEGRADED;
            break;
        }
        if (event == EVENT_DATA_READ)
        {
            read_status = TO_READ;
            read_status = LSM6DSV16X_ReadData();
            if (read_status == READ_FAIL)
            {
                cur_state = DEGRADED;
                break;
            }
            if (read_status == READ_SUCCESS)
            {
                Motion_Sample_Load();
                MotionBuffer_AddSample(_Motion_Sample);
                break;
            }
        }

        if (event == EVENT_CHECK_BUFFER)
        {
            if(!MotionBuffer_ShouldClassify())
            {
                break;
            }

            _BufferReadyToClassify = READY;
            cur_state = CLASSIFY;
        }
        break;

    case CLASSIFY:
        if (event == EVENT_ERROR)
        {
            cur_state = DEGRADED;
            break;
        }

        if (event == EVENT_CLASSIFY)
        {
            Motion_Sample *ordered_window = MotionBuffer_OrderedWindow();

            _features = MotionFeatures_Extract(ordered_window);

            int32_t class_id = MotionModel_Predict(_features);
            const char *class_name = MotionModel_ClassName(class_id);
            UartCmd_PrintPrediction(class_name);
            MotionBuffer_MarkClassified();
            _BufferReadyToClassify = NOT_READY;
            cur_state = DATA_READ;
        }

        break;

    case DEGRADED:
	UartCmd_PrintGeneral("degraded");

    default:
        break;
    }
}
