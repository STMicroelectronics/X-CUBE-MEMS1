/**
  ******************************************************************************
  * @file    lsm6dso16is_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LSM6DSO16IS driver file
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

#include "lsm6dso16is_reg.h"

/**
  * @defgroup  LSM6DSO16IS
  * @brief     This file provides a set of functions needed to drive the
  *            lsm6dso16is enhanced inertial module.
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
int32_t __weak lsm6dso16is_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
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
int32_t __weak lsm6dso16is_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
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
  * @defgroup    LSM6DSO16IS_Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lsm6dso16is_from_fs2g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 0.061f);
}

float_t lsm6dso16is_from_fs4g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 0.122f);
}

float_t lsm6dso16is_from_fs8g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 0.244f);
}

float_t lsm6dso16is_from_fs16g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 0.488f);
}

float_t lsm6dso16is_from_fs125dps_to_mdps(int16_t lsb)
{
  return ((float_t)lsb * 4.375f);
}

float_t lsm6dso16is_from_fs250dps_to_mdps(int16_t lsb)
{
  return ((float_t)lsb * 8.75f);
}

float_t lsm6dso16is_from_fs500dps_to_mdps(int16_t lsb)
{
  return ((float_t)lsb * 17.50f);
}

float_t lsm6dso16is_from_fs1000dps_to_mdps(int16_t lsb)
{
  return ((float_t)lsb * 35.0f);
}

float_t lsm6dso16is_from_fs2000dps_to_mdps(int16_t lsb)
{
  return ((float_t)lsb * 70.0f);
}

float_t lsm6dso16is_from_lsb_to_celsius(int16_t lsb)
{
  return (((float_t)lsb / 256.0f) + 25.0f);
}

/**
  * @defgroup Common
  * @brief    Common
  * @{/
  *
  */

/**
  * @brief  Difference in percentage of the effective ODR (and timestamp rate)
  *         with respect to the typical.[set]
  *         Step:  0.15%. 8-bit format, 2's complement.
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of freq_fine in reg INTERNAL_FREQ_FINE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_odr_cal_reg_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_internal_freq_fine_t internal_freq_fine;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INTERNAL_FREQ_FINE,
                             (uint8_t *)&internal_freq_fine, 1);

  if (ret == 0)
  {
    internal_freq_fine.freq_fine = (uint8_t)val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_INTERNAL_FREQ_FINE,
                                (uint8_t *)&internal_freq_fine, 1);
  }

  return ret;
}

/**
  * @brief  Difference in percentage of the effective ODR (and timestamp rate)
  *         with respect to the typical.[get]
  *         Step:  0.15%. 8-bit format, 2's complement.
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of freq_fine in reg INTERNAL_FREQ_FINE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_odr_cal_reg_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_internal_freq_fine_t internal_freq_fine;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INTERNAL_FREQ_FINE,
                             (uint8_t *)&internal_freq_fine, 1);
  *val = internal_freq_fine.freq_fine;

  return ret;
}

/**
  * @brief  Change memory bank.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, EMBED_FUNC_MEM_BANK, SENSOR_HUB_MEM_BANK, ISPU_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_mem_bank_set(const stmdev_ctx_t *ctx, lsm6dso16is_mem_bank_t val)
{
  lsm6dso16is_func_cfg_access_t func_cfg_access = {0x0};
  int32_t ret;

  /* no need to read it first as the only other field is a ispu reset bit */
  func_cfg_access.shub_reg_access = (val == LSM6DSO16IS_SENSOR_HUB_MEM_BANK) ? 0x1U : 0x0U;
  func_cfg_access.ispu_reg_access = (val == LSM6DSO16IS_ISPU_MEM_BANK) ? 0x1U : 0x0U;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  return ret;
}

/**
  * @brief  Change memory bank.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, EMBED_FUNC_MEM_BANK, SENSOR_HUB_MEM_BANK, ISPU_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_mem_bank_get(const stmdev_ctx_t *ctx, lsm6dso16is_mem_bank_t *val)
{
  lsm6dso16is_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  if (func_cfg_access.shub_reg_access == 1U)
  {
    *val = LSM6DSO16IS_SENSOR_HUB_MEM_BANK;
  }
  else if (func_cfg_access.ispu_reg_access == 1U)
  {
    *val = LSM6DSO16IS_ISPU_MEM_BANK;
  }
  else
  {
    *val = LSM6DSO16IS_MAIN_MEM_BANK;
  }

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
int32_t lsm6dso16is_data_ready_mode_set(const stmdev_ctx_t *ctx,
                                        lsm6dso16is_data_ready_mode_t val)
{
  lsm6dso16is_drdy_pulsed_reg_t drdy_pulsed_reg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_DRDY_PULSED_REG, (uint8_t *)&drdy_pulsed_reg, 1);

  if (ret == 0)
  {
    drdy_pulsed_reg.drdy_pulsed = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_DRDY_PULSED_REG, (uint8_t *)&drdy_pulsed_reg, 1);
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
int32_t lsm6dso16is_data_ready_mode_get(const stmdev_ctx_t *ctx,
                                        lsm6dso16is_data_ready_mode_t *val)
{
  lsm6dso16is_drdy_pulsed_reg_t drdy_pulsed_reg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_DRDY_PULSED_REG, (uint8_t *)&drdy_pulsed_reg, 1);

  switch ((drdy_pulsed_reg.drdy_pulsed))
  {
    case LSM6DSO16IS_DRDY_LATCHED:
      *val = LSM6DSO16IS_DRDY_LATCHED;
      break;

    case LSM6DSO16IS_DRDY_PULSED:
      *val = LSM6DSO16IS_DRDY_PULSED;
      break;

    default:
      *val = LSM6DSO16IS_DRDY_LATCHED;
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
int32_t lsm6dso16is_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_WHO_AM_I, (uint8_t *)val, 1);

  return ret;
}

/**
  * @brief  Software reset. Restore the default values in user registers.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_software_reset(const stmdev_ctx_t *ctx)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ret += lsm6dso16is_xl_data_rate_set(ctx, LSM6DSO16IS_XL_ODR_OFF);
    ret += lsm6dso16is_gy_data_rate_set(ctx, LSM6DSO16IS_GY_ODR_OFF);

    ctrl3_c.sw_reset = PROPERTY_ENABLE;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

    do
    {
      ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
    } while (ret == 0 && ctrl3_c.sw_reset == PROPERTY_ENABLE);
  }

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of boot in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_boot_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.boot = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
  }

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Get the values of boot in reg CTRL_REG1.(ptr)
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_boot_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
  *val = ctrl3_c.boot;

  return ret;
}

/**
  * @brief  Enable disable high-performance mode[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HIGH_PERFOMANCE_MODE_ENABLED, HIGH_PERFOMANCE_MODE_DISABLED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_xl_hm_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t val)
{
  lsm6dso16is_ctrl6_c_t ctrl6_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL6_C, (uint8_t *)&ctrl6_c, 1);

  if (ret == 0)
  {
    ctrl6_c.xl_hm_mode = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL6_C, (uint8_t *)&ctrl6_c, 1);
  }

  return ret;
}

/**
  * @brief  Enable disable high-performance mode[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HIGH_PERFOMANCE_MODE_ENABLED, HIGH_PERFOMANCE_MODE_DISABLED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_xl_hm_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t *val)
{
  lsm6dso16is_ctrl6_c_t ctrl6_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL6_C, (uint8_t *)&ctrl6_c, 1);

  switch ((ctrl6_c.xl_hm_mode))
  {
    case LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED;
      break;

    case LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED;
      break;

    default:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED;
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
int32_t lsm6dso16is_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_xl_full_scale_t val)
{
  lsm6dso16is_ctrl1_xl_t ctrl1_xl;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);

  if (ret == 0)
  {
    ctrl1_xl.fs_xl = ((uint8_t)val & 0x3U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);
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
int32_t lsm6dso16is_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_xl_full_scale_t *val)
{
  lsm6dso16is_ctrl1_xl_t ctrl1_xl;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);

  switch ((ctrl1_xl.fs_xl))
  {
    case LSM6DSO16IS_2g:
      *val = LSM6DSO16IS_2g;
      break;

    case LSM6DSO16IS_4g:
      *val = LSM6DSO16IS_4g;
      break;

    case LSM6DSO16IS_8g:
      *val = LSM6DSO16IS_8g;
      break;

    case LSM6DSO16IS_16g:
      *val = LSM6DSO16IS_16g;
      break;

    default:
      *val = LSM6DSO16IS_2g;
      break;
  }
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
int32_t lsm6dso16is_xl_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_data_rate_t val)
{
  lsm6dso16is_ctrl1_xl_t ctrl1_xl;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);

  if (ret == 0)
  {
    if (((uint8_t)val & 0x10U) == 0x10U)
    {
      ret += lsm6dso16is_xl_hm_mode_set(ctx, LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED);
    }
    else
    {
      ret += lsm6dso16is_xl_hm_mode_set(ctx, LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED);
    }

    ctrl1_xl.odr_xl = ((uint8_t)val & 0xfU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);
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
int32_t lsm6dso16is_xl_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_data_rate_t *val)
{
  lsm6dso16is_ctrl1_xl_t ctrl1_xl;
  lsm6dso16is_ctrl6_c_t ctrl6_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL1_XL, (uint8_t *)&ctrl1_xl, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL6_C, (uint8_t *)&ctrl6_c, 1);

  switch ((ctrl6_c.xl_hm_mode << 4) | (ctrl1_xl.odr_xl))
  {
    case LSM6DSO16IS_XL_ODR_OFF:
      *val = LSM6DSO16IS_XL_ODR_OFF;
      break;

    case LSM6DSO16IS_XL_ODR_AT_12Hz5_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_12Hz5_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_26H_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_26H_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_52Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_52Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_104Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_104Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_208Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_208Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_416Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_416Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_833Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_833Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_1667Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_1667Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_3333Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_3333Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_6667Hz_HP:
      *val = LSM6DSO16IS_XL_ODR_AT_6667Hz_HP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_12Hz5_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_12Hz5_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_26H_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_26H_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_52Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_52Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_104Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_104Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_208Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_208Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_416Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_416Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_833Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_833Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_1667Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_1667Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_3333Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_3333Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_6667Hz_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_6667Hz_LP;
      break;

    case LSM6DSO16IS_XL_ODR_AT_1Hz6_LP:
      *val = LSM6DSO16IS_XL_ODR_AT_1Hz6_LP;
      break;

    default:
      *val = LSM6DSO16IS_XL_ODR_OFF;
      break;
  }

  return ret;
}

/**
  * @brief  Enable disable high-performance mode[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HIGH_PERFOMANCE_MODE_ENABLED, HIGH_PERFOMANCE_MODE_DISABLED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_gy_hm_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t val)
{
  lsm6dso16is_ctrl7_g_t ctrl7_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL7_G, (uint8_t *)&ctrl7_g, 1);

  if (ret == 0)
  {
    ctrl7_g.g_hm_mode = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL7_G, (uint8_t *)&ctrl7_g, 1);
  }

  return ret;
}

/**
  * @brief  Enable disable high-performance mode[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      HIGH_PERFOMANCE_MODE_ENABLED, HIGH_PERFOMANCE_MODE_DISABLED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_gy_hm_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t *val)
{
  lsm6dso16is_ctrl7_g_t ctrl7_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL7_G, (uint8_t *)&ctrl7_g, 1);

  switch ((ctrl7_g.g_hm_mode))
  {
    case LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED;
      break;

    case LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED;
      break;

    default:
      *val = LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED;
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
int32_t lsm6dso16is_gy_full_scale_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_gy_full_scale_t val)
{
  lsm6dso16is_ctrl2_g_t ctrl2_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);

  if (ret == 0)
  {
    ctrl2_g.fs_g = ((uint8_t)val & 0x3U);
    ctrl2_g.fs_125 = ((uint8_t)val >> 4);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);
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
int32_t lsm6dso16is_gy_full_scale_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_gy_full_scale_t *val)
{
  lsm6dso16is_ctrl2_g_t ctrl2_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);

  switch ((ctrl2_g.fs_125 << 4) | (ctrl2_g.fs_g))
  {
    case LSM6DSO16IS_125dps:
      *val = LSM6DSO16IS_125dps;
      break;

    case LSM6DSO16IS_250dps:
      *val = LSM6DSO16IS_250dps;
      break;

    case LSM6DSO16IS_500dps:
      *val = LSM6DSO16IS_500dps;
      break;

    case LSM6DSO16IS_1000dps:
      *val = LSM6DSO16IS_1000dps;
      break;

    case LSM6DSO16IS_2000dps:
      *val = LSM6DSO16IS_2000dps;
      break;

    default:
      *val = LSM6DSO16IS_125dps;
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
int32_t lsm6dso16is_gy_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_data_rate_t val)
{
  lsm6dso16is_ctrl2_g_t ctrl2_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);

  if (ret == 0)
  {
    if (((uint8_t)val & 0x10U) == 0x10U)
    {
      ret += lsm6dso16is_gy_hm_mode_set(ctx, LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED);
    }
    else
    {
      ret += lsm6dso16is_gy_hm_mode_set(ctx, LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED);
    }

    ctrl2_g.odr_g = ((uint8_t)val & 0xfU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);
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
int32_t lsm6dso16is_gy_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_data_rate_t *val)
{
  lsm6dso16is_ctrl2_g_t ctrl2_g;
  lsm6dso16is_ctrl7_g_t ctrl7_g;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL2_G, (uint8_t *)&ctrl2_g, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL7_G, (uint8_t *)&ctrl7_g, 1);

  switch ((ctrl7_g.g_hm_mode << 4) | (ctrl2_g.odr_g))
  {
    case LSM6DSO16IS_GY_ODR_OFF:
      *val = LSM6DSO16IS_GY_ODR_OFF;
      break;

    case LSM6DSO16IS_GY_ODR_AT_12Hz5_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_12Hz5_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_26H_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_26H_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_52Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_52Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_104Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_104Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_208Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_208Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_416Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_416Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_833Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_833Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_1667Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_1667Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_3333Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_3333Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_6667Hz_HP:
      *val = LSM6DSO16IS_GY_ODR_AT_6667Hz_HP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_12Hz5_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_12Hz5_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_26H_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_26H_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_52Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_52Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_104Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_104Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_208Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_208Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_416Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_416Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_833Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_833Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_1667Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_1667Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_3333Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_3333Hz_LP;
      break;

    case LSM6DSO16IS_GY_ODR_AT_6667Hz_LP:
      *val = LSM6DSO16IS_GY_ODR_AT_6667Hz_LP;
      break;

    default:
      *val = LSM6DSO16IS_GY_ODR_OFF;
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
int32_t lsm6dso16is_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.if_inc = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
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
int32_t lsm6dso16is_auto_increment_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  *val = ctrl3_c.if_inc;

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
int32_t lsm6dso16is_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.bdu = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
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
int32_t lsm6dso16is_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  *val = ctrl3_c.bdu;

  return ret;
}

/**
  * @brief  Enables gyroscope sleep mode[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLEEP_G_ENABLE, SLEEP_G_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sleep_set(const stmdev_ctx_t *ctx, lsm6dso16is_sleep_t val)
{
  lsm6dso16is_ctrl4_c_t ctrl4_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);

  if (ret == 0)
  {
    ctrl4_c.sleep_g = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);
  }

  return ret;
}

/**
  * @brief  Enables gyroscope sleep mode[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLEEP_G_ENABLE, SLEEP_G_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sleep_get(const stmdev_ctx_t *ctx, lsm6dso16is_sleep_t *val)
{
  lsm6dso16is_ctrl4_c_t ctrl4_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);

  switch ((ctrl4_c.sleep_g))
  {
    case LSM6DSO16IS_SLEEP_G_ENABLE:
      *val = LSM6DSO16IS_SLEEP_G_ENABLE;
      break;

    case LSM6DSO16IS_SLEEP_G_DISABLE:
      *val = LSM6DSO16IS_SLEEP_G_DISABLE;
      break;

    default:
      *val = LSM6DSO16IS_SLEEP_G_ENABLE;
      break;
  }
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
int32_t lsm6dso16is_xl_self_test_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_self_test_t val)
{
  lsm6dso16is_ctrl5_c_t ctrl5_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);

  if (ret == 0)
  {
    ctrl5_c.st_xl = ((uint8_t)val & 0x3U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);
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
int32_t lsm6dso16is_xl_self_test_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_self_test_t *val)
{
  lsm6dso16is_ctrl5_c_t ctrl5_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);

  switch ((ctrl5_c.st_xl))
  {
    case LSM6DSO16IS_XL_ST_DISABLE:
      *val = LSM6DSO16IS_XL_ST_DISABLE;
      break;

    case LSM6DSO16IS_XL_ST_POSITIVE:
      *val = LSM6DSO16IS_XL_ST_POSITIVE;
      break;

    case LSM6DSO16IS_XL_ST_NEGATIVE:
      *val = LSM6DSO16IS_XL_ST_NEGATIVE;
      break;

    default:
      *val = LSM6DSO16IS_XL_ST_DISABLE;
      break;
  }
  return ret;
}

/**
  * @brief  Gyroscope self-test selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_ST_DISABLE, GY_ST_POSITIVE, GY_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_gy_self_test_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_self_test_t val)
{
  lsm6dso16is_ctrl5_c_t ctrl5_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);

  if (ret == 0)
  {
    ctrl5_c.st_g = ((uint8_t)val & 0x3U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);
  }

  return ret;
}

/**
  * @brief  Gyroscope self-test selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      GY_ST_DISABLE, GY_ST_POSITIVE, GY_ST_NEGATIVE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_gy_self_test_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_self_test_t *val)
{
  lsm6dso16is_ctrl5_c_t ctrl5_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL5_C, (uint8_t *)&ctrl5_c, 1);

  switch ((ctrl5_c.st_g))
  {
    case LSM6DSO16IS_GY_ST_DISABLE:
      *val = LSM6DSO16IS_GY_ST_DISABLE;
      break;

    case LSM6DSO16IS_GY_ST_POSITIVE:
      *val = LSM6DSO16IS_GY_ST_POSITIVE;
      break;

    case LSM6DSO16IS_GY_ST_NEGATIVE:
      *val = LSM6DSO16IS_GY_ST_NEGATIVE;
      break;

    default:
      *val = LSM6DSO16IS_GY_ST_DISABLE;
      break;
  }
  return ret;
}

/**
  * @defgroup Serial Interfaces
  * @brief    Serial Interfaces
  * @{/
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
int32_t lsm6dso16is_ui_sdo_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  if (ret == 0)
  {
    pin_ctrl.sdo_pu_en = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
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
int32_t lsm6dso16is_ui_sdo_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  *val = pin_ctrl.sdo_pu_en;

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
int32_t lsm6dso16is_spi_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_spi_mode_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.sim = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
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
int32_t lsm6dso16is_spi_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_spi_mode_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  switch ((ctrl3_c.sim))
  {
    case LSM6DSO16IS_SPI_4_WIRE:
      *val = LSM6DSO16IS_SPI_4_WIRE;
      break;

    case LSM6DSO16IS_SPI_3_WIRE:
      *val = LSM6DSO16IS_SPI_3_WIRE;
      break;

    default:
      *val = LSM6DSO16IS_SPI_4_WIRE;
      break;
  }
  return ret;
}

/**
  * @brief  Disables I2C on UI (User Interface).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      I2C_ENABLE, I2C_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ui_i2c_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_ui_i2c_mode_t val)
{
  lsm6dso16is_ctrl4_c_t ctrl4_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);

  if (ret == 0)
  {
    ctrl4_c.i2c_disable = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);
  }

  return ret;
}

/**
  * @brief  Disables I2C on UI (User Interface).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      I2C_ENABLE, I2C_DISABLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ui_i2c_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_ui_i2c_mode_t *val)
{
  lsm6dso16is_ctrl4_c_t ctrl4_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL4_C, (uint8_t *)&ctrl4_c, 1);

  switch ((ctrl4_c.i2c_disable))
  {
    case LSM6DSO16IS_I2C_ENABLE:
      *val = LSM6DSO16IS_I2C_ENABLE;
      break;

    case LSM6DSO16IS_I2C_DISABLE:
      *val = LSM6DSO16IS_I2C_DISABLE;
      break;

    default:
      *val = LSM6DSO16IS_I2C_ENABLE;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup Timestamp
  * @brief    Timestamp
  * @{/
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
int32_t lsm6dso16is_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_ctrl10_c_t ctrl10_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);

  if (ret == 0)
  {
    ctrl10_c.timestamp_en = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);
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
int32_t lsm6dso16is_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_ctrl10_c_t ctrl10_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);

  *val = ctrl10_c.timestamp_en;

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
int32_t lsm6dso16is_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_TIMESTAMP0, &buff[0], 4);

  *val = (uint32_t)buff[3];
  *val = (*val * 256U) + (uint32_t)buff[2];
  *val = (*val * 256U) + (uint32_t)buff[1];
  *val = (*val * 256U) + (uint32_t)buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @brief  Get the status of all the interrupt sources.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the status of all the interrupt sources.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_all_sources_get(const stmdev_ctx_t *ctx, lsm6dso16is_all_sources_t *val)
{
  lsm6dso16is_status_reg_t status_reg;
  lsm6dso16is_status_master_mainpage_t status_sh;
  uint32_t status_ispu;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_REG, (uint8_t *)&status_reg, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->drdy_xl = status_reg.xlda;
  val->drdy_gy = status_reg.gda;
  val->drdy_temp = status_reg.tda;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_MASTER_MAINPAGE, (uint8_t *)&status_sh, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->sh_endop = status_sh.sens_hub_endop;
  val->sh_slave0_nack = status_sh.sens_hub_endop;
  val->sh_slave1_nack = status_sh.sens_hub_endop;
  val->sh_slave2_nack = status_sh.sens_hub_endop;
  val->sh_slave3_nack = status_sh.sens_hub_endop;
  val->sh_wr_once = status_sh.sens_hub_endop;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_INT_STATUS0_MAINPAGE, (uint8_t *)&status_ispu, 4);
  if (ret != 0)
  {
    return ret;
  }

  val->ispu = status_ispu;

  return ret;
}

/**
  * @brief  The STATUS_REG register is read by the primary interface.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Get register STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_status_reg_get(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_status_reg_t *val)
{
  int32_t ret;
  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_REG, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Accelerometer new data available.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of xlda in reg STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_xl_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val)
{
  lsm6dso16is_status_reg_t status_reg;
  int32_t ret;
  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_REG,
                             (uint8_t *)&status_reg, 1);
  *val = status_reg.xlda;

  return ret;
}

/**
  * @brief  Gyroscope new data available.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of gda in reg STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_gy_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val)
{
  lsm6dso16is_status_reg_t status_reg;
  int32_t ret;
  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_REG,
                             (uint8_t *)&status_reg, 1);
  *val = status_reg.gda;

  return ret;
}

/**
  * @brief  Temperature new data available.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of tda in reg STATUS_REG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_temp_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                             uint8_t *val)
{
  lsm6dso16is_status_reg_t status_reg;
  int32_t ret;
  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_REG,
                             (uint8_t *)&status_reg, 1);
  *val = status_reg.tda;

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
int32_t lsm6dso16is_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_OUT_TEMP_L, &buff[0], 2);
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
int32_t lsm6dso16is_angular_rate_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_OUTX_L_G, buff, 6);
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
int32_t lsm6dso16is_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_OUTX_L_A, buff, 6);
  val[0] = (int16_t)buff[1];
  val[0] = (val[0] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[2] = (int16_t)buff[5];
  val[2] = (val[2] * 256) + (int16_t)buff[4];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup Interrupt PINs
  * @brief    Interrupt PINs
  * @{/
  *
  */
/**
  * @brief  It routes interrupt signals on INT 1 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      It routes interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int1_route_t val)
{
  lsm6dso16is_int1_ctrl_t int1_ctrl;
  lsm6dso16is_md1_cfg_t md1_cfg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  int1_ctrl.int1_drdy_xl = val.drdy_xl;
  int1_ctrl.int1_drdy_g = val.drdy_gy;
  int1_ctrl.int1_boot = val.boot;
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_INT1_CTRL, (uint8_t *)&int1_ctrl,
                               1);

  md1_cfg.int1_shub = val.sh_endop;
  md1_cfg.int1_ispu = val.ispu;
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MD1_CFG, (uint8_t *)&md1_cfg, 1);

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
int32_t lsm6dso16is_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int1_route_t *val)
{
  lsm6dso16is_int1_ctrl_t int1_ctrl;
  lsm6dso16is_md1_cfg_t md1_cfg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INT1_CTRL, (uint8_t *)&int1_ctrl, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->drdy_xl = int1_ctrl.int1_drdy_xl;
  val->drdy_gy = int1_ctrl.int1_drdy_g;
  val->boot = int1_ctrl.int1_boot;
  val->sh_endop = md1_cfg.int1_shub;
  val->ispu = md1_cfg.int1_ispu;

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
int32_t lsm6dso16is_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int2_route_t val)
{
  lsm6dso16is_int2_ctrl_t int2_ctrl;
  lsm6dso16is_md2_cfg_t md2_cfg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  int2_ctrl.int2_drdy_xl = val.drdy_xl;
  int2_ctrl.int2_drdy_g = val.drdy_gy;
  int2_ctrl.int2_drdy_temp = val.drdy_temp;
  int2_ctrl.int2_sleep_ispu = val.ispu_sleep;
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);

  md2_cfg.int2_ispu = val.ispu;
  md2_cfg.int2_timestamp = val.timestamp;
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MD2_CFG, (uint8_t *)&md2_cfg, 1);

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
int32_t lsm6dso16is_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int2_route_t *val)
{
  lsm6dso16is_int2_ctrl_t int2_ctrl;
  lsm6dso16is_md2_cfg_t md2_cfg;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_INT2_CTRL, (uint8_t *)&int2_ctrl, 1);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->drdy_xl = int2_ctrl.int2_drdy_xl;
  val->drdy_gy = int2_ctrl.int2_drdy_g;
  val->drdy_temp = int2_ctrl.int2_drdy_temp;
  val->ispu_sleep = int2_ctrl.int2_sleep_ispu;
  val->ispu = md2_cfg.int2_ispu;
  val->timestamp = md2_cfg.int2_timestamp;

  return ret;
}

/**
  * @brief  Push-pull/open-drain selection on INT1 and INT2 pins.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      PUSH_PULL, OPEN_DRAIN,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_int_pin_mode_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.pp_od = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
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
int32_t lsm6dso16is_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_int_pin_mode_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  switch ((ctrl3_c.pp_od))
  {
    case LSM6DSO16IS_PUSH_PULL:
      *val = LSM6DSO16IS_PUSH_PULL;
      break;

    case LSM6DSO16IS_OPEN_DRAIN:
      *val = LSM6DSO16IS_OPEN_DRAIN;
      break;

    default:
      *val = LSM6DSO16IS_PUSH_PULL;
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
int32_t lsm6dso16is_pin_polarity_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_pin_polarity_t val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  if (ret == 0)
  {
    ctrl3_c.h_lactive = ((uint8_t)val & 0x1U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);
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
int32_t lsm6dso16is_pin_polarity_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_pin_polarity_t *val)
{
  lsm6dso16is_ctrl3_c_t ctrl3_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL3_C, (uint8_t *)&ctrl3_c, 1);

  switch ((ctrl3_c.h_lactive))
  {
    case LSM6DSO16IS_ACTIVE_HIGH:
      *val = LSM6DSO16IS_ACTIVE_HIGH;
      break;

    case LSM6DSO16IS_ACTIVE_LOW:
      *val = LSM6DSO16IS_ACTIVE_LOW;
      break;

    default:
      *val = LSM6DSO16IS_ACTIVE_HIGH;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Sensor hub
  * @brief     This section groups all the functions that manage the
  *            sensor hub.
  * @{
  *
  */

/**
  * @brief  Sensor hub output registers.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensor hub output registers.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_read_data_raw_get(const stmdev_ctx_t *ctx, uint8_t *val,
                                         uint8_t len)
{
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_SENSOR_HUB_1, val, len);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Number of external sensors to be read by the sensor hub.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLV_0, SLV_0_1, SLV_0_1_2, SLV_0_1_2_3,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_slave_connected_set(const stmdev_ctx_t *ctx,
                                           lsm6dso16is_sh_slave_connected_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.aux_sens_on = (uint8_t)val & 0x3U;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Number of external sensors to be read by the sensor hub.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SLV_0, SLV_0_1, SLV_0_1_2, SLV_0_1_2_3,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_slave_connected_get(const stmdev_ctx_t *ctx,
                                           lsm6dso16is_sh_slave_connected_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch (master_config.aux_sens_on)
  {
    case LSM6DSO16IS_SLV_0:
      *val = LSM6DSO16IS_SLV_0;
      break;

    case LSM6DSO16IS_SLV_0_1:
      *val = LSM6DSO16IS_SLV_0_1;
      break;

    case LSM6DSO16IS_SLV_0_1_2:
      *val = LSM6DSO16IS_SLV_0_1_2;
      break;

    case LSM6DSO16IS_SLV_0_1_2_3:
      *val = LSM6DSO16IS_SLV_0_1_2_3;
      break;

    default:
      *val = LSM6DSO16IS_SLV_0;
      break;
  }

  return ret;
}

/**
  * @brief  Sensor hub I2C master enable.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensor hub I2C master enable.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_master_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.master_on = val;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor hub I2C master enable.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensor hub I2C master enable.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_master_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = master_config.master_on;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor Hub master I2C pull-up enable.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensor Hub master I2C pull-up enable.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_master_interface_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.shub_pu_en = val;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor Hub master I2C pull-up enable.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Sensor Hub master I2C pull-up enable.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_master_interface_pull_up_get(const stmdev_ctx_t *ctx,
                                                    uint8_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = master_config.shub_pu_en;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  I2C interface pass-through.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of pass_through_mode in reg MASTER_CONFIG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_sh_pass_through_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.pass_through_mode = (uint8_t)val;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  I2C interface pass-through.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of pass_through_mode in reg MASTER_CONFIG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lsm6dso16is_sh_pass_through_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

  *val = master_config.pass_through_mode;
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor hub trigger signal selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SH_TRG_XL_GY_DRDY, SH_TRIG_INT2,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_syncro_mode_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_sh_syncro_mode_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.start_config = (uint8_t)val & 0x01U;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor hub trigger signal selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SH_TRG_XL_GY_DRDY, SH_TRIG_INT2,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_syncro_mode_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_sh_syncro_mode_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch (master_config.start_config)
  {
    case LSM6DSO16IS_SH_TRG_XL_GY_DRDY:
      *val = LSM6DSO16IS_SH_TRG_XL_GY_DRDY;
      break;

    case LSM6DSO16IS_SH_TRIG_INT2:
      *val = LSM6DSO16IS_SH_TRIG_INT2;
      break;

    default:
      *val = LSM6DSO16IS_SH_TRG_XL_GY_DRDY;
      break;
  }

  return ret;
}

/**
  * @brief  Slave 0 write operation is performed only at the first sensor hub cycle.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      EACH_SH_CYCLE, ONLY_FIRST_CYCLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_write_mode_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_sh_write_mode_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.write_once = (uint8_t)val & 0x01U;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Slave 0 write operation is performed only at the first sensor hub cycle.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      EACH_SH_CYCLE, ONLY_FIRST_CYCLE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_write_mode_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_sh_write_mode_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch (master_config.write_once)
  {
    case LSM6DSO16IS_EACH_SH_CYCLE:
      *val = LSM6DSO16IS_EACH_SH_CYCLE;
      break;

    case LSM6DSO16IS_ONLY_FIRST_CYCLE:
      *val = LSM6DSO16IS_ONLY_FIRST_CYCLE;
      break;

    default:
      *val = LSM6DSO16IS_EACH_SH_CYCLE;
      break;
  }

  return ret;
}

/**
  * @brief  Reset Master logic and output registers. Must be set to ‘1’ and then set it to ‘0’.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Reset Master logic and output registers. Must be set to ‘1’ and then set it to ‘0’.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_reset_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  master_config.rst_master_regs = val;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Reset Master logic and output registers. Must be set to ‘1’ and then set it to ‘0’.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Reset Master logic and output registers. Must be set to ‘1’ and then set it to ‘0’.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_reset_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_master_config_t master_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_MASTER_CONFIG, (uint8_t *)&master_config, 1);
  if (ret != 0)
  {
    return ret;
  }

  *val = master_config.rst_master_regs;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Configure slave 0 for perform a write.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      a structure that contain
  *                      - uint8_t slv1_add;    8 bit i2c device address
  *                      - uint8_t slv1_subadd; 8 bit register device address
  *                      - uint8_t slv1_data;   8 bit data to write
  * @retval             interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_cfg_write(const stmdev_ctx_t *ctx,
                                 lsm6dso16is_sh_cfg_write_t *val)
{
  lsm6dso16is_slv0_add_t reg;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  reg.slave0_add = val->slv0_add;
  reg.rw_0 = 0;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_ADD, (uint8_t *)&reg, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_SUBADD,
                              &(val->slv0_subadd), 1);
  if (ret != 0)
  {
    goto exit;
  }

  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_DATAWRITE_SLV0,
                              &(val->slv0_data), 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Rate at which the master communicates.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SH_12_5Hz, SH_26Hz, SH_52Hz, SH_104Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_sh_data_rate_t val)
{
  lsm6dso16is_slv0_config_t slv0_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_SLV0_CONFIG, (uint8_t *)&slv0_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  slv0_config.shub_odr = (uint8_t)val & 0x07U;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_CONFIG, (uint8_t *)&slv0_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Rate at which the master communicates.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SH_12_5Hz, SH_26Hz, SH_52Hz, SH_104Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_sh_data_rate_t *val)
{
  lsm6dso16is_slv0_config_t slv0_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_SLV0_CONFIG, (uint8_t *)&slv0_config, 1);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch (slv0_config.shub_odr)
  {
    case LSM6DSO16IS_SH_12_5Hz:
      *val = LSM6DSO16IS_SH_12_5Hz;
      break;

    case LSM6DSO16IS_SH_26Hz:
      *val = LSM6DSO16IS_SH_26Hz;
      break;

    case LSM6DSO16IS_SH_52Hz:
      *val = LSM6DSO16IS_SH_52Hz;
      break;

    case LSM6DSO16IS_SH_104Hz:
      *val = LSM6DSO16IS_SH_104Hz;
      break;

    default:
      *val = LSM6DSO16IS_SH_12_5Hz;
      break;
  }

  return ret;
}

/**
  * @brief  Configure slave idx for perform a read.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Structure that contain
  *                      - uint8_t slv_add;    8 bit i2c device address
  *                      - uint8_t slv_subadd; 8 bit register device address
  *                      - uint8_t slv_len;    num of bit to read
  * @retval             interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_slv_cfg_read(const stmdev_ctx_t *ctx, uint8_t idx,
                                    lsm6dso16is_sh_cfg_read_t *val)
{
  lsm6dso16is_slv0_add_t slv_add;
  lsm6dso16is_slv0_config_t slv_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_SENSOR_HUB_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  slv_add.slave0_add = val->slv_add;
  slv_add.rw_0 = 1;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_ADD + idx * 3U,
                              (uint8_t *)&slv_add, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_SUBADD + idx * 3U,
                              &(val->slv_subadd), 1);
  if (ret != 0)
  {
    goto exit;
  }

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_SLV0_CONFIG + idx * 3U,
                             (uint8_t *)&slv_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  slv_config.slave0_numop = val->slv_len;
  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_SLV0_CONFIG + idx * 3U,
                              (uint8_t *)&slv_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Sensor hub source register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      union of registers from STATUS_MASTER to
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_sh_status_get(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_status_master_t *val)
{
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_STATUS_MASTER_MAINPAGE, (uint8_t *) val, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup ispu
  * @brief    ispu
  * @{/
  *
  */
/**
  * @brief  Software reset of ISPU core.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Software reset of ISPU core.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_reset_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lsm6dso16is_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  if (ret == 0)
  {
    func_cfg_access.sw_reset_ispu = val;
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  return ret;
}

/**
  * @brief  Software reset of ISPU core.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Software reset of ISPU core.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_reset_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lsm6dso16is_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  *val = func_cfg_access.sw_reset_ispu;


  return ret;
}

int32_t lsm6dso16is_ispu_clock_set(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_ispu_clock_sel_t val)
{
  lsm6dso16is_ctrl10_c_t ctrl10_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);

  if (ret == 0)
  {
    ctrl10_c.ispu_clk_sel = (uint8_t)val;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);
  }

  return ret;
}

int32_t lsm6dso16is_ispu_clock_get(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_ispu_clock_sel_t *val)
{
  lsm6dso16is_ctrl10_c_t ctrl10_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL10_C, (uint8_t *)&ctrl10_c, 1);

  switch (ctrl10_c.ispu_clk_sel)
  {
    default:
    case 0:
      *val = LSM6DSO16IS_ISPU_CLK_5MHz;
      break;
    case 1:
      *val = LSM6DSO16IS_ISPU_CLK_10MHz;
      break;
  }

  return ret;
}

/**
  * @brief  ISPU irq rate selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_ODR_OFF, ISPU_ODR_AT_12Hz5, ISPU_ODR_AT_26Hz, ISPU_ODR_AT_52Hz,
  *                  ISPU_ODR_AT_104Hz, ISPU_ODR_AT_208Hz, ISPU_ODR_AT_417Hz, ISPU_ODR_AT_833Hz,
  *                  ISPU_ODR_AT_1667Hz, ISPU_ODR_AT_3333Hz, ISPU_ODR_AT_6667Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_data_rate_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_ispu_data_rate_t val)
{
  lsm6dso16is_ctrl9_c_t ctrl9_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);

  if (ret == 0)
  {
    ctrl9_c.ispu_rate = ((uint8_t)val & 0xfU);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);
  }

  return ret;
}

/**
  * @brief  ISPU irq rate selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_ODR_OFF, ISPU_ODR_AT_12Hz5, ISPU_ODR_AT_26Hz, ISPU_ODR_AT_52Hz,
  *                  ISPU_ODR_AT_104Hz, ISPU_ODR_AT_208Hz, ISPU_ODR_AT_417Hz, ISPU_ODR_AT_833Hz,
  *                  ISPU_ODR_AT_1667Hz, ISPU_ODR_AT_3333Hz, ISPU_ODR_AT_6667Hz
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_data_rate_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_ispu_data_rate_t *val)
{
  lsm6dso16is_ctrl9_c_t ctrl9_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);

  switch ((ctrl9_c.ispu_rate))
  {
    case LSM6DSO16IS_ISPU_ODR_OFF:
      *val = LSM6DSO16IS_ISPU_ODR_OFF;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_12Hz5:
      *val = LSM6DSO16IS_ISPU_ODR_AT_12Hz5;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_26Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_26Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_52Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_52Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_104Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_104Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_208Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_208Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_416Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_416Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_833Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_833Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_1667Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_1667Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_3333Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_3333Hz;
      break;

    case LSM6DSO16IS_ISPU_ODR_AT_6667Hz:
      *val = LSM6DSO16IS_ISPU_ODR_AT_6667Hz;
      break;

    default:
      *val = LSM6DSO16IS_ISPU_ODR_OFF;
      break;
  }
  return ret;
}

/**
  * @brief  ISPU bdu selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_BDU_OFF, ISPU_BDU_ON_2B_4B, ISPU_BDU_ON_2B_2B, ISPU_BDU_ON_4B_4B,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_bdu_set(const stmdev_ctx_t *ctx, lsm6dso16is_ispu_bdu_t val)
{
  lsm6dso16is_ctrl9_c_t ctrl9_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);

  if (ret == 0)
  {
    ctrl9_c.ispu_bdu = ((uint8_t)val & 0x3U);
    ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);
  }

  return ret;
}

/**
  * @brief  ISPU bdu selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_BDU_OFF, ISPU_BDU_ON_2B_4B, ISPU_BDU_ON_2B_2B, ISPU_BDU_ON_4B_4B,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_bdu_get(const stmdev_ctx_t *ctx, lsm6dso16is_ispu_bdu_t *val)
{
  lsm6dso16is_ctrl9_c_t ctrl9_c;
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_CTRL9_C, (uint8_t *)&ctrl9_c, 1);

  switch ((ctrl9_c.ispu_rate))
  {
    case LSM6DSO16IS_ISPU_BDU_OFF:
      *val = LSM6DSO16IS_ISPU_BDU_OFF;
      break;

    case LSM6DSO16IS_ISPU_BDU_ON_2B_4B:
      *val = LSM6DSO16IS_ISPU_BDU_ON_2B_4B;
      break;

    case LSM6DSO16IS_ISPU_BDU_ON_2B_2B:
      *val = LSM6DSO16IS_ISPU_BDU_ON_2B_2B;
      break;

    case LSM6DSO16IS_ISPU_BDU_ON_4B_4B:
      *val = LSM6DSO16IS_ISPU_BDU_ON_4B_4B;
      break;

    default:
      *val = LSM6DSO16IS_ISPU_BDU_OFF;
      break;
  }
  return ret;
}

/**
  * @brief  Generic Interrupt Flags from ISPU.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Generic Interrupt Flags from ISPU.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ia_ispu_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_INT_STATUS0_MAINPAGE, &buff[0], 4);

  *val = (uint32_t)buff[3];
  *val = (*val * 256U) + (uint32_t)buff[2];
  *val = (*val * 256U) + (uint32_t)buff[1];
  *val = (*val * 256U) + (uint32_t)buff[0];

  return ret;
}

/**
  * @brief  General purpose input configuration register for ISPU[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  offset   offset from ISPU_DUMMY_CFG_1 register
  * @param  val      General purpose input configuration register for ISPU
  * @param  len      number of bytes to write
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_write_dummy_cfg(const stmdev_ctx_t *ctx, uint8_t offset,
                                         uint8_t *val, uint8_t len)
{
  int32_t ret;

  /* check if we are writing outside of the range */
  if (LSM6DSO16IS_ISPU_DUMMY_CFG_1_L + offset + len > LSM6DSO16IS_ISPU_DUMMY_CFG_4_H)
  {
    return -1;
  }

  ret = lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_DUMMY_CFG_1_L + offset, val, len);

  return ret;
}

/**
  * @brief  General purpose input configuration register for ISPU[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  offset   offset from ISPU_DUMMY_CFG_1 register
  * @param  val      General purpose input configuration register for ISPU
  * @param  len      number of bytes to write
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_read_dummy_cfg(const stmdev_ctx_t *ctx, uint8_t offset,
                                        uint8_t *val, uint8_t len)
{
  int32_t ret;

  /* check if we are reading outside of the range */
  if (LSM6DSO16IS_ISPU_DUMMY_CFG_1_L + offset + len > LSM6DSO16IS_ISPU_DUMMY_CFG_4_H)
  {
    return -1;
  }

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_DUMMY_CFG_1_L + offset, val, len);

  return ret;
}

/**
  * @brief  Boot ISPU core[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Boot ISPU core
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_boot_set(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_ispu_boot_latched_t val)
{
  lsm6dso16is_ispu_config_t ispu_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ispu_config.ispu_rst_n = (uint8_t)val;
  ispu_config.clk_dis = (uint8_t)val;
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Boot ISPU core[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Boot ISPU core
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_boot_get(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_ispu_boot_latched_t *val)
{
  lsm6dso16is_ispu_config_t ispu_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  *val = LSM6DSO16IS_ISPU_TURN_OFF;
  if (ispu_config.ispu_rst_n == 1U || ispu_config.clk_dis == 1U)
  {
    *val = LSM6DSO16IS_ISPU_TURN_ON;
  }

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables latched ISPU interrupt.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_INT_PULSED, ISPU_INT_LATCHED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int_latched_set(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_int_latched_t val)
{
  lsm6dso16is_ispu_config_t ispu_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ispu_config.latched = ((uint8_t)val & 0x1U);
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);

exit:
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables latched ISPU interrupt.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU_INT_PULSED, ISPU_INT_LATCHED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int_latched_get(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_int_latched_t *val)
{
  lsm6dso16is_ispu_config_t ispu_config;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch ((ispu_config.latched))
  {
    case LSM6DSO16IS_ISPU_INT_PULSED:
      *val = LSM6DSO16IS_ISPU_INT_PULSED;
      break;

    case LSM6DSO16IS_ISPU_INT_LATCHED:
      *val = LSM6DSO16IS_ISPU_INT_LATCHED;
      break;

    default:
      *val = LSM6DSO16IS_ISPU_INT_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  returns ISPU boot status
  *
  * @param  ctx      read / write interface definitions
  * @param  val      LSM6DSO16IS_ISPU_BOOT_IN_PROGRESS, LSM6DSO16IS_ISPU_BOOT_ENDED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_get_boot_status(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_boot_end_t *val)
{
  lsm6dso16is_ispu_status_t ispu_boot_status;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_STATUS, (uint8_t *)&ispu_boot_status, 1);
  *val = (lsm6dso16is_ispu_boot_end_t)ispu_boot_status.boot_end;
  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

static int32_t lsm6dso16is_ispu_sel_memory_addr(const stmdev_ctx_t *ctx, uint16_t mem_addr)
{
  uint8_t mem_addr_l, mem_addr_h;
  int32_t ret = 0;

  mem_addr_l = (uint8_t)(mem_addr & 0xFFU);
  mem_addr_h = (uint8_t)(mem_addr / 256U);
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_ADDR1,
                               (uint8_t *)&mem_addr_h, 1);
  ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_ADDR0,
                               (uint8_t *)&mem_addr_l, 1);

  return ret;
}

/**
  * @brief  ISPU write memory. ISPU clock is disabled inside the routine.
  *
  * @param  ctx      read / write interface definitions
  * @param  mem_sel  LSM6DSO16IS_ISPU_DATA_RAM_MEMORY, LSM6DSO16IS_ISPU_PROGRAM_RAM_MEMORY
  * @param  mem_addr memory address
  * @param  mem_data memory data
  * @param  len      data length
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_write_memory(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_ispu_memory_type_t mem_sel,
                                      uint16_t mem_addr, uint8_t *mem_data, uint16_t len)
{
  lsm6dso16is_ispu_mem_sel_t ispu_mem_sel;
  lsm6dso16is_ispu_config_t ispu_cfg;
  uint8_t clk_dis;
  int32_t ret;
  uint16_t i;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* disable ISPU clock */
    ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    clk_dis = ispu_cfg.clk_dis;
    ispu_cfg.clk_dis = 1;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);

    /* select memory to be written */
    ispu_mem_sel.read_mem_en = 0;
    ispu_mem_sel.mem_sel = (uint8_t)mem_sel;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_SEL, (uint8_t *)&ispu_mem_sel, 1);

    if (mem_sel == LSM6DSO16IS_ISPU_PROGRAM_RAM_MEMORY)
    {
      uint16_t addr_s[4] = {(uint16_t)0, (uint16_t)0, (uint16_t)0, (uint16_t)0};
      uint16_t len_s[4] = {(uint16_t)0, (uint16_t)0, (uint16_t)0, (uint16_t)0};
      uint8_t j = 0;
      uint16_t k;

      addr_s[0] = mem_addr;
      k = 0U;
      for (i = 0U; i < len; i++)
      {
        if ((mem_addr + i == 0x2000U) || (mem_addr + i == 0x4000U) || (mem_addr + i == 0x6000U))
        {
          len_s[j++] = k;
          addr_s[j] = mem_addr + i;
          k = 0U;
        }

        k++;
      }
      len_s[j++] = k;

      k = 0U;
      for (i = 0U; i < j; i++)
      {
        ret += lsm6dso16is_ispu_sel_memory_addr(ctx, addr_s[i]);
        ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_DATA, &mem_data[k], len_s[i]);
        k += len_s[i];
      }
    }
    else
    {
      /* select memory address */
      ret += lsm6dso16is_ispu_sel_memory_addr(ctx, mem_addr);
      ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_DATA, &mem_data[0], len);
    }

    /* set ISPU clock back to previous value */
    ispu_cfg.clk_dis = clk_dis;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU read memory. ISPU clock is disabled inside the routine.
  *
  * @param  ctx      read / write interface definitions
  * @param  mem_sel  LSM6DSO16IS_ISPU_DATA_RAM_MEMORY, LSM6DSO16IS_ISPU_PROGRAM_RAM_MEMORY
  * @param  mem_addr memory address
  * @param  mem_data memory data
  * @param  len      data length
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_read_memory(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_ispu_memory_type_t mem_sel,
                                     uint16_t mem_addr, uint8_t *mem_data, uint16_t len)
{
  lsm6dso16is_ispu_mem_sel_t ispu_mem_sel;
  lsm6dso16is_ispu_config_t ispu_cfg;
  uint8_t clk_dis;
  int32_t ret;
  uint8_t dummy;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* disable ISPU clock */
    ret = lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    clk_dis = ispu_cfg.clk_dis;
    ispu_cfg.clk_dis = 1;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);

    /* select memory to be read */
    ispu_mem_sel.read_mem_en = 1;
    ispu_mem_sel.mem_sel = (uint8_t)mem_sel;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_MEM_SEL, (uint8_t *)&ispu_mem_sel, 1);

    /* select memory address */
    ret += lsm6dso16is_ispu_sel_memory_addr(ctx, mem_addr);

    /* read data */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_MEM_DATA, &dummy, 1);
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_MEM_DATA, &mem_data[0], len);

    /* set ISPU clock back to previous value */
    ispu_cfg.clk_dis = clk_dis;
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU write flags (IF2S)
  *
  * @param  ctx      read / write interface definitions
  * @param  data     ISPU flags
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_write_flags(const stmdev_ctx_t *ctx, uint16_t data)
{
  lsm6dso16is_ispu_if2s_flag_l_t flag_l;
  lsm6dso16is_ispu_if2s_flag_h_t flag_h;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the flags */
    flag_h.if2s = (uint8_t)(data / 256U);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_IF2S_FLAG_H, (uint8_t *)&flag_h,
                                 1);
    flag_l.if2s = (uint8_t)(data & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_IF2S_FLAG_L, (uint8_t *)&flag_l,
                                 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU read flags (S2IF)
  *
  * @param  ctx      read / write interface definitions
  * @param  data     ISPU flags
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_read_flags(const stmdev_ctx_t *ctx, uint16_t *data)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read the flags */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_S2IF_FLAG_L, buff, 2);
    data[0] = (uint16_t)buff[1];
    data[0] = (data[0] * 256U) + (uint16_t)buff[0];
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU clear flags (S2IF)
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_clear_flags(const stmdev_ctx_t *ctx)
{
  uint8_t data = 1;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);

  if (ret == 0)
  {
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_S2IF_FLAG_H, &data, 1);
    ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);
  }

  return ret;
}

/**
  * @brief  ISPU DOUT registers.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU DOUT output registers.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_read_data_raw_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val,
                                           uint8_t len)
{
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_DOUT_00_L, (uint8_t *) val,
                                len);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int1_ctrl.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int1_ctrl register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int1_ctrl_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int1_ctrl reg */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_INT1_CTRL0, &buff[0], 4);

    *val = (uint32_t)buff[3];
    *val = (*val * 256U) + (uint32_t)buff[2];
    *val = (*val * 256U) + (uint32_t)buff[1];
    *val = (*val * 256U) + (uint32_t)buff[0];
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int1_ctrl.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int1_ctrl register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int1_ctrl_set(const stmdev_ctx_t *ctx, uint32_t val)
{
  lsm6dso16is_ispu_int1_ctrl0_t int1_ctrl0;
  lsm6dso16is_ispu_int1_ctrl1_t int1_ctrl1;
  lsm6dso16is_ispu_int1_ctrl2_t int1_ctrl2;
  lsm6dso16is_ispu_int1_ctrl3_t int1_ctrl3;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the int1_ctrl reg */
    int1_ctrl3.ispu_int1_ctrl = (uint8_t)((val >> 24) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT1_CTRL3, (uint8_t *)&int1_ctrl3,
                                 1);

    int1_ctrl2.ispu_int1_ctrl = (uint8_t)((val >> 16) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT1_CTRL2, (uint8_t *)&int1_ctrl2,
                                 1);

    int1_ctrl1.ispu_int1_ctrl = (uint8_t)((val >>  8) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT1_CTRL1, (uint8_t *)&int1_ctrl1,
                                 1);

    int1_ctrl0.ispu_int1_ctrl = (uint8_t)(val & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT1_CTRL0, (uint8_t *)&int1_ctrl0,
                                 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int2_ctrl.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int2_ctrl register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int2_ctrl_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int2_ctrl reg */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_INT2_CTRL0, &buff[0], 4);

    *val = (uint32_t)buff[3];
    *val = (*val * 256U) + (uint32_t)buff[2];
    *val = (*val * 256U) + (uint32_t)buff[1];
    *val = (*val * 256U) + (uint32_t)buff[0];
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int2_ctrl.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int2_ctrl register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int2_ctrl_set(const stmdev_ctx_t *ctx, uint32_t val)
{
  lsm6dso16is_ispu_int2_ctrl0_t int2_ctrl0;
  lsm6dso16is_ispu_int2_ctrl1_t int2_ctrl1;
  lsm6dso16is_ispu_int2_ctrl2_t int2_ctrl2;
  lsm6dso16is_ispu_int2_ctrl3_t int2_ctrl3;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the int2_ctrl reg */
    int2_ctrl3.ispu_int2_ctrl = (uint8_t)((val >> 24) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT2_CTRL3, (uint8_t *)&int2_ctrl3,
                                 1);

    int2_ctrl2.ispu_int2_ctrl = (uint8_t)((val >> 16) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT2_CTRL2, (uint8_t *)&int2_ctrl2,
                                 1);

    int2_ctrl1.ispu_int2_ctrl = (uint8_t)((val >>  8) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT2_CTRL1, (uint8_t *)&int2_ctrl1,
                                 1);

    int2_ctrl0.ispu_int2_ctrl = (uint8_t)(val & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_INT2_CTRL0, (uint8_t *)&int2_ctrl0,
                                 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int_status.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int2_status register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_int_status_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int2_ctrl reg */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_INT_STATUS0, &buff[0], 4);

    *val = (uint32_t)buff[3];
    *val = (*val * 256U) + (uint32_t)buff[2];
    *val = (*val * 256U) + (uint32_t)buff[1];
    *val = (*val * 256U) + (uint32_t)buff[0];
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU algo.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU algo register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_algo_get(const stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int2_ctrl reg */
    ret += lsm6dso16is_read_reg(ctx, LSM6DSO16IS_ISPU_ALGO0, &buff[0], 4);

    *val = (uint32_t)buff[3];
    *val = (*val * 256U) + (uint32_t)buff[2];
    *val = (*val * 256U) + (uint32_t)buff[1];
    *val = (*val * 256U) + (uint32_t)buff[0];
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU algo.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU algo register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lsm6dso16is_ispu_algo_set(const stmdev_ctx_t *ctx, uint32_t val)
{
  lsm6dso16is_ispu_algo0_t algo0;
  lsm6dso16is_ispu_algo1_t algo1;
  lsm6dso16is_ispu_algo2_t algo2;
  lsm6dso16is_ispu_algo3_t algo3;
  int32_t ret;

  ret = lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the algo reg */
    algo3.ispu_algo = (uint8_t)((val >> 24) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_ALGO3, (uint8_t *)&algo3, 1);

    algo2.ispu_algo = (uint8_t)((val >> 16) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_ALGO2, (uint8_t *)&algo2, 1);

    algo1.ispu_algo = (uint8_t)((val >>  8) & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_ALGO1, (uint8_t *)&algo1, 1);

    algo0.ispu_algo = (uint8_t)(val & 0xffU);
    ret += lsm6dso16is_write_reg(ctx, LSM6DSO16IS_ISPU_ALGO0, (uint8_t *)&algo0, 1);
  }

  ret += lsm6dso16is_mem_bank_set(ctx, LSM6DSO16IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */
