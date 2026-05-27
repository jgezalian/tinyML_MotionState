#include "uart_cmd.h"
#include "led.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static uint8_t rx_byte;
static volatile uint8_t pending_cmd;
static volatile bool cmd_ready;

void UartCmd_Init()
{
    rx_byte = 0;
    pending_cmd = 0;
    cmd_ready = false;
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void UartCmd_RxCompleteCallback(UART_HandleTypeDef *huart)
{
    pending_cmd = rx_byte;
    cmd_ready = true;
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void UartCmd_ProcessPending(void)
{
    if (!cmd_ready)
    {
        return;
    }
    uint8_t cmd = pending_cmd;
    cmd_ready = false;
    switch (cmd)
    {
    case '0':
        Led_SetMode(LED_OFF);
        UartCmd_PrintStatus();
        break;
    case '1':
        Led_SetMode(LED_ON);
        UartCmd_PrintStatus();
        break;
    case '2':
        Led_SetMode(LED_BLINK_SLOW);
        UartCmd_PrintStatus();
        break;
    case '3':
        Led_SetMode(LED_BLINK_FAST);
        UartCmd_PrintStatus();
        break;
    case 'f':
        uint32_t cur_blink_interval = Led_GetBlinkInterval();
        if (cur_blink_interval > 50)
            Led_SetBlinkInterval(cur_blink_interval - 50);
        UartCmd_PrintStatus();
        break;
    case 's':
        Led_SetBlinkInterval(Led_GetBlinkInterval() + 50);
        UartCmd_PrintStatus();
        break;
    case 'p':
        UartCmd_PrintStatus();
        break;
    case '?':
    case 'h':
        UartCmd_PrintHelp();
        break;
    case '\r':
    case '\n':
        break;
    default:
        UartCmd_PrintHelp();
        break;
    }
}

void UartCmd_PrintGeneral(char *str)
{
    char tx_buffer[strlen(str) + 1];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), str);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}
void UartCmd_PrintStatus()
{
    char tx_buffer[96];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), "mode: %s, blink_interval: %lu\r\n", Led_ModeToString(Led_GetMode()),
                     Led_GetBlinkInterval());
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintSensorStatus(const char **SensorStatuses)
{

    char tx_buffer[96];
    int n = snprintf(tx_buffer, sizeof(tx_buffer),
                     "\r\nSensor: %s\r\nDevice Ready: %s\r\nWhoAmI_Read: %s\r\nSensor Error: %s\r\n", SensorStatuses[0],
                     SensorStatuses[1], SensorStatuses[2], SensorStatuses[3]);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintSensorInitOk()
{
    char *str = "SENSOR INIT OK \r\n";
    char tx_buffer[strlen(str) + 1];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), str);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintSensorReadReady(char *sensor_name)
{
    char *str = "%s READY TO READ DATA \r\n";
    char tx_buffer[strlen(str) + strlen(sensor_name) + 1];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), str, sensor_name);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintLSM6DSV16XData(float *data)
{
    char *str = "\r\naccel_x: %.4f accel_y: %.4f accel_z: %.4f \r\n\
	   \r\ngyro_x: %.4f gyro_y: %.4f gyro_z: %.4f\r\n";
    char tx_buffer[128];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), str, data[0], data[1], data[2], data[3], data[4], data[5]);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintLPS22DFData(float data)
{
    char *str = "\r\npressure: %.4f\r\n";
    char tx_buffer[32];
    int n = snprintf(tx_buffer, sizeof(tx_buffer), str, data);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
}

void UartCmd_PrintSensorDataCSV(const LSM6DSV16X_Sample *LSM6DSV16X_Sample, const float LPS22DFSample)
{
    char *str = "%lu,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r\n";
    uint32_t timestamp = HAL_GetTick();
    char tx_buffer[160];
    float ax_g = LSM6DSV16X_Sample->ax_g;
    float ay_g = LSM6DSV16X_Sample->ay_g;
    float az_g = LSM6DSV16X_Sample->az_g;
    float dps_x = LSM6DSV16X_Sample->dps_x;
    float dps_y = LSM6DSV16X_Sample->dps_y;
    float dps_z = LSM6DSV16X_Sample->dps_z;
    int n =
        snprintf(tx_buffer, sizeof(tx_buffer), str, timestamp, ax_g, ay_g, az_g, dps_x, dps_y, dps_z, LPS22DFSample);

    if (n > 0 && n < sizeof(tx_buffer))
    {

        HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, (uint16_t)n, 100);
    }
}

void UartCmd_PrintHelp(void)
{
    const char *help = "\r\nCommands: \r\n"
                       "0: LED off\r\n"
                       "1: LED on\r\n"
                       "2: LED slow blink\r\n"
                       "3: Led fast blink\r\n"
                       "f: Increase blink rate\r\n"
                       "s: Decrease blink rate\r\n"
                       "p: Print status\r\n"
                       "h/?: help\r\n";

    HAL_UART_Transmit(&huart2, (uint8_t *)help, (uint16_t)strlen(help), 100);
}
