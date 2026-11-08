#ifndef SM_H
#define SM_H

typedef enum {
    NO_EVENT,
    EVENT_BOOT,
    EVENT_BOOT_SUCCESS,
    EVENT_SENSOR_INIT,
    EVENT_SENSOR_INIT_SUCCESS,
    EVENT_DATA_READ,
    EVENT_CHECK_BUFFER,
    EVENT_CLASSIFY,
    EVENT_ERROR,
} Events;


void sm_dispatch(Events event);

#endif
