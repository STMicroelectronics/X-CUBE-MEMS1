/**
 *******************************************************************************
 * @file    Serial_CMD.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains serial commands code
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0077, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0077
  *
  ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H

/* Exported constants --------------------------------------------------------*/
#define DATALOG_EXT_MODE     101


/**********  GENERIC  CMD  (0x00 - 0x0F) **********/
#define CMD_Ping                                                                0x01U
#define CMD_Read_PresString                                                     0x02U
#define CMD_NACK                                                                0x03U
#define CMD_CheckModeSupport                                                    0x04U
#define CMD_UploadAR                                                            0x05U
#define CMD_UploadCP                                                            0x06U
#define CMD_Start_Data_Streaming                                                0x08U
#define CMD_Stop_Data_Streaming                                                 0x09U
#define CMD_StartDemo                                                           0x0AU
#define CMD_Sleep_Sec                                                           0x0BU
#define CMD_Set_DateTime                                                        0x0CU
#define CMD_Get_DateTime                                                        0x0DU
#define CMD_Enter_DFU_Mode                                                      0x0EU
#define CMD_Reset                                                               0x0FU
#define CMD_Enable_Disable_Sensor                                               0x10U

#define CMD_Sensor                                                              0x50U

#define CMD_Reply_Add                                                           0x80U

/****************************************************/

#endif /* SERIAL_CMD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
