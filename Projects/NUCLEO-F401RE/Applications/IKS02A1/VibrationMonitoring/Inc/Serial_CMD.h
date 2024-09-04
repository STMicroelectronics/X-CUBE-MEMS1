/**
  ******************************************************************************
  * @file    Serial_CMD.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains serial commands code
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* GENERIC  CMD  (0x00 - 0x0F) -------------------*/
#define CMD_Ping                       0x01
#define CMD_Read_PresString            0x02
#define CMD_NACK                       0x03
#define CMD_CheckModeSupport           0x04
#define CMD_UploadXX                   0x05
#define CMD_Start_Data_Streaming       0x08
#define CMD_Stop_Data_Streaming        0x09

#define CMD_Set_DateTime               0x0C
#define CMD_Enter_DFU_Mode             0x0E
#define CMD_Reset                      0x0F

#define CMD_Reply_Add                  0x80U
#define CMD_SF_Data                    0x7D

/* Msg_t.Data[2] */
#define CMD_SUBCMD_SENSOR              0x50

/* Msg_t.Data[3] */
#define SUBCMD_GET_SENSOR_NAME         0x01
/* receive: Msg_t.Data[5]...:  Sensor Name */
#define SUBCMD_READ_REGISTER           0x02
/* send:    Msg_t.Data[5]: Register
   receive: Msg_t.Data[5]: Register; Msg_t.Data[6]: Value */
#define SUBCMD_WRITE_REGISTER          0x03
/* send:    Msg_t.Data[5]: Register; Msg_t.Data[6]: Value
   receive: Msg_t.Data[5]: Register; Msg_t.Data[6]: Value */
#define SUBCMD_GET_FS_LIST             0x04
/* receive: Msg_t.Data[5]: int32_t Count; Msg_t.Data[9]: int32_t value1 ... */
#define SUBCMD_SET_FS                  0x05
/* send:    Msg_t.Data[5]: int32_t full scale
   receive: Msg_t.Data[5]: int32_t selected full scale */
#define SUBCMD_GET_ODR_LIST            0x06
/* receive: Msg_t.Data[5]: int32_t Count; Msg_t.Data[9]: float value1 ... */
#define SUBCMD_SET_ODR                 0x07
/* send:    Msg_t.Data[5]: float ODR
   receive: Msg_t.Data[5]: float selected ODR */

/* All following commands will have Msg_t.Data[4]: SUBCMD_SENSOR_ACC */
#define SUBCMD_SET_MAGNITUDE           0x0D
/* send:    Msg_t.Data[5]: byte magnitude (1,..,7)
   receive: Msg_t.Data[5]: byte magnitude */
#define SUBCMD_SET_SENSING_AXIS        0x0E
/* send:    Msg_t.Data[5]: byte axis (1,2,3)
   receive: Msg_t.Data[5]: byte axis */
#define SUBCMD_SET_OP_MODE             0x0F
/* send:    Msg_t.Data[5]: byte mode (1,2,3)
   receive: Msg_t.Data[5]: byte mode */
#define SUBCMD_SET_FFT_SAMPLES         0x10
/* send:    Msg_t.Data[5]: byte samples (1,2,3)
   receive: Msg_t.Data[5]: byte samples */
#define SUBCMD_SET_HP_FILTER           0x11
/* send:    Msg_t.Data[5]: byte onoff (1,2)
   receive: Msg_t.Data[5]: byte onoff (1,2) */
#define SUBCMD_SET_ODR_INDEX           0x12
/* send:    Msg_t.Data[5]: byte index of requested ODR
   receive: Msg_t.Data[5]: byte index of set ODR */
#define SUBCMD_SET_FS_INDEX            0x13
/* send:    Msg_t.Data[5]: byte index of requested FS
   receive: Msg_t.Data[5]: byte index of set FS */
#define SUBCMD_GET_SENSOR_LIST         0x14
/* receive: Msg_t.Data[5]: comma separated list of available sensors */
#define SUBCMD_SET_SENSOR_INDEX        0x15
/* send:    Msg_t.Data[5]: byte index of requested sensor
   receive: Msg_t.Data[5]: byte index of set sensor */
#define SUBCMD_GET_OP_MODE             0x16
/* receive: Msg_t.Data[5]: byte mode (1,2,3) */
#define SUBCMD_RESTART                 0x17
#define SUBCMD_GET_FFT_SAMPLES_LIST    0x18
#define SUBCMD_CLEAR_PEAK              0x1B
#define SUBCMD_GET_WND_LIST            0x1C
/* receive: Msg_t.Data[5]: comma separated list of available windows */
#define SUBCMD_SET_WND                 0x1D
/* send:    Msg_t.Data[5]: byte window index
   receive: Msg_t.Data[5]: byte selected window */

/* Msg_t.Data[4] */
#define SUBCMD_SENSOR_ACC              0x01
#define SUBCMD_SENSOR_GYR              0x02
#define SUBCMD_SENSOR_MAG              0x03
#define SUBCMD_SENSOR_TMP              0x04
#define SUBCMD_SENSOR_HUM              0x05
#define SUBCMD_SENSOR_PRS              0x06

/* Algorithm  CMD  (0x51 - 0x5F) -----------------*/

/* ENVIRONMENTAL  CMD  (0x60 - 0x6F) -------------*/
#define CMD_PRESSURE_Init              0x60
#define CMD_HUMIDITY_TEMPERATURE_Init  0x62

/* INERTIAL  CMD  (0x70 - 0x7F) ------------------*/
#define CMD_ACCELERO_GYRO_Init         0x76
#define CMD_MAGNETO_Init               0x7A

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_CMD_H */
