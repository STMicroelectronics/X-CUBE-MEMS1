/*
 ******************************************************************************
 * @file    lis2du12_reg.c
 * @author  Sensors Software Solution Team
 * @brief   LIS2DU12 driver file
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

#include "lis2du12_reg.h"

/**
  * @defgroup    LIS2DU12
  * @brief       This file provides a set of functions needed to drive the
  *              lis2du12 sensor.
  * @{
  *
  */

/**
  * @defgroup    LIS2DU12_Interfaces_Functions
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
int32_t lis2du12_read_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                          uint16_t len)
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
int32_t lis2du12_write_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
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
  * @defgroup  LIS2DU12_Private_functions
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
  * @defgroup    LIS2DU12_Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lis2du12_from_fs2g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.061f;
}

float_t lis2du12_from_fs4g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.122f;
}

float_t lis2du12_from_fs8g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.244f;
}

float_t lis2du12_from_fs16g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.488f;
}

float_t lis2du12_from_lsb_to_celsius(int16_t lsb)
{
  return ((float_t)lsb / 355.5f) + 25.0f;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Basic functions
  * @brief     This section groups all the functions concerning
  *            device basic configuration.
  * @{
  *
  */

/**
  * @brief  Device "Who am I".[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   ID values.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_id_get(stmdev_ctx_t *ctx, lis2du12_id_t *val)
{
  uint8_t reg;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_WHO_AM_I, &reg, 1);
  val->whoami = reg;

  return ret;
}

/**
  * @brief  Configures the bus operating mode.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_bus_mode_set(stmdev_ctx_t *ctx, lis2du12_bus_mode_t val)
{
  lis2du12_if_ctrl_t if_ctrl;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  if (ret == 0)
  {
    if_ctrl.i3c_disable = (uint8_t)val & 0x01U;
    if_ctrl.i2c_disable = ((uint8_t)val & 0x02U) >> 1;
    ret = lis2du12_write_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    ctrl1.sim = ((uint8_t)val & 0x04U) >> 2;
    ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  return ret;

}

/**
  * @brief  Configures the bus operating mode.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_bus_mode_get(stmdev_ctx_t *ctx, lis2du12_bus_mode_t *val)
{
  lis2du12_if_ctrl_t if_ctrl;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  switch ((ctrl1.sim << 2) | (if_ctrl.i2c_disable) << 1 |
          (if_ctrl.i3c_disable))
  {
    case LIS2DU12_SEL_BY_HW:
      *val = LIS2DU12_SEL_BY_HW;
      break;
    case LIS2DU12_SPI_3W:
      *val = LIS2DU12_SPI_3W;
      break;
    default:
      *val = LIS2DU12_SEL_BY_HW;
      break;
  }

  return ret;
}

/**
  * @brief  Configures the bus operating mode.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_init_set(stmdev_ctx_t *ctx, lis2du12_init_t val)
{
  lis2du12_ctrl1_t ctrl1;
  lis2du12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
  switch (val)
  {
    case LIS2DU12_BOOT:
      ctrl4.boot = PROPERTY_ENABLE;
      ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
      break;
    case LIS2DU12_RESET:

      ctrl1.sw_reset = PROPERTY_ENABLE;
      ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
      break;
    case LIS2DU12_DRV_RDY:
      ctrl4.bdu = PROPERTY_ENABLE;
      ctrl1.if_add_inc = PROPERTY_ENABLE;
      ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
      if (ret == 0)
      {
        ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
      }
      break;
    default:
      ctrl1.sw_reset = PROPERTY_ENABLE;
      ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
      break;
  }
  return ret;
}

/**
  * @brief  Get the status of the device.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the status of the device.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_status_get(stmdev_ctx_t *ctx, lis2du12_status_t *val)
{
  lis2du12_status_register_t status_register;
  lis2du12_ctrl1_t ctrl1;
  lis2du12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_STATUS,
                          (uint8_t *)&status_register, 1);
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  val->sw_reset = ctrl1.sw_reset;
  val->boot     = ctrl4.boot;
  val->drdy_xl  = status_register.drdy;
  val->power_down  = status_register.pd_status;

  return ret;
}

/**
  * @brief  Electrical pin configuration.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the electrical settings for the configurable pins.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_conf_set(stmdev_ctx_t *ctx, lis2du12_pin_conf_t *val)
{
  lis2du12_if_pu_ctrl_t if_pu_ctrl;
  lis2du12_md2_cfg_t md2_cfg;
  lis2du12_if_ctrl_t if_ctrl;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_IF_PU_CTRL, (uint8_t *)&if_pu_ctrl, 1);
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  if (ret == 0)
  {
    if_pu_ctrl.sdo_pu_disc = ~val->sdo_pull_up;
    if_pu_ctrl.sda_pu_en = val->sda_pull_up;
    if_pu_ctrl.cs_pu_disc = ~val->cs_pull_up;
    ret = lis2du12_write_reg(ctx, LIS2DU12_IF_PU_CTRL, (uint8_t *)&if_pu_ctrl, 1);
  }
  if (ret == 0)
  {
    if_ctrl.pd_dis_int1 = val->int1_pull_down;
    ret = lis2du12_write_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ctrl1.pp_od = val->int1_int2_push_pull;
    ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    md2_cfg.pd_dis_int2 = val->int2_pull_down;
    ret = lis2du12_write_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Electrical pin configuration.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the electrical settings for the configurable pins.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_conf_get(stmdev_ctx_t *ctx, lis2du12_pin_conf_t *val)
{
  lis2du12_if_pu_ctrl_t if_pu_ctrl;
  lis2du12_md2_cfg_t md2_cfg;
  lis2du12_if_ctrl_t if_ctrl;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_IF_PU_CTRL, (uint8_t *)&if_pu_ctrl, 1);
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }
  val->sdo_pull_up = ~if_pu_ctrl.sdo_pu_disc;
  val->sda_pull_up = if_pu_ctrl.sda_pu_en;
  val->cs_pull_up = ~if_pu_ctrl.cs_pu_disc;
  val->int1_int2_push_pull = ctrl1.pp_od;
  val->int1_pull_down = if_ctrl.pd_dis_int1;
  val->int2_pull_down = md2_cfg.pd_dis_int2;

  return ret;
}

/**
  * @brief  Get the status of all the interrupt sources.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the status of all the interrupt sources.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_all_sources_get(stmdev_ctx_t *ctx,
                                 lis2du12_all_sources_t *val)
{
  lis2du12_status_register_t status_register;
  lis2du12_fifo_status1_t fifo_status1;
  lis2du12_fifo_status2_t fifo_status2;
  lis2du12_all_int_src_t all_int_src;
  lis2du12_wake_up_src_t wake_up_src;
  lis2du12_sixd_src_t sixd_src;
  lis2du12_tap_src_t tap_src;
  uint8_t reg[7];
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_SRC, (uint8_t *)&reg, 7);

  bytecpy((uint8_t *)&wake_up_src, &reg[0]);
  bytecpy((uint8_t *)&tap_src, &reg[1]);
  bytecpy((uint8_t *)&sixd_src, &reg[2]);
  bytecpy((uint8_t *)&all_int_src, &reg[3]);
  bytecpy((uint8_t *)&status_register, &reg[4]);
  bytecpy((uint8_t *)&fifo_status1, &reg[5]);
  bytecpy((uint8_t *)&fifo_status2, &reg[6]);

  val->base_int     = all_int_src.int_global;
  val->drdy_xl      = status_register.drdy;
  val->free_fall    = all_int_src.ff_ia_all;
  val->wake_up      = all_int_src.wu_ia_all;
  val->wake_up_z    = wake_up_src.z_wu;
  val->wake_up_y    = wake_up_src.y_wu;
  val->wake_up_x    = wake_up_src.x_wu;
  val->single_tap   = all_int_src.single_tap_all;
  val->double_tap   = all_int_src.double_tap_all;
  val->tap_z        = tap_src.z_tap;
  val->tap_y        = tap_src.y_tap;
  val->tap_x        = tap_src.x_tap;
  val->tap_sign     = tap_src.tap_ia;
  val->six_d        = all_int_src.sixd_ia_all;
  val->six_d_xl     = sixd_src.xl;
  val->six_d_xh     = sixd_src.xh;
  val->six_d_yl     = sixd_src.yl;
  val->six_d_yh     = sixd_src.yh;
  val->six_d_zl     = sixd_src.zl;
  val->six_d_zh     = sixd_src.zh;
  val->sleep_change = wake_up_src.sleep_change_ia;
  val->sleep_state  = wake_up_src.sleep_state;
  val->fifo_full    = (fifo_status2.fss & 0x80U) >> 7;
  val->fifo_ovr     = fifo_status1.fifo_ovr;
  val->fifo_th      = fifo_status1.fth;

  return ret;
}

/**
  * @brief  Sensor conversion parameters selection.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   set the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_mode_set(stmdev_ctx_t *ctx, lis2du12_md_t *val)
{
  lis2du12_ctrl5_t ctrl5;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL5, (uint8_t *)&ctrl5, 1);

  ctrl5.odr = (uint8_t)val->odr;
  ctrl5.fs = (uint8_t)val->fs;
  ctrl5.bw = (uint8_t)val->bw;

  if (ret == 0)
  {
    ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL5, (uint8_t *)&ctrl5, 1);
  }

  return ret;
}

/**
  * @brief  Sensor conversion parameters selection.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   get the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_mode_get(stmdev_ctx_t *ctx, lis2du12_md_t *val)
{
  lis2du12_ctrl5_t ctrl5;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL5, (uint8_t *)&ctrl5, 1);

  switch (ctrl5.odr)
  {
    case LIS2DU12_OFF:
      val->odr = LIS2DU12_OFF;
      break;
    case LIS2DU12_1Hz5_ULP:
      val->odr = LIS2DU12_1Hz5_ULP;
      break;
    case LIS2DU12_3Hz_ULP:
      val->odr = LIS2DU12_3Hz_ULP;
      break;
    case LIS2DU12_6Hz_ULP:
      val->odr = LIS2DU12_6Hz_ULP;
      break;
    case LIS2DU12_6Hz:
      val->odr = LIS2DU12_6Hz;
      break;
    case LIS2DU12_12Hz5:
      val->odr = LIS2DU12_12Hz5;
      break;
    case LIS2DU12_25Hz:
      val->odr = LIS2DU12_25Hz;
      break;
    case LIS2DU12_50Hz:
      val->odr = LIS2DU12_50Hz;
      break;
    case LIS2DU12_100Hz:
      val->odr = LIS2DU12_100Hz;
      break;
    case LIS2DU12_200Hz:
      val->odr = LIS2DU12_200Hz;
      break;
    case LIS2DU12_400Hz:
      val->odr = LIS2DU12_400Hz;
      break;
    case LIS2DU12_800Hz:
      val->odr = LIS2DU12_800Hz;
      break;
    case LIS2DU12_TRIG_PIN:
      val->odr = LIS2DU12_TRIG_PIN;
      break;
    case LIS2DU12_TRIG_SW:
      val->odr = LIS2DU12_TRIG_SW;
      break;
    default:
      val->odr = LIS2DU12_OFF;
      break;
  }

  switch (ctrl5.fs)
  {
    case LIS2DU12_2g:
      val->fs = LIS2DU12_2g;
      break;
    case LIS2DU12_4g:
      val->fs = LIS2DU12_4g;
      break;
    case LIS2DU12_8g:
      val->fs = LIS2DU12_8g;
      break;
    case LIS2DU12_16g:
      val->fs = LIS2DU12_16g;
      break;
    default:
      val->fs = LIS2DU12_2g;
      break;
  }

  switch (ctrl5.bw)
  {
    case LIS2DU12_ODR_div_2:
      val->bw = LIS2DU12_ODR_div_2;
      break;
    case LIS2DU12_ODR_div_4:
      val->bw = LIS2DU12_ODR_div_4;
      break;
    case LIS2DU12_ODR_div_8:
      val->bw = LIS2DU12_ODR_div_8;
      break;
    case LIS2DU12_ODR_div_16:
      val->bw = LIS2DU12_ODR_div_16;
      break;
    default:
      val->bw = LIS2DU12_ODR_div_2;
      break;
  }

  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_trigger_sw(stmdev_ctx_t *ctx, lis2du12_md_t *md)
{
  lis2du12_ctrl4_t ctrl4;
  int32_t ret = 0;

  if (md->odr == LIS2DU12_TRIG_SW)
  {
    ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
    ctrl4.soc = PROPERTY_ENABLE;
    if (ret == 0)
    {
      ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);
    }
  }
  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrieved from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_data_get(stmdev_ctx_t *ctx, lis2du12_md_t *md,
                          lis2du12_data_t *data)
{
  uint8_t buff[8];
  int32_t ret;
  uint8_t i;
  uint8_t j;

  ret = lis2du12_read_reg(ctx, LIS2DU12_OUTX_L, (uint8_t *)&buff, 8);

  /* acceleration conversion */
  j = 0U;
  for (i = 0U; i < 3U; i++)
  {
    data->xl.raw[i] = (int16_t)buff[j + 1U];
    data->xl.raw[i] = (data->xl.raw[i] * 256) + (int16_t) buff[j];
    j += 2U;
    switch (md->fs)
    {
      case LIS2DU12_2g:
        data->xl.mg[i] = lis2du12_from_fs2g_to_mg(data->xl.raw[i]);
        break;
      case LIS2DU12_4g:
        data->xl.mg[i] = lis2du12_from_fs4g_to_mg(data->xl.raw[i]);
        break;
      case LIS2DU12_8g:
        data->xl.mg[i] = lis2du12_from_fs8g_to_mg(data->xl.raw[i]);
        break;
      case LIS2DU12_16g:
        data->xl.mg[i] = lis2du12_from_fs16g_to_mg(data->xl.raw[i]);
        break;
      default:
        data->xl.mg[i] = 0.0f;
        break;
    }
  }

  data->heat.raw = (int16_t)buff[j + 1U];
  data->heat.raw = (data->heat.raw * 256) + (int16_t) buff[j];
  /* temperature conversion */
  data->heat.deg_c = lis2du12_from_lsb_to_celsius(data->heat.raw);

  return ret;
}

/**
  * @brief  Configures the self test.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   self test mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_self_test_sign_set(stmdev_ctx_t *ctx, lis2du12_st_t val)
{
  lis2du12_st_sign_t st_sign;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_ST_SIGN, (uint8_t *)&st_sign, 1);
  if (ret == 0)
  {
    st_sign.stsign = (uint8_t) val;
    ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&st_sign, 1);
  }
  return ret;
}

/**
  * @brief  Configures the self test.[start]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   valid values 2 (1st step) or 1 (2nd step)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_self_test_start(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2du12_ctrl3_t ctrl3;
  int32_t ret;

  if (val != 1 && val != 2)
  {
    return -1;
  }

  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&ctrl3, 1);
  if (ret == 0)
  {
    ctrl3.st = (uint8_t) val;
    ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&ctrl3, 1);
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     FIFO functions
  * @brief        This section groups all the functions concerning the
  *               management of FIFO.
  * @{
  *
  */

/**
  * @brief  FIFO operation mode selection.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   set the FIFO operation mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_fifo_mode_set(stmdev_ctx_t *ctx, lis2du12_fifo_md_t *val)
{
  lis2du12_fifo_ctrl_t fifo_ctrl;
  lis2du12_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_FIFO_CTRL, reg, 2);

  bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
  bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

  fifo_ctrl.f_mode = (uint8_t) val->operation;
  fifo_ctrl.fifo_depth = (uint8_t) val->store;

  if (val->watermark != 0x00U)
  {
    fifo_ctrl.stop_on_fth = PROPERTY_ENABLE;
  }
  else
  {
    fifo_ctrl.stop_on_fth = PROPERTY_DISABLE;
  }

  fifo_wtm.fth = val->watermark;

  bytecpy(&reg[0], (uint8_t *)&fifo_ctrl);
  bytecpy(&reg[1], (uint8_t *)&fifo_wtm);

  ret = lis2du12_write_reg(ctx, LIS2DU12_FIFO_CTRL, reg, 2);

  return ret;
}

/**
  * @brief  FIFO operation mode selection.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   get the FIFO operation mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_fifo_mode_get(stmdev_ctx_t *ctx, lis2du12_fifo_md_t *val)
{
  lis2du12_fifo_ctrl_t fifo_ctrl;
  lis2du12_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_FIFO_CTRL, reg, 2);

  bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
  bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

  switch (fifo_ctrl.f_mode)
  {
    case LIS2DU12_BYPASS:
      val->operation = LIS2DU12_BYPASS;
      break;
    case LIS2DU12_FIFO:
      val->operation = LIS2DU12_FIFO;
      break;
    case LIS2DU12_STREAM:
      val->operation = LIS2DU12_STREAM;
      break;
    case LIS2DU12_STREAM_TO_FIFO:
      val->operation = LIS2DU12_STREAM_TO_FIFO;
      break;
    case LIS2DU12_BYPASS_TO_STREAM:
      val->operation = LIS2DU12_BYPASS_TO_STREAM;
      break;
    case LIS2DU12_BYPASS_TO_FIFO:
      val->operation = LIS2DU12_BYPASS_TO_FIFO;
      break;
    default:
      val->operation = LIS2DU12_BYPASS;
      break;
  }

  switch (fifo_ctrl.fifo_depth)
  {
    case LIS2DU12_16_BIT:
      val->store = LIS2DU12_16_BIT;
      break;
    case LIS2DU12_8_BIT:
      val->store = LIS2DU12_8_BIT;
      break;
    default:
      val->store = LIS2DU12_16_BIT;
      break;
  }

  val->watermark = fifo_wtm.fth;

  return ret;
}

/**
  * @brief  Get the number of samples stored in FIFO.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  val   number of samples stored in FIFO.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_fifo_level_get(stmdev_ctx_t *ctx, lis2du12_fifo_md_t *md,
                                uint8_t *val)
{
  lis2du12_fifo_status2_t fifo_status2;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_FIFO_STATUS2,
                          (uint8_t *)&fifo_status2, 1);

  *val = fifo_status2.fss;

  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  fmd   get the FIFO operation mode.(ptr)
  * @param  data  data retrieved from FIFO.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_fifo_data_get(stmdev_ctx_t *ctx, lis2du12_md_t *md,
                               lis2du12_fifo_md_t *fmd,
                               lis2du12_fifo_data_t *data)
{
  uint8_t fifo_data[8];
  uint8_t i;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_OUTX_L, fifo_data, 8);

  if (fmd->store == LIS2DU12_8_BIT)
  {
    for (i = 0U; i < 3; i++)
    {
      data->xl[0].raw[i] = (int16_t)fifo_data[2 * i + 1U];
      data->xl[0].raw[i] = data->xl[0].raw[i] * 256 + (int16_t)fifo_data[2 * i];
    }

    data->heat.raw = (int16_t)fifo_data[7U];
    data->heat.raw = (data->heat.raw * 256) + (int16_t) fifo_data[6U];
    /* temperature conversion */
    data->heat.deg_c = lis2du12_from_lsb_to_celsius(data->heat.raw);
  }
  else
  {
    for (i = 0U; i < 3; i++)
    {
      data->xl[0].raw[i] = (int16_t)fifo_data[i] * 256;
      data->xl[1].raw[i] = (int16_t)fifo_data[3 + i] * 256;
    }
  }

  for (i = 0U; i < 3; i++)
  {
    switch (md->fs)
    {
      case LIS2DU12_2g:
        data->xl[0].mg[i] = lis2du12_from_fs2g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2du12_from_fs2g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DU12_4g:
        data->xl[0].mg[i] = lis2du12_from_fs4g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2du12_from_fs4g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DU12_8g:
        data->xl[0].mg[i] = lis2du12_from_fs8g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2du12_from_fs8g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DU12_16g:
        data->xl[0].mg[i] = lis2du12_from_fs16g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2du12_from_fs16g_to_mg(data->xl[1].raw[i]);
        break;
      default:
        data->xl[0].mg[i] = 0.0f;
        data->xl[1].mg[i] = 0.0f;
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
  * @defgroup     Interrupt signals
  * @brief        This section groups all the functions concerning
  *               the management of interrupt signals.
  * @{
  *
  */

/**
  * @brief  Interrupt pins hardware signal configuration.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the pins hardware signal settings.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_interrupt_mode_set(stmdev_ctx_t *ctx,
                                    lis2du12_int_mode_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);

  interrupt_cfg.int_short_en = (uint8_t)val->base_sig & 0x01U;
  interrupt_cfg.lir = ((uint8_t)val->base_sig & 0x02U) >> 1 ;


  interrupt_cfg.h_lactive = val->active_low;
  ctrl1.drdy_pulsed = ~val->drdy_latched;

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);

  return ret;
}

/**
  * @brief  Interrupt pins hardware signal configuration.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the pins hardware signal settings.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_interrupt_mode_get(stmdev_ctx_t *ctx,
                                    lis2du12_int_mode_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);

  val->active_low = interrupt_cfg.h_lactive;
  val->drdy_latched = ~ctrl1.drdy_pulsed;

  switch ((interrupt_cfg.lir << 1) | interrupt_cfg.int_short_en)
  {
    case LIS2DU12_INT_LEVEL:
      val->base_sig = LIS2DU12_INT_LEVEL;
      break;
    case LIS2DU12_INT_PULSED:
      val->base_sig = LIS2DU12_INT_PULSED;
      break;
    case LIS2DU12_INT_LATCHED:
      val->base_sig = LIS2DU12_INT_LATCHED;
      break;
    default:
      val->base_sig = LIS2DU12_INT_LEVEL;
      break;
  }
  return ret;
}

/**
  * @brief  Route interrupt signals on int1 pin.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the signals to route on int1 pin.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_int1_route_set(stmdev_ctx_t *ctx,
                                    lis2du12_pin_int1_route_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_md1_cfg_t md1_cfg;
  lis2du12_ctrl2_t ctrl2;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL2, (uint8_t *)&ctrl2, 1);

  ctrl2.int1_boot = val->boot;
  ctrl2.int1_drdy = val->drdy_xl;
  ctrl2.int1_f_fth = val->fifo_th;
  ctrl2.int1_f_ovr = val->fifo_ovr;
  ctrl2.int1_f_full = val->fifo_full;

  md1_cfg.int1_tap = val->tap;
  md1_cfg.int1_6d = val->six_d;
  md1_cfg.int1_wu = val->wake_up;
  md1_cfg.int1_ff = val->free_fall;
  md1_cfg.int1_single_tap = val->single_tap;

  if (val->sleep_state)
  {
    interrupt_cfg.sleep_status_on_int = PROPERTY_ENABLE;
    md1_cfg.int1_sleep_change = PROPERTY_ENABLE;
  }
  if (val->sleep_change)
  {
    interrupt_cfg.sleep_status_on_int = PROPERTY_DISABLE;
    md1_cfg.int1_sleep_change = PROPERTY_ENABLE;
  }

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL2, (uint8_t *)&ctrl2, 1);

  return ret;
}

/**
  * @brief  Route interrupt signals on int1 pin.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the signals that are routed on int1 pin.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_int1_route_get(stmdev_ctx_t *ctx,
                                    lis2du12_pin_int1_route_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_md1_cfg_t md1_cfg;
  lis2du12_ctrl2_t ctrl2;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL2, (uint8_t *)&ctrl2, 1);

  val->boot = ctrl2.int1_boot;
  val->drdy_xl = ctrl2.int1_drdy;
  val->fifo_th = ctrl2.int1_f_fth;
  val->fifo_ovr = ctrl2.int1_f_ovr;
  val->fifo_full = ctrl2.int1_f_full;

  val->tap = md1_cfg.int1_tap;
  val->six_d = md1_cfg.int1_6d;
  val->wake_up = md1_cfg.int1_wu;
  val->free_fall = md1_cfg.int1_ff;
  val->single_tap = md1_cfg.int1_single_tap;

  val->sleep_state = interrupt_cfg.sleep_status_on_int;

  if (val->sleep_state == PROPERTY_DISABLE)
  {
    val->sleep_change = md1_cfg.int1_sleep_change;
  }
  else
  {
    val->sleep_change = PROPERTY_DISABLE;
  }

  return ret;
}

/**
  * @brief  Route interrupt signals on int2 pin.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the signals to route on int2 pin.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_int2_route_set(stmdev_ctx_t *ctx,
                                    lis2du12_pin_int2_route_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_md2_cfg_t md2_cfg;
  lis2du12_ctrl3_t ctrl3;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&ctrl3, 1);

  ctrl3.int2_boot = val->boot;
  ctrl3.int2_drdy = val->drdy_xl;
  ctrl3.int2_f_fth = val->fifo_th;
  ctrl3.int2_f_ovr = val->fifo_ovr;
  ctrl3.int2_f_full = val->fifo_full;

  md2_cfg.int2_tap = val->tap;
  md2_cfg.int2_6d = val->six_d;
  md2_cfg.int2_wu = val->wake_up;
  md2_cfg.int2_ff = val->free_fall;
  md2_cfg.int2_single_tap = val->single_tap;

  if (val->sleep_state)
  {
    interrupt_cfg.sleep_status_on_int = PROPERTY_ENABLE;
    md2_cfg.int2_sleep_change = PROPERTY_ENABLE;
  }
  if (val->sleep_change)
  {
    interrupt_cfg.sleep_status_on_int = PROPERTY_DISABLE;
    md2_cfg.int2_sleep_change = PROPERTY_ENABLE;
  }

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&ctrl3, 1);

  return ret;
}

/**
  * @brief  Route interrupt signals on int2 pin.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the signals that are routed on int2 pin.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_pin_int2_route_get(stmdev_ctx_t *ctx,
                                    lis2du12_pin_int2_route_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_md2_cfg_t md2_cfg;
  lis2du12_ctrl3_t ctrl3;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL3, (uint8_t *)&ctrl3, 1);

  val->boot = ctrl3.int2_boot;
  val->drdy_xl = ctrl3.int2_drdy;
  val->fifo_th = ctrl3.int2_f_fth;
  val->fifo_ovr = ctrl3.int2_f_ovr;
  val->fifo_full = ctrl3.int2_f_full;

  val->tap = md2_cfg.int2_tap;
  val->six_d = md2_cfg.int2_6d;
  val->wake_up = md2_cfg.int2_wu;
  val->free_fall = md2_cfg.int2_ff;
  val->single_tap = md2_cfg.int2_single_tap;

  val->sleep_state = interrupt_cfg.sleep_status_on_int;

  if (val->sleep_state == PROPERTY_DISABLE)
  {
    val->sleep_change = md2_cfg.int2_sleep_change;
  }
  else
  {
    val->sleep_change = PROPERTY_DISABLE;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Wakeup functions
  * @brief        This section groups all the functions concerning
  *               the wake up functionality.
  * @{
  *
  */

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_wake_up_mode_set(stmdev_ctx_t *ctx, lis2du12_wkup_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_ths_t wake_up_ths;
  lis2du12_wake_up_dur_t wake_up_dur;
  lis2du12_md1_cfg_t md1_cfg;
  lis2du12_ctrl1_t ctrl1;
  lis2du12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_THS,
                          (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                          (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);

  ctrl1.wu_z_en = val->z_en;
  ctrl1.wu_y_en = val->y_en;
  ctrl1.wu_x_en = val->x_en;

  if (val->threshold > 63U)
  {
    interrupt_cfg.wake_ths_w = PROPERTY_ENABLE;
    wake_up_ths.wk_ths = val->threshold / 4;
  }
  else
  {
    interrupt_cfg.wake_ths_w = PROPERTY_DISABLE;
    wake_up_ths.wk_ths = val->threshold;
  }

  if (val->duration > 3U)
  {
    md1_cfg.wu_dur_x4 = PROPERTY_ENABLE;
    wake_up_dur.wake_dur = val->duration / 4;
  }
  else
  {
    md1_cfg.wu_dur_x4 = PROPERTY_DISABLE;
    wake_up_dur.wake_dur = val->duration;
  }

  wake_up_ths.sleep_on = val->sleep.en;
  ctrl4.inact_odr = (uint8_t)val->sleep.odr;
  wake_up_dur.sleep_dur = val->sleep.duration;

  interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_WAKE_UP_THS,
                           (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                           (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);

  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   retrieve the parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_wake_up_mode_get(stmdev_ctx_t *ctx, lis2du12_wkup_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_ths_t wake_up_ths;
  lis2du12_wake_up_dur_t wake_up_dur;
  lis2du12_md1_cfg_t md1_cfg;
  lis2du12_ctrl1_t ctrl1;
  lis2du12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_THS,
                          (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                          (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_CTRL4, (uint8_t *)&ctrl4, 1);

  val->z_en = ctrl1.wu_z_en;
  val->y_en = ctrl1.wu_y_en;
  val->x_en = ctrl1.wu_x_en;

  if (interrupt_cfg.wake_ths_w == PROPERTY_ENABLE)
  {
    val->threshold = wake_up_ths.wk_ths * 4;
  }
  else
  {
    val->threshold = wake_up_ths.wk_ths;
  }

  if (md1_cfg.wu_dur_x4 == PROPERTY_ENABLE)
  {
    val->duration = wake_up_dur.wake_dur * 4;
  }
  else
  {
    val->duration = wake_up_dur.wake_dur;
  }

  val->sleep.en = wake_up_ths.sleep_on;
  val->sleep.duration = wake_up_dur.sleep_dur;

  switch (ctrl4.inact_odr)
  {
    case LIS2DU12_DO_NOT_CHANGE:
      val->sleep.odr = LIS2DU12_DO_NOT_CHANGE;
      break;
    case LIS2DU12_SLEEP_AT_6Hz:
      val->sleep.odr = LIS2DU12_SLEEP_AT_6Hz;
      break;
    case LIS2DU12_SLEEP_AT_3Hz:
      val->sleep.odr = LIS2DU12_SLEEP_AT_3Hz;
      break;
    case LIS2DU12_SLEEP_AT_1Hz5:
      val->sleep.odr = LIS2DU12_SLEEP_AT_1Hz5;
      break;
    default:
      val->sleep.odr = LIS2DU12_DO_NOT_CHANGE;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    Tap functions
  * @brief       This section groups all the functions concerning
  *              the single/double tap functionality.
  * @{
  *
  */

/**
  * @brief  Configuration of Single and Double Tap.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_tap_mode_set(stmdev_ctx_t *ctx, lis2du12_tap_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_ths_t wake_up_ths;
  lis2du12_tap_ths_x_t tap_ths_x;
  lis2du12_tap_ths_y_t tap_ths_y;
  lis2du12_tap_ths_z_t tap_ths_z;
  lis2du12_int_dur_t int_dur;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_THS,
                          (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_X,
                          (uint8_t *)&tap_ths_x, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_Y, (uint8_t *)&tap_ths_y, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_Z, (uint8_t *)&tap_ths_z, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_INT_DUR, (uint8_t *)&int_dur, 1);

  tap_ths_z.tap_z_en = val->z_en;
  tap_ths_z.tap_y_en = val->y_en;
  tap_ths_z.tap_x_en = val->x_en;

  tap_ths_x.tap_ths_x = val->threshold.x;
  tap_ths_y.tap_ths_y = val->threshold.y;
  tap_ths_z.tap_ths_z = val->threshold.z;

  int_dur.shock = val->shock;
  int_dur.quiet = val->quiet;

  tap_ths_y.tap_priority = (uint8_t)val->priority;

  wake_up_ths.single_double_tap = ~val->tap_double.en;
  int_dur.latency = val->tap_double.latency;

  interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_WAKE_UP_THS,
                           (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_TAP_THS_X, (uint8_t *)&tap_ths_x, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_TAP_THS_Y, (uint8_t *)&tap_ths_y, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_TAP_THS_Z, (uint8_t *)&tap_ths_z, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_INT_DUR, (uint8_t *)&int_dur, 1);

  return ret;
}

/**
  * @brief  Configuration of Single and Double Tap.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   retrieve the parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_tap_mode_get(stmdev_ctx_t *ctx, lis2du12_tap_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_ths_t wake_up_ths;
  lis2du12_tap_ths_x_t tap_ths_x;
  lis2du12_tap_ths_y_t tap_ths_y;
  lis2du12_tap_ths_z_t tap_ths_z;
  lis2du12_int_dur_t int_dur;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_THS,
                          (uint8_t *)&wake_up_ths, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_X, (uint8_t *)&tap_ths_x, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_Y, (uint8_t *)&tap_ths_y, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_Z, (uint8_t *)&tap_ths_z, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_INT_DUR, (uint8_t *)&int_dur, 1);

  val->z_en = tap_ths_z.tap_z_en;
  val->y_en = tap_ths_z.tap_y_en;
  val->x_en = tap_ths_z.tap_x_en;

  val->threshold.x = tap_ths_x.tap_ths_x;
  val->threshold.y = tap_ths_y.tap_ths_y;
  val->threshold.z = tap_ths_z.tap_ths_z;

  val->shock = int_dur.shock;
  val->quiet = int_dur.quiet;

  switch (tap_ths_y.tap_priority)
  {
    case LIS2DU12_XYZ:
      val->priority = LIS2DU12_XYZ;
      break;
    case LIS2DU12_YXZ:
      val->priority = LIS2DU12_YXZ;
      break;
    case LIS2DU12_XZY:
      val->priority = LIS2DU12_XZY;
      break;
    case LIS2DU12_ZYX:
      val->priority = LIS2DU12_ZYX;
      break;
    case LIS2DU12_YZX:
      val->priority = LIS2DU12_YZX;
      break;
    case LIS2DU12_ZXY:
      val->priority = LIS2DU12_ZXY;
      break;
    default:
      val->priority = LIS2DU12_XYZ;
      break;
  }

  val->tap_double.en = ~wake_up_ths.single_double_tap;
  val->tap_double.latency = int_dur.latency;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Free Fall Configuration
  * @brief        This section groups all the functions concerning
  *               the free fall functionality.
  * @{
  *
  */

/**
  * @brief  Configuration Free Fall.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_free_fall_mode_set(stmdev_ctx_t *ctx, lis2du12_ff_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_dur_t wake_up_dur;
  lis2du12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                          (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_FREE_FALL, (uint8_t *)&free_fall, 1);

  wake_up_dur.ff_dur = val->duration & 0x1FU;
  free_fall.ff_dur = (val->duration) & 0x20U >> 5;

  free_fall.ff_ths = (uint8_t)val->threshold;

  interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                           (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_FREE_FALL, (uint8_t *)&free_fall, 1);

  return ret;
}

/**
  * @brief  Configuration Free Fall.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   retrieve the parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_free_fall_mode_get(stmdev_ctx_t *ctx, lis2du12_ff_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_wake_up_dur_t wake_up_dur;
  lis2du12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_WAKE_UP_DUR,
                          (uint8_t *)&wake_up_dur, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_FREE_FALL, (uint8_t *)&free_fall, 1);

  val->duration = (free_fall.ff_dur * 32) + wake_up_dur.ff_dur;

  switch (free_fall.ff_ths)
  {
    case LIS2DU12_156mg:
      val->threshold = LIS2DU12_156mg;
      break;
    case LIS2DU12_219mg:
      val->threshold = LIS2DU12_219mg;
      break;
    case LIS2DU12_250mg:
      val->threshold = LIS2DU12_250mg;
      break;
    case LIS2DU12_312mg:
      val->threshold = LIS2DU12_312mg;
      break;
    case LIS2DU12_344mg:
      val->threshold = LIS2DU12_344mg;
      break;
    case LIS2DU12_406mg:
      val->threshold = LIS2DU12_406mg;
      break;
    case LIS2DU12_469mg:
      val->threshold = LIS2DU12_469mg;
      break;
    case LIS2DU12_500mg:
      val->threshold = LIS2DU12_500mg;
      break;
    default:
      val->threshold = LIS2DU12_156mg;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Orientation 6D functions
  * @brief        This section groups all the functions concerning
  *               the 6/4D orientation functionality.
  * @{
  *
  */

/**
  * @brief  Configuration of detection 6D or 4D orientation.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_orientation_mode_set(stmdev_ctx_t *ctx,
                                      lis2du12_orient_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_tap_ths_x_t tap_ths_x;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_X, (uint8_t *)&tap_ths_x, 1);

  tap_ths_x.d6d_ths = (uint8_t)val->threshold;
  tap_ths_x.d4d_en = (uint8_t)val->deg_of_freedom;

  interrupt_cfg.interrupts_enable = PROPERTY_ENABLE;

  ret = lis2du12_write_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_write_reg(ctx, LIS2DU12_TAP_THS_X, (uint8_t *)&tap_ths_x, 1);

  return ret;
}

/**
  * @brief  Configuration of detection 6D or 4D orientation.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   retrieve the parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2du12_orientation_mode_get(stmdev_ctx_t *ctx,
                                      lis2du12_orient_md_t *val)
{
  lis2du12_interrupt_cfg_t interrupt_cfg;
  lis2du12_tap_ths_x_t tap_ths_x;
  int32_t ret;

  ret = lis2du12_read_reg(ctx, LIS2DU12_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  ret = lis2du12_read_reg(ctx, LIS2DU12_TAP_THS_X, (uint8_t *)&tap_ths_x, 1);

  switch (tap_ths_x.d6d_ths)
  {
    case LIS2DU12_DEG_80:
      val->threshold = LIS2DU12_DEG_80;
      break;
    case LIS2DU12_DEG_70:
      val->threshold = LIS2DU12_DEG_70;
      break;
    case LIS2DU12_DEG_60:
      val->threshold = LIS2DU12_DEG_60;
      break;
    case LIS2DU12_DEG_50:
      val->threshold = LIS2DU12_DEG_50;
      break;
    default:
      val->threshold = LIS2DU12_DEG_80;
      break;
  }

  switch (tap_ths_x.d4d_en)
  {
    case LIS2DU12_SIX:
      val->deg_of_freedom = LIS2DU12_SIX;
      break;
    case LIS2DU12_FOUR:
      val->deg_of_freedom = LIS2DU12_FOUR;
      break;
    default:
      val->deg_of_freedom = LIS2DU12_SIX;
      break;
  }
  return ret;

}

/**
  * @}
  *
  */
