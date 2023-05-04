/**
  ******************************************************************************
  * @file    lsm6dsv16b_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LSM6DSV16B driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "lsm6dsv16b_reg.h"

/**
  * @defgroup  LSM6DSV16B
  * @brief     This file provides a set of functions needed to drive the
  *            lsm6dsv16b enhanced inertial module.
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
int32_t lsm6dsv16b_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                             uint8_t *data,
                             uint16_t len)
{
  int32_t ret;

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
int32_t lsm6dsv16b_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                              uint8_t *data,
                              uint16_t len)
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
float_t lsm6dsv16b_from_sflp_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.061f;
}

float_t lsm6dsv16b_from_fs2_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.061f;
}

float_t lsm6dsv16b_from_fs4_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.122f;
}

float_t lsm6dsv16b_from_fs8_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.244f;
}

float_t lsm6dsv16b_from_fs16_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 0.488f;
}

float_t lsm6dsv16b_from_fs125_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 4.375f;
}

float_t lsm6dsv16b_from_fs250_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 8.750f;
}

float_t lsm6dsv16b_from_fs500_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 17.50f;
}

float_t lsm6dsv16b_from_fs1000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 35.0f;
}

float_t lsm6dsv16b_from_fs2000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 70.0f;
}

float_t lsm6dsv16b_from_fs4000_to_mdps(int16_t lsb)
{
  return ((float_t)lsb) * 140.0f;
}

float_t lsm6dsv16b_from_lsb_to_celsius(int16_t lsb)
{
  return (((float_t)lsb / 256.0f) + 25.0f);
}

uint64_t lsm6dsv16b_from_lsb_to_nsec(uint32_t lsb)
{
  return ((uint64_t)lsb * 21750);
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
int32_t lsm6dsv16b_reset_set(stmdev_ctx_t *ctx, lsm6dsv16b_reset_t val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  ctrl3.boot = ((uint8_t)val & 0x04U) >> 2;
  ctrl3.sw_reset = ((uint8_t)val & 0x02U) >> 1;
  func_cfg_access.sw_por = (uint8_t)val & 0x01U;

  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

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
int32_t lsm6dsv16b_reset_get(stmdev_ctx_t *ctx, lsm6dsv16b_reset_t *val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  switch ((ctrl3.sw_reset << 2) + (ctrl3.boot << 1) + func_cfg_access.sw_por)
  {
    case LSM6DSV16B_READY:
      *val = LSM6DSV16B_READY;
      break;

    case LSM6DSV16B_GLOBAL_RST:
      *val = LSM6DSV16B_GLOBAL_RST;
      break;

    case LSM6DSV16B_RESTORE_CAL_PARAM:
      *val = LSM6DSV16B_RESTORE_CAL_PARAM;
      break;

    case LSM6DSV16B_RESTORE_CTRL_REGS:
      *val = LSM6DSV16B_RESTORE_CTRL_REGS;
      break;

    default:
      *val = LSM6DSV16B_GLOBAL_RST;
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
int32_t lsm6dsv16b_mem_bank_set(stmdev_ctx_t *ctx, lsm6dsv16b_mem_bank_t val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret == 0)
  {
    func_cfg_access.emb_func_reg_access = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
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
int32_t lsm6dsv16b_mem_bank_get(stmdev_ctx_t *ctx, lsm6dsv16b_mem_bank_t *val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  switch (func_cfg_access.emb_func_reg_access)
  {
    case LSM6DSV16B_MAIN_MEM_BANK:
      *val = LSM6DSV16B_MAIN_MEM_BANK;
      break;

    case LSM6DSV16B_EMBED_FUNC_MEM_BANK:
      *val = LSM6DSV16B_EMBED_FUNC_MEM_BANK;
      break;

    default:
      *val = LSM6DSV16B_MAIN_MEM_BANK;
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
int32_t lsm6dsv16b_device_id_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_who_am_i_t who_am_i;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WHO_AM_I, (uint8_t *)&who_am_i, 1);
  *val = who_am_i.id;

  return ret;
}

/**
  * @brief  Accelerometer output data rate (ODR) selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ODR_OFF, XL_ODR_AT_1Hz875, XL_ODR_AT_7Hz5, XL_ODR_AT_15Hz, XL_ODR_AT_30Hz, XL_ODR_AT_60Hz, XL_ODR_AT_120Hz, XL_ODR_AT_240Hz, XL_ODR_AT_480Hz, XL_ODR_AT_960Hz, XL_ODR_AT_1920Hz, XL_ODR_AT_3840Hz, XL_ODR_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_data_rate_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_data_rate_t val)
{
  lsm6dsv16b_ctrl1_t ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);
  if (ret == 0)
  {
    ctrl1.odr_xl = (uint8_t)val & 0xFU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer output data rate (ODR) selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_ODR_OFF, XL_ODR_AT_1Hz875, XL_ODR_AT_7Hz5, XL_ODR_AT_15Hz, XL_ODR_AT_30Hz, XL_ODR_AT_60Hz, XL_ODR_AT_120Hz, XL_ODR_AT_240Hz, XL_ODR_AT_480Hz, XL_ODR_AT_960Hz, XL_ODR_AT_1920Hz, XL_ODR_AT_3840Hz, XL_ODR_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_data_rate_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_data_rate_t *val)
{
  lsm6dsv16b_ctrl1_t ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);

  switch (ctrl1.odr_xl)
  {
    case LSM6DSV16B_XL_ODR_OFF:
      *val = LSM6DSV16B_XL_ODR_OFF;
      break;

    case LSM6DSV16B_XL_ODR_AT_1Hz875:
      *val = LSM6DSV16B_XL_ODR_AT_1Hz875;
      break;

    case LSM6DSV16B_XL_ODR_AT_7Hz5:
      *val = LSM6DSV16B_XL_ODR_AT_7Hz5;
      break;

    case LSM6DSV16B_XL_ODR_AT_15Hz:
      *val = LSM6DSV16B_XL_ODR_AT_15Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_30Hz:
      *val = LSM6DSV16B_XL_ODR_AT_30Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_60Hz:
      *val = LSM6DSV16B_XL_ODR_AT_60Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_120Hz:
      *val = LSM6DSV16B_XL_ODR_AT_120Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_240Hz:
      *val = LSM6DSV16B_XL_ODR_AT_240Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_480Hz:
      *val = LSM6DSV16B_XL_ODR_AT_480Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_960Hz:
      *val = LSM6DSV16B_XL_ODR_AT_960Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_1920Hz:
      *val = LSM6DSV16B_XL_ODR_AT_1920Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_3840Hz:
      *val = LSM6DSV16B_XL_ODR_AT_3840Hz;
      break;

    case LSM6DSV16B_XL_ODR_AT_7680Hz:
      *val = LSM6DSV16B_XL_ODR_AT_7680Hz;
      break;

    default:
      *val = LSM6DSV16B_XL_ODR_OFF;
      break;
  }
  return ret;
}

/**
  * @brief  Accelerometer operating mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_HIGH_PERFORMANCE_MD, XL_HIGH_ACCURANCY_ODR_MD, XL_LOW_POWER_2_AVG_MD, XL_LOW_POWER_4_AVG_MD, XL_LOW_POWER_8_AVG_MD, XL_NORMAL_MD,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_xl_mode_t val)
{
  lsm6dsv16b_ctrl1_t ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);

  if (ret == 0)
  {
    ctrl1.op_mode_xl = (uint8_t)val & 0x07U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  Accelerometer operating mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_HIGH_PERFORMANCE_MD, XL_HIGH_ACCURANCY_ODR_MD, XL_LOW_POWER_2_AVG_MD, XL_LOW_POWER_4_AVG_MD, XL_LOW_POWER_8_AVG_MD, XL_NORMAL_MD,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_xl_mode_t *val)
{
  lsm6dsv16b_ctrl1_t ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL1, (uint8_t *)&ctrl1, 1);

  switch (ctrl1.op_mode_xl)
  {
    case LSM6DSV16B_XL_HIGH_PERFORMANCE_MD:
      *val = LSM6DSV16B_XL_HIGH_PERFORMANCE_MD;
      break;

    case LSM6DSV16B_XL_HIGH_ACCURANCY_ODR_MD:
      *val = LSM6DSV16B_XL_HIGH_ACCURANCY_ODR_MD;
      break;

    case LSM6DSV16B_XL_LOW_POWER_2_AVG_MD:
      *val = LSM6DSV16B_XL_LOW_POWER_2_AVG_MD;
      break;

    case LSM6DSV16B_XL_LOW_POWER_4_AVG_MD:
      *val = LSM6DSV16B_XL_LOW_POWER_4_AVG_MD;
      break;

    case LSM6DSV16B_XL_LOW_POWER_8_AVG_MD:
      *val = LSM6DSV16B_XL_LOW_POWER_8_AVG_MD;
      break;

    case LSM6DSV16B_XL_NORMAL_MD:
      *val = LSM6DSV16B_XL_NORMAL_MD;
      break;

    default:
      *val = LSM6DSV16B_XL_HIGH_PERFORMANCE_MD;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope output data rate (ODR) selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_ODR_OFF, GY_ODR_AT_7Hz5, GY_ODR_AT_15Hz, GY_ODR_AT_30Hz, GY_ODR_AT_60Hz, GY_ODR_AT_120Hz, GY_ODR_AT_240Hz, GY_ODR_AT_480Hz, GY_ODR_AT_960Hz, GY_ODR_AT_1920Hz, GY_ODR_AT_3840Hz, GY_ODR_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_gy_data_rate_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_data_rate_t val)
{
  lsm6dsv16b_ctrl2_t ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);

  if (ret == 0)
  {
    ctrl2.odr_g = (uint8_t)val & 0xFU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope output data rate (ODR) selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_ODR_OFF, GY_ODR_AT_7Hz5, GY_ODR_AT_15Hz, GY_ODR_AT_30Hz, GY_ODR_AT_60Hz, GY_ODR_AT_120Hz, GY_ODR_AT_240Hz, GY_ODR_AT_480Hz, GY_ODR_AT_960Hz, GY_ODR_AT_1920Hz, GY_ODR_AT_3840Hz, GY_ODR_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_gy_data_rate_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_data_rate_t *val)
{
  lsm6dsv16b_ctrl2_t ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);

  switch (ctrl2.odr_g)
  {
    case LSM6DSV16B_GY_ODR_OFF:
      *val = LSM6DSV16B_GY_ODR_OFF;
      break;

    case LSM6DSV16B_GY_ODR_AT_7Hz5:
      *val = LSM6DSV16B_GY_ODR_AT_7Hz5;
      break;

    case LSM6DSV16B_GY_ODR_AT_15Hz:
      *val = LSM6DSV16B_GY_ODR_AT_15Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_30Hz:
      *val = LSM6DSV16B_GY_ODR_AT_30Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_60Hz:
      *val = LSM6DSV16B_GY_ODR_AT_60Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_120Hz:
      *val = LSM6DSV16B_GY_ODR_AT_120Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_240Hz:
      *val = LSM6DSV16B_GY_ODR_AT_240Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_480Hz:
      *val = LSM6DSV16B_GY_ODR_AT_480Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_960Hz:
      *val = LSM6DSV16B_GY_ODR_AT_960Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_1920Hz:
      *val = LSM6DSV16B_GY_ODR_AT_1920Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_3840Hz:
      *val = LSM6DSV16B_GY_ODR_AT_3840Hz;
      break;

    case LSM6DSV16B_GY_ODR_AT_7680Hz:
      *val = LSM6DSV16B_GY_ODR_AT_7680Hz;
      break;

    default:
      *val = LSM6DSV16B_GY_ODR_OFF;
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
int32_t lsm6dsv16b_gy_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_gy_mode_t val)
{
  lsm6dsv16b_ctrl2_t ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);
  if (ret == 0)
  {
    ctrl2.op_mode_g = (uint8_t)val & 0x07U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);
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
int32_t lsm6dsv16b_gy_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_gy_mode_t *val)
{
  lsm6dsv16b_ctrl2_t ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL2, (uint8_t *)&ctrl2, 1);
  switch (ctrl2.op_mode_g)
  {
    case LSM6DSV16B_GY_HIGH_PERFORMANCE_MD:
      *val = LSM6DSV16B_GY_HIGH_PERFORMANCE_MD;
      break;

    case LSM6DSV16B_GY_HIGH_ACCURANCY_ODR_MD:
      *val = LSM6DSV16B_GY_HIGH_ACCURANCY_ODR_MD;
      break;

    case LSM6DSV16B_GY_SLEEP_MD:
      *val = LSM6DSV16B_GY_SLEEP_MD;
      break;

    case LSM6DSV16B_GY_LOW_POWER_MD:
      *val = LSM6DSV16B_GY_LOW_POWER_MD;
      break;

    default:
      *val = LSM6DSV16B_GY_HIGH_PERFORMANCE_MD;
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
int32_t lsm6dsv16b_auto_increment_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    ctrl3.if_inc = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
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
int32_t lsm6dsv16b_auto_increment_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
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
int32_t lsm6dsv16b_block_data_update_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);

  if (ret == 0)
  {
    ctrl3.bdu = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
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
int32_t lsm6dsv16b_block_data_update_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl3_t ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL3, (uint8_t *)&ctrl3, 1);
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
int32_t lsm6dsv16b_data_ready_mode_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_data_ready_mode_t val)
{
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    ctrl4.drdy_pulsed = (uint8_t)val & 0x1U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
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
int32_t lsm6dsv16b_data_ready_mode_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_data_ready_mode_t *val)
{
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);

  switch (ctrl4.drdy_pulsed)
  {
    case LSM6DSV16B_DRDY_LATCHED:
      *val = LSM6DSV16B_DRDY_LATCHED;
      break;

    case LSM6DSV16B_DRDY_PULSED:
      *val = LSM6DSV16B_DRDY_PULSED;
      break;

    default:
      *val = LSM6DSV16B_DRDY_LATCHED;
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
int32_t lsm6dsv16b_gy_full_scale_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_gy_full_scale_t val)
{
  lsm6dsv16b_ctrl6_t ctrl6;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);

  if (ret == 0)
  {
    ctrl6.fs_g = (uint8_t)val & 0xFU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);
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
int32_t lsm6dsv16b_gy_full_scale_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_gy_full_scale_t *val)
{
  lsm6dsv16b_ctrl6_t ctrl6;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);

  switch (ctrl6.fs_g)
  {
    case LSM6DSV16B_125dps:
      *val = LSM6DSV16B_125dps;
      break;

    case LSM6DSV16B_250dps:
      *val = LSM6DSV16B_250dps;
      break;

    case LSM6DSV16B_500dps:
      *val = LSM6DSV16B_500dps;
      break;

    case LSM6DSV16B_1000dps:
      *val = LSM6DSV16B_1000dps;
      break;

    case LSM6DSV16B_2000dps:
      *val = LSM6DSV16B_2000dps;
      break;

    case LSM6DSV16B_4000dps:
      *val = LSM6DSV16B_4000dps;
      break;

    default:
      *val = LSM6DSV16B_125dps;
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
int32_t lsm6dsv16b_xl_full_scale_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_xl_full_scale_t val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);

  if (ret == 0)
  {
    ctrl8.fs_xl = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
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
int32_t lsm6dsv16b_xl_full_scale_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_xl_full_scale_t *val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);

  switch (ctrl8.fs_xl)
  {
    case LSM6DSV16B_2g:
      *val = LSM6DSV16B_2g;
      break;

    case LSM6DSV16B_4g:
      *val = LSM6DSV16B_4g;
      break;

    case LSM6DSV16B_8g:
      *val = LSM6DSV16B_8g;
      break;

    case LSM6DSV16B_16g:
      *val = LSM6DSV16B_16g;
      break;

    default:
      *val = LSM6DSV16B_2g;
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
int32_t lsm6dsv16b_xl_dual_channel_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);

  if (ret == 0)
  {
    ctrl8.xl_dualc_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
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
int32_t lsm6dsv16b_xl_dual_channel_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
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
int32_t lsm6dsv16b_xl_self_test_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_self_test_t val)
{
  lsm6dsv16b_ctrl10_t ctrl10;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret == 0)
  {
    ctrl10.st_xl = (uint8_t)val & 0x3U;
    ctrl10.xl_st_offset = ((uint8_t)val & 0x04U) >> 2;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);
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
int32_t lsm6dsv16b_xl_self_test_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_self_test_t *val)
{
  lsm6dsv16b_ctrl10_t ctrl10;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  //switch (ctrl10.xl_st_offset)
  switch (ctrl10.st_xl)
  {
    case LSM6DSV16B_XL_ST_DISABLE:
      *val = LSM6DSV16B_XL_ST_DISABLE;
      break;

    case LSM6DSV16B_XL_ST_POSITIVE:
      *val = LSM6DSV16B_XL_ST_POSITIVE;
      break;

    case LSM6DSV16B_XL_ST_NEGATIVE:
      *val = LSM6DSV16B_XL_ST_NEGATIVE;
      break;

    default:
      *val = LSM6DSV16B_XL_ST_DISABLE;
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
int32_t lsm6dsv16b_gy_self_test_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_self_test_t val)
{
  lsm6dsv16b_ctrl10_t ctrl10;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  if (ret == 0)
  {
    ctrl10.st_g = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);
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
int32_t lsm6dsv16b_gy_self_test_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_self_test_t *val)
{
  lsm6dsv16b_ctrl10_t ctrl10;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  switch (ctrl10.st_g)
  {
    case LSM6DSV16B_GY_ST_DISABLE:
      *val = LSM6DSV16B_GY_ST_DISABLE;
      break;

    case LSM6DSV16B_GY_ST_POSITIVE:
      *val = LSM6DSV16B_GY_ST_POSITIVE;
      break;

    case LSM6DSV16B_GY_ST_NEGATIVE:
      *val = LSM6DSV16B_GY_ST_NEGATIVE;
      break;

    default:
      *val = LSM6DSV16B_GY_ST_DISABLE;
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
int32_t lsm6dsv16b_all_sources_get(stmdev_ctx_t *ctx,
                                    lsm6dsv16b_all_sources_t *val)
{
  lsm6dsv16b_emb_func_status_mainpage_t emb_func_status_mainpage;
  lsm6dsv16b_emb_func_exec_status_t emb_func_exec_status;
  lsm6dsv16b_fsm_status_mainpage_t fsm_status_mainpage;
  lsm6dsv16b_functions_enable_t functions_enable;
  lsm6dsv16b_emb_func_src_t emb_func_src;
  lsm6dsv16b_fifo_status2_t fifo_status2;
  lsm6dsv16b_all_int_src_t all_int_src;
  lsm6dsv16b_wake_up_src_t wake_up_src;
  lsm6dsv16b_status_reg_t status_reg;
  lsm6dsv16b_d6d_src_t d6d_src;
  lsm6dsv16b_tap_src_t tap_src;
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.dis_rst_lir_all_int = PROPERTY_ENABLE;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_STATUS1, (uint8_t *)&buff, 4);
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
  val->timestamp = status_reg.timestamp_endcount;

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }
  if (ret == 0)
  {
    functions_enable.dis_rst_lir_all_int = PROPERTY_DISABLE;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_SRC, (uint8_t *)&buff, 7);
  }

  if (ret == 0)
  {
    bytecpy((uint8_t *)&wake_up_src, &buff[0]);
    bytecpy((uint8_t *)&tap_src, &buff[1]);
    bytecpy((uint8_t *)&d6d_src, &buff[2]);
    bytecpy((uint8_t *)&emb_func_status_mainpage, &buff[4]);
    bytecpy((uint8_t *)&fsm_status_mainpage, &buff[5]);

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
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EXEC_STATUS, (uint8_t *)&emb_func_exec_status, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);
  val->emb_func_stand_by = emb_func_exec_status.emb_func_endop;
  val->emb_func_time_exceed = emb_func_exec_status.emb_func_exec_ovr;
  val->step_count_inc = emb_func_src.stepcounter_bit_set;
  val->step_count_overflow = emb_func_src.step_overflow;
  val->step_on_delta_time = emb_func_src.step_count_delta_ia;

  val->step_detector = emb_func_src.step_detected;

  return ret;
}

int32_t lsm6dsv16b_flag_data_ready_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_data_ready_t *val)
{
  lsm6dsv16b_status_reg_t status;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_STATUS_REG, (uint8_t *)&status, 1);
  val->drdy_xl = status.xlda;
  val->drdy_gy = status.gda;
  val->drdy_temp = status.tda;

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
int32_t lsm6dsv16b_temperature_raw_get(stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_OUT_TEMP_L, &buff[0], 2);
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
int32_t lsm6dsv16b_angular_rate_raw_get(stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_OUTX_L_G, &buff[0], 6);
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
int32_t lsm6dsv16b_acceleration_raw_get(stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_OUTZ_L_A, &buff[0], 6);
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
int32_t lsm6dsv16b_dual_acceleration_raw_get(stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_UI_OUTZ_L_A_OIS_DUALC, &buff[0], 6);
  val[2] = (int16_t)buff[1];
  val[2] = (val[2] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[0] = (int16_t)buff[5];
  val[0] = (val[0] * 256) + (int16_t)buff[4];

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
int32_t lsm6dsv16b_odr_cal_reg_get(stmdev_ctx_t *ctx, int8_t *val)
{
  lsm6dsv16b_internal_freq_t internal_freq;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INTERNAL_FREQ, (uint8_t *)&internal_freq, 1);
  *val = (int8_t)internal_freq.freq_fine;

  return ret;
}

/**
  * @brief  Enable accelerometer axis.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable accelerometer axis.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_axis_set(stmdev_ctx_t *ctx, lsm6dsv16b_xl_axis_t val)
{
  lsm6dsv16b_tdm_cfg1_t tdm_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  if (ret == 0)
  {
    tdm_cfg1.tdm_xl_z_en = val.z;
    tdm_cfg1.tdm_xl_y_en = val.y;
    tdm_cfg1.tdm_xl_x_en = val.x;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  }

  return ret;
}

/**
  * @brief  Enable accelerometer axis.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enable accelerometer axis.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_xl_axis_get(stmdev_ctx_t *ctx, lsm6dsv16b_xl_axis_t *val)
{
  lsm6dsv16b_tdm_cfg1_t tdm_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  val->x = tdm_cfg1.tdm_xl_x_en;
  val->y = tdm_cfg1.tdm_xl_y_en;
  val->z = tdm_cfg1.tdm_xl_z_en;

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
int32_t lsm6dsv16b_ln_pg_write(stmdev_ctx_t *ctx, uint16_t address,
                                uint8_t *buf, uint8_t len)
{
  lsm6dsv16b_page_address_t  page_address;
  lsm6dsv16b_page_sel_t page_sel;
  lsm6dsv16b_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);

  /* set page write */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_ENABLE;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);

  /* select page */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel, 1);
  page_sel.page_sel = msb;
  page_sel.not_used0 = 1; // Default value
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                               1);

  /* set page addr */
  page_address.page_addr = lsb;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_ADDRESS,
                               (uint8_t *)&page_address, 1);

  for (i = 0; ((i < len) && (ret == 0)); i++)
  {
    ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_VALUE, &buf[i], 1);
    lsb++;

    /* Check if page wrap */
    if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
    {
      msb++;
      ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel, 1);

      if (ret == 0)
      {
        page_sel.page_sel = msb;
        page_sel.not_used0 = 1; // Default value
        ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                                     1);
      }
    }
  }

  page_sel.page_sel = 0;
  page_sel.not_used0 = 1;// Default value
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                               1);

  /* unset page write */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_ln_pg_read(stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len)
{
  lsm6dsv16b_page_address_t  page_address;
  lsm6dsv16b_page_sel_t page_sel;
  lsm6dsv16b_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);

  /* set page write */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  page_rw.page_read = PROPERTY_ENABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);

  /* select page */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel, 1);
  page_sel.page_sel = msb;
  page_sel.not_used0 = 1; // Default value
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                               1);

  /* set page addr */
  page_address.page_addr = lsb;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_ADDRESS,
                               (uint8_t *)&page_address, 1);

  for (i = 0; ((i < len) && (ret == 0)); i++)
  {
    ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_VALUE, &buf[i], 1);
    lsb++;

    /* Check if page wrap */
    if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
    {
      msb++;
      ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel, 1);

      if (ret == 0)
      {
        page_sel.page_sel = msb;
        page_sel.not_used0 = 1; // Default value
        ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                                     1);
      }
    }
  }

  page_sel.page_sel = 0;
  page_sel.not_used0 = 1;// Default value
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_SEL, (uint8_t *)&page_sel,
                               1);

  /* unset page write */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  page_rw.page_read = PROPERTY_DISABLE;
  page_rw.page_write = PROPERTY_DISABLE;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_timestamp_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.timestamp_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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
int32_t lsm6dsv16b_timestamp_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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
int32_t lsm6dsv16b_timestamp_raw_get(stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TIMESTAMP0, &buff[0], 4);
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
int32_t lsm6dsv16b_filt_anti_spike_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_filt_anti_spike_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);

  if (ret == 0)
  {
    if_cfg.asf_ctrl = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_filt_anti_spike_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_filt_anti_spike_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  switch (if_cfg.asf_ctrl)
  {
    case LSM6DSV16B_AUTO:
      *val = LSM6DSV16B_AUTO;
      break;

    case LSM6DSV16B_ALWAYS_ACTIVE:
      *val = LSM6DSV16B_ALWAYS_ACTIVE;
      break;

    default:
      *val = LSM6DSV16B_AUTO;
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
int32_t lsm6dsv16b_filt_settling_mask_set(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_filt_settling_mask_t val)
{
  lsm6dsv16b_emb_func_cfg_t emb_func_cfg;
  lsm6dsv16b_tdm_cfg2_t tdm_cfg2;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    ctrl4.drdy_mask = val.drdy;

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);
  }

  if (ret == 0)
  {
    emb_func_cfg.emb_func_irq_mask_xl_settl = val.irq_xl;
    emb_func_cfg.emb_func_irq_mask_g_settl = val.irq_g;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
  }

  if (ret == 0)
  {
    tdm_cfg2.tdm_data_mask = val.tdm_excep_code;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
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
int32_t lsm6dsv16b_filt_settling_mask_get(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_filt_settling_mask_t *val)
{
  lsm6dsv16b_emb_func_cfg_t emb_func_cfg;
  lsm6dsv16b_tdm_cfg2_t tdm_cfg2;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_CFG, (uint8_t *)&emb_func_cfg, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
  }

  val->drdy = ctrl4.drdy_mask;
  val->irq_xl = emb_func_cfg.emb_func_irq_mask_xl_settl;
  val->irq_g = emb_func_cfg.emb_func_irq_mask_g_settl;
  val->tdm_excep_code = tdm_cfg2.tdm_data_mask;

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
int32_t lsm6dsv16b_filt_gy_lp1_bandwidth_set(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_filt_gy_lp1_bandwidth_t val)
{
  lsm6dsv16b_ctrl6_t ctrl6;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);
  if (ret == 0)
  {
    ctrl6.lpf1_g_bw = (uint8_t)val & 0x7U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);
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
int32_t lsm6dsv16b_filt_gy_lp1_bandwidth_get(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_filt_gy_lp1_bandwidth_t *val)
{
  lsm6dsv16b_ctrl6_t ctrl6;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL6, (uint8_t *)&ctrl6, 1);

  switch (ctrl6.lpf1_g_bw)
  {
    case LSM6DSV16B_GY_ULTRA_LIGHT:
      *val = LSM6DSV16B_GY_ULTRA_LIGHT;
      break;

    case LSM6DSV16B_GY_VERY_LIGHT:
      *val = LSM6DSV16B_GY_VERY_LIGHT;
      break;

    case LSM6DSV16B_GY_LIGHT:
      *val = LSM6DSV16B_GY_LIGHT;
      break;

    case LSM6DSV16B_GY_MEDIUM:
      *val = LSM6DSV16B_GY_MEDIUM;
      break;

    case LSM6DSV16B_GY_STRONG:
      *val = LSM6DSV16B_GY_STRONG;
      break;

    case LSM6DSV16B_GY_VERY_STRONG:
      *val = LSM6DSV16B_GY_VERY_STRONG;
      break;

    case LSM6DSV16B_GY_AGGRESSIVE:
      *val = LSM6DSV16B_GY_AGGRESSIVE;
      break;

    case LSM6DSV16B_GY_XTREME:
      *val = LSM6DSV16B_GY_XTREME;
      break;

    default:
      *val = LSM6DSV16B_GY_ULTRA_LIGHT;
      break;
  }
  return ret;
}

/**
  * @brief  It enables gyroscope digital LPF1 filter. If the OIS chain is disabled, the bandwidth can be selected through LPF1_G_BW.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables gyroscope digital LPF1 filter. If the OIS chain is disabled, the bandwidth can be selected through LPF1_G_BW.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_filt_gy_lp1_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl7_t ctrl7;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL7, (uint8_t *)&ctrl7, 1);

  if (ret == 0)
  {
    ctrl7.lpf1_g_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL7, (uint8_t *)&ctrl7, 1);
  }

  return ret;
}

/**
  * @brief  It enables gyroscope digital LPF1 filter. If the OIS chain is disabled, the bandwidth can be selected through LPF1_G_BW.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It enables gyroscope digital LPF1 filter. If the OIS chain is disabled, the bandwidth can be selected through LPF1_G_BW.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_filt_gy_lp1_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl7_t ctrl7;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL7, (uint8_t *)&ctrl7, 1);
  *val = ctrl7.lpf1_g_en;

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
int32_t lsm6dsv16b_filt_xl_lp2_bandwidth_set(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_filt_xl_lp2_bandwidth_t val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
  if (ret == 0)
  {
    ctrl8.hp_lpf2_xl_bw = (uint8_t)val & 0x7U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
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
int32_t lsm6dsv16b_filt_xl_lp2_bandwidth_get(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_filt_xl_lp2_bandwidth_t *val)
{
  lsm6dsv16b_ctrl8_t ctrl8;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL8, (uint8_t *)&ctrl8, 1);
  switch (ctrl8.hp_lpf2_xl_bw)
  {
    case LSM6DSV16B_XL_ULTRA_LIGHT:
      *val = LSM6DSV16B_XL_ULTRA_LIGHT;
      break;

    case LSM6DSV16B_XL_VERY_LIGHT:
      *val = LSM6DSV16B_XL_VERY_LIGHT;
      break;

    case LSM6DSV16B_XL_LIGHT:
      *val = LSM6DSV16B_XL_LIGHT;
      break;

    case LSM6DSV16B_XL_MEDIUM:
      *val = LSM6DSV16B_XL_MEDIUM;
      break;

    case LSM6DSV16B_XL_STRONG:
      *val = LSM6DSV16B_XL_STRONG;
      break;

    case LSM6DSV16B_XL_VERY_STRONG:
      *val = LSM6DSV16B_XL_VERY_STRONG;
      break;

    case LSM6DSV16B_XL_AGGRESSIVE:
      *val = LSM6DSV16B_XL_AGGRESSIVE;
      break;

    case LSM6DSV16B_XL_XTREME:
      *val = LSM6DSV16B_XL_XTREME;
      break;

    default:
      *val = LSM6DSV16B_XL_ULTRA_LIGHT;
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
int32_t lsm6dsv16b_filt_xl_lp2_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.lpf2_xl_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_lp2_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_hp_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.hp_slope_xl_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_hp_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_fast_settling_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.xl_fastsettl_mode = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_fast_settling_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_hp_mode_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_filt_xl_hp_mode_t val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.hp_ref_mode_xl = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_filt_xl_hp_mode_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_filt_xl_hp_mode_t *val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  switch (ctrl9.hp_ref_mode_xl)
  {
    case LSM6DSV16B_HP_MD_NORMAL:
      *val = LSM6DSV16B_HP_MD_NORMAL;
      break;

    case LSM6DSV16B_HP_MD_REFERENCE:
      *val = LSM6DSV16B_HP_MD_REFERENCE;
      break;

    default:
      *val = LSM6DSV16B_HP_MD_NORMAL;
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
int32_t lsm6dsv16b_filt_wkup_act_feed_set(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_filt_wkup_act_feed_t val)
{
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  }

  tap_cfg0.slope_fds = (uint8_t)val & 0x01U;
  wake_up_ths.usr_off_on_wu = ((uint8_t)val & 0x02U) >> 1;

  if (ret == 0)
  {

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }
  if (ret == 0)
  {

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
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
int32_t lsm6dsv16b_filt_wkup_act_feed_get(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_filt_wkup_act_feed_t *val)
{
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  switch ((wake_up_ths.usr_off_on_wu << 1) + tap_cfg0.slope_fds)
  {
    case LSM6DSV16B_WK_FEED_SLOPE:
      *val = LSM6DSV16B_WK_FEED_SLOPE;
      break;

    case LSM6DSV16B_WK_FEED_HIGH_PASS:
      *val = LSM6DSV16B_WK_FEED_HIGH_PASS;
      break;

    case LSM6DSV16B_WK_FEED_LP_WITH_OFFSET:
      *val = LSM6DSV16B_WK_FEED_LP_WITH_OFFSET;
      break;

    default:
      *val = LSM6DSV16B_WK_FEED_SLOPE;
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
int32_t lsm6dsv16b_mask_trigger_xl_settl_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);

  if (ret == 0)
  {
    tap_cfg0.hw_func_mask_xl_settl = val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
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
int32_t lsm6dsv16b_mask_trigger_xl_settl_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
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
int32_t lsm6dsv16b_filt_sixd_feed_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_sixd_feed_t val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.low_pass_on_6d = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
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
int32_t lsm6dsv16b_filt_sixd_feed_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_sixd_feed_t *val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  switch (tap_cfg0.low_pass_on_6d)
  {
    case LSM6DSV16B_SIXD_FEED_ODR_DIV_2:
      *val = LSM6DSV16B_SIXD_FEED_ODR_DIV_2;
      break;

    case LSM6DSV16B_SIXD_FEED_LOW_PASS:
      *val = LSM6DSV16B_SIXD_FEED_LOW_PASS;
      break;

    default:
      *val = LSM6DSV16B_SIXD_FEED_ODR_DIV_2;
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
int32_t lsm6dsv16b_ui_sdo_pull_up_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret == 0)
  {
    pin_ctrl.sdo_pu_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
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
int32_t lsm6dsv16b_ui_sdo_pull_up_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
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
int32_t lsm6dsv16b_ui_i2c_i3c_mode_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_ui_i2c_i3c_mode_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.i2c_i3c_disable = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_ui_i2c_i3c_mode_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_ui_i2c_i3c_mode_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  switch (if_cfg.i2c_i3c_disable)
  {
    case LSM6DSV16B_I2C_I3C_ENABLE:
      *val = LSM6DSV16B_I2C_I3C_ENABLE;
      break;

    case LSM6DSV16B_I2C_I3C_DISABLE:
      *val = LSM6DSV16B_I2C_I3C_DISABLE;
      break;

    default:
      *val = LSM6DSV16B_I2C_I3C_ENABLE;
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
int32_t lsm6dsv16b_spi_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_spi_mode_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.sim = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_spi_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_spi_mode_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  switch (if_cfg.sim)
  {
    case LSM6DSV16B_SPI_4_WIRE:
      *val = LSM6DSV16B_SPI_4_WIRE;
      break;

    case LSM6DSV16B_SPI_3_WIRE:
      *val = LSM6DSV16B_SPI_3_WIRE;
      break;

    default:
      *val = LSM6DSV16B_SPI_4_WIRE;
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
int32_t lsm6dsv16b_ui_sda_pull_up_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.sda_pu_en = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_ui_sda_pull_up_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_i3c_ibi_time_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_i3c_ibi_time_t val)
{
  lsm6dsv16b_ctrl5_t ctrl5;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret == 0)
  {
    ctrl5.bus_act_sel = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
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
int32_t lsm6dsv16b_i3c_ibi_time_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_i3c_ibi_time_t *val)
{
  lsm6dsv16b_ctrl5_t ctrl5;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
  switch (ctrl5.bus_act_sel)
  {
    case LSM6DSV16B_IBI_2us:
      *val = LSM6DSV16B_IBI_2us;
      break;

    case LSM6DSV16B_IBI_50us:
      *val = LSM6DSV16B_IBI_50us;
      break;

    case LSM6DSV16B_IBI_1ms:
      *val = LSM6DSV16B_IBI_1ms;
      break;

    case LSM6DSV16B_IBI_25ms:
      *val = LSM6DSV16B_IBI_25ms;
      break;

    default:
      *val = LSM6DSV16B_IBI_2us;
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
int32_t lsm6dsv16b_int_pin_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_int_pin_mode_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.pp_od = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_int_pin_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_int_pin_mode_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  switch (if_cfg.pp_od)
  {
    case LSM6DSV16B_PUSH_PULL:
      *val = LSM6DSV16B_PUSH_PULL;
      break;

    case LSM6DSV16B_OPEN_DRAIN:
      *val = LSM6DSV16B_OPEN_DRAIN;
      break;

    default:
      *val = LSM6DSV16B_PUSH_PULL;
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
int32_t lsm6dsv16b_pin_polarity_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_pin_polarity_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.h_lactive = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
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
int32_t lsm6dsv16b_pin_polarity_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_pin_polarity_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  switch (if_cfg.h_lactive)
  {
    case LSM6DSV16B_ACTIVE_HIGH:
      *val = LSM6DSV16B_ACTIVE_HIGH;
      break;

    case LSM6DSV16B_ACTIVE_LOW:
      *val = LSM6DSV16B_ACTIVE_LOW;
      break;

    default:
      *val = LSM6DSV16B_ACTIVE_HIGH;
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
int32_t lsm6dsv16b_pin_int1_route_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_pin_int1_route_t val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  lsm6dsv16b_pin_int2_route_t  pin_int2_route;
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_emb_func_int1_t emb_func_int1;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  lsm6dsv16b_int2_ctrl_t int2_ctrl;
  lsm6dsv16b_int1_ctrl_t int1_ctrl;
  lsm6dsv16b_fsm_int1_t fsm_int1;
  lsm6dsv16b_md1_cfg_t md1_cfg;
  lsm6dsv16b_md2_cfg_t md2_cfg;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_INT1, (uint8_t *)&fsm_int1, 1);
  }

  if (ret == 0)
  {
    emb_func_int1.int1_step_detector = val.step_detector;
    emb_func_int1.int1_tilt = val.tilt;
    emb_func_int1.int1_sig_mot = val.sig_mot;
    emb_func_int1.int1_fsm_lc = val.fsm_lc;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FSM_INT1, (uint8_t *)&fsm_int1, 1);
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }

  if (ret == 0)
  {
    int2_ctrl.int2_emb_func_endop = val.emb_func_stand_by;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    md2_cfg.int2_timestamp = val.timestamp;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    inactivity_dur.sleep_status_on_int = val.sleep_status;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  }

  if (ret == 0)
  {
    int1_ctrl.int1_drdy_xl = val.drdy_xl;
    int1_ctrl.int1_drdy_g = val.drdy_gy;
    int1_ctrl.int1_fifo_th = val.fifo_th;
    int1_ctrl.int1_fifo_ovr = val.fifo_ovr;
    int1_ctrl.int1_fifo_full = val.fifo_full;
    int1_ctrl.int1_cnt_bdr = val.fifo_bdr;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD1_CFG, (uint8_t *)&md1_cfg, 1);
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
         | fsm_int1.int1_fsm8) != PROPERTY_DISABLE)
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    pedo_cmd_reg.carry_count_en = val.step_count_overflow;
    ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_pin_int2_route_get(ctx, &pin_int2_route);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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
int32_t lsm6dsv16b_pin_int1_route_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_pin_int1_route_t *val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_emb_func_int1_t emb_func_int1;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  lsm6dsv16b_int1_ctrl_t int1_ctrl;
  lsm6dsv16b_int2_ctrl_t int2_ctrl;
  lsm6dsv16b_fsm_int1_t fsm_int1;
  lsm6dsv16b_md1_cfg_t md1_cfg;
  lsm6dsv16b_md2_cfg_t md2_cfg;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;


  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ctrl4.int2_on_int1 == PROPERTY_ENABLE)
  {
    if (ret == 0)
    {
      ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
      val->emb_func_stand_by = int2_ctrl.int2_emb_func_endop;
    }
    if (ret == 0)
    {
      ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
      val->timestamp = md2_cfg.int2_timestamp;
    }
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
    val->sleep_status = inactivity_dur.sleep_status_on_int;
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
    val->drdy_xl = int1_ctrl.int1_drdy_xl;
    val->drdy_gy = int1_ctrl.int1_drdy_g;
    val->fifo_th = int1_ctrl.int1_fifo_th;
    val->fifo_ovr = int1_ctrl.int1_fifo_ovr;
    val->fifo_full = int1_ctrl.int1_fifo_full;
    val->fifo_bdr = int1_ctrl.int1_cnt_bdr;
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD1_CFG, (uint8_t *)&md1_cfg, 1);
    val->six_d = md1_cfg.int1_6d;
    val->double_tap = md1_cfg.int1_double_tap;
    val->free_fall = md1_cfg.int1_ff;
    val->wake_up = md1_cfg.int1_wu;
    val->single_tap = md1_cfg.int1_single_tap;
    val->sleep_change = md1_cfg.int1_sleep_change;
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
    val->step_detector = emb_func_int1.int1_step_detector;
    val->tilt = emb_func_int1.int1_tilt;
    val->sig_mot = emb_func_int1.int1_sig_mot;
    val->fsm_lc = emb_func_int1.int1_fsm_lc;
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_INT1, (uint8_t *)&fsm_int1, 1);
    val->fsm1 = fsm_int1.int1_fsm1;
    val->fsm2 = fsm_int1.int1_fsm2;
    val->fsm3 = fsm_int1.int1_fsm3;
    val->fsm4 = fsm_int1.int1_fsm4;
    val->fsm5 = fsm_int1.int1_fsm5;
    val->fsm6 = fsm_int1.int1_fsm6;
    val->fsm7 = fsm_int1.int1_fsm7;
    val->fsm8 = fsm_int1.int1_fsm8;
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
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
int32_t lsm6dsv16b_pin_int2_route_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_pin_int2_route_t val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  lsm6dsv16b_pin_int1_route_t  pin_int1_route;
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_emb_func_int2_t emb_func_int2;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  lsm6dsv16b_int2_ctrl_t int2_ctrl;
  lsm6dsv16b_fsm_int2_t fsm_int2;
  lsm6dsv16b_md2_cfg_t md2_cfg;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;


  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_INT2, (uint8_t *)&fsm_int2, 1);
  }

  if (ret == 0)
  {
    emb_func_int2.int2_step_detector = val.step_detector;
    emb_func_int2.int2_tilt = val.tilt;
    emb_func_int2.int2_sig_mot = val.sig_mot;
    emb_func_int2.int2_fsm_lc = val.fsm_lc;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FSM_INT2, (uint8_t *)&fsm_int2, 1);
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  }
  if (ret == 0)
  {
    if ((val.emb_func_stand_by | val.timestamp) != PROPERTY_DISABLE)
    {
      ctrl4.int2_on_int1 = PROPERTY_DISABLE;
    }
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    inactivity_dur.sleep_status_on_int = val.sleep_status;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
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
         | fsm_int2.int2_fsm8) != PROPERTY_DISABLE)
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  if (ret == 0)
  {
    pedo_cmd_reg.carry_count_en = val.step_count_overflow;
    ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_pin_int1_route_get(ctx, &pin_int1_route);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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
int32_t lsm6dsv16b_pin_int2_route_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_pin_int2_route_t *val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_emb_func_int2_t emb_func_int2;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  lsm6dsv16b_int2_ctrl_t int2_ctrl;
  lsm6dsv16b_fsm_int2_t fsm_int2;
  lsm6dsv16b_md2_cfg_t md2_cfg;
  lsm6dsv16b_ctrl4_t ctrl4;
  int32_t ret;


  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL4, (uint8_t *)&ctrl4, 1);
  if (ctrl4.int2_on_int1 == PROPERTY_DISABLE)
  {
    if (ret == 0)
    {
      ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
      val->emb_func_stand_by = int2_ctrl.int2_emb_func_endop;
    }
    if (ret == 0)
    {
      ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
      val->timestamp = md2_cfg.int2_timestamp;
    }
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
    val->sleep_status = inactivity_dur.sleep_status_on_int;
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
    val->drdy_xl = int2_ctrl.int2_drdy_xl;
    val->drdy_gy = int2_ctrl.int2_drdy_g;
    val->fifo_th = int2_ctrl.int2_fifo_th;
    val->fifo_ovr = int2_ctrl.int2_fifo_ovr;
    val->fifo_full = int2_ctrl.int2_fifo_full;
    val->fifo_bdr = int2_ctrl.int2_cnt_bdr;
  }


  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_MD2_CFG, (uint8_t *)&md2_cfg, 1);
    val->six_d = md2_cfg.int2_6d;
    val->double_tap = md2_cfg.int2_double_tap;
    val->free_fall = md2_cfg.int2_ff;
    val->wake_up = md2_cfg.int2_wu;
    val->single_tap = md2_cfg.int2_single_tap;
    val->sleep_change = md2_cfg.int2_sleep_change;
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
    val->step_detector = emb_func_int2.int2_step_detector;
    val->tilt = emb_func_int2.int2_tilt;
    val->sig_mot = emb_func_int2.int2_sig_mot;
    val->fsm_lc = emb_func_int2.int2_fsm_lc;
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_INT2, (uint8_t *)&fsm_int2, 1);
    val->fsm1 = fsm_int2.int2_fsm1;
    val->fsm2 = fsm_int2.int2_fsm2;
    val->fsm3 = fsm_int2.int2_fsm3;
    val->fsm4 = fsm_int2.int2_fsm4;
    val->fsm5 = fsm_int2.int2_fsm5;
    val->fsm6 = fsm_int2.int2_fsm6;
    val->fsm7 = fsm_int2.int2_fsm7;
    val->fsm8 = fsm_int2.int2_fsm8;
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
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
int32_t lsm6dsv16b_pin_int_en_when_i2c_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl5_t ctrl5;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
  if (ret == 0)
  {
    ctrl5.int_en_i3c = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
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
int32_t lsm6dsv16b_pin_int_en_when_i2c_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl5_t ctrl5;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL5, (uint8_t *)&ctrl5, 1);
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
int32_t lsm6dsv16b_int_notification_set(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_int_notification_t val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  lsm6dsv16b_page_rw_t page_rw;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.lir = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  if (ret == 0)
  {
    page_rw.emb_func_lir = ((uint8_t)val & 0x02U) >> 1;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_int_notification_get(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_int_notification_t *val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  lsm6dsv16b_page_rw_t page_rw;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  switch ((page_rw.emb_func_lir << 1) + tap_cfg0.lir)
  {
    case LSM6DSV16B_ALL_INT_PULSED:
      *val = LSM6DSV16B_ALL_INT_PULSED;
      break;

    case LSM6DSV16B_BASE_LATCHED_EMB_PULSED:
      *val = LSM6DSV16B_BASE_LATCHED_EMB_PULSED;
      break;

    case LSM6DSV16B_BASE_PULSED_EMB_LATCHED:
      *val = LSM6DSV16B_BASE_PULSED_EMB_LATCHED;
      break;

    case LSM6DSV16B_ALL_INT_LATCHED:
      *val = LSM6DSV16B_ALL_INT_LATCHED;
      break;

    default:
      *val = LSM6DSV16B_ALL_INT_PULSED;
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
int32_t lsm6dsv16b_act_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_act_mode_t val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  if (ret == 0)
  {
    functions_enable.inact_en = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
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
int32_t lsm6dsv16b_act_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_act_mode_t *val)
{
  lsm6dsv16b_functions_enable_t functions_enable;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNCTIONS_ENABLE, (uint8_t *)&functions_enable, 1);
  switch (functions_enable.inact_en)
  {
    case LSM6DSV16B_XL_AND_GY_NOT_AFFECTED:
      *val = LSM6DSV16B_XL_AND_GY_NOT_AFFECTED;
      break;

    case LSM6DSV16B_XL_LOW_POWER_GY_NOT_AFFECTED:
      *val = LSM6DSV16B_XL_LOW_POWER_GY_NOT_AFFECTED;
      break;

    case LSM6DSV16B_XL_LOW_POWER_GY_SLEEP:
      *val = LSM6DSV16B_XL_LOW_POWER_GY_SLEEP;
      break;

    case LSM6DSV16B_XL_LOW_POWER_GY_POWER_DOWN:
      *val = LSM6DSV16B_XL_LOW_POWER_GY_POWER_DOWN;
      break;

    default:
      *val = LSM6DSV16B_XL_AND_GY_NOT_AFFECTED;
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
int32_t lsm6dsv16b_act_from_sleep_to_act_dur_set(stmdev_ctx_t *ctx,
                                                  lsm6dsv16b_act_from_sleep_to_act_dur_t val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    inactivity_dur.inact_dur = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
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
int32_t lsm6dsv16b_act_from_sleep_to_act_dur_get(stmdev_ctx_t *ctx,
                                                  lsm6dsv16b_act_from_sleep_to_act_dur_t *val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  switch (inactivity_dur.inact_dur)
  {
    case LSM6DSV16B_SLEEP_TO_ACT_AT_1ST_SAMPLE:
      *val = LSM6DSV16B_SLEEP_TO_ACT_AT_1ST_SAMPLE;
      break;

    case LSM6DSV16B_SLEEP_TO_ACT_AT_2ND_SAMPLE:
      *val = LSM6DSV16B_SLEEP_TO_ACT_AT_2ND_SAMPLE;
      break;

    case LSM6DSV16B_SLEEP_TO_ACT_AT_3RD_SAMPLE:
      *val = LSM6DSV16B_SLEEP_TO_ACT_AT_3RD_SAMPLE;
      break;

    case LSM6DSV16B_SLEEP_TO_ACT_AT_4th_SAMPLE:
      *val = LSM6DSV16B_SLEEP_TO_ACT_AT_4th_SAMPLE;
      break;

    default:
      *val = LSM6DSV16B_SLEEP_TO_ACT_AT_1ST_SAMPLE;
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
int32_t lsm6dsv16b_act_sleep_xl_odr_set(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_act_sleep_xl_odr_t val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    inactivity_dur.xl_inact_odr = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
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
int32_t lsm6dsv16b_act_sleep_xl_odr_get(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_act_sleep_xl_odr_t *val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  switch (inactivity_dur.xl_inact_odr)
  {
    case LSM6DSV16B_1Hz875:
      *val = LSM6DSV16B_1Hz875;
      break;

    case LSM6DSV16B_15Hz:
      *val = LSM6DSV16B_15Hz;
      break;

    case LSM6DSV16B_30Hz:
      *val = LSM6DSV16B_30Hz;
      break;

    case LSM6DSV16B_60Hz:
      *val = LSM6DSV16B_60Hz;
      break;

    default:
      *val = LSM6DSV16B_1Hz875;
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
int32_t lsm6dsv16b_act_thresholds_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_act_thresholds_t val)
{
  lsm6dsv16b_inactivity_ths_t inactivity_ths;
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  int32_t ret;
  float_t tmp;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  }
  if (ret == 0)
  {

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  }
  if (ret == 0)
  {

    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
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
int32_t lsm6dsv16b_act_thresholds_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_act_thresholds_t *val)
{
  lsm6dsv16b_inactivity_dur_t inactivity_dur;
  lsm6dsv16b_inactivity_ths_t inactivity_ths;
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  int32_t ret;
  float_t tmp;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_DUR, (uint8_t *)&inactivity_dur, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_INACTIVITY_THS, (uint8_t *)&inactivity_ths, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
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
int32_t lsm6dsv16b_act_wkup_time_windows_set(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_act_wkup_time_windows_t val)
{
  lsm6dsv16b_wake_up_dur_t wake_up_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret == 0)
  {
    wake_up_dur.wake_dur = val.shock;
    wake_up_dur.sleep_dur = val.quiet;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
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
int32_t lsm6dsv16b_act_wkup_time_windows_get(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_act_wkup_time_windows_t *val)
{
  lsm6dsv16b_wake_up_dur_t wake_up_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
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
int32_t lsm6dsv16b_tap_detection_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tap_detection_t val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  if (ret == 0)
  {
    tap_cfg0.tap_x_en = val.tap_x_en;
    tap_cfg0.tap_y_en = val.tap_y_en;
    tap_cfg0.tap_z_en = val.tap_z_en;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
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
int32_t lsm6dsv16b_tap_detection_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tap_detection_t *val)
{
  lsm6dsv16b_tap_cfg0_t tap_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
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
int32_t lsm6dsv16b_tap_thresholds_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_tap_thresholds_t val)
{
  lsm6dsv16b_tap_ths_6d_t tap_ths_6d;
  lsm6dsv16b_tap_cfg2_t tap_cfg2;
  lsm6dsv16b_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  }

  tap_cfg1.tap_ths_z = val.z;
  tap_cfg2.tap_ths_y = val.y;
  tap_ths_6d.tap_ths_x = val.x;

  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
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
int32_t lsm6dsv16b_tap_thresholds_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_tap_thresholds_t *val)
{
  lsm6dsv16b_tap_ths_6d_t tap_ths_6d;
  lsm6dsv16b_tap_cfg2_t tap_cfg2;
  lsm6dsv16b_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
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
int32_t lsm6dsv16b_tap_axis_priority_set(stmdev_ctx_t *ctx,
                                          lsm6dsv16b_tap_axis_priority_t val)
{
  lsm6dsv16b_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  if (ret == 0)
  {
    tap_cfg1.tap_priority = (uint8_t)val & 0x07U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
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
int32_t lsm6dsv16b_tap_axis_priority_get(stmdev_ctx_t *ctx,
                                          lsm6dsv16b_tap_axis_priority_t *val)
{
  lsm6dsv16b_tap_cfg1_t tap_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  switch (tap_cfg1.tap_priority)
  {
    case LSM6DSV16B_XYZ :
      *val = LSM6DSV16B_XYZ ;
      break;

    case LSM6DSV16B_YXZ :
      *val = LSM6DSV16B_YXZ ;
      break;

    case LSM6DSV16B_XZY:
      *val = LSM6DSV16B_XZY;
      break;

    case LSM6DSV16B_ZYX :
      *val = LSM6DSV16B_ZYX ;
      break;

    case LSM6DSV16B_YZX :
      *val = LSM6DSV16B_YZX ;
      break;

    case LSM6DSV16B_ZXY :
      *val = LSM6DSV16B_ZXY ;
      break;

    default:
      *val = LSM6DSV16B_XYZ ;
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
int32_t lsm6dsv16b_tap_time_windows_set(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tap_time_windows_t val)
{
  lsm6dsv16b_tap_dur_t tap_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_DUR, (uint8_t *)&tap_dur, 1);
  if (ret == 0)
  {
    tap_dur.shock = val.shock;
    tap_dur.quiet = val.quiet;
    tap_dur.dur = val.tap_gap;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_DUR, (uint8_t *)&tap_dur, 1);
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
int32_t lsm6dsv16b_tap_time_windows_get(stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tap_time_windows_t *val)
{
  lsm6dsv16b_tap_dur_t tap_dur;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_DUR, (uint8_t *)&tap_dur, 1);
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
int32_t lsm6dsv16b_tap_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_tap_mode_t val)
{
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  if (ret == 0)
  {
    wake_up_ths.single_double_tap = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
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
int32_t lsm6dsv16b_tap_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_tap_mode_t *val)
{
  lsm6dsv16b_wake_up_ths_t wake_up_ths;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_THS, (uint8_t *)&wake_up_ths, 1);
  switch (wake_up_ths.single_double_tap)
  {
    case LSM6DSV16B_ONLY_SINGLE:
      *val = LSM6DSV16B_ONLY_SINGLE;
      break;

    case LSM6DSV16B_BOTH_SINGLE_DOUBLE:
      *val = LSM6DSV16B_BOTH_SINGLE_DOUBLE;
      break;

    default:
      *val = LSM6DSV16B_ONLY_SINGLE;
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
int32_t lsm6dsv16b_6d_threshold_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_6d_threshold_t val)
{
  lsm6dsv16b_tap_ths_6d_t tap_ths_6d;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  if (ret == 0)
  {
    tap_ths_6d.sixd_ths = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
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
int32_t lsm6dsv16b_6d_threshold_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_6d_threshold_t *val)
{
  lsm6dsv16b_tap_ths_6d_t tap_ths_6d;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TAP_THS_6D, (uint8_t *)&tap_ths_6d, 1);
  switch (tap_ths_6d.sixd_ths)
  {
    case LSM6DSV16B_DEG_80:
      *val = LSM6DSV16B_DEG_80;
      break;

    case LSM6DSV16B_DEG_70:
      *val = LSM6DSV16B_DEG_70;
      break;

    case LSM6DSV16B_DEG_60:
      *val = LSM6DSV16B_DEG_60;
      break;

    case LSM6DSV16B_DEG_50:
      *val = LSM6DSV16B_DEG_50;
      break;

    default:
      *val = LSM6DSV16B_DEG_80;
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
int32_t lsm6dsv16b_ff_time_windows_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_wake_up_dur_t wake_up_dur;
  lsm6dsv16b_free_fall_t free_fall;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret == 0)
  {
    wake_up_dur.ff_dur = ((uint8_t)val & 0x20U) >> 5;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  if (ret == 0)
  {
    free_fall.ff_dur = (uint8_t)val & 0x1FU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);
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
int32_t lsm6dsv16b_ff_time_windows_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_wake_up_dur_t wake_up_dur;
  lsm6dsv16b_free_fall_t free_fall;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);
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
int32_t lsm6dsv16b_ff_thresholds_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_ff_thresholds_t val)
{
  lsm6dsv16b_free_fall_t free_fall;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);
  if (ret == 0)
  {
    free_fall.ff_ths = (uint8_t)val & 0x7U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);
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
int32_t lsm6dsv16b_ff_thresholds_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_ff_thresholds_t *val)
{
  lsm6dsv16b_free_fall_t free_fall;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FREE_FALL, (uint8_t *)&free_fall, 1);

  switch (free_fall.ff_ths)
  {
    case LSM6DSV16B_156_mg:
      *val = LSM6DSV16B_156_mg;
      break;

    case LSM6DSV16B_219_mg:
      *val = LSM6DSV16B_219_mg;
      break;

    case LSM6DSV16B_250_mg:
      *val = LSM6DSV16B_250_mg;
      break;

    case LSM6DSV16B_312_mg:
      *val = LSM6DSV16B_312_mg;
      break;

    case LSM6DSV16B_344_mg:
      *val = LSM6DSV16B_344_mg;
      break;

    case LSM6DSV16B_406_mg:
      *val = LSM6DSV16B_406_mg;
      break;

    case LSM6DSV16B_469_mg:
      *val = LSM6DSV16B_469_mg;
      break;

    case LSM6DSV16B_500_mg:
      *val = LSM6DSV16B_500_mg;
      break;

    default:
      *val = LSM6DSV16B_156_mg;
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
int32_t lsm6dsv16b_fifo_watermark_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_fifo_ctrl1_t fifo_ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);

  if (ret == 0)
  {
    fifo_ctrl1.wtm = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);
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
int32_t lsm6dsv16b_fifo_watermark_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_fifo_ctrl1_t fifo_ctrl1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL1, (uint8_t *)&fifo_ctrl1, 1);
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
int32_t lsm6dsv16b_fifo_xl_dual_fsm_batch_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.xl_dualc_batch_from_fsm = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_xl_dual_fsm_batch_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_compress_algo_set(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_fifo_compress_algo_t val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.uncompr_rate = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_compress_algo_get(stmdev_ctx_t *ctx,
                                           lsm6dsv16b_fifo_compress_algo_t *val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);

  switch (fifo_ctrl2.uncompr_rate)
  {
    case LSM6DSV16B_CMP_DISABLE:
      *val = LSM6DSV16B_CMP_DISABLE;
      break;

    case LSM6DSV16B_CMP_8_TO_1:
      *val = LSM6DSV16B_CMP_8_TO_1;
      break;

    case LSM6DSV16B_CMP_16_TO_1:
      *val = LSM6DSV16B_CMP_16_TO_1;
      break;

    case LSM6DSV16B_CMP_32_TO_1:
      *val = LSM6DSV16B_CMP_32_TO_1;
      break;

    default:
      *val = LSM6DSV16B_CMP_DISABLE;
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
int32_t lsm6dsv16b_fifo_virtual_sens_odr_chg_set(stmdev_ctx_t *ctx,
                                                  uint8_t val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.odr_chg_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_virtual_sens_odr_chg_get(stmdev_ctx_t *ctx,
                                                  uint8_t *val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_compress_algo_real_time_set(stmdev_ctx_t *ctx,
                                                     uint8_t val)
{
  lsm6dsv16b_emb_func_en_b_t emb_func_en_b;
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;

  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.fifo_compr_rt_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  if (ret == 0)
  {
    ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }
  if (ret == 0)
  {
    emb_func_en_b.fifo_compr_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fifo_compress_algo_real_time_get(stmdev_ctx_t *ctx,
                                                     uint8_t *val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);

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
int32_t lsm6dsv16b_fifo_stop_on_wtm_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.stop_on_wtm = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
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
int32_t lsm6dsv16b_fifo_stop_on_wtm_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  *val = fifo_ctrl2.stop_on_wtm;

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for accelerometer data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_NOT_BATCHED, XL_BATCHED_AT_1Hz875, XL_BATCHED_AT_7Hz5, XL_BATCHED_AT_15Hz, XL_BATCHED_AT_30Hz, XL_BATCHED_AT_60Hz, XL_BATCHED_AT_120Hz, XL_BATCHED_AT_240Hz, XL_BATCHED_AT_480Hz, XL_BATCHED_AT_960Hz, XL_BATCHED_AT_1920Hz, XL_BATCHED_AT_3840Hz, XL_BATCHED_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_xl_batch_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fifo_xl_batch_t val)
{
  lsm6dsv16b_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.bdr_xl = (uint8_t)val & 0xFU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for accelerometer data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_NOT_BATCHED, XL_BATCHED_AT_1Hz875, XL_BATCHED_AT_7Hz5, XL_BATCHED_AT_15Hz, XL_BATCHED_AT_30Hz, XL_BATCHED_AT_60Hz, XL_BATCHED_AT_120Hz, XL_BATCHED_AT_240Hz, XL_BATCHED_AT_480Hz, XL_BATCHED_AT_960Hz, XL_BATCHED_AT_1920Hz, XL_BATCHED_AT_3840Hz, XL_BATCHED_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_xl_batch_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fifo_xl_batch_t *val)
{
  lsm6dsv16b_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  switch (fifo_ctrl3.bdr_xl)
  {
    case LSM6DSV16B_XL_NOT_BATCHED:
      *val = LSM6DSV16B_XL_NOT_BATCHED;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_1Hz875:
      *val = LSM6DSV16B_XL_BATCHED_AT_1Hz875;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_7Hz5:
      *val = LSM6DSV16B_XL_BATCHED_AT_7Hz5;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_15Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_15Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_30Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_30Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_60Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_60Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_120Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_120Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_240Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_240Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_480Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_480Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_960Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_960Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_1920Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_1920Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_3840Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_3840Hz;
      break;

    case LSM6DSV16B_XL_BATCHED_AT_7680Hz:
      *val = LSM6DSV16B_XL_BATCHED_AT_7680Hz;
      break;

    default:
      *val = LSM6DSV16B_XL_NOT_BATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for gyroscope data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_NOT_BATCHED, XL_BATCHED_AT_1Hz875, XL_BATCHED_AT_7Hz5, XL_BATCHED_AT_15Hz, XL_BATCHED_AT_30Hz, XL_BATCHED_AT_60Hz, XL_BATCHED_AT_120Hz, XL_BATCHED_AT_240Hz, XL_BATCHED_AT_480Hz, XL_BATCHED_AT_960Hz, XL_BATCHED_AT_1920Hz, XL_BATCHED_AT_3840Hz, XL_BATCHED_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_gy_batch_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fifo_gy_batch_t val)
{
  lsm6dsv16b_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.bdr_gy = (uint8_t)val & 0xFU;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Selects Batch Data Rate (write frequency in FIFO) for gyroscope data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_NOT_BATCHED, XL_BATCHED_AT_1Hz875, XL_BATCHED_AT_7Hz5, XL_BATCHED_AT_15Hz, XL_BATCHED_AT_30Hz, XL_BATCHED_AT_60Hz, XL_BATCHED_AT_120Hz, XL_BATCHED_AT_240Hz, XL_BATCHED_AT_480Hz, XL_BATCHED_AT_960Hz, XL_BATCHED_AT_1920Hz, XL_BATCHED_AT_3840Hz, XL_BATCHED_AT_7680Hz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_gy_batch_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fifo_gy_batch_t *val)
{
  lsm6dsv16b_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  switch (fifo_ctrl3.bdr_gy)
  {
    case LSM6DSV16B_GY_NOT_BATCHED:
      *val = LSM6DSV16B_GY_NOT_BATCHED;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_1Hz875:
      *val = LSM6DSV16B_GY_BATCHED_AT_1Hz875;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_7Hz5:
      *val = LSM6DSV16B_GY_BATCHED_AT_7Hz5;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_15Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_15Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_30Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_30Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_60Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_60Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_120Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_120Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_240Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_240Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_480Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_480Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_960Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_960Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_1920Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_1920Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_3840Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_3840Hz;
      break;

    case LSM6DSV16B_GY_BATCHED_AT_7680Hz:
      *val = LSM6DSV16B_GY_BATCHED_AT_7680Hz;
      break;

    default:
      *val = LSM6DSV16B_GY_NOT_BATCHED;
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
int32_t lsm6dsv16b_fifo_mode_set(stmdev_ctx_t *ctx,
                                  lsm6dsv16b_fifo_mode_t val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.fifo_mode = (uint8_t)val & 0x07U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
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
int32_t lsm6dsv16b_fifo_mode_get(stmdev_ctx_t *ctx,
                                  lsm6dsv16b_fifo_mode_t *val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  switch (fifo_ctrl4.fifo_mode)
  {
    case LSM6DSV16B_BYPASS_MODE:
      *val = LSM6DSV16B_BYPASS_MODE;
      break;

    case LSM6DSV16B_FIFO_MODE:
      *val = LSM6DSV16B_FIFO_MODE;
      break;

    case LSM6DSV16B_STREAM_WTM_TO_FULL_MODE:
      *val = LSM6DSV16B_STREAM_WTM_TO_FULL_MODE;
      break;

    case LSM6DSV16B_STREAM_TO_FIFO_MODE:
      *val = LSM6DSV16B_STREAM_TO_FIFO_MODE;
      break;

    case LSM6DSV16B_BYPASS_TO_STREAM_MODE:
      *val = LSM6DSV16B_BYPASS_TO_STREAM_MODE;
      break;

    case LSM6DSV16B_STREAM_MODE:
      *val = LSM6DSV16B_STREAM_MODE;
      break;

    case LSM6DSV16B_BYPASS_TO_FIFO_MODE:
      *val = LSM6DSV16B_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = LSM6DSV16B_BYPASS_MODE;
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
int32_t lsm6dsv16b_fifo_temp_batch_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_fifo_temp_batch_t val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.odr_t_batch = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
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
int32_t lsm6dsv16b_fifo_temp_batch_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_fifo_temp_batch_t *val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  switch (fifo_ctrl4.odr_t_batch)
  {
    case LSM6DSV16B_TEMP_NOT_BATCHED:
      *val = LSM6DSV16B_TEMP_NOT_BATCHED;
      break;

    case LSM6DSV16B_TEMP_BATCHED_AT_1Hz875:
      *val = LSM6DSV16B_TEMP_BATCHED_AT_1Hz875;
      break;

    case LSM6DSV16B_TEMP_BATCHED_AT_15Hz:
      *val = LSM6DSV16B_TEMP_BATCHED_AT_15Hz;
      break;

    case LSM6DSV16B_TEMP_BATCHED_AT_60Hz:
      *val = LSM6DSV16B_TEMP_BATCHED_AT_60Hz;
      break;

    default:
      *val = LSM6DSV16B_TEMP_NOT_BATCHED;
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
int32_t lsm6dsv16b_fifo_timestamp_batch_set(stmdev_ctx_t *ctx,
                                             lsm6dsv16b_fifo_timestamp_batch_t val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  if (ret == 0)
  {
    fifo_ctrl4.dec_ts_batch = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
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
int32_t lsm6dsv16b_fifo_timestamp_batch_get(stmdev_ctx_t *ctx,
                                             lsm6dsv16b_fifo_timestamp_batch_t *val)
{
  lsm6dsv16b_fifo_ctrl4_t fifo_ctrl4;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_CTRL4, (uint8_t *)&fifo_ctrl4, 1);
  switch (fifo_ctrl4.dec_ts_batch)
  {
    case LSM6DSV16B_TMSTMP_NOT_BATCHED:
      *val = LSM6DSV16B_TMSTMP_NOT_BATCHED;
      break;

    case LSM6DSV16B_TMSTMP_DEC_1:
      *val = LSM6DSV16B_TMSTMP_DEC_1;
      break;

    case LSM6DSV16B_TMSTMP_DEC_8:
      *val = LSM6DSV16B_TMSTMP_DEC_8;
      break;

    case LSM6DSV16B_TMSTMP_DEC_32:
      *val = LSM6DSV16B_TMSTMP_DEC_32;
      break;

    default:
      *val = LSM6DSV16B_TMSTMP_NOT_BATCHED;
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
int32_t lsm6dsv16b_fifo_batch_counter_threshold_set(stmdev_ctx_t *ctx,
                                                     uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_COUNTER_BDR_REG1, (uint8_t *)&buff[0], 2);

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
int32_t lsm6dsv16b_fifo_batch_counter_threshold_get(stmdev_ctx_t *ctx,
                                                     uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_COUNTER_BDR_REG1, &buff[0], 2);
  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @brief  Selects the trigger for the internal counter of batch events between the accelerometer, gyroscope and EIS gyroscope.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_BATCH_EVENT, GY_BATCH_EVENT, GY_EIS_BATCH_EVENT,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_batch_cnt_event_set(stmdev_ctx_t *ctx,
                                             lsm6dsv16b_fifo_batch_cnt_event_t val)
{
  lsm6dsv16b_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);

  if (ret == 0)
  {
    counter_bdr_reg1.trig_counter_bdr = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Selects the trigger for the internal counter of batch events between the accelerometer, gyroscope and EIS gyroscope.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      XL_BATCH_EVENT, GY_BATCH_EVENT, GY_EIS_BATCH_EVENT,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_batch_cnt_event_get(stmdev_ctx_t *ctx,
                                             lsm6dsv16b_fifo_batch_cnt_event_t *val)
{
  lsm6dsv16b_counter_bdr_reg1_t counter_bdr_reg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_COUNTER_BDR_REG1, (uint8_t *)&counter_bdr_reg1, 1);
  switch (counter_bdr_reg1.trig_counter_bdr)
  {
    case LSM6DSV16B_XL_BATCH_EVENT:
      *val = LSM6DSV16B_XL_BATCH_EVENT;
      break;

    case LSM6DSV16B_GY_BATCH_EVENT:
      *val = LSM6DSV16B_GY_BATCH_EVENT;
      break;

    case LSM6DSV16B_GY_EIS_BATCH_EVENT:
      *val = LSM6DSV16B_GY_EIS_BATCH_EVENT;
      break;

    default:
      *val = LSM6DSV16B_XL_BATCH_EVENT;
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
int32_t lsm6dsv16b_fifo_sflp_batch_set(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_fifo_sflp_raw_t val)
{
  lsm6dsv16b_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
    emb_func_fifo_en_a.sflp_game_fifo_en = val.game_rotation;
    emb_func_fifo_en_a.sflp_gravity_fifo_en = val.gravity;
    emb_func_fifo_en_a.sflp_gbias_fifo_en = val.gbias;
    ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A,
                                 (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fifo_sflp_batch_get(stmdev_ctx_t *ctx,
                                        lsm6dsv16b_fifo_sflp_raw_t *val)
{
  lsm6dsv16b_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);

    val->game_rotation = emb_func_fifo_en_a.sflp_game_fifo_en;
    val->gravity = emb_func_fifo_en_a.sflp_gravity_fifo_en;
    val->gbias = emb_func_fifo_en_a.sflp_gbias_fifo_en;
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fifo_status_get(stmdev_ctx_t *ctx,
                                    lsm6dsv16b_fifo_status_t *val)
{
  uint8_t buff[2];
  lsm6dsv16b_fifo_status2_t status;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_STATUS1, (uint8_t *)&buff[0], 2);
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
  * @param  val      lsm6dsv16b_fifo_out_raw_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fifo_out_raw_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fifo_out_raw_t *val)
{
  lsm6dsv16b_fifo_data_out_tag_t fifo_data_out_tag;
  uint8_t buff[7];
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FIFO_DATA_OUT_TAG, buff, 7);
  bytecpy((uint8_t *)&fifo_data_out_tag, &buff[0]);

  switch (fifo_data_out_tag.tag_sensor)
  {
    case LSM6DSV16B_FIFO_EMPTY:
      val->tag = LSM6DSV16B_FIFO_EMPTY;
      break;

    case LSM6DSV16B_GY_NC_TAG:
      val->tag = LSM6DSV16B_GY_NC_TAG;
      break;

    case LSM6DSV16B_XL_NC_TAG:
      val->tag = LSM6DSV16B_XL_NC_TAG;
      break;

    case LSM6DSV16B_TIMESTAMP_TAG:
      val->tag = LSM6DSV16B_TIMESTAMP_TAG;
      break;

    case LSM6DSV16B_TEMPERATURE_TAG:
      val->tag = LSM6DSV16B_TEMPERATURE_TAG;
      break;

    case LSM6DSV16B_CFG_CHANGE_TAG:
      val->tag = LSM6DSV16B_CFG_CHANGE_TAG;
      break;

    case LSM6DSV16B_XL_NC_T_2_TAG:
      val->tag = LSM6DSV16B_XL_NC_T_2_TAG;
      break;

    case LSM6DSV16B_XL_NC_T_1_TAG:
      val->tag = LSM6DSV16B_XL_NC_T_1_TAG;
      break;

    case LSM6DSV16B_XL_2XC_TAG:
      val->tag = LSM6DSV16B_XL_2XC_TAG;
      break;

    case LSM6DSV16B_XL_3XC_TAG:
      val->tag = LSM6DSV16B_XL_3XC_TAG;
      break;

    case LSM6DSV16B_GY_NC_T_2_TAG:
      val->tag = LSM6DSV16B_GY_NC_T_2_TAG;
      break;

    case LSM6DSV16B_GY_NC_T_1_TAG:
      val->tag = LSM6DSV16B_GY_NC_T_1_TAG;
      break;

    case LSM6DSV16B_GY_2XC_TAG:
      val->tag = LSM6DSV16B_GY_2XC_TAG;
      break;

    case LSM6DSV16B_GY_3XC_TAG:
      val->tag = LSM6DSV16B_GY_3XC_TAG;
      break;

    case LSM6DSV16B_STEP_COUNTER_TAG:
      val->tag = LSM6DSV16B_STEP_COUNTER_TAG;
      break;

    case LSM6DSV16B_SFLP_GAME_ROTATION_VECTOR_TAG:
      val->tag = LSM6DSV16B_SFLP_GAME_ROTATION_VECTOR_TAG;
      break;

    case LSM6DSV16B_SFLP_GYROSCOPE_BIAS_TAG:
      val->tag = LSM6DSV16B_SFLP_GYROSCOPE_BIAS_TAG;
      break;

    case LSM6DSV16B_SFLP_GRAVITY_VECTOR_TAG:
      val->tag = LSM6DSV16B_SFLP_GRAVITY_VECTOR_TAG;
      break;

    case LSM6DSV16B_XL_DUAL_CORE:
      val->tag = LSM6DSV16B_XL_DUAL_CORE;
      break;

    default:
      val->tag = LSM6DSV16B_FIFO_EMPTY;
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
int32_t lsm6dsv16b_fifo_stpcnt_batch_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_fifo_en_a.step_counter_fifo_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fifo_stpcnt_batch_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_emb_func_fifo_en_a_t emb_func_fifo_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_FIFO_EN_A, (uint8_t *)&emb_func_fifo_en_a, 1);
  }

  *val = emb_func_fifo_en_a.step_counter_fifo_en;

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_stpcnt_mode_set(stmdev_ctx_t *ctx,
                                    lsm6dsv16b_stpcnt_mode_t val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  lsm6dsv16b_emb_func_en_b_t emb_func_en_b;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.pedo_en = val.step_counter_enable;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }
  if (ret == 0)
  {
    pedo_cmd_reg.fp_rejection_en = val.false_step_rej;
    ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
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
int32_t lsm6dsv16b_stpcnt_mode_get(stmdev_ctx_t *ctx,
                                    lsm6dsv16b_stpcnt_mode_t *val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  lsm6dsv16b_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }
  val->false_step_rej = pedo_cmd_reg.fp_rejection_en;
  val->step_counter_enable = emb_func_en_a.pedo_en;

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
int32_t lsm6dsv16b_stpcnt_steps_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_STEP_COUNTER_L, &buff[0], 2);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  *val = buff[1];
  *val = (*val * 256U) + buff[0];

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
int32_t lsm6dsv16b_stpcnt_rst_step_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_emb_func_src_t emb_func_src;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  if (ret == 0)
  {
    emb_func_src.pedo_rst_step = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_stpcnt_rst_step_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_emb_func_src_t emb_func_src;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  *val = emb_func_src.pedo_rst_step;

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_stpcnt_debounce_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf, 1);
  if (ret == 0)
  {
    pedo_deb_steps_conf.deb_step = val;
    ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf, 1);
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
int32_t lsm6dsv16b_stpcnt_debounce_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf, 1);
  *val = pedo_deb_steps_conf.deb_step;

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
int32_t lsm6dsv16b_stpcnt_period_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));

  ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_PEDO_SC_DELTAT_L, (uint8_t *)&buff[0], 2);

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
int32_t lsm6dsv16b_stpcnt_period_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_PEDO_SC_DELTAT_L, &buff[0], 2);
  *val = buff[1];
  *val = (*val * 256U) + buff[0];

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
int32_t lsm6dsv16b_sigmot_mode_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.sign_motion_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_sigmot_mode_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  *val = emb_func_en_a.sign_motion_en;

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_tilt_mode_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  if (ret == 0)
  {
    emb_func_en_a.tilt_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_tilt_mode_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  *val = emb_func_en_a.tilt_en;

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_sflp_game_rotation_set(stmdev_ctx_t *ctx, uint16_t val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
    emb_func_en_a.sflp_game_en = val;
    ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A,
                                 (uint8_t *)&emb_func_en_a, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_sflp_game_rotation_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  lsm6dsv16b_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
    *val = emb_func_en_a.sflp_game_en;
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_sflp_data_rate_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_sflp_data_rate_t val)
{
  lsm6dsv16b_sflp_odr_t sflp_odr;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_SFLP_ODR, (uint8_t *)&sflp_odr, 1);
    sflp_odr.sflp_game_odr = (uint8_t)val & 0x07U;
    ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_SFLP_ODR, (uint8_t *)&sflp_odr,
                                 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_sflp_data_rate_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_sflp_data_rate_t *val)
{
  lsm6dsv16b_sflp_odr_t sflp_odr;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_SFLP_ODR, (uint8_t *)&sflp_odr, 1);
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  switch (sflp_odr.sflp_game_odr)
  {
    case LSM6DSV16B_SFLP_15Hz:
      *val = LSM6DSV16B_SFLP_15Hz;
      break;

    case LSM6DSV16B_SFLP_30Hz:
      *val = LSM6DSV16B_SFLP_30Hz;
      break;

    case LSM6DSV16B_SFLP_60Hz:
      *val = LSM6DSV16B_SFLP_60Hz;
      break;

    case LSM6DSV16B_SFLP_120Hz:
      *val = LSM6DSV16B_SFLP_120Hz;
      break;

    case LSM6DSV16B_SFLP_240Hz:
      *val = LSM6DSV16B_SFLP_240Hz;
      break;

    case LSM6DSV16B_SFLP_480Hz:
      *val = LSM6DSV16B_SFLP_480Hz;
      break;

    default:
      *val = LSM6DSV16B_SFLP_15Hz;
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
int32_t lsm6dsv16b_sflp_game_gbias_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_sflp_gbias_t *val)
{
  lsm6dsv16b_sflp_data_rate_t sflp_odr;
  lsm6dsv16b_emb_func_exec_status_t emb_func_sts;
  lsm6dsv16b_data_ready_t drdy;
  lsm6dsv16b_xl_full_scale_t xl_fs;
  lsm6dsv16b_ctrl10_t ctrl10;
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

  ret = lsm6dsv16b_sflp_data_rate_get(ctx, &sflp_odr);
  if (ret != 0)
  {
    return ret;
  }

  /* Calculate k factor */
  switch (sflp_odr)
  {
    case LSM6DSV16B_SFLP_15Hz:
      k = 0.04f;
      break;
    case LSM6DSV16B_SFLP_30Hz:
      k = 0.02f;
      break;
    case LSM6DSV16B_SFLP_60Hz:
      k = 0.01f;
      break;
    case LSM6DSV16B_SFLP_120Hz:
      k = 0.005f;
      break;
    case LSM6DSV16B_SFLP_240Hz:
      k = 0.0025f;
      break;
    case LSM6DSV16B_SFLP_480Hz:
      k = 0.00125f;
      break;
  }

  /* compute gbias as half precision float in order to be put in embedded advanced feature register */
  gbias_hf[0] = npy_float_to_half(val->gbias_x * (3.14159265358979323846f / 180.0f) / k);
  gbias_hf[1] = npy_float_to_half(val->gbias_y * (3.14159265358979323846f / 180.0f) / k);
  gbias_hf[2] = npy_float_to_half(val->gbias_z * (3.14159265358979323846f / 180.0f) / k);

  /* Save sensor configuration and set high-performance mode (if the sensor is in power-down mode, turn it on) */
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL1, conf_saved, 2);
  ret += lsm6dsv16b_xl_mode_set(ctx, LSM6DSV16B_XL_HIGH_PERFORMANCE_MD);
  ret += lsm6dsv16b_gy_mode_set(ctx, LSM6DSV16B_GY_HIGH_PERFORMANCE_MD);
  if ((conf_saved[0] & 0x0FU) == LSM6DSV16B_XL_ODR_OFF)
  {
    ret += lsm6dsv16b_xl_data_rate_set(ctx, LSM6DSV16B_XL_ODR_AT_120Hz);
  }

  /* disable algos */
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, emb_func_en_saved,
                              2);
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, reg_zero, 2);
  do
  {
    ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EXEC_STATUS,
                                (uint8_t *)&emb_func_sts, 1);
  } while (emb_func_sts.emb_func_endop != 1);
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  // enable gbias setting
  ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);
  ctrl10.emb_func_debug = 1;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  /* enable algos */
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  emb_func_en_saved[0] |= 0x02; /* force SFLP GAME en */
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_A, emb_func_en_saved,
                               2);
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  ret += lsm6dsv16b_xl_full_scale_get(ctx, &xl_fs);

  /* Read XL data */
  do
  {
    ret += lsm6dsv16b_flag_data_ready_get(ctx, &drdy);
  } while (drdy.drdy_xl != 1);
  ret += lsm6dsv16b_acceleration_raw_get(ctx, xl_data);

  /* force sflp initialization */
  master_config = 0x40;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, &master_config,
                               1);
  for (i = 0; i < 3; i++)
  {
    j = 0;
    data_tmp = (int32_t)xl_data[i];
    data_tmp <<= xl_fs; // shift based on current fs
    ret += lsm6dsv16b_write_reg(ctx, 0x02 + 3 * i, &data_ptr[j++], 1);
    ret += lsm6dsv16b_write_reg(ctx, 0x03 + 3 * i, &data_ptr[j++], 1);
    ret += lsm6dsv16b_write_reg(ctx, 0x04 + 3 * i, &data_ptr[j], 1);
  }
  for (i = 0; i < 3; i++)
  {
    j = 0;
    data_tmp = 0;
    ret += lsm6dsv16b_write_reg(ctx, 0x0B + 3 * i, &data_ptr[j++], 1);
    ret += lsm6dsv16b_write_reg(ctx, 0x0C + 3 * i, &data_ptr[j++], 1);
    ret += lsm6dsv16b_write_reg(ctx, 0x0D + 3 * i, &data_ptr[j], 1);
  }
  master_config = 0x00;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, &master_config,
                               1);

  // wait end_op (and at least 30 us)
  ctx->mdelay(1);
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  do
  {
    ret += lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EXEC_STATUS,
                                (uint8_t *)&emb_func_sts, 1);
  } while (emb_func_sts.emb_func_endop != 1);
  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  /* write gbias in embedded advanced features registers */
  ret += lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_SFLP_GAME_GBIASX_L,
                                 (uint8_t *)gbias_hf, 6);

  /* reload previous sensor configuration */
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL1, conf_saved, 2);

  // disable gbias setting
  ctrl10.emb_func_debug = 0;
  ret += lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL10, (uint8_t *)&ctrl10, 1);

  return ret;
}

/**
  * @brief  SFLP initial configuration [set]
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_sflp_configure(stmdev_ctx_t *ctx)
{
  uint8_t val = 0x50;
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_write(ctx, 0xD2, &val, 1);

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
int32_t lsm6dsv16b_fsm_permission_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fsm_permission_t val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret == 0)
  {
    func_cfg_access.fsm_wr_ctrl_en = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
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
int32_t lsm6dsv16b_fsm_permission_status_get(stmdev_ctx_t *ctx,
                                              lsm6dsv16b_fsm_permission_status_t *val)
{
  lsm6dsv16b_ctrl_status_t status;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL_STATUS, (uint8_t *)&status, 1);
  *val = (status.fsm_wr_ctrl_status == 0) ? LSM6DSV16B_STD_IF_CONTROL : LSM6DSV16B_FSM_CONTROL;

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
int32_t lsm6dsv16b_fsm_permission_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fsm_permission_t *val)
{
  lsm6dsv16b_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  switch (func_cfg_access.fsm_wr_ctrl_en)
  {
    case LSM6DSV16B_PROTECT_CTRL_REGS:
      *val = LSM6DSV16B_PROTECT_CTRL_REGS;
      break;

    case LSM6DSV16B_WRITE_CTRL_REG:
      *val = LSM6DSV16B_WRITE_CTRL_REG;
      break;

    default:
      *val = LSM6DSV16B_PROTECT_CTRL_REGS;
      break;
  }
  return ret;
}

/**
  * @brief  Get the FSM permission status
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: All reg writable from std if - 1: some regs are under FSM control.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_fsm_permission_status(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl_status_t ctrl_status;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL_STATUS, (uint8_t *)&ctrl_status, 1);

  *val = ctrl_status.fsm_wr_ctrl_status;

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
int32_t lsm6dsv16b_fsm_mode_set(stmdev_ctx_t *ctx, lsm6dsv16b_fsm_mode_t val)
{
  lsm6dsv16b_emb_func_en_b_t emb_func_en_b;
  lsm6dsv16b_fsm_enable_t fsm_enable;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_mode_get(stmdev_ctx_t *ctx, lsm6dsv16b_fsm_mode_t *val)
{
  lsm6dsv16b_fsm_enable_t fsm_enable;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_ENABLE, (uint8_t *)&fsm_enable, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_long_cnt_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FSM_LONG_COUNTER_L, (uint8_t *)&buff[0], 2);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_long_cnt_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_LONG_COUNTER_L, &buff[0], 2);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_out_get(stmdev_ctx_t *ctx, lsm6dsv16b_fsm_out_t *val)
{
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_OUTS1, (uint8_t *)val, 8);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_data_rate_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fsm_data_rate_t val)
{
  lsm6dsv16b_fsm_odr_t fsm_odr;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  if (ret == 0)
  {
    fsm_odr.fsm_odr = (uint8_t)val & 0x07U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

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
int32_t lsm6dsv16b_fsm_data_rate_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fsm_data_rate_t *val)
{
  lsm6dsv16b_fsm_odr_t fsm_odr;
  int32_t ret;

  ret = lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_FSM_ODR, (uint8_t *)&fsm_odr, 1);
  }

  ret += lsm6dsv16b_mem_bank_set(ctx, LSM6DSV16B_MAIN_MEM_BANK);

  switch (fsm_odr.fsm_odr)
  {
    case LSM6DSV16B_FSM_15Hz:
      *val = LSM6DSV16B_FSM_15Hz;
      break;

    case LSM6DSV16B_FSM_30Hz:
      *val = LSM6DSV16B_FSM_30Hz;
      break;

    case LSM6DSV16B_FSM_60Hz:
      *val = LSM6DSV16B_FSM_60Hz;
      break;

    case LSM6DSV16B_FSM_120Hz:
      *val = LSM6DSV16B_FSM_120Hz;
      break;

    case LSM6DSV16B_FSM_240Hz:
      *val = LSM6DSV16B_FSM_240Hz;
      break;

    case LSM6DSV16B_FSM_480Hz:
      *val = LSM6DSV16B_FSM_480Hz;
      break;

    case LSM6DSV16B_FSM_960Hz:
      *val = LSM6DSV16B_FSM_960Hz;
      break;

    default:
      *val = LSM6DSV16B_FSM_15Hz;
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
int32_t lsm6dsv16b_fsm_long_cnt_timeout_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_FSM_LC_TIMEOUT_L, (uint8_t *)&buff[0], 2);

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
int32_t lsm6dsv16b_fsm_long_cnt_timeout_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_FSM_LC_TIMEOUT_L, &buff[0], 2);
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
int32_t lsm6dsv16b_fsm_number_of_programs_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_fsm_programs_t fsm_programs;
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
  if (ret == 0)
  {
    fsm_programs.fsm_n_prog = val;
    ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
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
int32_t lsm6dsv16b_fsm_number_of_programs_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_fsm_programs_t fsm_programs;
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_FSM_PROGRAMS, (uint8_t *)&fsm_programs, 1);
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
int32_t lsm6dsv16b_fsm_start_address_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = lsm6dsv16b_ln_pg_write(ctx, LSM6DSV16B_FSM_START_ADD_L, (uint8_t *)&buff[0], 2);

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
int32_t lsm6dsv16b_fsm_start_address_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dsv16b_ln_pg_read(ctx, LSM6DSV16B_FSM_START_ADD_L, &buff[0], 2);
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
int32_t lsm6dsv16b_xl_offset_on_out_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ctrl9.usr_off_on_out = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_xl_offset_on_out_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_xl_offset_mg_set(stmdev_ctx_t *ctx,
                                     lsm6dsv16bxl_offset_mg_t val)
{
  lsm6dsv16b_z_ofs_usr_t z_ofs_usr;
  lsm6dsv16b_y_ofs_usr_t y_ofs_usr;
  lsm6dsv16b_x_ofs_usr_t x_ofs_usr;
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;
  float_t tmp;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);
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
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
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
int32_t lsm6dsv16b_xl_offset_mg_get(stmdev_ctx_t *ctx,
                                     lsm6dsv16bxl_offset_mg_t *val)
{
  lsm6dsv16b_z_ofs_usr_t z_ofs_usr;
  lsm6dsv16b_y_ofs_usr_t y_ofs_usr;
  lsm6dsv16b_x_ofs_usr_t x_ofs_usr;
  lsm6dsv16b_ctrl9_t ctrl9;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_CTRL9, (uint8_t *)&ctrl9, 1);
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_Z_OFS_USR, (uint8_t *)&z_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_Y_OFS_USR, (uint8_t *)&y_ofs_usr, 1);
  }
  if (ret == 0)
  {
    ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_X_OFS_USR, (uint8_t *)&x_ofs_usr, 1);
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
int32_t lsm6dsv16b_i3c_reset_mode_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_i3c_reset_mode_t val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret == 0)
  {
    pin_ctrl.ibhr_por_en = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
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
int32_t lsm6dsv16b_i3c_reset_mode_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_i3c_reset_mode_t *val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  switch (pin_ctrl.ibhr_por_en)
  {
    case LSM6DSV16B_SW_RST_DYN_ADDRESS_RST:
      *val = LSM6DSV16B_SW_RST_DYN_ADDRESS_RST;
      break;

    case LSM6DSV16B_I3C_GLOBAL_RST:
      *val = LSM6DSV16B_I3C_GLOBAL_RST;
      break;

    default:
      *val = LSM6DSV16B_SW_RST_DYN_ADDRESS_RST;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @addtogroup  Time-Division Multiplexing (TDM)
  * @brief   This section group all the functions concerning the
  *          usage of Time-Division Multiplexing (TDM)
  * @{
  *
  */

/**
  * @brief  Disables pull-up on WCLK pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Disables pull-up on WCLK pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_dis_wclk_pull_up_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  if (ret == 0)
  {
    pin_ctrl.tdm_wclk_pu_dis = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  Disables pull-up on WCLK pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Disables pull-up on WCLK pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_dis_wclk_pull_up_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  *val = pin_ctrl.tdm_wclk_pu_dis;

  return ret;
}

/**
  * @brief  Enables pull-up on TDMout pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on TDMout pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_tdmout_pull_up_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret == 0)
  {
    if_cfg.tdm_out_pu_en = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enables pull-up on TDMout pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables pull-up on TDMout pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_tdmout_pull_up_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_if_cfg_t if_cfg;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_IF_CFG, (uint8_t *)&if_cfg, 1);
  *val = if_cfg.tdm_out_pu_en;

  return ret;
}

/**
  * @brief  WCLK and BCLK frequencies.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      WCLK_8kHZ_1024kHz, WCLK_16kHZ_2048kHz, WCLK_8kHZ_2048kHz, WCLK_16kHZ_1024kHz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_wclk_bclk_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_wclk_bclk_t val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  if (ret == 0)
  {
    tdm_cfg0.tdm_wclk_bclk_sel = ((uint8_t)val & 0x4U) >> 2;
    tdm_cfg0.tdm_wclk = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  WCLK and BCLK frequencies.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      WCLK_8kHZ_1024kHz, WCLK_16kHZ_2048kHz, WCLK_8kHZ_2048kHz, WCLK_16kHZ_1024kHz,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_wclk_bclk_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_wclk_bclk_t *val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  switch ((tdm_cfg0.tdm_wclk_bclk_sel << 2) + tdm_cfg0.tdm_wclk)
  {
    case LSM6DSV16B_WCLK_16kHZ_BCLK_2048kHz:
      *val = LSM6DSV16B_WCLK_16kHZ_BCLK_2048kHz;
      break;

    case LSM6DSV16B_WCLK_8kHZ_BCLK_2048kHz:
      *val = LSM6DSV16B_WCLK_8kHZ_BCLK_2048kHz;
      break;

    default:
      *val = LSM6DSV16B_WCLK_8kHZ_BCLK_2048kHz;
      break;
  }
  return ret;
}

/**
  * @brief  Selection of TDM slot for transmission.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLOT_012, SLOT_456,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_slot_set(stmdev_ctx_t *ctx, lsm6dsv16b_tdm_slot_t val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  if (ret == 0)
  {
    tdm_cfg0.tdm_slot_sel = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  Selection of TDM slot for transmission.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLOT_012, SLOT_456,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_slot_get(stmdev_ctx_t *ctx, lsm6dsv16b_tdm_slot_t *val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  switch (tdm_cfg0.tdm_slot_sel)
  {
    case LSM6DSV16B_SLOT_012:
      *val = LSM6DSV16B_SLOT_012;
      break;

    case LSM6DSV16B_SLOT_456:
      *val = LSM6DSV16B_SLOT_456;
      break;

    default:
      *val = LSM6DSV16B_SLOT_012;
      break;
  }
  return ret;
}

/**
  * @brief  BCLK edge selection for TDM interface.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BCLK_RISING, BCLK_FALLING,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_bclk_edge_set(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_bclk_edge_t val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  if (ret == 0)
  {
    tdm_cfg0.tdm_bclk_edge_sel = (uint8_t)val & 0x01U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  BCLK edge selection for TDM interface.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BCLK_RISING, BCLK_FALLING,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_bclk_edge_get(stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_bclk_edge_t *val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  switch (tdm_cfg0.tdm_bclk_edge_sel)
  {
    case LSM6DSV16B_BCLK_RISING:
      *val = LSM6DSV16B_BCLK_RISING;
      break;

    case LSM6DSV16B_BCLK_FALLING:
      *val = LSM6DSV16B_BCLK_FALLING;
      break;

    default:
      *val = LSM6DSV16B_BCLK_RISING;
      break;
  }
  return ret;
}

/**
  * @brief  Enables TDM delayed configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables TDM delayed configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_delayed_conf_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  if (ret == 0)
  {
    tdm_cfg0.tdm_delayed_cfg = val;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  }

  return ret;
}

/**
  * @brief  Enables TDM delayed configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables TDM delayed configuration.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_delayed_conf_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dsv16b_tdm_cfg0_t tdm_cfg0;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG0, (uint8_t *)&tdm_cfg0, 1);
  *val = tdm_cfg0.tdm_delayed_cfg;

  return ret;
}


/**
  * @brief  Selects order of transmission of TDM axes.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TDM_ORDER_ZYX, TDM_ORDER_XZY, TDM_ORDER_XYZ,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_axis_order_set(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_tdm_axis_order_t val)
{
  lsm6dsv16b_tdm_cfg1_t tdm_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  if (ret == 0)
  {
    tdm_cfg1.tdm_axes_ord_sel = (uint8_t)val & 0x03U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  }

  return ret;
}

/**
  * @brief  Selects order of transmission of TDM axes.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TDM_ORDER_ZYX, TDM_ORDER_XZY, TDM_ORDER_XYZ,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_axis_order_get(stmdev_ctx_t *ctx,
                                       lsm6dsv16b_tdm_axis_order_t *val)
{
  lsm6dsv16b_tdm_cfg1_t tdm_cfg1;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG1, (uint8_t *)&tdm_cfg1, 1);
  switch (tdm_cfg1.tdm_axes_ord_sel)
  {
    case LSM6DSV16B_TDM_ORDER_ZYX:
      *val = LSM6DSV16B_TDM_ORDER_ZYX;
      break;

    case LSM6DSV16B_TDM_ORDER_XZY:
      *val = LSM6DSV16B_TDM_ORDER_XZY;
      break;

    case LSM6DSV16B_TDM_ORDER_XYZ:
      *val = LSM6DSV16B_TDM_ORDER_XYZ;
      break;

    default:
      *val = LSM6DSV16B_TDM_ORDER_ZYX;
      break;
  }
  return ret;
}

/**
  * @brief  TDM channel accelerometer full-scale selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TDM_2g, TDM_4g, TDM_8g,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_xl_full_scale_set(stmdev_ctx_t *ctx,
                                          lsm6dsv16b_tdm_xl_full_scale_t val)
{
  lsm6dsv16b_tdm_cfg2_t tdm_cfg2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
  if (ret == 0)
  {
    tdm_cfg2.tdm_fs_xl = (uint8_t)val & 0x3U;
    ret = lsm6dsv16b_write_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
  }

  return ret;
}

/**
  * @brief  TDM channel accelerometer full-scale selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      TDM_2g, TDM_4g, TDM_8g,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dsv16b_tdm_xl_full_scale_get(stmdev_ctx_t *ctx,
                                          lsm6dsv16b_tdm_xl_full_scale_t *val)
{
  lsm6dsv16b_tdm_cfg2_t tdm_cfg2;
  int32_t ret;

  ret = lsm6dsv16b_read_reg(ctx, LSM6DSV16B_TDM_CFG2, (uint8_t *)&tdm_cfg2, 1);
  switch (tdm_cfg2.tdm_fs_xl)
  {
    case LSM6DSV16B_TDM_2g:
      *val = LSM6DSV16B_TDM_2g;
      break;

    case LSM6DSV16B_TDM_4g:
      *val = LSM6DSV16B_TDM_4g;
      break;

    case LSM6DSV16B_TDM_8g:
      *val = LSM6DSV16B_TDM_8g;
      break;

    default:
      *val = LSM6DSV16B_TDM_2g;
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
