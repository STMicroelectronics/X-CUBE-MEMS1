/**
  ******************************************************************************
  * @file    lps33hw_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LPS33HW driver file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "lps33hw_reg.h"

/**
  * @defgroup    LPS33HW
  * @brief       This file provides a set of functions needed to drive the
  *              ultra-compact piezoresistive absolute pressure sensor.
  * @{
  *
  */

/**
  * @defgroup    LPS33HW_Interfaces_functions
  * @brief       This section provide a set of functions used to read and
  *              write a generic register of the device.
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
int32_t __weak lps33hw_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
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
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to write
  * @param  data  pointer to data to write in register reg(ptr)
  * @param  len   number of consecutive register to write
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak lps33hw_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
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
  * @defgroup    LPS33HW_Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lps33hw_from_lsb_to_hpa(int32_t lsb)
{
  return ((float_t)lsb / 4096.0f);
}

float_t lps33hw_from_lsb_to_degc(int16_t lsb)
{
  return ((float_t)lsb / 100.0f);
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS33HW_data_generation_c
  * @brief       This section group all the functions concerning data
  *              generation
  * @{
  *
  */


/**
  * @brief  Reset Autozero function.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of reset_az in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */

int32_t lps33hw_autozero_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_az = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Reset Autozero function.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of reset_az in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_autozero_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_az;

  return ret;
}

/**
  * @brief  Enable Autozero function.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of autozero in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_autozero_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autozero = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enable Autozero function.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of autozero in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_autozero_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  Reset AutoRifP function.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of reset_arp in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_snap_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_arp = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Reset AutoRifP function.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of reset_arp in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_snap_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_arp;

  return ret;
}

/**
  * @brief  Enable AutoRifP function.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of autorifp in reg INTERRUPT_CFG.
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_snap_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autorifp = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enable AutoRifP function.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of autorifp in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_snap_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autorifp;

  return ret;
}

/**
  * @brief  Block data update.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of bdu in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.bdu = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Block data update.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of bdu in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  *val = ctrl_reg1.bdu;

  return ret;
}

/**
  * @brief  Low-pass bandwidth selection.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of lpfp in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_low_pass_filter_mode_set(const stmdev_ctx_t *ctx,
                                         lps33hw_lpfp_t val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.lpfp = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief   Low-pass bandwidth selection.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of lpfp in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_low_pass_filter_mode_get(const stmdev_ctx_t *ctx,
                                         lps33hw_lpfp_t *val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.lpfp)
  {
    case LPS33HW_LPF_ODR_DIV_2:
      *val = LPS33HW_LPF_ODR_DIV_2;
      break;

    case LPS33HW_LPF_ODR_DIV_9:
      *val = LPS33HW_LPF_ODR_DIV_9;
      break;

    case LPS33HW_LPF_ODR_DIV_20:
      *val = LPS33HW_LPF_ODR_DIV_20;
      break;

    default:
      *val = LPS33HW_LPF_ODR_DIV_2;
      break;
  }

  return ret;
}

/**
  * @brief  Output data rate selection.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of odr in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_data_rate_set(const stmdev_ctx_t *ctx, lps33hw_odr_t val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.odr = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Output data rate selection.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of odr in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_data_rate_get(const stmdev_ctx_t *ctx, lps33hw_odr_t *val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.odr)
  {
    case LPS33HW_POWER_DOWN:
      *val = LPS33HW_POWER_DOWN;
      break;

    case LPS33HW_ODR_1_Hz:
      *val = LPS33HW_ODR_1_Hz;
      break;

    case LPS33HW_ODR_10_Hz:
      *val = LPS33HW_ODR_10_Hz;
      break;

    case LPS33HW_ODR_25_Hz:
      *val = LPS33HW_ODR_25_Hz;
      break;

    case LPS33HW_ODR_50_Hz:
      *val = LPS33HW_ODR_50_Hz;
      break;

    case LPS33HW_ODR_75_Hz:
      *val = LPS33HW_ODR_75_Hz;
      break;

    default:
      *val = LPS33HW_ODR_1_Hz;
      break;
  }

  return ret;
}

/**
  * @brief  One-shot mode. Device perform a single measure.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of one_shot in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_one_shoot_trigger_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.one_shot = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  One-shot mode. Device perform a single measure.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of one_shot in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_one_shoot_trigger_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.one_shot;

  return ret;
}

/**
  * @brief  pressure_ref:   The Reference pressure value is a 24-bit data
  *         expressed as 2's complement. The value is used when AUTOZERO
  *         or AUTORIFP function is enabled.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_ref_set(const stmdev_ctx_t *ctx, int32_t val)
{
  uint8_t buff[3];
  int32_t ret;

  buff[2] = (uint8_t)((uint32_t)val / 65536U);
  buff[1] = (uint8_t)((uint32_t)val - (buff[2] * 65536U)) / 256U;
  buff[0] = (uint8_t)((uint32_t)val - (buff[2] * 65536U) -
                      (buff[1] * 256U));
  ret =  lps33hw_write_reg(ctx, LPS33HW_REF_P_XL, buff, 3);

  return ret;
}

/**
  * @brief  pressure_ref:   The Reference pressure value is a 24-bit data
  *         expressed as 2's complement. The value is used when AUTOZERO
  *         or AUTORIFP function is enabled.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_ref_get(const stmdev_ctx_t *ctx, int32_t *val)
{
  uint8_t buff[3];
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_REF_P_XL, buff, 3);
  *val = (int32_t)buff[2];
  *val = (*val * 256) + (int32_t)buff[1];
  *val = (*val * 256) + (int32_t)buff[0];

  return ret;
}

/**
  * @brief  The pressure offset value is 16-bit data that can be used to
  *         implement one-point calibration (OPC) after soldering.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_offset_set(const stmdev_ctx_t *ctx, int16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)((uint16_t)val / 256U);
  buff[0] = (uint8_t)((uint16_t)val - (buff[1] * 256U));
  ret =  lps33hw_write_reg(ctx, LPS33HW_RPDS_L, buff, 2);

  return ret;
}

/**
  * @brief  The pressure offset value is 16-bit data that can be used to
  *         implement one-point calibration (OPC) after soldering.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_offset_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_RPDS_L, buff, 2);
  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  Pressure data available.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of p_da in reg STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_press_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_status_t status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_STATUS, (uint8_t *)&status, 1);
  *val = status.p_da;

  return ret;
}

/**
  * @brief  Temperature data available.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of t_da in reg STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_temp_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_status_t status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_STATUS, (uint8_t *)&status, 1);
  *val = status.t_da;

  return ret;
}

/**
  * @brief  Pressure data overrun.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of p_or in reg STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_press_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_status_t status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_STATUS, (uint8_t *)&status, 1);
  *val = status.p_or;

  return ret;
}

/**
  * @brief  Temperature data overrun.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of t_or in reg STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_temp_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_status_t status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_STATUS, (uint8_t *)&status, 1);
  *val = status.t_or;

  return ret;
}

/**
  * @brief  Pressure output value[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff)
{
  uint8_t reg[3];
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_PRESS_OUT_XL, reg, 3);
  *buff = reg[2];
  *buff = (*buff * 256) + reg[1];
  *buff = (*buff * 256) + reg[0];
  *buff *= 256;

  return ret;
}

/**
  * @brief  temperature_raw:   Temperature output value[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read.
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff)
{
  uint8_t reg[2];
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_TEMP_OUT_L, (uint8_t *) reg, 2);
  *buff = reg[1];
  *buff = (*buff * 256) + reg[0];

  return ret;
}

/**
  * @brief  Low-pass filter reset register. If the LPFP is active, in
  *         order to avoid the transitory phase, the filter can be
  *         reset by reading this register before generating pressure
  *         measurements.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_low_pass_rst_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_LPFP_RES, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS33HW_common
  * @brief       This section group common useful functions
  * @{
  *
  */

/**
  * @brief  Device Who am I[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_device_id_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_WHO_AM_I, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @brief  Software reset. Restore the default values in user registers[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of swreset in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_reset_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.swreset = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  Software reset. Restore the default values in user registers[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of swreset in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_reset_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.swreset;

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of boot in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_boot_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.boot = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of boot in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_boot_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.boot;

  return ret;
}

/**
  * @brief  Low current mode.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of lc_en in reg RES_CONF
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_low_power_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_res_conf_t res_conf;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_RES_CONF, (uint8_t *)&res_conf, 1);

  if (ret == 0)
  {
    res_conf.lc_en = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_RES_CONF, (uint8_t *)&res_conf, 1);
  }

  return ret;
}

/**
  * @brief  Low current mode.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of lc_en in reg RES_CONF
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_low_power_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_res_conf_t res_conf;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_RES_CONF, (uint8_t *)&res_conf, 1);
  *val = res_conf.lc_en;

  return ret;
}

/**
  * @brief  If '1' indicates that the Boot (Reboot) phase is running.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of boot_status in reg INT_SOURCE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_boot_status_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_int_source_t int_source;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.boot_status;

  return ret;
}

/**
  * @brief  All the status bit, FIFO and data generation[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Structure of registers from FIFO_STATUS to STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_dev_status_get(const stmdev_ctx_t *ctx,
                               lps33hw_dev_stat_t *val)
{
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_FIFO_STATUS, (uint8_t *) val, 2);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS33HW_interrupts
  * @brief       This section group all the functions that manage interrupts
  * @{
  *
  */

/**
  * @brief  Enable interrupt generation on pressure low/high event.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of pe in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_sign_of_int_threshold_set(const stmdev_ctx_t *ctx,
                                          lps33hw_pe_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.pe = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enable interrupt generation on pressure low/high event.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of pe in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_sign_of_int_threshold_get(const stmdev_ctx_t *ctx,
                                          lps33hw_pe_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.pe)
  {
    case LPS33HW_NO_THRESHOLD:
      *val = LPS33HW_NO_THRESHOLD;
      break;

    case LPS33HW_POSITIVE:
      *val = LPS33HW_POSITIVE;
      break;

    case LPS33HW_NEGATIVE:
      *val = LPS33HW_NEGATIVE;
      break;

    case LPS33HW_BOTH:
      *val = LPS33HW_BOTH;
      break;

    default:
      *val = LPS33HW_NO_THRESHOLD;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt request to the INT_SOURCE (25h) register
  *         mode (pulsed / latched) [set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of lir in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_notification_mode_set(const stmdev_ctx_t *ctx,
                                          lps33hw_lir_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.lir = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief   Interrupt request to the INT_SOURCE (25h) register
  *          mode (pulsed / latched) [get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of lir in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_notification_mode_get(const stmdev_ctx_t *ctx,
                                          lps33hw_lir_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.lir)
  {
    case LPS33HW_INT_PULSED:
      *val = LPS33HW_INT_PULSED;
      break;

    case LPS33HW_INT_LATCHED:
      *val = LPS33HW_INT_LATCHED;
      break;

    default:
      *val = LPS33HW_INT_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  Enable interrupt generation.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of diff_en in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_generation_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.diff_en = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  Enable interrupt generation.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of diff_en in reg INTERRUPT_CFG
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_generation_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.diff_en;

  return ret;
}

/**
  * @brief  User-defined threshold value for pressure interrupt event[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that contains data to write
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_threshold_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret =  lps33hw_write_reg(ctx, LPS33HW_THS_P_L, (uint8_t *) buff, 2);

  return ret;
}

/**
  * @brief  User-defined threshold value for pressure interrupt event[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  buff   Buffer that stores data read
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_threshold_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_THS_P_L, (uint8_t *) buff, 2);
  *val = buff[1];
  *val = (*val * 256) + buff[0];

  return ret;
}

/**
  * @brief  Data signal on INT_DRDY pin control bits.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of int_s in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                 lps33hw_int_s_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_s = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  Data signal on INT_DRDY pin control bits.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of int_s in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                 lps33hw_int_s_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_s)
  {
    case LPS33HW_DRDY_OR_FIFO_FLAGS:
      *val = LPS33HW_DRDY_OR_FIFO_FLAGS;
      break;

    case LPS33HW_HIGH_PRES_INT:
      *val = LPS33HW_HIGH_PRES_INT;
      break;

    case LPS33HW_LOW_PRES_INT:
      *val = LPS33HW_LOW_PRES_INT;
      break;

    case LPS33HW_EVERY_PRES_INT:
      *val = LPS33HW_EVERY_PRES_INT;
      break;

    default:
      *val = LPS33HW_DRDY_OR_FIFO_FLAGS;
      break;
  }

  return ret;
}

/**
  * @brief  Data-ready signal on INT_DRDY pin.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of drdy in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_drdy_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.drdy = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  Data-ready signal on INT_DRDY pin.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of drdy in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_drdy_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.drdy;

  return ret;
}

/**
  * @brief  FIFO overrun interrupt on INT_DRDY pin.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_ovr in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_ovr_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_ovr = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO overrun interrupt on INT_DRDY pin.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_ovr in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_ovr_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_ovr;

  return ret;
}

/**
  * @brief  FIFO watermark status on INT_DRDY pin.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_fth in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_threshold_on_int_set(const stmdev_ctx_t *ctx,
                                          uint8_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fth = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief   FIFO watermark status on INT_DRDY pin.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_fth in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_threshold_on_int_get(const stmdev_ctx_t *ctx,
                                          uint8_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fth;

  return ret;
}

/**
  * @brief  FIFO full flag on INT_DRDY pin.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_fss5 in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_full_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fss5 = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO full flag on INT_DRDY pin.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_fss5 in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_full_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fss5;

  return ret;
}

/**
  * @brief  Push-pull/open drain selection on interrupt pads.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of pp_od in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pin_mode_set(const stmdev_ctx_t *ctx, lps33hw_pp_od_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.pp_od = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  Push-pull/open drain selection on interrupt pads.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of pp_od in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_pin_mode_get(const stmdev_ctx_t *ctx, lps33hw_pp_od_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.pp_od)
  {
    case LPS33HW_PUSH_PULL:
      *val = LPS33HW_PUSH_PULL;
      break;

    case LPS33HW_OPEN_DRAIN:
      *val = LPS33HW_OPEN_DRAIN;
      break;

    default:
      *val = LPS33HW_PUSH_PULL;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt active-high/low.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of int_h_l in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_polarity_set(const stmdev_ctx_t *ctx,
                                 lps33hw_int_h_l_t val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_h_l = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  Interrupt active-high/low.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of int_h_l in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_polarity_get(const stmdev_ctx_t *ctx,
                                 lps33hw_int_h_l_t *val)
{
  lps33hw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_h_l)
  {
    case LPS33HW_ACTIVE_HIGH:
      *val = LPS33HW_ACTIVE_HIGH;
      break;

    case LPS33HW_ACTIVE_LOW:
      *val = LPS33HW_ACTIVE_LOW;
      break;

    default:
      *val = LPS33HW_ACTIVE_HIGH;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt source register[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Register INT_SOURCE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_source_get(const stmdev_ctx_t *ctx,
                               lps33hw_int_source_t *val)
{
  int32_t ret;

  ret =  lps33hw_read_reg(ctx, LPS33HW_INT_SOURCE, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Differential pressure high interrupt flag.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of ph in reg INT_SOURCE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_on_press_high_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_int_source_t int_source;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ph;

  return ret;
}

/**
  * @brief  Differential pressure low interrupt flag.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of pl in reg INT_SOURCE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_int_on_press_low_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_int_source_t int_source;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.pl;

  return ret;
}

/**
  * @brief  Interrupt active flag.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of ia in reg INT_SOURCE
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_interrupt_event_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_int_source_t int_source;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ia;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS33HW_fifo
  * @brief       This section group all the functions concerning the
  *              fifo usage
  * @{
  *
  */

/**
  * @brief   Stop on FIFO watermark. Enable FIFO watermark level use.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of stop_on_fth in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_stop_on_fifo_threshold_set(const stmdev_ctx_t *ctx,
                                           uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.stop_on_fth = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief   Stop on FIFO watermark. Enable FIFO watermark level use.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of stop_on_fth in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_stop_on_fifo_threshold_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.stop_on_fth;

  return ret;
}

/**
  * @brief  FIFO enable.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of fifo_en in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.fifo_en = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  FIFO enable.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of fifo_en in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.fifo_en;

  return ret;
}

/**
  * @brief  FIFO watermark level selection.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of wtm in reg FIFO_CTRL
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.wtm = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO watermark level selection.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of wtm in reg FIFO_CTRL
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  *val = fifo_ctrl.wtm;

  return ret;
}

/**
  * @brief  FIFO mode selection.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of f_mode in reg FIFO_CTRL
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_mode_set(const stmdev_ctx_t *ctx, lps33hw_f_mode_t val)
{
  lps33hw_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.f_mode = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of f_mode in reg FIFO_CTRL
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_mode_get(const stmdev_ctx_t *ctx,
                              lps33hw_f_mode_t *val)
{
  lps33hw_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  switch (fifo_ctrl.f_mode)
  {
    case LPS33HW_BYPASS_MODE:
      *val = LPS33HW_BYPASS_MODE;
      break;

    case LPS33HW_FIFO_MODE:
      *val = LPS33HW_FIFO_MODE;
      break;

    case LPS33HW_STREAM_MODE:
      *val = LPS33HW_STREAM_MODE;
      break;

    case LPS33HW_STREAM_TO_FIFO_MODE:
      *val = LPS33HW_STREAM_TO_FIFO_MODE;
      break;

    case LPS33HW_BYPASS_TO_STREAM_MODE:
      *val = LPS33HW_BYPASS_TO_STREAM_MODE;
      break;

    case LPS33HW_DYNAMIC_STREAM_MODE:
      *val = LPS33HW_DYNAMIC_STREAM_MODE;
      break;

    case LPS33HW_BYPASS_TO_FIFO_MODE:
      *val = LPS33HW_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = LPS33HW_BYPASS_MODE;
      break;
  }

  return ret;
}

/**
  * @brief  FIFO stored data level.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of fss in reg FIFO_STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_data_level_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fss;

  return ret;
}

/**
  * @brief  FIFO overrun status.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of ovr in reg FIFO_STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_ovr_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.ovr;

  return ret;
}

/**
  * @brief  FIFO watermark status.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of fth_fifo in reg FIFO_STATUS
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_fifo_fth_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fth_fifo;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS33HW_serial_interface
  * @brief       This section group all the functions concerning serial
  *              interface management
  * @{
  *
  */

/**
  * @brief  SPI Serial Interface Mode selection.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of sim in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_spi_mode_set(const stmdev_ctx_t *ctx, lps33hw_sim_t val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.sim = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  SPI Serial Interface Mode selection.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of sim in reg CTRL_REG1
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_spi_mode_get(const stmdev_ctx_t *ctx, lps33hw_sim_t *val)
{
  lps33hw_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.sim)
  {
    case LPS33HW_SPI_4_WIRE:
      *val = LPS33HW_SPI_4_WIRE;
      break;

    case LPS33HW_SPI_3_WIRE:
      *val = LPS33HW_SPI_3_WIRE;
      break;

    default:
      *val = LPS33HW_SPI_4_WIRE;
      break;
  }

  return ret;
}

/**
  * @brief  Disable I2C interface.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of i2c_dis in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_i2c_interface_set(const stmdev_ctx_t *ctx,
                                  lps33hw_i2c_dis_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.i2c_dis = (uint8_t)val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  Disable I2C interface.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Get the values of i2c_dis in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_i2c_interface_get(const stmdev_ctx_t *ctx,
                                  lps33hw_i2c_dis_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.i2c_dis)
  {
    case LPS33HW_I2C_ENABLE:
      *val = LPS33HW_I2C_ENABLE;
      break;

    case LPS33HW_I2C_DISABLE:
      *val = LPS33HW_I2C_DISABLE;
      break;

    default:
      *val = LPS33HW_I2C_ENABLE;
      break;
  }

  return ret;
}

/**
  * @brief  Register address automatically incremented during a
  *         multiple byte access with a serial interface (I2C or SPI).[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of if_add_inc in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_auto_add_inc_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.if_add_inc = val;
    ret = lps33hw_write_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  Register address automatically incremented during a
  *         multiple byte access with a serial interface (I2C or SPI).[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the values of if_add_inc in reg CTRL_REG2
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t lps33hw_auto_add_inc_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps33hw_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps33hw_read_reg(ctx, LPS33HW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.if_add_inc;

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
