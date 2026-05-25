#include "sensors.h"
#include "uart_cmd.h"

// STTS22H 0011 1000
// 0111 0000
// 0x70

// LSM6DSV16X 0110 1011
// 1101 0110
// 0xD6

// LPS22DF 0101 1101
// 1011 1010
// 0xBA

// SHT40-AD1B 0x44
// 0100 0100
// 1000 1000
// 0x88

#define STTS22H_ADD 0x70    // temperature
#define LSM6DSV16X_ADD 0xD6 // IMU
#define LPS22DF_ADD 0xBA    // pressure
#define SHT40_AD1B_ADD 0x88 // temperature & humidity

#define SHT40_AD1B_CRC_POLY 0x31

#define LSM6DSV16X_WHO_AM_I 0x70
#define LPS22DF_WHO_AM_I 0xB4

#define LSM6DSV16X_INT1_CTRL 0x0D
#define LSM6DSV16X_CTRL1 0x10
#define LSM6DSV16X_CTRL2 0x11
#define LSM6DSV16X_CTRL6 0x15
#define LSM6DSV16X_CTRL8 0x17
#define LSM6DSV16X_STATUS_REG 0x1E

#define LSM6DSV16X_OUTX_L_G 0x22
#define LSM6DSV16X_OUTX_H_G 0x23
#define LSM6DSV16X_OUTY_L_G 0x24
#define LSM6DSV16X_OUTY_H_G 0x25
#define LSM6DSV16X_OUTZ_L_G 0x26
#define LSM6DSV16X_OUTZ_H_G 0x27

#define LSM6DSV16X_OUTX_L_A 0x28
#define LSM6DSV16X_OUTX_H_A 0x29
#define LSM6DSV16X_OUTY_L_A 0x2A
#define LSM6DSV16X_OUTY_H_A 0x2B
#define LSM6DSV16X_OUTZ_L_A 0x2C
#define LSM6DSV16X_OUTZ_H_A 0x2D

// linear accel sensitivity
#define LSM6DSV16X_LA_SO 0.061f
// angular rate sensitivity
#define LSM6DSV16X_G_SO 8.75f

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

typedef enum
{
    ID_MISMATCH,
    READ_READY_ERR,
    STATUS_REG_READ_ERR,
    ACCEL_DATA_READ_ERR,
    GYRO_DATA_READ_ERR,
    NONE
} SensorErrors;

typedef enum
{
    LSM6DSV16X,
    LPS22DF,
    SHT40_AD1B
} Sensors;

struct SensorCodes
{
    Sensors Sensor;
    SensorErrors SensorError;
    HAL_StatusTypeDef DeviceReadyStatus;
    HAL_StatusTypeDef WhoAmI_Read;
};

const char *SensorStatuses[4];
const char *const SensorErrorsStr[] = {[ID_MISMATCH] = "ID_MISMATCH",
                                       [NONE] = "NONE",
                                       [READ_READY_ERR] = "READ_READY_ERR",
                                       [STATUS_REG_READ_ERR] = "STATUS_REG_READ_ERR",
                                       [ACCEL_DATA_READ_ERR] = "ACCEL_DATA_READ_ERR",
                                       [GYRO_DATA_READ_ERR] = "GYRO_DATA_READ_ERR"};
const char *const SensorsStr[] = {[LSM6DSV16X] = "LSM6DSV16X", [LPS22DF] = "LPS22DF", [SHT40_AD1B] = "SHT40_AD1B"};
const char *const HAL_StatusStr[] = {
    [HAL_OK] = "HAL_OK", [HAL_ERROR] = "HAL_ERROR", [HAL_BUSY] = "HAL_BUSY", [HAL_TIMEOUT] = "HAL_TIMEOUT"};

// SHT40 returns 6 bytes of data, 2 2-byte temp/humidity and 1 crc for each
uint8_t SHT40_AD1B_RX[6] = {0};
uint8_t SHT40_AD1B_CRC1 = 0xFF;
uint8_t SHT40_AD1B_CRC2 = 0xFF;

// initialize with non-existing code
struct SensorCodes LSM6DSV16X_SensorCodes = {LSM6DSV16X, NONE, -1, -1};
struct SensorCodes LPS22DF_SensorCodes = {LPS22DF, NONE, -1, -1};
struct SensorCodes SHT40_AD1B_SensorCodes = {SHT40_AD1B, NONE, -1, -1};

float LSM6DSV16X_Data[6] = {0, 0, 0, 0, 0, 0};
LSM6DSV16X_Sample _LSM6DSV16X_Sample = {0};

void CALC_SHT40_AD1B_CRC(uint8_t *data, uint8_t *crc, uint8_t num_bytes)
{

    for (int i = 0; i < num_bytes; ++i)
    {
        *crc ^= *(data + i);
        for (int i = 0; i < 8; ++i)
        {
            // leading bit is 1
            if (*crc & 0x80)
            {
                *crc = (*crc << 1) ^ SHT40_AD1B_CRC_POLY;
            }

            else
            {
                *crc <<= 1;
            }
        }
    }
}

void Sensor_StatusToString(struct SensorCodes *sensorCode)
{
    SensorStatuses[0] = SensorsStr[sensorCode->Sensor];
    SensorStatuses[1] = HAL_StatusStr[sensorCode->DeviceReadyStatus];
    SensorStatuses[2] = HAL_StatusStr[sensorCode->WhoAmI_Read];
    SensorStatuses[3] = SensorErrorsStr[sensorCode->SensorError];
}

// check whoami registers
bool Sensors_Init()
{
    uint8_t whoami = 0;

    // LSM6DSV16X
    LSM6DSV16X_SensorCodes.DeviceReadyStatus = HAL_I2C_IsDeviceReady(&hi2c1, LSM6DSV16X_ADD, 3, 100);
    LSM6DSV16X_SensorCodes.WhoAmI_Read = HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, 0x0F, 1, &whoami, 1, 100);

    if (whoami != LSM6DSV16X_WHO_AM_I)
    {
        LSM6DSV16X_SensorCodes.SensorError = ID_MISMATCH;
    }

    if (LSM6DSV16X_SensorCodes.DeviceReadyStatus != HAL_OK || LSM6DSV16X_SensorCodes.WhoAmI_Read != HAL_OK ||
        LSM6DSV16X_SensorCodes.SensorError != NONE)
    {
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // LPS22DF
    LPS22DF_SensorCodes.DeviceReadyStatus = HAL_I2C_IsDeviceReady(&hi2c1, LPS22DF_ADD, 3, 100);
    LPS22DF_SensorCodes.WhoAmI_Read = HAL_I2C_Mem_Read(&hi2c1, LPS22DF_ADD, 0x0F, 1, &whoami, 1, 100);

    if (whoami != LPS22DF_WHO_AM_I)
    {
        LPS22DF_SensorCodes.SensorError = ID_MISMATCH;
    }

    if (LPS22DF_SensorCodes.DeviceReadyStatus != HAL_OK || LPS22DF_SensorCodes.WhoAmI_Read != HAL_OK ||
        LPS22DF_SensorCodes.SensorError != NONE)
    {
        Sensor_StatusToString(&LPS22DF_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // SHT40_AD1B does not have WHOAMI register, use serial #
    uint8_t SHT40_AD1B_SRL_CMD = 0x89;
    SHT40_AD1B_SensorCodes.DeviceReadyStatus =
        HAL_I2C_Master_Transmit(&hi2c1, SHT40_AD1B_ADD, &SHT40_AD1B_SRL_CMD, 1, 100);
    HAL_Delay(20);
    SHT40_AD1B_SensorCodes.WhoAmI_Read = HAL_I2C_Master_Receive(&hi2c1, SHT40_AD1B_ADD, SHT40_AD1B_RX, 6, 100);

    CALC_SHT40_AD1B_CRC(&SHT40_AD1B_RX[0], &SHT40_AD1B_CRC1, 2);
    CALC_SHT40_AD1B_CRC(&SHT40_AD1B_RX[3], &SHT40_AD1B_CRC2, 2);

    // compare against reference crc
    if (SHT40_AD1B_CRC1 != SHT40_AD1B_RX[2] || SHT40_AD1B_CRC2 != SHT40_AD1B_RX[5])
    {
        SHT40_AD1B_SensorCodes.SensorError = ID_MISMATCH;
    }

    if (SHT40_AD1B_SensorCodes.DeviceReadyStatus != HAL_OK || SHT40_AD1B_SensorCodes.WhoAmI_Read != HAL_OK ||
        SHT40_AD1B_SensorCodes.SensorError != NONE)
    {
        Sensor_StatusToString(&SHT40_AD1B_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    UartCmd_PrintSensorInitOk();
    return true;
}

// settings for data reading
bool LSM6DSV16X_Read_Ready()
{
    // LSM6DSV16X accelerometer
    //  set data-ready interrupt on INT1_CTRL
    uint8_t tx_byte = 0x01;
    //
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_INT1_CTRL, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // set high performance mode
    tx_byte = 0x08;
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_CTRL1, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // set full scale to +-2g
    tx_byte = 0x00;
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_CTRL8, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        return false;
    }

    // LSM6DSV16X gyro
    // data ready interrupt on INT1_CTRL
    tx_byte = 0x02;
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_INT1_CTRL, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // set high performance mode
    tx_byte = 0x08;
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_CTRL2, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    // set dps to +-250
    tx_byte = 0x01;
    if (HAL_I2C_Mem_Write(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_CTRL6, 1, &tx_byte, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = READ_READY_ERR;
        Sensor_StatusToString(&LSM6DSV16X_SensorCodes);
        UartCmd_PrintSensorStatus(SensorStatuses);
        return false;
    }

    UartCmd_PrintSensorReadReady("LSM6DSV16X");

    return true;
}

LSM6DSV16X_Sample *LSM6DSV16X_ReadData()
{

    uint8_t AccelDataH;
    uint8_t AccelDataL;
    uint8_t GyroDataH;
    uint8_t GyroDataL;
    uint8_t status = 0;

    if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_STATUS_REG, 1, &status, 1, 100) != HAL_OK)
    {
        LSM6DSV16X_SensorCodes.SensorError = STATUS_REG_READ_ERR;
    }

    // accel data ready
    if (status & 0x01)
    {

        // x-axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTX_L_A, 1, &AccelDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTX_H_A, 1, &AccelDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t ax_raw = (int16_t)(((uint16_t)AccelDataH << 8) | AccelDataL);
        float ax_mg = LSM6DSV16X_LA_SO * (float)ax_raw;
        float ax_g = ax_mg / 1000.0f;
        _LSM6DSV16X_Sample.ax_g = ax_g;
        // LSM6DSV16X_Data[0] = ax_g;

        // y-axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTY_L_A, 1, &AccelDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTY_H_A, 1, &AccelDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t ay_raw = (int16_t)(((uint16_t)AccelDataH << 8) | AccelDataL);
        float ay_mg = LSM6DSV16X_LA_SO * (float)ay_raw;
        float ay_g = ay_mg / 1000.0f;
        // LSM6DSV16X_Data[1] = ay_g;
        _LSM6DSV16X_Sample.ay_g = ay_g;

        // z-axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTZ_L_A, 1, &AccelDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTZ_H_A, 1, &AccelDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = ACCEL_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t az_raw = (int16_t)(((uint16_t)AccelDataH << 8) | AccelDataL);
        float az_mg = LSM6DSV16X_LA_SO * (float)az_raw;
        float az_g = az_mg / 1000.0f;
        // LSM6DSV16X_Data[2] = az_g;
        _LSM6DSV16X_Sample.az_g = az_g;
    }

    // gyro data ready
    if (status & 0x02)
    {
        // x axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTX_L_G, 1, &GyroDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTX_H_G, 1, &GyroDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t mdps_x_raw = (int16_t)(((uint16_t)GyroDataH << 8) | GyroDataL);
        float mdps_x_float = LSM6DSV16X_G_SO * (float)mdps_x_raw;
        float dps_x = mdps_x_float / 1000.0f;
        // LSM6DSV16X_Data[3] = dps_x;
        _LSM6DSV16X_Sample.dps_x = dps_x;

        // y axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTY_L_G, 1, &GyroDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTY_H_G, 1, &GyroDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t mdps_y_raw = (int16_t)(((uint16_t)GyroDataH << 8) | GyroDataL);
        float mdps_y_float = LSM6DSV16X_G_SO * (float)mdps_y_raw;
        float dps_y = mdps_y_float / 1000.0f;
        // LSM6DSV16X_Data[4] = dps_y;
        _LSM6DSV16X_Sample.dps_y = dps_y;

        // z axis
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTZ_L_G, 1, &GyroDataL, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }
        if (HAL_I2C_Mem_Read(&hi2c1, LSM6DSV16X_ADD, LSM6DSV16X_OUTZ_H_G, 1, &GyroDataH, 1, 100) != HAL_OK)
        {
            LSM6DSV16X_SensorCodes.SensorError = GYRO_DATA_READ_ERR;
        }

        // combine data L/H into one 16-bit word and apply sensitivity factor
        int16_t mdps_z_raw = (int16_t)(((uint16_t)GyroDataH << 8) | GyroDataL);
        float mdps_z_float = LSM6DSV16X_G_SO * (float)mdps_z_raw;
        float dps_z = mdps_z_float / 1000.0f;
        // LSM6DSV16X_Data[5] = dps_z;
        _LSM6DSV16X_Sample.dps_z = dps_z;
    }

    return &_LSM6DSV16X_Sample;
}
