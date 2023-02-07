/**
  *******************************************************************************
  * @file    demo_serial.h
  * @author  MEMS Software Solutions Team
  * @brief   Header for demo_serial.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEMO_SERIAL_H
#define DEMO_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "serial_protocol.h"
#include "serial_cmd.h"
#include "bsp_ip_conf.h"
#include "motion_tl_manager.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t subsec;
  float pressure;
  float humidity;
  float temperature;
  int32_t acceleration_x_mg;
  int32_t acceleration_y_mg;
  int32_t acceleration_z_mg;
  int32_t angular_rate_x_mdps;
  int32_t angular_rate_y_mdps;
  int32_t angular_rate_z_mdps;
  int32_t magnetic_field_x_mgauss;
  int32_t magnetic_field_y_mgauss;
  int32_t magnetic_field_z_mgauss;
} offline_data_t;

/* Exported defines ----------------------------------------------------------*/
#define SENDER_UART  0x01
#define SENDER_USB   0x02
#define SENDER_SPI   0x03

#define DEV_ADDR  50U
#define I2C_DATA_MAX_LENGTH_BYTES  16
#define MIN(A,B) ((A)<(B)?(A):(B))
#define OFFLINE_DATA_SIZE  8

/* Enable sensor masks */
#define PRESSURE_SENSOR       0x00000001U
#define TEMPERATURE_SENSOR    0x00000002U
#define HUMIDITY_SENSOR       0x00000004U
#define UV_SENSOR             0x00000008U /* for future use */
#define ACCELEROMETER_SENSOR  0x00000010U
#define GYROSCOPE_SENSOR      0x00000020U
#define MAGNETIC_SENSOR       0x00000040U

#define STREAMING_MSG_LENGTH  71

/* Algorithm-based defines */
#define CMD_ANGLE_MODE_PITCH_ROLL_GRAVITY_INCLINATION  0x01
#define CMD_ANGLE_MODE_THETA_PSI_PHI                   0x02
#define CMD_CAL_POS_LYING_NORMAL_ON_TABLE              0x03
#define CMD_CAL_POS_LYING_UPSIDEDOWN_ON_TABLE          0x04
#define CMD_CAL_POS_NUCLEO_CONNECTOR_DOWN              0x05
#define CMD_CAL_POS_NUCLEO_CONNECTOR_LEFT              0x06
#define CMD_CAL_POS_NUCLEO_CONNECTOR_UP                0x07
#define CMD_CAL_POS_NUCLEO_CONNECTOR_RIGHT             0x08
#define CMD_GET_CALIBRATION_COEFFICIENTS               0x09
#define CMD_GET_ESTIMATED_MEASUREMENT_TIME             0x0A

#define REQUIRED_DATA  (ACCELEROMETER_SENSOR)

/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t DataLoggerActive;
extern volatile uint32_t SensorsEnabled;
extern volatile uint8_t SensorReadRequest;
extern uint8_t UseOfflineData;
extern offline_data_t OfflineData[OFFLINE_DATA_SIZE];
extern int OfflineDataReadIndex;
extern int OfflineDataWriteIndex;
extern int OfflineDataCount;
extern uint32_t AlgoFreq;

extern MTL_angle_mode_t AngleMode;

/* Exported functions ------------------------------------------------------- */
void BUILD_REPLY_HEADER(TMsg *Msg);
void INIT_STREAMING_HEADER(TMsg *Msg);
void INIT_STREAMING_MSG(TMsg *Msg);
int HandleMSG(TMsg *Msg);

void RTC_DateRegulate(uint8_t y, uint8_t m, uint8_t d, uint8_t dw);
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss);
void Get_PresentationString(char *PresentationString, uint32_t *Length);

#ifdef __cplusplus
}
#endif

#endif /* DEMO_SERIAL_H */

