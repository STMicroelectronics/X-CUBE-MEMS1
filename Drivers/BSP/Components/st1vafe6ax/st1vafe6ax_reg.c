/**
  ******************************************************************************
  * @file    st1vafe6ax_reg.c
  * @author  Sensors Software Solution Team
  * @brief   ST1VAFE6AX driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "st1vafe6ax_reg.h"

/**
  * @defgroup  ST1VAFE6AX
  * @brief     This file provides a set of functions needed to drive the
  *            st1vafe6ax enhanced inertial module.
  * @{
  *
  */

/**
  * @defgroup  Interfaces functions
  * @brief     This section provide a set of functions used to read and
  *            write a generic register of the device.
  *            MANDATORY: return 0 -> no Error.
  * @{
  *
  */

/**
  * @brief  Read generic device register
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  reg   first register address to read.
  * @param  data  buffer for data read.(ptr)
  * @param  len   number of consecutive register to read.
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak st1vafe6ax_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                   uint8_t *data,
                                   uint16_t len)
{
  int32_t ret;

  if (ctx == NULL)
  {
    return -1;
  }

  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  Write generic device register
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  reg   first register address to write.
  * @param  data  the buffer contains data to be written.(ptr)
  * @param  len   number of consecutive register to write.
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak st1vafe6ax_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                    uint8_t *data,
                                    uint16_t len)
{
  int32_t ret;

  if (ctx == NULL)
  {
    return -1;
  }

  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Private functions
  * @brief     Section collect all the utility functions needed by APIs.
  * @{
  *
  */

static void bytecpy(uint8_t *target, uint8_t *source)
{
  if ((target != NULL) && (source != NULL))
  {
    *target = *source;
  }
}

/**
  * @}
  *
  */

/**
  * @defgroup  Sensitivity
  * @brief     These functions convert raw-data into engineering units.
  * @{
  *
  */
float_t st1vafe6ax_from_sflp_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.061f;
}

float_t st1vafe6ax_from_fs2_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.061f;
}

float_t st1vafe6ax_from_fs4_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.122f;
}

float_t st1vafe6ax_from_fs8_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.244f;
}

float_t st1vafe6ax_from_fs16_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.488f;
}

float_t st1vafe6ax_from_fs125_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 4.375f;
}

float_t st1vafe6ax_from_fs250_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 8.750f;
}

float_t st1vafe6ax_from_fs500_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 17.50f;
}

float_t st1vafe6ax_from_fs1000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 35.0f;
}

float_t st1vafe6ax_from_fs2000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 70.0f;
}

float_t st1vafe6ax_from_fs4000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 140.0f;
}

float_t st1vafe6ax_from_lsb_to_celsius(int16_t lsb)
{
  return (((float_t)lsb / 256.0f) + 25.0f);
}

uint64_t st1vafe6ax_from_lsb_to_nsec(uint32_t lsb)
{
  return ((uint64_t)lsb * 21750);
}

float_t st1vafe6ax_from_lsb_to_mv(int16_t lsb)
{
  return ((float_t)lsb) / 78.0f;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Common
  * @brief     This section groups common useful functions.
  *
  */

/**
  * @brief  Reset of the device.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Reset of the device.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_reset_set(const stmdev_ctx_t *ctx, st1vafe6ax_reset_t val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  if (ret != 0)
  {
    return ret;
  }

  ctrl3.boot = (val == ST1VAFE6AX_RESTORE_CAL_PARAM) ? 1 : 0;
  ctrl3.sw_reset = (val == ST1VAFE6AX_RESTORE_CTRL_REGS) ? 1 : 0;
  func_cfg_access.sw_por = (val == ST1VAFE6AX_GLOBAL_RST) ? 1 : 0;

  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  return ret;
}

/**
  * @brief  Global reset of the device.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Global reset of the device.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_reset_get(const stmdev_ctx_t *ctx, st1vafe6ax_reset_t *val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  if (ret != 0)
  {
    return ret;
  }

  switch ((ctrl3.sw_reset << 2) + (ctrl3.boot << 1) + func_cfg_access.sw_por)
  {
    case ST1VAFE6AX_READY:
      *val = ST1VAFE6AX_READY;
      break;

    case ST1VAFE6AX_GLOBAL_RST:
      *val = ST1VAFE6AX_GLOBAL_RST;
      break;

    case ST1VAFE6AX_RESTORE_CAL_PARAM:
      *val = ST1VAFE6AX_RESTORE_CAL_PARAM;
      break;

    case ST1VAFE6AX_RESTORE_CTRL_REGS:
      *val = ST1VAFE6AX_RESTORE_CTRL_REGS;
      break;

    default:
      *val = ST1VAFE6AX_GLOBAL_RST;
      break;
  }
  return ret;
}

/**
  * @brief  Change memory bank.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, EMBED_FUNC_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mem_bank_set(const stmdev_ctx_t *ctx, st1vafe6ax_mem_bank_t val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret == 0)
  {
    func_cfg_access.emb_func_reg_access = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  return ret;
}

/**
  * @brief  Change memory bank.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, SENSOR_HUB_MEM_BANK, EMBED_FUNC_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mem_bank_get(const stmdev_ctx_t *ctx, st1vafe6ax_mem_bank_t *val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (func_cfg_access.emb_func_reg_access)
  {
    case ST1VAFE6AX_MAIN_MEM_BANK:
      *val = ST1VAFE6AX_MAIN_MEM_BANK;
      break;

    case ST1VAFE6AX_EMBED_FUNC_MEM_BANK:
      *val = ST1VAFE6AX_EMBED_FUNC_MEM_BANK;
      break;

    default:
      *val = ST1VAFE6AX_MAIN_MEM_BANK;
      break;
  }
  return ret;
}

/**
  * @brief  Device ID.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Device ID.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_who_am_i_t who_am_i;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WHO_AM_I, (uint8_t *)&who_am_i, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = who_am_i.id;

  return ret;
}

/**
  * @brief  Accelerometer output data rate (ODR) selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_xl_data_rate_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_data_rate_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_data_rate_t val)
{
  st1vafe6ax_ctrl1_t ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);
  if (ret == 0)
  {
    ctrl1.odr_xl = (uint8_t)val & 0xFU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer output data rate (ODR) selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_xl_data_rate_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_data_rate_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_data_rate_t *val)
{
  st1vafe6ax_ctrl1_t ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl1.odr_xl)
  {
    case ST1VAFE6AX_XL_ODR_OFF:
      *val = ST1VAFE6AX_XL_ODR_OFF;
      break;

    case ST1VAFE6AX_XL_ODR_AT_1Hz875:
      *val = ST1VAFE6AX_XL_ODR_AT_1Hz875;
      break;

    case ST1VAFE6AX_XL_ODR_AT_7Hz5:
      *val = ST1VAFE6AX_XL_ODR_AT_7Hz5;
      break;

    case ST1VAFE6AX_XL_ODR_AT_15Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_15Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_30Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_30Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_60Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_60Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_120Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_120Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_240Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_240Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_480Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_480Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_960Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_960Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_1920Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_1920Hz;
      break;

    case ST1VAFE6AX_XL_ODR_AT_3840Hz:
      *val = ST1VAFE6AX_XL_ODR_AT_3840Hz;
      break;

    default:
      *val = ST1VAFE6AX_XL_ODR_OFF;
      break;
  }
  return ret;
}

/**
  * @brief  Accelerometer operating mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_xl_mode_t struct
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_xl_mode_t val)
{
  st1vafe6ax_ctrl1_t ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);

  if (ret == 0)
  {
    ctrl1.op_mode_xl = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer operating mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_xl_mode_t struct
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_xl_mode_t *val)
{
  st1vafe6ax_ctrl1_t ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL1, (uint8_t *)&ctrl1, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl1.op_mode_xl)
  {
    case ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD:
      *val = ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD;
      break;

    case ST1VAFE6AX_XL_HIGH_PERFORMANCE_2_MD:
      *val = ST1VAFE6AX_XL_HIGH_PERFORMANCE_2_MD;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_2_AVG_MD:
      *val = ST1VAFE6AX_XL_LOW_POWER_2_AVG_MD;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_4_AVG_MD:
      *val = ST1VAFE6AX_XL_LOW_POWER_4_AVG_MD;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_8_AVG_MD:
      *val = ST1VAFE6AX_XL_LOW_POWER_8_AVG_MD;
      break;

    default:
      *val = ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope output data rate (ODR) selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_gy_data_rate_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_data_rate_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_data_rate_t val)
{
  st1vafe6ax_ctrl2_t ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);

  if (ret == 0)
  {
    ctrl2.odr_g = (uint8_t)val & 0xFU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope output data rate (ODR) selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_gy_data_rate_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_data_rate_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_data_rate_t *val)
{
  st1vafe6ax_ctrl2_t ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl2.odr_g)
  {
    case ST1VAFE6AX_GY_ODR_OFF:
      *val = ST1VAFE6AX_GY_ODR_OFF;
      break;

    case ST1VAFE6AX_GY_ODR_AT_7Hz5:
      *val = ST1VAFE6AX_GY_ODR_AT_7Hz5;
      break;

    case ST1VAFE6AX_GY_ODR_AT_15Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_15Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_30Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_30Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_60Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_60Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_120Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_120Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_240Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_240Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_480Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_480Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_960Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_960Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_1920Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_1920Hz;
      break;

    case ST1VAFE6AX_GY_ODR_AT_3840Hz:
      *val = ST1VAFE6AX_GY_ODR_AT_3840Hz;
      break;

    default:
      *val = ST1VAFE6AX_GY_ODR_OFF;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope operating mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_HIGH_PERFORMANCE_MD, GY_HIGH_ACCURANCY_ODR_MD, GY_SLEEP_MD, GY_LOW_POWER_MD,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_gy_mode_t val)
{
  st1vafe6ax_ctrl2_t ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);
  if (ret == 0)
  {
    ctrl2.op_mode_g = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope operating mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_HIGH_PERFORMANCE_MD, GY_HIGH_ACCURANCY_ODR_MD, GY_SLEEP_MD, GY_LOW_POWER_MD,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_gy_mode_t *val)
{
  st1vafe6ax_ctrl2_t ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL2, (uint8_t *)&ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl2.op_mode_g)
  {
    case ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD:
      *val = ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD;
      break;

    case ST1VAFE6AX_GY_SLEEP_MD:
      *val = ST1VAFE6AX_GY_SLEEP_MD;
      break;

    case ST1VAFE6AX_GY_LOW_POWER_MD:
      *val = ST1VAFE6AX_GY_LOW_POWER_MD;
      break;

    default:
      *val = ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD;
      break;
  }
  return ret;
}

/**
  * @brief  Register address automatically incremented during a multiple byte access with a serial interface (enable by default).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Register address automatically incremented during a multiple byte access with a serial interface (enable by default).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    ctrl3.if_inc = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Register address automatically incremented during a multiple byte access with a serial interface (enable by default).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Register address automatically incremented during a multiple byte access with a serial interface (enable by default).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_auto_increment_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl3.if_inc;


  return ret;
}

/**
  * @brief  Block Data Update (BDU): output registers are not updated until LSB and MSB have been read). [set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Block Data Update (BDU): output registers are not updated until LSB and MSB have been read).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);

  if (ret == 0)
  {
    ctrl3.bdu = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Block Data Update (BDU): output registers are not updated until LSB and MSB have been read). [get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Block Data Update (BDU): output registers are not updated until LSB and MSB have been read).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl3_t ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl3.bdu;

  return ret;
}

/**
  * @brief  Enables pulsed data-ready mode (~75 us).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DRDY_LATCHED, DRDY_PULSED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_data_ready_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_mode_t val)
{
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    ctrl4.drdy_pulsed = (uint8_t)val & 0x1U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  Enables pulsed data-ready mode (~75 us).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DRDY_LATCHED, DRDY_PULSED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_data_ready_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_mode_t *val)
{
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl4.drdy_pulsed)
  {
    case ST1VAFE6AX_DRDY_LATCHED:
      *val = ST1VAFE6AX_DRDY_LATCHED;
      break;

    case ST1VAFE6AX_DRDY_PULSED:
      *val = ST1VAFE6AX_DRDY_PULSED;
      break;

    default:
      *val = ST1VAFE6AX_DRDY_LATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope full-scale selection[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      125dps, 250dps, 500dps, 1000dps, 2000dps, 4000dps,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_full_scale_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_gy_full_scale_t val)
{
  st1vafe6ax_ctrl6_t ctrl6;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);

  if (ret == 0)
  {
    ctrl6.fs_g = (uint8_t)val & 0xFU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope full-scale selection[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      125dps, 250dps, 500dps, 1000dps, 2000dps, 4000dps,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_full_scale_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_gy_full_scale_t *val)
{
  st1vafe6ax_ctrl6_t ctrl6;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl6.fs_g)
  {
    case ST1VAFE6AX_125dps:
      *val = ST1VAFE6AX_125dps;
      break;

    case ST1VAFE6AX_250dps:
      *val = ST1VAFE6AX_250dps;
      break;

    case ST1VAFE6AX_500dps:
      *val = ST1VAFE6AX_500dps;
      break;

    case ST1VAFE6AX_1000dps:
      *val = ST1VAFE6AX_1000dps;
      break;

    case ST1VAFE6AX_2000dps:
      *val = ST1VAFE6AX_2000dps;
      break;

    case ST1VAFE6AX_4000dps:
      *val = ST1VAFE6AX_4000dps;
      break;

    default:
      *val = ST1VAFE6AX_125dps;
      break;
  }
  return ret;
}

/**
  * @brief  Accelerometer full-scale selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      2g, 4g, 8g, 16g,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_xl_full_scale_t val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);

  if (ret == 0)
  {
    ctrl8.fs_xl = (uint8_t)val & 0x3U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer full-scale selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      2g, 4g, 8g, 16g,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_xl_full_scale_t *val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl8.fs_xl)
  {
    case ST1VAFE6AX_2g:
      *val = ST1VAFE6AX_2g;
      break;

    case ST1VAFE6AX_4g:
      *val = ST1VAFE6AX_4g;
      break;

    case ST1VAFE6AX_8g:
      *val = ST1VAFE6AX_8g;
      break;

    default:
      *val = ST1VAFE6AX_2g;
      break;
  }
  return ret;
}

/**
  * @brief  It enables the accelerometer Dual channel mode: data with selected full scale and data with maximum full scale are sent simultaneously to two different set of output registers.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables the accelerometer Dual channel mode: data with selected full scale and data with maximum full scale are sent simultaneously to two different set of output registers.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_dual_channel_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);

  if (ret == 0)
  {
    ctrl8.xl_dualc_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  }

  return ret;
}

/**
  * @brief  It enables the accelerometer Dual channel mode: data with selected full scale and data with maximum full scale are sent simultaneously to two different set of output registers.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables the accelerometer Dual channel mode: data with selected full scale and data with maximum full scale are sent simultaneously to two different set of output registers.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_dual_channel_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl8.xl_dualc_en;

  return ret;
}

/**
  * @brief  Accelerometer self-test selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ST_DISABLE, XL_ST_POSITIVE, XL_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_self_test_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_self_test_t val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret == 0)
  {
    ctrl10.st_xl = (uint8_t)val & 0x3U;
    ctrl10.xl_st_offset = ((uint8_t)val & 0x04U) >> 2;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer self-test selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ST_DISABLE, XL_ST_POSITIVE, XL_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_self_test_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_self_test_t *val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  if (ret != 0)
  {
    return ret;
  }

  //switch (ctrl10.xl_st_offset)
  switch (ctrl10.st_xl)
  {
    case ST1VAFE6AX_XL_ST_DISABLE:
      *val = ST1VAFE6AX_XL_ST_DISABLE;
      break;

    case ST1VAFE6AX_XL_ST_POSITIVE:
      *val = ST1VAFE6AX_XL_ST_POSITIVE;
      break;

    case ST1VAFE6AX_XL_ST_NEGATIVE:
      *val = ST1VAFE6AX_XL_ST_NEGATIVE;
      break;

    default:
      *val = ST1VAFE6AX_XL_ST_DISABLE;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope self-test selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ST_DISABLE, XL_ST_POSITIVE, XL_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_self_test_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_self_test_t val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret == 0)
  {
    ctrl10.st_g = (uint8_t)val & 0x3U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope self-test selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ST_DISABLE, XL_ST_POSITIVE, XL_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_gy_self_test_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_self_test_t *val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl10.st_g)
  {
    case ST1VAFE6AX_GY_ST_DISABLE:
      *val = ST1VAFE6AX_GY_ST_DISABLE;
      break;

    case ST1VAFE6AX_GY_ST_POSITIVE:
      *val = ST1VAFE6AX_GY_ST_POSITIVE;
      break;

    case ST1VAFE6AX_GY_ST_NEGATIVE:
      *val = ST1VAFE6AX_GY_ST_NEGATIVE;
      break;

    default:
      *val = ST1VAFE6AX_GY_ST_DISABLE;
      break;
  }
  return ret;
}

/**
  * @brief  Get the status of all the interrupt sources.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the status of all the interrupt sources.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_all_sources_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_all_sources_t *val)
{
  st1vafe6ax_emb_func_status_mainpage_t emb_func_status_mainpage;
  st1vafe6ax_emb_func_exec_status_t emb_func_exec_status;
  st1vafe6ax_fsm_status_mainpage_t fsm_status_mainpage;
  st1vafe6ax_mlc_status_mainpage_t mlc_status_mainpage;
  st1vafe6ax_functions_enable_t functions_enable;
  st1vafe6ax_emb_func_src_t emb_func_src;
  st1vafe6ax_fifo_status2_t fifo_status2;
  st1vafe6ax_all_int_src_t all_int_src;
  st1vafe6ax_wake_up_src_t wake_up_src;
  st1vafe6ax_status_reg_t status_reg;
  st1vafe6ax_d6d_src_t d6d_src;
  st1vafe6ax_tap_src_t tap_src;
  uint8_t buff[7];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.dis_rst_lir_all_int = PROPERTY_ENABLE;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_STATUS1, (uint8_t *)&buff, 4);
  }

  if (ret != 0)
  {
    return ret;
  }

  bytecpy((uint8_t *)&fifo_status2, &buff[1]);
  bytecpy((uint8_t *)&all_int_src, &buff[2]);
  bytecpy((uint8_t *)&status_reg, &buff[3]);

  val->fifo_ovr = fifo_status2.fifo_ovr_ia;
  val->fifo_bdr = fifo_status2.counter_bdr_ia;
  val->fifo_full = fifo_status2.fifo_full_ia;
  val->fifo_th = fifo_status2.fifo_wtm_ia;

  val->free_fall = all_int_src.ff_ia;
  val->wake_up = all_int_src.wu_ia;
  val->six_d = all_int_src.d6d_ia;

  val->drdy_xl = status_reg.xlda;
  val->drdy_gy = status_reg.gda;
  val->drdy_temp = status_reg.tda;
  val->drdy_ah_bio = status_reg.ah_bioda;
  val->timestamp = status_reg.timestamp_endcount;

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }
  if (ret == 0)
  {
    functions_enable.dis_rst_lir_all_int = PROPERTY_DISABLE;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_SRC, (uint8_t *)&buff, 7);
  }

  if (ret == 0)
  {
    bytecpy((uint8_t *)&wake_up_src, &buff[0]);
    bytecpy((uint8_t *)&tap_src, &buff[1]);
    bytecpy((uint8_t *)&d6d_src, &buff[2]);
    bytecpy((uint8_t *)&emb_func_status_mainpage, &buff[4]);
    bytecpy((uint8_t *)&fsm_status_mainpage, &buff[5]);
    bytecpy((uint8_t *)&mlc_status_mainpage, &buff[6]);

    val->sleep_change = wake_up_src.sleep_change_ia;
    val->wake_up_x = wake_up_src.x_wu;
    val->wake_up_y = wake_up_src.y_wu;
    val->wake_up_z = wake_up_src.z_wu;
    val->sleep_state = wake_up_src.sleep_state;

    val->tap_x = tap_src.x_tap;
    val->tap_y = tap_src.y_tap;
    val->tap_z = tap_src.z_tap;
    val->tap_sign = tap_src.tap_sign;
    val->double_tap = tap_src.double_tap;
    val->single_tap = tap_src.single_tap;

    val->six_d_zl = d6d_src.zl;
    val->six_d_zh = d6d_src.zh;
    val->six_d_yl = d6d_src.yl;
    val->six_d_yh = d6d_src.yh;
    val->six_d_xl = d6d_src.xl;
    val->six_d_xh = d6d_src.xh;

    val->step_detector = emb_func_status_mainpage.is_step_det;
    val->tilt = emb_func_status_mainpage.is_tilt;
    val->sig_mot = emb_func_status_mainpage.is_sigmot;
    val->fsm_lc = emb_func_status_mainpage.is_fsm_lc;

    val->fsm1 = fsm_status_mainpage.is_fsm1;
    val->fsm2 = fsm_status_mainpage.is_fsm2;
    val->fsm3 = fsm_status_mainpage.is_fsm3;
    val->fsm4 = fsm_status_mainpage.is_fsm4;
    val->fsm5 = fsm_status_mainpage.is_fsm5;
    val->fsm6 = fsm_status_mainpage.is_fsm6;
    val->fsm7 = fsm_status_mainpage.is_fsm7;
    val->fsm8 = fsm_status_mainpage.is_fsm8;

    val->mlc1 = mlc_status_mainpage.is_mlc1;
    val->mlc2 = mlc_status_mainpage.is_mlc2;
    val->mlc3 = mlc_status_mainpage.is_mlc3;
    val->mlc4 = mlc_status_mainpage.is_mlc4;
  }


  if (ret == 0)
  {
    ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EXEC_STATUS, (uint8_t *)&emb_func_exec_status,
                                1);
    }
    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
    }

    ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  }

  if (ret != 0)
  {
    return ret;
  }

  val->emb_func_stand_by = emb_func_exec_status.emb_func_endop;
  val->emb_func_time_exceed = emb_func_exec_status.emb_func_exec_ovr;
  val->step_count_inc = emb_func_src.stepcounter_bit_set;
  val->step_count_overflow = emb_func_src.step_overflow;
  val->step_on_delta_time = emb_func_src.step_count_delta_ia;

  val->step_detector = emb_func_src.step_detected;

  return ret;
}

int32_t st1vafe6ax_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_t *val)
{
  st1vafe6ax_status_reg_t status;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_STATUS_REG, (uint8_t *)&status, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->drdy_xl = status.xlda;
  val->drdy_gy = status.gda;
  val->drdy_temp = status.tda;
  val->drdy_ah_bio = status.ah_bioda;

  return ret;
}

/**
  * @brief  Temperature data output register[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Temperature data output register
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_OUT_TEMP_L, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  Angular rate sensor.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Angular rate sensor.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_angular_rate_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_OUTX_L_G, &buff[0], 6);
  if (ret != 0)
  {
    return ret;
  }

  val[0] = (int16_t)buff[1];
  val[0] = (val[0] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[2] = (int16_t)buff[5];
  val[2] = (val[2] * 256) + (int16_t)buff[4];

  return ret;
}

/**
  * @brief  Linear acceleration sensor.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Linear acceleration sensor.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_OUTZ_L_A, &buff[0], 6);
  if (ret != 0)
  {
    return ret;
  }

  val[2] = (int16_t)buff[1];
  val[2] = (val[2] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[0] = (int16_t)buff[5];
  val[0] = (val[0] * 256) + (int16_t)buff[4];

  return ret;
}

/**
  * @brief  Linear acceleration sensor for Dual channel mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Linear acceleration sensor or Dual channel mode.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_dual_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_UI_OUTZ_L_A_DUALC, &buff[0], 6);
  if (ret != 0)
  {
    return ret;
  }

  val[2] = (int16_t)buff[1];
  val[2] = (val[2] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[0] = (int16_t)buff[5];
  val[0] = (val[0] * 256) + (int16_t)buff[4];

  return ret;
}

/**
  * @brief  Bio data output register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio data output register.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ah_bio_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_AH_BIO_OUT_L, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  Difference in percentage of the effective ODR (and timestamp rate) with respect to the typical. Step: 0.13%. 8-bit format, 2's complement.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Difference in percentage of the effective ODR (and timestamp rate) with respect to the typical. Step: 0.13%. 8-bit format, 2's complement.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_odr_cal_reg_get(const stmdev_ctx_t *ctx, int8_t *val)
{
  st1vafe6ax_internal_freq_t internal_freq;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INTERNAL_FREQ, (uint8_t *)&internal_freq, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = (int8_t)internal_freq.freq_fine;

  return ret;
}

/**
  * @brief  Write buffer in a page.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Write buffer in a page.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ln_pg_write(const stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len)
{
  st1vafe6ax_page_address_t  page_address;
  st1vafe6ax_page_sel_t page_sel;
  st1vafe6ax_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  /* set page write */
  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_ENABLE;
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);

  /* select page */
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_sel.page_sel = msb;
  page_sel.not_used0 = 1; // Default value
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                              1);
  if (ret != 0)
  {
    goto exit;
  }

  /* set page addr */
  page_address.page_addr = lsb;
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_ADDRESS,
                              (uint8_t *)&page_address, 1);
  if (ret != 0)
  {
    goto exit;
  }

  for (i = 0; ((i < len) && (ret == 0)); i++)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_VALUE, &buf[i], 1);
    if (ret != 0)
    {
      goto exit;
    }
    lsb++;

    /* Check if page wrap */
    if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
    {
      msb++;
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel, 1);
      if (ret != 0)
      {
        goto exit;
      }

      page_sel.page_sel = msb;
      page_sel.not_used0 = 1; // Default value
      ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                                 1);
      if (ret != 0)
      {
        goto exit;
      }
    }
  }

  page_sel.page_sel = 0;
  page_sel.not_used0 = 1;// Default value
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                             1);
  if (ret != 0)
  {
    goto exit;
  }

  /* unset page write */
  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);

exit:
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Read buffer in a page.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Write buffer in a page.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ln_pg_read(const stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *buf, uint8_t len)
{
  st1vafe6ax_page_address_t  page_address;
  st1vafe6ax_page_sel_t page_sel;
  st1vafe6ax_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  /* set page write */
  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_rw.page_read = PROPERTY_ENABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);

  /* select page */
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_sel.page_sel = msb;
  page_sel.not_used0 = 1; // Default value
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                             1);
  if (ret != 0)
  {
    goto exit;
  }

  /* set page addr */
  page_address.page_addr = lsb;
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_ADDRESS,
                             (uint8_t *)&page_address, 1);
  if (ret != 0)
  {
    goto exit;
  }

  for (i = 0; ((i < len) && (ret == 0)); i++)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_VALUE, &buf[i], 1);
    if (ret != 0)
    {
      goto exit;
    }
    lsb++;

    /* Check if page wrap */
    if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
    {
      msb++;
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel, 1);
      if (ret != 0)
      {
        goto exit;
      }

      page_sel.page_sel = msb;
      page_sel.not_used0 = 1; // Default value
      ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                                 1);
      if (ret != 0)
      {
        goto exit;
      }
    }
  }

  page_sel.page_sel = 0;
  page_sel.not_used0 = 1;// Default value
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_SEL, (uint8_t *)&page_sel,
                             1);
  if (ret != 0)
  {
    goto exit;
  }

  /* unset page write */
  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
  if (ret != 0)
  {
    goto exit;
  }
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);

exit:
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Timestamp
  * @brief     This section groups all the functions that manage the
  *            timestamp generation.
  * @{
  *
  */

/**
  * @brief  Enables timestamp counter.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables timestamp counter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.timestamp_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  return ret;
}

/**
  * @brief  Enables timestamp counter.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables timestamp counter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = functions_enable.timestamp_en;

  return ret;
}

/**
  * @brief  Timestamp data output.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Timestamp data output.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TIMESTAMP0, &buff[0], 4);
  if (ret != 0)
  {
    return ret;
  }

  *val = buff[3];
  *val = (*val * 256U) + buff[2];
  *val = (*val * 256U) + buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Filters
  * @brief     This section group all the functions concerning the
  *            filters configuration
  * @{
  *
  */

/**
  * @brief  Protocol anti-spike filters.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      AUTO, ALWAYS_ACTIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_anti_spike_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_anti_spike_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);

  if (ret == 0)
  {
    if_cfg.asf_ctrl = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Protocol anti-spike filters.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      AUTO, ALWAYS_ACTIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_anti_spike_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_anti_spike_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (if_cfg.asf_ctrl)
  {
    case ST1VAFE6AX_AUTO:
      *val = ST1VAFE6AX_AUTO;
      break;

    case ST1VAFE6AX_ALWAYS_ACTIVE:
      *val = ST1VAFE6AX_ALWAYS_ACTIVE;
      break;

    default:
      *val = ST1VAFE6AX_AUTO;
      break;
  }
  return ret;
}

/**
  * @brief  It masks DRDY and Interrupts RQ until filter settling ends.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It masks DRDY and Interrupts RQ until filter settling ends.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_settling_mask_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_settling_mask_t val)
{
  st1vafe6ax_emb_func_cfg_t emb_func_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    ctrl4.drdy_mask = val.drdy;

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);
  }

  if (ret == 0)
  {
    emb_func_cfg.emb_func_irq_mask_xl_settl = val.irq_xl;
    emb_func_cfg.emb_func_irq_mask_g_settl = val.irq_g;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);
  }

  return ret;
}

/**
  * @brief  It masks DRDY and Interrupts RQ until filter settling ends.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It masks DRDY and Interrupts RQ until filter settling ends.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_settling_mask_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_settling_mask_t *val)
{
  st1vafe6ax_emb_func_cfg_t emb_func_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);

  if (ret != 0)
  {
    return ret;
  }

  val->drdy = ctrl4.drdy_mask;
  val->irq_xl = emb_func_cfg.emb_func_irq_mask_xl_settl;
  val->irq_g = emb_func_cfg.emb_func_irq_mask_g_settl;

  return ret;
}

/**
  * @brief  Gyroscope low-pass filter (LPF1) bandwidth selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ULTRA_LIGHT, VERY_LIGHT, LIGHT, MEDIUM, STRONG, VERY_STRONG, AGGRESSIVE, XTREME,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_gy_lp1_bandwidth_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_gy_lp1_bandwidth_t val)
{
  st1vafe6ax_ctrl6_t ctrl6;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);
  if (ret == 0)
  {
    ctrl6.lpf1_g_bw = (uint8_t)val & 0x7U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope low-pass filter (LPF1) bandwidth selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ULTRA_LIGHT, VERY_LIGHT, LIGHT, MEDIUM, STRONG, VERY_STRONG, AGGRESSIVE, XTREME,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_gy_lp1_bandwidth_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_gy_lp1_bandwidth_t *val)
{
  st1vafe6ax_ctrl6_t ctrl6;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL6, (uint8_t *)&ctrl6, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl6.lpf1_g_bw)
  {
    case ST1VAFE6AX_GY_ULTRA_LIGHT:
      *val = ST1VAFE6AX_GY_ULTRA_LIGHT;
      break;

    case ST1VAFE6AX_GY_VERY_LIGHT:
      *val = ST1VAFE6AX_GY_VERY_LIGHT;
      break;

    case ST1VAFE6AX_GY_LIGHT:
      *val = ST1VAFE6AX_GY_LIGHT;
      break;

    case ST1VAFE6AX_GY_MEDIUM:
      *val = ST1VAFE6AX_GY_MEDIUM;
      break;

    case ST1VAFE6AX_GY_STRONG:
      *val = ST1VAFE6AX_GY_STRONG;
      break;

    case ST1VAFE6AX_GY_VERY_STRONG:
      *val = ST1VAFE6AX_GY_VERY_STRONG;
      break;

    case ST1VAFE6AX_GY_AGGRESSIVE:
      *val = ST1VAFE6AX_GY_AGGRESSIVE;
      break;

    case ST1VAFE6AX_GY_XTREME:
      *val = ST1VAFE6AX_GY_XTREME;
      break;

    default:
      *val = ST1VAFE6AX_GY_ULTRA_LIGHT;
      break;
  }
  return ret;
}

/**
  * @brief  It enables gyroscope digital LPF1 filter.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables gyroscope digital LPF1 filter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_gy_lp1_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);

  if (ret == 0)
  {
    ctrl7.lpf1_g_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  }

  return ret;
}

/**
  * @brief  It enables gyroscope digital LPF1 filter.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables gyroscope digital LPF1 filter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_gy_lp1_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl7.lpf1_g_en;

  return ret;
}

/**
  * @brief  Bio filter configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio filter configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_ah_bio_conf_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_filt_ah_bio_conf_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);

  ctrl8.ah_bio_hpf = val.hpf;
  ctrl9.ah_bio_lpf = val.lpf;

  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Bio filter configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio filter configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_ah_bio_conf_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_filt_ah_bio_conf_t *val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);

  if (ret != 0)
  {
    return ret;
  }

  val->lpf = ctrl9.ah_bio_lpf;
  val->hpf = ctrl8.ah_bio_hpf;

  return ret;
}

/**
  * @brief  Accelerometer LPF2 and high pass filter configuration and cutoff setting.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ULTRA_LIGHT, VERY_LIGHT, LIGHT, MEDIUM, STRONG, VERY_STRONG, AGGRESSIVE, XTREME,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_lp2_bandwidth_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_xl_lp2_bandwidth_t val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  if (ret == 0)
  {
    ctrl8.hp_lpf2_xl_bw = (uint8_t)val & 0x7U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer LPF2 and high pass filter configuration and cutoff setting.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ULTRA_LIGHT, VERY_LIGHT, LIGHT, MEDIUM, STRONG, VERY_STRONG, AGGRESSIVE, XTREME,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_lp2_bandwidth_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_xl_lp2_bandwidth_t *val)
{
  st1vafe6ax_ctrl8_t ctrl8;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL8, (uint8_t *)&ctrl8, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl8.hp_lpf2_xl_bw)
  {
    case ST1VAFE6AX_XL_ULTRA_LIGHT:
      *val = ST1VAFE6AX_XL_ULTRA_LIGHT;
      break;

    case ST1VAFE6AX_XL_VERY_LIGHT:
      *val = ST1VAFE6AX_XL_VERY_LIGHT;
      break;

    case ST1VAFE6AX_XL_LIGHT:
      *val = ST1VAFE6AX_XL_LIGHT;
      break;

    case ST1VAFE6AX_XL_MEDIUM:
      *val = ST1VAFE6AX_XL_MEDIUM;
      break;

    case ST1VAFE6AX_XL_STRONG:
      *val = ST1VAFE6AX_XL_STRONG;
      break;

    case ST1VAFE6AX_XL_VERY_STRONG:
      *val = ST1VAFE6AX_XL_VERY_STRONG;
      break;

    case ST1VAFE6AX_XL_AGGRESSIVE:
      *val = ST1VAFE6AX_XL_AGGRESSIVE;
      break;

    case ST1VAFE6AX_XL_XTREME:
      *val = ST1VAFE6AX_XL_XTREME;
      break;

    default:
      *val = ST1VAFE6AX_XL_ULTRA_LIGHT;
      break;
  }
  return ret;
}

/**
  * @brief  Enable accelerometer LPS2 (Low Pass Filter 2) filtering stage.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable accelerometer LPS2 (Low Pass Filter 2) filtering stage.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_lp2_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.lpf2_xl_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Enable accelerometer LPS2 (Low Pass Filter 2) filtering stage.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable accelerometer LPS2 (Low Pass Filter 2) filtering stage.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_lp2_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl9.lpf2_xl_en;

  return ret;
}

/**
  * @brief  Accelerometer slope filter / high-pass filter selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Accelerometer slope filter / high-pass filter selection.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_hp_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.hp_slope_xl_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer slope filter / high-pass filter selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Accelerometer slope filter / high-pass filter selection.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_hp_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl9.hp_slope_xl_en;

  return ret;
}

/**
  * @brief  Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the first sample.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the first sample.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_fast_settling_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.xl_fastsettl_mode = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the first sample.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the first sample.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_fast_settling_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl9.xl_fastsettl_mode;

  return ret;
}

/**
  * @brief  Accelerometer high-pass filter mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HP_MD_NORMAL, HP_MD_REFERENCE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_hp_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_xl_hp_mode_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.hp_ref_mode_xl = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer high-pass filter mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HP_MD_NORMAL, HP_MD_REFERENCE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_xl_hp_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_xl_hp_mode_t *val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl9.hp_ref_mode_xl)
  {
    case ST1VAFE6AX_HP_MD_NORMAL:
      *val = ST1VAFE6AX_HP_MD_NORMAL;
      break;

    case ST1VAFE6AX_HP_MD_REFERENCE:
      *val = ST1VAFE6AX_HP_MD_REFERENCE;
      break;

    default:
      *val = ST1VAFE6AX_HP_MD_NORMAL;
      break;
  }
  return ret;
}

/**
  * @brief  HPF or SLOPE filter selection on wake-up and Activity/Inactivity functions.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      WK_FEED_SLOPE, WK_FEED_HIGH_PASS,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_wkup_act_feed_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_wkup_act_feed_t val)
{
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);

  if (ret != 0)
  {
    return ret;
  }

  tap_cfg0.slope_fds = (uint8_t)val & 0x01U;
  wake_up_ths.usr_off_on_wu = ((uint8_t)val & 0x02U) >> 1;

  if (ret == 0)
  {

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }
  if (ret == 0)
  {

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  }

  return ret;
}

/**
  * @brief  HPF or SLOPE filter selection on wake-up and Activity/Inactivity functions.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      WK_FEED_SLOPE, WK_FEED_HIGH_PASS,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_wkup_act_feed_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_wkup_act_feed_t *val)
{
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);

  if (ret != 0)
  {
    return ret;
  }

  switch ((wake_up_ths.usr_off_on_wu << 1) + tap_cfg0.slope_fds)
  {
    case ST1VAFE6AX_WK_FEED_SLOPE:
      *val = ST1VAFE6AX_WK_FEED_SLOPE;
      break;

    case ST1VAFE6AX_WK_FEED_HIGH_PASS:
      *val = ST1VAFE6AX_WK_FEED_HIGH_PASS;
      break;

    case ST1VAFE6AX_WK_FEED_LP_WITH_OFFSET:
      *val = ST1VAFE6AX_WK_FEED_LP_WITH_OFFSET;
      break;

    default:
      *val = ST1VAFE6AX_WK_FEED_SLOPE;
      break;
  }
  return ret;
}

/**
  * @brief  Mask hw function triggers when xl is settling.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0 or 1,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mask_trigger_xl_settl_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);

  if (ret == 0)
  {
    tap_cfg0.hw_func_mask_xl_settl = val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  Mask hw function triggers when xl is settling.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0 or 1,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mask_trigger_xl_settl_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = tap_cfg0.hw_func_mask_xl_settl;

  return ret;
}


/**
  * @brief  LPF2 filter on 6D (sixd) function selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SIXD_FEED_ODR_DIV_2, SIXD_FEED_LOW_PASS,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_sixd_feed_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_filt_sixd_feed_t val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.low_pass_on_6d = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  LPF2 filter on 6D (sixd) function selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SIXD_FEED_ODR_DIV_2, SIXD_FEED_LOW_PASS,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_filt_sixd_feed_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_filt_sixd_feed_t *val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (tap_cfg0.low_pass_on_6d)
  {
    case ST1VAFE6AX_SIXD_FEED_ODR_DIV_2:
      *val = ST1VAFE6AX_SIXD_FEED_ODR_DIV_2;
      break;

    case ST1VAFE6AX_SIXD_FEED_LOW_PASS:
      *val = ST1VAFE6AX_SIXD_FEED_LOW_PASS;
      break;

    default:
      *val = ST1VAFE6AX_SIXD_FEED_ODR_DIV_2;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Serial interfaces
  * @brief     This section groups all the functions concerning
  *            serial interfaces management (not auxiliary)
  * @{
  *
  */

/**
  * @brief  Enables pull-up on SDO pin of UI (User Interface).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on SDO pin of UI (User Interface).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_sdo_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret == 0)
  {
    pin_ctrl.sdo_pu_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  Enables pull-up on SDO pin of UI (User Interface).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on SDO pin of UI (User Interface).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_sdo_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = pin_ctrl.sdo_pu_en;

  return ret;
}

/**
  * @brief  Disables I2C and I3C on UI (User Interface).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      I2C_I3C_ENABLE, I2C_I3C_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_i2c_i3c_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_ui_i2c_i3c_mode_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.i2c_i3c_disable = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Disables I2C and I3C on UI (User Interface).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      I2C_I3C_ENABLE, I2C_I3C_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_i2c_i3c_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_ui_i2c_i3c_mode_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (if_cfg.i2c_i3c_disable)
  {
    case ST1VAFE6AX_I2C_I3C_ENABLE:
      *val = ST1VAFE6AX_I2C_I3C_ENABLE;
      break;

    case ST1VAFE6AX_I2C_I3C_DISABLE:
      *val = ST1VAFE6AX_I2C_I3C_DISABLE;
      break;

    default:
      *val = ST1VAFE6AX_I2C_I3C_ENABLE;
      break;
  }
  return ret;
}

/**
  * @brief  SPI Serial Interface Mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SPI_4_WIRE, SPI_3_WIRE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_spi_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_spi_mode_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.sim = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  SPI Serial Interface Mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SPI_4_WIRE, SPI_3_WIRE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_spi_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_spi_mode_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (if_cfg.sim)
  {
    case ST1VAFE6AX_SPI_4_WIRE:
      *val = ST1VAFE6AX_SPI_4_WIRE;
      break;

    case ST1VAFE6AX_SPI_3_WIRE:
      *val = ST1VAFE6AX_SPI_3_WIRE;
      break;

    default:
      *val = ST1VAFE6AX_SPI_4_WIRE;
      break;
  }
  return ret;
}

/**
  * @brief  Enables pull-up on SDA pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on SDA pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_sda_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.sda_pu_en = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enables pull-up on SDA pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on SDA pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ui_sda_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = if_cfg.sda_pu_en;

  return ret;
}

/**
  * @brief  Select the us activity time for IBI (In-Band Interrupt) with I3C[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      IBI_2us, IBI_50us, IBI_1ms, IBI_25ms,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_i3c_ibi_time_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_i3c_ibi_time_t val)
{
  st1vafe6ax_ctrl5_t ctrl5;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret == 0)
  {
    ctrl5.bus_act_sel = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  }

  return ret;
}

/**
  * @brief  Select the us activity time for IBI (In-Band Interrupt) with I3C[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      IBI_2us, IBI_50us, IBI_1ms, IBI_25ms,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_i3c_ibi_time_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_i3c_ibi_time_t *val)
{
  st1vafe6ax_ctrl5_t ctrl5;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl5.bus_act_sel)
  {
    case ST1VAFE6AX_IBI_2us:
      *val = ST1VAFE6AX_IBI_2us;
      break;

    case ST1VAFE6AX_IBI_50us:
      *val = ST1VAFE6AX_IBI_50us;
      break;

    case ST1VAFE6AX_IBI_1ms:
      *val = ST1VAFE6AX_IBI_1ms;
      break;

    case ST1VAFE6AX_IBI_25ms:
      *val = ST1VAFE6AX_IBI_25ms;
      break;

    default:
      *val = ST1VAFE6AX_IBI_2us;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Interrupt pins
  * @brief     This section groups all the functions that manage interrupt pins
  * @{
  *
  */

/**
  * @brief  Push-pull/open-drain selection on INT1 and INT2 pins.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      PUSH_PULL, OPEN_DRAIN,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_int_pin_mode_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.pp_od = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Push-pull/open-drain selection on INT1 and INT2 pins.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      PUSH_PULL, OPEN_DRAIN,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_int_pin_mode_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (if_cfg.pp_od)
  {
    case ST1VAFE6AX_PUSH_PULL:
      *val = ST1VAFE6AX_PUSH_PULL;
      break;

    case ST1VAFE6AX_OPEN_DRAIN:
      *val = ST1VAFE6AX_OPEN_DRAIN;
      break;

    default:
      *val = ST1VAFE6AX_PUSH_PULL;
      break;
  }
  return ret;
}

/**
  * @brief  Interrupt activation level.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ACTIVE_HIGH, ACTIVE_LOW,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_polarity_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_pin_polarity_t val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.h_lactive = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Interrupt activation level.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ACTIVE_HIGH, ACTIVE_LOW,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_polarity_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_pin_polarity_t *val)
{
  st1vafe6ax_if_cfg_t if_cfg;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (if_cfg.h_lactive)
  {
    case ST1VAFE6AX_ACTIVE_HIGH:
      *val = ST1VAFE6AX_ACTIVE_HIGH;
      break;

    case ST1VAFE6AX_ACTIVE_LOW:
      *val = ST1VAFE6AX_ACTIVE_LOW;
      break;

    default:
      *val = ST1VAFE6AX_ACTIVE_HIGH;
      break;
  }
  return ret;
}

/**
  * @brief  It routes interrupt signals on INT 1 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It routes interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  st1vafe6ax_pin_int_route_t  pin_int2_route;
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_emb_func_int1_t emb_func_int1;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  st1vafe6ax_int2_ctrl_t int2_ctrl;
  st1vafe6ax_int1_ctrl_t int1_ctrl;
  st1vafe6ax_fsm_int1_t fsm_int1;
  st1vafe6ax_mlc_int1_t mlc_int1;
  st1vafe6ax_md1_cfg_t md1_cfg;
  st1vafe6ax_md2_cfg_t md2_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_INT1, (uint8_t *)&fsm_int1, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_INT1, (uint8_t *)&mlc_int1, 1);
  }

  if (ret == 0)
  {
    emb_func_int1.int1_step_detector = val.step_detector;
    emb_func_int1.int1_tilt = val.tilt;
    emb_func_int1.int1_sig_mot = val.sig_mot;
    emb_func_int1.int1_fsm_lc = val.fsm_lc;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }
  if (ret == 0)
  {
    fsm_int1.int1_fsm1 = val.fsm1;
    fsm_int1.int1_fsm2 = val.fsm2;
    fsm_int1.int1_fsm3 = val.fsm3;
    fsm_int1.int1_fsm4 = val.fsm4;
    fsm_int1.int1_fsm5 = val.fsm5;
    fsm_int1.int1_fsm6 = val.fsm6;
    fsm_int1.int1_fsm7 = val.fsm7;
    fsm_int1.int1_fsm8 = val.fsm8;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FSM_INT1, (uint8_t *)&fsm_int1, 1);
  }
  if (ret == 0)
  {
    mlc_int1.int1_mlc1 = val.mlc1;
    mlc_int1.int1_mlc2 = val.mlc2;
    mlc_int1.int1_mlc3 = val.mlc3;
    mlc_int1.int1_mlc4 = val.mlc4;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MLC_INT1, (uint8_t *)&mlc_int1, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }
  if (ret == 0)
  {
    if ((val.emb_func_stand_by | val.timestamp) != PROPERTY_DISABLE)
    {
      ctrl4.int2_on_int1 = PROPERTY_ENABLE;
    }
    else
    {
      ctrl4.int2_on_int1 = PROPERTY_DISABLE;
    }
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }

  if (ret == 0)
  {
    int2_ctrl.int2_emb_func_endop = val.emb_func_stand_by;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }


  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    md2_cfg.int2_timestamp = val.timestamp;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    inactivity_dur.sleep_status_on_int = val.sleep_status;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }


  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  }

  if (ret == 0)
  {
    int1_ctrl.int1_drdy_xl = val.drdy_xl;
    int1_ctrl.int1_drdy_g = val.drdy_gy;
    int1_ctrl.int1_fifo_th = val.fifo_th;
    int1_ctrl.int1_fifo_ovr = val.fifo_ovr;
    int1_ctrl.int1_fifo_full = val.fifo_full;
    int1_ctrl.int1_cnt_bdr = val.fifo_bdr;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  }

  if (ret == 0)
  {
    if ((emb_func_int1.int1_fsm_lc
         | emb_func_int1.int1_sig_mot
         | emb_func_int1.int1_step_detector
         | emb_func_int1.int1_tilt
         | fsm_int1.int1_fsm1
         | fsm_int1.int1_fsm2
         | fsm_int1.int1_fsm3
         | fsm_int1.int1_fsm4
         | fsm_int1.int1_fsm5
         | fsm_int1.int1_fsm6
         | fsm_int1.int1_fsm7
         | fsm_int1.int1_fsm8
         | mlc_int1.int1_mlc1
         | mlc_int1.int1_mlc2
         | mlc_int1.int1_mlc3
         | mlc_int1.int1_mlc4) != PROPERTY_DISABLE)
    {
      md1_cfg.int1_emb_func = PROPERTY_ENABLE;
    }
    else
    {
      md1_cfg.int1_emb_func = PROPERTY_DISABLE;
    }
    md1_cfg.int1_6d = val.six_d;
    md1_cfg.int1_double_tap = val.double_tap;
    md1_cfg.int1_ff = val.free_fall;
    md1_cfg.int1_wu = val.wake_up;
    md1_cfg.int1_single_tap = val.single_tap;
    if ((val.sleep_status | val.sleep_change) != PROPERTY_DISABLE)
    {
      md1_cfg.int1_sleep_change = PROPERTY_ENABLE;
    }
    else
    {
      md1_cfg.int1_sleep_change = PROPERTY_DISABLE;
    }
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    pedo_cmd_reg.carry_count_en = val.step_count_overflow;
    ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }


  if (ret == 0)
  {
    ret = st1vafe6ax_pin_int2_route_get(ctx, &pin_int2_route);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }
  if (ret == 0)
  {
    if ((pin_int2_route.six_d
         | pin_int2_route.double_tap
         | pin_int2_route.free_fall
         | pin_int2_route.wake_up
         | pin_int2_route.single_tap
         | pin_int2_route.sleep_status
         | pin_int2_route.sleep_change
         | val.six_d
         | val.double_tap
         | val.free_fall
         | val.wake_up
         | val.single_tap
         | val.sleep_status
         | val.sleep_change) != PROPERTY_DISABLE)
    {
      functions_enable.interrupts_enable = PROPERTY_ENABLE;
    }

    else
    {
      functions_enable.interrupts_enable = PROPERTY_DISABLE;
    }

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  return ret;
}

/**
  * @brief  It routes interrupt signals on INT 1 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It routes interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t *val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_emb_func_int1_t emb_func_int1;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  st1vafe6ax_int1_ctrl_t int1_ctrl;
  st1vafe6ax_int2_ctrl_t int2_ctrl;
  st1vafe6ax_fsm_int1_t fsm_int1;
  st1vafe6ax_mlc_int1_t mlc_int1;
  st1vafe6ax_md1_cfg_t md1_cfg;
  st1vafe6ax_md2_cfg_t md2_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;


  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ctrl4.int2_on_int1 == PROPERTY_ENABLE)
  {
    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
      ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
    }

    if (ret == 0)
    {
      val->emb_func_stand_by = int2_ctrl.int2_emb_func_endop;
      val->timestamp = md2_cfg.int2_timestamp;
    }
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  }


  if (ret == 0)
  {
    val->sleep_status = inactivity_dur.sleep_status_on_int;

    val->drdy_xl   = int1_ctrl.int1_drdy_xl;
    val->drdy_gy   = int1_ctrl.int1_drdy_g;
    val->fifo_th   = int1_ctrl.int1_fifo_th;
    val->fifo_ovr  = int1_ctrl.int1_fifo_ovr;
    val->fifo_full = int1_ctrl.int1_fifo_full;
    val->fifo_bdr  = int1_ctrl.int1_cnt_bdr;

    val->six_d        = md1_cfg.int1_6d;
    val->double_tap   = md1_cfg.int1_double_tap;
    val->free_fall    = md1_cfg.int1_ff;
    val->wake_up      = md1_cfg.int1_wu;
    val->single_tap   = md1_cfg.int1_single_tap;
    val->sleep_change = md1_cfg.int1_sleep_change;
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
      ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_INT1, (uint8_t *)&fsm_int1, 1);
      ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_INT1, (uint8_t *)&mlc_int1, 1);
    }

    if (ret == 0)
    {
      val->step_detector = emb_func_int1.int1_step_detector;
      val->tilt          = emb_func_int1.int1_tilt;
      val->sig_mot       = emb_func_int1.int1_sig_mot;
      val->fsm_lc        = emb_func_int1.int1_fsm_lc;

      val->fsm1 = fsm_int1.int1_fsm1;
      val->fsm2 = fsm_int1.int1_fsm2;
      val->fsm3 = fsm_int1.int1_fsm3;
      val->fsm4 = fsm_int1.int1_fsm4;
      val->fsm5 = fsm_int1.int1_fsm5;
      val->fsm6 = fsm_int1.int1_fsm6;
      val->fsm7 = fsm_int1.int1_fsm7;
      val->fsm8 = fsm_int1.int1_fsm8;

      val->mlc1 = mlc_int1.int1_mlc1;
      val->mlc2 = mlc_int1.int1_mlc2;
      val->mlc3 = mlc_int1.int1_mlc3;
      val->mlc4 = mlc_int1.int1_mlc4;
    }

    ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    val->step_count_overflow = pedo_cmd_reg.carry_count_en;
  }

  return ret;
}


/**
  * @brief  It routes interrupt signals on INT 2 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It routes interrupt signals on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  st1vafe6ax_pin_int_route_t  pin_int1_route;
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_emb_func_int2_t emb_func_int2;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  st1vafe6ax_int2_ctrl_t int2_ctrl;
  st1vafe6ax_fsm_int2_t fsm_int2;
  st1vafe6ax_mlc_int2_t mlc_int2;
  st1vafe6ax_ctrl7_t ctrl7;
  st1vafe6ax_md2_cfg_t md2_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;


  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_INT2, (uint8_t *)&fsm_int2, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_INT2, (uint8_t *)&mlc_int2, 1);
  }

  if (ret == 0)
  {
    emb_func_int2.int2_step_detector = val.step_detector;
    emb_func_int2.int2_tilt = val.tilt;
    emb_func_int2.int2_sig_mot = val.sig_mot;
    emb_func_int2.int2_fsm_lc = val.fsm_lc;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
  }
  if (ret == 0)
  {
    fsm_int2.int2_fsm1 = val.fsm1;
    fsm_int2.int2_fsm2 = val.fsm2;
    fsm_int2.int2_fsm3 = val.fsm3;
    fsm_int2.int2_fsm4 = val.fsm4;
    fsm_int2.int2_fsm5 = val.fsm5;
    fsm_int2.int2_fsm6 = val.fsm6;
    fsm_int2.int2_fsm7 = val.fsm7;
    fsm_int2.int2_fsm8 = val.fsm8;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FSM_INT2, (uint8_t *)&fsm_int2, 1);
  }
  if (ret == 0)
  {
    mlc_int2.int2_mlc1 = val.mlc1;
    mlc_int2.int2_mlc2 = val.mlc2;
    mlc_int2.int2_mlc3 = val.mlc3;
    mlc_int2.int2_mlc4 = val.mlc4;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MLC_INT2, (uint8_t *)&mlc_int2, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }
  if (ret == 0)
  {
    if ((val.emb_func_stand_by | val.timestamp) != PROPERTY_DISABLE)
    {
      ctrl4.int2_on_int1 = PROPERTY_DISABLE;
    }
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    inactivity_dur.sleep_status_on_int = val.sleep_status;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }

  if (ret == 0)
  {
    int2_ctrl.int2_drdy_xl = val.drdy_xl;
    int2_ctrl.int2_drdy_g = val.drdy_gy;
    int2_ctrl.int2_fifo_th = val.fifo_th;
    int2_ctrl.int2_fifo_ovr = val.fifo_ovr;
    int2_ctrl.int2_fifo_full = val.fifo_full;
    int2_ctrl.int2_cnt_bdr = val.fifo_bdr;
    int2_ctrl.int2_emb_func_endop = val.emb_func_stand_by;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
    ctrl7.int2_drdy_ah_bio = val.drdy_ah_bio;
  }

  if (ret == 0)
  {
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    if ((emb_func_int2.int2_fsm_lc
         | emb_func_int2.int2_sig_mot
         | emb_func_int2.int2_step_detector
         | emb_func_int2.int2_tilt
         | fsm_int2.int2_fsm1
         | fsm_int2.int2_fsm2
         | fsm_int2.int2_fsm3
         | fsm_int2.int2_fsm4
         | fsm_int2.int2_fsm5
         | fsm_int2.int2_fsm6
         | fsm_int2.int2_fsm7
         | fsm_int2.int2_fsm8
         | mlc_int2.int2_mlc1
         | mlc_int2.int2_mlc2
         | mlc_int2.int2_mlc3
         | mlc_int2.int2_mlc4) != PROPERTY_DISABLE)
    {
      md2_cfg.int2_emb_func = PROPERTY_ENABLE;
    }
    else
    {
      md2_cfg.int2_emb_func = PROPERTY_DISABLE;
    }
    md2_cfg.int2_6d = val.six_d;
    md2_cfg.int2_double_tap = val.double_tap;
    md2_cfg.int2_ff = val.free_fall;
    md2_cfg.int2_wu = val.wake_up;
    md2_cfg.int2_single_tap = val.single_tap;
    md2_cfg.int2_timestamp = val.timestamp;
    if ((val.sleep_status | val.sleep_change) != PROPERTY_DISABLE)
    {
      md2_cfg.int2_sleep_change = PROPERTY_ENABLE;
    }
    else
    {
      md2_cfg.int2_sleep_change = PROPERTY_DISABLE;
    }
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    pedo_cmd_reg.carry_count_en = val.step_count_overflow;
    ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }


  if (ret == 0)
  {
    ret = st1vafe6ax_pin_int1_route_get(ctx, &pin_int1_route);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }
  if (ret == 0)
  {
    if ((pin_int1_route.six_d
         | pin_int1_route.double_tap
         | pin_int1_route.free_fall
         | pin_int1_route.wake_up
         | pin_int1_route.single_tap
         | pin_int1_route.sleep_status
         | pin_int1_route.sleep_change
         | val.six_d
         | val.double_tap
         | val.free_fall
         | val.wake_up
         | val.single_tap
         | val.sleep_status
         | val.sleep_change) != PROPERTY_DISABLE)
    {
      functions_enable.interrupts_enable = PROPERTY_ENABLE;
    }

    else
    {
      functions_enable.interrupts_enable = PROPERTY_DISABLE;
    }

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  return ret;
}

/**
  * @brief  It routes interrupt signals on INT 2 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It routes interrupt signals on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t *val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_emb_func_int2_t emb_func_int2;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  st1vafe6ax_int2_ctrl_t int2_ctrl;
  st1vafe6ax_fsm_int2_t fsm_int2;
  st1vafe6ax_mlc_int2_t mlc_int2;
  st1vafe6ax_ctrl7_t ctrl7;
  st1vafe6ax_md2_cfg_t md2_cfg;
  st1vafe6ax_ctrl4_t ctrl4;
  int32_t ret;


  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ret != 0)
  {
    return ret;
  }

  if (ctrl4.int2_on_int1 == PROPERTY_DISABLE)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
    if (ret != 0)
    {
      return ret;
    }

    val->timestamp = md2_cfg.int2_timestamp;
    val->emb_func_stand_by = int2_ctrl.int2_emb_func_endop;
  }

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->sleep_status = inactivity_dur.sleep_status_on_int;

  val->drdy_xl = int2_ctrl.int2_drdy_xl;
  val->drdy_gy = int2_ctrl.int2_drdy_g;
  val->fifo_th = int2_ctrl.int2_fifo_th;
  val->fifo_ovr = int2_ctrl.int2_fifo_ovr;
  val->fifo_full = int2_ctrl.int2_fifo_full;
  val->fifo_bdr = int2_ctrl.int2_cnt_bdr;

  val->six_d = md2_cfg.int2_6d;
  val->double_tap = md2_cfg.int2_double_tap;
  val->free_fall = md2_cfg.int2_ff;
  val->wake_up = md2_cfg.int2_wu;
  val->single_tap = md2_cfg.int2_single_tap;
  val->sleep_change = md2_cfg.int2_sleep_change;

  val->drdy_ah_bio = ctrl7.int2_drdy_ah_bio;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_INT2, (uint8_t *)&fsm_int2, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_INT2, (uint8_t *)&mlc_int2, 1);
  }

  if (ret == 0)
  {
    val->step_detector = emb_func_int2.int2_step_detector;
    val->tilt = emb_func_int2.int2_tilt;
    val->sig_mot = emb_func_int2.int2_sig_mot;
    val->fsm_lc = emb_func_int2.int2_fsm_lc;

    val->fsm1 = fsm_int2.int2_fsm1;
    val->fsm2 = fsm_int2.int2_fsm2;
    val->fsm3 = fsm_int2.int2_fsm3;
    val->fsm4 = fsm_int2.int2_fsm4;
    val->fsm5 = fsm_int2.int2_fsm5;
    val->fsm6 = fsm_int2.int2_fsm6;
    val->fsm7 = fsm_int2.int2_fsm7;
    val->fsm8 = fsm_int2.int2_fsm8;

    val->mlc1 = mlc_int2.int2_mlc1;
    val->mlc2 = mlc_int2.int2_mlc2;
    val->mlc3 = mlc_int2.int2_mlc3;
    val->mlc4 = mlc_int2.int2_mlc4;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    val->step_count_overflow = pedo_cmd_reg.carry_count_en;
  }

  return ret;
}

/**
  * @brief  Enables INT pin when I3C is enabled.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables INT pin when I3C is enabled.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int_en_when_i2c_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl5_t ctrl5;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret == 0)
  {
    ctrl5.int_en_i3c = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  }

  return ret;
}

/**
  * @brief  Enables INT pin when I3C is enabled.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables INT pin when I3C is enabled.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_pin_int_en_when_i2c_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl5_t ctrl5;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl5.int_en_i3c;

  return ret;
}

/**
  * @brief  Interrupt notification mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ALL_INT_PULSED, BASE_LATCHED_EMB_PULSED, BASE_PULSED_EMB_LATCHED, ALL_INT_LATCHED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_int_notification_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_int_notification_t val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  st1vafe6ax_page_rw_t page_rw;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.lir = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
    }

    if (ret == 0)
    {
      page_rw.emb_func_lir = ((uint8_t)val & 0x02U) >> 1;
      ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
    }

    ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  }

  return ret;
}

/**
  * @brief  Interrupt notification mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ALL_INT_PULSED, BASE_LATCHED_EMB_PULSED, BASE_PULSED_EMB_LATCHED, ALL_INT_LATCHED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_int_notification_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_int_notification_t *val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  st1vafe6ax_page_rw_t page_rw;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PAGE_RW, (uint8_t *)&page_rw, 1);
    }

    ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  }

  if (ret != 0)
  {
    return ret;
  }

  switch ((page_rw.emb_func_lir << 1) + tap_cfg0.lir)
  {
    case ST1VAFE6AX_ALL_INT_PULSED:
      *val = ST1VAFE6AX_ALL_INT_PULSED;
      break;

    case ST1VAFE6AX_BASE_LATCHED_EMB_PULSED:
      *val = ST1VAFE6AX_BASE_LATCHED_EMB_PULSED;
      break;

    case ST1VAFE6AX_BASE_PULSED_EMB_LATCHED:
      *val = ST1VAFE6AX_BASE_PULSED_EMB_LATCHED;
      break;

    case ST1VAFE6AX_ALL_INT_LATCHED:
      *val = ST1VAFE6AX_ALL_INT_LATCHED;
      break;

    default:
      *val = ST1VAFE6AX_ALL_INT_PULSED;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Wake Up event and Activity / Inactivity detection
  * @brief     This section groups all the functions that manage the Wake Up
  *            event generation.
  * @{
  *
  */

/**
  * @brief  Enable activity/inactivity (sleep) function.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_AND_GY_NOT_AFFECTED, XL_LOW_POWER_GY_NOT_AFFECTED, XL_LOW_POWER_GY_SLEEP, XL_LOW_POWER_GY_POWER_DOWN,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_act_mode_t val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.inact_en = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  return ret;
}

/**
  * @brief  Enable activity/inactivity (sleep) function.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_AND_GY_NOT_AFFECTED, XL_LOW_POWER_GY_NOT_AFFECTED, XL_LOW_POWER_GY_SLEEP, XL_LOW_POWER_GY_POWER_DOWN,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_act_mode_t *val)
{
  st1vafe6ax_functions_enable_t functions_enable;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (functions_enable.inact_en)
  {
    case ST1VAFE6AX_XL_AND_GY_NOT_AFFECTED:
      *val = ST1VAFE6AX_XL_AND_GY_NOT_AFFECTED;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_GY_NOT_AFFECTED:
      *val = ST1VAFE6AX_XL_LOW_POWER_GY_NOT_AFFECTED;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_GY_SLEEP:
      *val = ST1VAFE6AX_XL_LOW_POWER_GY_SLEEP;
      break;

    case ST1VAFE6AX_XL_LOW_POWER_GY_POWER_DOWN:
      *val = ST1VAFE6AX_XL_LOW_POWER_GY_POWER_DOWN;
      break;

    default:
      *val = ST1VAFE6AX_XL_AND_GY_NOT_AFFECTED;
      break;
  }
  return ret;
}

/**
  * @brief  Duration in the transition from Stationary to Motion (from Inactivity to Activity).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLEEP_TO_ACT_AT_1ST_SAMPLE, SLEEP_TO_ACT_AT_2ND_SAMPLE, SLEEP_TO_ACT_AT_3RD_SAMPLE, SLEEP_TO_ACT_AT_4th_SAMPLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_from_sleep_to_act_dur_set(const stmdev_ctx_t *ctx,
                                                 st1vafe6ax_act_from_sleep_to_act_dur_t val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    inactivity_dur.inact_dur = (uint8_t)val & 0x3U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  return ret;
}

/**
  * @brief  Duration in the transition from Stationary to Motion (from Inactivity to Activity).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLEEP_TO_ACT_AT_1ST_SAMPLE, SLEEP_TO_ACT_AT_2ND_SAMPLE, SLEEP_TO_ACT_AT_3RD_SAMPLE, SLEEP_TO_ACT_AT_4th_SAMPLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_from_sleep_to_act_dur_get(const stmdev_ctx_t *ctx,
                                                 st1vafe6ax_act_from_sleep_to_act_dur_t *val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (inactivity_dur.inact_dur)
  {
    case ST1VAFE6AX_SLEEP_TO_ACT_AT_1ST_SAMPLE:
      *val = ST1VAFE6AX_SLEEP_TO_ACT_AT_1ST_SAMPLE;
      break;

    case ST1VAFE6AX_SLEEP_TO_ACT_AT_2ND_SAMPLE:
      *val = ST1VAFE6AX_SLEEP_TO_ACT_AT_2ND_SAMPLE;
      break;

    case ST1VAFE6AX_SLEEP_TO_ACT_AT_3RD_SAMPLE:
      *val = ST1VAFE6AX_SLEEP_TO_ACT_AT_3RD_SAMPLE;
      break;

    case ST1VAFE6AX_SLEEP_TO_ACT_AT_4th_SAMPLE:
      *val = ST1VAFE6AX_SLEEP_TO_ACT_AT_4th_SAMPLE;
      break;

    default:
      *val = ST1VAFE6AX_SLEEP_TO_ACT_AT_1ST_SAMPLE;
      break;
  }
  return ret;
}

/**
  * @brief  Selects the accelerometer data rate during Inactivity.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1Hz875, 15Hz, 30Hz, 60Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_sleep_xl_odr_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_act_sleep_xl_odr_t val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    inactivity_dur.xl_inact_odr = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  return ret;
}

/**
  * @brief  Selects the accelerometer data rate during Inactivity.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1Hz875, 15Hz, 30Hz, 60Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_sleep_xl_odr_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_act_sleep_xl_odr_t *val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (inactivity_dur.xl_inact_odr)
  {
    case ST1VAFE6AX_1Hz875:
      *val = ST1VAFE6AX_1Hz875;
      break;

    case ST1VAFE6AX_15Hz:
      *val = ST1VAFE6AX_15Hz;
      break;

    case ST1VAFE6AX_30Hz:
      *val = ST1VAFE6AX_30Hz;
      break;

    case ST1VAFE6AX_60Hz:
      *val = ST1VAFE6AX_60Hz;
      break;

    default:
      *val = ST1VAFE6AX_1Hz875;
      break;
  }
  return ret;
}

/**
  * @brief  Wakeup and activity/inactivity threshold.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Wakeup and activity/inactivity threshold.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_thresholds_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_act_thresholds_t val)
{
  st1vafe6ax_inactivity_ths_t inactivity_ths;
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  int32_t ret;
  float_t tmp;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);

  if (ret != 0)
  {
    return ret;
  }

  if ((val.wk_ths_mg < (uint32_t)(7.8125f * 63.0f))
      && (val.inact_ths_mg < (uint32_t)(7.8125f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 0;

    tmp = (float_t)val.inact_ths_mg / 7.8125f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 7.8125f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else if ((val.wk_ths_mg < (uint32_t)(15.625f * 63.0f))
           && (val.inact_ths_mg < (uint32_t)(15.625f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 1;

    tmp = (float_t)val.inact_ths_mg / 15.625f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 15.625f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else if ((val.wk_ths_mg < (uint32_t)(31.25f * 63.0f))
           && (val.inact_ths_mg < (uint32_t)(31.25f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 2;

    tmp = (float_t)val.inact_ths_mg / 31.25f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 31.25f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else if ((val.wk_ths_mg < (uint32_t)(62.5f * 63.0f))
           && (val.inact_ths_mg < (uint32_t)(62.5f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 3;

    tmp = (float_t)val.inact_ths_mg / 62.5f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 62.5f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else if ((val.wk_ths_mg < (uint32_t)(125.0f * 63.0f))
           && (val.inact_ths_mg < (uint32_t)(125.0f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 4;

    tmp = (float_t)val.inact_ths_mg / 125.0f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 125.0f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else if ((val.wk_ths_mg < (uint32_t)(250.0f * 63.0f))
           && (val.inact_ths_mg < (uint32_t)(250.0f * 63.0f)))
  {
    inactivity_dur.wu_inact_ths_w = 5;

    tmp = (float_t)val.inact_ths_mg / 250.0f;
    inactivity_ths.inact_ths = (uint8_t)tmp;

    tmp = (float_t)val.wk_ths_mg / 250.0f;
    wake_up_ths.wk_ths = (uint8_t)tmp;
  }
  else // out of limit
  {
    inactivity_dur.wu_inact_ths_w = 5;
    inactivity_ths.inact_ths = 0x3FU;
    wake_up_ths.wk_ths = 0x3FU;
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }
  if (ret == 0)
  {

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  }
  if (ret == 0)
  {

    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  }

  return ret;
}

/**
  * @brief  Wakeup and activity/inactivity threshold.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Wakeup and activity/inactivity threshold.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_thresholds_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_act_thresholds_t *val)
{
  st1vafe6ax_inactivity_dur_t inactivity_dur;
  st1vafe6ax_inactivity_ths_t inactivity_ths;
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  int32_t ret;
  float_t tmp;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);

  if (ret != 0)
  {
    return ret;
  }

  switch (inactivity_dur.wu_inact_ths_w)
  {
    case 0:
      tmp = (float_t)wake_up_ths.wk_ths * 7.8125f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 7.8125f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;

    case 1:
      tmp = (float_t)wake_up_ths.wk_ths * 15.625f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 15.625f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;

    case 2:
      tmp = (float_t)wake_up_ths.wk_ths * 31.25f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 31.25f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;

    case 3:
      tmp = (float_t)wake_up_ths.wk_ths * 62.5f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 62.5f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;

    case 4:
      tmp = (float_t)wake_up_ths.wk_ths * 125.0f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 125.0f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;

    default:
      tmp = (float_t)wake_up_ths.wk_ths * 250.0f;
      val->wk_ths_mg = (uint32_t)tmp;

      tmp = (float_t)inactivity_ths.inact_ths * 250.0f;
      val->inact_ths_mg = (uint32_t)tmp;
      break;
  }

  return ret;
}

/**
  * @brief  Time windows configuration for Wake Up - Activity - Inactivity (SLEEP, WAKE). Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR) 1 LSB = 512/ODR_XL time. Wake up duration event. 1 LSB = 1/ODR_XL time. [set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Wake Up - Activity - Inactivity (SLEEP, WAKE). Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR) 1 LSB = 512/ODR_XL time. Wake up duration event. 1 LSB = 1/ODR_XL time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_wkup_time_windows_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_act_wkup_time_windows_t val)
{
  st1vafe6ax_wake_up_dur_t wake_up_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret == 0)
  {
    wake_up_dur.wake_dur = val.shock;
    wake_up_dur.sleep_dur = val.quiet;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  }

  return ret;
}

/**
  * @brief  Time windows configuration for Wake Up - Activity - Inactivity (SLEEP, WAKE). Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR) 1 LSB = 512/ODR_XL time. Wake up duration event. 1 LSB = 1/ODR_XL time. [get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Wake Up - Activity - Inactivity (SLEEP, WAKE). Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR) 1 LSB = 512/ODR_XL time. Wake up duration event. 1 LSB = 1/ODR_XL time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_act_wkup_time_windows_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_act_wkup_time_windows_t *val)
{
  st1vafe6ax_wake_up_dur_t wake_up_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->shock = wake_up_dur.wake_dur;
  val->quiet = wake_up_dur.sleep_dur;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Tap Generator
  * @brief     This section groups all the functions that manage the
  *            tap and double tap event generation.
  * @{
  *
  */

/**
  * @brief  Enable axis for Tap - Double Tap detection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable axis for Tap - Double Tap detection.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_detection_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_tap_detection_t val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.tap_x_en = val.tap_x_en;
    tap_cfg0.tap_y_en = val.tap_y_en;
    tap_cfg0.tap_z_en = val.tap_z_en;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  Enable axis for Tap - Double Tap detection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable axis for Tap - Double Tap detection.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_detection_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_tap_detection_t *val)
{
  st1vafe6ax_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->tap_x_en = tap_cfg0.tap_x_en;
  val->tap_y_en = tap_cfg0.tap_y_en;
  val->tap_z_en = tap_cfg0.tap_z_en;

  return ret;
}

/**
  * @brief  axis Tap - Double Tap recognition thresholds.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      axis Tap - Double Tap recognition thresholds.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_thresholds_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_tap_thresholds_t val)
{
  st1vafe6ax_tap_ths_6d_t tap_ths_6d;
  st1vafe6ax_tap_cfg2_t tap_cfg2;
  st1vafe6ax_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  if (ret != 0)
  {
    return ret;
  }

  tap_cfg1.tap_ths_z = val.z;
  tap_cfg2.tap_ths_y = val.y;
  tap_ths_6d.tap_ths_x = val.x;

  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  }

  return ret;
}

/**
  * @brief  axis Tap - Double Tap recognition thresholds.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      axis Tap - Double Tap recognition thresholds.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_thresholds_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_tap_thresholds_t *val)
{
  st1vafe6ax_tap_ths_6d_t tap_ths_6d;
  st1vafe6ax_tap_cfg2_t tap_cfg2;
  st1vafe6ax_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->z  = tap_cfg1.tap_ths_z;
  val->y = tap_cfg2.tap_ths_y;
  val->x = tap_ths_6d.tap_ths_x;

  return ret;
}

/**
  * @brief  Selection of axis priority for TAP detection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XYZ , YXZ , XZY, ZYX , YZX , ZXY ,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_axis_priority_set(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_tap_axis_priority_t val)
{
  st1vafe6ax_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  if (ret == 0)
  {
    tap_cfg1.tap_priority = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  }

  return ret;
}

/**
  * @brief  Selection of axis priority for TAP detection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XYZ , YXZ , XZY, ZYX , YZX , ZXY ,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_axis_priority_get(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_tap_axis_priority_t *val)
{
  st1vafe6ax_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (tap_cfg1.tap_priority)
  {
    case ST1VAFE6AX_XYZ :
      *val = ST1VAFE6AX_XYZ ;
      break;

    case ST1VAFE6AX_YXZ :
      *val = ST1VAFE6AX_YXZ ;
      break;

    case ST1VAFE6AX_XZY:
      *val = ST1VAFE6AX_XZY;
      break;

    case ST1VAFE6AX_ZYX :
      *val = ST1VAFE6AX_ZYX ;
      break;

    case ST1VAFE6AX_YZX :
      *val = ST1VAFE6AX_YZX ;
      break;

    case ST1VAFE6AX_ZXY :
      *val = ST1VAFE6AX_ZXY ;
      break;

    default:
      *val = ST1VAFE6AX_XYZ ;
      break;
  }
  return ret;
}


/**
  * @brief  Time windows configuration for Tap - Double Tap SHOCK, QUIET, DUR : SHOCK Maximum duration is the maximum time of an overthreshold signal detection to be recognized as a tap event. The default value of these bits is 00b which corresponds to 4/ODR_XL time. If the SHOCK bits are set to a different value, 1LSB corresponds to 8/ODR_XL time. QUIET Expected quiet time after a tap detection. Quiet time is the time after the first detected tap in which there must not be any overthreshold event. The default value of these bits is 00b which corresponds to 2/ODR_XL time. If the QUIET bits are set to a different value, 1LSB corresponds to 4/ODR_XL time. DUR Duration of maximum time gap for double tap recognition. When double tap recognition is enabled, this register expresses the maximum time between two consecutive detected taps to determine a double tap event. The default value of these bits is 0000b which corresponds to 16/ODR_XL time. If the DUR_[3:0] bits are set to a different value, 1LSB corresponds to 32/ODR_XL time.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Tap - Double Tap SHOCK, QUIET, DUR : SHOCK Maximum duration is the maximum time of an overthreshold signal detection to be recognized as a tap event. The default value of these bits is 00b which corresponds to 4/ODR_XL time. If the SHOCK bits are set to a different value, 1LSB corresponds to 8/ODR_XL time. QUIET Expected quiet time after a tap detection. Quiet time is the time after the first detected tap in which there must not be any overthreshold event. The default value of these bits is 00b which corresponds to 2/ODR_XL time. If the QUIET bits are set to a different value, 1LSB corresponds to 4/ODR_XL time. DUR Duration of maximum time gap for double tap recognition. When double tap recognition is enabled, this register expresses the maximum time between two consecutive detected taps to determine a double tap event. The default value of these bits is 0000b which corresponds to 16/ODR_XL time. If the DUR_[3:0] bits are set to a different value, 1LSB corresponds to 32/ODR_XL time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_time_windows_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_tap_time_windows_t val)
{
  st1vafe6ax_tap_dur_t tap_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_DUR, (uint8_t *)&tap_dur, 1);
  if (ret == 0)
  {
    tap_dur.shock = val.shock;
    tap_dur.quiet = val.quiet;
    tap_dur.dur = val.tap_gap;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_DUR, (uint8_t *)&tap_dur, 1);
  }

  return ret;
}

/**
  * @brief  Time windows configuration for Tap - Double Tap SHOCK, QUIET, DUR : SHOCK Maximum duration is the maximum time of an overthreshold signal detection to be recognized as a tap event. The default value of these bits is 00b which corresponds to 4/ODR_XL time. If the SHOCK bits are set to a different value, 1LSB corresponds to 8/ODR_XL time. QUIET Expected quiet time after a tap detection. Quiet time is the time after the first detected tap in which there must not be any overthreshold event. The default value of these bits is 00b which corresponds to 2/ODR_XL time. If the QUIET bits are set to a different value, 1LSB corresponds to 4/ODR_XL time. DUR Duration of maximum time gap for double tap recognition. When double tap recognition is enabled, this register expresses the maximum time between two consecutive detected taps to determine a double tap event. The default value of these bits is 0000b which corresponds to 16/ODR_XL time. If the DUR_[3:0] bits are set to a different value, 1LSB corresponds to 32/ODR_XL time.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Tap - Double Tap SHOCK, QUIET, DUR : SHOCK Maximum duration is the maximum time of an overthreshold signal detection to be recognized as a tap event. The default value of these bits is 00b which corresponds to 4/ODR_XL time. If the SHOCK bits are set to a different value, 1LSB corresponds to 8/ODR_XL time. QUIET Expected quiet time after a tap detection. Quiet time is the time after the first detected tap in which there must not be any overthreshold event. The default value of these bits is 00b which corresponds to 2/ODR_XL time. If the QUIET bits are set to a different value, 1LSB corresponds to 4/ODR_XL time. DUR Duration of maximum time gap for double tap recognition. When double tap recognition is enabled, this register expresses the maximum time between two consecutive detected taps to determine a double tap event. The default value of these bits is 0000b which corresponds to 16/ODR_XL time. If the DUR_[3:0] bits are set to a different value, 1LSB corresponds to 32/ODR_XL time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_time_windows_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_tap_time_windows_t *val)
{
  st1vafe6ax_tap_dur_t tap_dur;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_DUR, (uint8_t *)&tap_dur, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->shock = tap_dur.shock;
  val->quiet = tap_dur.quiet;
  val->tap_gap = tap_dur.dur;

  return ret;
}

/**
  * @brief  Single/double-tap event enable.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ONLY_SINGLE, BOTH_SINGLE_DOUBLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_tap_mode_t val)
{
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  if (ret == 0)
  {
    wake_up_ths.single_double_tap = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  }

  return ret;
}

/**
  * @brief  Single/double-tap event enable.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ONLY_SINGLE, BOTH_SINGLE_DOUBLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tap_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_tap_mode_t *val)
{
  st1vafe6ax_wake_up_ths_t wake_up_ths;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (wake_up_ths.single_double_tap)
  {
    case ST1VAFE6AX_ONLY_SINGLE:
      *val = ST1VAFE6AX_ONLY_SINGLE;
      break;

    case ST1VAFE6AX_BOTH_SINGLE_DOUBLE:
      *val = ST1VAFE6AX_BOTH_SINGLE_DOUBLE;
      break;

    default:
      *val = ST1VAFE6AX_ONLY_SINGLE;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Six position detection (6D)
  * @brief   This section groups all the functions concerning six position
  *          detection (6D).
  * @{
  *
  */

/**
  * @brief  Threshold for 4D/6D function.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DEG_80, DEG_70, DEG_60, DEG_50,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_6d_threshold_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_6d_threshold_t val)
{
  st1vafe6ax_tap_ths_6d_t tap_ths_6d;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  if (ret == 0)
  {
    tap_ths_6d.sixd_ths = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  }

  return ret;
}

/**
  * @brief  Threshold for 4D/6D function.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DEG_80, DEG_70, DEG_60, DEG_50,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_6d_threshold_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_6d_threshold_t *val)
{
  st1vafe6ax_tap_ths_6d_t tap_ths_6d;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (tap_ths_6d.sixd_ths)
  {
    case ST1VAFE6AX_DEG_80:
      *val = ST1VAFE6AX_DEG_80;
      break;

    case ST1VAFE6AX_DEG_70:
      *val = ST1VAFE6AX_DEG_70;
      break;

    case ST1VAFE6AX_DEG_60:
      *val = ST1VAFE6AX_DEG_60;
      break;

    case ST1VAFE6AX_DEG_50:
      *val = ST1VAFE6AX_DEG_50;
      break;

    default:
      *val = ST1VAFE6AX_DEG_80;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Free fall
  * @brief   This section group all the functions concerning the free
  *          fall detection.
  * @{
  *
  */

/**
  * @brief  Time windows configuration for Free Fall detection 1 LSB = 1/ODR_XL time[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Free Fall detection 1 LSB = 1/ODR_XL time
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ff_time_windows_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_wake_up_dur_t wake_up_dur;
  st1vafe6ax_free_fall_t free_fall;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret == 0)
  {
    wake_up_dur.ff_dur = ((uint8_t)val & 0x20U) >> 5;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  if (ret == 0)
  {
    free_fall.ff_dur = (uint8_t)val & 0x1FU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  return ret;
}

/**
  * @brief  Time windows configuration for Free Fall detection 1 LSB = 1/ODR_XL time[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time windows configuration for Free Fall detection 1 LSB = 1/ODR_XL time
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ff_time_windows_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_wake_up_dur_t wake_up_dur;
  st1vafe6ax_free_fall_t free_fall;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = (wake_up_dur.ff_dur << 5) + free_fall.ff_dur;

  return ret;
}

/**
  * @brief  Free fall threshold setting.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      156_mg, 219_mg, 250_mg, 312_mg, 344_mg, 406_mg, 469_mg, 500_mg,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ff_thresholds_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_ff_thresholds_t val)
{
  st1vafe6ax_free_fall_t free_fall;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  if (ret == 0)
  {
    free_fall.ff_ths = (uint8_t)val & 0x7U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  return ret;
}

/**
  * @brief  Free fall threshold setting.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      156_mg, 219_mg, 250_mg, 312_mg, 344_mg, 406_mg, 469_mg, 500_mg,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ff_thresholds_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_ff_thresholds_t *val)
{
  st1vafe6ax_free_fall_t free_fall;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FREE_FALL, (uint8_t *)&free_fall, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (free_fall.ff_ths)
  {
    case ST1VAFE6AX_156_mg:
      *val = ST1VAFE6AX_156_mg;
      break;

    case ST1VAFE6AX_219_mg:
      *val = ST1VAFE6AX_219_mg;
      break;

    case ST1VAFE6AX_250_mg:
      *val = ST1VAFE6AX_250_mg;
      break;

    case ST1VAFE6AX_312_mg:
      *val = ST1VAFE6AX_312_mg;
      break;

    case ST1VAFE6AX_344_mg:
      *val = ST1VAFE6AX_344_mg;
      break;

    case ST1VAFE6AX_406_mg:
      *val = ST1VAFE6AX_406_mg;
      break;

    case ST1VAFE6AX_469_mg:
      *val = ST1VAFE6AX_469_mg;
      break;

    case ST1VAFE6AX_500_mg:
      *val = ST1VAFE6AX_500_mg;
      break;

    default:
      *val = ST1VAFE6AX_156_mg;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  FIFO
  * @brief   This section group all the functions concerning the fifo usage
  * @{
  *
  */

/**
  * @brief  FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (6 Bytes) written in FIFO).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (6 Bytes) written in FIFO).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_fifo_ctrl1_t fifo_ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);

  if (ret == 0)
  {
    fifo_ctrl1.wtm = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (6 Bytes) written in FIFO).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (6 Bytes) written in FIFO).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_fifo_ctrl1_t fifo_ctrl1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fifo_ctrl1.wtm;

  return ret;
}

/**
  * @brief  When dual channel mode is enabled, this function enables FSM-triggered batching in FIFO of accelerometer channel 2.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      When dual channel mode is enabled, this function enables FSM-triggered batching in FIFO of accelerometer channel 2.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_xl_dual_fsm_batch_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.xl_dualc_batch_from_fsm = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  When dual channel mode is enabled, this function enables FSM-triggered batching in FIFO of accelerometer channel 2.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      When dual channel mode is enabled, this function enables FSM-triggered batching in FIFO of accelerometer channel 2.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_xl_dual_fsm_batch_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fifo_ctrl2.xl_dualc_batch_from_fsm;

  return ret;
}

/**
  * @brief  It configures the compression algorithm to write non-compressed data at each rate.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      CMP_DISABLE, CMP_ALWAYS, CMP_8_TO_1, CMP_16_TO_1, CMP_32_TO_1,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_compress_algo_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_fifo_compress_algo_t val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.uncompr_rate = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  It configures the compression algorithm to write non-compressed data at each rate.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      CMP_DISABLE, CMP_ALWAYS, CMP_8_TO_1, CMP_16_TO_1, CMP_32_TO_1,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_compress_algo_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_fifo_compress_algo_t *val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl2.uncompr_rate)
  {
    case ST1VAFE6AX_CMP_DISABLE:
      *val = ST1VAFE6AX_CMP_DISABLE;
      break;

    case ST1VAFE6AX_CMP_8_TO_1:
      *val = ST1VAFE6AX_CMP_8_TO_1;
      break;

    case ST1VAFE6AX_CMP_16_TO_1:
      *val = ST1VAFE6AX_CMP_16_TO_1;
      break;

    case ST1VAFE6AX_CMP_32_TO_1:
      *val = ST1VAFE6AX_CMP_32_TO_1;
      break;

    default:
      *val = ST1VAFE6AX_CMP_DISABLE;
      break;
  }
  return ret;
}

/**
  * @brief  Enables ODR CHANGE virtual sensor to be batched in FIFO.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables ODR CHANGE virtual sensor to be batched in FIFO.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_virtual_sens_odr_chg_set(const stmdev_ctx_t *ctx,
                                                 uint8_t val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.odr_chg_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Enables ODR CHANGE virtual sensor to be batched in FIFO.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables ODR CHANGE virtual sensor to be batched in FIFO.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_virtual_sens_odr_chg_get(const stmdev_ctx_t *ctx,
                                                 uint8_t *val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fifo_ctrl2.odr_chg_en;

  return ret;
}

/**
  * @brief  Enables/Disables compression algorithm runtime.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables/Disables compression algorithm runtime.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_compress_algo_real_time_set(const stmdev_ctx_t *ctx,
                                                    uint8_t val)
{
  st1vafe6ax_emb_func_en_b_t emb_func_en_b;
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;

  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.fifo_compr_rt_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    if (ret == 0)
    {
      ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
    }
    if (ret == 0)
    {
      emb_func_en_b.fifo_compr_en = val;
      ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
    }

    ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  }

  return ret;
}

/**
  * @brief  Enables/Disables compression algorithm runtime.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables/Disables compression algorithm runtime.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_compress_algo_real_time_get(const stmdev_ctx_t *ctx,
                                                    uint8_t *val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fifo_ctrl2.fifo_compr_rt_en;

  return ret;
}

/**
  * @brief  Sensing chain FIFO stop values memorization at threshold level.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensing chain FIFO stop values memorization at threshold level.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_stop_on_wtm_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.stop_on_wtm = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Sensing chain FIFO stop values memorization at threshold level.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensing chain FIFO stop values memorization at threshold level.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_stop_on_wtm_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fifo_ctrl2.stop_on_wtm;

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for accelerometer data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_fifo_xl_batch_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_xl_batch_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_xl_batch_t val)
{
  st1vafe6ax_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.bdr_xl = (uint8_t)val & 0xFU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for accelerometer data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_fifo_xl_batch_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_xl_batch_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_xl_batch_t *val)
{
  st1vafe6ax_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl3.bdr_xl)
  {
    case ST1VAFE6AX_XL_NOT_BATCHED:
      *val = ST1VAFE6AX_XL_NOT_BATCHED;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_1Hz875:
      *val = ST1VAFE6AX_XL_BATCHED_AT_1Hz875;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_7Hz5:
      *val = ST1VAFE6AX_XL_BATCHED_AT_7Hz5;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_15Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_15Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_30Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_30Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_60Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_60Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_120Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_120Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_240Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_240Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_480Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_480Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_960Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_960Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_1920Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_1920Hz;
      break;

    case ST1VAFE6AX_XL_BATCHED_AT_3840Hz:
      *val = ST1VAFE6AX_XL_BATCHED_AT_3840Hz;
      break;

    default:
      *val = ST1VAFE6AX_XL_NOT_BATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for gyroscope data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_fifo_gy_batch_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_gy_batch_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_gy_batch_t val)
{
  st1vafe6ax_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.bdr_gy = (uint8_t)val & 0xFU;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for gyroscope data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_fifo_gy_batch_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_gy_batch_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_gy_batch_t *val)
{
  st1vafe6ax_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl3.bdr_gy)
  {
    case ST1VAFE6AX_GY_NOT_BATCHED:
      *val = ST1VAFE6AX_GY_NOT_BATCHED;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_1Hz875:
      *val = ST1VAFE6AX_GY_BATCHED_AT_1Hz875;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_7Hz5:
      *val = ST1VAFE6AX_GY_BATCHED_AT_7Hz5;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_15Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_15Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_30Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_30Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_60Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_60Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_120Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_120Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_240Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_240Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_480Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_480Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_960Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_960Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_1920Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_1920Hz;
      break;

    case ST1VAFE6AX_GY_BATCHED_AT_3840Hz:
      *val = ST1VAFE6AX_GY_BATCHED_AT_3840Hz;
      break;

    default:
      *val = ST1VAFE6AX_GY_NOT_BATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  FIFO mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BYPASS_MODE, FIFO_MODE, STREAM_WTM_TO_FULL_MODE, STREAM_TO_FIFO_MODE, BYPASS_TO_STREAM_MODE, STREAM_MODE, BYPASS_TO_FIFO_MODE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mode_set(const stmdev_ctx_t *ctx,
                                 st1vafe6ax_fifo_mode_t val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.fifo_mode = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BYPASS_MODE, FIFO_MODE, STREAM_WTM_TO_FULL_MODE, STREAM_TO_FIFO_MODE, BYPASS_TO_STREAM_MODE, STREAM_MODE, BYPASS_TO_FIFO_MODE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mode_get(const stmdev_ctx_t *ctx,
                                 st1vafe6ax_fifo_mode_t *val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl4.fifo_mode)
  {
    case ST1VAFE6AX_BYPASS_MODE:
      *val = ST1VAFE6AX_BYPASS_MODE;
      break;

    case ST1VAFE6AX_FIFO_MODE:
      *val = ST1VAFE6AX_FIFO_MODE;
      break;

    case ST1VAFE6AX_STREAM_WTM_TO_FULL_MODE:
      *val = ST1VAFE6AX_STREAM_WTM_TO_FULL_MODE;
      break;

    case ST1VAFE6AX_STREAM_TO_FIFO_MODE:
      *val = ST1VAFE6AX_STREAM_TO_FIFO_MODE;
      break;

    case ST1VAFE6AX_BYPASS_TO_STREAM_MODE:
      *val = ST1VAFE6AX_BYPASS_TO_STREAM_MODE;
      break;

    case ST1VAFE6AX_STREAM_MODE:
      *val = ST1VAFE6AX_STREAM_MODE;
      break;

    case ST1VAFE6AX_BYPASS_TO_FIFO_MODE:
      *val = ST1VAFE6AX_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = ST1VAFE6AX_BYPASS_MODE;
      break;
  }
  return ret;
}

/**
  * @brief  Selects batch data rate (write frequency in FIFO) for temperature data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TEMP_NOT_BATCHED, TEMP_BATCHED_AT_1Hz875, TEMP_BATCHED_AT_15Hz, TEMP_BATCHED_AT_60Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_temp_batch_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_temp_batch_t val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.odr_t_batch = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  Selects batch data rate (write frequency in FIFO) for temperature data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TEMP_NOT_BATCHED, TEMP_BATCHED_AT_1Hz875, TEMP_BATCHED_AT_15Hz, TEMP_BATCHED_AT_60Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_temp_batch_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_temp_batch_t *val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl4.odr_t_batch)
  {
    case ST1VAFE6AX_TEMP_NOT_BATCHED:
      *val = ST1VAFE6AX_TEMP_NOT_BATCHED;
      break;

    case ST1VAFE6AX_TEMP_BATCHED_AT_1Hz875:
      *val = ST1VAFE6AX_TEMP_BATCHED_AT_1Hz875;
      break;

    case ST1VAFE6AX_TEMP_BATCHED_AT_15Hz:
      *val = ST1VAFE6AX_TEMP_BATCHED_AT_15Hz;
      break;

    case ST1VAFE6AX_TEMP_BATCHED_AT_60Hz:
      *val = ST1VAFE6AX_TEMP_BATCHED_AT_60Hz;
      break;

    default:
      *val = ST1VAFE6AX_TEMP_NOT_BATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  Selects decimation for timestamp batching in FIFO. Write rate will be the maximum rate between XL and GYRO BDR divided by decimation decoder.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TMSTMP_NOT_BATCHED, TMSTMP_DEC_1, TMSTMP_DEC_8, TMSTMP_DEC_32,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_timestamp_batch_set(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_timestamp_batch_t val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.dec_ts_batch = (uint8_t)val & 0x3U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  Selects decimation for timestamp batching in FIFO. Write rate will be the maximum rate between XL and GYRO BDR divided by decimation decoder.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TMSTMP_NOT_BATCHED, TMSTMP_DEC_1, TMSTMP_DEC_8, TMSTMP_DEC_32,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_timestamp_batch_get(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_timestamp_batch_t *val)
{
  st1vafe6ax_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (fifo_ctrl4.dec_ts_batch)
  {
    case ST1VAFE6AX_TMSTMP_NOT_BATCHED:
      *val = ST1VAFE6AX_TMSTMP_NOT_BATCHED;
      break;

    case ST1VAFE6AX_TMSTMP_DEC_1:
      *val = ST1VAFE6AX_TMSTMP_DEC_1;
      break;

    case ST1VAFE6AX_TMSTMP_DEC_8:
      *val = ST1VAFE6AX_TMSTMP_DEC_8;
      break;

    case ST1VAFE6AX_TMSTMP_DEC_32:
      *val = ST1VAFE6AX_TMSTMP_DEC_32;
      break;

    default:
      *val = ST1VAFE6AX_TMSTMP_NOT_BATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  The threshold for the internal counter of batch events. When this counter reaches the threshold, the counter is reset and the interrupt flag is set to 1.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      The threshold for the internal counter of batch events. When this counter reaches the threshold, the counter is reset and the interrupt flag is set to 1.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_counter_threshold_set(const stmdev_ctx_t *ctx,
                                                    uint16_t val)
{
  st1vafe6ax_counter_bdr_reg1_t counter_bdr_reg1;
  st1vafe6ax_counter_bdr_reg2_t counter_bdr_reg2;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);

  if (ret == 0)
  {
    counter_bdr_reg2.cnt_bdr_th = (uint8_t)val & 0xFFU;
    counter_bdr_reg1.cnt_bdr_th = (uint8_t)(val >> 8) & 0x3U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG2, (uint8_t *)&counter_bdr_reg2, 1);
  }

  return ret;
}

/**
  * @brief  The threshold for the internal counter of batch events. When this counter reaches the threshold, the counter is reset and the interrupt flag is set to 1.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      The threshold for the internal counter of batch events. When this counter reaches the threshold, the counter is reset and the interrupt flag is set to 1.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_counter_threshold_get(const stmdev_ctx_t *ctx,
                                                    uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = (uint16_t)buff[0] & 0x3U;
  *val = (*val * 256U) + (uint16_t)buff[1];

  return ret;
}

/**
  * @brief  Enables AH_BIO batching in FIFO.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables AH_BIO batching in FIFO.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_ah_bio_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  if (ret == 0)
  {
    counter_bdr_reg1.ah_bio_batch_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Enables AH_BIO batching in FIFO.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables AH_BIO batching in FIFO.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_ah_bio_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = counter_bdr_reg1.ah_bio_batch_en;

  return ret;
}

/**
  * @brief  Selects the trigger for the internal counter of batch events between the accelerometer, gyroscope.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_BATCH_EVENT, GY_BATCH_EVENT
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_cnt_event_set(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_batch_cnt_event_t val)
{
  st1vafe6ax_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);

  if (ret == 0)
  {
    counter_bdr_reg1.trig_counter_bdr = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Selects the trigger for the internal counter of batch events between the accelerometer, gyroscope.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_BATCH_EVENT, GY_BATCH_EVENT
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_batch_cnt_event_get(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_batch_cnt_event_t *val)
{
  st1vafe6ax_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (counter_bdr_reg1.trig_counter_bdr)
  {
    case ST1VAFE6AX_XL_BATCH_EVENT:
      *val = ST1VAFE6AX_XL_BATCH_EVENT;
      break;

    case ST1VAFE6AX_GY_BATCH_EVENT:
      *val = ST1VAFE6AX_GY_BATCH_EVENT;
      break;

    default:
      *val = ST1VAFE6AX_XL_BATCH_EVENT;
      break;
  }
  return ret;
}

/**
  * @brief  Batching in FIFO buffer of SFLP.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of SFLP values.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_sflp_batch_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_sflp_raw_t val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_fifo_en_a.sflp_game_fifo_en = val.game_rotation;
    emb_func_fifo_en_a.sflp_gravity_fifo_en = val.gravity;
    emb_func_fifo_en_a.sflp_gbias_fifo_en = val.gbias;
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A,
                                (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Batching in FIFO buffer of SFLP.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of SFLP values.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_sflp_batch_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_sflp_raw_t *val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    val->game_rotation = emb_func_fifo_en_a.sflp_game_fifo_en;
    val->gravity = emb_func_fifo_en_a.sflp_gravity_fifo_en;
    val->gbias = emb_func_fifo_en_a.sflp_gbias_fifo_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Status of FIFO.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Status of FIFO (level and flags).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_status_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_fifo_status_t *val)
{
  uint8_t buff[2];
  st1vafe6ax_fifo_status2_t status;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_STATUS1, (uint8_t *)&buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  bytecpy((uint8_t *)&status, &buff[1]);

  val->fifo_bdr = status.counter_bdr_ia;
  val->fifo_ovr = status.fifo_ovr_ia;
  val->fifo_full = status.fifo_full_ia;
  val->fifo_th = status.fifo_wtm_ia;

  val->fifo_level = (uint16_t)buff[1] & 0x01U;
  val->fifo_level = (val->fifo_level * 256U) + buff[0];

  return ret;
}

/**
  * @brief  FIFO data output[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      st1vafe6ax_fifo_out_raw_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_out_raw_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_fifo_out_raw_t *val)
{
  st1vafe6ax_fifo_data_out_tag_t fifo_data_out_tag;
  uint8_t buff[7];
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FIFO_DATA_OUT_TAG, buff, 7);
  if (ret != 0)
  {
    return ret;
  }

  bytecpy((uint8_t *)&fifo_data_out_tag, &buff[0]);

  switch (fifo_data_out_tag.tag_sensor)
  {
    case ST1VAFE6AX_FIFO_EMPTY:
      val->tag = ST1VAFE6AX_FIFO_EMPTY;
      break;

    case ST1VAFE6AX_GY_NC_TAG:
      val->tag = ST1VAFE6AX_GY_NC_TAG;
      break;

    case ST1VAFE6AX_XL_NC_TAG:
      val->tag = ST1VAFE6AX_XL_NC_TAG;
      break;

    case ST1VAFE6AX_TIMESTAMP_TAG:
      val->tag = ST1VAFE6AX_TIMESTAMP_TAG;
      break;

    case ST1VAFE6AX_TEMPERATURE_TAG:
      val->tag = ST1VAFE6AX_TEMPERATURE_TAG;
      break;

    case ST1VAFE6AX_CFG_CHANGE_TAG:
      val->tag = ST1VAFE6AX_CFG_CHANGE_TAG;
      break;

    case ST1VAFE6AX_XL_NC_T_2_TAG:
      val->tag = ST1VAFE6AX_XL_NC_T_2_TAG;
      break;

    case ST1VAFE6AX_XL_NC_T_1_TAG:
      val->tag = ST1VAFE6AX_XL_NC_T_1_TAG;
      break;

    case ST1VAFE6AX_XL_2XC_TAG:
      val->tag = ST1VAFE6AX_XL_2XC_TAG;
      break;

    case ST1VAFE6AX_XL_3XC_TAG:
      val->tag = ST1VAFE6AX_XL_3XC_TAG;
      break;

    case ST1VAFE6AX_GY_NC_T_2_TAG:
      val->tag = ST1VAFE6AX_GY_NC_T_2_TAG;
      break;

    case ST1VAFE6AX_GY_NC_T_1_TAG:
      val->tag = ST1VAFE6AX_GY_NC_T_1_TAG;
      break;

    case ST1VAFE6AX_GY_2XC_TAG:
      val->tag = ST1VAFE6AX_GY_2XC_TAG;
      break;

    case ST1VAFE6AX_GY_3XC_TAG:
      val->tag = ST1VAFE6AX_GY_3XC_TAG;
      break;

    case ST1VAFE6AX_STEP_COUNTER_TAG:
      val->tag = ST1VAFE6AX_STEP_COUNTER_TAG;
      break;

    case ST1VAFE6AX_MLC_RESULT_TAG:
      val->tag = ST1VAFE6AX_MLC_RESULT_TAG;
      break;

    case ST1VAFE6AX_SFLP_GAME_ROTATION_VECTOR_TAG:
      val->tag = ST1VAFE6AX_SFLP_GAME_ROTATION_VECTOR_TAG;
      break;

    case ST1VAFE6AX_SFLP_GYROSCOPE_BIAS_TAG:
      val->tag = ST1VAFE6AX_SFLP_GYROSCOPE_BIAS_TAG;
      break;

    case ST1VAFE6AX_SFLP_GRAVITY_VECTOR_TAG:
      val->tag = ST1VAFE6AX_SFLP_GRAVITY_VECTOR_TAG;
      break;

    case ST1VAFE6AX_MLC_FILTER:
      val->tag = ST1VAFE6AX_MLC_FILTER;
      break;

    case ST1VAFE6AX_MLC_FEATURE:
      val->tag = ST1VAFE6AX_MLC_FEATURE;
      break;

    case ST1VAFE6AX_XL_DUAL_CORE:
      val->tag = ST1VAFE6AX_XL_DUAL_CORE;
      break;

    case ST1VAFE6AX_AH_VAFE:
      val->tag = ST1VAFE6AX_AH_VAFE;
      break;

    default:
      val->tag = ST1VAFE6AX_FIFO_EMPTY;
      break;
  }

  val->cnt = fifo_data_out_tag.tag_cnt;

  val->data[0] = buff[1];
  val->data[1] = buff[2];
  val->data[2] = buff[3];
  val->data[3] = buff[4];
  val->data[4] = buff[5];
  val->data[5] = buff[6];

  return ret;
}

/**
  * @brief  Batching in FIFO buffer of step counter value.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of step counter value.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_stpcnt_batch_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_fifo_en_a.step_counter_fifo_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Batching in FIFO buffer of step counter value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of step counter value.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_stpcnt_batch_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    *val = emb_func_fifo_en_a.step_counter_fifo_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);


  return ret;
}

/**
  * @brief  Batching in FIFO buffer of machine learning core results.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of machine learning core results.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mlc_batch_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_fifo_en_a.mlc_fifo_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Batching in FIFO buffer of machine learning core results.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Batching in FIFO buffer of machine learning core results.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mlc_batch_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    *val = emb_func_fifo_en_a.mlc_fifo_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables batching in FIFO buffer of machine learning core filters and features.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables batching in FIFO buffer of machine learning core filters and features.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mlc_filt_batch_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_fifo_en_b_t emb_func_fifo_en_b;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_B, (uint8_t *)&emb_func_fifo_en_b, 1);
  }

  if (ret == 0)
  {
    emb_func_fifo_en_b.mlc_filter_feature_fifo_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_B, (uint8_t *)&emb_func_fifo_en_b, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables batching in FIFO buffer of machine learning core filters and features.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables batching in FIFO buffer of machine learning core filters and features.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fifo_mlc_filt_batch_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_fifo_en_b_t emb_func_fifo_en_b;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_FIFO_EN_B, (uint8_t *)&emb_func_fifo_en_b, 1);
  }

  if (ret == 0)
  {
    *val = emb_func_fifo_en_b.mlc_filter_feature_fifo_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Step Counter
  * @brief     This section groups all the functions that manage pedometer.
  * @{
  *
  */

/**
  * @brief  Step counter mode[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      false_step_rej, step_counter, step_detector,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_mode_set(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_stpcnt_mode_t val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  st1vafe6ax_emb_func_en_b_t emb_func_en_b;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }
  if ((val.false_step_rej == PROPERTY_ENABLE)
      && ((emb_func_en_a.mlc_before_fsm_en & emb_func_en_b.mlc_en) ==
          PROPERTY_DISABLE))
  {
    emb_func_en_a.mlc_before_fsm_en = PROPERTY_ENABLE;
  }
  if (ret == 0)
  {
    emb_func_en_a.pedo_en = val.step_counter_enable;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }
  if (ret == 0)
  {
    pedo_cmd_reg.fp_rejection_en = val.false_step_rej;
    ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  return ret;
}

/**
  * @brief  Step counter mode[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      false_step_rej, step_counter, step_detector,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_mode_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_stpcnt_mode_t *val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  st1vafe6ax_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    val->false_step_rej = pedo_cmd_reg.fp_rejection_en;
    val->step_counter_enable = emb_func_en_a.pedo_en;
  }

  return ret;
}

/**
  * @brief  Step counter output, number of detected steps.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Step counter output, number of detected steps.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_steps_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_STEP_COUNTER_L, &buff[0], 2);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret == 0)
  {
    *val = buff[1];
    *val = (*val * 256U) + buff[0];
  }

  return ret;
}

/**
  * @brief  Reset step counter.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Reset step counter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_rst_step_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_src_t emb_func_src;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  if (ret == 0)
  {
    emb_func_src.pedo_rst_step = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Reset step counter.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Reset step counter.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_rst_step_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_src_t emb_func_src;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  if (ret == 0)
  {
    *val = emb_func_src.pedo_rst_step;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Pedometer debounce configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Pedometer debounce configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_debounce_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf,
                              1);
  if (ret == 0)
  {
    pedo_deb_steps_conf.deb_step = val;
    ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf,
                                 1);
  }

  return ret;
}

/**
  * @brief  Pedometer debounce configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Pedometer debounce configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_debounce_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf,
                              1);
  if (ret == 0)
  {
    *val = pedo_deb_steps_conf.deb_step;
  }

  return ret;
}

/**
  * @brief  Time period register for step detection on delta time.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time period register for step detection on delta time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_period_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));

  ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_PEDO_SC_DELTAT_L, (uint8_t *)&buff[0], 2);

  return ret;
}

/**
  * @brief  Time period register for step detection on delta time.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Time period register for step detection on delta time.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_stpcnt_period_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_PEDO_SC_DELTAT_L, &buff[0], 2);

  if (ret == 0)
  {
    *val = buff[1];
    *val = (*val * 256U) + buff[0];
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Significant motion
  * @brief   This section groups all the functions that manage the
  *          significant motion detection.
  * @{
  *
  */

/**
  * @brief  Enables significant motion detection function.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables significant motion detection function.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sigmot_mode_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.sign_motion_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);


  return ret;
}

/**
  * @brief  Enables significant motion detection function.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables significant motion detection function.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sigmot_mode_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    *val = emb_func_en_a.sign_motion_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);


  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Tilt detection
  * @brief     This section groups all the functions that manage the tilt
  *            event detection.
  * @{
  *
  */

/**
  * @brief  Tilt calculation.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Tilt calculation.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tilt_mode_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.tilt_en = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Tilt calculation.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Tilt calculation.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_tilt_mode_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    *val = emb_func_en_a.tilt_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Sensor Fusion Low Power (SFLP)
  * @brief     This section groups all the functions that manage pedometer.
  * @{
  *
  */

/**
  * @brief  Enable SFLP Game Rotation Vector (6x).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable/Disable game rotation value (0/1).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_game_rotation_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.sflp_game_en = val;
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A,
                                (uint8_t *)&emb_func_en_a, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enable SFLP Game Rotation Vector (6x).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable/Disable game rotation value (0/1).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_game_rotation_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  st1vafe6ax_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    *val = emb_func_en_a.sflp_game_en;
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  SFLP Data Rate (ODR) configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SFLP_15Hz, SFLP_30Hz, SFLP_60Hz, SFLP_120Hz, SFLP_240Hz, SFLP_480Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_data_rate_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_sflp_data_rate_t val)
{
  st1vafe6ax_sflp_odr_t sflp_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_SFLP_ODR, (uint8_t *)&sflp_odr, 1);
  }
  if (ret == 0)
  {
    sflp_odr.sflp_game_odr = (uint8_t)val & 0x07U;
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_SFLP_ODR, (uint8_t *)&sflp_odr,
                                1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  SFLP Data Rate (ODR) configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SFLP_15Hz, SFLP_30Hz, SFLP_60Hz, SFLP_120Hz, SFLP_240Hz, SFLP_480Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_data_rate_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_sflp_data_rate_t *val)
{
  st1vafe6ax_sflp_odr_t sflp_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_SFLP_ODR, (uint8_t *)&sflp_odr, 1);
  }
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  switch (sflp_odr.sflp_game_odr)
  {
    case ST1VAFE6AX_SFLP_15Hz:
      *val = ST1VAFE6AX_SFLP_15Hz;
      break;

    case ST1VAFE6AX_SFLP_30Hz:
      *val = ST1VAFE6AX_SFLP_30Hz;
      break;

    case ST1VAFE6AX_SFLP_60Hz:
      *val = ST1VAFE6AX_SFLP_60Hz;
      break;

    case ST1VAFE6AX_SFLP_120Hz:
      *val = ST1VAFE6AX_SFLP_120Hz;
      break;

    case ST1VAFE6AX_SFLP_240Hz:
      *val = ST1VAFE6AX_SFLP_240Hz;
      break;

    case ST1VAFE6AX_SFLP_480Hz:
      *val = ST1VAFE6AX_SFLP_480Hz;
      break;

    default:
      *val = ST1VAFE6AX_SFLP_15Hz;
      break;
  }
  return ret;
}

/*
 * Original conversion routines taken from: https://github.com/numpy/numpy
 *
 * uint16_t npy_floatbits_to_halfbits(uint32_t f);
 * uint16_t npy_float_to_half(float_t f);
 *
 * Released under BSD-3-Clause License
 */

#define NPY_HALF_GENERATE_OVERFLOW  0 /* do not trigger FP overflow */
#define NPY_HALF_GENERATE_UNDERFLOW 0 /* do not trigger FP underflow */
#ifndef NPY_HALF_ROUND_TIES_TO_EVEN
#define NPY_HALF_ROUND_TIES_TO_EVEN 1
#endif

static uint16_t npy_floatbits_to_halfbits(uint32_t f)
{
  uint32_t f_exp, f_sig;
  uint16_t h_sgn, h_exp, h_sig;

  h_sgn = (uint16_t)((f & 0x80000000u) >> 16);
  f_exp = (f & 0x7f800000u);

  /* Exponent overflow/NaN converts to signed inf/NaN */
  if (f_exp >= 0x47800000u)
  {
    if (f_exp == 0x7f800000u)
    {
      /* Inf or NaN */
      f_sig = (f & 0x007fffffu);
      if (f_sig != 0)
      {
        /* NaN - propagate the flag in the significand... */
        uint16_t ret = (uint16_t)(0x7c00u + (f_sig >> 13));
        /* ...but make sure it stays a NaN */
        if (ret == 0x7c00u)
        {
          ret++;
        }
        return h_sgn + ret;
      }
      else
      {
        /* signed inf */
        return (uint16_t)(h_sgn + 0x7c00u);
      }
    }
    else
    {
      /* overflow to signed inf */
#if NPY_HALF_GENERATE_OVERFLOW
      npy_set_floatstatus_overflow();
#endif
      return (uint16_t)(h_sgn + 0x7c00u);
    }
  }

  /* Exponent underflow converts to a subnormal half or signed zero */
  if (f_exp <= 0x38000000u)
  {
    /*
     * Signed zeros, subnormal floats, and floats with small
     * exponents all convert to signed zero half-floats.
     */
    if (f_exp < 0x33000000u)
    {
#if NPY_HALF_GENERATE_UNDERFLOW
      /* If f != 0, it underflowed to 0 */
      if ((f & 0x7fffffff) != 0)
      {
        npy_set_floatstatus_underflow();
      }
#endif
      return h_sgn;
    }
    /* Make the subnormal significand */
    f_exp >>= 23;
    f_sig = (0x00800000u + (f & 0x007fffffu));
#if NPY_HALF_GENERATE_UNDERFLOW
    /* If it's not exactly represented, it underflowed */
    if ((f_sig & (((uint32_t)1 << (126 - f_exp)) - 1)) != 0)
    {
      npy_set_floatstatus_underflow();
    }
#endif
    /*
     * Usually the significand is shifted by 13. For subnormals an
     * additional shift needs to occur. This shift is one for the largest
     * exponent giving a subnormal `f_exp = 0x38000000 >> 23 = 112`, which
     * offsets the new first bit. At most the shift can be 1+10 bits.
     */
    f_sig >>= (113 - f_exp);
    /* Handle rounding by adding 1 to the bit beyond half precision */
#if NPY_HALF_ROUND_TIES_TO_EVEN
    /*
     * If the last bit in the half significand is 0 (already even), and
     * the remaining bit pattern is 1000...0, then we do not add one
     * to the bit after the half significand. However, the (113 - f_exp)
     * shift can lose up to 11 bits, so the || checks them in the original.
     * In all other cases, we can just add one.
     */
    if (((f_sig & 0x00003fffu) != 0x00001000u) || (f & 0x000007ffu))
    {
      f_sig += 0x00001000u;
    }
#else
    f_sig += 0x00001000u;
#endif
    h_sig = (uint16_t)(f_sig >> 13);
    /*
     * If the rounding causes a bit to spill into h_exp, it will
     * increment h_exp from zero to one and h_sig will be zero.
     * This is the correct result.
     */
    return (uint16_t)(h_sgn + h_sig);
  }

  /* Regular case with no overflow or underflow */
  h_exp = (uint16_t)((f_exp - 0x38000000u) >> 13);
  /* Handle rounding by adding 1 to the bit beyond half precision */
  f_sig = (f & 0x007fffffu);
#if NPY_HALF_ROUND_TIES_TO_EVEN
  /*
   * If the last bit in the half significand is 0 (already even), and
   * the remaining bit pattern is 1000...0, then we do not add one
   * to the bit after the half significand.  In all other cases, we do.
   */
  if ((f_sig & 0x00003fffu) != 0x00001000u)
  {
    f_sig += 0x00001000u;
  }
#else
  f_sig += 0x00001000u;
#endif
  h_sig = (uint16_t)(f_sig >> 13);
  /*
   * If the rounding causes a bit to spill into h_exp, it will
   * increment h_exp by one and h_sig will be zero.  This is the
   * correct result.  h_exp may increment to 15, at greatest, in
   * which case the result overflows to a signed inf.
   */
#if NPY_HALF_GENERATE_OVERFLOW
  h_sig += h_exp;
  if (h_sig == 0x7c00u)
  {
    npy_set_floatstatus_overflow();
  }
  return h_sgn + h_sig;
#else
  return h_sgn + h_exp + h_sig;
#endif
}

static uint16_t npy_float_to_half(float_t f)
{
  union
  {
    float_t f;
    uint32_t fbits;
  } conv;
  conv.f = f;
  return npy_floatbits_to_halfbits(conv.fbits);
}

/**
  * @brief  SFLP GBIAS value. The register value is expressed as half-precision
  *         floating-point format: SEEEEEFFFFFFFFFF (S: 1 sign bit; E: 5 exponent
  *          bits; F: 10 fraction bits).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GBIAS x/y/z val.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_game_gbias_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_sflp_gbias_t *val)
{
  st1vafe6ax_sflp_data_rate_t sflp_odr;
  st1vafe6ax_emb_func_exec_status_t emb_func_sts;
  st1vafe6ax_data_ready_t drdy;
  st1vafe6ax_xl_full_scale_t xl_fs;
  st1vafe6ax_ctrl10_t ctrl10;
  uint8_t master_config;
  uint8_t emb_func_en_saved[2];
  uint8_t conf_saved[2];
  uint8_t reg_zero[2] = {0x0, 0x0};
  uint16_t gbias_hf[3];
  float_t k = 0.005f;
  int16_t xl_data[3];
  int32_t data_tmp;
  uint8_t *data_ptr = (uint8_t *)&data_tmp;
  uint8_t i, j;
  int32_t ret;

  ret = st1vafe6ax_sflp_data_rate_get(ctx, &sflp_odr);
  if (ret != 0)
  {
    return ret;
  }

  /* Calculate k factor */
  switch (sflp_odr)
  {
    case ST1VAFE6AX_SFLP_15Hz:
      k = 0.04f;
      break;
    case ST1VAFE6AX_SFLP_30Hz:
      k = 0.02f;
      break;
    case ST1VAFE6AX_SFLP_60Hz:
      k = 0.01f;
      break;
    case ST1VAFE6AX_SFLP_120Hz:
      k = 0.005f;
      break;
    case ST1VAFE6AX_SFLP_240Hz:
      k = 0.0025f;
      break;
    case ST1VAFE6AX_SFLP_480Hz:
      k = 0.00125f;
      break;
  }

  /* compute gbias as half precision float in order to be put in embedded advanced feature register */
  gbias_hf[0] = npy_float_to_half(val->gbias_x * (3.14159265358979323846f / 180.0f) / k);
  gbias_hf[1] = npy_float_to_half(val->gbias_y * (3.14159265358979323846f / 180.0f) / k);
  gbias_hf[2] = npy_float_to_half(val->gbias_z * (3.14159265358979323846f / 180.0f) / k);

  /* Save sensor configuration and set high-performance mode (if the sensor is in power-down mode, turn it on) */
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL1, conf_saved, 2);
  ret += st1vafe6ax_xl_mode_set(ctx, ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD);
  ret += st1vafe6ax_gy_mode_set(ctx, ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD);

  if (ret != 0)
  {
    return ret;
  }

  if ((conf_saved[0] & 0x0FU) == ST1VAFE6AX_XL_ODR_OFF)
  {
    ret += st1vafe6ax_xl_data_rate_set(ctx, ST1VAFE6AX_XL_ODR_AT_120Hz);
  }

  /* disable algos */
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, emb_func_en_saved,
                               2);
  }
  if (ret == 0)
  {
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, reg_zero, 2);
  }
  if (ret == 0)
  {
    do
    {
      ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EXEC_STATUS,
                                 (uint8_t *)&emb_func_sts, 1);
    } while (emb_func_sts.emb_func_endop != 1);
  }
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  // enable gbias setting
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  if (ret != 0)
  {
    return ret;
  }
  ctrl10.emb_func_debug = 1;
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  if (ret != 0)
  {
    return ret;
  }

  /* enable algos */
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    emb_func_en_saved[0] |= 0x02; /* force SFLP GAME en */
    ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, emb_func_en_saved,
                                2);
  }
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  ret += st1vafe6ax_xl_full_scale_get(ctx, &xl_fs);
  if (ret != 0)
  {
    return ret;
  }

  /* Read XL data */
  do
  {
    ret += st1vafe6ax_flag_data_ready_get(ctx, &drdy);
  } while (drdy.drdy_xl != 1);
  ret += st1vafe6ax_acceleration_raw_get(ctx, xl_data);
  if (ret != 0)
  {
    return ret;
  }

  /* force sflp initialization */
  master_config = 0x40;
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, &master_config,
                              1);
  if (ret != 0)
  {
    return ret;
  }

  for (i = 0; i < 3; i++)
  {
    j = 0;
    data_tmp = (int32_t)xl_data[i];
    data_tmp <<= xl_fs; // shift based on current fs
    ret += st1vafe6ax_write_reg(ctx, 0x02 + 3 * i, &data_ptr[j++], 1);
    ret += st1vafe6ax_write_reg(ctx, 0x03 + 3 * i, &data_ptr[j++], 1);
    ret += st1vafe6ax_write_reg(ctx, 0x04 + 3 * i, &data_ptr[j], 1);
  }
  for (i = 0; i < 3; i++)
  {
    j = 0;
    data_tmp = 0;
    ret += st1vafe6ax_write_reg(ctx, 0x0B + 3 * i, &data_ptr[j++], 1);
    ret += st1vafe6ax_write_reg(ctx, 0x0C + 3 * i, &data_ptr[j++], 1);
    ret += st1vafe6ax_write_reg(ctx, 0x0D + 3 * i, &data_ptr[j], 1);
  }
  master_config = 0x00;
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, &master_config,
                              1);
  if (ret != 0)
  {
    return ret;
  }

  // wait end_op (and at least 30 us)
  ctx->mdelay(1);
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    do
    {
      ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EXEC_STATUS,
                                 (uint8_t *)&emb_func_sts, 1);
    } while (emb_func_sts.emb_func_endop != 1);
  }
  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  /* write gbias in embedded advanced features registers */
  ret += st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_SFLP_GAME_GBIASX_L,
                                (uint8_t *)gbias_hf, 6);
  if (ret != 0)
  {
    return ret;
  }

  /* reload previous sensor configuration */
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL1, conf_saved, 2);
  if (ret != 0)
  {
    return ret;
  }

  // disable gbias setting
  ctrl10.emb_func_debug = 0;
  ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);

  return ret;
}

/**
  * @brief  SFLP initial configuration [set]
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_sflp_configure(const stmdev_ctx_t *ctx)
{
  uint8_t val = 0x50;
  int32_t ret;

  ret = st1vafe6ax_ln_pg_write(ctx, 0xD2, &val, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Finite State Machine (FSM)
  * @brief     This section groups all the functions that manage the
  *            state_machine.
  * @{
  *
  */

/**
  * @brief  Enables the control of the CTRL registers to FSM (FSM can change some configurations of the device autonomously).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      PROTECT_CTRL_REGS, WRITE_CTRL_REG,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_permission_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_fsm_permission_t val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret == 0)
  {
    func_cfg_access.fsm_wr_ctrl_en = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  return ret;
}

/**
  * @brief  Enables the control of the CTRL registers to FSM (FSM can change some configurations of the device autonomously).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      PROTECT_CTRL_REGS, WRITE_CTRL_REG,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_permission_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_fsm_permission_t *val)
{
  st1vafe6ax_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (func_cfg_access.fsm_wr_ctrl_en)
  {
    case ST1VAFE6AX_PROTECT_CTRL_REGS:
      *val = ST1VAFE6AX_PROTECT_CTRL_REGS;
      break;

    case ST1VAFE6AX_WRITE_CTRL_REG:
      *val = ST1VAFE6AX_WRITE_CTRL_REG;
      break;

    default:
      *val = ST1VAFE6AX_PROTECT_CTRL_REGS;
      break;
  }
  return ret;
}

/**
  * @brief  Return the status of the CTRL registers permission (standard interface vs FSM).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: all FSM regs are under std_if control, 1: some regs are under FSM control.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_permission_status(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_fsm_permission_status_t *val)
{
  st1vafe6ax_ctrl_status_t status;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL_STATUS, (uint8_t *)&status, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = (status.fsm_wr_ctrl_status == 0) ? ST1VAFE6AX_STD_IF_CONTROL : ST1VAFE6AX_FSM_CONTROL;

  return ret;
}

/**
  * @brief  Enable Finite State Machine (FSM) feature.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable Finite State Machine (FSM) feature.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_fsm_mode_t val)
{
  st1vafe6ax_emb_func_en_b_t emb_func_en_b;
  st1vafe6ax_fsm_enable_t fsm_enable;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
  }
  if ((val.fsm1_en | val.fsm2_en | val.fsm1_en | val.fsm1_en
       | val.fsm1_en | val.fsm2_en | val.fsm1_en | val.fsm1_en) == PROPERTY_ENABLE)
  {
    emb_func_en_b.fsm_en = PROPERTY_ENABLE;
  }
  else
  {
    emb_func_en_b.fsm_en = PROPERTY_DISABLE;
  }
  if (ret == 0)
  {
    fsm_enable.fsm1_en = val.fsm1_en;
    fsm_enable.fsm2_en = val.fsm2_en;
    fsm_enable.fsm3_en = val.fsm3_en;
    fsm_enable.fsm4_en = val.fsm4_en;
    fsm_enable.fsm5_en = val.fsm5_en;
    fsm_enable.fsm6_en = val.fsm6_en;
    fsm_enable.fsm7_en = val.fsm7_en;
    fsm_enable.fsm8_en = val.fsm8_en;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enable Finite State Machine (FSM) feature.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable Finite State Machine (FSM) feature.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_fsm_mode_t *val)
{
  st1vafe6ax_fsm_enable_t fsm_enable;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  val->fsm1_en = fsm_enable.fsm1_en;
  val->fsm2_en = fsm_enable.fsm2_en;
  val->fsm3_en = fsm_enable.fsm3_en;
  val->fsm4_en = fsm_enable.fsm4_en;
  val->fsm5_en = fsm_enable.fsm5_en;
  val->fsm6_en = fsm_enable.fsm6_en;
  val->fsm7_en = fsm_enable.fsm7_en;
  val->fsm8_en = fsm_enable.fsm8_en;

  return ret;
}

/**
  * @brief  FSM long counter status register. Long counter value is an unsigned integer value (16-bit format).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM long counter status register. Long counter value is an unsigned integer value (16-bit format).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_long_cnt_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FSM_LONG_COUNTER_L, (uint8_t *)&buff[0], 2);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM long counter status register. Long counter value is an unsigned integer value (16-bit format).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM long counter status register. Long counter value is an unsigned integer value (16-bit format).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_long_cnt_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_LONG_COUNTER_L, &buff[0], 2);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @brief  FSM output registers[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM output registers
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_out_get(const stmdev_ctx_t *ctx, st1vafe6ax_fsm_out_t *val)
{
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_OUTS1, (uint8_t *)val, 8);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Finite State Machine Output Data Rate (ODR) configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM_15Hz, FSM_30Hz, FSM_60Hz, FSM_120Hz, FSM_240Hz, FSM_480Hz, FSM_960Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fsm_data_rate_t val)
{
  st1vafe6ax_fsm_odr_t fsm_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  if (ret == 0)
  {
    fsm_odr.fsm_odr = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Finite State Machine Output Data Rate (ODR) configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM_15Hz, FSM_30Hz, FSM_60Hz, FSM_120Hz, FSM_240Hz, FSM_480Hz, FSM_960Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fsm_data_rate_t *val)
{
  st1vafe6ax_fsm_odr_t fsm_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  switch (fsm_odr.fsm_odr)
  {
    case ST1VAFE6AX_FSM_15Hz:
      *val = ST1VAFE6AX_FSM_15Hz;
      break;

    case ST1VAFE6AX_FSM_30Hz:
      *val = ST1VAFE6AX_FSM_30Hz;
      break;

    case ST1VAFE6AX_FSM_60Hz:
      *val = ST1VAFE6AX_FSM_60Hz;
      break;

    case ST1VAFE6AX_FSM_120Hz:
      *val = ST1VAFE6AX_FSM_120Hz;
      break;

    case ST1VAFE6AX_FSM_240Hz:
      *val = ST1VAFE6AX_FSM_240Hz;
      break;

    case ST1VAFE6AX_FSM_480Hz:
      *val = ST1VAFE6AX_FSM_480Hz;
      break;

    case ST1VAFE6AX_FSM_960Hz:
      *val = ST1VAFE6AX_FSM_960Hz;
      break;

    default:
      *val = ST1VAFE6AX_FSM_15Hz;
      break;
  }
  return ret;
}

/**
  * @brief  FSM long counter timeout. The long counter timeout value is an unsigned integer value (16-bit format). When the long counter value reached this value, the FSM generates an interrupt.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM long counter timeout. The long counter timeout value is an unsigned integer value (16-bit format). When the long counter value reached this value, the FSM generates an interrupt.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_long_cnt_timeout_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_FSM_LC_TIMEOUT_L, (uint8_t *)&buff[0], 2);

  return ret;
}

/**
  * @brief  FSM long counter timeout. The long counter timeout value is an unsigned integer value (16-bit format). When the long counter value reached this value, the FSM generates an interrupt.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM long counter timeout. The long counter timeout value is an unsigned integer value (16-bit format). When the long counter value reached this value, the FSM generates an interrupt.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_long_cnt_timeout_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_FSM_LC_TIMEOUT_L, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @brief  FSM number of programs.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM number of programs.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_number_of_programs_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_fsm_programs_t fsm_programs;
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
  if (ret == 0)
  {
    fsm_programs.fsm_n_prog = val;
    ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
  }

  return ret;
}

/**
  * @brief  FSM number of programs.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM number of programs.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_number_of_programs_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_fsm_programs_t fsm_programs;
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = fsm_programs.fsm_n_prog;


  return ret;
}

/**
  * @brief  FSM start address. First available address is 0x35C.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM start address. First available address is 0x35C.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_start_address_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_FSM_START_ADD_L, (uint8_t *)&buff[0], 2);

  return ret;
}

/**
  * @brief  FSM start address. First available address is 0x35C.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FSM start address. First available address is 0x35C.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_start_address_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_FSM_START_ADD_L, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @addtogroup  Machine Learning Core
  * @brief   This section group all the functions concerning the
  *          usage of Machine Learning Core
  * @{
  *
  */

/**
  * @brief  It enables Machine Learning Core feature (MLC). When the Machine Learning Core is enabled the Finite State Machine (FSM) programs are executed before executing the MLC algorithms.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MLC_OFF, MLC_ON, MLC_BEFORE_FSM,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_set(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_mode_t val)
{
  st1vafe6ax_emb_func_en_b_t emb_en_b;
  st1vafe6ax_emb_func_en_a_t emb_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);

    if (ret == 0)
    {
      switch (val)
      {
        case ST1VAFE6AX_MLC_OFF:
          emb_en_a.mlc_before_fsm_en = 0;
          emb_en_b.mlc_en = 0;
          break;
        case ST1VAFE6AX_MLC_ON:
          emb_en_a.mlc_before_fsm_en = 0;
          emb_en_b.mlc_en = 1;
          break;
        case ST1VAFE6AX_MLC_ON_BEFORE_FSM:
          emb_en_a.mlc_before_fsm_en = 1;
          emb_en_b.mlc_en = 0;
          break;
        default:
          break;
      }

      ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
      ret += st1vafe6ax_write_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);
    }
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  It enables Machine Learning Core feature (MLC). When the Machine Learning Core is enabled the Finite State Machine (FSM) programs are executed before executing the MLC algorithms.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MLC_OFF, MLC_ON, MLC_BEFORE_FSM,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_get(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_mode_t *val)
{
  st1vafe6ax_emb_func_en_b_t emb_en_b;
  st1vafe6ax_emb_func_en_a_t emb_en_a;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
    ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);

    if (ret == 0)
    {
      if (emb_en_a.mlc_before_fsm_en == 0U && emb_en_b.mlc_en == 0U)
      {
        *val = ST1VAFE6AX_MLC_OFF;
      }
      else if (emb_en_a.mlc_before_fsm_en == 0U && emb_en_b.mlc_en == 1U)
      {
        *val = ST1VAFE6AX_MLC_ON;
      }
      else if (emb_en_a.mlc_before_fsm_en == 1U)
      {
        *val = ST1VAFE6AX_MLC_ON_BEFORE_FSM;
      }
      else
      {
        /* Do nothing */
      }
    }
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Machine Learning Core Output Data Rate (ODR) configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MLC_15Hz, MLC_30Hz, MLC_60Hz, MLC_120Hz, MLC_240Hz, MLC_480Hz, MLC_960Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_mlc_data_rate_t val)
{
  st1vafe6ax_mlc_odr_t mlc_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_ODR, (uint8_t *)&mlc_odr, 1);
  }

  if (ret == 0)
  {
    mlc_odr.mlc_odr = (uint8_t)val & 0x07U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_MLC_ODR, (uint8_t *)&mlc_odr, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Machine Learning Core Output Data Rate (ODR) configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MLC_15Hz, MLC_30Hz, MLC_60Hz, MLC_120Hz, MLC_240Hz, MLC_480Hz, MLC_960Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_mlc_data_rate_t *val)
{
  st1vafe6ax_mlc_odr_t mlc_odr;
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC_ODR, (uint8_t *)&mlc_odr, 1);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);

  if (ret != 0)
  {
    return ret;
  }

  switch (mlc_odr.mlc_odr)
  {
    case ST1VAFE6AX_MLC_15Hz:
      *val = ST1VAFE6AX_MLC_15Hz;
      break;

    case ST1VAFE6AX_MLC_30Hz:
      *val = ST1VAFE6AX_MLC_30Hz;
      break;

    case ST1VAFE6AX_MLC_60Hz:
      *val = ST1VAFE6AX_MLC_60Hz;
      break;

    case ST1VAFE6AX_MLC_120Hz:
      *val = ST1VAFE6AX_MLC_120Hz;
      break;

    case ST1VAFE6AX_MLC_240Hz:
      *val = ST1VAFE6AX_MLC_240Hz;
      break;

    case ST1VAFE6AX_MLC_480Hz:
      *val = ST1VAFE6AX_MLC_480Hz;
      break;

    case ST1VAFE6AX_MLC_960Hz:
      *val = ST1VAFE6AX_MLC_960Hz;
      break;

    default:
      *val = ST1VAFE6AX_MLC_15Hz;
      break;
  }
  return ret;
}

/**
  * @brief  Output value of all MLC decision trees.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Output value of all MLC decision trees.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_out_get(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_out_t *val)
{
  int32_t ret;

  ret = st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_MLC1_SRC, (uint8_t *)val, 4);
  }

  ret += st1vafe6ax_mem_bank_set(ctx, ST1VAFE6AX_MAIN_MEM_BANK);
  return ret;
}

/**
  * @brief  Bio sensor sensitivity value register for the Machine Learning Core.
  *         This register corresponds to the conversion value of the Bio sensor.
  *         The register value is expressed as half-precision floating-point format:
  *         SEEEEEFFFFFFFFFF (S: 1 sign bit; E: 5 exponent bits; F: 10 fraction bits).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio sensor sensitivity value register for the Machine Learning Core.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_bio_sensitivity_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_MLC_BIO_SENSITIVITY_L, (uint8_t *)&buff[0], 2);

  return ret;
}

/**
  * @brief  Bio sensor sensitivity value register for the Machine Learning Core.
  *         This register corresponds to the conversion value of the Bio sensor.
  *         The register value is expressed as half-precision floating-point format:
  *         SEEEEEFFFFFFFFFF (S: 1 sign bit; E: 5 exponent bits; F: 10 fraction bits).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio sensor sensitivity value register for the Machine Learning Core.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_mlc_bio_sensitivity_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_MLC_BIO_SENSITIVITY_L, &buff[0], 2);

  if (ret != 0)
  {
    return ret;
  }

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @addtogroup  Accelerometer user offset correction
  * @brief   This section group all the functions concerning the
  *          usage of Accelerometer user offset correction
  * @{
  *
  */

/**
  * @brief  Enables accelerometer user offset correction block; it is valid for the low-pass path.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables accelerometer user offset correction block; it is valid for the low-pass path.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_offset_on_out_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.usr_off_on_out = val;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }

  return ret;
}

/**
  * @brief  Enables accelerometer user offset correction block; it is valid for the low-pass path.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables accelerometer user offset correction block; it is valid for the low-pass path.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_offset_on_out_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = ctrl9.usr_off_on_out;

  return ret;
}

/**
  * @brief  Accelerometer user offset correction values in mg.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Accelerometer user offset correction values in mg.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_offset_mg_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_offset_mg_t val)
{
  st1vafe6ax_z_ofs_usr_t z_ofs_usr;
  st1vafe6ax_y_ofs_usr_t y_ofs_usr;
  st1vafe6ax_x_ofs_usr_t x_ofs_usr;
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;
  float_t tmp;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);

  if (ret != 0)
  {
    return ret;
  }

  if ((val.x_mg < (0.0078125f * 127.0f)) && (val.x_mg > (0.0078125f * -127.0f)) &&
      (val.y_mg < (0.0078125f * 127.0f)) && (val.y_mg > (0.0078125f * -127.0f)) &&
      (val.z_mg < (0.0078125f * 127.0f)) && (val.z_mg > (0.0078125f * -127.0f)))
  {
    ctrl9.usr_off_w = 0;

    tmp = val.z_mg / 0.0078125f;
    z_ofs_usr.z_ofs_usr = (uint8_t)tmp;

    tmp = val.y_mg / 0.0078125f;
    y_ofs_usr.y_ofs_usr = (uint8_t)tmp;

    tmp = val.x_mg / 0.0078125f;
    x_ofs_usr.x_ofs_usr = (uint8_t)tmp;
  }
  else if ((val.x_mg < (0.125f * 127.0f)) && (val.x_mg > (0.125f * -127.0f)) &&
           (val.y_mg < (0.125f * 127.0f)) && (val.y_mg > (0.125f * -127.0f)) &&
           (val.z_mg < (0.125f * 127.0f)) && (val.z_mg > (0.125f * -127.0f)))
  {
    ctrl9.usr_off_w = 1;

    tmp = val.z_mg / 0.125f;
    z_ofs_usr.z_ofs_usr = (uint8_t)tmp;

    tmp = val.y_mg / 0.125f;
    y_ofs_usr.y_ofs_usr = (uint8_t)tmp;

    tmp = val.x_mg / 0.125f;
    x_ofs_usr.x_ofs_usr = (uint8_t)tmp;
  }
  else // out of limit
  {
    ctrl9.usr_off_w = 1;
    z_ofs_usr.z_ofs_usr = 0xFFU;
    y_ofs_usr.y_ofs_usr = 0xFFU;
    x_ofs_usr.x_ofs_usr = 0xFFU;
  }

  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  }
  return ret;
}

/**
  * @brief  Accelerometer user offset correction values in mg.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Accelerometer user offset correction values in mg.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_xl_offset_mg_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_offset_mg_t *val)
{
  st1vafe6ax_z_ofs_usr_t z_ofs_usr;
  st1vafe6ax_y_ofs_usr_t y_ofs_usr;
  st1vafe6ax_x_ofs_usr_t x_ofs_usr;
  st1vafe6ax_ctrl9_t ctrl9;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL9, (uint8_t *)&ctrl9, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);

  if (ret != 0)
  {
    return ret;
  }

  if (ctrl9.usr_off_w == PROPERTY_DISABLE)
  {
    val->z_mg = ((float_t)z_ofs_usr.z_ofs_usr * 0.0078125f);
    val->y_mg = ((float_t)y_ofs_usr.y_ofs_usr * 0.0078125f);
    val->x_mg = ((float_t)x_ofs_usr.x_ofs_usr * 0.0078125f);
  }
  else
  {
    val->z_mg = ((float_t)z_ofs_usr.z_ofs_usr * 0.125f);
    val->y_mg = ((float_t)y_ofs_usr.y_ofs_usr * 0.125f);
    val->x_mg = ((float_t)x_ofs_usr.x_ofs_usr * 0.125f);
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @addtogroup  AH_BIO
  * @brief   This section group all the functions concerning the
  *          usage of AH_BIO
  * @{
  *
  */

/**
  * @brief  Enables AH_BIO chain. When this bit is set to ‘1’, the AH_BIO buffers are
  *         connected to the AH1/Bio1 and AH1/Bio2 pins. Before setting this bit to 1,
  *         the accelerometer and gyroscope sensor have to be configured in power-down mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: Enables AH_BIO chain, 0: Disable the AH_BIO chain
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ah_bio_mode_set(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_ah_bio_mode_t val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret == 0)
  {
    if ((val.ah_bio1_en | val.ah_bio2_en) == PROPERTY_ENABLE)
    {
      ctrl7.ah_bio_en = PROPERTY_ENABLE;
    }
    else
    {
      ctrl7.ah_bio_en = PROPERTY_DISABLE;
    }
    ctrl7.ah_bio1_en = val.ah_bio1_en;
    ctrl7.ah_bio2_en = val.ah_bio2_en;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  }
  if (ret == 0)
  {
    ctrl10.ah_bio_sw = val.swaps;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);
  }
  return ret;
}

/**
  * @brief  Enables AH_BIO chain. When this bit is set to ‘1’, the AH_BIO buffers are
  *         connected to the AH1/Bio1 and AH1/Bio2 pins. Before setting this bit to 1,
  *         the accelerometer and gyroscope sensor have to be configured in power-down mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: Enables AH_BIO chain, 0: Disable the AH_BIO chain
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ah_bio_mode_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_ah_bio_mode_t *val)
{
  st1vafe6ax_ctrl10_t ctrl10;
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  ret += st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret != 0)
  {
    return ret;
  }

  val->ah_bio1_en = ctrl7.ah_bio1_en;
  val->ah_bio2_en = ctrl7.ah_bio2_en;
  val->swaps = ctrl10.ah_bio_sw;

  return ret;
}

/**
  * @brief  Configures the equivalent input impedance of the AH_BIO buffers.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      2400MOhm, 730MOhm, 300MOhm, 255MOhm,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ah_bio_zin_set(const stmdev_ctx_t *ctx,
                                  st1vafe6ax_ah_bio_zin_t val)
{
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  if (ret == 0)
  {
    ctrl7.ah_bio_c_zin = (uint8_t)val & 0x03U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  }

  return ret;
}

/**
  * @brief  Configures the equivalent input impedance of the AH_BIO buffers.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      2400MOhm, 730MOhm, 300MOhm, 255MOhm,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_ah_bio_zin_get(const stmdev_ctx_t *ctx,
                                  st1vafe6ax_ah_bio_zin_t *val)
{
  st1vafe6ax_ctrl7_t ctrl7;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_CTRL7, (uint8_t *)&ctrl7, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (ctrl7.ah_bio_c_zin)
  {
    case ST1VAFE6AX_2400MOhm:
      *val = ST1VAFE6AX_2400MOhm;
      break;

    case ST1VAFE6AX_730MOhm:
      *val = ST1VAFE6AX_730MOhm;
      break;

    case ST1VAFE6AX_300MOhm:
      *val = ST1VAFE6AX_300MOhm;
      break;

    case ST1VAFE6AX_255MOhm:
      *val = ST1VAFE6AX_255MOhm;
      break;

    default:
      *val = ST1VAFE6AX_2400MOhm;
      break;
  }
  return ret;
}

/**
  * @brief  Bio sensor sensitivity value register for the Finite State Machine.
  *         This register corresponds to the conversion value of the Bio sensor.
  *         The register value is expressed as half-precision floating-point format:
  *         SEEEEEFFFFFFFFFF (S: 1 sign bit; E: 5 exponent bits; F: 10 fraction bits).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio sensor sensitivity value register for the Finite State Machine.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_bio_sensitivity_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = st1vafe6ax_ln_pg_write(ctx, ST1VAFE6AX_FSM_BIO_SENSITIVITY_L, (uint8_t *)&buff[0], 2);

  return ret;
}

/**
  * @brief  Bio sensor sensitivity value register for the Finite State Machine.
  *         This register corresponds to the conversion value of the Bio sensor.
  *         The register value is expressed as half-precision floating-point format:
  *         SEEEEEFFFFFFFFFF (S: 1 sign bit; E: 5 exponent bits; F: 10 fraction bits).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Bio sensor sensitivity value register for the Finite State Machine.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_fsm_bio_sensitivity_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = st1vafe6ax_ln_pg_read(ctx, ST1VAFE6AX_FSM_BIO_SENSITIVITY_L, &buff[0], 2);
  if (ret != 0)
  {
    return ret;
  }

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @addtogroup  SenseWire (I3C)
  * @brief   This section group all the functions concerning the
  *          usage of SenseWire (I3C)
  * @{
  *
  */

/**
  * @brief  Selects the action the device will perform after "Reset whole chip" I3C pattern.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SW_RST_DYN_ADDRESS_RST, GLOBAL_RST_,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_i3c_reset_mode_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_i3c_reset_mode_t val)
{
  st1vafe6ax_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret == 0)
  {
    pin_ctrl.ibhr_por_en = (uint8_t)val & 0x01U;
    ret = st1vafe6ax_write_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  Selects the action the device will perform after "Reset whole chip" I3C pattern.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SW_RST_DYN_ADDRESS_RST, GLOBAL_RST_,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t st1vafe6ax_i3c_reset_mode_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_i3c_reset_mode_t *val)
{
  st1vafe6ax_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = st1vafe6ax_read_reg(ctx, ST1VAFE6AX_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret != 0)
  {
    return ret;
  }

  switch (pin_ctrl.ibhr_por_en)
  {
    case ST1VAFE6AX_SW_RST_DYN_ADDRESS_RST:
      *val = ST1VAFE6AX_SW_RST_DYN_ADDRESS_RST;
      break;

    case ST1VAFE6AX_I3C_GLOBAL_RST:
      *val = ST1VAFE6AX_I3C_GLOBAL_RST;
      break;

    default:
      *val = ST1VAFE6AX_SW_RST_DYN_ADDRESS_RST;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */

