/**
  ******************************************************************************
  * @file    lsm6dso16is_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          lsm6dso16is_reg.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSO16IS_REGS_H
#define LSM6DSO16IS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LSM6DSO16IS
  * @{
  *
  */

/** @defgroup  Endianness definitions
  * @{
  *
  */

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN    4321

/** if _BYTE_ORDER is not defined, choose the endianness of your architecture
  * by uncommenting the define which fits your platform endianness
  */
//#define DRV_BYTE_ORDER    DRV_BIG_ENDIAN
#define DRV_BYTE_ORDER    DRV_LITTLE_ENDIAN

#else /* defined __BYTE_ORDER__ */

#define DRV_LITTLE_ENDIAN  __ORDER_LITTLE_ENDIAN__
#define DRV_BIG_ENDIAN     __ORDER_BIG_ENDIAN__
#define DRV_BYTE_ORDER     __BYTE_ORDER__

#endif /* __BYTE_ORDER__*/
#endif /* DRV_BYTE_ORDER */

/**
  * @}
  *
  */

/** @defgroup STMicroelectronics sensors common types
  * @{
  *
  */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                         : 1;
  uint8_t bit1                         : 1;
  uint8_t bit2                         : 1;
  uint8_t bit3                         : 1;
  uint8_t bit4                         : 1;
  uint8_t bit5                         : 1;
  uint8_t bit6                         : 1;
  uint8_t bit7                         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                         : 1;
  uint8_t bit6                         : 1;
  uint8_t bit5                         : 1;
  uint8_t bit4                         : 1;
  uint8_t bit3                         : 1;
  uint8_t bit2                         : 1;
  uint8_t bit1                         : 1;
  uint8_t bit0                         : 1;
#endif /* DRV_BYTE_ORDER */
} bitwise_t;

#define PROPERTY_DISABLE                (0U)
#define PROPERTY_ENABLE                 (1U)

/** @addtogroup  Interfaces_Functions
  * @brief       This section provide a set of functions used to read and
  *              write a generic register of the device.
  *              MANDATORY: return 0 -> no Error.
  * @{
  *
  */

typedef int32_t (*stmdev_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);
typedef int32_t (*stmdev_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);
typedef void (*stmdev_mdelay_ptr)(uint32_t millisec);

typedef struct
{
  /** Component mandatory fields **/
  stmdev_write_ptr  write_reg;
  stmdev_read_ptr   read_reg;
  /** Component optional fields **/
  stmdev_mdelay_ptr   mdelay;
  /** Customizable optional pointer **/
  void *handle;
} stmdev_ctx_t;

/**
  * @}
  *
  */

#endif /* MEMS_SHARED_TYPES */

#ifndef MEMS_UCF_SHARED_TYPES
#define MEMS_UCF_SHARED_TYPES

/** @defgroup    Generic address-data structure definition
  * @brief       This structure is useful to load a predefined configuration
  *              of a sensor.
  *              You can create a sensor configuration by your own or using
  *              Unico / Unicleo tools available on STMicroelectronics
  *              web site.
  *
  * @{
  *
  */

typedef struct
{
  uint8_t address;
  uint8_t data;
} ucf_line_t;

/**
  * @}
  *
  */

#endif /* MEMS_UCF_SHARED_TYPES */

/**
  * @}
  *
  */

/** @defgroup LSM6DSO16IS_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define LSM6DSO16IS_I2C_ADD_L                       0xD5U
#define LSM6DSO16IS_I2C_ADD_H                       0xD7U

/** Device Identification (Who am I) **/
#define LSM6DSO16IS_ID                              0x22U

/**
  * @}
  *
  */

/** @defgroup bitfields page main
  * @{
  *
  */

#define LSM6DSO16IS_FUNC_CFG_ACCESS                 0x1U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t sw_reset_ispu                : 1;
  uint8_t not_used1                    : 4;
  uint8_t shub_reg_access              : 1;
  uint8_t ispu_reg_access              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_reg_access              : 1;
  uint8_t shub_reg_access              : 1;
  uint8_t not_used1                    : 4;
  uint8_t sw_reset_ispu                : 1;
  uint8_t not_used0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_func_cfg_access_t;

#define LSM6DSO16IS_PIN_CTRL                        0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 6;
  uint8_t sdo_pu_en                    : 1;
  uint8_t not_used1                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 1;
  uint8_t sdo_pu_en                    : 1;
  uint8_t not_used0                    : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_pin_ctrl_t;

#define LSM6DSO16IS_DRDY_PULSED_REG                 0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 7;
  uint8_t drdy_pulsed                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t drdy_pulsed                  : 1;
  uint8_t not_used0                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_drdy_pulsed_reg_t;

#define LSM6DSO16IS_INT1_CTRL                       0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_drdy_xl                 : 1;
  uint8_t int1_drdy_g                  : 1;
  uint8_t int1_boot                    : 1;
  uint8_t not_used0                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t int1_boot                    : 1;
  uint8_t int1_drdy_g                  : 1;
  uint8_t int1_drdy_xl                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_int1_ctrl_t;

#define LSM6DSO16IS_INT2_CTRL                       0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_drdy_xl                 : 1;
  uint8_t int2_drdy_g                  : 1;
  uint8_t int2_drdy_temp               : 1;
  uint8_t not_used0                    : 4;
  uint8_t int2_sleep_ispu              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_sleep_ispu              : 1;
  uint8_t not_used0                    : 4;
  uint8_t int2_drdy_temp               : 1;
  uint8_t int2_drdy_g                  : 1;
  uint8_t int2_drdy_xl                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_int2_ctrl_t;

#define LSM6DSO16IS_WHO_AM_I                        0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t id                           : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t id                           : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_who_am_i_t;

#define LSM6DSO16IS_CTRL1_XL                        0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t fs_xl                        : 2;
  uint8_t odr_xl                       : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_xl                       : 4;
  uint8_t fs_xl                        : 2;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl1_xl_t;

#define LSM6DSO16IS_CTRL2_G                         0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t fs_125                       : 1;
  uint8_t fs_g                         : 2;
  uint8_t odr_g                        : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_g                        : 4;
  uint8_t fs_g                         : 2;
  uint8_t fs_125                       : 1;
  uint8_t not_used0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl2_g_t;

#define LSM6DSO16IS_CTRL3_C                         0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sw_reset                     : 1;
  uint8_t not_used0                    : 1;
  uint8_t if_inc                       : 1;
  uint8_t sim                          : 1;
  uint8_t pp_od                        : 1;
  uint8_t h_lactive                    : 1;
  uint8_t bdu                          : 1;
  uint8_t boot                         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot                         : 1;
  uint8_t bdu                          : 1;
  uint8_t h_lactive                    : 1;
  uint8_t pp_od                        : 1;
  uint8_t sim                          : 1;
  uint8_t if_inc                       : 1;
  uint8_t not_used0                    : 1;
  uint8_t sw_reset                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl3_c_t;

#define LSM6DSO16IS_CTRL4_C                         0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t i2c_disable                  : 1;
  uint8_t not_used1                    : 2;
  uint8_t int2_on_int1                 : 1;
  uint8_t sleep_g                      : 1;
  uint8_t not_used2                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used2                    : 1;
  uint8_t sleep_g                      : 1;
  uint8_t int2_on_int1                 : 1;
  uint8_t not_used1                    : 2;
  uint8_t i2c_disable                  : 1;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl4_c_t;

#define LSM6DSO16IS_CTRL5_C                         0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_xl                        : 2;
  uint8_t st_g                         : 2;
  uint8_t not_used0                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t st_g                         : 2;
  uint8_t st_xl                        : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl5_c_t;

#define LSM6DSO16IS_CTRL6_C                         0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t xl_hm_mode                   : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t xl_hm_mode                   : 1;
  uint8_t not_used0                    : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl6_c_t;

#define LSM6DSO16IS_CTRL7_G                         0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 7;
  uint8_t g_hm_mode                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t g_hm_mode                    : 1;
  uint8_t not_used0                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl7_g_t;

#define LSM6DSO16IS_CTRL9_C                         0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_bdu                     : 2;
  uint8_t not_used0                    : 2;
  uint8_t ispu_rate                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_rate                    : 4;
  uint8_t not_used0                    : 2;
  uint8_t ispu_bdu                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl9_c_t;

#define LSM6DSO16IS_CTRL10_C                        0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ispu_clk_sel                 : 1;
  uint8_t not_used1                    : 2;
  uint8_t timestamp_en                 : 1;
  uint8_t not_used2                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used2                    : 2;
  uint8_t timestamp_en                 : 1;
  uint8_t not_used1                    : 2;
  uint8_t ispu_clk_sel                 : 1;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ctrl10_c_t;

#define LSM6DSO16IS_ISPU_INT_STATUS0_MAINPAGE       0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ia_ispu                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ia_ispu                      : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status0_mainpage_t;

#define LSM6DSO16IS_ISPU_INT_STATUS1_MAINPAGE       0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ia_ispu                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ia_ispu                      : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status1_mainpage_t;

#define LSM6DSO16IS_ISPU_INT_STATUS2_MAINPAGE       0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ia_ispu                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ia_ispu                      : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status2_mainpage_t;

#define LSM6DSO16IS_ISPU_INT_STATUS3_MAINPAGE       0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ia_ispu                      : 6;
  uint8_t not_used0                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ia_ispu                      : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status3_mainpage_t;

#define LSM6DSO16IS_STATUS_REG                      0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlda                         : 1;
  uint8_t gda                          : 1;
  uint8_t tda                          : 1;
  uint8_t not_used0                    : 4;
  uint8_t timestamp_endcount           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_endcount           : 1;
  uint8_t not_used0                    : 4;
  uint8_t tda                          : 1;
  uint8_t gda                          : 1;
  uint8_t xlda                         : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_status_reg_t;

#define LSM6DSO16IS_OUT_TEMP_L                      0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_out_temp_l_t;

#define LSM6DSO16IS_OUT_TEMP_H                      0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_out_temp_h_t;

#define LSM6DSO16IS_OUTX_L_G                        0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outx_l_g_t;

#define LSM6DSO16IS_OUTX_H_G                        0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outx_h_g_t;

#define LSM6DSO16IS_OUTY_L_G                        0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outy_l_g_t;

#define LSM6DSO16IS_OUTY_H_G                        0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outy_h_g_t;

#define LSM6DSO16IS_OUTZ_L_G                        0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outz_l_g_t;

#define LSM6DSO16IS_OUTZ_H_G                        0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outz_h_g_t;

#define LSM6DSO16IS_OUTX_L_A                        0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outx_l_a_t;

#define LSM6DSO16IS_OUTX_H_A                        0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outx_h_a_t;

#define LSM6DSO16IS_OUTY_L_A                        0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outy_l_a_t;

#define LSM6DSO16IS_OUTY_H_A                        0x2BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outy_h_a_t;

#define LSM6DSO16IS_OUTZ_L_A                        0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outz_l_a_t;

#define LSM6DSO16IS_OUTZ_H_A                        0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_outz_h_a_t;

#define LSM6DSO16IS_STATUS_MASTER_MAINPAGE          0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sens_hub_endop               : 1;
  uint8_t not_used0                    : 2;
  uint8_t slave0_nack                  : 1;
  uint8_t slave1_nack                  : 1;
  uint8_t slave2_nack                  : 1;
  uint8_t slave3_nack                  : 1;
  uint8_t wr_once_done                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wr_once_done                 : 1;
  uint8_t slave3_nack                  : 1;
  uint8_t slave2_nack                  : 1;
  uint8_t slave1_nack                  : 1;
  uint8_t slave0_nack                  : 1;
  uint8_t not_used0                    : 2;
  uint8_t sens_hub_endop               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_status_master_mainpage_t;

#define LSM6DSO16IS_TIMESTAMP0                      0x40U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_timestamp0_t;

#define LSM6DSO16IS_TIMESTAMP1                      0x41U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_timestamp1_t;

#define LSM6DSO16IS_TIMESTAMP2                      0x42U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_timestamp2_t;

#define LSM6DSO16IS_TIMESTAMP3                      0x43U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_timestamp3_t;

#define LSM6DSO16IS_MD1_CFG                         0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_shub                    : 1;
  uint8_t int1_ispu                    : 1;
  uint8_t not_used0                    : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 6;
  uint8_t int1_ispu                    : 1;
  uint8_t int1_shub                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_md1_cfg_t;

#define LSM6DSO16IS_MD2_CFG                         0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_timestamp               : 1;
  uint8_t int2_ispu                    : 1;
  uint8_t not_used0                    : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 6;
  uint8_t int2_ispu                    : 1;
  uint8_t int2_timestamp               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_md2_cfg_t;

#define LSM6DSO16IS_INTERNAL_FREQ_FINE              0x63U
typedef struct
{
  uint8_t freq_fine                    : 8;
} lsm6dso16is_internal_freq_fine_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_1_L              0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_1             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_1             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_1_l_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_1_H              0x74U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_1             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_1             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_1_h_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_2_L              0x75U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_2             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_2             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_2_l_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_2_H              0x76U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_2             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_2             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_2_h_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_3_L              0x77U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_3             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_3             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_3_l_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_3_H              0x78U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_3             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_3             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_3_h_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_4_L              0x79U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_4             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_4             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_4_l_t;

#define LSM6DSO16IS_ISPU_DUMMY_CFG_4_H              0x7AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_dummy_cfg_4             : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_dummy_cfg_4             : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dummy_cfg_4_h_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page sensor_hub
  * @{
  *
  */

#define LSM6DSO16IS_SENSOR_HUB_1                    0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub1                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub1                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_1_t;

#define LSM6DSO16IS_SENSOR_HUB_2                    0x3U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub2                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub2                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_2_t;

#define LSM6DSO16IS_SENSOR_HUB_3                    0x4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub3                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub3                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_3_t;

#define LSM6DSO16IS_SENSOR_HUB_4                    0x5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub4                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub4                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_4_t;

#define LSM6DSO16IS_SENSOR_HUB_5                    0x6U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub5                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub5                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_5_t;

#define LSM6DSO16IS_SENSOR_HUB_6                    0x7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub6                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub6                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_6_t;

#define LSM6DSO16IS_SENSOR_HUB_7                    0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub7                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub7                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_7_t;

#define LSM6DSO16IS_SENSOR_HUB_8                    0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub8                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub8                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_8_t;

#define LSM6DSO16IS_SENSOR_HUB_9                    0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub9                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub9                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_9_t;

#define LSM6DSO16IS_SENSOR_HUB_10                   0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub10                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub10                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_10_t;

#define LSM6DSO16IS_SENSOR_HUB_11                   0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub11                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub11                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_11_t;

#define LSM6DSO16IS_SENSOR_HUB_12                   0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub12                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub12                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_12_t;

#define LSM6DSO16IS_SENSOR_HUB_13                   0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub13                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub13                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_13_t;

#define LSM6DSO16IS_SENSOR_HUB_14                   0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub14                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub14                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_14_t;

#define LSM6DSO16IS_SENSOR_HUB_15                   0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub15                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub15                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_15_t;

#define LSM6DSO16IS_SENSOR_HUB_16                   0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub16                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub16                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_16_t;

#define LSM6DSO16IS_SENSOR_HUB_17                   0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub17                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub17                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_17_t;

#define LSM6DSO16IS_SENSOR_HUB_18                   0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sensorhub18                  : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sensorhub18                  : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_sensor_hub_18_t;

#define LSM6DSO16IS_MASTER_CONFIG                   0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t aux_sens_on                  : 2;
  uint8_t master_on                    : 1;
  uint8_t shub_pu_en                   : 1;
  uint8_t pass_through_mode            : 1;
  uint8_t start_config                 : 1;
  uint8_t write_once                   : 1;
  uint8_t rst_master_regs              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t rst_master_regs              : 1;
  uint8_t write_once                   : 1;
  uint8_t start_config                 : 1;
  uint8_t pass_through_mode            : 1;
  uint8_t shub_pu_en                   : 1;
  uint8_t master_on                    : 1;
  uint8_t aux_sens_on                  : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_master_config_t;

#define LSM6DSO16IS_SLV0_ADD                        0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t rw_0                         : 1;
  uint8_t slave0_add                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave0_add                   : 7;
  uint8_t rw_0                         : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv0_add_t;

#define LSM6DSO16IS_SLV0_SUBADD                     0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave0_reg                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave0_reg                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv0_subadd_t;

#define LSM6DSO16IS_SLV0_CONFIG                   0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave0_numop                 : 3;
  uint8_t not_used0                    : 3;
  uint8_t shub_odr                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t shub_odr                     : 2;
  uint8_t not_used0                    : 3;
  uint8_t slave0_numop                 : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv0_config_t;

#define LSM6DSO16IS_SLV1_ADD                        0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_1                          : 1;
  uint8_t slave1_add                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave1_add                   : 7;
  uint8_t r_1                          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv1_add_t;

#define LSM6DSO16IS_SLV1_SUBADD                     0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave1_reg                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave1_reg                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv1_subadd_t;

#define LSM6DSO16IS_SLV1_CONFIG                   0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave1_numop                 : 3;
  uint8_t not_used0                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t slave1_numop                 : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv1_config_t;

#define LSM6DSO16IS_SLV2_ADD                        0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_2                          : 1;
  uint8_t slave2_add                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave2_add                   : 7;
  uint8_t r_2                          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv2_add_t;

#define LSM6DSO16IS_SLV2_SUBADD                     0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave2_reg                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave2_reg                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv2_subadd_t;

#define LSM6DSO16IS_SLV2_CONFIG                   0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave2_numop                 : 3;
  uint8_t not_used0                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t slave2_numop                 : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv2_config_t;

#define LSM6DSO16IS_SLV3_ADD                        0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_3                          : 1;
  uint8_t slave3_add                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave3_add                   : 7;
  uint8_t r_3                          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv3_add_t;

#define LSM6DSO16IS_SLV3_SUBADD                     0x1FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave3_reg                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave3_reg                   : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv3_subadd_t;

#define LSM6DSO16IS_SLV3_CONFIG                   0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave3_numop                 : 3;
  uint8_t not_used0                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t slave3_numop                 : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_slv3_config_t;

#define LSM6DSO16IS_DATAWRITE_SLV0                  0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave0_dataw                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave0_dataw                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_datawrite_slv0_t;

#define LSM6DSO16IS_STATUS_MASTER                   0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sens_hub_endop               : 1;
  uint8_t not_used0                    : 2;
  uint8_t slave0_nack                  : 1;
  uint8_t slave1_nack                  : 1;
  uint8_t slave2_nack                  : 1;
  uint8_t slave3_nack                  : 1;
  uint8_t wr_once_done                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wr_once_done                 : 1;
  uint8_t slave3_nack                  : 1;
  uint8_t slave2_nack                  : 1;
  uint8_t slave1_nack                  : 1;
  uint8_t slave0_nack                  : 1;
  uint8_t not_used0                    : 2;
  uint8_t sens_hub_endop               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_status_master_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page ispu
  * @{
  *
  */

#define LSM6DSO16IS_ISPU_CONFIG                     0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_rst_n                   : 1;
  uint8_t clk_dis                      : 1;
  uint8_t not_used0                    : 2;
  uint8_t latched                      : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t latched                      : 1;
  uint8_t not_used0                    : 2;
  uint8_t clk_dis                      : 1;
  uint8_t ispu_rst_n                   : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_config_t;

#define LSM6DSO16IS_ISPU_STATUS                     0x4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t boot_end                     : 1;
  uint8_t not_used1                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 5;
  uint8_t boot_end                     : 1;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_status_t;

#define LSM6DSO16IS_ISPU_MEM_SEL                    0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mem_sel                      : 1;
  uint8_t not_used0                    : 5;
  uint8_t read_mem_en                  : 1;
  uint8_t not_used1                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 1;
  uint8_t read_mem_en                  : 1;
  uint8_t not_used0                    : 5;
  uint8_t mem_sel                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_mem_sel_t;

#define LSM6DSO16IS_ISPU_MEM_ADDR1                  0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mem_addr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mem_addr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_mem_addr1_t;

#define LSM6DSO16IS_ISPU_MEM_ADDR0                  0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mem_addr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mem_addr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_mem_addr0_t;

#define LSM6DSO16IS_ISPU_MEM_DATA                   0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mem_data                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mem_data                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_mem_data_t;

#define LSM6DSO16IS_ISPU_IF2S_FLAG_L                0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t if2s                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t if2s                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_if2s_flag_l_t;

#define LSM6DSO16IS_ISPU_IF2S_FLAG_H                0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t if2s                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t if2s                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_if2s_flag_h_t;

#define LSM6DSO16IS_ISPU_S2IF_FLAG_L                0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t s2if                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t s2if                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_s2if_flag_l_t;

#define LSM6DSO16IS_ISPU_S2IF_FLAG_H                0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t s2if                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t s2if                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_s2if_flag_h_t;

#define LSM6DSO16IS_ISPU_DOUT_00_L                  0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout0                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout0                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_00_l_t;

#define LSM6DSO16IS_ISPU_DOUT_00_H                  0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout0                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout0                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_00_h_t;

#define LSM6DSO16IS_ISPU_DOUT_01_L                  0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout1                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout1                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_01_l_t;

#define LSM6DSO16IS_ISPU_DOUT_01_H                  0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout1                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout1                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_01_h_t;

#define LSM6DSO16IS_ISPU_DOUT_02_L                  0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout2                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout2                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_02_l_t;

#define LSM6DSO16IS_ISPU_DOUT_02_H                  0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout2                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout2                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_02_h_t;

#define LSM6DSO16IS_ISPU_DOUT_03_L                  0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout3                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout3                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_03_l_t;

#define LSM6DSO16IS_ISPU_DOUT_03_H                  0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout3                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout3                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_03_h_t;

#define LSM6DSO16IS_ISPU_DOUT_04_L                  0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout4                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout4                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_04_l_t;

#define LSM6DSO16IS_ISPU_DOUT_04_H                  0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout4                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout4                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_04_h_t;

#define LSM6DSO16IS_ISPU_DOUT_05_L                  0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout5                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout5                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_05_l_t;

#define LSM6DSO16IS_ISPU_DOUT_05_H                  0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout5                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout5                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_05_h_t;

#define LSM6DSO16IS_ISPU_DOUT_06_L                  0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout6                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout6                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_06_l_t;

#define LSM6DSO16IS_ISPU_DOUT_06_H                  0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout6                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout6                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_06_h_t;

#define LSM6DSO16IS_ISPU_DOUT_07_L                  0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout7                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout7                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_07_l_t;

#define LSM6DSO16IS_ISPU_DOUT_07_H                  0x1FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout7                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout7                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_07_h_t;

#define LSM6DSO16IS_ISPU_DOUT_08_L                  0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout8                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout8                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_08_l_t;

#define LSM6DSO16IS_ISPU_DOUT_08_H                  0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout8                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout8                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_08_h_t;

#define LSM6DSO16IS_ISPU_DOUT_09_L                  0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout9                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout9                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_09_l_t;

#define LSM6DSO16IS_ISPU_DOUT_09_H                  0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout9                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout9                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_09_h_t;

#define LSM6DSO16IS_ISPU_DOUT_10_L                  0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout10                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout10                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_10_l_t;

#define LSM6DSO16IS_ISPU_DOUT_10_H                  0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout10                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout10                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_10_h_t;

#define LSM6DSO16IS_ISPU_DOUT_11_L                  0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout11                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout11                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_11_l_t;

#define LSM6DSO16IS_ISPU_DOUT_11_H                  0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout11                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout11                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_11_h_t;

#define LSM6DSO16IS_ISPU_DOUT_12_L                  0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout12                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout12                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_12_l_t;

#define LSM6DSO16IS_ISPU_DOUT_12_H                  0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout12                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout12                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_12_h_t;

#define LSM6DSO16IS_ISPU_DOUT_13_L                  0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout13                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout13                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_13_l_t;

#define LSM6DSO16IS_ISPU_DOUT_13_H                  0x2BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout13                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout13                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_13_h_t;

#define LSM6DSO16IS_ISPU_DOUT_14_L                  0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout14                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout14                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_14_l_t;

#define LSM6DSO16IS_ISPU_DOUT_14_H                  0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout14                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout14                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_14_h_t;

#define LSM6DSO16IS_ISPU_DOUT_15_L                  0x2EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout15                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout15                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_15_l_t;

#define LSM6DSO16IS_ISPU_DOUT_15_H                  0x2FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout15                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout15                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_15_h_t;

#define LSM6DSO16IS_ISPU_DOUT_16_L                  0x30U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout16                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout16                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_16_l_t;

#define LSM6DSO16IS_ISPU_DOUT_16_H                  0x31U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout16                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout16                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_16_h_t;

#define LSM6DSO16IS_ISPU_DOUT_17_L                  0x32U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout17                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout17                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_17_l_t;

#define LSM6DSO16IS_ISPU_DOUT_17_H                  0x33U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout17                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout17                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_17_h_t;

#define LSM6DSO16IS_ISPU_DOUT_18_L                  0x34U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout18                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout18                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_18_l_t;

#define LSM6DSO16IS_ISPU_DOUT_18_H                  0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout18                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout18                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_18_h_t;

#define LSM6DSO16IS_ISPU_DOUT_19_L                  0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout19                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout19                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_19_l_t;

#define LSM6DSO16IS_ISPU_DOUT_19_H                  0x37U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout19                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout19                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_19_h_t;

#define LSM6DSO16IS_ISPU_DOUT_20_L                  0x38U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout20                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout20                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_20_l_t;

#define LSM6DSO16IS_ISPU_DOUT_20_H                  0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout20                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout20                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_20_h_t;

#define LSM6DSO16IS_ISPU_DOUT_21_L                  0x3AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout21                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout21                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_21_l_t;

#define LSM6DSO16IS_ISPU_DOUT_21_H                  0x3BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout21                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout21                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_21_h_t;

#define LSM6DSO16IS_ISPU_DOUT_22_L                  0x3CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout22                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout22                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_22_l_t;

#define LSM6DSO16IS_ISPU_DOUT_22_H                  0x3DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout22                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout22                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_22_h_t;

#define LSM6DSO16IS_ISPU_DOUT_23_L                  0x3EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout23                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout23                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_23_l_t;

#define LSM6DSO16IS_ISPU_DOUT_23_H                  0x3FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout23                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout23                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_23_h_t;

#define LSM6DSO16IS_ISPU_DOUT_24_L                  0x40U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout24                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout24                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_24_l_t;

#define LSM6DSO16IS_ISPU_DOUT_24_H                  0x41U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout24                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout24                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_24_h_t;

#define LSM6DSO16IS_ISPU_DOUT_25_L                  0x42U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout25                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout25                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_25_l_t;

#define LSM6DSO16IS_ISPU_DOUT_25_H                  0x43U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout25                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout25                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_25_h_t;

#define LSM6DSO16IS_ISPU_DOUT_26_L                  0x44U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout26                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout26                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_26_l_t;

#define LSM6DSO16IS_ISPU_DOUT_26_H                  0x45U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout26                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout26                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_26_h_t;

#define LSM6DSO16IS_ISPU_DOUT_27_L                  0x46U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout27                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout27                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_27_l_t;

#define LSM6DSO16IS_ISPU_DOUT_27_H                  0x47U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout27                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout27                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_27_h_t;

#define LSM6DSO16IS_ISPU_DOUT_28_L                  0x48U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout28                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout28                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_28_l_t;

#define LSM6DSO16IS_ISPU_DOUT_28_H                  0x49U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout28                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout28                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_28_h_t;

#define LSM6DSO16IS_ISPU_DOUT_29_L                  0x4AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout29                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout29                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_29_l_t;

#define LSM6DSO16IS_ISPU_DOUT_29_H                  0x4BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout29                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout29                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_29_h_t;

#define LSM6DSO16IS_ISPU_DOUT_30_L                  0x4CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout30                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout30                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_30_l_t;

#define LSM6DSO16IS_ISPU_DOUT_30_H                  0x4DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout30                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout30                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_30_h_t;

#define LSM6DSO16IS_ISPU_DOUT_31_L                  0x4EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout31                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout31                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_31_l_t;

#define LSM6DSO16IS_ISPU_DOUT_31_H                  0x4FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t dout31                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dout31                       : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_dout_31_h_t;

#define LSM6DSO16IS_ISPU_INT1_CTRL0                 0x50U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int1_ctrl0_t;

#define LSM6DSO16IS_ISPU_INT1_CTRL1                 0x51U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int1_ctrl1_t;

#define LSM6DSO16IS_ISPU_INT1_CTRL2                 0x52U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int1_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int1_ctrl2_t;

#define LSM6DSO16IS_ISPU_INT1_CTRL3                 0x53U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int1_ctrl               : 6;
  uint8_t not_used0                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ispu_int1_ctrl               : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int1_ctrl3_t;

#define LSM6DSO16IS_ISPU_INT2_CTRL0                 0x54U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int2_ctrl0_t;

#define LSM6DSO16IS_ISPU_INT2_CTRL1                 0x55U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int2_ctrl1_t;

#define LSM6DSO16IS_ISPU_INT2_CTRL2                 0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int2_ctrl               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int2_ctrl2_t;

#define LSM6DSO16IS_ISPU_INT2_CTRL3                 0x57U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int2_ctrl               : 6;
  uint8_t not_used0                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ispu_int2_ctrl               : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int2_ctrl3_t;

#define LSM6DSO16IS_ISPU_INT_STATUS0                0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int_status              : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int_status              : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status0_t;

#define LSM6DSO16IS_ISPU_INT_STATUS1                0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int_status              : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int_status              : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status1_t;

#define LSM6DSO16IS_ISPU_INT_STATUS2                0x5AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int_status              : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_int_status              : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status2_t;

#define LSM6DSO16IS_ISPU_INT_STATUS3                0x5BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_int_status              : 6;
  uint8_t not_used0                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ispu_int_status              : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_int_status3_t;

#define LSM6DSO16IS_ISPU_ALGO0                      0x70U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_algo                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_algo                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_algo0_t;

#define LSM6DSO16IS_ISPU_ALGO1                      0x71U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_algo                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_algo                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_algo1_t;

#define LSM6DSO16IS_ISPU_ALGO2                      0x72U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_algo                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ispu_algo                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_algo2_t;

#define LSM6DSO16IS_ISPU_ALGO3                      0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ispu_algo                    : 6;
  uint8_t not_used0                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t ispu_algo                    : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso16is_ispu_algo3_t;

/**
  * @}
  *
  */

typedef union
{
  lsm6dso16is_func_cfg_access_t    func_cfg_access;
  lsm6dso16is_pin_ctrl_t    pin_ctrl;
  lsm6dso16is_drdy_pulsed_reg_t    drdy_pulsed_reg;
  lsm6dso16is_int1_ctrl_t    int1_ctrl;
  lsm6dso16is_int2_ctrl_t    int2_ctrl;
  lsm6dso16is_who_am_i_t    who_am_i;
  lsm6dso16is_ctrl1_xl_t    ctrl1_xl;
  lsm6dso16is_ctrl2_g_t    ctrl2_g;
  lsm6dso16is_ctrl3_c_t    ctrl3_c;
  lsm6dso16is_ctrl4_c_t    ctrl4_c;
  lsm6dso16is_ctrl5_c_t    ctrl5_c;
  lsm6dso16is_ctrl6_c_t    ctrl6_c;
  lsm6dso16is_ctrl7_g_t    ctrl7_g;
  lsm6dso16is_ctrl9_c_t    ctrl9_c;
  lsm6dso16is_ctrl10_c_t    ctrl10_c;
  lsm6dso16is_ispu_int_status0_mainpage_t    ispu_int_status0_mainpage;
  lsm6dso16is_ispu_int_status1_mainpage_t    ispu_int_status1_mainpage;
  lsm6dso16is_ispu_int_status2_mainpage_t    ispu_int_status2_mainpage;
  lsm6dso16is_ispu_int_status3_mainpage_t    ispu_int_status3_mainpage;
  lsm6dso16is_status_reg_t    status_reg;
  lsm6dso16is_out_temp_l_t    out_temp_l;
  lsm6dso16is_out_temp_h_t    out_temp_h;
  lsm6dso16is_outx_l_g_t    outx_l_g;
  lsm6dso16is_outx_h_g_t    outx_h_g;
  lsm6dso16is_outy_l_g_t    outy_l_g;
  lsm6dso16is_outy_h_g_t    outy_h_g;
  lsm6dso16is_outz_l_g_t    outz_l_g;
  lsm6dso16is_outz_h_g_t    outz_h_g;
  lsm6dso16is_outx_l_a_t    outx_l_a;
  lsm6dso16is_outx_h_a_t    outx_h_a;
  lsm6dso16is_outy_l_a_t    outy_l_a;
  lsm6dso16is_outy_h_a_t    outy_h_a;
  lsm6dso16is_outz_l_a_t    outz_l_a;
  lsm6dso16is_outz_h_a_t    outz_h_a;
  lsm6dso16is_status_master_mainpage_t    status_master_mainpage;
  lsm6dso16is_timestamp0_t    timestamp0;
  lsm6dso16is_timestamp1_t    timestamp1;
  lsm6dso16is_timestamp2_t    timestamp2;
  lsm6dso16is_timestamp3_t    timestamp3;
  lsm6dso16is_md1_cfg_t    md1_cfg;
  lsm6dso16is_md2_cfg_t    md2_cfg;
  lsm6dso16is_internal_freq_fine_t            internal_freq_fine;
  lsm6dso16is_ispu_dummy_cfg_1_l_t    ispu_dummy_cfg_1_l;
  lsm6dso16is_ispu_dummy_cfg_1_h_t    ispu_dummy_cfg_1_h;
  lsm6dso16is_ispu_dummy_cfg_2_l_t    ispu_dummy_cfg_2_l;
  lsm6dso16is_ispu_dummy_cfg_2_h_t    ispu_dummy_cfg_2_h;
  lsm6dso16is_ispu_dummy_cfg_3_l_t    ispu_dummy_cfg_3_l;
  lsm6dso16is_ispu_dummy_cfg_3_h_t    ispu_dummy_cfg_3_h;
  lsm6dso16is_ispu_dummy_cfg_4_l_t    ispu_dummy_cfg_4_l;
  lsm6dso16is_ispu_dummy_cfg_4_h_t    ispu_dummy_cfg_4_h;
  lsm6dso16is_sensor_hub_1_t    sensor_hub_1;
  lsm6dso16is_sensor_hub_2_t    sensor_hub_2;
  lsm6dso16is_sensor_hub_3_t    sensor_hub_3;
  lsm6dso16is_sensor_hub_4_t    sensor_hub_4;
  lsm6dso16is_sensor_hub_5_t    sensor_hub_5;
  lsm6dso16is_sensor_hub_6_t    sensor_hub_6;
  lsm6dso16is_sensor_hub_7_t    sensor_hub_7;
  lsm6dso16is_sensor_hub_8_t    sensor_hub_8;
  lsm6dso16is_sensor_hub_9_t    sensor_hub_9;
  lsm6dso16is_sensor_hub_10_t    sensor_hub_10;
  lsm6dso16is_sensor_hub_11_t    sensor_hub_11;
  lsm6dso16is_sensor_hub_12_t    sensor_hub_12;
  lsm6dso16is_sensor_hub_13_t    sensor_hub_13;
  lsm6dso16is_sensor_hub_14_t    sensor_hub_14;
  lsm6dso16is_sensor_hub_15_t    sensor_hub_15;
  lsm6dso16is_sensor_hub_16_t    sensor_hub_16;
  lsm6dso16is_sensor_hub_17_t    sensor_hub_17;
  lsm6dso16is_sensor_hub_18_t    sensor_hub_18;
  lsm6dso16is_master_config_t    master_config;
  lsm6dso16is_slv0_add_t    slv0_add;
  lsm6dso16is_slv0_subadd_t    slv0_subadd;
  lsm6dso16is_slv0_config_t    slv0_config;
  lsm6dso16is_slv1_add_t    slv1_add;
  lsm6dso16is_slv1_subadd_t    slv1_subadd;
  lsm6dso16is_slv1_config_t    slv1_config;
  lsm6dso16is_slv2_add_t    slv2_add;
  lsm6dso16is_slv2_subadd_t    slv2_subadd;
  lsm6dso16is_slv2_config_t    slv2_config;
  lsm6dso16is_slv3_add_t    slv3_add;
  lsm6dso16is_slv3_subadd_t    slv3_subadd;
  lsm6dso16is_slv3_config_t    slv3_config;
  lsm6dso16is_datawrite_slv0_t    datawrite_slv0;
  lsm6dso16is_status_master_t    status_master;
  lsm6dso16is_ispu_config_t    ispu_config;
  lsm6dso16is_ispu_status_t    ispu_status;
  lsm6dso16is_ispu_mem_sel_t    ispu_mem_sel;
  lsm6dso16is_ispu_mem_addr1_t    ispu_mem_addr1;
  lsm6dso16is_ispu_mem_addr0_t    ispu_mem_addr0;
  lsm6dso16is_ispu_mem_data_t    ispu_mem_data;
  lsm6dso16is_ispu_if2s_flag_l_t    ispu_if2s_flag_l;
  lsm6dso16is_ispu_if2s_flag_h_t    ispu_if2s_flag_h;
  lsm6dso16is_ispu_s2if_flag_l_t    ispu_s2if_flag_l;
  lsm6dso16is_ispu_s2if_flag_h_t    ispu_s2if_flag_h;
  lsm6dso16is_ispu_dout_00_l_t    ispu_dout_00_l;
  lsm6dso16is_ispu_dout_00_h_t    ispu_dout_00_h;
  lsm6dso16is_ispu_dout_01_l_t    ispu_dout_01_l;
  lsm6dso16is_ispu_dout_01_h_t    ispu_dout_01_h;
  lsm6dso16is_ispu_dout_02_l_t    ispu_dout_02_l;
  lsm6dso16is_ispu_dout_02_h_t    ispu_dout_02_h;
  lsm6dso16is_ispu_dout_03_l_t    ispu_dout_03_l;
  lsm6dso16is_ispu_dout_03_h_t    ispu_dout_03_h;
  lsm6dso16is_ispu_dout_04_l_t    ispu_dout_04_l;
  lsm6dso16is_ispu_dout_04_h_t    ispu_dout_04_h;
  lsm6dso16is_ispu_dout_05_l_t    ispu_dout_05_l;
  lsm6dso16is_ispu_dout_05_h_t    ispu_dout_05_h;
  lsm6dso16is_ispu_dout_06_l_t    ispu_dout_06_l;
  lsm6dso16is_ispu_dout_06_h_t    ispu_dout_06_h;
  lsm6dso16is_ispu_dout_07_l_t    ispu_dout_07_l;
  lsm6dso16is_ispu_dout_07_h_t    ispu_dout_07_h;
  lsm6dso16is_ispu_dout_08_l_t    ispu_dout_08_l;
  lsm6dso16is_ispu_dout_08_h_t    ispu_dout_08_h;
  lsm6dso16is_ispu_dout_09_l_t    ispu_dout_09_l;
  lsm6dso16is_ispu_dout_09_h_t    ispu_dout_09_h;
  lsm6dso16is_ispu_dout_10_l_t    ispu_dout_10_l;
  lsm6dso16is_ispu_dout_10_h_t    ispu_dout_10_h;
  lsm6dso16is_ispu_dout_11_l_t    ispu_dout_11_l;
  lsm6dso16is_ispu_dout_11_h_t    ispu_dout_11_h;
  lsm6dso16is_ispu_dout_12_l_t    ispu_dout_12_l;
  lsm6dso16is_ispu_dout_12_h_t    ispu_dout_12_h;
  lsm6dso16is_ispu_dout_13_l_t    ispu_dout_13_l;
  lsm6dso16is_ispu_dout_13_h_t    ispu_dout_13_h;
  lsm6dso16is_ispu_dout_14_l_t    ispu_dout_14_l;
  lsm6dso16is_ispu_dout_14_h_t    ispu_dout_14_h;
  lsm6dso16is_ispu_dout_15_l_t    ispu_dout_15_l;
  lsm6dso16is_ispu_dout_15_h_t    ispu_dout_15_h;
  lsm6dso16is_ispu_dout_16_l_t    ispu_dout_16_l;
  lsm6dso16is_ispu_dout_16_h_t    ispu_dout_16_h;
  lsm6dso16is_ispu_dout_17_l_t    ispu_dout_17_l;
  lsm6dso16is_ispu_dout_17_h_t    ispu_dout_17_h;
  lsm6dso16is_ispu_dout_18_l_t    ispu_dout_18_l;
  lsm6dso16is_ispu_dout_18_h_t    ispu_dout_18_h;
  lsm6dso16is_ispu_dout_19_l_t    ispu_dout_19_l;
  lsm6dso16is_ispu_dout_19_h_t    ispu_dout_19_h;
  lsm6dso16is_ispu_dout_20_l_t    ispu_dout_20_l;
  lsm6dso16is_ispu_dout_20_h_t    ispu_dout_20_h;
  lsm6dso16is_ispu_dout_21_l_t    ispu_dout_21_l;
  lsm6dso16is_ispu_dout_21_h_t    ispu_dout_21_h;
  lsm6dso16is_ispu_dout_22_l_t    ispu_dout_22_l;
  lsm6dso16is_ispu_dout_22_h_t    ispu_dout_22_h;
  lsm6dso16is_ispu_dout_23_l_t    ispu_dout_23_l;
  lsm6dso16is_ispu_dout_23_h_t    ispu_dout_23_h;
  lsm6dso16is_ispu_dout_24_l_t    ispu_dout_24_l;
  lsm6dso16is_ispu_dout_24_h_t    ispu_dout_24_h;
  lsm6dso16is_ispu_dout_25_l_t    ispu_dout_25_l;
  lsm6dso16is_ispu_dout_25_h_t    ispu_dout_25_h;
  lsm6dso16is_ispu_dout_26_l_t    ispu_dout_26_l;
  lsm6dso16is_ispu_dout_26_h_t    ispu_dout_26_h;
  lsm6dso16is_ispu_dout_27_l_t    ispu_dout_27_l;
  lsm6dso16is_ispu_dout_27_h_t    ispu_dout_27_h;
  lsm6dso16is_ispu_dout_28_l_t    ispu_dout_28_l;
  lsm6dso16is_ispu_dout_28_h_t    ispu_dout_28_h;
  lsm6dso16is_ispu_dout_29_l_t    ispu_dout_29_l;
  lsm6dso16is_ispu_dout_29_h_t    ispu_dout_29_h;
  lsm6dso16is_ispu_dout_30_l_t    ispu_dout_30_l;
  lsm6dso16is_ispu_dout_30_h_t    ispu_dout_30_h;
  lsm6dso16is_ispu_dout_31_l_t    ispu_dout_31_l;
  lsm6dso16is_ispu_dout_31_h_t    ispu_dout_31_h;
  lsm6dso16is_ispu_int1_ctrl0_t    ispu_int1_ctrl0;
  lsm6dso16is_ispu_int1_ctrl1_t    ispu_int1_ctrl1;
  lsm6dso16is_ispu_int1_ctrl2_t    ispu_int1_ctrl2;
  lsm6dso16is_ispu_int1_ctrl3_t    ispu_int1_ctrl3;
  lsm6dso16is_ispu_int2_ctrl0_t    ispu_int2_ctrl0;
  lsm6dso16is_ispu_int2_ctrl1_t    ispu_int2_ctrl1;
  lsm6dso16is_ispu_int2_ctrl2_t    ispu_int2_ctrl2;
  lsm6dso16is_ispu_int2_ctrl3_t    ispu_int2_ctrl3;
  lsm6dso16is_ispu_int_status0_t    ispu_int_status0;
  lsm6dso16is_ispu_int_status1_t    ispu_int_status1;
  lsm6dso16is_ispu_int_status2_t    ispu_int_status2;
  lsm6dso16is_ispu_int_status3_t    ispu_int_status3;
  lsm6dso16is_ispu_algo0_t    ispu_algo0;
  lsm6dso16is_ispu_algo1_t    ispu_algo1;
  lsm6dso16is_ispu_algo2_t    ispu_algo2;
  lsm6dso16is_ispu_algo3_t    ispu_algo3;
  bitwise_t    bitwise;
  uint8_t    byte;
} lsm6dso16is_reg_t;

/**
  * @}
  *
  */

#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */

/*
 * These are the basic platform dependent I/O routines to read
 * and write device registers connected on a standard bus.
 * The driver keeps offering a default implementation based on function
 * pointers to read/write routines for backward compatibility.
 * The __weak directive allows the final application to overwrite
 * them with a custom implementation.
 */

int32_t lsm6dso16is_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                             uint8_t *data,
                             uint16_t len);
int32_t lsm6dso16is_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                              uint8_t *data,
                              uint16_t len);

float_t lsm6dso16is_from_fs2g_to_mg(int16_t lsb);
float_t lsm6dso16is_from_fs4g_to_mg(int16_t lsb);
float_t lsm6dso16is_from_fs8g_to_mg(int16_t lsb);
float_t lsm6dso16is_from_fs16g_to_mg(int16_t lsb);

float_t lsm6dso16is_from_fs125dps_to_mdps(int16_t lsb);
float_t lsm6dso16is_from_fs250dps_to_mdps(int16_t lsb);
float_t lsm6dso16is_from_fs500dps_to_mdps(int16_t lsb);
float_t lsm6dso16is_from_fs1000dps_to_mdps(int16_t lsb);
float_t lsm6dso16is_from_fs2000dps_to_mdps(int16_t lsb);

float_t lsm6dso16is_from_lsb_to_celsius(int16_t lsb);

typedef enum
{
  LSM6DSO16IS_MAIN_MEM_BANK =             0x0,
  LSM6DSO16IS_SENSOR_HUB_MEM_BANK =       0x2,
  LSM6DSO16IS_ISPU_MEM_BANK =             0x3,
} lsm6dso16is_mem_bank_t;
int32_t lsm6dso16is_mem_bank_set(const stmdev_ctx_t *ctx, lsm6dso16is_mem_bank_t val);
int32_t lsm6dso16is_mem_bank_get(const stmdev_ctx_t *ctx, lsm6dso16is_mem_bank_t *val);

typedef enum
{
  LSM6DSO16IS_DRDY_LATCHED =              0x0,
  LSM6DSO16IS_DRDY_PULSED =               0x1,
} lsm6dso16is_data_ready_mode_t;
int32_t lsm6dso16is_data_ready_mode_set(const stmdev_ctx_t *ctx,
                                        lsm6dso16is_data_ready_mode_t val);
int32_t lsm6dso16is_data_ready_mode_get(const stmdev_ctx_t *ctx,
                                        lsm6dso16is_data_ready_mode_t *val);

int32_t lsm6dso16is_device_id_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso16is_software_reset(const stmdev_ctx_t *ctx);

int32_t lsm6dso16is_boot_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_boot_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO16IS_HIGH_PERFOMANCE_MODE_ENABLED =        0x0,
  LSM6DSO16IS_HIGH_PERFOMANCE_MODE_DISABLED =       0x1,
} lsm6dso16is_hm_mode_t;
int32_t lsm6dso16is_xl_hm_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t val);
int32_t lsm6dso16is_xl_hm_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t *val);
int32_t lsm6dso16is_gy_hm_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t val);
int32_t lsm6dso16is_gy_hm_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_hm_mode_t *val);

typedef enum
{
  LSM6DSO16IS_2g =                        0x0,
  LSM6DSO16IS_16g =                       0x1,
  LSM6DSO16IS_4g =                        0x2,
  LSM6DSO16IS_8g =                        0x3,
} lsm6dso16is_xl_full_scale_t;
int32_t lsm6dso16is_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_xl_full_scale_t val);
int32_t lsm6dso16is_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_xl_full_scale_t *val);

typedef enum
{
  LSM6DSO16IS_XL_ODR_OFF =                0x0,
  LSM6DSO16IS_XL_ODR_AT_12Hz5_HP =        0x1,
  LSM6DSO16IS_XL_ODR_AT_26H_HP =          0x2,
  LSM6DSO16IS_XL_ODR_AT_52Hz_HP =         0x3,
  LSM6DSO16IS_XL_ODR_AT_104Hz_HP =        0x4,
  LSM6DSO16IS_XL_ODR_AT_208Hz_HP =        0x5,
  LSM6DSO16IS_XL_ODR_AT_416Hz_HP =        0x6,
  LSM6DSO16IS_XL_ODR_AT_833Hz_HP =        0x7,
  LSM6DSO16IS_XL_ODR_AT_1667Hz_HP =       0x8,
  LSM6DSO16IS_XL_ODR_AT_3333Hz_HP =       0x9,
  LSM6DSO16IS_XL_ODR_AT_6667Hz_HP =       0xa,
  LSM6DSO16IS_XL_ODR_AT_12Hz5_LP =        0x11,
  LSM6DSO16IS_XL_ODR_AT_26H_LP =          0x12,
  LSM6DSO16IS_XL_ODR_AT_52Hz_LP =         0x13,
  LSM6DSO16IS_XL_ODR_AT_104Hz_LP =        0x14,
  LSM6DSO16IS_XL_ODR_AT_208Hz_LP =        0x15,
  LSM6DSO16IS_XL_ODR_AT_416Hz_LP =        0x16,
  LSM6DSO16IS_XL_ODR_AT_833Hz_LP =        0x17,
  LSM6DSO16IS_XL_ODR_AT_1667Hz_LP =       0x18,
  LSM6DSO16IS_XL_ODR_AT_3333Hz_LP =       0x19,
  LSM6DSO16IS_XL_ODR_AT_6667Hz_LP =       0x1a,
  LSM6DSO16IS_XL_ODR_AT_1Hz6_LP =         0x1b,
} lsm6dso16is_xl_data_rate_t;
int32_t lsm6dso16is_xl_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_data_rate_t val);
int32_t lsm6dso16is_xl_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_data_rate_t *val);

typedef enum
{
  LSM6DSO16IS_250dps =                    0x0,
  LSM6DSO16IS_500dps =                    0x1,
  LSM6DSO16IS_1000dps =                   0x2,
  LSM6DSO16IS_2000dps =                   0x3,
  LSM6DSO16IS_125dps =                    0x10,
} lsm6dso16is_gy_full_scale_t;
int32_t lsm6dso16is_gy_full_scale_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_gy_full_scale_t val);
int32_t lsm6dso16is_gy_full_scale_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_gy_full_scale_t *val);

typedef enum
{
  LSM6DSO16IS_GY_ODR_OFF =                0x0,
  LSM6DSO16IS_GY_ODR_AT_12Hz5_HP =        0x1,
  LSM6DSO16IS_GY_ODR_AT_26H_HP =          0x2,
  LSM6DSO16IS_GY_ODR_AT_52Hz_HP =         0x3,
  LSM6DSO16IS_GY_ODR_AT_104Hz_HP =        0x4,
  LSM6DSO16IS_GY_ODR_AT_208Hz_HP =        0x5,
  LSM6DSO16IS_GY_ODR_AT_416Hz_HP =        0x6,
  LSM6DSO16IS_GY_ODR_AT_833Hz_HP =        0x7,
  LSM6DSO16IS_GY_ODR_AT_1667Hz_HP =       0x8,
  LSM6DSO16IS_GY_ODR_AT_3333Hz_HP =       0x9,
  LSM6DSO16IS_GY_ODR_AT_6667Hz_HP =       0xa,
  LSM6DSO16IS_GY_ODR_AT_12Hz5_LP =        0x11,
  LSM6DSO16IS_GY_ODR_AT_26H_LP =          0x12,
  LSM6DSO16IS_GY_ODR_AT_52Hz_LP =         0x13,
  LSM6DSO16IS_GY_ODR_AT_104Hz_LP =        0x14,
  LSM6DSO16IS_GY_ODR_AT_208Hz_LP =        0x15,
  LSM6DSO16IS_GY_ODR_AT_416Hz_LP =        0x16,
  LSM6DSO16IS_GY_ODR_AT_833Hz_LP =        0x17,
  LSM6DSO16IS_GY_ODR_AT_1667Hz_LP =       0x18,
  LSM6DSO16IS_GY_ODR_AT_3333Hz_LP =       0x19,
  LSM6DSO16IS_GY_ODR_AT_6667Hz_LP =       0x1a,
} lsm6dso16is_gy_data_rate_t;
int32_t lsm6dso16is_gy_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_data_rate_t val);
int32_t lsm6dso16is_gy_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_data_rate_t *val);

int32_t lsm6dso16is_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_auto_increment_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso16is_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO16IS_SLEEP_G_ENABLE =            0x0,
  LSM6DSO16IS_SLEEP_G_DISABLE =           0x1,
} lsm6dso16is_sleep_t;
int32_t lsm6dso16is_sleep_set(const stmdev_ctx_t *ctx, lsm6dso16is_sleep_t val);
int32_t lsm6dso16is_sleep_get(const stmdev_ctx_t *ctx, lsm6dso16is_sleep_t *val);

typedef enum
{
  LSM6DSO16IS_XL_ST_DISABLE =             0x0,
  LSM6DSO16IS_XL_ST_POSITIVE =            0x1,
  LSM6DSO16IS_XL_ST_NEGATIVE =            0x2,
} lsm6dso16is_xl_self_test_t;
int32_t lsm6dso16is_xl_self_test_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_self_test_t val);
int32_t lsm6dso16is_xl_self_test_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_xl_self_test_t *val);

typedef enum
{
  LSM6DSO16IS_GY_ST_DISABLE =             0x0,
  LSM6DSO16IS_GY_ST_POSITIVE =            0x1,
  LSM6DSO16IS_GY_ST_NEGATIVE =            0x3,
} lsm6dso16is_gy_self_test_t;
int32_t lsm6dso16is_gy_self_test_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_self_test_t val);
int32_t lsm6dso16is_gy_self_test_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_gy_self_test_t *val);

int32_t lsm6dso16is_ui_sdo_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_ui_sdo_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO16IS_SPI_4_WIRE =                0x0,
  LSM6DSO16IS_SPI_3_WIRE =                0x1,
} lsm6dso16is_spi_mode_t;
int32_t lsm6dso16is_spi_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_spi_mode_t val);
int32_t lsm6dso16is_spi_mode_get(const stmdev_ctx_t *ctx, lsm6dso16is_spi_mode_t *val);

typedef enum
{
  LSM6DSO16IS_I2C_ENABLE =                0x0,
  LSM6DSO16IS_I2C_DISABLE =               0x1,
} lsm6dso16is_ui_i2c_mode_t;
int32_t lsm6dso16is_ui_i2c_mode_set(const stmdev_ctx_t *ctx, lsm6dso16is_ui_i2c_mode_t val);
int32_t lsm6dso16is_ui_i2c_mode_get(const stmdev_ctx_t *ctx,
                                    lsm6dso16is_ui_i2c_mode_t *val);


int32_t lsm6dso16is_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso16is_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val);

typedef struct
{
  uint8_t drdy_xl                      : 1;
  uint8_t drdy_gy                      : 1;
  uint8_t boot                         : 1;
  uint8_t sh_endop                     : 1;
  uint8_t ispu                         : 1;
} lsm6dso16is_pin_int1_route_t;
int32_t lsm6dso16is_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int1_route_t val);
int32_t lsm6dso16is_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int1_route_t *val);

typedef struct
{
  uint8_t drdy_xl                      : 1;
  uint8_t drdy_gy                      : 1;
  uint8_t drdy_temp                    : 1;
  uint8_t timestamp                    : 1;
  uint8_t ispu_sleep                   : 1;
  uint8_t ispu                         : 1;
} lsm6dso16is_pin_int2_route_t;
int32_t lsm6dso16is_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int2_route_t val);
int32_t lsm6dso16is_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_pin_int2_route_t *val);

typedef enum
{
  LSM6DSO16IS_PUSH_PULL =                 0x0,
  LSM6DSO16IS_OPEN_DRAIN =                0x1,
} lsm6dso16is_int_pin_mode_t;
int32_t lsm6dso16is_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_int_pin_mode_t val);
int32_t lsm6dso16is_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_int_pin_mode_t *val);

typedef enum
{
  LSM6DSO16IS_ACTIVE_HIGH =               0x0,
  LSM6DSO16IS_ACTIVE_LOW =                0x1,
} lsm6dso16is_pin_polarity_t;
int32_t lsm6dso16is_pin_polarity_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_pin_polarity_t val);
int32_t lsm6dso16is_pin_polarity_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_pin_polarity_t *val);

typedef struct
{
  uint8_t drdy_xl                      : 1;
  uint8_t drdy_gy                      : 1;
  uint8_t drdy_temp                    : 1;
  uint8_t sh_endop                     : 1;
  uint8_t sh_slave0_nack               : 1;
  uint8_t sh_slave1_nack               : 1;
  uint8_t sh_slave2_nack               : 1;
  uint8_t sh_slave3_nack               : 1;
  uint8_t sh_wr_once                   : 1;
  uint32_t ispu                        : 30;
} lsm6dso16is_all_sources_t;
int32_t lsm6dso16is_all_sources_get(const stmdev_ctx_t *ctx,
                                    lsm6dso16is_all_sources_t *val);

int32_t lsm6dso16is_status_reg_get(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_status_reg_t *val);

int32_t lsm6dso16is_xl_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val);

int32_t lsm6dso16is_gy_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val);

int32_t lsm6dso16is_temp_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                             uint8_t *val);

int32_t lsm6dso16is_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dso16is_angular_rate_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dso16is_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dso16is_odr_cal_reg_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_odr_cal_reg_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso16is_sh_read_data_raw_get(const stmdev_ctx_t *ctx, uint8_t *val,
                                         uint8_t len);

typedef enum
{
  LSM6DSO16IS_SLV_0 =                     0x0,
  LSM6DSO16IS_SLV_0_1 =                   0x1,
  LSM6DSO16IS_SLV_0_1_2 =                 0x2,
  LSM6DSO16IS_SLV_0_1_2_3 =               0x3,
} lsm6dso16is_sh_slave_connected_t;
int32_t lsm6dso16is_sh_slave_connected_set(const stmdev_ctx_t *ctx,
                                           lsm6dso16is_sh_slave_connected_t val);
int32_t lsm6dso16is_sh_slave_connected_get(const stmdev_ctx_t *ctx,
                                           lsm6dso16is_sh_slave_connected_t *val);

int32_t lsm6dso16is_sh_master_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_sh_master_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso16is_sh_master_interface_pull_up_set(const stmdev_ctx_t *ctx,
                                                    uint8_t val);
int32_t lsm6dso16is_sh_master_interface_pull_up_get(const stmdev_ctx_t *ctx,
                                                    uint8_t *val);

int32_t lsm6dso16is_sh_pass_through_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_sh_pass_through_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO16IS_SH_TRG_XL_GY_DRDY =         0x0,
  LSM6DSO16IS_SH_TRIG_INT2 =              0x1,
} lsm6dso16is_sh_syncro_mode_t;
int32_t lsm6dso16is_sh_syncro_mode_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_sh_syncro_mode_t val);
int32_t lsm6dso16is_sh_syncro_mode_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_sh_syncro_mode_t *val);

typedef enum
{
  LSM6DSO16IS_EACH_SH_CYCLE =             0x0,
  LSM6DSO16IS_ONLY_FIRST_CYCLE =          0x1,
} lsm6dso16is_sh_write_mode_t;
int32_t lsm6dso16is_sh_write_mode_set(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_sh_write_mode_t val);
int32_t lsm6dso16is_sh_write_mode_get(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_sh_write_mode_t *val);

int32_t lsm6dso16is_sh_reset_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_sh_reset_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  uint8_t   slv0_add;
  uint8_t   slv0_subadd;
  uint8_t   slv0_data;
} lsm6dso16is_sh_cfg_write_t;
int32_t lsm6dso16is_sh_cfg_write(const stmdev_ctx_t *ctx,
                                 lsm6dso16is_sh_cfg_write_t *val);
typedef enum
{
  LSM6DSO16IS_SH_104Hz =                  0x0,
  LSM6DSO16IS_SH_52Hz =                   0x1,
  LSM6DSO16IS_SH_26Hz =                   0x2,
  LSM6DSO16IS_SH_12_5Hz =                 0x3,
} lsm6dso16is_sh_data_rate_t;
int32_t lsm6dso16is_sh_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_sh_data_rate_t val);
int32_t lsm6dso16is_sh_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_sh_data_rate_t *val);

typedef struct
{
  uint8_t   slv_add;
  uint8_t   slv_subadd;
  uint8_t   slv_len;
} lsm6dso16is_sh_cfg_read_t;
int32_t lsm6dso16is_sh_slv_cfg_read(const stmdev_ctx_t *ctx, uint8_t idx,
                                    lsm6dso16is_sh_cfg_read_t *val);

int32_t lsm6dso16is_sh_status_get(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_status_master_t *val);

int32_t lsm6dso16is_ispu_reset_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso16is_ispu_reset_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_CLK_5MHz =             0x0,
  LSM6DSO16IS_ISPU_CLK_10MHz =            0x1,
} lsm6dso16is_ispu_clock_sel_t;
int32_t lsm6dso16is_ispu_clock_set(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_ispu_clock_sel_t val);
int32_t lsm6dso16is_ispu_clock_get(const stmdev_ctx_t *ctx,
                                   lsm6dso16is_ispu_clock_sel_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_ODR_OFF =              0x0,
  LSM6DSO16IS_ISPU_ODR_AT_12Hz5 =         0x1,
  LSM6DSO16IS_ISPU_ODR_AT_26Hz =          0x2,
  LSM6DSO16IS_ISPU_ODR_AT_52Hz =          0x3,
  LSM6DSO16IS_ISPU_ODR_AT_104Hz =         0x4,
  LSM6DSO16IS_ISPU_ODR_AT_208Hz =         0x5,
  LSM6DSO16IS_ISPU_ODR_AT_416Hz =         0x6,
  LSM6DSO16IS_ISPU_ODR_AT_833Hz =         0x7,
  LSM6DSO16IS_ISPU_ODR_AT_1667Hz =        0x8,
  LSM6DSO16IS_ISPU_ODR_AT_3333Hz =        0x9,
  LSM6DSO16IS_ISPU_ODR_AT_6667Hz =        0xa,
} lsm6dso16is_ispu_data_rate_t;
int32_t lsm6dso16is_ispu_data_rate_set(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_ispu_data_rate_t val);
int32_t lsm6dso16is_ispu_data_rate_get(const stmdev_ctx_t *ctx,
                                       lsm6dso16is_ispu_data_rate_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_BDU_OFF =              0x0,
  LSM6DSO16IS_ISPU_BDU_ON_2B_4B =         0x1,
  LSM6DSO16IS_ISPU_BDU_ON_2B_2B =         0x2,
  LSM6DSO16IS_ISPU_BDU_ON_4B_4B =         0x3,
} lsm6dso16is_ispu_bdu_t;
int32_t lsm6dso16is_ispu_bdu_set(const stmdev_ctx_t *ctx, lsm6dso16is_ispu_bdu_t val);
int32_t lsm6dso16is_ispu_bdu_get(const stmdev_ctx_t *ctx, lsm6dso16is_ispu_bdu_t *val);

int32_t lsm6dso16is_ia_ispu_get(const stmdev_ctx_t *ctx, uint32_t *val);

int32_t lsm6dso16is_ispu_write_dummy_cfg(const stmdev_ctx_t *ctx, uint8_t offset,
                                         uint8_t *val, uint8_t len);
int32_t lsm6dso16is_ispu_read_dummy_cfg(const stmdev_ctx_t *ctx, uint8_t offset,
                                        uint8_t *val, uint8_t len);

typedef enum
{
  LSM6DSO16IS_ISPU_TURN_ON =              0x0,
  LSM6DSO16IS_ISPU_TURN_OFF =             0x1,
} lsm6dso16is_ispu_boot_latched_t;
int32_t lsm6dso16is_ispu_boot_set(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_ispu_boot_latched_t val);
int32_t lsm6dso16is_ispu_boot_get(const stmdev_ctx_t *ctx,
                                  lsm6dso16is_ispu_boot_latched_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_INT_PULSED =           0x0,
  LSM6DSO16IS_ISPU_INT_LATCHED =          0x1,
} lsm6dso16is_ispu_int_latched_t;
int32_t lsm6dso16is_ispu_int_latched_set(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_int_latched_t val);
int32_t lsm6dso16is_ispu_int_latched_get(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_int_latched_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_BOOT_IN_PROGRESS =     0x0,
  LSM6DSO16IS_ISPU_BOOT_ENDED =           0x1,
} lsm6dso16is_ispu_boot_end_t;
int32_t lsm6dso16is_ispu_get_boot_status(const stmdev_ctx_t *ctx,
                                         lsm6dso16is_ispu_boot_end_t *val);

typedef enum
{
  LSM6DSO16IS_ISPU_DATA_RAM_MEMORY =      0x0,
  LSM6DSO16IS_ISPU_PROGRAM_RAM_MEMORY =   0x1,
} lsm6dso16is_ispu_memory_type_t;
int32_t lsm6dso16is_ispu_read_memory_enable(const stmdev_ctx_t *ctx,
                                            lsm6dso16is_ispu_memory_type_t val);
int32_t lsm6dso16is_ispu_read_memory_disable(const stmdev_ctx_t *ctx);

int32_t lsm6dso16is_ispu_write_memory(const stmdev_ctx_t *ctx,
                                      lsm6dso16is_ispu_memory_type_t mem_sel,
                                      uint16_t mem_addr, uint8_t *mem_data, uint16_t len);
int32_t lsm6dso16is_ispu_read_memory(const stmdev_ctx_t *ctx,
                                     lsm6dso16is_ispu_memory_type_t mem_sel,
                                     uint16_t mem_addr, uint8_t *mem_data, uint16_t len);

int32_t lsm6dso16is_ispu_write_flags(const stmdev_ctx_t *ctx, uint16_t data);
int32_t lsm6dso16is_ispu_read_flags(const stmdev_ctx_t *ctx, uint16_t *data);
int32_t lsm6dso16is_ispu_clear_flags(const stmdev_ctx_t *ctx);

int32_t lsm6dso16is_ispu_read_data_raw_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val,
                                           uint8_t len);

int32_t lsm6dso16is_ispu_int1_ctrl_get(const stmdev_ctx_t *ctx, uint32_t *val);
int32_t lsm6dso16is_ispu_int1_ctrl_set(const stmdev_ctx_t *ctx, uint32_t val);
int32_t lsm6dso16is_ispu_int2_ctrl_get(const stmdev_ctx_t *ctx, uint32_t *val);
int32_t lsm6dso16is_ispu_int2_ctrl_set(const stmdev_ctx_t *ctx, uint32_t val);

int32_t lsm6dso16is_ispu_int_status_get(const stmdev_ctx_t *ctx, uint32_t *val);

int32_t lsm6dso16is_ispu_algo_get(const stmdev_ctx_t *ctx, uint32_t *val);
int32_t lsm6dso16is_ispu_algo_set(const stmdev_ctx_t *ctx, uint32_t val);
/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*LSM6DSO16IS_DRIVER_H */
