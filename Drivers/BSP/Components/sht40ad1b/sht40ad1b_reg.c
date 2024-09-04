/**
  ******************************************************************************
  * @file    sht40ad1b_reg.c
  * @author  Sensors Software Solution Team
  * @brief   SHT40AD1B driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "sht40ad1b_reg.h"

/**
  * @defgroup  SHT40AD1B
  * @brief     This file provides a set of functions needed to drive the
  *            sht40ad1b sensor.
  * @{
  *
  */

static uint8_t crc_calculate(const uint8_t *data, uint16_t count);
static uint8_t crc_check(const uint8_t *data, uint16_t count, uint8_t crc);

/**
  * @defgroup  SHT40AD1B_interfaces_functions
  * @brief     This section provide a set of functions used to read and write
  *            a generic register of the device.
  * @{
  *
  */

/**
  * @brief  Read generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to read
  * @param  data  pointer to buffer that store the data read(ptr)
  * @param  len   number of consecutive register to read
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak sht40ad1b_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                                  uint8_t *data, uint16_t len)
{
  int32_t ret;

  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  Write generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to write
  * @param  data  pointer to data to write in register reg(ptr)
  * @param  len   number of consecutive register to write
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak sht40ad1b_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                                   uint8_t *data, uint16_t len)
{
  int32_t ret;

  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  SHT40AD1B_Data_Read
  * @brief     This section group all the functions concerning data reading
  * @{
  *
  */

/**
  * @brief  Humidity and Temperature output value [get]
  *
  * @param  ctx     read / write interface definitions
  * @param  buffer  buffer to store humidity and temperature values pair
  * @retval         interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t sht40ad1b_data_get(stmdev_ctx_t *ctx, float_t *buffer)
{
  uint8_t command = 0xFD;
  uint8_t data[6] = {0};

  if (sht40ad1b_write_reg(ctx, 0, &command, 1) != 0)
  {
    return 1;
  }

  /* Wait 10 ms */
  ctx->mdelay(10);

  if (sht40ad1b_read_reg(ctx, 0, data, 6) != 0)
  {
    return 1;
  }

  uint16_t temp_value_raw = (data[0] * 0x100U) + data[1];
  uint8_t temp_value_crc  = data[2];
  uint16_t hum_value_raw  = (data[3] * 0x100U) + data[4];
  uint8_t hum_value_crc   = data[5];

  /* Check CRC for temperature value */
  if (crc_check(&data[0], 2, temp_value_crc) != 0U)
  {
    return 1;
  }

  /* Check CRC for humidity value */
  if (crc_check(&data[3], 2, hum_value_crc) != 0U)
  {
    return 1;
  }

  float_t temp_value = -45.0f + (175.0f * (float_t)temp_value_raw / (float_t)0xFFFF);
  float_t hum_value  =  -6.0f + (125.0f * (float_t)hum_value_raw  / (float_t)0xFFFF);

  hum_value = (hum_value > 100.0f) ? 100.0f
              : (hum_value <   0.0f) ?   0.0f
              :                        hum_value;

  buffer[0] = hum_value;
  buffer[1] = temp_value;
  return 0;
}


/**
  * @brief  serial ID value [get]
  *
  * @param  ctx     read / write interface definitions
  * @param  buffer  buffer to store humidity and temperature values pair
  * @retval         interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t sht40ad1b_serial_get(stmdev_ctx_t *ctx, uint32_t *buffer)
{
  uint8_t command = 0x89;  // TODO: Replace value 0x89 with enum value
  uint8_t data[6] = {0};

  if (sht40ad1b_write_reg(ctx, 0, &command, 1) != 0)
  {
    return 1;
  }

  /* Wait 10 ms */
  ctx->mdelay(10);

  if (sht40ad1b_read_reg(ctx, 0, data, 6) != 0)
  {
    return 1;
  }

  uint16_t serial_first_part = (data[0] * 0x100U) + data[1];
  uint8_t first_value_crc  = data[2];
  uint16_t serial_second_part  = (data[3] * 0x100U) + data[4];
  uint8_t second_value_crc   = data[5];

  /* Check CRC first part */
  if (crc_check(&data[0], 2, first_value_crc) != 0U)
  {
    return 1;
  }

  /* Check CRC second part */
  if (crc_check(&data[3], 2, second_value_crc) != 0U)
  {
    return 1;
  }
  
  *buffer = (serial_first_part<<16) + serial_second_part;
  
  return 0;
}

/**
  * @}
  *
  */

/**
  * @defgroup  SHT40AD1B_common
  * @brief     This section group common useful functions
  * @{
  *
  */

/**
  * @brief  Device Who amI.[get]
  *
  * @param  ctx     read / write interface definitions
  * @param  buff    buffer that stores data read
  * @retval         interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t sht40ad1b_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  (void)ctx;
  *buff = SHT40AD1B_ID;
  return 0;
}

/**
  * @brief  Calculate CRC
  *
  * @param  data   data stream bytes
  * @param  count  number of data bytes
  * @retval        CRC check sum of data stream
  *
  */
static uint8_t crc_calculate(const uint8_t *data, uint16_t count)
{
  const uint8_t crc8_polynomial = 0x31;
  uint8_t crc = 0xFF;

  /* Calculate 8-bit checksum for given polynomial */
  for (uint16_t index = 0; index < count; index++)
  {
    crc ^= data[index];
    for (uint8_t crc_bit = 8U; crc_bit > 0U; crc_bit--)
    {
      crc = ((crc & 0x80U) != 0U) ? ((crc << 1) ^ crc8_polynomial) : (crc << 1);
    }
  }

  return crc;
}

/**
  * @brief  Check CRC
  *
  * @param  data   data stream bytes
  * @param  count  number of data bytes
  * @param  crc    CRC check sum of data stream
  * @retval        0 if CRC is OK else 1
  *
  */
static uint8_t crc_check(const uint8_t *data, uint16_t count, uint8_t crc)
{
  return (crc_calculate(data, count) == crc) ? 0U : 1U;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */
