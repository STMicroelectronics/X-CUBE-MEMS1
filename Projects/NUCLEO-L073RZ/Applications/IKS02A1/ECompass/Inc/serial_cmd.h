/**
 *******************************************************************************
 * @file    serial_cmd.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains serial commands code
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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
#define CMD_ChangeSF                   0x07
#define CMD_Start_Data_Streaming       0x08
#define CMD_Stop_Data_Streaming        0x09
#define CMD_Offline_Data               0x10 /* Offline data stream */
#define CMD_Use_Offline_Data           0x11 /* From Msg->Data[3]: uint8_t UseOfflineData (1 ON, 0 OFF) */
#define CMD_Get_App_Info               0x12 /* From Msg->Data[3]: int AlgoFreq; uint8_t RequiredData; */

#define CMD_Set_DateTime               0x0C
#define CMD_Enter_DFU_Mode             0x0E
#define CMD_Reset                      0x0F
#define CMD_Reply_Add                  0x80U

/* Algorithm  CMD  (0x51 - 0x5F) -----------------*/
#define CMD_Algo_Mode                  0x51
#define CMD_Angle_Mode_Cal_Pos         0x52
#define CMD_Calibration_Mode           0x53
#define CMD_Calibration_Reset          0x54

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
