/**
  ******************************************************************************
  * @file    lsm6dso32x_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          lsm6dso32x_reg.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSO32X_REGS_H
#define LSM6DSO32X_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LSM6DSO32X
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
  uint8_t bit0       : 1;
  uint8_t bit1       : 1;
  uint8_t bit2       : 1;
  uint8_t bit3       : 1;
  uint8_t bit4       : 1;
  uint8_t bit5       : 1;
  uint8_t bit6       : 1;
  uint8_t bit7       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7       : 1;
  uint8_t bit6       : 1;
  uint8_t bit5       : 1;
  uint8_t bit4       : 1;
  uint8_t bit3       : 1;
  uint8_t bit2       : 1;
  uint8_t bit1       : 1;
  uint8_t bit0       : 1;
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

/** @defgroup LSM6DSO32X_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define LSM6DSO32X_I2C_ADD_L                    0xD5U
#define LSM6DSO32X_I2C_ADD_H                    0xD7U

/** Device Identification (Who am I) **/
#define LSM6DSO32X_ID                           0x6CU

/**
  * @}
  *
  */

#define LSM6DSO32X_FUNC_CFG_ACCESS              0x01U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 6;
uint8_t reg_access               :
  2; /* shub_reg_access + func_cfg_access */
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
uint8_t reg_access               :
  2; /* shub_reg_access + func_cfg_access */
  uint8_t not_used_01              : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_func_cfg_access_t;

#define LSM6DSO32X_PIN_CTRL                     0x02U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 6;
  uint8_t sdo_pu_en                : 1;
  uint8_t not_used_02              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 1;
  uint8_t sdo_pu_en                : 1;
  uint8_t not_used_01              : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_pin_ctrl_t;

#define LSM6DSO32X_FIFO_CTRL1                   0x07U
typedef struct
{
  uint8_t wtm                      : 8;
} lsm6dso32x_fifo_ctrl1_t;

#define LSM6DSO32X_FIFO_CTRL2                   0x08U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm                      : 1;
  uint8_t uncoptr_rate             : 2;
  uint8_t not_used_01              : 1;
  uint8_t odrchg_en                : 1;
  uint8_t not_used_02              : 1;
  uint8_t fifo_compr_rt_en         : 1;
  uint8_t stop_on_wtm              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t stop_on_wtm              : 1;
  uint8_t fifo_compr_rt_en         : 1;
  uint8_t not_used_02              : 1;
  uint8_t odrchg_en                : 1;
  uint8_t not_used_01              : 1;
  uint8_t uncoptr_rate             : 2;
  uint8_t wtm                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fifo_ctrl2_t;

#define LSM6DSO32X_FIFO_CTRL3                   0x09U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bdr_xl                   : 4;
  uint8_t bdr_gy                   : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bdr_gy                   : 4;
  uint8_t bdr_xl                   : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fifo_ctrl3_t;

#define LSM6DSO32X_FIFO_CTRL4                   0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_mode                : 3;
  uint8_t not_used_01              : 1;
  uint8_t odr_t_batch              : 2;
  uint8_t odr_ts_batch             : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_ts_batch             : 2;
  uint8_t odr_t_batch              : 2;
  uint8_t not_used_01              : 1;
  uint8_t fifo_mode                : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fifo_ctrl4_t;

#define LSM6DSO32X_COUNTER_BDR_REG1             0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t cnt_bdr_th               : 3;
  uint8_t not_used_01              : 2;
  uint8_t trig_counter_bdr         : 1;
  uint8_t rst_counter_bdr          : 1;
  uint8_t dataready_pulsed         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dataready_pulsed         : 1;
  uint8_t rst_counter_bdr          : 1;
  uint8_t trig_counter_bdr         : 1;
  uint8_t not_used_01              : 2;
  uint8_t cnt_bdr_th               : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_counter_bdr_reg1_t;

#define LSM6DSO32X_COUNTER_BDR_REG2             0x0CU
typedef struct
{
  uint8_t cnt_bdr_th               : 8;
} lsm6dso32x_counter_bdr_reg2_t;

#define LSM6DSO32X_INT1_CTRL                    0x0D
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_drdy_xl             : 1;
  uint8_t int1_drdy_g              : 1;
  uint8_t int1_boot                : 1;
  uint8_t int1_fifo_th             : 1;
  uint8_t int1_fifo_ovr            : 1;
  uint8_t int1_fifo_full           : 1;
  uint8_t int1_cnt_bdr             : 1;
  uint8_t den_drdy_flag            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t den_drdy_flag            : 1;
  uint8_t int1_cnt_bdr             : 1;
  uint8_t int1_fifo_full           : 1;
  uint8_t int1_fifo_ovr            : 1;
  uint8_t int1_fifo_th             : 1;
  uint8_t int1_boot                : 1;
  uint8_t int1_drdy_g              : 1;
  uint8_t int1_drdy_xl             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_int1_ctrl_t;

#define LSM6DSO32X_INT2_CTRL                    0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_drdy_xl             : 1;
  uint8_t int2_drdy_g              : 1;
  uint8_t int2_drdy_temp           : 1;
  uint8_t int2_fifo_th             : 1;
  uint8_t int2_fifo_ovr            : 1;
  uint8_t int2_fifo_full           : 1;
  uint8_t int2_cnt_bdr             : 1;
  uint8_t not_used_01              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t int2_cnt_bdr             : 1;
  uint8_t int2_fifo_full           : 1;
  uint8_t int2_fifo_ovr            : 1;
  uint8_t int2_fifo_th             : 1;
  uint8_t int2_drdy_temp           : 1;
  uint8_t int2_drdy_g              : 1;
  uint8_t int2_drdy_xl             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_int2_ctrl_t;

#define LSM6DSO32X_WHO_AM_I                     0x0FU
#define LSM6DSO32X_CTRL1_XL                     0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t lpf2_xl_en               : 1;
  uint8_t fs_xl                    : 2;
  uint8_t odr_xl                   : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_xl                   : 4;
  uint8_t fs_xl                    : 2;
  uint8_t lpf2_xl_en               : 1;
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl1_xl_t;

#define LSM6DSO32X_CTRL2_G                      0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t fs_g                     : 3; /* fs_125 + fs_g */
  uint8_t odr_g                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_g                    : 4;
  uint8_t fs_g                     : 3; /* fs_125 + fs_g */
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl2_g_t;

#define LSM6DSO32X_CTRL3_C                      0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sw_reset                 : 1;
  uint8_t not_used_01              : 1;
  uint8_t if_inc                   : 1;
  uint8_t sim                      : 1;
  uint8_t pp_od                    : 1;
  uint8_t h_lactive                : 1;
  uint8_t bdu                      : 1;
  uint8_t boot                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot                     : 1;
  uint8_t bdu                      : 1;
  uint8_t h_lactive                : 1;
  uint8_t pp_od                    : 1;
  uint8_t sim                      : 1;
  uint8_t if_inc                   : 1;
  uint8_t not_used_01              : 1;
  uint8_t sw_reset                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl3_c_t;

#define LSM6DSO32X_CTRL4_C                      0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t lpf1_sel_g               : 1;
  uint8_t i2c_disable              : 1;
  uint8_t drdy_mask                : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_on_int1             : 1;
  uint8_t sleep_g                  : 1;
  uint8_t not_used_03              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_03              : 1;
  uint8_t sleep_g                  : 1;
  uint8_t int2_on_int1             : 1;
  uint8_t not_used_02              : 1;
  uint8_t drdy_mask                : 1;
  uint8_t i2c_disable              : 1;
  uint8_t lpf1_sel_g               : 1;
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl4_c_t;

#define LSM6DSO32X_CTRL5_C                      0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_xl                    : 2;
  uint8_t st_g                     : 2;
  uint8_t not_used_01              : 1;
  uint8_t rounding                 : 2;
  uint8_t xl_ulp_en                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t xl_ulp_en                : 1;
  uint8_t rounding                 : 2;
  uint8_t not_used_01              : 1;
  uint8_t st_g                     : 2;
  uint8_t st_xl                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl5_c_t;

#define LSM6DSO32X_CTRL6_C                      0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ftype                    : 3;
  uint8_t usr_off_w                : 1;
  uint8_t xl_hm_mode               : 1;
uint8_t den_mode                 :
  3;   /* trig_en + lvl1_en + lvl2_en */
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
uint8_t den_mode                 :
  3;   /* trig_en + lvl1_en + lvl2_en */
  uint8_t xl_hm_mode               : 1;
  uint8_t usr_off_w                : 1;
  uint8_t ftype                    : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl6_c_t;

#define LSM6DSO32X_CTRL7_G                      0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t usr_off_on_out           : 1;
  uint8_t not_used_02              : 2;
  uint8_t hpm_g                    : 2;
  uint8_t hp_en_g                  : 1;
  uint8_t g_hm_mode                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t g_hm_mode                : 1;
  uint8_t hp_en_g                  : 1;
  uint8_t hpm_g                    : 2;
  uint8_t not_used_02              : 2;
  uint8_t usr_off_on_out           : 1;
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl7_g_t;

#define LSM6DSO32X_CTRL8_XL                     0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t low_pass_on_6d           : 1;
  uint8_t not_used_01              : 1;
  uint8_t hp_slope_xl_en           : 1;
  uint8_t fastsettl_mode_xl        : 1;
  uint8_t hp_ref_mode_xl           : 1;
  uint8_t hpcf_xl                  : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t hpcf_xl                  : 3;
  uint8_t hp_ref_mode_xl           : 1;
  uint8_t fastsettl_mode_xl        : 1;
  uint8_t hp_slope_xl_en           : 1;
  uint8_t not_used_01              : 1;
  uint8_t low_pass_on_6d           : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl8_xl_t;

#define LSM6DSO32X_CTRL9_XL                     0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t i3c_disable              : 1;
  uint8_t den_lh                   : 1;
  uint8_t den_xl_g                 : 2;   /* den_xl_en + den_xl_g */
  uint8_t den_z                    : 1;
  uint8_t den_y                    : 1;
  uint8_t den_x                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t den_x                    : 1;
  uint8_t den_y                    : 1;
  uint8_t den_z                    : 1;
  uint8_t den_xl_g                 : 2;   /* den_xl_en + den_xl_g */
  uint8_t den_lh                   : 1;
  uint8_t i3c_disable              : 1;
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl9_xl_t;

#define LSM6DSO32X_CTRL10_C                     0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t timestamp_en             : 1;
  uint8_t not_used_02              : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 2;
  uint8_t timestamp_en             : 1;
  uint8_t not_used_01              : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_ctrl10_c_t;

#define LSM6DSO32X_ALL_INT_SRC                  0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ia                    : 1;
  uint8_t wu_ia                    : 1;
  uint8_t single_tap               : 1;
  uint8_t double_tap               : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t not_used_01              : 1;
  uint8_t timestamp_endcount       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_endcount       : 1;
  uint8_t not_used_01              : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t double_tap               : 1;
  uint8_t single_tap               : 1;
  uint8_t wu_ia                    : 1;
  uint8_t ff_ia                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_all_int_src_t;

#define LSM6DSO32X_WAKE_UP_SRC                  0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_wu                     : 1;
  uint8_t y_wu                     : 1;
  uint8_t x_wu                     : 1;
  uint8_t wu_ia                    : 1;
  uint8_t sleep_state              : 1;
  uint8_t ff_ia                    : 1;
  uint8_t sleep_change_ia          : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sleep_change_ia          : 2;
  uint8_t ff_ia                    : 1;
  uint8_t sleep_state              : 1;
  uint8_t wu_ia                    : 1;
  uint8_t x_wu                     : 1;
  uint8_t y_wu                     : 1;
  uint8_t z_wu                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_wake_up_src_t;

#define LSM6DSO32X_TAP_SRC                      0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_tap                    : 1;
  uint8_t y_tap                    : 1;
  uint8_t x_tap                    : 1;
  uint8_t tap_sign                 : 1;
  uint8_t double_tap               : 1;
  uint8_t single_tap               : 1;
  uint8_t tap_ia                   : 1;
  uint8_t not_used_02              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 1;
  uint8_t tap_ia                   : 1;
  uint8_t single_tap               : 1;
  uint8_t double_tap               : 1;
  uint8_t tap_sign                 : 1;
  uint8_t x_tap                    : 1;
  uint8_t y_tap                    : 1;
  uint8_t z_tap                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_tap_src_t;

#define LSM6DSO32X_D6D_SRC                      0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl                       : 1;
  uint8_t xh                       : 1;
  uint8_t yl                       : 1;
  uint8_t yh                       : 1;
  uint8_t zl                       : 1;
  uint8_t zh                       : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t den_drdy                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t den_drdy                 : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t zh                       : 1;
  uint8_t zl                       : 1;
  uint8_t yh                       : 1;
  uint8_t yl                       : 1;
  uint8_t xh                       : 1;
  uint8_t xl                       : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_d6d_src_t;

#define LSM6DSO32X_STATUS_REG                   0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlda                     : 1;
  uint8_t gda                      : 1;
  uint8_t tda                      : 1;
  uint8_t not_used_01              : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t tda                      : 1;
  uint8_t gda                      : 1;
  uint8_t xlda                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_status_reg_t;

#define LSM6DSO32X_OUT_TEMP_L                   0x20U
#define LSM6DSO32X_OUT_TEMP_H                   0x21U
#define LSM6DSO32X_OUTX_L_G                     0x22U
#define LSM6DSO32X_OUTX_H_G                     0x23U
#define LSM6DSO32X_OUTY_L_G                     0x24U
#define LSM6DSO32X_OUTY_H_G                     0x25U
#define LSM6DSO32X_OUTZ_L_G                     0x26U
#define LSM6DSO32X_OUTZ_H_G                     0x27U
#define LSM6DSO32X_OUTX_L_A                     0x28U
#define LSM6DSO32X_OUTX_H_A                     0x29U
#define LSM6DSO32X_OUTY_L_A                     0x2AU
#define LSM6DSO32X_OUTY_H_A                     0x2BU
#define LSM6DSO32X_OUTZ_L_A                     0x2CU
#define LSM6DSO32X_OUTZ_H_A                     0x2DU
#define LSM6DSO32X_EMB_FUNC_STATUS_MAINPAGE     0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01             : 3;
  uint8_t is_step_det             : 1;
  uint8_t is_tilt                 : 1;
  uint8_t is_sigmot               : 1;
  uint8_t not_used_02             : 1;
  uint8_t is_fsm_lc               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc               : 1;
  uint8_t not_used_02             : 1;
  uint8_t is_sigmot               : 1;
  uint8_t is_tilt                 : 1;
  uint8_t is_step_det             : 1;
  uint8_t not_used_01             : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_status_mainpage_t;

#define LSM6DSO32X_FSM_STATUS_A_MAINPAGE        0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                 : 1;
  uint8_t is_fsm2                 : 1;
  uint8_t is_fsm3                 : 1;
  uint8_t is_fsm4                 : 1;
  uint8_t is_fsm5                 : 1;
  uint8_t is_fsm6                 : 1;
  uint8_t is_fsm7                 : 1;
  uint8_t is_fsm8                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                 : 1;
  uint8_t is_fsm7                 : 1;
  uint8_t is_fsm6                 : 1;
  uint8_t is_fsm5                 : 1;
  uint8_t is_fsm4                 : 1;
  uint8_t is_fsm3                 : 1;
  uint8_t is_fsm2                 : 1;
  uint8_t is_fsm1                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_status_a_mainpage_t;

#define LSM6DSO32X_FSM_STATUS_B_MAINPAGE        0x37U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm9                 : 1;
  uint8_t is_fsm10                : 1;
  uint8_t is_fsm11                : 1;
  uint8_t is_fsm12                : 1;
  uint8_t is_fsm13                : 1;
  uint8_t is_fsm14                : 1;
  uint8_t is_fsm15                : 1;
  uint8_t is_fsm16                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm16                : 1;
  uint8_t is_fsm15                : 1;
  uint8_t is_fsm14                : 1;
  uint8_t is_fsm13                : 1;
  uint8_t is_fsm12                : 1;
  uint8_t is_fsm11                : 1;
  uint8_t is_fsm10                : 1;
  uint8_t is_fsm9                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_status_b_mainpage_t;

#define LSM6DSO32X_MLC_STATUS_MAINPAGE     0x38U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1             : 1;
  uint8_t is_mlc2             : 1;
  uint8_t is_mlc3             : 1;
  uint8_t is_mlc4             : 1;
  uint8_t is_mlc5             : 1;
  uint8_t is_mlc6             : 1;
  uint8_t is_mlc7             : 1;
  uint8_t is_mlc8             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_mlc8             : 1;
  uint8_t is_mlc7             : 1;
  uint8_t is_mlc6             : 1;
  uint8_t is_mlc5             : 1;
  uint8_t is_mlc4             : 1;
  uint8_t is_mlc3             : 1;
  uint8_t is_mlc2             : 1;
  uint8_t is_mlc1             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mlc_status_mainpage_t;

#define LSM6DSO32X_STATUS_MASTER_MAINPAGE       0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sens_hub_endop          : 1;
  uint8_t not_used_01             : 2;
  uint8_t slave0_nack             : 1;
  uint8_t slave1_nack             : 1;
  uint8_t slave2_nack             : 1;
  uint8_t slave3_nack             : 1;
  uint8_t wr_once_done            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wr_once_done            : 1;
  uint8_t slave3_nack             : 1;
  uint8_t slave2_nack             : 1;
  uint8_t slave1_nack             : 1;
  uint8_t slave0_nack             : 1;
  uint8_t not_used_01             : 2;
  uint8_t sens_hub_endop          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_status_master_mainpage_t;

#define LSM6DSO32X_FIFO_STATUS1                 0x3AU
typedef struct
{
  uint8_t diff_fifo                : 8;
} lsm6dso32x_fifo_status1_t;

#define LSM6DSO32X_FIFO_STATUS2                 0x3B
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t diff_fifo                : 2;
  uint8_t not_used_01              : 1;
  uint8_t over_run_latched         : 1;
  uint8_t counter_bdr_ia           : 1;
  uint8_t fifo_full_ia             : 1;
  uint8_t fifo_ovr_ia              : 1;
  uint8_t fifo_wtm_ia              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_wtm_ia              : 1;
  uint8_t fifo_ovr_ia              : 1;
  uint8_t fifo_full_ia             : 1;
  uint8_t counter_bdr_ia           : 1;
  uint8_t over_run_latched         : 1;
  uint8_t not_used_01              : 1;
  uint8_t diff_fifo                : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fifo_status2_t;

#define LSM6DSO32X_TIMESTAMP0                   0x40U
#define LSM6DSO32X_TIMESTAMP1                   0x41U
#define LSM6DSO32X_TIMESTAMP2                   0x42U
#define LSM6DSO32X_TIMESTAMP3                   0x43U

#define LSM6DSO32X_TAP_CFG0                     0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lir                      : 1;
  uint8_t tap_z_en                 : 1;
  uint8_t tap_y_en                 : 1;
  uint8_t tap_x_en                 : 1;
  uint8_t slope_fds                : 1;
  uint8_t sleep_status_on_int      : 1;
  uint8_t int_clr_on_read          : 1;
  uint8_t not_used_01              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t int_clr_on_read          : 1;
  uint8_t sleep_status_on_int      : 1;
  uint8_t slope_fds                : 1;
  uint8_t tap_x_en                 : 1;
  uint8_t tap_y_en                 : 1;
  uint8_t tap_z_en                 : 1;
  uint8_t lir                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_tap_cfg0_t;

#define LSM6DSO32X_TAP_CFG1                     0x57U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_x                : 5;
  uint8_t tap_priority             : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_priority             : 3;
  uint8_t tap_ths_x                : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_tap_cfg1_t;

#define LSM6DSO32X_TAP_CFG2                     0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_y                : 5;
  uint8_t inact_en                 : 2;
  uint8_t interrupts_enable        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t interrupts_enable        : 1;
  uint8_t inact_en                 : 2;
  uint8_t tap_ths_y                : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_tap_cfg2_t;

#define LSM6DSO32X_TAP_THS_6D                   0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_z                : 5;
  uint8_t sixd_ths                 : 2;
  uint8_t d4d_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t d4d_en                   : 1;
  uint8_t sixd_ths                 : 2;
  uint8_t tap_ths_z                : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_tap_ths_6d_t;

#define LSM6DSO32X_INT_DUR2                     0x5AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t shock                    : 2;
  uint8_t quiet                    : 2;
  uint8_t dur                      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dur                      : 4;
  uint8_t quiet                    : 2;
  uint8_t shock                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_int_dur2_t;

#define LSM6DSO32X_WAKE_UP_THS                  0x5BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wk_ths                   : 6;
  uint8_t usr_off_on_wu            : 1;
  uint8_t single_double_tap        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t single_double_tap        : 1;
  uint8_t usr_off_on_wu            : 1;
  uint8_t wk_ths                   : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_wake_up_ths_t;

#define LSM6DSO32X_WAKE_UP_DUR                  0x5CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sleep_dur                : 4;
  uint8_t wake_ths_w               : 1;
  uint8_t wake_dur                 : 2;
  uint8_t ff_dur                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                   : 1;
  uint8_t wake_dur                 : 2;
  uint8_t wake_ths_w               : 1;
  uint8_t sleep_dur                : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_wake_up_dur_t;

#define LSM6DSO32X_FREE_FALL                    0x5DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths                   : 3;
  uint8_t ff_dur                   : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                   : 5;
  uint8_t ff_ths                   : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_free_fall_t;

#define LSM6DSO32X_MD1_CFG                      0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_shub                : 1;
  uint8_t int1_emb_func            : 1;
  uint8_t int1_6d                  : 1;
  uint8_t int1_double_tap          : 1;
  uint8_t int1_ff                  : 1;
  uint8_t int1_wu                  : 1;
  uint8_t int1_single_tap          : 1;
  uint8_t int1_sleep_change        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_sleep_change        : 1;
  uint8_t int1_single_tap          : 1;
  uint8_t int1_wu                  : 1;
  uint8_t int1_ff                  : 1;
  uint8_t int1_double_tap          : 1;
  uint8_t int1_6d                  : 1;
  uint8_t int1_emb_func            : 1;
  uint8_t int1_shub                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_md1_cfg_t;

#define LSM6DSO32X_MD2_CFG                      0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_timestamp           : 1;
  uint8_t int2_emb_func            : 1;
  uint8_t int2_6d                  : 1;
  uint8_t int2_double_tap          : 1;
  uint8_t int2_ff                  : 1;
  uint8_t int2_wu                  : 1;
  uint8_t int2_single_tap          : 1;
  uint8_t int2_sleep_change        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_sleep_change        : 1;
  uint8_t int2_single_tap          : 1;
  uint8_t int2_wu                  : 1;
  uint8_t int2_ff                  : 1;
  uint8_t int2_double_tap          : 1;
  uint8_t int2_6d                  : 1;
  uint8_t int2_emb_func            : 1;
  uint8_t int2_timestamp           : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_md2_cfg_t;

#define LSM6DSO32X_I3C_BUS_AVB                  0x62U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_dis_int1              : 1;
  uint8_t not_used_01              : 2;
  uint8_t i3c_bus_avb_sel          : 2;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t i3c_bus_avb_sel          : 2;
  uint8_t not_used_01              : 2;
  uint8_t pd_dis_int1              : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_i3c_bus_avb_t;

#define LSM6DSO32X_INTERNAL_FREQ_FINE           0x63U
typedef struct
{
  uint8_t freq_fine                : 8;
} lsm6dso32x_internal_freq_fine_t;

#define LSM6DSO32X_X_OFS_USR                    0x73U
#define LSM6DSO32X_Y_OFS_USR                    0x74U
#define LSM6DSO32X_Z_OFS_USR                    0x75U
#define LSM6DSO32X_FIFO_DATA_OUT_TAG            0x78U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tag_parity               : 1;
  uint8_t tag_cnt                  : 2;
  uint8_t tag_sensor               : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tag_sensor               : 5;
  uint8_t tag_cnt                  : 2;
  uint8_t tag_parity               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fifo_data_out_tag_t;

#define LSM6DSO32X_FIFO_DATA_OUT_X_L            0x79
#define LSM6DSO32X_FIFO_DATA_OUT_X_H            0x7A
#define LSM6DSO32X_FIFO_DATA_OUT_Y_L            0x7B
#define LSM6DSO32X_FIFO_DATA_OUT_Y_H            0x7C
#define LSM6DSO32X_FIFO_DATA_OUT_Z_L            0x7D
#define LSM6DSO32X_FIFO_DATA_OUT_Z_H            0x7E

#define LSM6DSO32X_PAGE_SEL                     0x02U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 4;
  uint8_t page_sel                 : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_sel                 : 4;
  uint8_t not_used_01              : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_page_sel_t;

#define LSM6DSO32X_EMB_FUNC_EN_A                0x04U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 3;
  uint8_t pedo_en                  : 1;
  uint8_t tilt_en                  : 1;
  uint8_t sign_motion_en           : 1;
  uint8_t not_used_02              : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 2;
  uint8_t sign_motion_en           : 1;
  uint8_t tilt_en                  : 1;
  uint8_t pedo_en                  : 1;
  uint8_t not_used_01              : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_en_a_t;

#define LSM6DSO32X_EMB_FUNC_EN_B                0x05U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_en                   : 1;
  uint8_t not_used_01              : 2;
  uint8_t fifo_compr_en            : 1;
  uint8_t mlc_en                   : 1;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t mlc_en                   : 1;
  uint8_t fifo_compr_en            : 1;
  uint8_t not_used_01              : 2;
  uint8_t fsm_en                   : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_en_b_t;

#define LSM6DSO32X_PAGE_ADDRESS                 0x08U
typedef struct
{
  uint8_t page_addr                : 8;
} lsm6dso32x_page_address_t;

#define LSM6DSO32X_PAGE_VALUE                   0x09U
typedef struct
{
  uint8_t page_value               : 8;
} lsm6dso32x_page_value_t;

#define LSM6DSO32X_EMB_FUNC_INT1                0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 3;
  uint8_t int1_step_detector       : 1;
  uint8_t int1_tilt                : 1;
  uint8_t int1_sig_mot             : 1;
  uint8_t not_used_02              : 1;
  uint8_t int1_fsm_lc              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm_lc              : 1;
  uint8_t not_used_02              : 1;
  uint8_t int1_sig_mot             : 1;
  uint8_t int1_tilt                : 1;
  uint8_t int1_step_detector       : 1;
  uint8_t not_used_01              : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_int1_t;

#define LSM6DSO32X_FSM_INT1_A                   0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_fsm1                : 1;
  uint8_t int1_fsm2                : 1;
  uint8_t int1_fsm3                : 1;
  uint8_t int1_fsm4                : 1;
  uint8_t int1_fsm5                : 1;
  uint8_t int1_fsm6                : 1;
  uint8_t int1_fsm7                : 1;
  uint8_t int1_fsm8                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm8                : 1;
  uint8_t int1_fsm7                : 1;
  uint8_t int1_fsm6                : 1;
  uint8_t int1_fsm5                : 1;
  uint8_t int1_fsm4                : 1;
  uint8_t int1_fsm3                : 1;
  uint8_t int1_fsm2                : 1;
  uint8_t int1_fsm1                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_int1_a_t;

#define LSM6DSO32X_FSM_INT1_B                   0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_fsm16               : 1;
  uint8_t int1_fsm15               : 1;
  uint8_t int1_fsm14               : 1;
  uint8_t int1_fsm13               : 1;
  uint8_t int1_fsm12               : 1;
  uint8_t int1_fsm11               : 1;
  uint8_t int1_fsm10               : 1;
  uint8_t int1_fsm9                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_int1_b_t;

#define LSM6DSO32X_MLC_INT1                     0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_mlc1                : 1;
  uint8_t int1_mlc2                : 1;
  uint8_t int1_mlc3                : 1;
  uint8_t int1_mlc4                : 1;
  uint8_t int1_mlc5                : 1;
  uint8_t int1_mlc6                : 1;
  uint8_t int1_mlc7                : 1;
  uint8_t int1_mlc8                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_mlc8                : 1;
  uint8_t int1_mlc7                : 1;
  uint8_t int1_mlc6                : 1;
  uint8_t int1_mlc5                : 1;
  uint8_t int1_mlc4                : 1;
  uint8_t int1_mlc3                : 1;
  uint8_t int1_mlc2                : 1;
  uint8_t int1_mlc1                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mlc_int1_t;

#define LSM6DSO32X_EMB_FUNC_INT2                0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 3;
  uint8_t int2_step_detector       : 1;
  uint8_t int2_tilt                : 1;
  uint8_t int2_sig_mot             : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_fsm_lc              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm_lc              : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_sig_mot             : 1;
  uint8_t int2_tilt                : 1;
  uint8_t int2_step_detector       : 1;
  uint8_t not_used_01              : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_int2_t;

#define LSM6DSO32X_FSM_INT2_A                   0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_fsm1                : 1;
  uint8_t int2_fsm2                : 1;
  uint8_t int2_fsm3                : 1;
  uint8_t int2_fsm4                : 1;
  uint8_t int2_fsm5                : 1;
  uint8_t int2_fsm6                : 1;
  uint8_t int2_fsm7                : 1;
  uint8_t int2_fsm8                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm8                : 1;
  uint8_t int2_fsm7                : 1;
  uint8_t int2_fsm6                : 1;
  uint8_t int2_fsm5                : 1;
  uint8_t int2_fsm4                : 1;
  uint8_t int2_fsm3                : 1;
  uint8_t int2_fsm2                : 1;
  uint8_t int2_fsm1                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_int2_a_t;

#define LSM6DSO32X_FSM_INT2_B                   0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_fsm9                : 1;
  uint8_t int2_fsm10               : 1;
  uint8_t int2_fsm11               : 1;
  uint8_t int2_fsm12               : 1;
  uint8_t int2_fsm13               : 1;
  uint8_t int2_fsm14               : 1;
  uint8_t int2_fsm15               : 1;
  uint8_t int2_fsm16               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm16               : 1;
  uint8_t int2_fsm15               : 1;
  uint8_t int2_fsm14               : 1;
  uint8_t int2_fsm13               : 1;
  uint8_t int2_fsm12               : 1;
  uint8_t int2_fsm11               : 1;
  uint8_t int2_fsm10               : 1;
  uint8_t int2_fsm9                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_int2_b_t;

#define LSM6DSO32X_MLC_INT2                     0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_mlc1             : 1;
  uint8_t int2_mlc2             : 1;
  uint8_t int2_mlc3             : 1;
  uint8_t int2_mlc4             : 1;
  uint8_t int2_mlc5             : 1;
  uint8_t int2_mlc6             : 1;
  uint8_t int2_mlc7             : 1;
  uint8_t int2_mlc8             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_mlc8             : 1;
  uint8_t int2_mlc7             : 1;
  uint8_t int2_mlc6             : 1;
  uint8_t int2_mlc5             : 1;
  uint8_t int2_mlc4             : 1;
  uint8_t int2_mlc3             : 1;
  uint8_t int2_mlc2             : 1;
  uint8_t int2_mlc1             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mlc_int2_t;

#define LSM6DSO32X_EMB_FUNC_STATUS              0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 3;
  uint8_t is_step_det              : 1;
  uint8_t is_tilt                  : 1;
  uint8_t is_sigmot                : 1;
  uint8_t not_used_02              : 1;
  uint8_t is_fsm_lc                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                : 1;
  uint8_t not_used_02              : 1;
  uint8_t is_sigmot                : 1;
  uint8_t is_tilt                  : 1;
  uint8_t is_step_det              : 1;
  uint8_t not_used_01              : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_status_t;

#define LSM6DSO32X_FSM_STATUS_A                 0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                  : 1;
  uint8_t is_fsm2                  : 1;
  uint8_t is_fsm3                  : 1;
  uint8_t is_fsm4                  : 1;
  uint8_t is_fsm5                  : 1;
  uint8_t is_fsm6                  : 1;
  uint8_t is_fsm7                  : 1;
  uint8_t is_fsm8                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                  : 1;
  uint8_t is_fsm7                  : 1;
  uint8_t is_fsm6                  : 1;
  uint8_t is_fsm5                  : 1;
  uint8_t is_fsm4                  : 1;
  uint8_t is_fsm3                  : 1;
  uint8_t is_fsm2                  : 1;
  uint8_t is_fsm1                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_status_a_t;

#define LSM6DSO32X_FSM_STATUS_B                 0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm9                  : 1;
  uint8_t is_fsm10                 : 1;
  uint8_t is_fsm11                 : 1;
  uint8_t is_fsm12                 : 1;
  uint8_t is_fsm13                 : 1;
  uint8_t is_fsm14                 : 1;
  uint8_t is_fsm15                 : 1;
  uint8_t is_fsm16                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm16                 : 1;
  uint8_t is_fsm15                 : 1;
  uint8_t is_fsm14                 : 1;
  uint8_t is_fsm13                 : 1;
  uint8_t is_fsm12                 : 1;
  uint8_t is_fsm11                 : 1;
  uint8_t is_fsm10                 : 1;
  uint8_t is_fsm9                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_status_b_t;

#define LSM6DSO32X_MLC_STATUS                   0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1            : 1;
  uint8_t is_mlc2            : 1;
  uint8_t is_mlc3            : 1;
  uint8_t is_mlc4            : 1;
  uint8_t is_mlc5            : 1;
  uint8_t is_mlc6            : 1;
  uint8_t is_mlc7            : 1;
  uint8_t is_mlc8            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_mlc8            : 1;
  uint8_t is_mlc7            : 1;
  uint8_t is_mlc6            : 1;
  uint8_t is_mlc5            : 1;
  uint8_t is_mlc4            : 1;
  uint8_t is_mlc3            : 1;
  uint8_t is_mlc2            : 1;
  uint8_t is_mlc1            : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mlc_status_t;

#define LSM6DSO32X_PAGE_RW                      0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t page_rw                  : 2;  /* page_write + page_read */
  uint8_t emb_func_lir             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_lir             : 1;
  uint8_t page_rw                  : 2;  /* page_write + page_read */
  uint8_t not_used_01              : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_page_rw_t;

#define LSM6DSO32X_EMB_FUNC_FIFO_CFG             0x44U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_00              : 6;
  uint8_t pedo_fifo_en             : 1;
  uint8_t not_used_01              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t pedo_fifo_en             : 1;
  uint8_t not_used_00              : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_fifo_cfg_t;

#define LSM6DSO32X_FSM_ENABLE_A                 0x46U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm1_en                  : 1;
  uint8_t fsm2_en                  : 1;
  uint8_t fsm3_en                  : 1;
  uint8_t fsm4_en                  : 1;
  uint8_t fsm5_en                  : 1;
  uint8_t fsm6_en                  : 1;
  uint8_t fsm7_en                  : 1;
  uint8_t fsm8_en                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm8_en                  : 1;
  uint8_t fsm7_en                  : 1;
  uint8_t fsm6_en                  : 1;
  uint8_t fsm5_en                  : 1;
  uint8_t fsm4_en                  : 1;
  uint8_t fsm3_en                  : 1;
  uint8_t fsm2_en                  : 1;
  uint8_t fsm1_en                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_enable_a_t;

#define LSM6DSO32X_FSM_ENABLE_B                 0x47U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm9_en                  : 1;
  uint8_t fsm10_en                 : 1;
  uint8_t fsm11_en                 : 1;
  uint8_t fsm12_en                 : 1;
  uint8_t fsm13_en                 : 1;
  uint8_t fsm14_en                 : 1;
  uint8_t fsm15_en                 : 1;
  uint8_t fsm16_en                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm16_en                 : 1;
  uint8_t fsm15_en                 : 1;
  uint8_t fsm14_en                 : 1;
  uint8_t fsm13_en                 : 1;
  uint8_t fsm12_en                 : 1;
  uint8_t fsm11_en                 : 1;
  uint8_t fsm10_en                 : 1;
  uint8_t fsm9_en                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_enable_b_t;

#define LSM6DSO32X_FSM_LONG_COUNTER_L           0x48U
#define LSM6DSO32X_FSM_LONG_COUNTER_H           0x49U
#define LSM6DSO32X_FSM_LONG_COUNTER_CLEAR       0x4AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
uint8_t fsm_lc_clr               :
  2;  /* fsm_lc_cleared + fsm_lc_clear */
  uint8_t not_used_01              : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 6;
uint8_t fsm_lc_clr               :
  2;  /* fsm_lc_cleared + fsm_lc_clear */
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_long_counter_clear_t;

#define LSM6DSO32X_FSM_OUTS1                    0x4CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs1_t;

#define LSM6DSO32X_FSM_OUTS2                    0x4DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs2_t;

#define LSM6DSO32X_FSM_OUTS3                    0x4EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs3_t;

#define LSM6DSO32X_FSM_OUTS4                    0x4FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs4_t;

#define LSM6DSO32X_FSM_OUTS5                    0x50U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs5_t;

#define LSM6DSO32X_FSM_OUTS6                    0x51U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs6_t;

#define LSM6DSO32X_FSM_OUTS7                    0x52U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs7_t;

#define LSM6DSO32X_FSM_OUTS8                    0x53U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs8_t;

#define LSM6DSO32X_FSM_OUTS9                    0x54U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs9_t;

#define LSM6DSO32X_FSM_OUTS10                   0x55U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs10_t;

#define LSM6DSO32X_FSM_OUTS11                   0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs11_t;

#define LSM6DSO32X_FSM_OUTS12                   0x57U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs12_t;

#define LSM6DSO32X_FSM_OUTS13                   0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs13_t;

#define LSM6DSO32X_FSM_OUTS14                   0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs14_t;

#define LSM6DSO32X_FSM_OUTS15                   0x5AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs15_t;

#define LSM6DSO32X_FSM_OUTS16                   0x5BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_fsm_outs16_t;

#define LSM6DSO32X_EMB_FUNC_ODR_CFG_B           0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 3;
  uint8_t fsm_odr                  : 2;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t fsm_odr                  : 2;
  uint8_t not_used_01              : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_odr_cfg_b_t;

#define LSM6DSO32X_EMB_FUNC_ODR_CFG_C           0x60U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01             : 4;
  uint8_t mlc_odr                 : 2;
  uint8_t not_used_02             : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02             : 2;
  uint8_t mlc_odr                 : 2;
  uint8_t not_used_01             : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_odr_cfg_c_t;

#define LSM6DSO32X_STEP_COUNTER_L               0x62U
#define LSM6DSO32X_STEP_COUNTER_H               0x63U
#define LSM6DSO32X_EMB_FUNC_SRC                 0x64U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 2;
  uint8_t stepcounter_bit_set      : 1;
  uint8_t step_overflow            : 1;
  uint8_t step_count_delta_ia      : 1;
  uint8_t step_detected            : 1;
  uint8_t not_used_02              : 1;
  uint8_t pedo_rst_step            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pedo_rst_step            : 1;
  uint8_t not_used_02              : 1;
  uint8_t step_detected            : 1;
  uint8_t step_count_delta_ia      : 1;
  uint8_t step_overflow            : 1;
  uint8_t stepcounter_bit_set      : 1;
  uint8_t not_used_01              : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_src_t;

#define LSM6DSO32X_EMB_FUNC_INIT_A              0x66U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01               : 3;
  uint8_t step_det_init             : 1;
  uint8_t tilt_init                 : 1;
  uint8_t sig_mot_init              : 1;
  uint8_t not_used_02               : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02               : 2;
  uint8_t sig_mot_init              : 1;
  uint8_t tilt_init                 : 1;
  uint8_t step_det_init             : 1;
  uint8_t not_used_01               : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_init_a_t;

#define LSM6DSO32X_EMB_FUNC_INIT_B              0x67U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_init                 : 1;
  uint8_t not_used_01              : 2;
  uint8_t fifo_compr_init          : 1;
  uint8_t mlc_init                 : 1;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t mlc_init                 : 1;
  uint8_t fifo_compr_init          : 1;
  uint8_t not_used_01              : 2;
  uint8_t fsm_init                 : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_emb_func_init_b_t;

#define LSM6DSO32X_MLC0_SRC                     0x70U
#define LSM6DSO32X_MLC1_SRC                     0x71U
#define LSM6DSO32X_MLC2_SRC                     0x72U
#define LSM6DSO32X_MLC3_SRC                     0x73U
#define LSM6DSO32X_MLC4_SRC                     0x74U
#define LSM6DSO32X_MLC5_SRC                     0x75U
#define LSM6DSO32X_MLC6_SRC                     0x76U
#define LSM6DSO32X_MLC7_SRC                     0x77U
#define LSM6DSO32X_MAG_SENSITIVITY_L            0xBAU
#define LSM6DSO32X_MAG_SENSITIVITY_H            0xBBU
#define LSM6DSO32X_MAG_OFFX_L                   0xC0U
#define LSM6DSO32X_MAG_OFFX_H                   0xC1U
#define LSM6DSO32X_MAG_OFFY_L                   0xC2U
#define LSM6DSO32X_MAG_OFFY_H                   0xC3U
#define LSM6DSO32X_MAG_OFFZ_L                   0xC4U
#define LSM6DSO32X_MAG_OFFZ_H                   0xC5U
#define LSM6DSO32X_MAG_SI_XX_L                  0xC6U
#define LSM6DSO32X_MAG_SI_XX_H                  0xC7U
#define LSM6DSO32X_MAG_SI_XY_L                  0xC8U
#define LSM6DSO32X_MAG_SI_XY_H                  0xC9U
#define LSM6DSO32X_MAG_SI_XZ_L                  0xCAU
#define LSM6DSO32X_MAG_SI_XZ_H                  0xCBU
#define LSM6DSO32X_MAG_SI_YY_L                  0xCCU
#define LSM6DSO32X_MAG_SI_YY_H                  0xCDU
#define LSM6DSO32X_MAG_SI_YZ_L                  0xCEU
#define LSM6DSO32X_MAG_SI_YZ_H                  0xCFU
#define LSM6DSO32X_MAG_SI_ZZ_L                  0xD0U
#define LSM6DSO32X_MAG_SI_ZZ_H                  0xD1U
#define LSM6DSO32X_MAG_CFG_A                    0xD4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mag_z_axis               : 3;
  uint8_t not_used_01              : 1;
  uint8_t mag_y_axis               : 3;
  uint8_t not_used_02              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 1;
  uint8_t mag_y_axis               : 3;
  uint8_t not_used_01              : 1;
  uint8_t mag_z_axis               : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mag_cfg_a_t;

#define LSM6DSO32X_MAG_CFG_B                    0xD5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mag_x_axis               : 3;
  uint8_t not_used_01              : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t mag_x_axis               : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_mag_cfg_b_t;

#define LSM6DSO32X_FSM_LC_TIMEOUT_L             0x17AU
#define LSM6DSO32X_FSM_LC_TIMEOUT_H             0x17BU
#define LSM6DSO32X_FSM_PROGRAMS                 0x17CU
#define LSM6DSO32X_FSM_START_ADD_L              0x17EU
#define LSM6DSO32X_FSM_START_ADD_H              0x17FU
#define LSM6DSO32X_PEDO_CMD_REG                 0x183U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ad_det_en                : 1;
  uint8_t not_used_01              : 1;
  uint8_t fp_rejection_en          : 1;
  uint8_t carry_count_en           : 1;
  uint8_t not_used_02              : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 4;
  uint8_t carry_count_en           : 1;
  uint8_t fp_rejection_en          : 1;
  uint8_t not_used_01              : 1;
  uint8_t ad_det_en                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_pedo_cmd_reg_t;

#define LSM6DSO32X_PEDO_DEB_STEPS_CONF          0x184U
#define LSM6DSO32X_PEDO_SC_DELTAT_L             0x1D0U
#define LSM6DSO32X_PEDO_SC_DELTAT_H             0x1D1U

#define LSM6DSO32X_MLC_MAG_SENSITIVITY_L        0x1E8U
#define LSM6DSO32X_MLC_MAG_SENSITIVITY_H        0x1E9U

#define LSM6DSO32X_SENSOR_HUB_1                 0x02U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_1_t;

#define LSM6DSO32X_SENSOR_HUB_2                 0x03U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_2_t;

#define LSM6DSO32X_SENSOR_HUB_3                 0x04U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_3_t;

#define LSM6DSO32X_SENSOR_HUB_4                 0x05U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_4_t;

#define LSM6DSO32X_SENSOR_HUB_5                 0x06U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_5_t;

#define LSM6DSO32X_SENSOR_HUB_6                 0x07U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_6_t;

#define LSM6DSO32X_SENSOR_HUB_7                 0x08U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_7_t;

#define LSM6DSO32X_SENSOR_HUB_8                 0x09U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_8_t;

#define LSM6DSO32X_SENSOR_HUB_9                 0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_9_t;

#define LSM6DSO32X_SENSOR_HUB_10                0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_10_t;

#define LSM6DSO32X_SENSOR_HUB_11                0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_11_t;

#define LSM6DSO32X_SENSOR_HUB_12                0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_12_t;

#define LSM6DSO32X_SENSOR_HUB_13                0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_13_t;

#define LSM6DSO32X_SENSOR_HUB_14                0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_14_t;

#define LSM6DSO32X_SENSOR_HUB_15                0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_15_t;

#define LSM6DSO32X_SENSOR_HUB_16                0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_16_t;

#define LSM6DSO32X_SENSOR_HUB_17                0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_17_t;

#define LSM6DSO32X_SENSOR_HUB_18                0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit7                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                    : 1;
  uint8_t bit6                    : 1;
  uint8_t bit5                    : 1;
  uint8_t bit4                    : 1;
  uint8_t bit3                    : 1;
  uint8_t bit2                    : 1;
  uint8_t bit1                    : 1;
  uint8_t bit0                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_sensor_hub_18_t;

#define LSM6DSO32X_MASTER_CONFIG                0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t aux_sens_on              : 2;
  uint8_t master_on                : 1;
  uint8_t shub_pu_en               : 1;
  uint8_t pass_through_mode        : 1;
  uint8_t start_config             : 1;
  uint8_t write_once               : 1;
  uint8_t rst_master_regs          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t rst_master_regs          : 1;
  uint8_t write_once               : 1;
  uint8_t start_config             : 1;
  uint8_t pass_through_mode        : 1;
  uint8_t shub_pu_en               : 1;
  uint8_t master_on                : 1;
  uint8_t aux_sens_on              : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_master_config_t;

#define LSM6DSO32X_SLV0_ADD                     0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t rw_0                     : 1;
  uint8_t slave0                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave0                   : 7;
  uint8_t rw_0                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv0_add_t;

#define LSM6DSO32X_SLV0_SUBADD                  0x16U
typedef struct
{
  uint8_t slave0_reg               : 8;
} lsm6dso32x_slv0_subadd_t;

#define LSM6DSO32X_SLV0_CONFIG                  0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave0_numop             : 3;
  uint8_t batch_ext_sens_0_en      : 1;
  uint8_t not_used_01              : 2;
  uint8_t shub_odr                 : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t shub_odr                 : 2;
  uint8_t not_used_01              : 2;
  uint8_t batch_ext_sens_0_en      : 1;
  uint8_t slave0_numop             : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv0_config_t;

#define LSM6DSO32X_SLV1_ADD                     0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_1                      : 1;
  uint8_t slave1_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave1_add               : 7;
  uint8_t r_1                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv1_add_t;

#define LSM6DSO32X_SLV1_SUBADD                  0x19U
typedef struct
{
  uint8_t slave1_reg               : 8;
} lsm6dso32x_slv1_subadd_t;

#define LSM6DSO32X_SLV1_CONFIG                  0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave1_numop             : 3;
  uint8_t batch_ext_sens_1_en      : 1;
  uint8_t not_used_01              : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 4;
  uint8_t batch_ext_sens_1_en      : 1;
  uint8_t slave1_numop             : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv1_config_t;

#define LSM6DSO32X_SLV2_ADD                     0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_2                      : 1;
  uint8_t slave2_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave2_add               : 7;
  uint8_t r_2                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv2_add_t;

#define LSM6DSO32X_SLV2_SUBADD                  0x1CU
typedef struct
{
  uint8_t slave2_reg               : 8;
} lsm6dso32x_slv2_subadd_t;

#define LSM6DSO32X_SLV2_CONFIG                  0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave2_numop             : 3;
  uint8_t batch_ext_sens_2_en      : 1;
  uint8_t not_used_01              : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 4;
  uint8_t batch_ext_sens_2_en      : 1;
  uint8_t slave2_numop             : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv2_config_t;

#define LSM6DSO32X_SLV3_ADD                     0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_3                      : 1;
  uint8_t slave3_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave3_add               : 7;
  uint8_t r_3                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv3_add_t;

#define LSM6DSO32X_SLV3_SUBADD                  0x1FU
typedef struct
{
  uint8_t slave3_reg               : 8;
} lsm6dso32x_slv3_subadd_t;

#define LSM6DSO32X_SLV3_CONFIG                  0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave3_numop             : 3;
  uint8_t  batch_ext_sens_3_en     : 1;
  uint8_t not_used_01              : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 4;
  uint8_t  batch_ext_sens_3_en     : 1;
  uint8_t slave3_numop             : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_slv3_config_t;

#define LSM6DSO32X_DATAWRITE_SLV0               0x21U
typedef struct
{
  uint8_t slave0_dataw             : 8;
} lsm6dso32x_datawrite_slv0_t;

#define LSM6DSO32X_STATUS_MASTER                0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sens_hub_endop           : 1;
  uint8_t not_used_01              : 2;
  uint8_t slave0_nack              : 1;
  uint8_t slave1_nack              : 1;
  uint8_t slave2_nack              : 1;
  uint8_t slave3_nack              : 1;
  uint8_t wr_once_done             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wr_once_done             : 1;
  uint8_t slave3_nack              : 1;
  uint8_t slave2_nack              : 1;
  uint8_t slave1_nack              : 1;
  uint8_t slave0_nack              : 1;
  uint8_t not_used_01              : 2;
  uint8_t sens_hub_endop           : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dso32x_status_master_t;

#define LSM6DSO32X_START_FSM_ADD                0x0400U

/**
  * @defgroup LSM6DSO32X_Register_Union
  * @brief    This union group all the registers that has a bitfield
  *           description.
  *           This union is useful but not need by the driver.
  *
  *           REMOVING this union you are compliant with:
  *           MISRA-C 2012 [Rule 19.2] -> " Union are not allowed "
  *
  * @{
  *
  */
typedef union
{
  lsm6dso32x_func_cfg_access_t               func_cfg_access;
  lsm6dso32x_pin_ctrl_t                      pin_ctrl;
  lsm6dso32x_fifo_ctrl1_t                    fifo_ctrl1;
  lsm6dso32x_fifo_ctrl2_t                    fifo_ctrl2;
  lsm6dso32x_fifo_ctrl3_t                    fifo_ctrl3;
  lsm6dso32x_fifo_ctrl4_t                    fifo_ctrl4;
  lsm6dso32x_counter_bdr_reg1_t              counter_bdr_reg1;
  lsm6dso32x_counter_bdr_reg2_t              counter_bdr_reg2;
  lsm6dso32x_int1_ctrl_t                     int1_ctrl;
  lsm6dso32x_int2_ctrl_t                     int2_ctrl;
  lsm6dso32x_ctrl1_xl_t                      ctrl1_xl;
  lsm6dso32x_ctrl2_g_t                       ctrl2_g;
  lsm6dso32x_ctrl3_c_t                       ctrl3_c;
  lsm6dso32x_ctrl4_c_t                       ctrl4_c;
  lsm6dso32x_ctrl5_c_t                       ctrl5_c;
  lsm6dso32x_ctrl6_c_t                       ctrl6_c;
  lsm6dso32x_ctrl7_g_t                       ctrl7_g;
  lsm6dso32x_ctrl8_xl_t                      ctrl8_xl;
  lsm6dso32x_ctrl9_xl_t                      ctrl9_xl;
  lsm6dso32x_ctrl10_c_t                      ctrl10_c;
  lsm6dso32x_all_int_src_t                   all_int_src;
  lsm6dso32x_wake_up_src_t                   wake_up_src;
  lsm6dso32x_tap_src_t                       tap_src;
  lsm6dso32x_d6d_src_t                       d6d_src;
  lsm6dso32x_status_reg_t                    status_reg;
  lsm6dso32x_fifo_status1_t                  fifo_status1;
  lsm6dso32x_fifo_status2_t                  fifo_status2;
  lsm6dso32x_tap_cfg0_t                      tap_cfg0;
  lsm6dso32x_tap_cfg1_t                      tap_cfg1;
  lsm6dso32x_tap_cfg2_t                      tap_cfg2;
  lsm6dso32x_tap_ths_6d_t                    tap_ths_6d;
  lsm6dso32x_int_dur2_t                      int_dur2;
  lsm6dso32x_wake_up_ths_t                   wake_up_ths;
  lsm6dso32x_wake_up_dur_t                   wake_up_dur;
  lsm6dso32x_free_fall_t                     free_fall;
  lsm6dso32x_md1_cfg_t                       md1_cfg;
  lsm6dso32x_md2_cfg_t                       md2_cfg;
  lsm6dso32x_i3c_bus_avb_t                   i3c_bus_avb;
  lsm6dso32x_internal_freq_fine_t            internal_freq_fine;
  lsm6dso32x_fifo_data_out_tag_t             fifo_data_out_tag;
  lsm6dso32x_page_sel_t                      page_sel;
  lsm6dso32x_emb_func_en_a_t                 emb_func_en_a;
  lsm6dso32x_emb_func_en_b_t                 emb_func_en_b;
  lsm6dso32x_page_address_t                  page_address;
  lsm6dso32x_page_value_t                    page_value;
  lsm6dso32x_emb_func_int1_t                 emb_func_int1;
  lsm6dso32x_fsm_int1_a_t                    fsm_int1_a;
  lsm6dso32x_fsm_int1_b_t                    fsm_int1_b;
  lsm6dso32x_emb_func_int2_t                 emb_func_int2;
  lsm6dso32x_fsm_int2_a_t                    fsm_int2_a;
  lsm6dso32x_fsm_int2_b_t                    fsm_int2_b;
  lsm6dso32x_emb_func_status_t               emb_func_status;
  lsm6dso32x_fsm_status_a_t                  fsm_status_a;
  lsm6dso32x_fsm_status_b_t                  fsm_status_b;
  lsm6dso32x_page_rw_t                       page_rw;
  lsm6dso32x_emb_func_fifo_cfg_t             emb_func_fifo_cfg;
  lsm6dso32x_fsm_enable_a_t                  fsm_enable_a;
  lsm6dso32x_fsm_enable_b_t                  fsm_enable_b;
  lsm6dso32x_fsm_long_counter_clear_t        fsm_long_counter_clear;
  lsm6dso32x_fsm_outs1_t                     fsm_outs1;
  lsm6dso32x_fsm_outs2_t                     fsm_outs2;
  lsm6dso32x_fsm_outs3_t                     fsm_outs3;
  lsm6dso32x_fsm_outs4_t                     fsm_outs4;
  lsm6dso32x_fsm_outs5_t                     fsm_outs5;
  lsm6dso32x_fsm_outs6_t                     fsm_outs6;
  lsm6dso32x_fsm_outs7_t                     fsm_outs7;
  lsm6dso32x_fsm_outs8_t                     fsm_outs8;
  lsm6dso32x_fsm_outs9_t                     fsm_outs9;
  lsm6dso32x_fsm_outs10_t                    fsm_outs10;
  lsm6dso32x_fsm_outs11_t                    fsm_outs11;
  lsm6dso32x_fsm_outs12_t                    fsm_outs12;
  lsm6dso32x_fsm_outs13_t                    fsm_outs13;
  lsm6dso32x_fsm_outs14_t                    fsm_outs14;
  lsm6dso32x_fsm_outs15_t                    fsm_outs15;
  lsm6dso32x_fsm_outs16_t                    fsm_outs16;
  lsm6dso32x_emb_func_odr_cfg_b_t            emb_func_odr_cfg_b;
  lsm6dso32x_emb_func_odr_cfg_c_t            emb_func_odr_cfg_c;
  lsm6dso32x_emb_func_src_t                  emb_func_src;
  lsm6dso32x_emb_func_init_a_t               emb_func_init_a;
  lsm6dso32x_emb_func_init_b_t               emb_func_init_b;
  lsm6dso32x_mag_cfg_a_t                     mag_cfg_a;
  lsm6dso32x_mag_cfg_b_t                     mag_cfg_b;
  lsm6dso32x_pedo_cmd_reg_t                  pedo_cmd_reg;
  lsm6dso32x_sensor_hub_1_t                  sensor_hub_1;
  lsm6dso32x_sensor_hub_2_t                  sensor_hub_2;
  lsm6dso32x_sensor_hub_3_t                  sensor_hub_3;
  lsm6dso32x_sensor_hub_4_t                  sensor_hub_4;
  lsm6dso32x_sensor_hub_5_t                  sensor_hub_5;
  lsm6dso32x_sensor_hub_6_t                  sensor_hub_6;
  lsm6dso32x_sensor_hub_7_t                  sensor_hub_7;
  lsm6dso32x_sensor_hub_8_t                  sensor_hub_8;
  lsm6dso32x_sensor_hub_9_t                  sensor_hub_9;
  lsm6dso32x_sensor_hub_10_t                 sensor_hub_10;
  lsm6dso32x_sensor_hub_11_t                 sensor_hub_11;
  lsm6dso32x_sensor_hub_12_t                 sensor_hub_12;
  lsm6dso32x_sensor_hub_13_t                 sensor_hub_13;
  lsm6dso32x_sensor_hub_14_t                 sensor_hub_14;
  lsm6dso32x_sensor_hub_15_t                 sensor_hub_15;
  lsm6dso32x_sensor_hub_16_t                 sensor_hub_16;
  lsm6dso32x_sensor_hub_17_t                 sensor_hub_17;
  lsm6dso32x_sensor_hub_18_t                 sensor_hub_18;
  lsm6dso32x_master_config_t                 master_config;
  lsm6dso32x_slv0_add_t                      slv0_add;
  lsm6dso32x_slv0_subadd_t                   slv0_subadd;
  lsm6dso32x_slv0_config_t                   slv0_config;
  lsm6dso32x_slv1_add_t                      slv1_add;
  lsm6dso32x_slv1_subadd_t                   slv1_subadd;
  lsm6dso32x_slv1_config_t                   slv1_config;
  lsm6dso32x_slv2_add_t                      slv2_add;
  lsm6dso32x_slv2_subadd_t                   slv2_subadd;
  lsm6dso32x_slv2_config_t                   slv2_config;
  lsm6dso32x_slv3_add_t                      slv3_add;
  lsm6dso32x_slv3_subadd_t                   slv3_subadd;
  lsm6dso32x_slv3_config_t                   slv3_config;
  lsm6dso32x_datawrite_slv0_t                datawrite_slv0;
  lsm6dso32x_status_master_t                 status_master;
  bitwise_t                                bitwise;
  uint8_t                                  byte;
} lsm6dso32x_reg_t;

/**
  * @}
  *
  */

int32_t lsm6dso32x_read_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                            uint16_t len);
int32_t lsm6dso32x_write_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                             uint16_t len);

float_t lsm6dso32x_from_fs4_to_mg(int16_t lsb);
float_t lsm6dso32x_from_fs8_to_mg(int16_t lsb);
float_t lsm6dso32x_from_fs16_to_mg(int16_t lsb);
float_t lsm6dso32x_from_fs32_to_mg(int16_t lsb);
float_t lsm6dso32x_from_fs125_to_mdps(int16_t lsb);
float_t lsm6dso32x_from_fs500_to_mdps(int16_t lsb);
float_t lsm6dso32x_from_fs250_to_mdps(int16_t lsb);
float_t lsm6dso32x_from_fs1000_to_mdps(int16_t lsb);
float_t lsm6dso32x_from_fs2000_to_mdps(int16_t lsb);
float_t lsm6dso32x_from_lsb_to_celsius(int16_t lsb);
float_t lsm6dso32x_from_lsb_to_nsec(int16_t lsb);

typedef enum
{
  LSM6DSO32X_4g   = 0,
  LSM6DSO32X_32g  = 1,
  LSM6DSO32X_8g   = 2,
  LSM6DSO32X_16g  = 3,
} lsm6dso32x_fs_xl_t;
int32_t lsm6dso32x_xl_full_scale_set(stmdev_ctx_t *ctx, lsm6dso32x_fs_xl_t val);
int32_t lsm6dso32x_xl_full_scale_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_fs_xl_t *val);

typedef enum
{
  LSM6DSO32X_XL_ODR_OFF    = 0,
  LSM6DSO32X_XL_ODR_12Hz5  = 1,
  LSM6DSO32X_XL_ODR_26Hz   = 2,
  LSM6DSO32X_XL_ODR_52Hz   = 3,
  LSM6DSO32X_XL_ODR_104Hz  = 4,
  LSM6DSO32X_XL_ODR_208Hz  = 5,
  LSM6DSO32X_XL_ODR_417Hz  = 6,
  LSM6DSO32X_XL_ODR_833Hz  = 7,
  LSM6DSO32X_XL_ODR_1667Hz = 8,
  LSM6DSO32X_XL_ODR_3333Hz = 9,
  LSM6DSO32X_XL_ODR_6667Hz = 10,
  LSM6DSO32X_XL_ODR_1Hz6   = 11, /* (low power only) */
} lsm6dso32x_odr_xl_t;
int32_t lsm6dso32x_xl_data_rate_set(stmdev_ctx_t *ctx, lsm6dso32x_odr_xl_t val);
int32_t lsm6dso32x_xl_data_rate_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_odr_xl_t *val);

typedef enum
{
  LSM6DSO32X_250dps   = 0,
  LSM6DSO32X_125dps   = 1,
  LSM6DSO32X_500dps   = 2,
  LSM6DSO32X_1000dps  = 4,
  LSM6DSO32X_2000dps  = 6,
} lsm6dso32x_fs_g_t;
int32_t lsm6dso32x_gy_full_scale_set(stmdev_ctx_t *ctx, lsm6dso32x_fs_g_t val);
int32_t lsm6dso32x_gy_full_scale_get(stmdev_ctx_t *ctx, lsm6dso32x_fs_g_t *val);

typedef enum
{
  LSM6DSO32X_GY_ODR_OFF    = 0,
  LSM6DSO32X_GY_ODR_12Hz5  = 1,
  LSM6DSO32X_GY_ODR_26Hz   = 2,
  LSM6DSO32X_GY_ODR_52Hz   = 3,
  LSM6DSO32X_GY_ODR_104Hz  = 4,
  LSM6DSO32X_GY_ODR_208Hz  = 5,
  LSM6DSO32X_GY_ODR_417Hz  = 6,
  LSM6DSO32X_GY_ODR_833Hz  = 7,
  LSM6DSO32X_GY_ODR_1667Hz = 8,
  LSM6DSO32X_GY_ODR_3333Hz = 9,
  LSM6DSO32X_GY_ODR_6667Hz = 10,
} lsm6dso32x_odr_g_t;
int32_t lsm6dso32x_gy_data_rate_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_odr_g_t val);
int32_t lsm6dso32x_gy_data_rate_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_odr_g_t *val);

int32_t lsm6dso32x_block_data_update_set(stmdev_ctx_t *ctx,
                                         uint8_t val);
int32_t lsm6dso32x_block_data_update_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

typedef enum
{
  LSM6DSO32X_LSb_1mg  = 0,
  LSM6DSO32X_LSb_16mg = 1,
} lsm6dso32x_usr_off_w_t;
int32_t lsm6dso32x_xl_offset_weight_set(stmdev_ctx_t *ctx,
                                        lsm6dso32x_usr_off_w_t val);
int32_t lsm6dso32x_xl_offset_weight_get(stmdev_ctx_t *ctx,
                                        lsm6dso32x_usr_off_w_t *val);

typedef enum
{
  LSM6DSO32X_HIGH_PERFORMANCE_MD  = 0,
  LSM6DSO32X_LOW_NORMAL_POWER_MD  = 1,
  LSM6DSO32X_ULTRA_LOW_POWER_MD   = 2,
} lsm6dso32x_xl_hm_mode_t;
int32_t lsm6dso32x_xl_power_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_xl_hm_mode_t val);
int32_t lsm6dso32x_xl_power_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_xl_hm_mode_t *val);

typedef enum
{
  LSM6DSO32X_GY_HIGH_PERFORMANCE  = 0,
  LSM6DSO32X_GY_NORMAL            = 1,
} lsm6dso32x_g_hm_mode_t;
int32_t lsm6dso32x_gy_power_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_g_hm_mode_t val);
int32_t lsm6dso32x_gy_power_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_g_hm_mode_t *val);

int32_t lsm6dso32x_status_reg_get(stmdev_ctx_t *ctx,
                                  lsm6dso32x_status_reg_t *val);

int32_t lsm6dso32x_xl_flag_data_ready_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_gy_flag_data_ready_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_temp_flag_data_ready_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lsm6dso32x_xl_usr_offset_x_set(stmdev_ctx_t *ctx,
                                       uint8_t *buff);
int32_t lsm6dso32x_xl_usr_offset_x_get(stmdev_ctx_t *ctx,
                                       uint8_t *buff);

int32_t lsm6dso32x_xl_usr_offset_y_set(stmdev_ctx_t *ctx,
                                       uint8_t *buff);
int32_t lsm6dso32x_xl_usr_offset_y_get(stmdev_ctx_t *ctx,
                                       uint8_t *buff);

int32_t lsm6dso32x_xl_usr_offset_z_set(stmdev_ctx_t *ctx,
                                       uint8_t *buff);
int32_t lsm6dso32x_xl_usr_offset_z_get(stmdev_ctx_t *ctx,
                                       uint8_t *buff);

int32_t lsm6dso32x_xl_usr_offset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_xl_usr_offset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_timestamp_rst(stmdev_ctx_t *ctx);

int32_t lsm6dso32x_timestamp_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_timestamp_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_timestamp_raw_get(stmdev_ctx_t *ctx,
                                     uint32_t *val);

typedef enum
{
  LSM6DSO32X_NO_ROUND      = 0,
  LSM6DSO32X_ROUND_XL      = 1,
  LSM6DSO32X_ROUND_GY      = 2,
  LSM6DSO32X_ROUND_GY_XL   = 3,
} lsm6dso32x_rounding_t;
int32_t lsm6dso32x_rounding_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_rounding_t val);
int32_t lsm6dso32x_rounding_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_rounding_t *val);

int32_t lsm6dso32x_temperature_raw_get(stmdev_ctx_t *ctx,
                                       int16_t *val);

int32_t lsm6dso32x_angular_rate_raw_get(stmdev_ctx_t *ctx,
                                        int16_t *val);

int32_t lsm6dso32x_acceleration_raw_get(stmdev_ctx_t *ctx,
                                        int16_t *val);

int32_t lsm6dso32x_fifo_out_raw_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso32x_number_of_steps_get(stmdev_ctx_t *ctx,
                                       uint16_t *val);

int32_t lsm6dso32x_steps_reset(stmdev_ctx_t *ctx);

int32_t lsm6dso32x_mlc_out_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso32x_odr_cal_reg_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_odr_cal_reg_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_USER_BANK           = 0,
  LSM6DSO32X_SENSOR_HUB_BANK     = 1,
  LSM6DSO32X_EMBEDDED_FUNC_BANK  = 2,
} lsm6dso32x_reg_access_t;
int32_t lsm6dso32x_mem_bank_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_reg_access_t val);
int32_t lsm6dso32x_mem_bank_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_reg_access_t *val);

int32_t lsm6dso32x_ln_pg_write_byte(stmdev_ctx_t *ctx,
                                    uint16_t address,
                                    uint8_t *val);
int32_t lsm6dso32x_ln_pg_read_byte(stmdev_ctx_t *ctx,
                                   uint16_t address,
                                   uint8_t *val);

int32_t lsm6dso32x_ln_pg_write(stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len);
int32_t lsm6dso32x_ln_pg_read(stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *val);

typedef enum
{
  LSM6DSO32X_DRDY_LATCHED = 0,
  LSM6DSO32X_DRDY_PULSED  = 1,
} lsm6dso32x_dataready_pulsed_t;
int32_t lsm6dso32x_data_ready_mode_set(stmdev_ctx_t *ctx,
                                       lsm6dso32x_dataready_pulsed_t val);
int32_t lsm6dso32x_data_ready_mode_get(stmdev_ctx_t *ctx,
                                       lsm6dso32x_dataready_pulsed_t *val);

int32_t lsm6dso32x_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso32x_reset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_auto_increment_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_auto_increment_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lsm6dso32x_boot_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_boot_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_XL_ST_DISABLE  = 0,
  LSM6DSO32X_XL_ST_POSITIVE = 1,
  LSM6DSO32X_XL_ST_NEGATIVE = 2,
} lsm6dso32x_st_xl_t;
int32_t lsm6dso32x_xl_self_test_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_st_xl_t val);
int32_t lsm6dso32x_xl_self_test_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_st_xl_t *val);

typedef enum
{
  LSM6DSO32X_GY_ST_DISABLE  = 0,
  LSM6DSO32X_GY_ST_POSITIVE = 1,
  LSM6DSO32X_GY_ST_NEGATIVE = 3,
} lsm6dso32x_st_g_t;
int32_t lsm6dso32x_gy_self_test_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_st_g_t val);
int32_t lsm6dso32x_gy_self_test_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_st_g_t *val);

int32_t lsm6dso32x_xl_filter_lp2_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_xl_filter_lp2_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_gy_filter_lp1_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_gy_filter_lp1_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_filter_settling_mask_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm6dso32x_filter_settling_mask_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

typedef enum
{
  LSM6DSO32X_ULTRA_LIGHT  = 0,
  LSM6DSO32X_VERY_LIGHT   = 1,
  LSM6DSO32X_LIGHT        = 2,
  LSM6DSO32X_MEDIUM       = 3,
  LSM6DSO32X_STRONG       = 4,
  LSM6DSO32X_VERY_STRONG  = 5,
  LSM6DSO32X_AGGRESSIVE   = 6,
  LSM6DSO32X_XTREME       = 7,
} lsm6dso32x_ftype_t;
int32_t lsm6dso32x_gy_lp1_bandwidth_set(stmdev_ctx_t *ctx,
                                        lsm6dso32x_ftype_t val);
int32_t lsm6dso32x_gy_lp1_bandwidth_get(stmdev_ctx_t *ctx,
                                        lsm6dso32x_ftype_t *val);

int32_t lsm6dso32x_xl_lp2_on_6d_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_xl_lp2_on_6d_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_HP_PATH_DISABLE_ON_OUT    = 0x00,
  LSM6DSO32X_SLOPE_ODR_DIV_4           = 0x10,
  LSM6DSO32X_HP_ODR_DIV_10             = 0x11,
  LSM6DSO32X_HP_ODR_DIV_20             = 0x12,
  LSM6DSO32X_HP_ODR_DIV_45             = 0x13,
  LSM6DSO32X_HP_ODR_DIV_100            = 0x14,
  LSM6DSO32X_HP_ODR_DIV_200            = 0x15,
  LSM6DSO32X_HP_ODR_DIV_400            = 0x16,
  LSM6DSO32X_HP_ODR_DIV_800            = 0x17,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_10      = 0x31,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_20      = 0x32,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_45      = 0x33,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_100     = 0x34,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_200     = 0x35,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_400     = 0x36,
  LSM6DSO32X_HP_REF_MD_ODR_DIV_800     = 0x37,
  LSM6DSO32X_LP_ODR_DIV_10             = 0x01,
  LSM6DSO32X_LP_ODR_DIV_20             = 0x02,
  LSM6DSO32X_LP_ODR_DIV_45             = 0x03,
  LSM6DSO32X_LP_ODR_DIV_100            = 0x04,
  LSM6DSO32X_LP_ODR_DIV_200            = 0x05,
  LSM6DSO32X_LP_ODR_DIV_400            = 0x06,
  LSM6DSO32X_LP_ODR_DIV_800            = 0x07,
} lsm6dso32x_hp_slope_xl_en_t;
int32_t lsm6dso32x_xl_hp_path_on_out_set(stmdev_ctx_t *ctx,
                                         lsm6dso32x_hp_slope_xl_en_t val);
int32_t lsm6dso32x_xl_hp_path_on_out_get(stmdev_ctx_t *ctx,
                                         lsm6dso32x_hp_slope_xl_en_t *val);

int32_t lsm6dso32x_xl_fast_settling_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_xl_fast_settling_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

typedef enum
{
  LSM6DSO32X_USE_SLOPE = 0,
  LSM6DSO32X_USE_HPF   = 1,
} lsm6dso32x_slope_fds_t;
int32_t lsm6dso32x_xl_hp_path_internal_set(stmdev_ctx_t *ctx,
                                           lsm6dso32x_slope_fds_t val);
int32_t lsm6dso32x_xl_hp_path_internal_get(stmdev_ctx_t *ctx,
                                           lsm6dso32x_slope_fds_t *val);

typedef enum
{
  LSM6DSO32X_HP_FILTER_NONE     = 0x00,
  LSM6DSO32X_HP_FILTER_16mHz    = 0x80,
  LSM6DSO32X_HP_FILTER_65mHz    = 0x81,
  LSM6DSO32X_HP_FILTER_260mHz   = 0x82,
  LSM6DSO32X_HP_FILTER_1Hz04    = 0x83,
} lsm6dso32x_hpm_g_t;
int32_t lsm6dso32x_gy_hp_path_internal_set(stmdev_ctx_t *ctx,
                                           lsm6dso32x_hpm_g_t val);
int32_t lsm6dso32x_gy_hp_path_internal_get(stmdev_ctx_t *ctx,
                                           lsm6dso32x_hpm_g_t *val);

typedef enum
{
  LSM6DSO32X_PULL_UP_DISC       = 0,
  LSM6DSO32X_PULL_UP_CONNECT    = 1,
} lsm6dso32x_sdo_pu_en_t;
int32_t lsm6dso32x_sdo_sa0_mode_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sdo_pu_en_t val);
int32_t lsm6dso32x_sdo_sa0_mode_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sdo_pu_en_t *val);

typedef enum
{
  LSM6DSO32X_SPI_4_WIRE = 0,
  LSM6DSO32X_SPI_3_WIRE = 1,
} lsm6dso32x_sim_t;
int32_t lsm6dso32x_spi_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_sim_t val);
int32_t lsm6dso32x_spi_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_sim_t *val);

typedef enum
{
  LSM6DSO32X_I2C_ENABLE  = 0,
  LSM6DSO32X_I2C_DISABLE = 1,
} lsm6dso32x_i2c_disable_t;
int32_t lsm6dso32x_i2c_interface_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_i2c_disable_t val);
int32_t lsm6dso32x_i2c_interface_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_i2c_disable_t *val);

typedef enum
{
  LSM6DSO32X_I3C_DISABLE         = 0x80,
  LSM6DSO32X_I3C_ENABLE_T_50us   = 0x00,
  LSM6DSO32X_I3C_ENABLE_T_2us    = 0x01,
  LSM6DSO32X_I3C_ENABLE_T_1ms    = 0x02,
  LSM6DSO32X_I3C_ENABLE_T_25ms   = 0x03,
} lsm6dso32x_i3c_disable_t;
int32_t lsm6dso32x_i3c_disable_set(stmdev_ctx_t *ctx,
                                   lsm6dso32x_i3c_disable_t val);
int32_t lsm6dso32x_i3c_disable_get(stmdev_ctx_t *ctx,
                                   lsm6dso32x_i3c_disable_t *val);

typedef enum
{
  LSM6DSO32X_PUSH_PULL                         = 0x00,
  LSM6DSO32X_OPEN_DRAIN                        = 0x01,
  LSM6DSO32X_INT1_NOPULL_DOWN_INT2_PUSH_PULL   = 0x02,
  LSM6DSO32X_INT1_NOPULL_DOWN_INT2_OPEN_DRAIN  = 0x03,
} lsm6dso32x_pp_od_t;
int32_t lsm6dso32x_pin_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_pp_od_t val);
int32_t lsm6dso32x_pin_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_pp_od_t *val);

typedef enum
{
  LSM6DSO32X_ACTIVE_HIGH = 0,
  LSM6DSO32X_ACTIVE_LOW  = 1,
} lsm6dso32x_h_lactive_t;
int32_t lsm6dso32x_pin_polarity_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_h_lactive_t val);
int32_t lsm6dso32x_pin_polarity_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_h_lactive_t *val);

int32_t lsm6dso32x_all_on_int1_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_all_on_int1_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_ALL_INT_PULSED            = 0,
  LSM6DSO32X_BASE_LATCHED_EMB_PULSED   = 1,
  LSM6DSO32X_BASE_PULSED_EMB_LATCHED   = 2,
  LSM6DSO32X_ALL_INT_LATCHED           = 3,
} lsm6dso32x_lir_t;
int32_t lsm6dso32x_int_notification_set(stmdev_ctx_t *ctx,
                                        lsm6dso32x_lir_t val);
int32_t lsm6dso32x_int_notification_get(stmdev_ctx_t *ctx,
                                        lsm6dso32x_lir_t *val);

typedef enum
{
  LSM6DSO32X_LSb_FS_DIV_64       = 0,
  LSM6DSO32X_LSb_FS_DIV_256      = 1,
} lsm6dso32x_wake_ths_w_t;
int32_t lsm6dso32x_wkup_ths_weight_set(stmdev_ctx_t *ctx,
                                       lsm6dso32x_wake_ths_w_t val);
int32_t lsm6dso32x_wkup_ths_weight_get(stmdev_ctx_t *ctx,
                                       lsm6dso32x_wake_ths_w_t *val);

int32_t lsm6dso32x_wkup_threshold_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_wkup_threshold_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lsm6dso32x_xl_usr_offset_on_wkup_set(stmdev_ctx_t *ctx,
                                             uint8_t val);
int32_t lsm6dso32x_xl_usr_offset_on_wkup_get(stmdev_ctx_t *ctx,
                                             uint8_t *val);

int32_t lsm6dso32x_wkup_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_wkup_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_gy_sleep_mode_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_gy_sleep_mode_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_DRIVE_SLEEP_CHG_EVENT = 0,
  LSM6DSO32X_DRIVE_SLEEP_STATUS    = 1,
} lsm6dso32x_sleep_status_on_int_t;
int32_t lsm6dso32x_act_pin_notification_set(stmdev_ctx_t *ctx,
                                            lsm6dso32x_sleep_status_on_int_t val);
int32_t lsm6dso32x_act_pin_notification_get(stmdev_ctx_t *ctx,
                                            lsm6dso32x_sleep_status_on_int_t *val);

typedef enum
{
  LSM6DSO32X_XL_AND_GY_NOT_AFFECTED      = 0,
  LSM6DSO32X_XL_12Hz5_GY_NOT_AFFECTED    = 1,
  LSM6DSO32X_XL_12Hz5_GY_SLEEP           = 2,
  LSM6DSO32X_XL_12Hz5_GY_PD              = 3,
} lsm6dso32x_inact_en_t;
int32_t lsm6dso32x_act_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_inact_en_t val);
int32_t lsm6dso32x_act_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_inact_en_t *val);

int32_t lsm6dso32x_act_sleep_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_act_sleep_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_tap_detection_on_z_set(stmdev_ctx_t *ctx,
                                          uint8_t val);
int32_t lsm6dso32x_tap_detection_on_z_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_tap_detection_on_y_set(stmdev_ctx_t *ctx,
                                          uint8_t val);
int32_t lsm6dso32x_tap_detection_on_y_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_tap_detection_on_x_set(stmdev_ctx_t *ctx,
                                          uint8_t val);
int32_t lsm6dso32x_tap_detection_on_x_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_tap_threshold_x_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_tap_threshold_x_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

typedef enum
{
  LSM6DSO32X_XYZ = 0,
  LSM6DSO32X_YXZ = 1,
  LSM6DSO32X_XZY = 2,
  LSM6DSO32X_ZYX = 3,
  LSM6DSO32X_YZX = 5,
  LSM6DSO32X_ZXY = 6,
} lsm6dso32x_tap_priority_t;
int32_t lsm6dso32x_tap_axis_priority_set(stmdev_ctx_t *ctx,
                                         lsm6dso32x_tap_priority_t val);
int32_t lsm6dso32x_tap_axis_priority_get(stmdev_ctx_t *ctx,
                                         lsm6dso32x_tap_priority_t *val);

int32_t lsm6dso32x_tap_threshold_y_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_tap_threshold_y_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm6dso32x_tap_threshold_z_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_tap_threshold_z_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm6dso32x_tap_shock_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_tap_shock_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_tap_quiet_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_tap_quiet_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_tap_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_tap_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_ONLY_SINGLE = 0,
  LSM6DSO32X_BOTH_SINGLE_DOUBLE = 1,
} lsm6dso32x_single_double_tap_t;
int32_t lsm6dso32x_tap_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_single_double_tap_t val);
int32_t lsm6dso32x_tap_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_single_double_tap_t *val);

typedef enum
{
  LSM6DSO32X_DEG_80  = 0,
  LSM6DSO32X_DEG_70  = 1,
  LSM6DSO32X_DEG_60  = 2,
  LSM6DSO32X_DEG_50  = 3,
} lsm6dso32x_sixd_ths_t;
int32_t lsm6dso32x_6d_threshold_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sixd_ths_t val);
int32_t lsm6dso32x_6d_threshold_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sixd_ths_t *val);

int32_t lsm6dso32x_4d_mode_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_4d_mode_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_FF_TSH_312mg = 0,
  LSM6DSO32X_FF_TSH_438mg = 1,
  LSM6DSO32X_FF_TSH_500mg = 2,
} lsm6dso32x_ff_ths_t;
int32_t lsm6dso32x_ff_threshold_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_ff_ths_t val);
int32_t lsm6dso32x_ff_threshold_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_ff_ths_t *val);

int32_t lsm6dso32x_ff_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_ff_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_fifo_watermark_set(stmdev_ctx_t *ctx,
                                      uint16_t val);
int32_t lsm6dso32x_fifo_watermark_get(stmdev_ctx_t *ctx,
                                      uint16_t *val);

int32_t lsm6dso32x_compression_algo_init_set(stmdev_ctx_t *ctx,
                                             uint8_t val);
int32_t lsm6dso32x_compression_algo_init_get(stmdev_ctx_t *ctx,
                                             uint8_t *val);

typedef enum
{
  LSM6DSO32X_CMP_DISABLE  = 0x00,
  LSM6DSO32X_CMP_ALWAYS   = 0x04,
  LSM6DSO32X_CMP_8_TO_1   = 0x05,
  LSM6DSO32X_CMP_16_TO_1  = 0x06,
  LSM6DSO32X_CMP_32_TO_1  = 0x07,
} lsm6dso32x_uncoptr_rate_t;
int32_t lsm6dso32x_compression_algo_set(stmdev_ctx_t *ctx,
                                        lsm6dso32x_uncoptr_rate_t val);
int32_t lsm6dso32x_compression_algo_get(stmdev_ctx_t *ctx,
                                        lsm6dso32x_uncoptr_rate_t *val);

int32_t lsm6dso32x_fifo_virtual_sens_odr_chg_set(stmdev_ctx_t *ctx,
                                                 uint8_t val);
int32_t lsm6dso32x_fifo_virtual_sens_odr_chg_get(stmdev_ctx_t *ctx,
                                                 uint8_t *val);

int32_t lsm6dso32x_compression_algo_real_time_set(stmdev_ctx_t *ctx,
                                                  uint8_t val);
int32_t lsm6dso32x_compression_algo_real_time_get(stmdev_ctx_t *ctx,
                                                  uint8_t *val);

int32_t lsm6dso32x_fifo_stop_on_wtm_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_fifo_stop_on_wtm_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

typedef enum
{
  LSM6DSO32X_XL_NOT_BATCHED       =  0,
  LSM6DSO32X_XL_BATCHED_AT_12Hz5   =  1,
  LSM6DSO32X_XL_BATCHED_AT_26Hz    =  2,
  LSM6DSO32X_XL_BATCHED_AT_52Hz    =  3,
  LSM6DSO32X_XL_BATCHED_AT_104Hz   =  4,
  LSM6DSO32X_XL_BATCHED_AT_208Hz   =  5,
  LSM6DSO32X_XL_BATCHED_AT_417Hz   =  6,
  LSM6DSO32X_XL_BATCHED_AT_833Hz   =  7,
  LSM6DSO32X_XL_BATCHED_AT_1667Hz  =  8,
  LSM6DSO32X_XL_BATCHED_AT_3333Hz  =  9,
  LSM6DSO32X_XL_BATCHED_AT_6667Hz  = 10,
  LSM6DSO32X_XL_BATCHED_AT_6Hz5    = 11,
} lsm6dso32x_bdr_xl_t;
int32_t lsm6dso32x_fifo_xl_batch_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_bdr_xl_t val);
int32_t lsm6dso32x_fifo_xl_batch_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_bdr_xl_t *val);

typedef enum
{
  LSM6DSO32X_GY_NOT_BATCHED         = 0,
  LSM6DSO32X_GY_BATCHED_AT_12Hz5    = 1,
  LSM6DSO32X_GY_BATCHED_AT_26Hz     = 2,
  LSM6DSO32X_GY_BATCHED_AT_52Hz     = 3,
  LSM6DSO32X_GY_BATCHED_AT_104Hz    = 4,
  LSM6DSO32X_GY_BATCHED_AT_208Hz    = 5,
  LSM6DSO32X_GY_BATCHED_AT_417Hz    = 6,
  LSM6DSO32X_GY_BATCHED_AT_833Hz    = 7,
  LSM6DSO32X_GY_BATCHED_AT_1667Hz   = 8,
  LSM6DSO32X_GY_BATCHED_AT_3333Hz   = 9,
  LSM6DSO32X_GY_BATCHED_AT_6667Hz   = 10,
  LSM6DSO32X_GY_BATCHED_AT_6Hz5     = 11,
} lsm6dso32x_bdr_gy_t;
int32_t lsm6dso32x_fifo_gy_batch_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_bdr_gy_t val);
int32_t lsm6dso32x_fifo_gy_batch_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_bdr_gy_t *val);

typedef enum
{
  LSM6DSO32X_BYPASS_MODE             = 0,
  LSM6DSO32X_FIFO_MODE               = 1,
  LSM6DSO32X_STREAM_TO_FIFO_MODE     = 3,
  LSM6DSO32X_BYPASS_TO_STREAM_MODE   = 4,
  LSM6DSO32X_STREAM_MODE             = 6,
  LSM6DSO32X_BYPASS_TO_FIFO_MODE     = 7,
} lsm6dso32x_fifo_mode_t;
int32_t lsm6dso32x_fifo_mode_set(stmdev_ctx_t *ctx,
                                 lsm6dso32x_fifo_mode_t val);
int32_t lsm6dso32x_fifo_mode_get(stmdev_ctx_t *ctx,
                                 lsm6dso32x_fifo_mode_t *val);

typedef enum
{
  LSM6DSO32X_TEMP_NOT_BATCHED        = 0,
  LSM6DSO32X_TEMP_BATCHED_AT_1Hz6    = 1,
  LSM6DSO32X_TEMP_BATCHED_AT_12Hz5   = 2,
  LSM6DSO32X_TEMP_BATCHED_AT_52Hz    = 3,
} lsm6dso32x_odr_t_batch_t;
int32_t lsm6dso32x_fifo_temp_batch_set(stmdev_ctx_t *ctx,
                                       lsm6dso32x_odr_t_batch_t val);
int32_t lsm6dso32x_fifo_temp_batch_get(stmdev_ctx_t *ctx,
                                       lsm6dso32x_odr_t_batch_t *val);

typedef enum
{
  LSM6DSO32X_NO_DECIMATION = 0,
  LSM6DSO32X_DEC_1         = 1,
  LSM6DSO32X_DEC_8         = 2,
  LSM6DSO32X_DEC_32        = 3,
} lsm6dso32x_odr_ts_batch_t;
int32_t lsm6dso32x_fifo_timestamp_decimation_set(stmdev_ctx_t *ctx,
                                                 lsm6dso32x_odr_ts_batch_t val);
int32_t lsm6dso32x_fifo_timestamp_decimation_get(stmdev_ctx_t *ctx,
                                                 lsm6dso32x_odr_ts_batch_t *val);

typedef enum
{
  LSM6DSO32X_XL_BATCH_EVENT   = 0,
  LSM6DSO32X_GYRO_BATCH_EVENT = 1,
} lsm6dso32x_trig_counter_bdr_t;

typedef enum
{
  LSM6DSO32X_GYRO_NC_TAG    = 1,
  LSM6DSO32X_XL_NC_TAG,
  LSM6DSO32X_TEMPERATURE_TAG,
  LSM6DSO32X_TIMESTAMP_TAG,
  LSM6DSO32X_CFG_CHANGE_TAG,
  LSM6DSO32X_XL_NC_T_2_TAG,
  LSM6DSO32X_XL_NC_T_1_TAG,
  LSM6DSO32X_XL_2XC_TAG,
  LSM6DSO32X_XL_3XC_TAG,
  LSM6DSO32X_GYRO_NC_T_2_TAG,
  LSM6DSO32X_GYRO_NC_T_1_TAG,
  LSM6DSO32X_GYRO_2XC_TAG,
  LSM6DSO32X_GYRO_3XC_TAG,
  LSM6DSO32X_SENSORHUB_SLAVE0_TAG,
  LSM6DSO32X_SENSORHUB_SLAVE1_TAG,
  LSM6DSO32X_SENSORHUB_SLAVE2_TAG,
  LSM6DSO32X_SENSORHUB_SLAVE3_TAG,
  LSM6DSO32X_STEP_CPUNTER_TAG,
  LSM6DSO32X_GAME_ROTATION_TAG,
  LSM6DSO32X_GEOMAG_ROTATION_TAG,
  LSM6DSO32X_ROTATION_TAG,
  LSM6DSO32X_SENSORHUB_NACK_TAG  = 0x19,
} lsm6dso32x_fifo_tag_t;
int32_t lsm6dso32x_fifo_cnt_event_batch_set(stmdev_ctx_t *ctx,
                                            lsm6dso32x_trig_counter_bdr_t val);
int32_t lsm6dso32x_fifo_cnt_event_batch_get(stmdev_ctx_t *ctx,
                                            lsm6dso32x_trig_counter_bdr_t *val);

int32_t lsm6dso32x_rst_batch_counter_set(stmdev_ctx_t *ctx,
                                         uint8_t val);
int32_t lsm6dso32x_rst_batch_counter_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

int32_t lsm6dso32x_batch_counter_threshold_set(stmdev_ctx_t *ctx,
                                               uint16_t val);
int32_t lsm6dso32x_batch_counter_threshold_get(stmdev_ctx_t *ctx,
                                               uint16_t *val);

int32_t lsm6dso32x_fifo_data_level_get(stmdev_ctx_t *ctx,
                                       uint16_t *val);

int32_t lsm6dso32x_fifo_status_get(stmdev_ctx_t *ctx,
                                   lsm6dso32x_fifo_status2_t *val);

int32_t lsm6dso32x_fifo_full_flag_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lsm6dso32x_fifo_ovr_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_fifo_wtm_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_fifo_sensor_tag_get(stmdev_ctx_t *ctx,
                                       lsm6dso32x_fifo_tag_t *val);

int32_t lsm6dso32x_fifo_pedo_batch_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_fifo_pedo_batch_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm6dso32x_sh_batch_slave_0_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_sh_batch_slave_0_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

int32_t lsm6dso32x_sh_batch_slave_1_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_sh_batch_slave_1_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

int32_t lsm6dso32x_sh_batch_slave_2_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_sh_batch_slave_2_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

int32_t lsm6dso32x_sh_batch_slave_3_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm6dso32x_sh_batch_slave_3_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

typedef enum
{
  LSM6DSO32X_DEN_DISABLE    = 0,
  LSM6DSO32X_LEVEL_FIFO     = 6,
  LSM6DSO32X_LEVEL_LETCHED  = 3,
  LSM6DSO32X_LEVEL_TRIGGER  = 2,
  LSM6DSO32X_EDGE_TRIGGER   = 4,
} lsm6dso32x_den_mode_t;
int32_t lsm6dso32x_den_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_den_mode_t val);
int32_t lsm6dso32x_den_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_den_mode_t *val);

typedef enum
{
  LSM6DSO32X_DEN_ACT_LOW  = 0,
  LSM6DSO32X_DEN_ACT_HIGH = 1,
} lsm6dso32x_den_lh_t;
int32_t lsm6dso32x_den_polarity_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_den_lh_t val);
int32_t lsm6dso32x_den_polarity_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_den_lh_t *val);

typedef enum
{
  LSM6DSO32X_STAMP_IN_GY_DATA     = 0,
  LSM6DSO32X_STAMP_IN_XL_DATA     = 1,
  LSM6DSO32X_STAMP_IN_GY_XL_DATA  = 2,
} lsm6dso32x_den_xl_g_t;
int32_t lsm6dso32x_den_enable_set(stmdev_ctx_t *ctx,
                                  lsm6dso32x_den_xl_g_t val);
int32_t lsm6dso32x_den_enable_get(stmdev_ctx_t *ctx,
                                  lsm6dso32x_den_xl_g_t *val);

int32_t lsm6dso32x_den_mark_axis_x_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_den_mark_axis_x_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm6dso32x_den_mark_axis_y_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_den_mark_axis_y_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm6dso32x_den_mark_axis_z_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_den_mark_axis_z_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

typedef enum
{
  LSM6DSO32X_PEDO_BASE_MODE            = 0x00,
  LSM6DSO32X_FALSE_STEP_REJ            = 0x10,
  LSM6DSO32X_FALSE_STEP_REJ_ADV_MODE   = 0x30,
} lsm6dso32x_pedo_md_t;
int32_t lsm6dso32x_pedo_sens_set(stmdev_ctx_t *ctx,
                                 lsm6dso32x_pedo_md_t val);
int32_t lsm6dso32x_pedo_sens_get(stmdev_ctx_t *ctx,
                                 lsm6dso32x_pedo_md_t *val);

int32_t lsm6dso32x_pedo_step_detect_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

int32_t lsm6dso32x_pedo_debounce_steps_set(stmdev_ctx_t *ctx,
                                           uint8_t *buff);
int32_t lsm6dso32x_pedo_debounce_steps_get(stmdev_ctx_t *ctx,
                                           uint8_t *buff);

int32_t lsm6dso32x_pedo_steps_period_set(stmdev_ctx_t *ctx,
                                         uint16_t val);
int32_t lsm6dso32x_pedo_steps_period_get(stmdev_ctx_t *ctx,
                                         uint16_t *val);

int32_t lsm6dso32x_pedo_adv_detection_set(stmdev_ctx_t *ctx,
                                          uint8_t val);
int32_t lsm6dso32x_pedo_adv_detection_get(stmdev_ctx_t *ctx,
                                          uint8_t *val);

int32_t lsm6dso32x_pedo_false_step_rejection_set(stmdev_ctx_t *ctx,
                                                 uint8_t val);
int32_t lsm6dso32x_pedo_false_step_rejection_get(stmdev_ctx_t *ctx,
                                                 uint8_t *val);

typedef enum
{
  LSM6DSO32X_EVERY_STEP     = 0,
  LSM6DSO32X_COUNT_OVERFLOW = 1,
} lsm6dso32x_carry_count_en_t;
int32_t lsm6dso32x_pedo_int_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_carry_count_en_t val);
int32_t lsm6dso32x_pedo_int_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_carry_count_en_t *val);

int32_t lsm6dso32x_motion_flag_data_ready_get(stmdev_ctx_t *ctx,
                                              uint8_t *val);

int32_t lsm6dso32x_tilt_flag_data_ready_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lsm6dso32x_sh_mag_sensitivity_set(stmdev_ctx_t *ctx,
                                          uint16_t val);
int32_t lsm6dso32x_sh_mag_sensitivity_get(stmdev_ctx_t *ctx,
                                          uint16_t *val);

int32_t lsm6dso32x_mlc_mag_sensitivity_set(stmdev_ctx_t *ctx,
                                           uint16_t val);
int32_t lsm6dso32x_mlc_mag_sensitivity_get(stmdev_ctx_t *ctx,
                                           uint16_t *val);

int32_t lsm6dso32x_mag_offset_set(stmdev_ctx_t *ctx, int16_t *val);
int32_t lsm6dso32x_mag_offset_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dso32x_mag_soft_iron_set(stmdev_ctx_t *ctx,
                                     uint16_t *val);
int32_t lsm6dso32x_mag_soft_iron_get(stmdev_ctx_t *ctx,
                                     uint16_t *val);

typedef enum
{
  LSM6DSO32X_Z_EQ_Y     = 0,
  LSM6DSO32X_Z_EQ_MIN_Y = 1,
  LSM6DSO32X_Z_EQ_X     = 2,
  LSM6DSO32X_Z_EQ_MIN_X = 3,
  LSM6DSO32X_Z_EQ_MIN_Z = 4,
  LSM6DSO32X_Z_EQ_Z     = 5,
} lsm6dso32x_mag_z_axis_t;
int32_t lsm6dso32x_mag_z_orient_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_z_axis_t val);
int32_t lsm6dso32x_mag_z_orient_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_z_axis_t *val);

typedef enum
{
  LSM6DSO32X_Y_EQ_Y     = 0,
  LSM6DSO32X_Y_EQ_MIN_Y = 1,
  LSM6DSO32X_Y_EQ_X     = 2,
  LSM6DSO32X_Y_EQ_MIN_X = 3,
  LSM6DSO32X_Y_EQ_MIN_Z = 4,
  LSM6DSO32X_Y_EQ_Z     = 5,
} lsm6dso32x_mag_y_axis_t;
int32_t lsm6dso32x_mag_y_orient_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_y_axis_t val);
int32_t lsm6dso32x_mag_y_orient_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_y_axis_t *val);

typedef enum
{
  LSM6DSO32X_X_EQ_Y     = 0,
  LSM6DSO32X_X_EQ_MIN_Y = 1,
  LSM6DSO32X_X_EQ_X     = 2,
  LSM6DSO32X_X_EQ_MIN_X = 3,
  LSM6DSO32X_X_EQ_MIN_Z = 4,
  LSM6DSO32X_X_EQ_Z     = 5,
} lsm6dso32x_mag_x_axis_t;
int32_t lsm6dso32x_mag_x_orient_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_x_axis_t val);
int32_t lsm6dso32x_mag_x_orient_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_mag_x_axis_t *val);

int32_t lsm6dso32x_long_cnt_flag_data_ready_get(stmdev_ctx_t *ctx,
                                                uint8_t *val);

typedef struct
{
  lsm6dso32x_fsm_enable_a_t          fsm_enable_a;
  lsm6dso32x_fsm_enable_b_t          fsm_enable_b;
} lsm6dso32x_emb_fsm_enable_t;
int32_t lsm6dso32x_fsm_enable_set(stmdev_ctx_t *ctx,
                                  lsm6dso32x_emb_fsm_enable_t *val);
int32_t lsm6dso32x_fsm_enable_get(stmdev_ctx_t *ctx,
                                  lsm6dso32x_emb_fsm_enable_t *val);

int32_t lsm6dso32x_long_cnt_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dso32x_long_cnt_get(stmdev_ctx_t *ctx, uint16_t *val);

typedef enum
{
  LSM6DSO32X_LC_NORMAL     = 0,
  LSM6DSO32X_LC_CLEAR      = 1,
  LSM6DSO32X_LC_CLEAR_DONE = 2,
} lsm6dso32x_fsm_lc_clr_t;
int32_t lsm6dso32x_long_clr_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_fsm_lc_clr_t val);
int32_t lsm6dso32x_long_clr_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_fsm_lc_clr_t *val);

typedef struct
{
  lsm6dso32x_fsm_outs1_t    fsm_outs1;
  lsm6dso32x_fsm_outs2_t    fsm_outs2;
  lsm6dso32x_fsm_outs3_t    fsm_outs3;
  lsm6dso32x_fsm_outs4_t    fsm_outs4;
  lsm6dso32x_fsm_outs5_t    fsm_outs5;
  lsm6dso32x_fsm_outs6_t    fsm_outs6;
  lsm6dso32x_fsm_outs7_t    fsm_outs7;
  lsm6dso32x_fsm_outs8_t    fsm_outs8;
  lsm6dso32x_fsm_outs1_t    fsm_outs9;
  lsm6dso32x_fsm_outs2_t    fsm_outs10;
  lsm6dso32x_fsm_outs3_t    fsm_outs11;
  lsm6dso32x_fsm_outs4_t    fsm_outs12;
  lsm6dso32x_fsm_outs5_t    fsm_outs13;
  lsm6dso32x_fsm_outs6_t    fsm_outs14;
  lsm6dso32x_fsm_outs7_t    fsm_outs15;
  lsm6dso32x_fsm_outs8_t    fsm_outs16;
} lsm6dso32x_fsm_out_t;
int32_t lsm6dso32x_fsm_out_get(stmdev_ctx_t *ctx,
                               lsm6dso32x_fsm_out_t *val);

typedef enum
{
  LSM6DSO32X_ODR_FSM_12Hz5 = 0,
  LSM6DSO32X_ODR_FSM_26Hz  = 1,
  LSM6DSO32X_ODR_FSM_52Hz  = 2,
  LSM6DSO32X_ODR_FSM_104Hz = 3,
} lsm6dso32x_fsm_odr_t;
int32_t lsm6dso32x_fsm_data_rate_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_fsm_odr_t val);
int32_t lsm6dso32x_fsm_data_rate_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_fsm_odr_t *val);

int32_t lsm6dso32x_fsm_init_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_fsm_init_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso32x_long_cnt_int_value_set(stmdev_ctx_t *ctx,
                                          uint16_t val);
int32_t lsm6dso32x_long_cnt_int_value_get(stmdev_ctx_t *ctx,
                                          uint16_t *val);

int32_t lsm6dso32x_fsm_number_of_programs_set(stmdev_ctx_t *ctx,
                                              uint8_t val);
int32_t lsm6dso32x_fsm_number_of_programs_get(stmdev_ctx_t *ctx,
                                              uint8_t *val);

int32_t lsm6dso32x_fsm_start_address_set(stmdev_ctx_t *ctx,
                                         uint16_t val);
int32_t lsm6dso32x_fsm_start_address_get(stmdev_ctx_t *ctx,
                                         uint16_t *val);

int32_t lsm6dso32x_mlc_status_get(stmdev_ctx_t *ctx,
                                  lsm6dso32x_mlc_status_mainpage_t *val);

typedef enum
{
  LSM6DSO32X_ODR_PRGS_12Hz5 = 0,
  LSM6DSO32X_ODR_PRGS_26Hz  = 1,
  LSM6DSO32X_ODR_PRGS_52Hz  = 2,
  LSM6DSO32X_ODR_PRGS_104Hz = 3,
} lsm6dso32x_mlc_odr_t;
int32_t lsm6dso32x_mlc_data_rate_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_mlc_odr_t val);
int32_t lsm6dso32x_mlc_data_rate_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_mlc_odr_t *val);

typedef struct
{
  lsm6dso32x_sensor_hub_1_t   sh_byte_1;
  lsm6dso32x_sensor_hub_2_t   sh_byte_2;
  lsm6dso32x_sensor_hub_3_t   sh_byte_3;
  lsm6dso32x_sensor_hub_4_t   sh_byte_4;
  lsm6dso32x_sensor_hub_5_t   sh_byte_5;
  lsm6dso32x_sensor_hub_6_t   sh_byte_6;
  lsm6dso32x_sensor_hub_7_t   sh_byte_7;
  lsm6dso32x_sensor_hub_8_t   sh_byte_8;
  lsm6dso32x_sensor_hub_9_t   sh_byte_9;
  lsm6dso32x_sensor_hub_10_t  sh_byte_10;
  lsm6dso32x_sensor_hub_11_t  sh_byte_11;
  lsm6dso32x_sensor_hub_12_t  sh_byte_12;
  lsm6dso32x_sensor_hub_13_t  sh_byte_13;
  lsm6dso32x_sensor_hub_14_t  sh_byte_14;
  lsm6dso32x_sensor_hub_15_t  sh_byte_15;
  lsm6dso32x_sensor_hub_16_t  sh_byte_16;
  lsm6dso32x_sensor_hub_17_t  sh_byte_17;
  lsm6dso32x_sensor_hub_18_t  sh_byte_18;
} lsm6dso32x_emb_sh_read_t;
int32_t lsm6dso32x_sh_read_data_raw_get(stmdev_ctx_t *ctx,
                                        lsm6dso32x_emb_sh_read_t *val,
                                        uint8_t len);

typedef enum
{
  LSM6DSO32X_SLV_0       = 0,
  LSM6DSO32X_SLV_0_1     = 1,
  LSM6DSO32X_SLV_0_1_2   = 2,
  LSM6DSO32X_SLV_0_1_2_3 = 3,
} lsm6dso32x_aux_sens_on_t;
int32_t lsm6dso32x_sh_slave_connected_set(stmdev_ctx_t *ctx,
                                          lsm6dso32x_aux_sens_on_t val);
int32_t lsm6dso32x_sh_slave_connected_get(stmdev_ctx_t *ctx,
                                          lsm6dso32x_aux_sens_on_t *val);

int32_t lsm6dso32x_sh_master_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dso32x_sh_master_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_EXT_PULL_UP      = 0,
  LSM6DSO32X_INTERNAL_PULL_UP = 1,
} lsm6dso32x_shub_pu_en_t;
int32_t lsm6dso32x_sh_pin_mode_set(stmdev_ctx_t *ctx,
                                   lsm6dso32x_shub_pu_en_t val);
int32_t lsm6dso32x_sh_pin_mode_get(stmdev_ctx_t *ctx,
                                   lsm6dso32x_shub_pu_en_t *val);

int32_t lsm6dso32x_sh_pass_through_set(stmdev_ctx_t *ctx,
                                       uint8_t val);
int32_t lsm6dso32x_sh_pass_through_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

typedef enum
{
  LSM6DSO32X_EXT_ON_INT2_PIN = 1,
  LSM6DSO32X_XL_GY_DRDY      = 0,
} lsm6dso32x_start_config_t;
int32_t lsm6dso32x_sh_syncro_mode_set(stmdev_ctx_t *ctx,
                                      lsm6dso32x_start_config_t val);
int32_t lsm6dso32x_sh_syncro_mode_get(stmdev_ctx_t *ctx,
                                      lsm6dso32x_start_config_t *val);

typedef enum
{
  LSM6DSO32X_EACH_SH_CYCLE    = 0,
  LSM6DSO32X_ONLY_FIRST_CYCLE = 1,
} lsm6dso32x_write_once_t;
int32_t lsm6dso32x_sh_write_mode_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_write_once_t val);
int32_t lsm6dso32x_sh_write_mode_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_write_once_t *val);

int32_t lsm6dso32x_sh_reset_set(stmdev_ctx_t *ctx);
int32_t lsm6dso32x_sh_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSO32X_SH_ODR_104Hz = 0,
  LSM6DSO32X_SH_ODR_52Hz  = 1,
  LSM6DSO32X_SH_ODR_26Hz  = 2,
  LSM6DSO32X_SH_ODR_13Hz  = 3,
} lsm6dso32x_shub_odr_t;
int32_t lsm6dso32x_sh_data_rate_set(stmdev_ctx_t *ctx,
                                    lsm6dso32x_shub_odr_t val);
int32_t lsm6dso32x_sh_data_rate_get(stmdev_ctx_t *ctx,
                                    lsm6dso32x_shub_odr_t *val);

typedef struct
{
  uint8_t   slv0_add;
  uint8_t   slv0_subadd;
  uint8_t   slv0_data;
} lsm6dso32x_sh_cfg_write_t;
int32_t lsm6dso32x_sh_cfg_write(stmdev_ctx_t *ctx,
                                lsm6dso32x_sh_cfg_write_t *val);

typedef struct
{
  uint8_t   slv_add;
  uint8_t   slv_subadd;
  uint8_t   slv_len;
} lsm6dso32x_sh_cfg_read_t;
int32_t lsm6dso32x_sh_slv0_cfg_read(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sh_cfg_read_t *val);
int32_t lsm6dso32x_sh_slv1_cfg_read(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sh_cfg_read_t *val);
int32_t lsm6dso32x_sh_slv2_cfg_read(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sh_cfg_read_t *val);
int32_t lsm6dso32x_sh_slv3_cfg_read(stmdev_ctx_t *ctx,
                                    lsm6dso32x_sh_cfg_read_t *val);

int32_t lsm6dso32x_sh_status_get(stmdev_ctx_t *ctx,
                                 lsm6dso32x_status_master_t *val);
typedef struct
{
  uint8_t ui;
  uint8_t aux;
} lsm6dso32x_id_t;
int32_t lsm6dso32x_id_get(stmdev_ctx_t *ctx,
                          lsm6dso32x_id_t *val);

typedef struct
{
  enum
  {
    LSM6DSO32X_SEL_BY_HW   = 0x00, /* bus mode select by HW (SPI 3W disable) */
    LSM6DSO32X_SPI_4W      = 0x06, /* Only SPI: SDO / SDI separated pins */
    LSM6DSO32X_SPI_3W      = 0x07, /* Only SPI: SDO / SDI share the same pin */
    LSM6DSO32X_I2C         = 0x04, /* Only I2C */
    LSM6DSO32X_I3C_T_50us  = 0x02, /* I3C: available time equal to 50 us */
    LSM6DSO32X_I3C_T_2us   = 0x12, /* I3C: available time equal to 2 us */
    LSM6DSO32X_I3C_T_1ms   = 0x22, /* I3C: available time equal to 1 ms */
    LSM6DSO32X_I3C_T_25ms  = 0x32, /* I3C: available time equal to 25 ms */
  } ui_bus_md;
  enum
  {
    LSM6DSO32X_SPI_4W_AUX  = 0x00,
    LSM6DSO32X_SPI_3W_AUX  = 0x01,
  } aux_bus_md;
} lsm6dso32x_bus_mode_t;
int32_t lsm6dso32x_bus_mode_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_bus_mode_t val);
int32_t lsm6dso32x_bus_mode_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_bus_mode_t *val);

typedef enum
{
  LSM6DSO32X_DRV_RDY   = 0x00, /* Initialize the device for driver usage */
  LSM6DSO32X_BOOT      = 0x01, /* Restore calib. param. ( it takes 10ms ) */
  LSM6DSO32X_RESET     = 0x02, /* Reset configuration registers */
  LSM6DSO32X_FIFO_COMP = 0x04, /* FIFO compression initialization request. */
  LSM6DSO32X_FSM       = 0x08, /* Finite State Machine initialization request */
  LSM6DSO32X_MLC       = 0x10, /* Machine Learning Core initialization request */
  LSM6DSO32X_PEDO      = 0x20, /* Pedometer algo initialization request. */
  LSM6DSO32X_TILT      = 0x40, /* Tilt algo initialization request */
  LSM6DSO32X_SMOTION   = 0x80, /* Significant Motion initialization request */
} lsm6dso32x_init_t;
int32_t lsm6dso32x_init_set(stmdev_ctx_t *ctx, lsm6dso32x_init_t val);

typedef struct
{
uint8_t sw_reset           :
  1; /* Restoring configuration registers */
  uint8_t boot               : 1; /* Restoring calibration parameters */
  uint8_t drdy_xl            : 1; /* Accelerometer data ready */
  uint8_t drdy_g             : 1; /* Gyroscope data ready */
  uint8_t drdy_temp          : 1; /* Temperature data ready */
} lsm6dso32x_status_t;
int32_t lsm6dso32x_status_get(stmdev_ctx_t *ctx,
                              lsm6dso32x_status_t *val);

typedef struct
{
  uint8_t sdo_sa0_pull_up     : 1; /* 1 = pull-up on SDO/SA0 pin */
uint8_t aux_sdo_ocs_pull_up :
  1; /* 1 = pull-up on OCS_Aux/SDO_Aux pins */
  uint8_t int1_int2_push_pull : 1; /* 1 = push-pull / 0 = open-drain*/
uint8_t int1_pull_down      :
  1; /* 1 = pull-down always disabled (0=auto) */
} lsm6dso32x_pin_conf_t;
int32_t lsm6dso32x_pin_conf_set(stmdev_ctx_t *ctx,
                                lsm6dso32x_pin_conf_t val);
int32_t lsm6dso32x_pin_conf_get(stmdev_ctx_t *ctx,
                                lsm6dso32x_pin_conf_t *val);

typedef struct
{
  uint8_t active_low   : 1; /* 1 = active low / 0 = active high */
uint8_t base_latched :
  1; /* base functions are: FF, WU, 6D, Tap, Act/Inac */
uint8_t emb_latched  :
  1; /* emb functions are: Pedo, Tilt, SMot, Timestamp */
} lsm6dso32x_int_mode_t;
int32_t lsm6dso32x_interrupt_mode_set(stmdev_ctx_t *ctx,
                                      lsm6dso32x_int_mode_t val);
int32_t lsm6dso32x_interrupt_mode_get(stmdev_ctx_t *ctx,
                                      lsm6dso32x_int_mode_t *val);

typedef struct
{
  uint8_t drdy_xl       : 1; /* Accelerometer data ready */
  uint8_t drdy_g        : 1; /* Gyroscope data ready */
uint8_t drdy_temp     :
  1; /* Temperature data ready (1 = int2 pin disable) */
  uint8_t boot          : 1; /* Restoring calibration parameters */
  uint8_t fifo_th       : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr      : 1; /* FIFO overrun */
  uint8_t fifo_full     : 1; /* FIFO full */
  uint8_t fifo_bdr      : 1; /* FIFO Batch counter threshold reached */
uint8_t den_flag      :
  1; /* external trigger level recognition (DEN) */
  uint8_t sh_endop      : 1; /* sensor hub end operation */
uint8_t timestamp     :
  1; /* timestamp overflow (1 = int2 pin disable) */
  uint8_t six_d         : 1; /* orientation change (6D/4D detection) */
  uint8_t double_tap    : 1; /* double-tap event */
  uint8_t free_fall     : 1; /* free fall event */
  uint8_t wake_up       : 1; /* wake up event */
  uint8_t single_tap    : 1; /* single-tap event */
uint8_t sleep_change  :
  1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t step_detector : 1; /* Step detected */
  uint8_t tilt          : 1; /* Relative tilt event detected */
  uint8_t sig_mot       : 1; /* "significant motion" event detected */
uint8_t fsm_lc        :
  1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1          : 1; /* fsm 1 interrupt event */
  uint8_t fsm2          : 1; /* fsm 2 interrupt event */
  uint8_t fsm3          : 1; /* fsm 3 interrupt event */
  uint8_t fsm4          : 1; /* fsm 4 interrupt event */
  uint8_t fsm5          : 1; /* fsm 5 interrupt event */
  uint8_t fsm6          : 1; /* fsm 6 interrupt event */
  uint8_t fsm7          : 1; /* fsm 7 interrupt event */
  uint8_t fsm8          : 1; /* fsm 8 interrupt event */
  uint8_t fsm9          : 1; /* fsm 9 interrupt event */
  uint8_t fsm10         : 1; /* fsm 10 interrupt event */
  uint8_t fsm11         : 1; /* fsm 11 interrupt event */
  uint8_t fsm12         : 1; /* fsm 12 interrupt event */
  uint8_t fsm13         : 1; /* fsm 13 interrupt event */
  uint8_t fsm14         : 1; /* fsm 14 interrupt event */
  uint8_t fsm15         : 1; /* fsm 15 interrupt event */
  uint8_t fsm16         : 1; /* fsm 16 interrupt event */
  uint8_t mlc1          : 1; /* mlc 1 interrupt event */
  uint8_t mlc2          : 1; /* mlc 2 interrupt event */
  uint8_t mlc3          : 1; /* mlc 3 interrupt event */
  uint8_t mlc4          : 1; /* mlc 4 interrupt event */
  uint8_t mlc5          : 1; /* mlc 5 interrupt event */
  uint8_t mlc6          : 1; /* mlc 6 interrupt event */
  uint8_t mlc7          : 1; /* mlc 7 interrupt event */
  uint8_t mlc8          : 1; /* mlc 8 interrupt event */
} lsm6dso32x_pin_int1_route_t;

int32_t lsm6dso32x_pin_int1_route_set(stmdev_ctx_t *ctx,
                                      lsm6dso32x_pin_int1_route_t val);
int32_t lsm6dso32x_pin_int1_route_get(stmdev_ctx_t *ctx,
                                      lsm6dso32x_pin_int1_route_t *val);

typedef struct
{
  uint8_t drdy_xl       : 1; /* Accelerometer data ready */
  uint8_t drdy_g        : 1; /* Gyroscope data ready */
  uint8_t drdy_temp     : 1; /* Temperature data ready */
  uint8_t fifo_th       : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr      : 1; /* FIFO overrun */
  uint8_t fifo_full     : 1; /* FIFO full */
  uint8_t fifo_bdr      : 1; /* FIFO Batch counter threshold reached */
  uint8_t timestamp     : 1; /* timestamp overflow */
  uint8_t six_d         : 1; /* orientation change (6D/4D detection) */
  uint8_t double_tap    : 1; /* double-tap event */
  uint8_t free_fall     : 1; /* free fall event */
  uint8_t wake_up       : 1; /* wake up event */
  uint8_t single_tap    : 1; /* single-tap event */
uint8_t sleep_change  :
  1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t step_detector : 1; /* Step detected */
  uint8_t tilt          : 1; /* Relative tilt event detected */
  uint8_t sig_mot       : 1; /* "significant motion" event detected */
uint8_t fsm_lc        :
  1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1          : 1; /* fsm 1 interrupt event */
  uint8_t fsm2          : 1; /* fsm 2 interrupt event */
  uint8_t fsm3          : 1; /* fsm 3 interrupt event */
  uint8_t fsm4          : 1; /* fsm 4 interrupt event */
  uint8_t fsm5          : 1; /* fsm 5 interrupt event */
  uint8_t fsm6          : 1; /* fsm 6 interrupt event */
  uint8_t fsm7          : 1; /* fsm 7 interrupt event */
  uint8_t fsm8          : 1; /* fsm 8 interrupt event */
  uint8_t fsm9          : 1; /* fsm 9 interrupt event */
  uint8_t fsm10         : 1; /* fsm 10 interrupt event */
  uint8_t fsm11         : 1; /* fsm 11 interrupt event */
  uint8_t fsm12         : 1; /* fsm 12 interrupt event */
  uint8_t fsm13         : 1; /* fsm 13 interrupt event */
  uint8_t fsm14         : 1; /* fsm 14 interrupt event */
  uint8_t fsm15         : 1; /* fsm 15 interrupt event */
  uint8_t fsm16         : 1; /* fsm 16 interrupt event */
  uint8_t mlc1          : 1; /* mlc 1 interrupt event */
  uint8_t mlc2          : 1; /* mlc 2 interrupt event */
  uint8_t mlc3          : 1; /* mlc 3 interrupt event */
  uint8_t mlc4          : 1; /* mlc 4 interrupt event */
  uint8_t mlc5          : 1; /* mlc 5 interrupt event */
  uint8_t mlc6          : 1; /* mlc 6 interrupt event */
  uint8_t mlc7          : 1; /* mlc 7 interrupt event */
  uint8_t mlc8          : 1; /* mlc 8 interrupt event */
} lsm6dso32x_pin_int2_route_t;

int32_t lsm6dso32x_pin_int2_route_set(stmdev_ctx_t *ctx,
                                      lsm6dso32x_pin_int2_route_t val);
int32_t lsm6dso32x_pin_int2_route_get(stmdev_ctx_t *ctx,
                                      lsm6dso32x_pin_int2_route_t *val);

typedef struct
{
  uint8_t drdy_xl          :  1; /* Accelerometer data ready */
  uint8_t drdy_g           :  1; /* Gyroscope data ready */
  uint8_t drdy_temp        :  1; /* Temperature data ready */
uint8_t den_flag         :
  1; /* external trigger level recognition (DEN) */
uint8_t timestamp        :
  1; /* timestamp overflow (1 = int2 pin disable) */
  uint8_t free_fall        :  1; /* free fall event */
  uint8_t wake_up          :  1; /* wake up event */
  uint8_t wake_up_z        :  1; /* wake up on Z axis event */
  uint8_t wake_up_y        :  1; /* wake up on Y axis event */
  uint8_t wake_up_x        :  1; /* wake up on X axis event */
  uint8_t single_tap       :  1; /* single-tap event */
  uint8_t double_tap       :  1; /* double-tap event */
  uint8_t tap_z            :  1; /* single-tap on Z axis event */
  uint8_t tap_y            :  1; /* single-tap on Y axis event */
  uint8_t tap_x            :  1; /* single-tap on X axis event */
  uint8_t tap_sign         :  1; /* sign of tap event (0-pos / 1-neg) */
uint8_t six_d            :
  1; /* orientation change (6D/4D detection) */
uint8_t six_d_xl         :
  1; /* X-axis low 6D/4D event (under threshold) */
uint8_t six_d_xh         :
  1; /* X-axis high 6D/4D event (over threshold) */
uint8_t six_d_yl         :
  1; /* Y-axis low 6D/4D event (under threshold) */
uint8_t six_d_yh         :
  1; /* Y-axis high 6D/4D event (over threshold) */
uint8_t six_d_zl         :
  1; /* Z-axis low 6D/4D event (under threshold) */
uint8_t six_d_zh         :
  1; /* Z-axis high 6D/4D event (over threshold) */
uint8_t sleep_change     :
  1; /* Act/Inact (or Vice-versa) status changed */
uint8_t sleep_state      :
  1; /* Act/Inact status flag (0-Act / 1-Inact) */
  uint8_t step_detector    :  1; /* Step detected */
  uint8_t tilt             :  1; /* Relative tilt event detected */
uint8_t sig_mot          :
  1; /* "significant motion" event detected */
uint8_t fsm_lc           :
  1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1             :  1; /* fsm 1 interrupt event */
  uint8_t fsm2             :  1; /* fsm 2 interrupt event */
  uint8_t fsm3             :  1; /* fsm 3 interrupt event */
  uint8_t fsm4             :  1; /* fsm 4 interrupt event */
  uint8_t fsm5             :  1; /* fsm 5 interrupt event */
  uint8_t fsm6             :  1; /* fsm 6 interrupt event */
  uint8_t fsm7             :  1; /* fsm 7 interrupt event */
  uint8_t fsm8             :  1; /* fsm 8 interrupt event */
  uint8_t fsm9             :  1; /* fsm 9 interrupt event */
  uint8_t fsm10            :  1; /* fsm 10 interrupt event */
  uint8_t fsm11            :  1; /* fsm 11 interrupt event */
  uint8_t fsm12            :  1; /* fsm 12 interrupt event */
  uint8_t fsm13            :  1; /* fsm 13 interrupt event */
  uint8_t fsm14            :  1; /* fsm 14 interrupt event */
  uint8_t fsm15            :  1; /* fsm 15 interrupt event */
  uint8_t fsm16            :  1; /* fsm 16 interrupt event */
  uint8_t mlc1             :  1; /* mlc 1 interrupt event */
  uint8_t mlc2             :  1; /* mlc 2 interrupt event */
  uint8_t mlc3             :  1; /* mlc 3 interrupt event */
  uint8_t mlc4             :  1; /* mlc 4 interrupt event */
  uint8_t mlc5             :  1; /* mlc 5 interrupt event */
  uint8_t mlc6             :  1; /* mlc 6 interrupt event */
  uint8_t mlc7             :  1; /* mlc 7 interrupt event */
  uint8_t mlc8             :  1; /* mlc 8 interrupt event */
  uint8_t sh_endop         :  1; /* sensor hub end operation */
uint8_t sh_slave0_nack   :
  1; /* Not acknowledge on sensor hub slave 0 */
uint8_t sh_slave1_nack   :
  1; /* Not acknowledge on sensor hub slave 1 */
uint8_t sh_slave2_nack   :
  1; /* Not acknowledge on sensor hub slave 2 */
uint8_t sh_slave3_nack   :
  1; /* Not acknowledge on sensor hub slave 3 */
uint8_t sh_wr_once       :
  1; /* "WRITE_ONCE" end on sensor hub slave 0 */
uint16_t fifo_diff       :
  10; /* Number of unread sensor data in FIFO*/
  uint8_t fifo_ovr_latched :  1; /* Latched FIFO overrun status */
uint8_t fifo_bdr         :
  1; /* FIFO Batch counter threshold reached */
  uint8_t fifo_full        :  1; /* FIFO full */
  uint8_t fifo_ovr         :  1; /* FIFO overrun */
  uint8_t fifo_th          :  1; /* FIFO threshold reached */
} lsm6dso32x_all_sources_t;
int32_t lsm6dso32x_all_sources_get(stmdev_ctx_t *ctx,
                                   lsm6dso32x_all_sources_t *val);

typedef struct
{
  uint8_t odr_fine_tune;
} lsm6dso32x_dev_cal_t;
int32_t lsm6dso32x_calibration_get(stmdev_ctx_t *ctx,
                                   lsm6dso32x_dev_cal_t *val);

typedef struct
{
  struct
  {
    struct
    {
      enum
      {
        LSM6DSO32X_XL_UI_OFF       = 0x00, /* in power down */
        LSM6DSO32X_XL_UI_1Hz6_LP   = 0x1B, /* @1Hz6 (low power) */
        LSM6DSO32X_XL_UI_1Hz6_ULP  = 0x2B, /* @1Hz6 (ultra low/Gy) */
        LSM6DSO32X_XL_UI_12Hz5_HP  = 0x01, /* @12Hz5 (high performance) */
        LSM6DSO32X_XL_UI_12Hz5_LP  = 0x11, /* @12Hz5 (low power) */
        LSM6DSO32X_XL_UI_12Hz5_ULP = 0x21, /* @12Hz5 (ultra low/Gy) */
        LSM6DSO32X_XL_UI_26Hz_HP   = 0x02, /* @26Hz  (high performance) */
        LSM6DSO32X_XL_UI_26Hz_LP   = 0x12, /* @26Hz  (low power) */
        LSM6DSO32X_XL_UI_26Hz_ULP  = 0x22, /* @26Hz  (ultra low/Gy) */
        LSM6DSO32X_XL_UI_52Hz_HP   = 0x03, /* @52Hz  (high performance) */
        LSM6DSO32X_XL_UI_52Hz_LP   = 0x13, /* @52Hz  (low power) */
        LSM6DSO32X_XL_UI_52Hz_ULP  = 0x23, /* @52Hz  (ultra low/Gy) */
        LSM6DSO32X_XL_UI_104Hz_HP  = 0x04, /* @104Hz (high performance) */
        LSM6DSO32X_XL_UI_104Hz_NM  = 0x14, /* @104Hz (normal mode) */
        LSM6DSO32X_XL_UI_104Hz_ULP = 0x24, /* @104Hz (ultra low/Gy) */
        LSM6DSO32X_XL_UI_208Hz_HP  = 0x05, /* @208Hz (high performance) */
        LSM6DSO32X_XL_UI_208Hz_NM  = 0x15, /* @208Hz (normal mode) */
        LSM6DSO32X_XL_UI_208Hz_ULP = 0x25, /* @208Hz (ultra low/Gy) */
        LSM6DSO32X_XL_UI_416Hz_HP  = 0x06, /* @416Hz (high performance) */
        LSM6DSO32X_XL_UI_833Hz_HP  = 0x07, /* @833Hz (high performance) */
        LSM6DSO32X_XL_UI_1667Hz_HP = 0x08, /* @1kHz66 (high performance) */
        LSM6DSO32X_XL_UI_3333Hz_HP = 0x09, /* @3kHz33 (high performance) */
        LSM6DSO32X_XL_UI_6667Hz_HP = 0x0A, /* @6kHz66 (high performance) */
      } odr;
      enum
      {
        LSM6DSO32X_XL_UI_4g   = 0,
        LSM6DSO32X_XL_UI_32g  = 1,
        LSM6DSO32X_XL_UI_8g   = 2,
        LSM6DSO32X_XL_UI_16g  = 3,
      } fs;
    } xl;
    struct
    {
      enum
      {
        LSM6DSO32X_GY_UI_OFF       = 0x00, /* gy in power down */
        LSM6DSO32X_GY_UI_12Hz5_LP  = 0x11, /* gy @12Hz5 (low power) */
        LSM6DSO32X_GY_UI_12Hz5_HP  = 0x01, /* gy @12Hz5 (high performance) */
        LSM6DSO32X_GY_UI_26Hz_LP   = 0x12, /* gy @26Hz  (low power) */
        LSM6DSO32X_GY_UI_26Hz_HP   = 0x02, /* gy @26Hz  (high performance) */
        LSM6DSO32X_GY_UI_52Hz_LP   = 0x13, /* gy @52Hz  (low power) */
        LSM6DSO32X_GY_UI_52Hz_HP   = 0x03, /* gy @52Hz  (high performance) */
        LSM6DSO32X_GY_UI_104Hz_NM  = 0x14, /* gy @104Hz (low power) */
        LSM6DSO32X_GY_UI_104Hz_HP  = 0x04, /* gy @104Hz (high performance) */
        LSM6DSO32X_GY_UI_208Hz_NM  = 0x15, /* gy @208Hz (low power) */
        LSM6DSO32X_GY_UI_208Hz_HP  = 0x05, /* gy @208Hz (high performance) */
        LSM6DSO32X_GY_UI_416Hz_HP  = 0x06, /* gy @416Hz (high performance) */
        LSM6DSO32X_GY_UI_833Hz_HP  = 0x07, /* gy @833Hz (high performance) */
        LSM6DSO32X_GY_UI_1667Hz_HP = 0x08, /* gy @1kHz66 (high performance) */
        LSM6DSO32X_GY_UI_3333Hz_HP = 0x09, /* gy @3kHz33 (high performance) */
        LSM6DSO32X_GY_UI_6667Hz_HP = 0x0A, /* gy @6kHz66 (high performance) */
      } odr;
      enum
      {
        LSM6DSO32X_GY_UI_250dps   = 0,
        LSM6DSO32X_GY_UI_125dps   = 1,
        LSM6DSO32X_GY_UI_500dps   = 2,
        LSM6DSO32X_GY_UI_1000dps  = 4,
        LSM6DSO32X_GY_UI_2000dps  = 6,
      } fs;
    } gy;
  } ui;
  struct
  {
    enum
    {
      LSM6DSO32X_FSM_DISABLE = 0x00,
      LSM6DSO32X_FSM_XL      = 0x01,
      LSM6DSO32X_FSM_GY      = 0x02,
      LSM6DSO32X_FSM_XL_GY   = 0x03,
    } sens;
    enum
    {
      LSM6DSO32X_FSM_12Hz5 = 0x00,
      LSM6DSO32X_FSM_26Hz  = 0x01,
      LSM6DSO32X_FSM_52Hz  = 0x02,
      LSM6DSO32X_FSM_104Hz = 0x03,
    } odr;
  } fsm;
  struct
  {
    enum
    {
      LSM6DSO32X_MLC_DISABLE = 0x00,
      LSM6DSO32X_MLC_XL      = 0x01,
      LSM6DSO32X_MLC_XL_GY   = 0x03,
    } sens;
    enum
    {
      LSM6DSO32X_MLC_12Hz5 = 0x00,
      LSM6DSO32X_MLC_26Hz  = 0x01,
      LSM6DSO32X_MLC_52Hz  = 0x02,
      LSM6DSO32X_MLC_104Hz = 0x03,
    } odr;
  } mlc;
} lsm6dso32x_md_t;
int32_t lsm6dso32x_mode_set(stmdev_ctx_t *ctx,
                            lsm6dso32x_md_t *val);
int32_t lsm6dso32x_mode_get(stmdev_ctx_t *ctx,
                            lsm6dso32x_md_t *val);

typedef struct
{
  struct
  {
    struct
    {
      float mg[3];
      int16_t raw[3];
    } xl;
    struct
    {
      float mdps[3];
      int16_t raw[3];
    } gy;
    struct
    {
      float deg_c;
      int16_t raw;
    } heat;
  } ui;
} lsm6dso32x_data_t;
int32_t lsm6dso32x_data_get(stmdev_ctx_t *ctx,
                            lsm6dso32x_md_t *md,
                            lsm6dso32x_data_t *data);

typedef struct
{
  uint8_t sig_mot      : 1; /* significant motion */
  uint8_t tilt         : 1; /* tilt detection  */
  uint8_t step         : 1; /* step counter/detector */
  uint8_t mlc          : 1; /* machine learning core */
  uint8_t fsm          : 1; /* finite state machine */
  uint8_t fifo_compr   : 1; /* mlc 8 interrupt event */
} lsm6dso32x_emb_sens_t;
int32_t lsm6dso32x_embedded_sens_set(stmdev_ctx_t *ctx,
                                     lsm6dso32x_emb_sens_t *emb_sens);
int32_t lsm6dso32x_embedded_sens_get(stmdev_ctx_t *ctx,
                                     lsm6dso32x_emb_sens_t *emb_sens);
int32_t lsm6dso32x_embedded_sens_off(stmdev_ctx_t *ctx);

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*LSM6DSO32X_DRIVER_H */
