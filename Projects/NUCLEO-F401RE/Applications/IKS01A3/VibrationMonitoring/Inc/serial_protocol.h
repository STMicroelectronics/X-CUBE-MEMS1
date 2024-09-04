/**
  ******************************************************************************
  * @file    serial_protocol.h
  * @author  MEMS Software Solutions Team
  * @brief   header for serial_protocol.c
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported defines --------------------------------------------------------*/
#define Msg_EOF                0xF0
#define Msg_BS                 0xF1
#define Msg_BS_EOF             0xF2

#define Msg_MaxLen             4096

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  Serial message structure definition
 */
typedef struct
{
  uint32_t Len;
  uint8_t Data[Msg_MaxLen];
} Msg_t;

/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t ByteStuffCopyByte(uint8_t *Dest, uint8_t Source);
int32_t ReverseByteStuffCopyByte2(uint8_t Source0, uint8_t Source1, uint8_t *Dest);
int32_t ByteStuffCopy(uint8_t *Dest, Msg_t *Source);
int32_t ReverseByteStuffCopyByte(uint8_t *Source, uint8_t *Dest);
int32_t ReverseByteStuffCopy(Msg_t *Dest, uint8_t *Source);
void CHK_ComputeAndAdd(Msg_t *Msg);
int32_t CHK_CheckAndRemove(Msg_t *Msg);
uint32_t Deserialize(uint8_t *Source, uint32_t Len);
int32_t Deserialize_s32(uint8_t *Source, uint32_t Len);
void Serialize(uint8_t *Dest, uint32_t Source, uint32_t Len);
void Serialize_s32(uint8_t *Dest, int32_t Source, uint32_t Len);
void FloatToArray(uint8_t *Dest, float Data);

#endif /* SERIAL_PROTOCOL_H */
