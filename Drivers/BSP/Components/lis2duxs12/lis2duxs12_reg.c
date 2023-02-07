/**
  ******************************************************************************
  * @file    lis2duxs12_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LIS2DUXS12 driver file
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

#include "lis2duxs12_reg.h"

/**
  * @defgroup    LIS2DUXS12
  * @brief       This file provides a set of functions needed to drive the
  *              lis2duxs12 sensor.
  * @{
  *
  */

/**
  * @defgroup    LIS2DUXS12_Interfaces_Functions
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
int32_t lis2duxs12_read_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
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
int32_t lis2duxs12_write_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
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
  * @defgroup    LIS2DUXS12_Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lis2duxs12_from_fs2g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.061f;
}

float_t lis2duxs12_from_fs4g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.122f;
}

float_t lis2duxs12_from_fs8g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.244f;
}

float_t lis2duxs12_from_fs16g_to_mg(int16_t lsb)
{
  return (float_t)lsb * 0.488f;
}

float_t lis2duxs12_from_lsb_to_celsius(int16_t lsb)
{
  return ((float_t)lsb / 355.5f) + 25.0f;
}

/**
  * @}
  *
  */

/**
  * @defgroup Common
  * @brief    Common
  * @{/
  *
  */
/**
  * @brief  Device ID.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Device ID.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_device_id_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WHO_AM_I, val, 1);

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
int32_t lis2duxs12_init_set(stmdev_ctx_t *ctx, lis2duxs12_init_t val)
{
  lis2duxs12_ctrl1_t ctrl1;
  lis2duxs12_ctrl4_t ctrl4;
  int32_t ret = 0;

  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  switch (val)
  {
    case LIS2DUXS12_BOOT:
      ctrl4.boot = PROPERTY_ENABLE;
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
      break;
    case LIS2DUXS12_RESET:

      ctrl1.sw_reset = PROPERTY_ENABLE;
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
      break;
    case LIS2DUXS12_SENSOR_ONLY_ON:
      /* no embedded funcs are used */
      ctrl4.emb_func_en = PROPERTY_DISABLE;
      ctrl4.bdu = PROPERTY_ENABLE;
      ctrl1.if_add_inc = PROPERTY_ENABLE;
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
      break;
    case LIS2DUXS12_SENSOR_EMB_FUNC_ON:
      /* complete configuration is used */
      ctrl4.emb_func_en = PROPERTY_ENABLE;
      ctrl4.bdu = PROPERTY_ENABLE;
      ctrl1.if_add_inc = PROPERTY_ENABLE;
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
      break;
    default:
      ctrl1.sw_reset = PROPERTY_ENABLE;
      ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
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
int32_t lis2duxs12_status_get(stmdev_ctx_t *ctx, lis2duxs12_status_t *val)
{
  lis2duxs12_status_register_t status_register;
  lis2duxs12_ctrl1_t ctrl1;
  lis2duxs12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_STATUS,
                            (uint8_t *)&status_register, 1);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
  }
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  val->sw_reset = ctrl1.sw_reset;
  val->boot     = ctrl4.boot;
  val->drdy     = status_register.drdy;
  //val->power_down  = status_register.pd_status;

  return ret;
}

/**
  * @brief  Get the status of the embedded funcs.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the status of the embedded funcs.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_embedded_status_get(stmdev_ctx_t *ctx, lis2duxs12_embedded_status_t *val)
{
  lis2duxs12_emb_func_status_t status;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_STATUS,
                              (uint8_t *)&status, 1);
  }

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  val->is_step_det = status.is_step_det;
  val->is_tilt = status.is_tilt;
  val->is_sigmot = status.is_sigmot;

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
int32_t lis2duxs12_data_ready_mode_set(stmdev_ctx_t *ctx, lis2duxs12_data_ready_mode_t val)
{
  lis2duxs12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);

  if (ret == 0)
  {
    ctrl1.drdy_pulsed = ((uint8_t)val & 0x1);
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
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
int32_t lis2duxs12_data_ready_mode_get(stmdev_ctx_t *ctx, lis2duxs12_data_ready_mode_t *val)
{
  lis2duxs12_ctrl1_t ctrl1;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);

  switch ((ctrl1.drdy_pulsed))
  {
    case LIS2DUXS12_DRDY_LATCHED:
      *val = LIS2DUXS12_DRDY_LATCHED;
      break;

    case LIS2DUXS12_DRDY_PULSED:
      *val = LIS2DUXS12_DRDY_PULSED;
      break;

    default:
      *val = LIS2DUXS12_DRDY_LATCHED;
      break;
  }
  return ret;
}

/**
  * @brief  Sensor mode.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   set the sensor FS and ODR.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_mode_set(stmdev_ctx_t *ctx, lis2duxs12_md_t *val)
{
  lis2duxs12_ctrl3_t ctrl3;
  lis2duxs12_ctrl5_t ctrl5;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL5, (uint8_t *)&ctrl5, 1);

  ctrl5.odr = (uint8_t)val->odr & 0xF;
  ctrl5.fs = (uint8_t)val->fs;
  ctrl5.bw = (uint8_t)val->bw;

  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);

  ctrl3.hp_en = ((uint8_t)val->odr & 0x10) ? 1 : 0;

  if (ret == 0)
  {
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL5, (uint8_t *)&ctrl5, 1);
    ret |= lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);
  }

  return ret;
}

/**
  * @brief  Sensor mode.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   get the sensor FS and ODR.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_mode_get(stmdev_ctx_t *ctx, lis2duxs12_md_t *val)
{
  lis2duxs12_ctrl3_t ctrl3;
  lis2duxs12_ctrl5_t ctrl5;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL5, (uint8_t *)&ctrl5, 1);
  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);

  switch (ctrl5.odr)
  {
    case LIS2DUXS12_OFF:
      val->odr = LIS2DUXS12_OFF;
      break;
    case LIS2DUXS12_1Hz5_ULP:
      val->odr = LIS2DUXS12_1Hz5_ULP;
      break;
    case LIS2DUXS12_3Hz_ULP:
      val->odr = LIS2DUXS12_3Hz_ULP;
      break;
    case LIS2DUXS12_25Hz_ULP:
      val->odr = LIS2DUXS12_25Hz_ULP;
      break;
    case LIS2DUXS12_6Hz:
      val->odr = LIS2DUXS12_6Hz;
      break;
    case LIS2DUXS12_12Hz5:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_12Hz5_HP : LIS2DUXS12_12Hz5;
      break;
    case LIS2DUXS12_25Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_25Hz_HP : LIS2DUXS12_25Hz;
      break;
    case LIS2DUXS12_50Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_50Hz_HP : LIS2DUXS12_50Hz;
      break;
    case LIS2DUXS12_100Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_100Hz_HP : LIS2DUXS12_100Hz;
      break;
    case LIS2DUXS12_200Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_200Hz_HP : LIS2DUXS12_200Hz;
      break;
    case LIS2DUXS12_400Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_400Hz_HP : LIS2DUXS12_400Hz;
      break;
    case LIS2DUXS12_800Hz:
      val->odr = (ctrl3.hp_en) ? LIS2DUXS12_800Hz_HP : LIS2DUXS12_800Hz;
      break;
    case LIS2DUXS12_TRIG_PIN:
      val->odr = LIS2DUXS12_TRIG_PIN;
      break;
    case LIS2DUXS12_TRIG_SW:
      val->odr = LIS2DUXS12_TRIG_SW;
      break;
    default:
      val->odr = LIS2DUXS12_OFF;
      break;
  }

  switch (ctrl5.fs)
  {
    case LIS2DUXS12_2g:
      val->fs = LIS2DUXS12_2g;
      break;
    case LIS2DUXS12_4g:
      val->fs = LIS2DUXS12_4g;
      break;
    case LIS2DUXS12_8g:
      val->fs = LIS2DUXS12_8g;
      break;
    case LIS2DUXS12_16g:
      val->fs = LIS2DUXS12_16g;
      break;
    default:
      val->fs = LIS2DUXS12_2g;
      break;
  }

  switch (ctrl5.bw)
  {
    case LIS2DUXS12_ODR_div_2:
      val->bw = LIS2DUXS12_ODR_div_2;
      break;
    case LIS2DUXS12_ODR_div_4:
      val->bw = LIS2DUXS12_ODR_div_4;
      break;
    case LIS2DUXS12_ODR_div_8:
      val->bw = LIS2DUXS12_ODR_div_8;
      break;
    case LIS2DUXS12_ODR_div_16:
      val->bw = LIS2DUXS12_ODR_div_16;
      break;
    default:
      val->bw = LIS2DUXS12_ODR_div_2;
      break;
  }

  return ret;
}

/**
  * @brief  Enter deep power down[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enter deep power down
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_enter_deep_power_down(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_sleep_t sleep;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SLEEP, (uint8_t *)&sleep, 1);

  if (ret == 0)
  {
    sleep.deep_pd = val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_SLEEP, (uint8_t *)&sleep, 1);
  }

  return ret;
}

/**
  * @brief  Enter soft power down in SPI case[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enter soft power down in SPI case
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_exit_deep_power_down(stmdev_ctx_t *ctx)
{
  lis2duxs12_if_wake_up_t if_wake_up;
  int32_t ret;

  if_wake_up.soft_pd = PROPERTY_ENABLE;
  ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_IF_WAKE_UP, (uint8_t *)&if_wake_up, 1);

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
int32_t lis2duxs12_trigger_sw(stmdev_ctx_t *ctx, lis2duxs12_md_t *md)
{
  lis2duxs12_ctrl4_t ctrl4;
  int32_t ret = 0;

  if (md->odr == LIS2DUXS12_TRIG_SW)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
    ctrl4.soc = PROPERTY_ENABLE;
    if (ret == 0)
    {
      ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
    }
  }
  return ret;
}

int32_t lis2duxs12_all_sources_get(stmdev_ctx_t *ctx, lis2duxs12_all_sources_t *val)
{
  lis2duxs12_status_register_t status;
  lis2duxs12_all_int_src_t all_int_src;
  int32_t ret = 0;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_STATUS, (uint8_t *)&status, 1);
  val->drdy = status.drdy;

  if (ret == 0 && status.int_global == 1)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_ALL_INT_SRC, (uint8_t *)&all_int_src, 1);

    if (ret == 0)
    {
      if (all_int_src.d6d_ia_all)
      {
        lis2duxs12_sixd_src_t sixd_src;

        ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SIXD_SRC, (uint8_t *)&sixd_src, 1);

        val->six_d = 1;
        val->six_d_xl = sixd_src.xl;
        val->six_d_xh = sixd_src.xh;
        val->six_d_yl = sixd_src.yl;
        val->six_d_yh = sixd_src.yh;
        val->six_d_zl = sixd_src.zl;
        val->six_d_zh = sixd_src.zh;
      }

      if (all_int_src.wu_ia_all || all_int_src.sleep_change_ia_all)
      {
        lis2duxs12_wake_up_src_t wu_src;

        ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_SRC, (uint8_t *)&wu_src, 1);

        val->wake_up = wu_src.wu_ia;
        val->wake_up_z = wu_src.z_wu;
        val->wake_up_y = wu_src.y_wu;
        val->wake_up_x = wu_src.z_wu;

        val->sleep_change = wu_src.sleep_change_ia;
        val->sleep_state = wu_src.sleep_state;
      }

      val->single_tap = all_int_src.single_tap_all;
      val->double_tap = all_int_src.double_tap_all;
      val->triple_tap = all_int_src.triple_tap_all;
      val->free_fall = all_int_src.ff_ia_all;
    }
  }

  return ret;
}

/**
  * @brief  Accelerometer data.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrived from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_xl_data_get(stmdev_ctx_t *ctx, lis2duxs12_md_t *md,
                               lis2duxs12_xl_data_t *data)
{
  uint8_t buff[6];
  int32_t ret;
  uint8_t i;
  uint8_t j;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_OUT_X_L, buff, 6);

  /* acceleration conversion */
  j = 0U;
  for (i = 0U; i < 3U; i++)
  {
    data->raw[i] = (int16_t)buff[j + 1U];
    data->raw[i] = (data->raw[i] * 256) + (int16_t) buff[j];
    j += 2U;
    switch (md->fs)
    {
      case LIS2DUXS12_2g:
        data->mg[i] = lis2duxs12_from_fs2g_to_mg(data->raw[i]);
        break;
      case LIS2DUXS12_4g:
        data->mg[i] = lis2duxs12_from_fs4g_to_mg(data->raw[i]);
        break;
      case LIS2DUXS12_8g:
        data->mg[i] = lis2duxs12_from_fs8g_to_mg(data->raw[i]);
        break;
      case LIS2DUXS12_16g:
        data->mg[i] = lis2duxs12_from_fs16g_to_mg(data->raw[i]);
        break;
      default:
        data->mg[i] = 0.0f;
        break;
    }
  }

  return ret;
}

/**
  * @brief  OUTT data.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrived from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_outt_data_get(stmdev_ctx_t *ctx, lis2duxs12_md_t *md,
                                 lis2duxs12_outt_data_t *data)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_OUT_T_AH_QVAR_L, buff, 2);

  data->outt.heat.raw = (int16_t)buff[1U];
  data->outt.heat.raw = (data->outt.heat.raw * 256) + (int16_t) buff[0];
  /* temperature conversion */
  data->outt.heat.deg_c = lis2duxs12_from_lsb_to_celsius(data->outt.heat.raw);

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
int32_t lis2duxs12_self_test_sign_set(stmdev_ctx_t *ctx, lis2duxs12_xl_self_test_t val)
{
  lis2duxs12_ctrl3_t ctrl3;
  lis2duxs12_wake_up_dur_t wkup_dur;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wkup_dur, 1);

  switch (val)
  {
    case LIS2DUXS12_XL_ST_POSITIVE:
      ctrl3.st_sign_x = 1;
      ctrl3.st_sign_y = 1;
      wkup_dur.st_sign_z = 0;
      break;

    case LIS2DUXS12_XL_ST_NEGATIVE:
      ctrl3.st_sign_x = 0;
      ctrl3.st_sign_y = 0;
      wkup_dur.st_sign_z = 1;
      break;

    case LIS2DUXS12_XL_ST_DISABLE:
    default:
      // TODO: Implement this case
      break;
  }


  ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);
  ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wkup_dur, 1);

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
int32_t lis2duxs12_self_test_start(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_self_test_t self_test;
  int32_t ret;

  if (val != 1 && val != 2)
  {
    return -1;
  }

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SELF_TEST, (uint8_t *)&self_test, 1);
  if (ret == 0)
  {
    self_test.st = (uint8_t) val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_SELF_TEST, (uint8_t *)&self_test, 1);
  }
  return ret;
}

/**
  * @brief  Configures the self test.[stop]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_self_test_stop(stmdev_ctx_t *ctx)
{
  lis2duxs12_self_test_t self_test;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SELF_TEST, (uint8_t *)&self_test, 1);
  if (ret == 0)
  {
    self_test.st = 0;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_SELF_TEST, (uint8_t *)&self_test, 1);
  }
  return ret;
}

/**
  * @brief  Configures I3C bus.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configuration params
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_i3c_configure_set(stmdev_ctx_t *ctx, lis2duxs12_i3c_cfg_t *val)
{
  lis2duxs12_i3c_if_ctrl_t i3c_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_I3C_IF_CTRL, (uint8_t *)&i3c_cfg, 1);

  if (ret == 0)
  {
    i3c_cfg.bus_act_sel = val->bus_act_sel;
    i3c_cfg.dis_drstdaa = val->drstdaa_en;
    i3c_cfg.asf_on = val->asf_on;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_I3C_IF_CTRL, (uint8_t *)&i3c_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Configures I3C bus.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configuration params
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */int32_t lis2duxs12_i3c_configure_get(stmdev_ctx_t *ctx, lis2duxs12_i3c_cfg_t *val)
{
  lis2duxs12_i3c_if_ctrl_t i3c_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_I3C_IF_CTRL, (uint8_t *)&i3c_cfg, 1);

  val->drstdaa_en = i3c_cfg.dis_drstdaa;
  val->asf_on = i3c_cfg.asf_on;

  switch (val->bus_act_sel)
  {
    case LIS2DUXS12_I3C_BUS_AVAIL_TIME_20US:
      val->bus_act_sel = LIS2DUXS12_I3C_BUS_AVAIL_TIME_20US;
      break;

    case LIS2DUXS12_I3C_BUS_AVAIL_TIME_50US:
      val->bus_act_sel = LIS2DUXS12_I3C_BUS_AVAIL_TIME_50US;
      break;

    case LIS2DUXS12_I3C_BUS_AVAIL_TIME_1MS:
      val->bus_act_sel = LIS2DUXS12_I3C_BUS_AVAIL_TIME_1MS;
      break;

    case LIS2DUXS12_I3C_BUS_AVAIL_TIME_25MS:
      val->bus_act_sel = LIS2DUXS12_I3C_BUS_AVAIL_TIME_25MS;
      break;
  }

  return ret;
}

/**
  * @brief  Change memory bank.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, EMBED_FUNC_MEM_BANK, SENSOR_HUB_MEM_BANK, STRED_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_mem_bank_set(stmdev_ctx_t *ctx, lis2duxs12_mem_bank_t val)
{
  lis2duxs12_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  if (ret == 0)
  {
    func_cfg_access.emb_func_reg_access = ((uint8_t)val & 0x1);
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  }

  return ret;
}

/**
  * @brief  Change memory bank.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      MAIN_MEM_BANK, EMBED_FUNC_MEM_BANK, SENSOR_HUB_MEM_BANK, STRED_MEM_BANK,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_mem_bank_get(stmdev_ctx_t *ctx, lis2duxs12_mem_bank_t *val)
{
  lis2duxs12_func_cfg_access_t func_cfg_access;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);

  switch ((func_cfg_access.emb_func_reg_access))
  {
    case LIS2DUXS12_MAIN_MEM_BANK:
      *val = LIS2DUXS12_MAIN_MEM_BANK;
      break;

    case LIS2DUXS12_EMBED_FUNC_MEM_BANK:
      *val = LIS2DUXS12_EMBED_FUNC_MEM_BANK;
      break;

    default:
      *val = LIS2DUXS12_MAIN_MEM_BANK;
      break;
  }
  return ret;
}

/**
  * @brief  Write buffer in a page.
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Write buffer in a page.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_ln_pg_write(stmdev_ctx_t *ctx, uint16_t address, uint8_t *buf, uint8_t len)
{
  lis2duxs12_page_address_t  page_address;
  lis2duxs12_page_sel_t page_sel;
  lis2duxs12_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  if (ret == 0)
  {
    page_rw.page_read = PROPERTY_DISABLE;
    page_rw.page_write = PROPERTY_ENABLE;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }
  if (ret == 0)
  {
    page_sel.page_sel = msb;
    page_sel.not_used0 = 1; // Default value
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }
  if (ret == 0)
  {
    page_address.page_addr = lsb;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_ADDRESS, (uint8_t *)&page_address, 1);
  }

  if (ret == 0)
  {
    for (i = 0; ((i < len) && (ret == 0)); i++)
    {
      ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_VALUE, &buf[i], 1);
      lsb++;

      /* Check if page wrap */
      if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
      {
        msb++;
        ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);

        if (ret == 0)
        {
          page_sel.page_sel = msb;
          page_sel.not_used0 = 1; // Default value
          ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
        }
      }
    }
  }

  if (ret == 0)
  {
    page_sel.page_sel = 0;
    page_sel.not_used0 = 1;// Default value
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }
  if (ret == 0)
  {
    page_rw.page_read = PROPERTY_DISABLE;
    page_rw.page_write = PROPERTY_DISABLE;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Read buffer in a page.
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Write buffer in a page.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_ln_pg_read(stmdev_ctx_t *ctx, uint16_t address, uint8_t *buf, uint8_t len)
{
  lis2duxs12_page_address_t  page_address;
  lis2duxs12_page_sel_t page_sel;
  lis2duxs12_page_rw_t page_rw;
  uint8_t msb;
  uint8_t lsb;
  int32_t ret;
  uint8_t i ;

  msb = ((uint8_t)(address >> 8) & 0x0FU);
  lsb = (uint8_t)address & 0xFFU;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  if (ret == 0)
  {
    page_rw.page_read = PROPERTY_ENABLE;
    page_rw.page_write = PROPERTY_DISABLE;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }
  if (ret == 0)
  {
    page_sel.page_sel = msb;
    page_sel.not_used0 = 1; // Default value
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }
  if (ret == 0)
  {
    page_address.page_addr = lsb;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_ADDRESS, (uint8_t *)&page_address, 1);
  }

  if (ret == 0)
  {
    for (i = 0; ((i < len) && (ret == 0)); i++)
    {
      ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_VALUE, &buf[i], 1);
      lsb++;

      /* Check if page wrap */
      if (((lsb & 0xFFU) == 0x00U) && (ret == 0))
      {
        msb++;
        ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);

        if (ret == 0)
        {
          page_sel.page_sel = msb;
          page_sel.not_used0 = 1; // Default value
          ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
        }
      }
    }
  }

  if (ret == 0)
  {
    page_sel.page_sel = 0;
    page_sel.not_used0 = 1;// Default value
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_SEL, (uint8_t *)&page_sel, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }
  if (ret == 0)
  {
    page_rw.page_read = PROPERTY_DISABLE;
    page_rw.page_write = PROPERTY_DISABLE;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

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
  * @brief       Electrical pin configuration.[set]
  *
  * @param  ctx  read / write interface definitions
  * @param  val  the electrical settings for the configurable pins.(ptr)
  * @retval      interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_conf_set(stmdev_ctx_t *ctx, lis2duxs12_pin_conf_t *val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  if (ret == 0)
  {
    pin_ctrl.cs_pu_dis = ~val->cs_pull_up;
    pin_ctrl.pd_dis_int1 = ~val->int1_pull_down;
    pin_ctrl.pd_dis_int2 = ~val->int2_pull_down;
    pin_ctrl.sda_pu_en = val->sda_pull_up;
    pin_ctrl.sdo_pu_en = val->sdo_pull_up;
    pin_ctrl.pp_od = ~val->int1_int2_push_pull;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  }

  return ret;
}

/**
  * @brief       Electrical pin configuration.[get]
  *
  * @param  ctx  read / write interface definitions
  * @param  val  the electrical settings for the configurable pins.(ptr)
  * @retval      interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_conf_get(stmdev_ctx_t *ctx, lis2duxs12_pin_conf_t *val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  val->cs_pull_up = ~pin_ctrl.cs_pu_dis;
  val->int1_pull_down = ~pin_ctrl.pd_dis_int1;
  val->int2_pull_down = ~pin_ctrl.pd_dis_int2;
  val->sda_pull_up = pin_ctrl.sda_pu_en;
  val->sdo_pull_up = pin_ctrl.sdo_pu_en;
  val->int1_int2_push_pull = ~pin_ctrl.pp_od;

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
int32_t lis2duxs12_int_pin_polarity_set(stmdev_ctx_t *ctx, lis2duxs12_int_pin_polarity_t val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  if (ret == 0)
  {
    pin_ctrl.h_lactive = (uint8_t)val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
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
int32_t lis2duxs12_int_pin_polarity_get(stmdev_ctx_t *ctx, lis2duxs12_int_pin_polarity_t *val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  switch ((pin_ctrl.h_lactive))
  {
    case LIS2DUXS12_ACTIVE_HIGH:
      *val = LIS2DUXS12_ACTIVE_HIGH;
      break;

    case LIS2DUXS12_ACTIVE_LOW:
      *val = LIS2DUXS12_ACTIVE_LOW;
      break;

    default:
      *val = LIS2DUXS12_ACTIVE_HIGH;
      break;
  }
  return ret;
}

/**
  * @brief  SPI mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SPI_4_WIRE, SPI_3_WIRE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_spi_mode_set(stmdev_ctx_t *ctx, lis2duxs12_spi_mode val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  if (ret == 0)
  {
    pin_ctrl.sim = (uint8_t)val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  SPI mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      SPI_4_WIRE, SPI_3_WIRE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_spi_mode_get(stmdev_ctx_t *ctx, lis2duxs12_spi_mode *val)
{
  lis2duxs12_pin_ctrl_t pin_ctrl;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PIN_CTRL, (uint8_t *)&pin_ctrl, 1);

  switch ((pin_ctrl.h_lactive))
  {
    case LIS2DUXS12_SPI_4_WIRE:
      *val = LIS2DUXS12_SPI_4_WIRE;
      break;

    case LIS2DUXS12_SPI_3_WIRE:
      *val = LIS2DUXS12_SPI_3_WIRE;
      break;

    default:
      *val = LIS2DUXS12_SPI_4_WIRE;
      break;
  }
  return ret;
}

/**
  * @brief  routes interrupt signals on INT 1 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_int1_route_set(stmdev_ctx_t *ctx, lis2duxs12_pin_int_route_t *val)
{
  lis2duxs12_ctrl1_t ctrl1;
  lis2duxs12_ctrl2_t ctrl2;
  lis2duxs12_md1_cfg_t md1_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);

  if (ret == 0)
  {
    ctrl1.int1_on_res = val->int_on_res;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL2, (uint8_t *)&ctrl2, 1);

    if (ret == 0)
    {
      ctrl2.int1_drdy = val->drdy;
      ctrl2.int1_fifo_ovr = val->fifo_ovr;
      ctrl2.int1_fifo_th = val->fifo_th;
      ctrl2.int1_fifo_full = val->fifo_full;
      ctrl2.int1_boot = val->boot;

      ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL2, (uint8_t *)&ctrl2, 1);
    }
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg, 1);

    if (ret == 0)
    {
      md1_cfg.int1_ff = val->free_fall;
      md1_cfg.int1_6d = val->six_d;
      md1_cfg.int1_tap = val->tap;
      md1_cfg.int1_wu = val->wake_up;
      md1_cfg.int1_sleep_change = val->sleep_change;
      md1_cfg.int1_emb_func = val->emb_function;
      md1_cfg.int1_timestamp = val->timestamp;

      ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
    }
  }

  return ret;
}

/**
  * @brief  routes interrupt signals on INT 1 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get interrupt signals routing on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_int1_route_get(stmdev_ctx_t *ctx, lis2duxs12_pin_int_route_t *val)
{
  lis2duxs12_ctrl1_t ctrl1;
  lis2duxs12_ctrl2_t ctrl2;
  lis2duxs12_md1_cfg_t md1_cfg;
  int32_t ret = 0;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL2, (uint8_t *)&ctrl2, 1);
  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg, 1);

  if (ret == 0)
  {
    val->int_on_res = ctrl1.int1_on_res;
    val->drdy = ctrl2.int1_drdy;
    val->fifo_ovr = ctrl2.int1_fifo_ovr;
    val->fifo_th = ctrl2.int1_fifo_th;
    val->fifo_full = ctrl2.int1_fifo_full;
    val->boot = ctrl2.int1_boot;
    val->free_fall = md1_cfg.int1_ff;
    val->six_d = md1_cfg.int1_6d;
    val->tap = md1_cfg.int1_tap;
    val->wake_up = md1_cfg.int1_wu;
    val->sleep_change = md1_cfg.int1_sleep_change;
    val->emb_function = md1_cfg.int1_emb_func;
    val->timestamp = md1_cfg.int1_timestamp;
  }

  return ret;
}

/**
  * @brief  routes embedded func interrupt signals on INT 1 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes embedded func interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_emb_pin_int1_route_set(stmdev_ctx_t *ctx,
                                          lis2duxs12_emb_pin_int_route_t *val)
{
  lis2duxs12_emb_func_int1_t emb_func_int1;
  lis2duxs12_md1_cfg_t md1_cfg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }

  if (ret == 0)
  {
    emb_func_int1.int1_tilt = val->tilt;
    emb_func_int1.int1_sig_mot = val->sig_mot;
    emb_func_int1.int1_step_det = val->step_det;
    emb_func_int1.int1_fsm_lc = val->fsm_lc;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }
  ret |= lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  if (ret == 0)
  {
    md1_cfg.int1_emb_func = 1;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md1_cfg, 1);
  }

  return ret;
}

/**
  * @brief  routes embedded func interrupt signals on INT 1 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes embedded func interrupt signals on INT 1 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_emb_pin_int1_route_get(stmdev_ctx_t *ctx,
                                          lis2duxs12_emb_pin_int_route_t *val)
{
  lis2duxs12_emb_func_int1_t emb_func_int1;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INT1, (uint8_t *)&emb_func_int1, 1);
  }

  if (ret == 0)
  {
    val->tilt = emb_func_int1.int1_tilt;
    val->sig_mot = emb_func_int1.int1_sig_mot;
    val->step_det = emb_func_int1.int1_step_det;
    val->fsm_lc = emb_func_int1.int1_fsm_lc;
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  routes interrupt signals on INT 2 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes interrupt signals on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_int2_route_set(stmdev_ctx_t *ctx, lis2duxs12_pin_int_route_t *val)
{
  lis2duxs12_ctrl3_t ctrl3;
  lis2duxs12_md2_cfg_t md2_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL2, (uint8_t *)&ctrl3, 1);

  if (ret == 0)
  {
    ctrl3.int2_drdy = val->drdy;
    ctrl3.int2_fifo_ovr = val->fifo_ovr;
    ctrl3.int2_fifo_th = val->fifo_th;
    ctrl3.int2_fifo_full = val->fifo_full;
    ctrl3.int2_boot = val->boot;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL3, (uint8_t *)&ctrl3, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_MD2_CFG, (uint8_t *)&md2_cfg, 1);

    if (ret == 0)
    {
      md2_cfg.int2_ff = val->free_fall;
      md2_cfg.int2_6d = val->six_d;
      md2_cfg.int2_tap = val->tap;
      md2_cfg.int2_wu = val->wake_up;
      md2_cfg.int2_sleep_change = val->sleep_change;
      md2_cfg.int2_emb_func = val->emb_function;
      md2_cfg.int2_timestamp = val->timestamp;

      ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
    }
  }

  return ret;
}

/**
  * @brief  routes interrupt signals on INT 2 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get interrupt signals routing on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_pin_int2_route_get(stmdev_ctx_t *ctx, lis2duxs12_pin_int_route_t *val)
{
  lis2duxs12_ctrl3_t ctrl3;
  lis2duxs12_md2_cfg_t md2_cfg;
  int32_t ret = 0;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL2, (uint8_t *)&ctrl3, 1);
  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_MD1_CFG, (uint8_t *)&md2_cfg, 1);

  if (ret == 0)
  {
    val->drdy = ctrl3.int2_drdy;
    val->fifo_ovr = ctrl3.int2_fifo_ovr;
    val->fifo_th = ctrl3.int2_fifo_th;
    val->fifo_full = ctrl3.int2_fifo_full;
    val->boot = ctrl3.int2_boot;
    val->free_fall = md2_cfg.int2_ff;
    val->six_d = md2_cfg.int2_6d;
    val->tap = md2_cfg.int2_tap;
    val->wake_up = md2_cfg.int2_wu;
    val->sleep_change = md2_cfg.int2_sleep_change;
    val->emb_function = md2_cfg.int2_emb_func;
    val->timestamp = md2_cfg.int2_timestamp;
  }

  return ret;
}

/**
  * @brief  routes embedded func interrupt signals on INT 2 pin.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes embedded func interrupt signals on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_emb_pin_int2_route_set(stmdev_ctx_t *ctx,
                                          lis2duxs12_emb_pin_int_route_t *val)
{
  lis2duxs12_emb_func_int2_t emb_func_int2;
  lis2duxs12_md2_cfg_t md2_cfg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
  }

  if (ret == 0)
  {
    emb_func_int2.int2_tilt = val->tilt;
    emb_func_int2.int2_sig_mot = val->sig_mot;
    emb_func_int2.int2_step_det = val->step_det;
    emb_func_int2.int2_fsm_lc = val->fsm_lc;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
  }
  ret |= lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  ret |= lis2duxs12_read_reg(ctx, LIS2DUXS12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  if (ret == 0)
  {
    md2_cfg.int2_emb_func = 1;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_MD2_CFG, (uint8_t *)&md2_cfg, 1);
  }

  return ret;
}

/**
  * @brief  routes embedded func interrupt signals on INT 2 pin.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      routes embedded func interrupt signals on INT 2 pin.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_emb_pin_int2_route_get(stmdev_ctx_t *ctx,
                                          lis2duxs12_emb_pin_int_route_t *val)
{
  lis2duxs12_emb_func_int2_t emb_func_int2;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INT2, (uint8_t *)&emb_func_int2, 1);
  }

  if (ret == 0)
  {
    val->tilt = emb_func_int2.int2_tilt;
    val->sig_mot = emb_func_int2.int2_sig_mot;
    val->step_det = emb_func_int2.int2_step_det;
    val->fsm_lc = emb_func_int2.int2_fsm_lc;
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Interrupt configuration mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      INT_DISABLED, INT_LEVEL, INT_LATCHED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_int_config_set(stmdev_ctx_t *ctx, lis2duxs12_int_config_t *val)
{
  lis2duxs12_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    switch (val->int_cfg)
    {
      case LIS2DUXS12_INT_DISABLED:
        interrupt_cfg.interrupts_enable = 0;
        break;
      case LIS2DUXS12_INT_LEVEL:
        interrupt_cfg.interrupts_enable = 1;
        interrupt_cfg.lir = 0;
        break;
      case LIS2DUXS12_INT_LATCHED:
        interrupt_cfg.interrupts_enable = 1;
        interrupt_cfg.lir = 1;
        break;
    }

    interrupt_cfg.dis_rst_lir_all_int = val->dis_rst_lir_all_int;
    interrupt_cfg.sleep_status_on_int = val->sleep_status_on_int;

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Interrupt configuration mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      INT_DISABLED, INT_LEVEL, INT_LATCHED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_int_config_get(stmdev_ctx_t *ctx, lis2duxs12_int_config_t *val)
{
  lis2duxs12_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    val->dis_rst_lir_all_int = interrupt_cfg.dis_rst_lir_all_int;
    val->sleep_status_on_int = interrupt_cfg.sleep_status_on_int;

    if (interrupt_cfg.interrupts_enable == 0)
    {
      val->int_cfg = LIS2DUXS12_INT_DISABLED;
    }
    else if (interrupt_cfg.lir == 0)
    {
      val->int_cfg = LIS2DUXS12_INT_LEVEL;
    }
    else
    {
      val->int_cfg = LIS2DUXS12_INT_LATCHED;
    }
  }

  return ret;
}

/**
  * @brief  Embedded Interrupt configuration mode.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      INT_PULSED, INT_LATCHED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_embedded_int_config_set(stmdev_ctx_t *ctx, lis2duxs12_embedded_int_config_t val)
{
  lis2duxs12_page_rw_t page_rw;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  if (ret == 0)
  {
    switch (val)
    {
      case LIS2DUXS12_EMBEDDED_INT_LEVEL:
        page_rw.emb_func_lir = 0;
        break;
      case LIS2DUXS12_EMBEDDED_INT_LATCHED:
        page_rw.emb_func_lir = 1;
        break;
    }

    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }
  ret |= lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Interrupt configuration mode.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      INT_DISABLED, INT_PULSED, INT_LATCHED
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_embedded_int_config_get(stmdev_ctx_t *ctx, lis2duxs12_embedded_int_config_t *val)
{
  lis2duxs12_page_rw_t page_rw;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_PAGE_RW, (uint8_t *)&page_rw, 1);
  }

  if (ret == 0)
  {
    if (page_rw.emb_func_lir == 0)
    {
      *val = LIS2DUXS12_EMBEDDED_INT_LEVEL;
    }
    else
    {
      *val = LIS2DUXS12_EMBEDDED_INT_LATCHED;
    }
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup FIFO
  * @brief    FIFO
  * @{/
  *
  */

/**
  * @brief  FIFO mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BYPASS_MODE, FIFO_MODE, STREAM_TO_FIFO_MODE, BYPASS_TO_STREAM_MODE, STREAM_MODE, BYPASS_TO_FIFO_MODE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_fifo_mode_set(stmdev_ctx_t *ctx, lis2duxs12_fifo_mode_t val)
{
  lis2duxs12_ctrl4_t ctrl4;
  lis2duxs12_fifo_ctrl_t fifo_ctrl;
  lis2duxs12_fifo_wtm_t fifo_wtm;
  lis2duxs12_fifo_batch_dec_t fifo_batch;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_BATCH_DEC, (uint8_t *)&fifo_batch, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_WTM, (uint8_t *)&fifo_wtm, 1);

  if (ret == 0)
  {
    /* set FIFO mode */
    if (val.operation != LIS2DUXS12_FIFO_OFF)
    {
      ctrl4.fifo_en = 1;

      if (ret == 0)
      {
        fifo_ctrl.fifo_mode = ((uint8_t)val.operation & 0x7);
      }
    }
    else
    {
      ctrl4.fifo_en = 0;
    }

    /* set fifo depth (1X/2X) */
    fifo_ctrl.fifo_depth = val.store;

    /* Set xl_only_fifo */
    fifo_wtm.xl_only_fifo = val.xl_only;

    /* set batching info */
    if (val.batch.dec_ts != LIS2DUXS12_DEC_TS_OFF)
    {
      fifo_batch.dec_ts_batch = (uint8_t)val.batch.dec_ts;
      fifo_batch.bdr_xl = (uint8_t)val.batch.bdr_xl;
    }

    fifo_ctrl.cfg_chg_en = val.cfg_change_in_fifo;

    /* set watermark */
    if (val.watermark > 0)
    {
      fifo_ctrl.stop_on_fth = 1;
      fifo_wtm.fth = val.watermark;
    }

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_FIFO_BATCH_DEC, (uint8_t *)&fifo_batch, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_FIFO_WTM, (uint8_t *)&fifo_wtm, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      BYPASS_MODE, FIFO_MODE, STREAM_TO_FIFO_MODE, BYPASS_TO_STREAM_MODE, STREAM_MODE, BYPASS_TO_FIFO_MODE,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_fifo_mode_get(stmdev_ctx_t *ctx, lis2duxs12_fifo_mode_t *val)
{
  lis2duxs12_ctrl4_t ctrl4;
  lis2duxs12_fifo_ctrl_t fifo_ctrl;
  lis2duxs12_fifo_wtm_t fifo_wtm;
  lis2duxs12_fifo_batch_dec_t fifo_batch;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_BATCH_DEC, (uint8_t *)&fifo_batch, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_WTM, (uint8_t *)&fifo_wtm, 1);

  if (ret == 0)
  {
    /* get FIFO mode */
    if (ctrl4.fifo_en == 0)
    {
      val->operation = LIS2DUXS12_FIFO_OFF;
    }
    else
    {
      val->operation = (enum lis2duxs12_operation)fifo_ctrl.fifo_mode;
    }
    val->cfg_change_in_fifo = fifo_ctrl.cfg_chg_en;

    /* get fifo depth (1X/2X) */
    val->store = (enum lis2duxs12_store)fifo_ctrl.fifo_depth;

    /* Get xl_only_fifo */
    val->xl_only = fifo_wtm.xl_only_fifo;

    /* get batching info */
    val->batch.dec_ts = (enum lis2duxs12_dec_ts)fifo_batch.dec_ts_batch;
    val->batch.bdr_xl = (enum lis2duxs12_bdr_xl)fifo_batch.bdr_xl;

    /* get watermark */
    val->watermark = fifo_wtm.fth;
  }

  return ret;
}

/**
  * @brief  Number of unread sensor data (TAG + 6 bytes) stored in FIFO.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Number of unread sensor data (TAG + 6 bytes) stored in FIFO.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_fifo_data_level_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_STATUS2, &buff, 1);

  *val = buff;

  return ret;
}

int32_t lis2duxs12_fifo_wtm_flag_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_fifo_status1_t fifo_status1;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_STATUS1, (uint8_t *)&fifo_status1, 1);

  *val = fifo_status1.fifo_wtm_ia;

  return ret;
}

int32_t lis2duxs12_fifo_sensor_tag_get(stmdev_ctx_t *ctx, lis2duxs12_fifo_sensor_tag_t *val)
{
  lis2duxs12_fifo_data_out_tag_t fifo_tag;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_DATA_OUT_TAG, (uint8_t *)&fifo_tag, 1);

  *val = (lis2duxs12_fifo_sensor_tag_t) fifo_tag.tag_sensor;

  return ret;
}

int32_t lis2duxs12_fifo_out_raw_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_DATA_OUT_X_L, buff, 6);

  return ret;
}

int32_t lis2duxs12_fifo_data_get(stmdev_ctx_t *ctx, lis2duxs12_md_t *md,
                                 lis2duxs12_fifo_mode_t *fmd,
                                 lis2duxs12_fifo_data_t *data)
{
  lis2duxs12_fifo_data_out_tag_t fifo_tag;
  uint8_t fifo_raw[6];
  int32_t ret, i;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FIFO_DATA_OUT_TAG, (uint8_t *)&fifo_tag, 1);
  data->tag = fifo_tag.tag_sensor;

  switch (fifo_tag.tag_sensor)
  {
    case LIS2DUXS12_XL_ONLY_2X_TAG:
    case LIS2DUXS12_XL_ONLY_2X_TAG_2ND:
      /* A FIFO sample consists of 2X 8-bits 3-axis XL at ODR/2 */
      ret = lis2duxs12_fifo_out_raw_get(ctx, fifo_raw);
      for (i = 0U; i < 3; i++)
      {
        data->xl[0].raw[i] = (int16_t)fifo_raw[i] * 256;
        data->xl[1].raw[i] = (int16_t)fifo_raw[3 + i] * 256;
      }
      break;
    case LIS2DUXS12_XL_AND_QVAR:
    case LIS2DUXS12_XL_TEMP_TAG:
      ret = lis2duxs12_fifo_out_raw_get(ctx, fifo_raw);
      if (fmd->xl_only == 0)
      {
        /* A FIFO sample consists of 12-bits 3-axis XL + T at ODR*/
        data->xl[0].raw[0] = (fifo_raw[0] | (fifo_raw[1] << 8)) * 16;
        data->xl[0].raw[1] = (fifo_raw[1] >> 4 | fifo_raw[2] << 4) * 16;
        data->xl[0].raw[2] = (fifo_raw[3] | (fifo_raw[4] << 8)) * 16;
        data->heat.raw = (fifo_raw[4] >> 4 | fifo_raw[5] << 4) * 16;
        if (fifo_tag.tag_sensor == LIS2DUXS12_XL_TEMP_TAG)
        {
          data->heat.deg_c = lis2duxs12_from_lsb_to_celsius(data->heat.raw);
        }
        else
        {
          data->ah_qvar = data->heat.raw;
        }
      }
      else
      {
        /* A FIFO sample consists of 16-bits 3-axis XL at ODR  */
        data->xl[0].raw[0] = (fifo_raw[0] | (fifo_raw[1] << 8));
        data->xl[0].raw[1] = (fifo_raw[1] | (fifo_raw[3] << 8));
        data->xl[0].raw[2] = (fifo_raw[2] | (fifo_raw[5] << 8));
      }
      break;
    case LIS2DUXS12_TIMESTAMP_TAG:
      ret = lis2duxs12_fifo_out_raw_get(ctx, fifo_raw);

      data->cfg_chg.cfg_change = fifo_raw[0] >> 7;
      data->cfg_chg.odr = (fifo_raw[0] >> 3) & 0xFU;
      data->cfg_chg.bw = (fifo_raw[0] >> 1) & 0x3U;
      data->cfg_chg.lp_hp = fifo_raw[0] & 0x1U;
      data->cfg_chg.qvar_en = fifo_raw[1] >> 7;
      data->cfg_chg.fs = (fifo_raw[1] >> 5) & 0x3;
      data->cfg_chg.dec_ts = (fifo_raw[1] >> 3) & 0x3U;
      data->cfg_chg.odr_xl_batch = fifo_raw[1] & 0x7U;

      data->cfg_chg.timestamp = fifo_raw[5];
      data->cfg_chg.timestamp = (data->cfg_chg.timestamp * 256U) +  fifo_raw[4];
      data->cfg_chg.timestamp = (data->cfg_chg.timestamp * 256U) +  fifo_raw[3];
      data->cfg_chg.timestamp = (data->cfg_chg.timestamp * 256U) +  fifo_raw[2];
      break;

    case LIS2DUXS12_STEP_COUNTER_TAG:
      ret = lis2duxs12_fifo_out_raw_get(ctx, fifo_raw);

      data->pedo.steps = fifo_raw[1];
      data->pedo.steps = (data->pedo.steps * 256U) +  fifo_raw[0];

      data->pedo.timestamp = fifo_raw[5];
      data->pedo.timestamp = (data->pedo.timestamp * 256U) +  fifo_raw[4];
      data->pedo.timestamp = (data->pedo.timestamp * 256U) +  fifo_raw[3];
      data->pedo.timestamp = (data->pedo.timestamp * 256U) +  fifo_raw[2];

      break;

    case LIS2DUXS12_FIFO_EMPTY:
    default:
      break;
  }

  for (i = 0U; i < 3; i++)
  {
    switch (md->fs)
    {
      case LIS2DUXS12_2g:
        data->xl[0].mg[i] = lis2duxs12_from_fs2g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2duxs12_from_fs2g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DUXS12_4g:
        data->xl[0].mg[i] = lis2duxs12_from_fs4g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2duxs12_from_fs4g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DUXS12_8g:
        data->xl[0].mg[i] = lis2duxs12_from_fs8g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2duxs12_from_fs8g_to_mg(data->xl[1].raw[i]);
        break;
      case LIS2DUXS12_16g:
        data->xl[0].mg[i] = lis2duxs12_from_fs16g_to_mg(data->xl[0].raw[i]);
        data->xl[1].mg[i] = lis2duxs12_from_fs16g_to_mg(data->xl[1].raw[i]);
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
  * @brief  Enables AH_QVAR chain.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables and configures AH_QVAR chain.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_ah_qvar_mode_set(stmdev_ctx_t *ctx,
                                    lis2duxs12_ah_qvar_mode_t val)
{
  lis2duxs12_ah_qvar_cfg_t ah_qvar_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_AH_QVAR_CFG, (uint8_t *)&ah_qvar_cfg, 1);
  if (ret == 0)
  {
    ah_qvar_cfg.ah_qvar_gain = val.ah_qvar_gain_t;
    ah_qvar_cfg.ah_qvar_c_zin = val.ah_qvar_zin_t;
    ah_qvar_cfg.ah_qvar_notch_cutoff = val.ah_qvar_notch_t;
    ah_qvar_cfg.ah_qvar_notch_en = val.ah_qvar_notch_en;
    ah_qvar_cfg.ah_qvar_en = val.ah_qvar_en;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_AH_QVAR_CFG, (uint8_t *)&ah_qvar_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enables AH_QVAR chain.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Enables and configures AH_QVAR chain.
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_ah_qvar_mode_get(stmdev_ctx_t *ctx,
                                    lis2duxs12_ah_qvar_mode_t *val)
{
  lis2duxs12_ah_qvar_cfg_t ah_qvar_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_AH_QVAR_CFG, (uint8_t *)&ah_qvar_cfg, 1);

  switch (ah_qvar_cfg.ah_qvar_gain)
  {
    case LIS2DUXS12_GAIN_0_5:
      val->ah_qvar_gain_t = LIS2DUXS12_GAIN_0_5;
      break;

    case LIS2DUXS12_GAIN_1:
      val->ah_qvar_gain_t = LIS2DUXS12_GAIN_1;
      break;

    case LIS2DUXS12_GAIN_2:
      val->ah_qvar_gain_t = LIS2DUXS12_GAIN_2;
      break;

    case LIS2DUXS12_GAIN_4:
      val->ah_qvar_gain_t = LIS2DUXS12_GAIN_4;
      break;
  }

  switch (ah_qvar_cfg.ah_qvar_c_zin)
  {
    case LIS2DUXS12_520MOhm:
      val->ah_qvar_zin_t = LIS2DUXS12_520MOhm;
      break;

    case LIS2DUXS12_175MOhm:
      val->ah_qvar_zin_t = LIS2DUXS12_175MOhm;
      break;

    case LIS2DUXS12_310MOhm:
      val->ah_qvar_zin_t = LIS2DUXS12_310MOhm;
      break;

    case LIS2DUXS12_75MOhm:
      val->ah_qvar_zin_t = LIS2DUXS12_75MOhm;
      break;
  }

  switch (ah_qvar_cfg.ah_qvar_notch_cutoff)
  {
    case LIS2DUXS12_NOTCH_50HZ:
      val->ah_qvar_notch_t = LIS2DUXS12_NOTCH_50HZ;
      break;

    case LIS2DUXS12_NOTCH_60HZ:
      val->ah_qvar_notch_t = LIS2DUXS12_NOTCH_60HZ;
      break;
  }

  val->ah_qvar_notch_en = ah_qvar_cfg.ah_qvar_notch_en;
  val->ah_qvar_en = ah_qvar_cfg.ah_qvar_en;

  return ret;
}

/**
  * @defgroup Step Counter (Pedometer)
  * @brief    Step Counter (Pedometer)
  * @{/
  *
  */
/**
  * @brief  Step counter mode[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Step counter mode
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_stpcnt_mode_set(stmdev_ctx_t *ctx, lis2duxs12_stpcnt_mode_t val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  lis2duxs12_emb_func_en_b_t emb_func_en_b;
  lis2duxs12_emb_func_fifo_en_t emb_func_fifo_en;
  lis2duxs12_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_FIFO_EN, (uint8_t *)&emb_func_fifo_en, 1);

  if ((val.false_step_rej == PROPERTY_ENABLE)  && ((emb_func_en_a.mlc_before_fsm_en & emb_func_en_b.mlc_en) == PROPERTY_DISABLE))
  {
    emb_func_en_a.mlc_before_fsm_en = PROPERTY_ENABLE;
  }

  emb_func_fifo_en.step_counter_fifo_en = val.step_counter_in_fifo;
  ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_FIFO_EN, (uint8_t *)&emb_func_fifo_en, 1);

  emb_func_en_a.pedo_en = val.step_counter_enable;
  ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);
  ret += lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);

  if (ret == 0)
  {
    pedo_cmd_reg.fp_rejection_en = val.false_step_rej;
    ret += lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
  }

  return ret;
}

/**
  * @brief  Step counter mode[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Step counter mode
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_stpcnt_mode_get(stmdev_ctx_t *ctx, lis2duxs12_stpcnt_mode_t *val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  lis2duxs12_pedo_cmd_reg_t pedo_cmd_reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_CMD_REG, (uint8_t *)&pedo_cmd_reg, 1);
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
int32_t lis2duxs12_stpcnt_steps_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_STEP_COUNTER_L, &buff[0], 2);
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  *val = buff[1];
  *val = (*val * 256) + buff[0];

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
int32_t lis2duxs12_stpcnt_rst_step_set(stmdev_ctx_t *ctx)
{
  lis2duxs12_emb_func_src_t emb_func_src;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }

  if (ret == 0)
  {
    emb_func_src.pedo_rst_step = 1;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_SRC, (uint8_t *)&emb_func_src, 1);
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

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
int32_t lis2duxs12_stpcnt_debounce_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  pedo_deb_steps_conf.deb_step = val;
  ret = lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf, 1);

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
int32_t lis2duxs12_stpcnt_debounce_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_pedo_deb_steps_conf_t pedo_deb_steps_conf;
  int32_t ret;

  ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_DEB_STEPS_CONF, (uint8_t *)&pedo_deb_steps_conf, 1);
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
int32_t lis2duxs12_stpcnt_period_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));

  ret = lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_SC_DELTAT_L, (uint8_t *)buff, 2);

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
int32_t lis2duxs12_stpcnt_period_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_EMB_ADV_PG_0 + LIS2DUXS12_PEDO_SC_DELTAT_L, (uint8_t *)buff, 2);
  *val = buff[1];
  *val = (*val * 256) + buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup Tilt
  * @brief    Tilt
  * @{/
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
int32_t lis2duxs12_tilt_mode_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_en_a.tilt_en = val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

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
int32_t lis2duxs12_tilt_mode_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  *val = emb_func_en_a.tilt_en;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup Significant motion detection
  * @brief    Significant motion detection
  * @{/
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
int32_t lis2duxs12_sigmot_mode_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  if (ret == 0)
  {
    emb_func_en_a.sign_motion_en = val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

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
int32_t lis2duxs12_sigmot_mode_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_emb_func_en_a_t emb_func_en_a;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);
  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
  }

  *val = emb_func_en_a.sign_motion_en;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */


/**
  * @defgroup Free Fall
  * @brief    Free Fall
  * @{/
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
int32_t lis2duxs12_ff_duration_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_wake_up_dur_t wake_up_dur;
  lis2duxs12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);

  if (ret == 0)
  {
    wake_up_dur.ff_dur = (val >> 5) & 0x1;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  if (ret == 0)
  {
    free_fall.ff_dur = val & 0x1F;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);
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
int32_t lis2duxs12_ff_duration_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_wake_up_dur_t wake_up_dur;
  lis2duxs12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wake_up_dur, 1);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);
  }

  *val = (wake_up_dur.ff_dur << 5) | free_fall.ff_dur;

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
int32_t lis2duxs12_ff_thresholds_set(stmdev_ctx_t *ctx, lis2duxs12_ff_thresholds_t val)
{
  lis2duxs12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);

  if (ret == 0)
  {
    free_fall.ff_ths = ((uint8_t)val & 0x7);
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);
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
int32_t lis2duxs12_ff_thresholds_get(stmdev_ctx_t *ctx, lis2duxs12_ff_thresholds_t *val)
{
  lis2duxs12_free_fall_t free_fall;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FREE_FALL, (uint8_t *)&free_fall, 1);

  switch (free_fall.ff_ths)
  {
    case LIS2DUXS12_156_mg:
      *val = LIS2DUXS12_156_mg;
      break;

    case LIS2DUXS12_219_mg:
      *val = LIS2DUXS12_219_mg;
      break;

    case LIS2DUXS12_250_mg:
      *val = LIS2DUXS12_250_mg;
      break;

    case LIS2DUXS12_312_mg:
      *val = LIS2DUXS12_312_mg;
      break;

    case LIS2DUXS12_344_mg:
      *val = LIS2DUXS12_344_mg;
      break;

    case LIS2DUXS12_406_mg:
      *val = LIS2DUXS12_406_mg;
      break;

    case LIS2DUXS12_469_mg:
      *val = LIS2DUXS12_469_mg;
      break;

    case LIS2DUXS12_500_mg:
      *val = LIS2DUXS12_500_mg;
      break;

    default:
      *val = LIS2DUXS12_156_mg;
      break;
  }
  return ret;
}

/**
  * @}
  *
  */


/**
  * @defgroup Orientation 6D (and 4D)
  * @brief    Orientation 6D (and 4D)
  * @{/
  *
  */
/**
  * @brief  configuration for 4D/6D function.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      4D/6D, DEG_80, DEG_70, DEG_60, DEG_50,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_sixd_config_set(stmdev_ctx_t *ctx, lis2duxs12_sixd_config_t val)
{
  lis2duxs12_sixd_t sixd;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SIXD, (uint8_t *)&sixd, 1);

  if (ret == 0)
  {
    sixd.d4d_en = ((uint8_t)val.mode);
    sixd.d6d_ths = ((uint8_t)val.threshold);
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_SIXD, (uint8_t *)&sixd, 1);
  }

  return ret;
}

/**
  * @brief  configuration for 4D/6D function.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      4D/6D, DEG_80, DEG_70, DEG_60, DEG_50,
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_sixd_config_get(stmdev_ctx_t *ctx, lis2duxs12_sixd_config_t *val)
{
  lis2duxs12_sixd_t sixd;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_SIXD, (uint8_t *)&sixd, 1);

  val->mode = (enum lis2duxs12_mode)sixd.d4d_en;

  switch ((sixd.d6d_ths))
  {
    case LIS2DUXS12_DEG_80:
      val->threshold = LIS2DUXS12_DEG_80;
      break;

    case LIS2DUXS12_DEG_70:
      val->threshold = LIS2DUXS12_DEG_70;
      break;

    case LIS2DUXS12_DEG_60:
      val->threshold = LIS2DUXS12_DEG_60;
      break;

    case LIS2DUXS12_DEG_50:
      val->threshold = LIS2DUXS12_DEG_50;
      break;

    default:
      val->threshold = LIS2DUXS12_DEG_80;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup wakeup configuration
  * @brief    wakeup configuration
  * @{/
  *
  */

/**
  * @brief  configuration for wakeup function.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      threshold, duration, ...
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_wakeup_config_set(stmdev_ctx_t *ctx, lis2duxs12_wakeup_config_t val)
{
  lis2duxs12_wake_up_ths_t wup_ths;
  lis2duxs12_wake_up_dur_t wup_dur;
  lis2duxs12_wake_up_dur_ext_t wup_dur_ext;
  lis2duxs12_interrupt_cfg_t int_cfg;
  lis2duxs12_ctrl1_t ctrl1;
  lis2duxs12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_THS, (uint8_t *)&wup_ths, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    wup_dur.wake_dur = val.wake_dur & 0x3;
    wup_dur_ext.wu_dur_extended = val.wake_dur >> 2;
    wup_dur.sleep_dur = val.sleep_dur;

    int_cfg.wake_ths_w = val.wake_ths_weight;
    wup_ths.wk_ths = val.wake_ths;
    wup_ths.sleep_on = val.wake_enable;
    ctrl4.inact_odr = val.inact_odr;

    if (val.wake_enable == 1)
    {
      ctrl1.wu_x_en = 1;
      ctrl1.wu_y_en = 1;
      ctrl1.wu_z_en = 1;
    }
    else
    {
      ctrl1.wu_x_en = 0;
      ctrl1.wu_y_en = 0;
      ctrl1.wu_z_en = 0;
    }

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_WAKE_UP_THS, (uint8_t *)&wup_ths, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL1, (uint8_t *)&ctrl1, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);
  }

  return ret;
}

/**
  * @brief  configuration for wakeup function.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      threshold, duration, ...
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis2duxs12_wakeup_config_get(stmdev_ctx_t *ctx, lis2duxs12_wakeup_config_t *val)
{
  lis2duxs12_wake_up_ths_t wup_ths;
  lis2duxs12_wake_up_dur_t wup_dur;
  lis2duxs12_wake_up_dur_ext_t wup_dur_ext;
  lis2duxs12_interrupt_cfg_t int_cfg;
  lis2duxs12_ctrl4_t ctrl4;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_THS, (uint8_t *)&wup_ths, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR, (uint8_t *)&wup_dur, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_WAKE_UP_DUR_EXT, (uint8_t *)&wup_dur_ext, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);
  ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_CTRL4, (uint8_t *)&ctrl4, 1);

  if (ret == 0)
  {
    val->wake_dur = (enum lis2duxs12_wake_dur)((wup_dur_ext.wu_dur_extended << 2) | wup_dur.wake_dur);
    val->sleep_dur = wup_dur.sleep_dur;

    val->wake_ths_weight = int_cfg.wake_ths_w;
    val->wake_ths = wup_ths.wk_ths;
    val->wake_enable = (enum lis2duxs12_wake_enable)wup_ths.sleep_on;
    val->inact_odr = (enum lis2duxs12_inact_odr)ctrl4.inact_odr;
  }

  return ret;
}

/**
  * @}
  *
  */

int32_t lis2duxs12_tap_config_set(stmdev_ctx_t *ctx, lis2duxs12_tap_config_t val)
{
  lis2duxs12_tap_cfg0_t tap_cfg0;
  lis2duxs12_tap_cfg1_t tap_cfg1;
  lis2duxs12_tap_cfg2_t tap_cfg2;
  lis2duxs12_tap_cfg3_t tap_cfg3;
  lis2duxs12_tap_cfg4_t tap_cfg4;
  lis2duxs12_tap_cfg5_t tap_cfg5;
  lis2duxs12_tap_cfg6_t tap_cfg6;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG3, (uint8_t *)&tap_cfg3, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG4, (uint8_t *)&tap_cfg4, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG5, (uint8_t *)&tap_cfg5, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG6, (uint8_t *)&tap_cfg6, 1);

  if (ret == 0)
  {
    tap_cfg0.axis = val.axis;
    tap_cfg0.invert_t = val.inverted_peak_time;
    tap_cfg1.pre_still_ths = val.pre_still_ths;
    tap_cfg3.post_still_ths = val.post_still_ths;
    tap_cfg1.post_still_t = val.post_still_time & 0xFU;
    tap_cfg2.post_still_t = val.post_still_time >> 4;
    tap_cfg2.wait_t = val.shock_wait_time;
    tap_cfg3.latency_t = val.latency;
    tap_cfg4.wait_end_latency = val.wait_end_latency;
    tap_cfg4.peak_ths = val.peak_ths;
    tap_cfg5.rebound_t = val.rebound;
    tap_cfg5.single_tap_en = val.single_tap_on;
    tap_cfg5.double_tap_en = val.double_tap_on;
    tap_cfg5.triple_tap_en = val.triple_tap_on;
    tap_cfg6.pre_still_st = val.pre_still_start;
    tap_cfg6.pre_still_n = val.pre_still_n;

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG3, (uint8_t *)&tap_cfg3, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG4, (uint8_t *)&tap_cfg4, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG5, (uint8_t *)&tap_cfg5, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_TAP_CFG6, (uint8_t *)&tap_cfg6, 1);
  }

  return ret;
}

int32_t lis2duxs12_tap_config_get(stmdev_ctx_t *ctx, lis2duxs12_tap_config_t *val)
{
  lis2duxs12_tap_cfg0_t tap_cfg0;
  lis2duxs12_tap_cfg1_t tap_cfg1;
  lis2duxs12_tap_cfg2_t tap_cfg2;
  lis2duxs12_tap_cfg3_t tap_cfg3;
  lis2duxs12_tap_cfg4_t tap_cfg4;
  lis2duxs12_tap_cfg5_t tap_cfg5;
  lis2duxs12_tap_cfg6_t tap_cfg6;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG1, (uint8_t *)&tap_cfg1, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG2, (uint8_t *)&tap_cfg2, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG3, (uint8_t *)&tap_cfg3, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG4, (uint8_t *)&tap_cfg4, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG5, (uint8_t *)&tap_cfg5, 1);
  ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_TAP_CFG6, (uint8_t *)&tap_cfg6, 1);

  if (ret == 0)
  {
    val->axis = (enum lis2duxs12_axis)tap_cfg0.axis;
    val->inverted_peak_time = tap_cfg0.invert_t;
    val->pre_still_ths = tap_cfg1.pre_still_ths;
    val->post_still_ths = tap_cfg3.post_still_ths;
    val->post_still_time = (tap_cfg2.post_still_t << 4) | tap_cfg1.post_still_t;
    val->shock_wait_time = tap_cfg2.wait_t;
    val->latency = tap_cfg3.latency_t;
    val->wait_end_latency = tap_cfg4.wait_end_latency;
    val->peak_ths = tap_cfg4.peak_ths;
    val->rebound = tap_cfg5.rebound_t;
    val->single_tap_on = tap_cfg5.single_tap_en;
    val->double_tap_on = tap_cfg5.double_tap_en;
    val->triple_tap_on = tap_cfg5.triple_tap_en;
    val->pre_still_start = tap_cfg6.pre_still_st;
    val->pre_still_n = tap_cfg6.pre_still_n;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup   lis2duxs12_Timestamp
  * @brief      This section groups all the functions that manage the
  *             timestamp generation.
  * @{
  *
  */

/**
  * @brief  Enables timestamp counter.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of timestamp_en in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_timestamp_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_interrupt_cfg_t int_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);

  if (ret == 0)
  {
    int_cfg.timestamp_en = (uint8_t)val;
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enables timestamp counter.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of timestamp_en in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_timestamp_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_interrupt_cfg_t int_cfg;
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_INTERRUPT_CFG, (uint8_t *)&int_cfg, 1);
  *val = int_cfg.timestamp_en;

  return ret;
}

/**
  * @brief  Timestamp first data output register (r).
  *         The value is expressed as a 32-bit word and the bit resolution
  *         is 10 us.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_timestamp_raw_get(stmdev_ctx_t *ctx, uint32_t *val)
{
  uint8_t buff[4];
  int32_t ret;

  ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_TIMESTAMP0, buff, 4);
  *val = buff[3];
  *val = (*val * 256U) +  buff[2];
  *val = (*val * 256U) +  buff[1];
  *val = (*val * 256U) +  buff[0];

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup   LIS2DUXS12_finite_state_machine
  * @brief      This section groups all the functions that manage the
  *             state_machine.
  * @{
  *
  */

/**
  * @brief  Interrupt status bit for FSM long counter timeout interrupt
  *         event.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of is_fsm_lc in reg EMB_FUNC_STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_long_cnt_flag_data_ready_get(stmdev_ctx_t *ctx,
                                                uint8_t *val)
{
  lis2duxs12_emb_func_status_t emb_func_status;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_STATUS,
                              (uint8_t *)&emb_func_status, 1);

    *val = emb_func_status.is_fsm_lc;
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Embedded final state machine functions mode.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of fsm_en in reg EMB_FUNC_EN_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_emb_fsm_en_set(stmdev_ctx_t *ctx, uint8_t val)
{
  int32_t ret;

  lis2duxs12_emb_func_en_b_t emb_func_en_b;
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                              (uint8_t *)&emb_func_en_b, 1);

    emb_func_en_b.fsm_en = (uint8_t)val;

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                                (uint8_t *)&emb_func_en_b, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Embedded final state machine functions mode.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Get the values of fsm_en in reg EMB_FUNC_EN_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_emb_fsm_en_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  int32_t ret;

  lis2duxs12_emb_func_en_b_t emb_func_en_b;
  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                              (uint8_t *)&emb_func_en_b, 1);

    *val = emb_func_en_b.fsm_en;

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                                (uint8_t *)&emb_func_en_b, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Embedded final state machine functions mode.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Structure of registers from FSM_ENABLE_A to FSM_ENABLE_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_enable_set(stmdev_ctx_t *ctx,
                                  lis2duxs12_emb_fsm_enable_t *val)
{
  lis2duxs12_emb_func_en_b_t emb_func_en_b;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_FSM_ENABLE,
                               (uint8_t *)&val->fsm_enable, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                              (uint8_t *)&emb_func_en_b, 1);

    if ((val->fsm_enable.fsm1_en |
         val->fsm_enable.fsm2_en |
         val->fsm_enable.fsm3_en |
         val->fsm_enable.fsm4_en |
         val->fsm_enable.fsm5_en |
         val->fsm_enable.fsm6_en |
         val->fsm_enable.fsm7_en |
         val->fsm_enable.fsm8_en) != PROPERTY_DISABLE)
    {
      emb_func_en_b.fsm_en = PROPERTY_ENABLE;
    }
    else
    {
      emb_func_en_b.fsm_en = PROPERTY_DISABLE;
    }

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B,
                                (uint8_t *)&emb_func_en_b, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Embedded final state machine functions mode.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Structure of registers from FSM_ENABLE_A to FSM_ENABLE_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_enable_get(stmdev_ctx_t *ctx,
                                  lis2duxs12_emb_fsm_enable_t *val)
{
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FSM_ENABLE,
                              (uint8_t *)&val->fsm_enable, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM long counter status register. Long counter value is an
  *         unsigned integer value (16-bit format).[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_long_cnt_set(stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    buff[1] = (uint8_t)(val / 256U);
    buff[0] = (uint8_t)(val - (buff[1] * 256U));
    ret = lis2duxs12_write_reg(ctx, LIS2DUXS12_FSM_LONG_COUNTER_L, buff, 2);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM long counter status register. Long counter value is an
  *         unsigned integer value (16-bit format).[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_long_cnt_get(stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FSM_LONG_COUNTER_L, buff, 2);
    *val = buff[1];
    *val = (*val * 256U) + buff[0];
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM output registers.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Structure of registers from FSM_OUTS1 to FSM_OUTS16
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_out_get(stmdev_ctx_t *ctx,
                               lis2duxs12_fsm_out_t *val)
{
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FSM_OUTS1,
                              (uint8_t *)&val->fsm_outs1, 16);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Finite State Machine ODR configuration.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of fsm_odr in reg EMB_FUNC_ODR_CFG_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_data_rate_set(stmdev_ctx_t *ctx,
                                     lis2duxs12_fsm_val_odr_t val)
{
  lis2duxs12_fsm_odr_t fsm_odr_reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FSM_ODR,
                              (uint8_t *)&fsm_odr_reg, 1);

    fsm_odr_reg.fsm_odr = (uint8_t)val;
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_FSM_ODR,
                                (uint8_t *)&fsm_odr_reg, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Finite State Machine ODR configuration.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Get the values of fsm_odr in reg EMB_FUNC_ODR_CFG_B
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_data_rate_get(stmdev_ctx_t *ctx,
                                     lis2duxs12_fsm_val_odr_t *val)
{
  lis2duxs12_fsm_odr_t fsm_odr_reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_FSM_ODR,
                              (uint8_t *)&fsm_odr_reg, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  switch (fsm_odr_reg.fsm_odr)
  {
    case LIS2DUXS12_ODR_FSM_12Hz5:
      *val = LIS2DUXS12_ODR_FSM_12Hz5;
      break;

    case LIS2DUXS12_ODR_FSM_25Hz:
      *val = LIS2DUXS12_ODR_FSM_25Hz;
      break;

    case LIS2DUXS12_ODR_FSM_50Hz:
      *val = LIS2DUXS12_ODR_FSM_50Hz;
      break;

    case LIS2DUXS12_ODR_FSM_100Hz:
      *val = LIS2DUXS12_ODR_FSM_100Hz;
      break;

    case LIS2DUXS12_ODR_FSM_200Hz:
      *val = LIS2DUXS12_ODR_FSM_200Hz;
      break;

    case LIS2DUXS12_ODR_FSM_400Hz:
      *val = LIS2DUXS12_ODR_FSM_400Hz;
      break;

    case LIS2DUXS12_ODR_FSM_800Hz:
      *val = LIS2DUXS12_ODR_FSM_800Hz;
      break;

    default:
      *val = LIS2DUXS12_ODR_FSM_12Hz5;
      break;
  }

  return ret;
}

/**
  * @brief  FSM initialization request.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of fsm_init in reg FSM_INIT
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_init_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis2duxs12_emb_func_init_b_t emb_func_init_b;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INIT_B,
                              (uint8_t *)&emb_func_init_b, 1);

    emb_func_init_b.fsm_init = (uint8_t)val;

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_INIT_B,
                                (uint8_t *)&emb_func_init_b, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM initialization request.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  val    Change the values of fsm_init in reg FSM_INIT
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_init_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis2duxs12_emb_func_init_b_t emb_func_init_b;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_INIT_B,
                              (uint8_t *)&emb_func_init_b, 1);

    *val = emb_func_init_b.fsm_init;
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  FSM long counter timeout register (r/w). The long counter
  *         timeout value is an unsigned integer value (16-bit format).
  *         When the long counter value reached this value, the FSM
  *         generates an interrupt.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_long_cnt_int_value_set(stmdev_ctx_t *ctx,
                                          uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_FSM_LC_TIMEOUT_L, buff, 2);

  return ret;
}

/**
  * @brief  FSM long counter timeout register (r/w). The long counter
  *         timeout value is an unsigned integer value (16-bit format).
  *         When the long counter value reached this value, the FSM generates
  *         an interrupt.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_long_cnt_int_value_get(stmdev_ctx_t *ctx,
                                          uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_FSM_LC_TIMEOUT_L, buff, 2);
  *val = buff[1];
  *val = (*val * 256U) + buff[0];

  return ret;
}

/**
  * @brief  FSM number of programs register.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_number_of_programs_set(stmdev_ctx_t *ctx,
                                              uint8_t *buff)
{
  int32_t ret;

  ret = lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_FSM_PROGRAMS, buff, 2);

  return ret;
}

/**
  * @brief  FSM number of programs register.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_number_of_programs_get(stmdev_ctx_t *ctx,
                                              uint8_t *buff)
{
  int32_t ret;

  ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_FSM_PROGRAMS, buff, 2);

  return ret;
}

/**
  * @brief  FSM start address register (r/w). First available address is
  *         0x033C.[set]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_start_address_set(stmdev_ctx_t *ctx,
                                         uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret = lis2duxs12_ln_pg_write(ctx, LIS2DUXS12_FSM_START_ADD_L, buff, 2);

  return ret;
}

/**
  * @brief  FSM start address register (r/w). First available address
  *         is 0x033C.[get]
  *
  * @param  ctx    Read / write interface definitions.(ptr)
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lis2duxs12_fsm_start_address_get(stmdev_ctx_t *ctx,
                                         uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret = lis2duxs12_ln_pg_read(ctx, LIS2DUXS12_FSM_START_ADD_L, buff, 2);
  *val = buff[1];
  *val = (*val * 256U) +  buff[0];

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
  * @brief  Enable Machine Learning Core.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of mlc_en in
  *                  reg EMB_FUNC_EN_B and mlc_before_fsm_en
  *                  in EMB_FUNC_INIT_A
  *
  */
int32_t lis2duxs12_mlc_set(stmdev_ctx_t *ctx, lis2duxs12_mlc_mode_t val)
{
  lis2duxs12_emb_func_en_a_t emb_en_a;
  lis2duxs12_emb_func_en_b_t emb_en_b;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
    ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);

    switch (val)
    {
      case LIS2DUXS12_MLC_OFF:
        emb_en_a.mlc_before_fsm_en = 0;
        emb_en_b.mlc_en = 0;
        break;
      case LIS2DUXS12_MLC_ON:
        emb_en_a.mlc_before_fsm_en = 0;
        emb_en_b.mlc_en = 1;
        break;
      case LIS2DUXS12_MLC_ON_BEFORE_FSM:
        emb_en_a.mlc_before_fsm_en = 1;
        emb_en_b.mlc_en = 1;
        break;
      default:
        break;
    }

    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Enable Machine Learning Core.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of mlc_en in
  *                  reg EMB_FUNC_EN_B and mlc_before_fsm_en
  *                  in EMB_FUNC_INIT_A
  *
  */
int32_t lis2duxs12_mlc_get(stmdev_ctx_t *ctx, lis2duxs12_mlc_mode_t *val)
{
  lis2duxs12_emb_func_en_a_t emb_en_a;
  lis2duxs12_emb_func_en_b_t emb_en_b;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_A, (uint8_t *)&emb_en_a, 1);
    ret += lis2duxs12_read_reg(ctx, LIS2DUXS12_EMB_FUNC_EN_B, (uint8_t *)&emb_en_b, 1);

    if (emb_en_a.mlc_before_fsm_en == 0 && emb_en_b.mlc_en == 0)
    {
      *val = LIS2DUXS12_MLC_OFF;
    }
    else if (emb_en_a.mlc_before_fsm_en == 0 && emb_en_b.mlc_en == 1)
    {
      *val = LIS2DUXS12_MLC_ON;
    }
    else if (emb_en_a.mlc_before_fsm_en == 1)
    {
      *val = LIS2DUXS12_MLC_ON_BEFORE_FSM;
    }
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Machine Learning Core status register[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      register MLC_STATUS_MAINPAGE
  *
  */
int32_t lis2duxs12_mlc_status_get(stmdev_ctx_t *ctx,
                                  lis2duxs12_mlc_status_mainpage_t *val)
{
  return lis2duxs12_read_reg(ctx, LIS2DUXS12_MLC_STATUS_MAINPAGE,
                             (uint8_t *) val, 1);
}

/**
  * @brief  prgsens_out: [get] Output value of all MLCx decision trees.
  *
  * @param  ctx_t *ctx: read / write interface definitions
  * @param  uint8_t * : buffer that stores data read
  *
  */
int32_t lis2duxs12_mlc_out_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_MLC1_SRC, buff, 4);
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @brief  Machine Learning Core data rate selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of mlc_odr in
  *                  reg EMB_FUNC_ODR_CFG_C
  *
  */
int32_t lis2duxs12_mlc_data_rate_set(stmdev_ctx_t *ctx,
                                     lis2duxs12_mlc_odr_val_t val)
{
  lis2duxs12_mlc_odr_t reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_MLC_ODR, (uint8_t *)&reg, 1);
    reg.mlc_odr = (uint8_t)val;
    ret += lis2duxs12_write_reg(ctx, LIS2DUXS12_MLC_ODR, (uint8_t *)&reg, 1);
  }

  if (ret == 0)
  {
    ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);
  }

  return ret;
}

/**
  * @brief  Machine Learning Core data rate selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of mlc_odr in
  *                  reg EMB_FUNC_ODR_CFG_C
  *
  */
int32_t lis2duxs12_mlc_data_rate_get(stmdev_ctx_t *ctx,
                                     lis2duxs12_mlc_odr_val_t *val)
{
  lis2duxs12_mlc_odr_t reg;
  int32_t ret;

  ret = lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_EMBED_FUNC_MEM_BANK);

  if (ret == 0)
  {
    ret = lis2duxs12_read_reg(ctx, LIS2DUXS12_MLC_ODR, (uint8_t *)&reg, 1);

    switch (reg.mlc_odr)
    {
      case LIS2DUXS12_ODR_PRGS_12Hz5:
        *val = LIS2DUXS12_ODR_PRGS_12Hz5;
        break;

      case LIS2DUXS12_ODR_PRGS_25Hz:
        *val = LIS2DUXS12_ODR_PRGS_25Hz;
        break;

      case LIS2DUXS12_ODR_PRGS_50Hz:
        *val = LIS2DUXS12_ODR_PRGS_50Hz;
        break;

      case LIS2DUXS12_ODR_PRGS_100Hz:
        *val = LIS2DUXS12_ODR_PRGS_100Hz;
        break;

      case LIS2DUXS12_ODR_PRGS_200Hz:
        *val = LIS2DUXS12_ODR_PRGS_200Hz;
        break;

      default:
        *val = LIS2DUXS12_ODR_PRGS_12Hz5;
        break;
    }
  }

  ret += lis2duxs12_mem_bank_set(ctx, LIS2DUXS12_MAIN_MEM_BANK);

  return ret;
}

/**
  * @}
  *
  */
