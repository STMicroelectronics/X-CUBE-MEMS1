/*
 ******************************************************************************
 * @file    iis3dwb10is_reg.c
 * @author  Sensors Software Solution Team
 * @brief   IIS3DWB10IS driver file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "iis3dwb10is_reg.h"

/**
  * @defgroup    IIS3DWB10IS
  * @brief       This file provides a set of functions needed to drive the
  *              iis3dwb10is sensor.
  * @{
  *
  */

/**
  * @defgroup    IIS3DWB10IS_Interfaces_Functions
  * @brief       This section provide a set of functions used to read and
  *              write a generic register of the device.
  *              MANDATORY: return 0 -> no Error.
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
int32_t __weak iis3dwb10is_read_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                                    uint16_t len)
{
  if (ctx == NULL)
  {
    return -1;
  }

  return ctx->read_reg(ctx->handle, reg, data, len);
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
int32_t __weak iis3dwb10is_write_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                                     uint16_t len)
{
  if (ctx == NULL)
  {
    return -1;
  }

  return ctx->write_reg(ctx->handle, reg, data, len);
}

/**
  * @}
  *
  */

/**
  * @defgroup  Private_functions
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

uint64_t iis3dwb10is_from_lsb_to_us(uint64_t lsb)
{
  return (lsb * 25U);
}

float_t iis3dwb10is_from_lsb_to_celsius(int16_t lsb)
{
  return ((float_t)lsb / 200.0f);
}

float_t iis3dwb10is_16b_from_fs50g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 1.526f);
}

float_t iis3dwb10is_16b_from_fs100g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 3.052f);
}

float_t iis3dwb10is_16b_from_fs200g_to_mg(int16_t lsb)
{
  return ((float_t)lsb * 6.104f);
}

float_t iis3dwb10is_from_fs50g_to_mg(int32_t lsb)
{
  return ((float_t)lsb * 0.095f);
}

float_t iis3dwb10is_from_fs100g_to_mg(int32_t lsb)
{
  return ((float_t)lsb * 0.191f);
}

float_t iis3dwb10is_from_fs200g_to_mg(int32_t lsb)
{
  return ((float_t)lsb * 0.381f);
}

/**
  * @}
  *
  */

/**
  * @defgroup  Utility
  * @brief     These are standard utility functions
  * @{
  *
  */

/**
  * @brief  Device Who am I.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  id     Pointer to id buffer
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t iis3dwb10is_device_id_get(const stmdev_ctx_t *ctx, uint8_t *id)
{
  return iis3dwb10is_read_reg(ctx, IIS3DWB10IS_WHO_AM_I, id, 1);
}

/**
  * @brief  Change memory bank.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, ISPU_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_mem_bank_set(const stmdev_ctx_t *ctx, iis3dwb10is_mem_bank_t val)
{
  iis3dwb10is_ram_access_t ram_access;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_RAM_ACCESS, (uint8_t *)&ram_access, 1);

  /* MAIN_MEM_BANK */
  ram_access.page_sel = 0U;
  ram_access.ispu_ram_access_if = 0U;

  if (val == IIS3DWB10IS_ISPU_MEM_BANK)
  {
    ram_access.page_sel = 1U;
  }
  else if (val == IIS3DWB10IS_ISPU_MEM_BANK_RAM_ACCESS)
  {
    ram_access.page_sel = 1U;
    ram_access.ispu_ram_access_if = 1U;
  }

  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_RAM_ACCESS, (uint8_t *)&ram_access, 1);

  return ret;
}

/**
  * @brief  Change memory bank.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Pointer to MAIN_MEM_BANK, ISPU_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_mem_bank_get(const stmdev_ctx_t *ctx, iis3dwb10is_mem_bank_t *val)
{
  iis3dwb10is_ram_access_t ram_access;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_RAM_ACCESS, (uint8_t *)&ram_access, 1);

  if (ram_access.page_sel == 1U)
  {
    *val = IIS3DWB10IS_ISPU_MEM_BANK;
  }
  else
  {
    *val = IIS3DWB10IS_MAIN_MEM_BANK;
  }

  return ret;
}

/**
  * @brief  Perform reboot of the device.
  *
  * @param  ctx      read / write interface definitions
  * @retval          0: reboot has been performed, -1: error
  *
  */
int32_t iis3dwb10is_reboot(const stmdev_ctx_t *ctx)
{
  iis3dwb10is_ctrl3_t ctrl3;
  iis3dwb10is_data_rate_t val;
  int32_t ret;

  if (ctx->mdelay == NULL)
  {
    ret = -1;
    goto exit;
  }

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    goto exit;
  }

  /* 1. Set the accelerometer in idle mode */
  val.odr = IIS3DWB10IS_ODR_IDLE;
  ret = iis3dwb10is_xl_data_rate_set(ctx, val);
  if (ret != 0)
  {
    goto exit;
  }

  /* 2. wait 1 ms */
  ctx->mdelay(1);

  /* 3. Set the BOOT bit of the CTRL3 register to 1. */
  ctrl3.boot = 1;
  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    goto exit;
  }

  /* 4. Wait 20 ms. */
  ctx->mdelay(20);

exit:
  return ret;
}

/**
  * @brief  Perform power-on-reset of the device.
  *
  * @param  ctx      read / write interface definitions
  * @retval          0: power-on-reset has been performed, -1: error
  *
  */
int32_t iis3dwb10is_sw_por(const stmdev_ctx_t *ctx)
{
  iis3dwb10is_ram_access_t ram_access = {0};
  iis3dwb10is_data_rate_t val;
  int32_t ret;

  if (ctx->mdelay == NULL)
  {
    ret = -1;
    goto exit;
  }

  /* 1. Set the accelerometer in idle mode */
  val.odr = IIS3DWB10IS_ODR_IDLE;
  ret = iis3dwb10is_xl_data_rate_set(ctx, val);
  if (ret != 0)
  {
    goto exit;
  }

  /* 2. wait 1 ms */
  ctx->mdelay(1);

  /* 3. Set the SW_POR bit of the FUNC_CFG_ACCESS register to 1. */
  ram_access.sw_por = 1;
  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_RAM_ACCESS, (uint8_t *)&ram_access, 1);
  if (ret != 0)
  {
    goto exit;
  }

  /* 4. Wait 20 ms. */
  ctx->mdelay(20);

exit:
  return ret;
}

/**
  * @brief  Perform s/w reset of the device.
  *
  * @param  ctx      read / write interface definitions
  * @retval          0: s/w reset has been performed, -1: error
  *
  */
int32_t iis3dwb10is_sw_reset(const stmdev_ctx_t *ctx)
{
  iis3dwb10is_ctrl3_t ctrl3 = {0};
  iis3dwb10is_data_rate_t val;
  int32_t ret;

  if (ctx->mdelay == NULL)
  {
    ret = -1;
    goto exit;
  }

  /* 1. Set the accelerometer in idle mode */
  val.odr = IIS3DWB10IS_ODR_IDLE;
  ret = iis3dwb10is_xl_data_rate_set(ctx, val);
  if (ret != 0)
  {
    goto exit;
  }

  /* 2. Wait 1 ms */
  ctx->mdelay(1);

  /* 3. Set the SW_RESET bit to 1 */
  ctrl3.sw_reset = 1;
  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    goto exit;
  }

  /* 4. Wait 1 ms */
  ctx->mdelay(1);

exit:
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  device_configuration
  * @brief     These functions configure rate and full scale
  * @{
  *
  */

/**
  * @brief  ODR and burst values.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      enum iis3dwb10is_data_rate_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_data_rate_set(const stmdev_ctx_t *ctx, iis3dwb10is_data_rate_t val)
{
  iis3dwb10is_ctrl1_t ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL1, (uint8_t *)&ctrl1, 1);

  /* write burst when odr is 0 */
  if (val.burst != IIS3DWB10IS_CONTINUOS_MODE)
  {
    ctrl1.burst_cfg = (uint8_t)val.burst;
    ctrl1.odr_xl = 0U;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  ctrl1.odr_xl = (uint8_t)val.odr;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL1, (uint8_t *)&ctrl1, 1);

  return ret;
}

/**
  * @brief  ODR and burst values.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to enum iis3dwb10is_data_rate_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_data_rate_get(const stmdev_ctx_t *ctx, iis3dwb10is_data_rate_t *val)
{
  iis3dwb10is_ctrl1_t ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL1, (uint8_t *)&ctrl1, 1);

  switch (ctrl1.burst_cfg)
  {
    case 0x0:
      val->burst = IIS3DWB10IS_CONTINUOS_MODE;
      break;

    case 0x1:
      val->burst = IIS3DWB10IS_TON_UI_TOFF_ISPU;
      break;

    case 0x2:
      val->burst = IIS3DWB10IS_TON_UI_TOFF_FIFO;
      break;

    case 0x4:
      val->burst = IIS3DWB10IS_TON_STC_TOFF_UI;
      break;

    case 0x5:
      val->burst = IIS3DWB10IS_TON_STC_TOFF_ISPU;
      break;

    case 0x6:
      val->burst = IIS3DWB10IS_TON_STC_TOFF_FIFO;
      break;

    case 0x8:
      val->burst = IIS3DWB10IS_TON_EXT_TOFF_UI;
      break;

    case 0x9:
      val->burst = IIS3DWB10IS_TON_EXT_TOFF_ISPU;
      break;

    case 0xa:
      val->burst = IIS3DWB10IS_TON_EXT_TOFF_FIFO;
      break;

    default:
      val->burst = IIS3DWB10IS_CONTINUOS_MODE;
      break;
  }

  switch (ctrl1.odr_xl)
  {
    case 0:
      val->odr = IIS3DWB10IS_ODR_IDLE;
      break;

    case 2:
      val->odr = IIS3DWB10IS_ODR_2KHz5;
      break;

    case 3:
      val->odr = IIS3DWB10IS_ODR_5KHz;
      break;

    case 4:
      val->odr = IIS3DWB10IS_ODR_10KHz;
      break;

    case 5:
      val->odr = IIS3DWB10IS_ODR_20KHz;
      break;

    case 6:
      val->odr = IIS3DWB10IS_ODR_40KHz;
      break;

    case 7:
      val->odr = IIS3DWB10IS_ODR_80KHz;
      break;

    default:
      val->odr = IIS3DWB10IS_ODR_IDLE;
      break;
  }

  return ret;
}

/**
  * @brief  ODR and burst values.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      enum iis3dwb10is_data_rate_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_full_scale_set(const stmdev_ctx_t *ctx, iis3dwb10is_fs_xl_t val)
{
  iis3dwb10is_ctrl2_t ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL2, (uint8_t *)&ctrl2, 1);
  ctrl2.fs_xl = (uint8_t)val;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL2, (uint8_t *)&ctrl2, 1);

  return ret;
}

/**
  * @brief  ODR and burst values.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      enum iis3dwb10is_data_rate_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_full_scale_get(const stmdev_ctx_t *ctx, iis3dwb10is_fs_xl_t *val)
{
  iis3dwb10is_ctrl2_t ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL2, (uint8_t *)&ctrl2, 1);
  switch (ctrl2.fs_xl)
  {
    case 0:
      *val = IIS3DWB10IS_50g;
      break;

    case 1:
      *val = IIS3DWB10IS_100g;
      break;

    case 2:
      *val = IIS3DWB10IS_200g;
      break;

    default:
      *val = IIS3DWB10IS_50g;
      break;
  }

  return ret;
}

/**
  * @brief  Block data update.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: bdu off, 1: bdu on
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  ctrl3.bdu = (uint8_t)val;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);

  return ret;
}

/**
  * @brief  Block data update.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: bdu off, 1: bdu on
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  *val = ctrl3.bdu;

  return ret;
}

/**
  * @brief  XL axis configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_xl_data_cfg_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_data_config_set(const stmdev_ctx_t *ctx, iis3dwb10is_xl_data_cfg_t val)
{
  iis3dwb10is_ctrl4_t ctrl4;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL4, (uint8_t *)&ctrl4, 1);
  ctrl4.x_axis_enable = val.x_axis_en;
  ctrl4.y_axis_enable = val.y_axis_en;
  ctrl4.z_axis_enable = val.z_axis_en;
  ctrl4.rounding = (uint8_t)val.rounding;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL4, (uint8_t *)&ctrl4, 1);

  return ret;
}

/**
  * @brief  XL axis configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to iis3dwb10is_xl_data_cfg_t enum
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_xl_data_config_get(const stmdev_ctx_t *ctx, iis3dwb10is_xl_data_cfg_t *val)
{
  iis3dwb10is_ctrl4_t ctrl4;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL4, (uint8_t *)&ctrl4, 1);
  val->x_axis_en = ctrl4.x_axis_enable;
  val->y_axis_en = ctrl4.y_axis_enable;
  val->z_axis_en = ctrl4.z_axis_enable;

  switch (ctrl4.rounding)
  {
    case IIS3DWB10IS_WRAPAROUND_DISABLED:
      val->rounding = IIS3DWB10IS_WRAPAROUND_DISABLED;
      break;

    case IIS3DWB10IS_WRAPAROUND_1_EN:
      val->rounding = IIS3DWB10IS_WRAPAROUND_1_EN;
      break;

    case IIS3DWB10IS_WRAPAROUND_2_EN:
      val->rounding = IIS3DWB10IS_WRAPAROUND_2_EN;
      break;

    default:
      val->rounding = IIS3DWB10IS_WRAPAROUND_DISABLED;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  interrupts
  * @brief     These functions configure the interrupts
  * @{
  *
  */

/**
  * @brief  Configure INT1 and INT2 pins.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      enum iis3dwb10is_int_pin_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_interrupt_pin_mode_set(const stmdev_ctx_t *ctx, iis3dwb10is_int_pin_t val)
{
  iis3dwb10is_pin_ctrl_t pin;
  iis3dwb10is_if_cfg_t if_cfg;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PIN_CTRL, (uint8_t *)&pin, 1);
  pin.io_pin_strength = (uint8_t)val.strength;
  pin.pd_dis_int = (uint8_t)val.pd_dis;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_PIN_CTRL, (uint8_t *)&pin, 1);
  if (ret != 0)
  {
    return -1;
  }

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);
  if_cfg.int_pp_od = (uint8_t)val.pp_od;
  if_cfg.int_active_level = val.int_active_level;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);


  return ret;
}

/**
  * @brief  Configure INT1 and INT2 pins.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to enum iis3dwb10is_int_pin_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_interrupt_pin_mode_get(const stmdev_ctx_t *ctx, iis3dwb10is_int_pin_t *val)
{
  iis3dwb10is_pin_ctrl_t pin;
  iis3dwb10is_if_cfg_t if_cfg;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PIN_CTRL, (uint8_t *)&pin, 1);
  if (ret != 0)
  {
    return -1;
  }

  switch (pin.io_pin_strength)
  {
    case IIS3DWB10IS_PAD_STRENGTH_LOWER:
      val->strength = IIS3DWB10IS_PAD_STRENGTH_LOWER;
      break;

    case IIS3DWB10IS_PAD_STRENGTH_INTERMEDIATE:
      val->strength = IIS3DWB10IS_PAD_STRENGTH_INTERMEDIATE;
      break;

    case IIS3DWB10IS_PAD_STRENGTH_HIGHEST:
      val->strength = IIS3DWB10IS_PAD_STRENGTH_HIGHEST;
      break;

    default:
      val->strength = IIS3DWB10IS_PAD_STRENGTH_LOWER;
      break;
  }


  switch (pin.pd_dis_int)
  {
    case IIS3DWB10IS_PD_INT1_ON_INT2_ON:
      val->pd_dis = IIS3DWB10IS_PD_INT1_ON_INT2_ON;
      break;

    case IIS3DWB10IS_PD_INT1_OFF_INT2_ON:
      val->pd_dis = IIS3DWB10IS_PD_INT1_OFF_INT2_ON;
      break;

    case IIS3DWB10IS_PD_INT1_ON_INT2_OFF:
      val->pd_dis = IIS3DWB10IS_PD_INT1_ON_INT2_OFF;
      break;

    case IIS3DWB10IS_PD_INT1_OFF_INT2_OFF:
      val->pd_dis = IIS3DWB10IS_PD_INT1_OFF_INT2_OFF;
      break;

    default:
      val->pd_dis = IIS3DWB10IS_PD_INT1_ON_INT2_ON;
      break;
  }

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);
  if (ret != 0)
  {
    return -1;
  }

  val->int_active_level = if_cfg.int_active_level;

  switch (if_cfg.int_pp_od)
  {
    case IIS3DWB10IS_PUSH_PULL:
      val->pp_od = IIS3DWB10IS_PUSH_PULL;
      break;

    case IIS3DWB10IS_OPEN_DRAIN:
      val->pp_od = IIS3DWB10IS_OPEN_DRAIN;
      break;

    default:
      val->pp_od = IIS3DWB10IS_PUSH_PULL;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  bus_cfg
  * @brief     These functions configure various busses
  * @{
  *
  */

/**
  * @brief  I3C configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_i3c_cfg_t enum.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_i3c_configure_set(const stmdev_ctx_t *ctx, iis3dwb10is_i3c_cfg_t val)
{
  iis3dwb10is_if_cfg_t if_cfg;
  iis3dwb10is_i3c_ctrl_t i3c_ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_I3C_CTRL, (uint8_t *)&i3c_ctrl, 1);

  if (ret == 0)
  {
    if_cfg.i3c_disable = val.i3c_disable;
    if_cfg.sda_pu_en = val.sda_pu_en;
    i3c_ctrl.int_en_i3c = val.i3c_int_en;
    i3c_ctrl.bus_act_sel = (uint8_t)val.bus_act_sel;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_I3C_CTRL, (uint8_t *)&i3c_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  I3C configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      poniter to iis3dwb10is_i3c_cfg_t enum.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_i3c_configure_get(const stmdev_ctx_t *ctx, iis3dwb10is_i3c_cfg_t *val)
{
  iis3dwb10is_if_cfg_t if_cfg;
  iis3dwb10is_i3c_ctrl_t i3c_ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_IF_CFG, (uint8_t *)&if_cfg, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_I3C_CTRL, (uint8_t *)&i3c_ctrl, 1);

  if (ret == 0)
  {
    val->i3c_disable = if_cfg.i3c_disable;
    val->sda_pu_en = if_cfg.sda_pu_en;
    val->i3c_int_en = i3c_ctrl.int_en_i3c;

    switch (i3c_ctrl.bus_act_sel)
    {
      case 0:
        val->bus_act_sel = IIS3DWB10IS_I3C_BUS_AVAIL_TIME_2US;
        break;

      case 1:
        val->bus_act_sel = IIS3DWB10IS_I3C_BUS_AVAIL_TIME_12_5US;
        break;

      case 2:
        val->bus_act_sel = IIS3DWB10IS_I3C_BUS_AVAIL_TIME_25US;
        break;

      case 3:
        val->bus_act_sel = IIS3DWB10IS_I3C_BUS_AVAIL_TIME_50US;
        break;

      default:
        val->bus_act_sel = IIS3DWB10IS_I3C_BUS_AVAIL_TIME_50US;
        break;
    }
  }

  return ret;
}

/**
  * @brief  SPI mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_spi_mode enum.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_spi_mode_set(const stmdev_ctx_t *ctx, iis3dwb10is_spi_mode_t val)
{
  iis3dwb10is_spi_ctrl_t spi_ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SPI_CTRL, (uint8_t *)&spi_ctrl, 1);

  if (ret == 0)
  {
    spi_ctrl.sim = (uint8_t)val;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_SPI_CTRL, (uint8_t *)&spi_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  SPI mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to iis3dwb10is_spi_mode enum.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_spi_mode_get(const stmdev_ctx_t *ctx, iis3dwb10is_spi_mode_t *val)
{
  iis3dwb10is_spi_ctrl_t spi_ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SPI_CTRL, (uint8_t *)&spi_ctrl, 1);

  if (ret == 0)
  {
    switch (spi_ctrl.sim)
    {
      case IIS3DWB10IS_SPI_4_WIRE:
        *val = IIS3DWB10IS_SPI_4_WIRE;
        break;

      case IIS3DWB10IS_SPI_3_WIRE:
        *val = IIS3DWB10IS_SPI_3_WIRE;
        break;

      default:
        *val = IIS3DWB10IS_SPI_4_WIRE;
        break;
    }
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  filters
  * @brief     These functions configure the device filters
  * @{
  *
  */

/**
  * @brief  LPF1 filter configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Filter configuration structure
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_lpf1_cfg_set(const stmdev_ctx_t *ctx, iis3dwb10is_lpf1_cfg_t val)
{
  iis3dwb10is_st_ctrl_t st_ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ST_CTRL, (uint8_t *)&st_ctrl1, 1);

  if (ret == 0)
  {
    st_ctrl1.lpf1_cfg = (uint8_t)val;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ST_CTRL, (uint8_t *)&st_ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  LPF1 filter configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Filter configuration structure
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_lpf1_cfg_get(const stmdev_ctx_t *ctx, iis3dwb10is_lpf1_cfg_t *val)
{
  iis3dwb10is_st_ctrl_t st_ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ST_CTRL, (uint8_t *)&st_ctrl1, 1);

  switch (st_ctrl1.lpf1_cfg)
  {
    case  0:
      *val = IIS3DWB10IS_LPF1_BW_20KHz;
      break;

    case 1:
      *val = IIS3DWB10IS_LPF1_BW_17KHz2;
      break;

    case 2:
      *val = IIS3DWB10IS_LPF1_BW_14KHz7;
      break;

    case 3:
      *val = IIS3DWB10IS_LPF1_BW_12KHz;
      break;

    case 4:
      *val = IIS3DWB10IS_LPF1_BW_9KHz32;
      break;

    case 5:
      *val = IIS3DWB10IS_LPF1_BW_6KHz89;
      break;

    case 6:
      *val = IIS3DWB10IS_LPF1_BW_4KHz55;
      break;

    case 7:
      *val = IIS3DWB10IS_LPF1_BW_2KHz56;
      break;

    case 8:
      *val = IIS3DWB10IS_LPF1_SWITCH_ODR;
      break;

    default:
      *val = IIS3DWB10IS_LPF1_BW_20KHz;
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
  * @brief     These functions configure the device FIFO
  * @{
  *
  */

/**
  * @brief  FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (9 Bytes) written in FIFO).[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      FIFO watermark threshold.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_watermark_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buf[2];
  iis3dwb10is_fifo_ctrl1_t *fifo_ctrl1;
  iis3dwb10is_fifo_ctrl2_t *fifo_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL1, buf, 2);

  if (ret == 0)
  {
    fifo_ctrl1 = (iis3dwb10is_fifo_ctrl1_t *)&buf[0];
    fifo_ctrl2 = (iis3dwb10is_fifo_ctrl2_t *)&buf[1];

    fifo_ctrl1->wtm = (uint8_t)(val & 0xffU);
    fifo_ctrl2->wtm = (uint8_t)((val >> 8U) & 0xfU);
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_FIFO_CTRL1, buf, 2);
  }

  return ret;
}

/**
  * @brief  FIFO watermark threshold (1 LSb = TAG (1 Byte) + 1 sensor (9 Bytes) written in FIFO).[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to FIFO watermark threshold.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_watermark_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buf[2];
  iis3dwb10is_fifo_ctrl1_t *fifo_ctrl1;
  iis3dwb10is_fifo_ctrl2_t *fifo_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL1, buf, 2);

  if (ret == 0)
  {
    fifo_ctrl1 = (iis3dwb10is_fifo_ctrl1_t *)&buf[0];
    fifo_ctrl2 = (iis3dwb10is_fifo_ctrl2_t *)&buf[1];

    *val = (fifo_ctrl2->wtm * 256U) | (uint16_t)fifo_ctrl1->wtm;
  }

  return ret;
}

/**
  * @brief  Stop FIFO recording when watermark is reached.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: stop enabled, 0: stop disabled.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_stop_on_wtm_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.stop_on_wtm = val;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Stop FIFO recording when watermark is reached.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: stop enabled, 0: stop disabled.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_stop_on_wtm_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  iis3dwb10is_fifo_ctrl3_t fifo_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  *val = fifo_ctrl3.stop_on_wtm;

  return ret;
}

/**
  * @brief  FIFO mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_mode_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_mode_set(const stmdev_ctx_t *ctx, iis3dwb10is_fifo_mode_t val)
{
  iis3dwb10is_fifo_ctrl3_t fifo_ctrl3;
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    fifo_ctrl3.fifo_mode = (uint8_t)val & 0x07U;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);

    ctrl3.fifo_en = (val == IIS3DWB10IS_FIFO_OFF) ? 0U : 1U;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_mode_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_mode_get(const stmdev_ctx_t *ctx, iis3dwb10is_fifo_mode_t *val)
{
  iis3dwb10is_fifo_ctrl3_t fifo_ctrl3;
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL3, (uint8_t *)&fifo_ctrl3, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret != 0)
  {
    return ret;
  }

  if (ctrl3.fifo_en == 0U)
  {
    *val = IIS3DWB10IS_FIFO_OFF;
    return 0;
  }

  switch (fifo_ctrl3.fifo_mode)
  {
    case IIS3DWB10IS_BYPASS_MODE:
      *val = IIS3DWB10IS_BYPASS_MODE;
      break;

    case IIS3DWB10IS_FIFO_MODE:
      *val = IIS3DWB10IS_FIFO_MODE;
      break;

    case IIS3DWB10IS_STREAM_MODE:
      *val = IIS3DWB10IS_STREAM_MODE;
      break;

    case IIS3DWB10IS_STREAM_TO_FIFO_MODE:
      *val = IIS3DWB10IS_STREAM_TO_FIFO_MODE;
      break;

    case IIS3DWB10IS_BYPASS_TO_STREAM_MODE:
      *val = IIS3DWB10IS_BYPASS_TO_STREAM_MODE;
      break;

    case IIS3DWB10IS_BYPASS_TO_FIFO_MODE:
      *val = IIS3DWB10IS_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = IIS3DWB10IS_BYPASS_MODE;
      break;
  }
  return ret;
}

/**
  * @brief  Batching in FIFO buffer of sensor data.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: enable, 0: disable (sensors: XL, temp, ISPU).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_batch_set(const stmdev_ctx_t *ctx, iis3dwb10is_fifo_sensor_batch_t val)
{
  uint8_t buf[2];
  iis3dwb10is_fifo_ctrl2_t *fifo_ctrl2;
  iis3dwb10is_fifo_ctrl3_t *fifo_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, buf, 2);
  if (ret == 0)
  {
    fifo_ctrl2 = (iis3dwb10is_fifo_ctrl2_t *)&buf[0];
    fifo_ctrl3 = (iis3dwb10is_fifo_ctrl3_t *)&buf[1];

    fifo_ctrl2->dec_ts_batch = (uint8_t)val.batch_ts;
    fifo_ctrl3->xl_batch   = val.batch_xl;
    fifo_ctrl3->t_batch    = val.batch_temp;
    fifo_ctrl3->ispu_batch = val.batch_ispu;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, buf, 2);
  }

  return ret;
}

/**
  * @brief  Batching in FIFO buffer of sensor data.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      1: enable, 0: disable (sensors: XL, temp, ISPU).
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_batch_get(const stmdev_ctx_t *ctx, iis3dwb10is_fifo_sensor_batch_t *val)
{
  uint8_t buf[2];
  iis3dwb10is_fifo_ctrl2_t *fifo_ctrl2;
  iis3dwb10is_fifo_ctrl3_t *fifo_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, buf, 2);
  if (ret == 0)
  {
    fifo_ctrl2 = (iis3dwb10is_fifo_ctrl2_t *)&buf[0];
    fifo_ctrl3 = (iis3dwb10is_fifo_ctrl3_t *)&buf[1];

    switch (fifo_ctrl2->dec_ts_batch)
    {
      case IIS3DWB10IS_TMSTMP_NOT_BATCHED:
        val->batch_ts = IIS3DWB10IS_TMSTMP_NOT_BATCHED;
        break;

      case IIS3DWB10IS_TMSTMP_DEC_1:
        val->batch_ts = IIS3DWB10IS_TMSTMP_DEC_1;
        break;

      case IIS3DWB10IS_TMSTMP_DEC_8:
        val->batch_ts = IIS3DWB10IS_TMSTMP_DEC_8;
        break;

      case IIS3DWB10IS_TMSTMP_DEC_32:
        val->batch_ts = IIS3DWB10IS_TMSTMP_DEC_32;
        break;

      default:
        val->batch_ts = IIS3DWB10IS_TMSTMP_NOT_BATCHED;
        break;
    }

    val->batch_xl   = fifo_ctrl3->xl_batch;
    val->batch_temp = fifo_ctrl3->t_batch;
    val->batch_ispu = fifo_ctrl3->ispu_batch;
  }

  return ret;
}

/**
  * @brief  Configure FIFO ISPU control.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_ispu_ctrl_batch_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_ispu_ctrl_set(const stmdev_ctx_t *ctx,
                                       iis3dwb10is_fifo_ispu_ctrl_batch_t val)
{
  iis3dwb10is_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret == 0)
  {
    fifo_ctrl2.fifo_read_from_ispu   = val.read_from_ispu;
    fifo_ctrl2.fifo_trigger_cfg      = (uint8_t)val.trigger;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Configure FIFO ISPU control.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_ispu_ctrl_batch_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_ispu_ctrl_get(const stmdev_ctx_t *ctx,
                                       iis3dwb10is_fifo_ispu_ctrl_batch_t *val)
{
  iis3dwb10is_fifo_ctrl2_t fifo_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_CTRL2, (uint8_t *)&fifo_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->read_from_ispu = fifo_ctrl2.fifo_read_from_ispu;
  switch (fifo_ctrl2.fifo_trigger_cfg)
  {
    case IIS3DWB10IS_FIFO_TRIGGER_ISPU:
      val->trigger = IIS3DWB10IS_FIFO_TRIGGER_ISPU;
      break;

    case IIS3DWB10IS_FIFO_TRIGGER_INT2:
      val->trigger = IIS3DWB10IS_FIFO_TRIGGER_INT2;
      break;

    default:
      val->trigger = IIS3DWB10IS_FIFO_TRIGGER_ISPU;
      break;
  }

  return ret;
}

int32_t iis3dwb10is_fifo_status_get(const stmdev_ctx_t *ctx, iis3dwb10is_fifo_status_t *val)
{
  uint8_t buff[2];
  iis3dwb10is_fifo_status2_t *status;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_STATUS1, buff, 2);
  if (ret != 0)
  {
    return ret;
  }

  status = (iis3dwb10is_fifo_status2_t *)&buff[1];

  val->fifo_ovr  = status->fifo_ovr_ia;
  val->fifo_full = status->fifo_full_ia;
  val->fifo_th   = status->fifo_wtm_ia;

  val->fifo_level = (uint16_t)buff[1] & 0x0fU;
  val->fifo_level = (val->fifo_level * 256U) + buff[0];

  return ret;
}

/**
  * @brief  FIFO raw data output[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_out_raw_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_out_raw_get(const stmdev_ctx_t *ctx, uint8_t *fifo_buf, uint16_t cnt)
{
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_FIFO_DATA_OUT_TAG, fifo_buf, FIFO_ROW_LEN * cnt);

  return ret;
}

/**
  * @brief  FIFO data process[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_fifo_out_raw_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_fifo_process(uint8_t *fifo_buf, iis3dwb10is_fifo_out_raw_t *val)
{
  iis3dwb10is_fifo_data_out_tag_t *fifo_tag;
  uint8_t *datap;
  uint8_t i;

  fifo_tag = (iis3dwb10is_fifo_data_out_tag_t *)&fifo_buf[0];
  datap = &fifo_buf[1];

  switch (fifo_tag->tag_sensor)
  {
    case IIS3DWB10IS_TAG_EMPTY:
      val->tag = IIS3DWB10IS_TAG_EMPTY;
      break;

    case IIS3DWB10IS_TAG_XL:
      val->tag = IIS3DWB10IS_TAG_XL;

      val->xl.x_raw = (int32_t)datap[2];
      val->xl.x_raw = (val->xl.x_raw * 256) + (int32_t)datap[1];
      val->xl.x_raw = (val->xl.x_raw * 256) + (int32_t)datap[0];
      val->xl.y_raw = (int32_t)datap[5];
      val->xl.y_raw = (val->xl.y_raw * 256) + (int32_t)datap[4];
      val->xl.y_raw = (val->xl.y_raw * 256) + (int32_t)datap[3];
      val->xl.z_raw = (int32_t)datap[8];
      val->xl.z_raw = (val->xl.z_raw * 256) + (int32_t)datap[7];
      val->xl.z_raw = (val->xl.z_raw * 256) + (int32_t)datap[6];
      break;

    case IIS3DWB10IS_TAG_TEMP:
      val->tag = IIS3DWB10IS_TAG_TEMP;

      val->temp_raw = (int16_t)datap[2];
      val->temp_raw = (val->temp_raw * 256) + (int16_t)datap[1];
      break;

    case IIS3DWB10IS_TAG_TS:
      val->tag = IIS3DWB10IS_TAG_TS;

      val->ts_raw = (uint64_t)datap[4];
      val->ts_raw = (val->ts_raw * 256U) + (uint64_t)datap[3];
      val->ts_raw = (val->ts_raw * 256U) + (uint64_t)datap[2];
      val->ts_raw = (val->ts_raw * 256U) + (uint64_t)datap[1];
      val->ts_raw = (val->ts_raw * 256U) + (uint64_t)datap[0];
      break;

    default:
      /* unknown tag */
      return -1;
  }

  /* always return register raw data */
  for (i = 0U; i < (FIFO_ROW_LEN - 1U); i++)
  {
    val->raw[i] = datap[i];
  }

  return 0;
}

/**
  * @brief  COUNTER ODR configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      cnt_bdr_th value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_counter_bdr_cfg_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  iis3dwb10is_counter_bdr_h_t bdr_h;
  iis3dwb10is_counter_bdr_l_t bdr_l;
  int32_t ret;

  if (val > 4096)
    return -1;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_COUNTER_BDR_H, (uint8_t *)&bdr_h, 1);

  if (ret != 0)
  {
    return ret;
  }

  bdr_l.cnt_bdr_th = val & 0xff;
  bdr_h.cnt_bdr_th = val / 256;

  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_COUNTER_BDR_L, (uint8_t *)&bdr_l, 1);
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_COUNTER_BDR_H, (uint8_t *)&bdr_h, 1);

  return ret;
}

/**
  * @brief  COUNTER ODR configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      cnt_bdr_th value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_counter_bdr_cfg_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  iis3dwb10is_counter_bdr_h_t bdr_h;
  iis3dwb10is_counter_bdr_l_t bdr_l;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_COUNTER_BDR_L, (uint8_t *)&bdr_l, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_COUNTER_BDR_H, (uint8_t *)&bdr_h, 1);

  if (ret != 0)
  {
    return ret;
  }

  *val = bdr_l.cnt_bdr_th;
  *val += bdr_h.cnt_bdr_th * 256;

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
  * @brief  ISPU enable
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_enable(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  ctrl3.ispu_en = val & 0x1U;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);

  return ret;
}

/**
  * @brief  ISPU reset
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_reset(const stmdev_ctx_t *ctx)
{
  iis3dwb10is_ispu_ctrl2_t ctrl;
  int32_t ret;

  /* assert reset bit */
  ctrl.sw_reset_ispu = 1;
  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL2, (uint8_t *)&ctrl, 1);

  /* wait at least 100 usecs */
  ctx->mdelay(1);

  /* de-assert reset bit */
  ctrl.sw_reset_ispu = 0;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL2, (uint8_t *)&ctrl, 1);

  return ret;
}

/**
  * @brief  ispu TIMESTAMP_EN.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: disable timestamp, 1: enable timestamp
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_timestamp_en_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ispu_ctrl2_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL2, (uint8_t *)&ctrl, 1);
  if (ret == 0)
  {
    ctrl.timestamp_en = val;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL2, (uint8_t *)&ctrl, 1);
  }

  return ret;
}

/**
  * @brief  ispu TIMESTAMP_EN.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: disable timestamp, 1: enable timestamp
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_timestamp_en_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  iis3dwb10is_ispu_ctrl2_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL2, (uint8_t *)&ctrl, 1);
  *val = ctrl.timestamp_en;

  return ret;
}

/**
  * @brief  ispu rate.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      rate values (iis3dwb10is_ispu_rate_t)
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_rate_set(const stmdev_ctx_t *ctx, iis3dwb10is_ispu_rate_t val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  if (ret == 0)
  {
    ctrl.ispu_rate = val;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  }

  return ret;
}

/**
  * @brief  ispu rate.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      rate values (iis3dwb10is_ispu_rate_t)
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_rate_get(const stmdev_ctx_t *ctx, iis3dwb10is_ispu_rate_t *val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  switch (ctrl.ispu_rate)
  {
    case 0:
      *val = IIS3DWB10IS_ISPU_RATE_IDLE;
      break;

    case 2:
      *val = IIS3DWB10IS_ISPU_RATE_2KHz5;
      break;

    case 3:
      *val = IIS3DWB10IS_ISPU_RATE_5KHz;
      break;

    case 4:
      *val = IIS3DWB10IS_ISPU_RATE_10KHz;
      break;

    case 5:
      *val = IIS3DWB10IS_ISPU_RATE_20KHz;
      break;

    case 6:
      *val = IIS3DWB10IS_ISPU_RATE_40KHz;
      break;

    case 7:
      *val = IIS3DWB10IS_ISPU_RATE_80KHz;
      break;

    default:
      *val = IIS3DWB10IS_ISPU_RATE_IDLE;
      break;
  }

  return ret;
}

/**
  * @brief  ispu bdu.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: bdu disabled, 1: bdu enabled
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_bdu_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  if (ret == 0)
  {
    ctrl.ispu_bdu = val;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  }

  return ret;
}

/**
  * @brief  ispu bdu.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: bdu disabled, 1: bdu enabled
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_bdu_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  *val = ctrl.ispu_bdu;

  return ret;
}

/**
  * @brief  ispu grant register access.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: access disabled, 1: access enabled
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_grant_ispu_regs_access_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  if (ret == 0)
  {
    ctrl.reg_access_confirm = val;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  }

  return ret;
}

/**
  * @brief  ispu register access.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: access disabled, 1: access enabled
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_grant_ispu_regs_access_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  *val = ctrl.reg_access_confirm;

  return ret;
}

/**
  * @brief  ispu trigger interrupt.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: clear interrupt trigger, 1: assert interrupt trigger
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_trigger_interrupt(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ispu_ctrl1_t ctrl;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  if (ret == 0)
  {
    ctrl.loprio_user_trig = val;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CTRL1, (uint8_t *)&ctrl, 1);
  }

  return ret;
}

/**
  * @brief  ispu status.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_int_status_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_status_get(const stmdev_ctx_t *ctx, iis3dwb10is_ispu_int_status_t *val)
{
  uint8_t reg[2];
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_INT_STATUS1_MAINPAGE, reg, 2);
  if (ret == 0)
  {
    if (val != NULL)
    {
      uint8_t *ia_p = (uint8_t *)&val->ispu_ia;

      bytecpy(&ia_p[0], &reg[0]);
      bytecpy(&ia_p[1], &reg[1]);
    }
    else
    {
      ret = -1;
    }
  }

  return ret;
}

/**
  * @brief  ispu tpf.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      tpf value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_tpf_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  return iis3dwb10is_read_reg(ctx, IIS3DWB10IS_TPF, val, 1);
}

/**
  * @brief  ispu device status.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_dev_status_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_dev_status_get(const stmdev_ctx_t *ctx,
                                        iis3dwb10is_ispu_dev_status_t *val)
{
  iis3dwb10is_device_status_t status;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_DEVICE_STATUS, (uint8_t *)&status, 1);
  if (ret == 0)
  {
      val->ctrl_access = status.ispu_ctrl_access;
      val->core_sleep = status.ispu_core_sleep;
  }

  return ret;
}

/**
  * @brief  ispu loprio configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_loprio_cfg_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_loprio_cfg_set(const stmdev_ctx_t *ctx,
                                        iis3dwb10is_ispu_loprio_cfg_t val)
{
  iis3dwb10is_ispu_loprio_en_t loprio;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_LOPRIO_EN, (uint8_t *)&loprio, 1);
  if (ret == 0)
  {
    loprio.loprio_fifo_en = val.fifo_en;
    loprio.loprio_sleepcnt_en = val.sleepcnt_en;
    loprio.loprio_ext_trg_en = val.ext_trg_en;
    loprio.loprio_user_trg_en = val.user_trg_en;
    loprio.loprio_temp_en = val.temp_en;

    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_LOPRIO_EN, (uint8_t *)&loprio, 1);
  }

  return ret;
}

/**
  * @brief  ispu loprio configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_loprio_cfg_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_loprio_cfg_get(const stmdev_ctx_t *ctx,
                                        iis3dwb10is_ispu_loprio_cfg_t *val)
{
  iis3dwb10is_ispu_loprio_en_t loprio;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_LOPRIO_EN, (uint8_t *)&loprio, 1);
  if (ret == 0)
  {
    val->fifo_en = loprio.loprio_fifo_en;
    val->sleepcnt_en = loprio.loprio_sleepcnt_en;
    val->ext_trg_en = loprio.loprio_ext_trg_en;
    val->user_trg_en = loprio.loprio_user_trg_en;
    val->temp_en = loprio.loprio_temp_en;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup ispu page
  * @brief    control interaction with ISPU
  * @{/
  *
  */

/**
  * @brief  Boot ISPU core[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      IIS3DWB10IS_ISPU_TURN_ON / IIS3DWB10IS_ISPU_TURN_OFF
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_boot_set(const stmdev_ctx_t *ctx,
                                  iis3dwb10is_ispu_boot_latched_t val)
{
  iis3dwb10is_ispu_config_t ispu_config = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ispu_config.rst_n = (val == IIS3DWB10IS_ISPU_TURN_ON) ? 1U : 0U;
  ispu_config.clk_dis = (val == IIS3DWB10IS_ISPU_TURN_ON) ? 0U : 1U;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);

exit:
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables latched ISPU interrupt.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_int_latched_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_int_latched_set(const stmdev_ctx_t *ctx,
                                         iis3dwb10is_ispu_int_latched_t val)
{
  iis3dwb10is_ispu_config_t ispu_config = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  if (ret != 0)
  {
    goto exit;
  }

  ispu_config.latched = ((uint8_t)val & 0x3U);
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);

exit:
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enables latched ISPU interrupt.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_ispu_int_latched_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_int_latched_get(const stmdev_ctx_t *ctx,
                                         iis3dwb10is_ispu_int_latched_t *val)
{
  iis3dwb10is_ispu_config_t ispu_config = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_config, 1);
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);
  if (ret != 0)
  {
    return ret;
  }

  switch ((ispu_config.latched))
  {
    case 0x00:
      *val = IIS3DWB10IS_ISPU_INT_PULSED_PIN_PULSED;
      break;

    case 0x01:
      *val = IIS3DWB10IS_ISPU_INT_LATCHED_PIN_PULSED;
      break;

    case 0x02:
    case 0x03:
      *val = IIS3DWB10IS_ISPU_INT_LATCHED_PIN_LATCHED;
      break;

    default:
      *val = IIS3DWB10IS_ISPU_INT_PULSED_PIN_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  returns ISPU boot status
  *
  * @param  ctx      read / write interface definitions
  * @param  val      IIS3DWB10IS_ISPU_BOOT_IN_PROGRESS / IIS3DWB10IS_ISPU_BOOT_ENDED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_get_boot_status(const stmdev_ctx_t *ctx,
                                         iis3dwb10is_ispu_boot_end_t *val)
{
  iis3dwb10is_ispu_status_t ispu_boot_status = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_STATUS, (uint8_t *)&ispu_boot_status, 1);
  if (ret == 0)
  {
    *val = (iis3dwb10is_ispu_boot_end_t)ispu_boot_status.boot_end;
  }
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

static int32_t iis3dwb10is_ispu_sel_memory_addr(const stmdev_ctx_t *ctx, uint16_t mem_addr)
{
  uint8_t mem_addr_l = 0, mem_addr_h = 0;
  int32_t ret = 0;

  mem_addr_l = (uint8_t)(mem_addr & 0xFFU);
  mem_addr_h = (uint8_t)(mem_addr / 256U);
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_MEM_ADDR2,
                               (uint8_t *)&mem_addr_h, 1);
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_MEM_ADDR1,
                               (uint8_t *)&mem_addr_l, 1);

  return ret;
}

/**
  * @brief  ISPU write memory. ISPU clock is disabled inside the routine.
  *
  * @param  ctx      read / write interface definitions
  * @param  mem_sel  IIS3DWB10IS_ISPU_DATA_RAM_MEMORY, IIS3DWB10IS_ISPU_PROGRAM_RAM_MEMORY
  * @param  mem_addr relative memory address in memory, i.e.
  *                  [0x0 - 0x7FFF] for Program RAM (32KB)
  *                  [0x0 - 0xDFFF] for Data RAM (56KB)
  * @param  mem_data memory data
  * @param  len      data length
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_write_memory(const stmdev_ctx_t *ctx,
                                      iis3dwb10is_ispu_memory_type_t mem_sel,
                                      uint16_t mem_addr, uint8_t *mem_data, uint16_t len)
{
  iis3dwb10is_ispu_mem_sel_t ispu_mem_sel = {0};
  iis3dwb10is_ispu_config_t ispu_cfg = {0};
  uint8_t clk_dis = 0;
  int32_t ret = 0;

  if (ctx->mdelay == NULL)
  {
    return -1;
  }

  if (mem_sel == IIS3DWB10IS_ISPU_DATA_RAM_MEMORY)
  {
    /* Data RAM size is 56KB */
    if (mem_addr >= 0xE000)
      goto exit;

    /* transform address into physical */
    mem_addr += IIS3DWB10IS_ISPU_DATA_RAM_START;
  }
  else
  {
    /* Program RAM size is 8KB */
    if (mem_addr >= 0x8000)
      goto exit;
  }

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK_RAM_ACCESS);
  if (ret == 0)
  {
    ctx->mdelay(1); /* latency of 450 us */

    /* disable ISPU clock */
    ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    if (ret != 0)
    {
      goto exit;
    }
    clk_dis = ispu_cfg.clk_dis;
    ispu_cfg.clk_dis = 1;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    if (ret != 0)
    {
      goto exit;
    }

    /* select memory to be written */
    ispu_mem_sel.read_ram = 0;
    ispu_mem_sel.rom_sel = (uint8_t)mem_sel;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_MEM_SEL, (uint8_t *)&ispu_mem_sel, 1);
    if (ret != 0)
    {
      goto exit;
    }

    /* select memory address */
    ret += iis3dwb10is_ispu_sel_memory_addr(ctx, mem_addr);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_MEM_DATA_1, &mem_data[0], len);

    if (ret != 0)
    {
      goto exit;
    }

    /* set ISPU clock back to previous value */
    ispu_cfg.clk_dis = clk_dis;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    if (ret != 0)
    {
      goto exit;
    }
  }

exit:
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU read memory. ISPU clock is disabled inside the routine.
  *
  * @param  ctx      read / write interface definitions
  * @param  mem_sel  IIS3DWB10IS_ISPU_DATA_RAM_MEMORY, IIS3DWB10IS_ISPU_PROGRAM_RAM_MEMORY
  * @param  mem_addr relative memory address in memory, i.e.
  *                  [0x0 - 0x7FFF] for Program RAM (32KB)
  *                  [0x0 - 0xDFFF] for Data RAM (56KB)
  * @param  mem_data memory data
  * @param  len      data length
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_read_memory(const stmdev_ctx_t *ctx,
                                     iis3dwb10is_ispu_memory_type_t mem_sel,
                                     uint16_t mem_addr, uint8_t *mem_data, uint16_t len)
{
  iis3dwb10is_ispu_mem_sel_t ispu_mem_sel = {0};
  iis3dwb10is_ispu_config_t ispu_cfg = {0};
  uint8_t clk_dis = 0;
  int32_t ret = 0;
  uint8_t dummy[2];

  if (mem_sel == IIS3DWB10IS_ISPU_DATA_RAM_MEMORY)
  {
    /* Data RAM size is 56KB */
    if (mem_addr >= 0xE000)
      goto exit;

    /* transform address into physical */
    mem_addr += IIS3DWB10IS_ISPU_DATA_RAM_START;
  }
  else
  {
    /* Program RAM size is 8KB */
    if (mem_addr >= 0x8000)
      goto exit;
  }

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK_RAM_ACCESS);
  if (ret == 0)
  {
    /* disable ISPU clock */
    ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    if (ret != 0)
    {
      goto exit;
    }
    clk_dis = ispu_cfg.clk_dis;
    ispu_cfg.clk_dis = 1;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
    if (ret != 0)
    {
      goto exit;
    }

    /* select memory to be read */
    ispu_mem_sel.read_ram = 1;
    ispu_mem_sel.rom_sel = (uint8_t)mem_sel;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_MEM_SEL, (uint8_t *)&ispu_mem_sel, 1);
    if (ret != 0)
    {
      goto exit;
    }

    /* select memory address */
    ret += iis3dwb10is_ispu_sel_memory_addr(ctx, mem_addr);
    if (ret != 0)
    {
      goto exit;
    }

    /* read data */
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_MEM_DATA_1, dummy, 2);
    if (ret != 0)
    {
      goto exit;
    }
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_MEM_DATA_1, &mem_data[0], len);
    if (ret != 0)
    {
      goto exit;
    }

    /* set ISPU clock back to previous value */
    ispu_cfg.clk_dis = clk_dis;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_CONFIG, (uint8_t *)&ispu_cfg, 1);
  }

exit:
  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_write_flags(const stmdev_ctx_t *ctx, uint16_t data)
{
  uint8_t flag = 0U;
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the flags */
    flag = (uint8_t)(data / 256U);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_IF2S_FLAG_H, &flag, 1);

    flag = (uint8_t)(data & 0xffU);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_IF2S_FLAG_L, &flag, 1);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_read_flags(const stmdev_ctx_t *ctx, uint16_t *data)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);

  /* read the flags */
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_S2IF_FLAG_L, buff, 2);
  if (ret == 0)
  {
    data[0] = (uint16_t)buff[1];
    data[0] = (data[0] * 256U) + (uint16_t)buff[0];
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU clear flags (S2IF)
  *
  * @param  ctx      read / write interface definitions
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_clear_flags(const stmdev_ctx_t *ctx)
{
  uint8_t data = 1;
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);

  if (ret == 0)
  {
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_S2IF_FLAG_H, &data, 1);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_read_data_raw_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val,
                                           uint8_t len)
{
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_DOUT_00_L, (uint8_t *) val,
                                len);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_int1_ctrl_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int1_ctrl reg */
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_INT1_CTRL1, &buff[0], 2);

    if (ret == 0)
    {
      *val = (uint32_t)buff[1];
      *val = (*val * 256U) + (uint32_t)buff[0];
    }
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_int1_ctrl_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the int1_ctrl reg */
    buff[1] = (uint8_t)((val >>  8) & 0xffU);
    buff[0] = (uint8_t)(val & 0xffU);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_INT1_CTRL1, buff, 2);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_int2_ctrl_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read int1_ctrl reg */
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_INT2_CTRL1, &buff[0], 2);

    if (ret == 0)
    {
      *val = (uint32_t)buff[1];
      *val = (*val * 256U) + (uint32_t)buff[0];
    }
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_int2_ctrl_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the int1_ctrl reg */
    buff[1] = (uint8_t)((val >>  8) & 0xffU);
    buff[0] = (uint8_t)(val & 0xffU);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_INT2_CTRL1, buff, 2);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  ISPU int_status.[get]
  *         You may also use `int32_t iis3dwb10is_ispu_status_get()` which is a
  *         more efficient API.
  *
  * @param  ctx      read / write interface definitions
  * @param  val      ISPU int2_status register value
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_ispu_int_status_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read ISPU int_status reg */
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_INT_STATUS1, &buff[0], 2);

    if (ret == 0)
    {
      *val = (uint32_t)buff[1];
      *val = (*val * 256U) + (uint32_t)buff[0];
    }
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_algo_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* read ispu algo reg */
    ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_ISPU_ALGO1, &buff[0], 2);

    if (ret == 0)
    {
      *val = (uint32_t)buff[1];
      *val = (*val * 256U) + (uint32_t)buff[0];
    }
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

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
int32_t iis3dwb10is_ispu_algo_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2] = {0};
  int32_t ret = 0;

  ret = iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_ISPU_MEM_BANK);
  if (ret == 0)
  {
    /* write the algo reg */
    buff[1] = (uint8_t)((val >>  8) & 0xffU);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_ALGO2, &buff[1], 1);

    buff[0] = (uint8_t)(val & 0xffU);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_ISPU_ALGO1, &buff[0], 1);
  }

  ret += iis3dwb10is_mem_bank_set(ctx, IIS3DWB10IS_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @brief  SLEEPCNT configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_slpcnt_cfg_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_sleepcnt_cfg_set(const stmdev_ctx_t *ctx, iis3dwb10is_slpcnt_cfg_t val)
{
  iis3dwb10is_sleepcnt_cfg_t cfg;
  uint8_t threshold[2];
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SLEEPCNT_CFG, (uint8_t *)&cfg, 1);
  if (ret == 0)
  {
    threshold[0] = (uint8_t)(val.threshold_val & 0xFFU);
    threshold[1] = (uint8_t)(val.threshold_val / 256U);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_SLEEPCNT_TH_L, threshold, 2);

    cfg.rst_sleepcnt = val.reset;
    cfg.enable_sleepcnt = val.enable;
    cfg.tick_sel = (uint8_t)val.tick_sel;
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_SLEEPCNT_CFG, (uint8_t *)&cfg, 1);
  }

  return ret;
}

/**
  * @brief  SLEEPCNT configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to iis3dwb10is_slpcnt_cfg_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_sleepcnt_cfg_get(const stmdev_ctx_t *ctx, iis3dwb10is_slpcnt_cfg_t *val)
{
  iis3dwb10is_sleepcnt_cfg_t cfg;
  uint8_t threshold[2];
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SLEEPCNT_CFG, (uint8_t *)&cfg, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SLEEPCNT_TH_L, threshold, 2);
  if (ret == 0)
  {
    val->threshold_val = threshold[1];
    val->threshold_val = (val->threshold_val * 256U) + threshold[0];

    val->reset = cfg.rst_sleepcnt;
    val->enable = cfg.enable_sleepcnt;

    switch (cfg.tick_sel)
    {
      case IIS3DWB10IS_SLP_TICK_SLOW:
        val->tick_sel = IIS3DWB10IS_SLP_TICK_SLOW;
        break;

      case IIS3DWB10IS_SLP_TICK_FAST:
        val->tick_sel = IIS3DWB10IS_SLP_TICK_FAST;
        break;

      default:
        val->tick_sel = IIS3DWB10IS_SLP_TICK_SLOW;
        break;
    }
  }

  return ret;
}

/**
  * @brief  SLEEPCNT time.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      tiime (uint16_t)
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_sleepcnt_time_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t *valp = (uint8_t *)&val;
  int32_t ret;

  ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_SLEEPCNT_TIME_L, valp, 2);

  return ret;
}

/**
  * @brief  SLEEPCNT time.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      tiime (uint16_t)
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_sleepcnt_time_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buf[2];
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_SLEEPCNT_TIME_L, buf, 2);

  *val = buf[0] | (buf[1] & 0x7F) * 256U;

  return ret;
}

/**
  * @brief  Set Trigger from UI.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      0: trigger OFF - 1: trigger ON
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_burst_ui_trigger_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  iis3dwb10is_ctrl3_t ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);
  ctrl3.burst_force_trg = val & 0x1U;
  ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_CTRL3, (uint8_t *)&ctrl3, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @brief  Configure PLL control.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_pll_ctrl_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pll_ctrl_set(const stmdev_ctx_t *ctx, iis3dwb10is_pll_ctrl_t val)
{
  iis3dwb10is_pll_ctrl_1_t pll_ctrl1;
  iis3dwb10is_pll_ctrl_2_t pll_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PLL_CTRL_1, (uint8_t *)&pll_ctrl1, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PLL_CTRL_2, (uint8_t *)&pll_ctrl2, 1);
  if (ret == 0)
  {
    pll_ctrl1.osc_ext_sel  = (uint8_t)val.osc_ext_sel;
    pll_ctrl1.ref_div      = (uint8_t)val.ref_div;
    pll_ctrl2.pll_div      = val.pll_div;
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_PLL_CTRL_1, (uint8_t *)&pll_ctrl1, 1);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_PLL_CTRL_2, (uint8_t *)&pll_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Configure PLL control.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pointer to iis3dwb10is_pll_ctrl_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pll_ctrl_get(const stmdev_ctx_t *ctx, iis3dwb10is_pll_ctrl_t *val)
{
  iis3dwb10is_pll_ctrl_1_t pll_ctrl1;
  iis3dwb10is_pll_ctrl_2_t pll_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PLL_CTRL_1, (uint8_t *)&pll_ctrl1, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_PLL_CTRL_2, (uint8_t *)&pll_ctrl2, 1);
  if (ret != 0)
  {
    return ret;
  }

  val->pll_div = pll_ctrl2.pll_div;

  switch (pll_ctrl1.ref_div)
  {
    case IIS3DWB10IS_PLL_NO_DIVIDER:
      val->ref_div = IIS3DWB10IS_PLL_NO_DIVIDER;
      break;

    case IIS3DWB10IS_PLL_DIV_2:
      val->ref_div = IIS3DWB10IS_PLL_DIV_2;
      break;

    case IIS3DWB10IS_PLL_DIV_4:
      val->ref_div = IIS3DWB10IS_PLL_DIV_4;
      break;

    case IIS3DWB10IS_PLL_DIV_8:
      val->ref_div = IIS3DWB10IS_PLL_DIV_8;
      break;

    case IIS3DWB10IS_PLL_DIV_16:
      val->ref_div = IIS3DWB10IS_PLL_DIV_16;
      break;

    case IIS3DWB10IS_PLL_DIV_32:
      val->ref_div = IIS3DWB10IS_PLL_DIV_32;
      break;

    case IIS3DWB10IS_PLL_DIV_64:
      val->ref_div = IIS3DWB10IS_PLL_DIV_64;
      break;

    case IIS3DWB10IS_PLL_DIV_128:
      val->ref_div = IIS3DWB10IS_PLL_DIV_128;
      break;

    default:
      val->ref_div = IIS3DWB10IS_PLL_NO_DIVIDER;
      break;
  }

  switch (pll_ctrl1.osc_ext_sel)
  {
    case IIS3DWB10IS_PLL_INTERNAL_CLOCK:
      val->osc_ext_sel = IIS3DWB10IS_PLL_INTERNAL_CLOCK;
      break;

    case IIS3DWB10IS_PLL_EXTERNAL_CLOCK:
      val->osc_ext_sel = IIS3DWB10IS_PLL_EXTERNAL_CLOCK;
      break;

    default:
      val->osc_ext_sel = IIS3DWB10IS_PLL_INTERNAL_CLOCK;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @brief  Enables pulsed data-ready mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DRDY_LEVEL, DRDY_PULSED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_data_ready_mode_set(const stmdev_ctx_t *ctx, iis3dwb10is_data_ready_mode_t val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);

  if (ret == 0)
  {
    int_ctrl1.pulsed_dataready = ((uint8_t)val & 0x1U);
    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
  }

  return ret;
}

/**
  * @brief  Enables pulsed data-ready mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      DRDY_LATCHED, DRDY_PULSED,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_data_ready_mode_get(const stmdev_ctx_t *ctx, iis3dwb10is_data_ready_mode_t *val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);

  if (ret == 0)
  {
    switch (int_ctrl1.pulsed_dataready)
    {
      case IIS3DWB10IS_DRDY_LEVEL:
        *val = IIS3DWB10IS_DRDY_LEVEL;
        break;

      case IIS3DWB10IS_DRDY_PULSED:
        *val = IIS3DWB10IS_DRDY_PULSED;
        break;

      default:
        *val = IIS3DWB10IS_DRDY_LEVEL;
        break;
    }
  }

  return ret;
}

/**
  * @brief  Configure interrupt routing on INT1 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_pin_int_route_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                      iis3dwb10is_pin_int_route_t val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  iis3dwb10is_int_ctrl2_t int_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
  if (ret == 0)
  {
    int_ctrl1.int1_boot        = val.boot;
    int_ctrl1.int1_drdy_temp   = val.drdy_temp;
    int_ctrl2.int1_drdy_xl     = val.drdy_xl;
    int_ctrl2.int1_ext_trig    = val.ext_trig;
    int_ctrl2.int1_fifo_th     = val.fifo_th;
    int_ctrl2.int1_fifo_ovr    = val.fifo_ovr;
    int_ctrl2.int1_fifo_full   = val.fifo_full;
    int_ctrl2.int1_sleepcnt    = val.sleep_cnt;

    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_INT_CTRL2, (uint8_t *)&int_ctrl2, 1);
  }

  return ret;
}

/**
  * @brief  Configure interrupt routing on INT1 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pinter to iis3dwb10is_pin_int_route_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                       iis3dwb10is_pin_int_route_t *val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  iis3dwb10is_int_ctrl2_t int_ctrl2;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL2, (uint8_t *)&int_ctrl2, 1);
  if (ret == 0)
  {
    val->boot       = int_ctrl1.int1_boot;
    val->drdy_temp  = int_ctrl1.int1_drdy_temp;
    val->drdy_xl    = int_ctrl2.int1_drdy_xl;
    val->ext_trig   = int_ctrl2.int1_ext_trig;
    val->fifo_th    = int_ctrl2.int1_fifo_th;
    val->fifo_ovr   = int_ctrl2.int1_fifo_ovr;
    val->fifo_full  = int_ctrl2.int1_fifo_full;
    val->sleep_cnt  = int_ctrl2.int1_sleepcnt;
  }

  return ret;
}

/**
  * @brief  Configure interrupt routing on INT2 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      iis3dwb10is_pin_int_route_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                      iis3dwb10is_pin_int_route_t val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  iis3dwb10is_int_ctrl3_t int_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
  if (ret == 0)
  {
    int_ctrl1.int2_drdy_temp   = val.drdy_temp;
    int_ctrl1.int2_sleep_ispu  = val.sleep_ispu;
    int_ctrl1.int2_on_int1     = val.int2_on_int1;
    int_ctrl3.int2_drdy_xl     = val.drdy_xl;
    int_ctrl3.int2_fifo_th     = val.fifo_th;
    int_ctrl3.int2_fifo_ovr    = val.fifo_ovr;
    int_ctrl3.int2_fifo_full   = val.fifo_full;
    int_ctrl3.int2_sleepcnt    = val.sleep_cnt;

    ret = iis3dwb10is_write_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
    ret += iis3dwb10is_write_reg(ctx, IIS3DWB10IS_INT_CTRL3, (uint8_t *)&int_ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Configure interrupt routing on INT2 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pinter to iis3dwb10is_pin_int_route_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                       iis3dwb10is_pin_int_route_t *val)
{
  iis3dwb10is_int_ctrl1_t int_ctrl1;
  iis3dwb10is_int_ctrl3_t int_ctrl3;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL1, (uint8_t *)&int_ctrl1, 1);
  ret += iis3dwb10is_read_reg(ctx, IIS3DWB10IS_INT_CTRL3, (uint8_t *)&int_ctrl3, 1);
  if (ret == 0)
  {
    val->drdy_temp  = int_ctrl1.int2_drdy_temp;
    val->sleep_ispu = int_ctrl1.int2_sleep_ispu;
    val->int2_on_int1   = int_ctrl1.int2_on_int1;
    val->drdy_xl    = int_ctrl3.int2_drdy_xl;
    val->fifo_th    = int_ctrl3.int2_fifo_th;
    val->fifo_ovr   = int_ctrl3.int2_fifo_ovr;
    val->fifo_full  = int_ctrl3.int2_fifo_full;
    val->sleep_cnt  = int_ctrl3.int2_sleepcnt;
  }

  return ret;
}

/**
  * @brief  return data ready status flag.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      pinter to iis3dwb10is_data_ready_t
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t iis3dwb10is_data_ready_get(const stmdev_ctx_t *ctx, iis3dwb10is_data_ready_t *val)
{
  iis3dwb10is_status_reg_t status;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_STATUS_REG, (uint8_t *)&status, 1);
  val->drdy_xl   = status.xlda;
  val->drdy_temp = status.tda;
  val->ext_trig_ia = status.ext_trig_ia;
  val->sleepcnt_ia = status.sleepcnt_ia;
  val->timestamp_endcount = status.timestamp_endcount;
  val->ispu_ia = status.ispu_ia;

  return ret;
}

/**
  * @brief  Temperature data output register (r).
  *         L and H registers together express a 16-bit word in two’s
  *         complement.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t iis3dwb10is_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];

  const int32_t ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_OUT_TEMP_L, buff, 2);
  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  Linear acceleration output register. The value is expressed as a
  *         16-bit word in two's complement.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */

int32_t iis3dwb10is_acceleration_16b_raw_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  uint8_t i;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_OUTX_L_A, buff, 6);
  for (i = 0U; i < 3U; i++)
  {
    val[i] = (int16_t)buff[2U * i + 1U];
    val[i] = (val[i] * 256) + (int16_t)buff[2U * i];
  }

  return ret;
}

/**
  * @brief  Linear acceleration output register. The value is expressed as a
  *         20-bit word in two's complement.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t iis3dwb10is_acceleration_raw_get(const stmdev_ctx_t *ctx, int32_t *val)
{
  uint8_t buff[12];
  uint8_t i;
  int32_t ret;

  ret = iis3dwb10is_read_reg(ctx, IIS3DWB10IS_OUTX_L_A, buff, 12);
  for (i = 0U; i < 3U; i++)
  {
    val[i] = (int32_t)buff[4U * i + 3U];
    val[i] = (val[i] * 256) + (int32_t)buff[4U * i + 2U];
    val[i] = (val[i] * 256) + (int32_t)buff[4U * i + 1U];
    val[i] = (val[i] * 256) + (int32_t)buff[4U * i];
  }

  return ret;
}

/**
  * @}
  *
  */
