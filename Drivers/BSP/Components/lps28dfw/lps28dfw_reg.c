/*
 ******************************************************************************
 * @file    lps28dfw_reg.c
 * @author  Sensors Software Solution Team
 * @brief   LPS28DFW driver file
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

#include "lps28dfw_reg.h"

/**
  * @defgroup    LPS28DFW
  * @brief       This file provides a set of functions needed to drive the
  *              lps28dfw nano pressure sensor.
  * @{
  *
  */

/**
  * @defgroup    Interfaces_Functions
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
int32_t __weak lps28dfw_read_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
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
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to write
  * @param  data  pointer to data to write in register reg(ptr)
  * @param  len   number of consecutive register to write
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak lps28dfw_write_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
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
  * @defgroup    Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lps28dfw_from_fs1260_to_hPa(int32_t lsb)
{
  return ((float_t)lsb / 1048576.0f);   /* 4096.0f * 256 */
}

float_t lps28dfw_from_fs4000_to_hPa(int32_t lsb)
{
  return ((float_t)lsb /  524288.0f);   /* 2048.0f * 256 */
}

float_t lps28dfw_from_lsb_to_celsius(int16_t lsb)
{
  return ((float_t)lsb / 100.0f);
}

/**
  * @}
  *
  */

/**
  * @defgroup    Basic functions
  * @brief       This section groups all the functions concerning device basic
  *              configuration.
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
int32_t lps28dfw_id_get(const stmdev_ctx_t *ctx, lps28dfw_id_t *val)
{
  uint8_t reg;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_WHO_AM_I, &reg, 1);
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
int32_t lps28dfw_bus_mode_set(const stmdev_ctx_t *ctx, lps28dfw_bus_mode_t *val)
{
  lps28dfw_i3c_if_ctrl_add_t i3c_if_ctrl_add;
  lps28dfw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  if (ret == 0)
  {
    if_ctrl.int_en_i3c = ((uint8_t)val->interface & 0x04U) >> 2;
    ret = lps28dfw_write_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_I3C_IF_CTRL_ADD,
                            (uint8_t *)&i3c_if_ctrl_add, 1);
  }
  if (ret == 0)
  {
    i3c_if_ctrl_add.asf_on = (uint8_t)val->filter & 0x01U;
    i3c_if_ctrl_add.I3C_Bus_Avb_Sel = (uint8_t)val->bus_avb_time & 0x03U;
    ret = lps28dfw_write_reg(ctx, LPS28DFW_I3C_IF_CTRL_ADD,
                             (uint8_t *)&i3c_if_ctrl_add, 1);
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
int32_t lps28dfw_bus_mode_get(const stmdev_ctx_t *ctx, lps28dfw_bus_mode_t *val)
{
  lps28dfw_i3c_if_ctrl_add_t i3c_if_ctrl_add;
  lps28dfw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_I3C_IF_CTRL_ADD,
                            (uint8_t *)&i3c_if_ctrl_add, 1);

    switch (if_ctrl.int_en_i3c << 2)
    {
      case LPS28DFW_SEL_BY_HW:
        val->interface = LPS28DFW_SEL_BY_HW;
        break;
      case LPS28DFW_INT_PIN_ON_I3C:
        val->interface = LPS28DFW_INT_PIN_ON_I3C;
        break;
      default:
        val->interface = LPS28DFW_SEL_BY_HW;
        break;
    }

    switch (i3c_if_ctrl_add.asf_on)
    {
      case LPS28DFW_AUTO:
        val->filter = LPS28DFW_AUTO;
        break;
      case LPS28DFW_ALWAYS_ON:
        val->filter = LPS28DFW_ALWAYS_ON;
        break;
      default:
        val->filter = LPS28DFW_AUTO;
        break;
    }

    switch (i3c_if_ctrl_add.I3C_Bus_Avb_Sel)
    {
      case LPS28DFW_BUS_AVB_TIME_50us:
        val->bus_avb_time = LPS28DFW_BUS_AVB_TIME_50us;
        break;
      case LPS28DFW_BUS_AVB_TIME_2us:
        val->bus_avb_time = LPS28DFW_BUS_AVB_TIME_2us;
        break;
      case LPS28DFW_BUS_AVB_TIME_1ms:
        val->bus_avb_time = LPS28DFW_BUS_AVB_TIME_1ms;
        break;
      case LPS28DFW_BUS_AVB_TIME_25ms:
        val->bus_avb_time = LPS28DFW_BUS_AVB_TIME_25ms;
        break;
      default:
        val->bus_avb_time = LPS28DFW_BUS_AVB_TIME_50us;
        break;
    }
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
int32_t lps28dfw_init_set(const stmdev_ctx_t *ctx, lps28dfw_init_t val)
{
  lps28dfw_ctrl_reg2_t ctrl_reg2;
  lps28dfw_ctrl_reg3_t ctrl_reg3;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG2, reg, 2);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg2, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg3, &reg[1]);

    switch (val)
    {
      case LPS28DFW_BOOT:
        ctrl_reg2.boot = PROPERTY_ENABLE;
        ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG2,
                                 (uint8_t *)&ctrl_reg2, 1);
        break;
      case LPS28DFW_RESET:
        ctrl_reg2.swreset = PROPERTY_ENABLE;
        ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG2,
                                 (uint8_t *)&ctrl_reg2, 1);
        break;
      case LPS28DFW_DRV_RDY:
        ctrl_reg2.bdu = PROPERTY_ENABLE;
        ctrl_reg3.if_add_inc = PROPERTY_ENABLE;
        bytecpy(&reg[0], (uint8_t *)&ctrl_reg2);
        bytecpy(&reg[1], (uint8_t *)&ctrl_reg3);
        ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG2, reg, 2);
        break;
      default:
        ctrl_reg2.swreset = PROPERTY_ENABLE;
        ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG2,
                                 (uint8_t *)&ctrl_reg2, 1);
        break;
    }
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
int32_t lps28dfw_status_get(const stmdev_ctx_t *ctx, lps28dfw_stat_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  lps28dfw_int_source_t int_source;
  lps28dfw_ctrl_reg2_t ctrl_reg2;
  lps28dfw_status_t status;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG2,
                          (uint8_t *)&ctrl_reg2, 1);
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_INT_SOURCE, (uint8_t *)&int_source, 1);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_STATUS, (uint8_t *)&status, 1);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }
  val->sw_reset  = ctrl_reg2.swreset;
  val->boot      = int_source.boot_on;
  val->drdy_pres = status.p_da;
  val->drdy_temp = status.t_da;
  val->ovr_pres  = status.p_or;
  val->ovr_temp  = status.t_or;
  val->end_meas  = ~ctrl_reg2.oneshot;
  val->ref_done = ~interrupt_cfg.autozero;

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
int32_t lps28dfw_pin_conf_set(const stmdev_ctx_t *ctx, lps28dfw_pin_conf_t *val)
{
  lps28dfw_ctrl_reg3_t ctrl_reg3;
  lps28dfw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);

  if (ret == 0)
  {
    if_ctrl.int_pd_dis = ~val->int_pull_down;
    if_ctrl.sda_pu_en = val->sda_pull_up;
    ret = lps28dfw_write_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }
  if (ret == 0)
  {
    ctrl_reg3.pp_od = ~val->int_push_pull;
    ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
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
int32_t lps28dfw_pin_conf_get(const stmdev_ctx_t *ctx, lps28dfw_pin_conf_t *val)
{
  lps28dfw_ctrl_reg3_t ctrl_reg3;
  lps28dfw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  val->int_pull_down = ~if_ctrl.int_pd_dis;
  val->sda_pull_up  = if_ctrl.sda_pu_en;
  val->int_push_pull  = ~ctrl_reg3.pp_od;

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
int32_t lps28dfw_all_sources_get(const stmdev_ctx_t *ctx,
                                 lps28dfw_all_sources_t *val)
{
  lps28dfw_fifo_status2_t fifo_status2;
  lps28dfw_int_source_t int_source;
  lps28dfw_status_t status;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_STATUS, (uint8_t *)&status, 1);
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_INT_SOURCE,
                            (uint8_t *)&int_source, 1);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_FIFO_STATUS2,
                            (uint8_t *)&fifo_status2, 1);
  }

  val->drdy_pres        = status.p_da;
  val->drdy_temp        = status.t_da;
  val->over_pres        = int_source.ph;
  val->under_pres       = int_source.pl;
  val->thrsld_pres      = int_source.ia;
  val->fifo_full        = fifo_status2.fifo_full_ia;
  val->fifo_ovr         = fifo_status2.fifo_ovr_ia;
  val->fifo_th          = fifo_status2.fifo_wtm_ia;

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
int32_t lps28dfw_mode_set(const stmdev_ctx_t *ctx, lps28dfw_md_t *val)
{
  lps28dfw_ctrl_reg1_t ctrl_reg1;
  lps28dfw_ctrl_reg2_t ctrl_reg2;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG1, reg, 2);

  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg1, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg2, &reg[1]);

    ctrl_reg1.odr = (uint8_t)val->odr;
    ctrl_reg1.avg = (uint8_t)val->avg;
    ctrl_reg2.en_lpfp = (uint8_t)val->lpf & 0x01U;
    ctrl_reg2.lfpf_cfg = ((uint8_t)val->lpf & 0x02U) >> 2;
    ctrl_reg2.fs_mode = (uint8_t)val->fs;

    bytecpy(&reg[0], (uint8_t *)&ctrl_reg1);
    bytecpy(&reg[1], (uint8_t *)&ctrl_reg2);
    ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG1, reg, 2);
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
int32_t lps28dfw_mode_get(const stmdev_ctx_t *ctx, lps28dfw_md_t *val)
{
  lps28dfw_ctrl_reg1_t ctrl_reg1;
  lps28dfw_ctrl_reg2_t ctrl_reg2;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG1, reg, 2);

  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg1, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg2, &reg[1]);

    switch (ctrl_reg2.fs_mode)
    {
      case LPS28DFW_1260hPa:
        val->fs = LPS28DFW_1260hPa;
        break;
      case LPS28DFW_4000hPa:
        val->fs = LPS28DFW_4000hPa;
        break;
      default:
        val->fs = LPS28DFW_1260hPa;
        break;
    }

    switch (ctrl_reg1.odr)
    {
      case LPS28DFW_ONE_SHOT:
        val->odr = LPS28DFW_ONE_SHOT;
        break;
      case LPS28DFW_1Hz:
        val->odr = LPS28DFW_1Hz;
        break;
      case LPS28DFW_4Hz:
        val->odr = LPS28DFW_4Hz;
        break;
      case LPS28DFW_10Hz:
        val->odr = LPS28DFW_10Hz;
        break;
      case LPS28DFW_25Hz:
        val->odr = LPS28DFW_25Hz;
        break;
      case LPS28DFW_50Hz:
        val->odr = LPS28DFW_50Hz;
        break;
      case LPS28DFW_75Hz:
        val->odr = LPS28DFW_75Hz;
        break;
      case LPS28DFW_100Hz:
        val->odr = LPS28DFW_100Hz;
        break;
      case LPS28DFW_200Hz:
        val->odr = LPS28DFW_200Hz;
        break;
      default:
        val->odr = LPS28DFW_ONE_SHOT;
        break;
    }

    switch (ctrl_reg1.avg)
    {
      case LPS28DFW_4_AVG:
        val->avg = LPS28DFW_4_AVG;
        break;
      case LPS28DFW_8_AVG:
        val->avg = LPS28DFW_8_AVG;
        break;
      case LPS28DFW_16_AVG:
        val->avg = LPS28DFW_16_AVG;
        break;
      case LPS28DFW_32_AVG:
        val->avg = LPS28DFW_32_AVG;
        break;
      case LPS28DFW_64_AVG:
        val->avg = LPS28DFW_64_AVG;
        break;
      case LPS28DFW_128_AVG:
        val->avg = LPS28DFW_128_AVG;
        break;
      case LPS28DFW_256_AVG:
        val->avg = LPS28DFW_256_AVG;
        break;
      case LPS28DFW_512_AVG:
        val->avg = LPS28DFW_512_AVG;
        break;
      default:
        val->avg = LPS28DFW_4_AVG;
        break;
    }

    switch ((ctrl_reg2.lfpf_cfg << 2) | ctrl_reg2.en_lpfp)
    {
      case LPS28DFW_LPF_DISABLE:
        val->lpf = LPS28DFW_LPF_DISABLE;
        break;
      case LPS28DFW_LPF_ODR_DIV_4:
        val->lpf = LPS28DFW_LPF_ODR_DIV_4;
        break;
      case LPS28DFW_LPF_ODR_DIV_9:
        val->lpf = LPS28DFW_LPF_ODR_DIV_9;
        break;
      default:
        val->lpf = LPS28DFW_LPF_DISABLE;
        break;
    }
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
int32_t lps28dfw_trigger_sw(const stmdev_ctx_t *ctx, lps28dfw_md_t *md)
{
  lps28dfw_ctrl_reg2_t ctrl_reg2;
  int32_t ret = 0;

  if (md->odr == LPS28DFW_ONE_SHOT)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
    ctrl_reg2.oneshot = PROPERTY_ENABLE;
    if (ret == 0)
    {
      ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
    }
  }
  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrived from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_data_get(const stmdev_ctx_t *ctx, lps28dfw_md_t *md,
                          lps28dfw_data_t *data)
{
  uint8_t buff[5];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_PRESS_OUT_XL, buff, 5);

  /* pressure conversion */
  data->pressure.raw = (int32_t)buff[2];
  data->pressure.raw = (data->pressure.raw * 256) + (int32_t) buff[1];
  data->pressure.raw = (data->pressure.raw * 256) + (int32_t) buff[0];
  data->pressure.raw = data->pressure.raw * 256;

  switch (md->fs)
  {
    case LPS28DFW_1260hPa:
      data->pressure.hpa = lps28dfw_from_fs1260_to_hPa(data->pressure.raw);
      break;
    case LPS28DFW_4000hPa:
      data->pressure.hpa = lps28dfw_from_fs4000_to_hPa(data->pressure.raw);
      break;
    default:
      data->pressure.hpa = 0.0f;
      break;
  }

  /* temperature conversion */
  data->heat.raw = (int16_t)buff[4];
  data->heat.raw = (data->heat.raw * 256) + (int16_t) buff[3];
  data->heat.deg_c = lps28dfw_from_lsb_to_celsius(data->heat.raw);

  return ret;
}

/**
  * @brief  Pressure output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff)
{
  int32_t ret;
  uint8_t reg[3];

  ret =  lps28dfw_read_reg(ctx, LPS28DFW_PRESS_OUT_XL, reg, 3);
  *buff = reg[2];
  *buff = (*buff * 256U) + reg[1];
  *buff = (*buff * 256U) + reg[0];
  *buff *= 256U;

  return ret;
}

/**
  * @brief  Temperature output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff)
{
  int32_t ret;
  uint8_t reg[2];

  ret =  lps28dfw_read_reg(ctx, LPS28DFW_TEMP_OUT_L, reg, 2);
  *buff = (int16_t)reg[1];
  *buff = (*buff * 256) + (int16_t)reg[0];

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
int32_t lps28dfw_fifo_mode_set(const stmdev_ctx_t *ctx, lps28dfw_fifo_md_t *val)
{
  lps28dfw_fifo_ctrl_t fifo_ctrl;
  lps28dfw_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_FIFO_CTRL, reg, 2);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
    bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

    fifo_ctrl.f_mode = (uint8_t)val->operation & 0x03U;
    fifo_ctrl.trig_modes = ((uint8_t)val->operation & 0x04U) >> 2;

    if (val->watermark != 0x00U)
    {
      fifo_ctrl.stop_on_wtm = PROPERTY_ENABLE;
    }
    else
    {
      fifo_ctrl.stop_on_wtm = PROPERTY_DISABLE;
    }

    fifo_wtm.wtm = val->watermark;

    bytecpy(&reg[0], (uint8_t *)&fifo_ctrl);
    bytecpy(&reg[1], (uint8_t *)&fifo_wtm);

    ret = lps28dfw_write_reg(ctx, LPS28DFW_FIFO_CTRL, reg, 2);
  }
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
int32_t lps28dfw_fifo_mode_get(const stmdev_ctx_t *ctx, lps28dfw_fifo_md_t *val)
{
  lps28dfw_fifo_ctrl_t fifo_ctrl;
  lps28dfw_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_FIFO_CTRL, reg, 2);

  bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
  bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

  switch ((fifo_ctrl.trig_modes << 2) | fifo_ctrl.f_mode)
  {
    case LPS28DFW_BYPASS:
      val->operation = LPS28DFW_BYPASS;
      break;
    case LPS28DFW_FIFO:
      val->operation = LPS28DFW_FIFO;
      break;
    case LPS28DFW_STREAM:
      val->operation = LPS28DFW_STREAM;
      break;
    case LPS28DFW_STREAM_TO_FIFO:
      val->operation = LPS28DFW_STREAM_TO_FIFO;
      break;
    case LPS28DFW_BYPASS_TO_STREAM:
      val->operation = LPS28DFW_BYPASS_TO_STREAM;
      break;
    case LPS28DFW_BYPASS_TO_FIFO:
      val->operation = LPS28DFW_BYPASS_TO_FIFO;
      break;
    default:
      val->operation = LPS28DFW_BYPASS;
      break;
  }

  val->watermark = fifo_wtm.wtm;

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
int32_t lps28dfw_fifo_level_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps28dfw_fifo_status1_t fifo_status1;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_FIFO_STATUS1,
                          (uint8_t *)&fifo_status1, 1);

  *val = fifo_status1.fss;

  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  fmd   get the FIFO operation mode.(ptr)
  * @param  samp  number of samples stored in FIFO.(ptr)
  * @param  data  data retrived from FIFO.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_fifo_data_get(const stmdev_ctx_t *ctx, uint8_t samp,
                               lps28dfw_md_t *md, lps28dfw_fifo_data_t *data)
{
  uint8_t fifo_data[3];
  uint8_t i;
  int32_t ret = 0;

  for (i = 0U; i < samp; i++)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_FIFO_DATA_OUT_PRESS_XL, fifo_data, 3);
    data[i].raw = (int32_t)fifo_data[2];
    data[i].raw = (data[i].raw * 256) + (int32_t)fifo_data[1];
    data[i].raw = (data[i].raw * 256) + (int32_t)fifo_data[0];
    data[i].raw = (data[i].raw * 256);

    switch (md->fs)
    {
      case LPS28DFW_1260hPa:
        data[i].hpa = lps28dfw_from_fs1260_to_hPa(data[i].raw);
        break;
      case LPS28DFW_4000hPa:
        data[i].hpa = lps28dfw_from_fs4000_to_hPa(data[i].raw);
        break;
      default:
        data[i].hpa = 0.0f;
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
int32_t lps28dfw_interrupt_mode_set(const stmdev_ctx_t *ctx,
                                    lps28dfw_int_mode_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  lps28dfw_ctrl_reg3_t ctrl_reg3;
  lps28dfw_ctrl_reg4_t ctrl_reg4;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG3, reg, 2);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg3, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg4, &reg[1]);

    ctrl_reg3.int_h_l = val->active_low;
    ctrl_reg4.drdy_pls = ~val->drdy_latched;

    bytecpy(&reg[0], (uint8_t *)&ctrl_reg3);
    bytecpy(&reg[1], (uint8_t *)&ctrl_reg4);

    ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG3, reg, 2);
  }
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }
  if (ret == 0)
  {
    interrupt_cfg.lir = val->int_latched ;
    ret = lps28dfw_write_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                             (uint8_t *)&interrupt_cfg, 1);
  }
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
int32_t lps28dfw_interrupt_mode_get(const stmdev_ctx_t *ctx,
                                    lps28dfw_int_mode_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  lps28dfw_ctrl_reg3_t ctrl_reg3;
  lps28dfw_ctrl_reg4_t ctrl_reg4;
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG3, reg, 2);
  if (ret == 0)
  {
    ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  bytecpy((uint8_t *)&ctrl_reg3, &reg[0]);
  bytecpy((uint8_t *)&ctrl_reg4, &reg[1]);

  val->active_low = ctrl_reg3.int_h_l;
  val->drdy_latched = ~ctrl_reg4.drdy_pls;
  val->int_latched = interrupt_cfg.lir;

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
int32_t lps28dfw_pin_int_route_set(const stmdev_ctx_t *ctx,
                                   lps28dfw_pin_int_route_t *val)
{
  lps28dfw_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  if (ret == 0)
  {
    ctrl_reg4.drdy = val->drdy_pres;
    ctrl_reg4.int_f_wtm = val->fifo_th;
    ctrl_reg4.int_f_ovr = val->fifo_ovr;
    ctrl_reg4.int_f_full = val->fifo_full;

    if ((val->fifo_th != 0U) || (val->fifo_ovr != 0U) || (val->fifo_full != 0U))
    {
      ctrl_reg4.int_en = PROPERTY_ENABLE;
    }
    else
    {
      ctrl_reg4.int_en = PROPERTY_DISABLE;
    }

    ret = lps28dfw_write_reg(ctx, LPS28DFW_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  }
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
int32_t lps28dfw_pin_int_route_get(const stmdev_ctx_t *ctx,
                                   lps28dfw_pin_int_route_t *val)
{
  lps28dfw_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  val->drdy_pres =  ctrl_reg4.drdy;
  val->fifo_th = ctrl_reg4.int_f_wtm;
  val->fifo_ovr = ctrl_reg4.int_f_ovr;
  val->fifo_full = ctrl_reg4.int_f_full;

  return ret;

}

/**
  * @}
  *
  */

/**
  * @defgroup     Interrupt on threshold functions
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
int32_t lps28dfw_int_on_threshold_mode_set(const stmdev_ctx_t *ctx,
                                           lps28dfw_int_th_md_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  lps28dfw_ths_p_l_t ths_p_l;
  lps28dfw_ths_p_h_t ths_p_h;
  uint8_t reg[3];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG, reg, 3);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&interrupt_cfg, &reg[0]);
    bytecpy((uint8_t *)&ths_p_l, &reg[1]);
    bytecpy((uint8_t *)&ths_p_h, &reg[2]);

    interrupt_cfg.phe = val->over_th;
    interrupt_cfg.ple = val->under_th;
    ths_p_h.ths = (uint8_t)(val->threshold / 256U);
    ths_p_l.ths = (uint8_t)(val->threshold - (ths_p_h.ths * 256U));

    bytecpy(&reg[0], (uint8_t *)&interrupt_cfg);
    bytecpy(&reg[1], (uint8_t *)&ths_p_l);
    bytecpy(&reg[2], (uint8_t *)&ths_p_h);

    ret = lps28dfw_write_reg(ctx, LPS28DFW_INTERRUPT_CFG, reg, 3);
  }
  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_int_on_threshold_mode_get(const stmdev_ctx_t *ctx,
                                           lps28dfw_int_th_md_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  lps28dfw_ths_p_l_t ths_p_l;
  lps28dfw_ths_p_h_t ths_p_h;
  uint8_t reg[3];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG, reg, 3);

  bytecpy((uint8_t *)&interrupt_cfg, &reg[0]);
  bytecpy((uint8_t *)&ths_p_l, &reg[1]);
  bytecpy((uint8_t *)&ths_p_h, &reg[2]);

  val->over_th = interrupt_cfg.phe;
  val->under_th = interrupt_cfg.ple;
  val->threshold = ths_p_h.ths;
  val->threshold = (val->threshold * 256U)  + ths_p_l.ths;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Reference value of pressure
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
int32_t lps28dfw_reference_mode_set(const stmdev_ctx_t *ctx, lps28dfw_ref_md_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);
  if (ret == 0)
  {

    interrupt_cfg.autozero = val->get_ref;
    interrupt_cfg.autorefp = (uint8_t)val->apply_ref & 0x01U;

    interrupt_cfg.reset_az  = ((uint8_t)val->apply_ref & 0x02U) >> 1;
    interrupt_cfg.reset_arp = ((uint8_t)val->apply_ref & 0x02U) >> 1;

    ret = lps28dfw_write_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                             (uint8_t *)&interrupt_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_reference_mode_get(const stmdev_ctx_t *ctx, lps28dfw_ref_md_t *val)
{
  lps28dfw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_INTERRUPT_CFG,
                          (uint8_t *)&interrupt_cfg, 1);

  switch ((interrupt_cfg.reset_az << 1) |
          interrupt_cfg.autorefp)
  {
    case LPS28DFW_OUT_AND_INTERRUPT:
      val->apply_ref = LPS28DFW_OUT_AND_INTERRUPT;
      break;
    case LPS28DFW_ONLY_INTERRUPT:
      val->apply_ref = LPS28DFW_ONLY_INTERRUPT;
      break;
    default:
      val->apply_ref = LPS28DFW_RST_REFS;
      break;
  }
  val->get_ref = interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  Reference Pressure LSB data .[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_refp_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_REF_P_L, reg, 2);

  *val = (int16_t)reg[1];
  *val = *val * 256 + (int16_t)reg[0];

  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_opc_set(const stmdev_ctx_t *ctx, int16_t val)
{
  uint8_t reg[2];
  int32_t ret;

  reg[1] = (uint8_t)(((uint16_t)val & 0xFF00U) / 256U);
  reg[0] = (uint8_t)((uint16_t)val & 0x00FFU);

  ret = lps28dfw_write_reg(ctx, LPS28DFW_RPDS_L, reg, 2);

  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lps28dfw_opc_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t reg[2];
  int32_t ret;

  ret = lps28dfw_read_reg(ctx, LPS28DFW_RPDS_L, reg, 2);

  *val = (int16_t)reg[1];
  *val = *val * 256 + (int16_t)reg[0];

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
