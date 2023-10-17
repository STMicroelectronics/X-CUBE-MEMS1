/**
  ******************************************************************************
  * @file    sgp40_reg.c
  * @author  Sensors Software Solution Team
  * @brief   SGP40 driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics
  * All rights reserved
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "sgp40_reg.h"

/**
  * @defgroup  SGP40
  * @brief     This file provides a set of functions needed to drive the
  *            sgp40 sensor.
  * @{
  *
  */

static uint8_t crc_calculate(const uint8_t *data, uint16_t count);
static uint8_t crc_check(const uint8_t *data, uint16_t count, uint8_t crc);
static void hum_to_ticks(float_t value, uint8_t *value_ticks);
static void temp_to_ticks(float_t value, uint8_t *value_ticks);

/**
  * @defgroup  SGP40_interfaces_functions
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
int32_t __weak sgp40_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
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
int32_t __weak sgp40_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
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
  * @defgroup  SGP40_Data_Read
  * @brief     This section group all the functions concerning data reading
  * @{
  *
  */

/**
  * @brief  VOC index output value [get]
  *
  * @param  ctx       read / write interface definitions
  * @param  value     VOC index value
  * @param  ht_value  Humidity and Temperature value
  *                   or NULL if no humidity compensation required
  * @retval           interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t sgp40_data_get(stmdev_ctx_t *ctx, uint16_t *value, float_t *ht_value)
{
  uint8_t command[] =
  {
    0x26, 0x0F,
    0x80, 0x00, 0xA2,
    0x66, 0x66, 0x93,
  };

  /* Increased array size due to MISRAC2012-Rule-18.1_d */
  uint8_t data[8] = {0};

  if (ht_value != NULL)
  {
    hum_to_ticks(ht_value[0], &command[2]);
    command[4] = crc_calculate(&command[2], 2);

    temp_to_ticks(ht_value[1], &command[5]);
    command[7] = crc_calculate(&command[5], 2);
  }

  if (sgp40_write_reg(ctx, 0, command, 8) != 0)
  {
    return 1;
  }

  /* Wait 30 ms */
  ctx->mdelay(30);

  if (sgp40_read_reg(ctx, 0, data, 3) != 0)
  {
    return 1;
  }

  /* Check CRC */
  if (crc_check(&data[0], 2, data[2]) != 0U)
  {
    return 1;
  }

  *value = (data[0] * 0x100U) + data[1];
  return 0;
}

/**
  * @}
  *
  */

/**
  * @defgroup  SGP40_common
  * @brief     This section group common useful functions
  * @{
  *
  */

/**
  * @brief  Device WHO AM I [get]
  *
  * @param  ctx  read / write interface definitions
  * @param  id   component ID (WHO AM I value)
  * @retval      interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t sgp40_device_id_get(stmdev_ctx_t *ctx, uint8_t *id)
{
  (void)ctx;

  *id = SGP40_ID;

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
  * @brief  Convert humidity value to ticks
  *
  * @param  value        Humidity value (0 .. 100 [%])
  * @param  value_ticks  Humidity value converted to ticks
  *
  */
static void hum_to_ticks(float_t value, uint8_t *value_ticks)
{
  float_t value_local;
  float_t value_raw_float;
  uint16_t value_raw_uint;

  value_local = (value > 100.0f) ? 100.0f
                : (value < 0.0f) ?   0.0f
                :                  value;

  value_raw_float = value_local * (float_t)0xFFFF / 100.0f;
  value_raw_uint = (uint16_t)(value_raw_float);
  value_ticks[0] = (uint8_t)value_raw_uint >> 8;
  value_ticks[1] = (uint8_t)value_raw_uint;
}

/**
  * @brief  Convert temperature value to ticks
  *
  * @param  value        Temperature value (-45 .. 125 [degC])
  * @param  value_ticks  Temperature value converted to ticks
  *
  */
static void temp_to_ticks(float_t value, uint8_t *value_ticks)
{
  float_t value_local;
  float_t value_raw_float;
  uint16_t value_raw_uint;

  value_local = (value >   130.0f) ? 130.0f
                : (value < -45.0f) ? -45.0f
                :                    value;

  value_raw_float = (value_local + 45.0f) * (float_t)0xFFFF / 175.0f;
  value_raw_uint = (uint16_t)(value_raw_float);
  value_ticks[0] = (uint8_t)value_raw_uint >> 8;
  value_ticks[1] = (uint8_t)value_raw_uint;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */
