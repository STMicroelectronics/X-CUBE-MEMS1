/**
  ******************************************************************************
  * @file    asm330lhhx_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          asm330lhhx_reg.c driver.
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
#ifndef ASM330LHHX_REGS_H
#define ASM330LHHX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/** @addtogroup ASM330LHHX
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

/** @defgroup ASM330LHHX Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define ASM330LHHX_I2C_ADD_L                    0xD5U
#define ASM330LHHX_I2C_ADD_H                    0xD7U

/** Device Identification (Who am I) **/
#define ASM330LHHX_ID                           0x6BU

/**
  * @}
  *
  */

#define ASM330LHHX_FUNC_CFG_ACCESS              0x01U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 6;
  uint8_t reg_access               : 2; /* shub_reg_access + func_cfg_access */
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t reg_access               : 2; /* shub_reg_access + func_cfg_access */
  uint8_t not_used_01              : 6;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_func_cfg_access_t;

#define ASM330LHHX_PIN_CTRL                     0x02U
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
} asm330lhhx_pin_ctrl_t;

#define ASM330LHHX_FIFO_CTRL1                   0x07U
typedef struct
{
  uint8_t wtm                      : 8;
} asm330lhhx_fifo_ctrl1_t;

#define ASM330LHHX_FIFO_CTRL2                   0x08U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm                      : 1;
  uint8_t not_used_01              : 3;
  uint8_t odrchg_en                : 1;
  uint8_t not_used_02              : 2;
  uint8_t stop_on_wtm              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t stop_on_wtm              : 1;
  uint8_t not_used_02              : 2;
  uint8_t odrchg_en                : 1;
  uint8_t not_used_01              : 3;
  uint8_t wtm                      : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_fifo_ctrl2_t;

#define ASM330LHHX_FIFO_CTRL3                   0x09U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bdr_xl                   : 4;
  uint8_t bdr_gy                   : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bdr_gy                   : 4;
  uint8_t bdr_xl                   : 4;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_fifo_ctrl3_t;

#define ASM330LHHX_FIFO_CTRL4                   0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_mode                : 3;
  uint8_t not_used_01              : 1;
  uint8_t odr_t_batch              : 2;
  uint8_t dec_ts_batch             : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dec_ts_batch             : 2;
  uint8_t odr_t_batch              : 2;
  uint8_t not_used_01              : 1;
  uint8_t fifo_mode                : 3;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_fifo_ctrl4_t;

#define ASM330LHHX_COUNTER_BDR_REG1             0x0BU
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
} asm330lhhx_counter_bdr_reg1_t;

#define ASM330LHHX_COUNTER_BDR_REG2             0x0CU
typedef struct
{
  uint8_t cnt_bdr_th               : 8;
} asm330lhhx_counter_bdr_reg2_t;

#define ASM330LHHX_INT1_CTRL                    0x0DU
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
} asm330lhhx_int1_ctrl_t;

#define ASM330LHHX_INT2_CTRL                    0x0EU
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
} asm330lhhx_int2_ctrl_t;

#define ASM330LHHX_WHO_AM_I                     0x0FU

#define ASM330LHHX_CTRL1_XL                     0x10U
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
} asm330lhhx_ctrl1_xl_t;

#define ASM330LHHX_CTRL2_G                      0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs_g                     : 4; /* fs_4000 + fs_125 + fs_g */
  uint8_t odr_g                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr_g                    : 4;
  uint8_t fs_g                     : 4; /* fs_4000 + fs_125 + fs_g */
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_ctrl2_g_t;

#define ASM330LHHX_CTRL3_C                      0x12U
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
} asm330lhhx_ctrl3_c_t;

#define ASM330LHHX_CTRL4_C                      0x13U
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
} asm330lhhx_ctrl4_c_t;

#define ASM330LHHX_CTRL5_C                      0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_xl                    : 2;
  uint8_t st_g                     : 2;
  uint8_t not_used_01              : 1;
  uint8_t rounding                 : 2;
  uint8_t not_used_02              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 1;
  uint8_t rounding                 : 2;
  uint8_t not_used_01              : 1;
  uint8_t st_g                     : 2;
  uint8_t st_xl                    : 2;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_ctrl5_c_t;

#define ASM330LHHX_CTRL6_C                      0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ftype                    : 3;
  uint8_t usr_off_w                : 1;
  uint8_t xl_hm_mode               : 1;
  uint8_t den_mode                 : 3;   /* trig_en + lvl1_en + lvl2_en */
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t den_mode                 : 3;   /* trig_en + lvl1_en + lvl2_en */
  uint8_t xl_hm_mode               : 1;
  uint8_t usr_off_w                : 1;
  uint8_t ftype                    : 3;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_ctrl6_c_t;

#define ASM330LHHX_CTRL7_G                      0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_00              : 1;
  uint8_t usr_off_on_out           : 1;
  uint8_t not_used_01              : 2;
  uint8_t hpm_g                    : 2;
  uint8_t hp_en_g                  : 1;
  uint8_t g_hm_mode                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t g_hm_mode                : 1;
  uint8_t hp_en_g                  : 1;
  uint8_t hpm_g                    : 2;
  uint8_t not_used_01              : 2;
  uint8_t usr_off_on_out           : 1;
  uint8_t not_used_00              : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_ctrl7_g_t;

#define ASM330LHHX_CTRL8_XL                     0x17U
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
} asm330lhhx_ctrl8_xl_t;

#define ASM330LHHX_CTRL9_XL                     0x18U
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
} asm330lhhx_ctrl9_xl_t;

#define ASM330LHHX_CTRL10_C                     0x19U
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
} asm330lhhx_ctrl10_c_t;

#define ASM330LHHX_ALL_INT_SRC                  0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ia                    : 1;
  uint8_t wu_ia                    : 1;
  uint8_t not_used_00              : 2;
  uint8_t d6d_ia                   : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t not_used_01              : 1;
  uint8_t timestamp_endcount       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_endcount       : 1;
  uint8_t not_used_01              : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t not_used_00              : 2;
  uint8_t wu_ia                    : 1;
  uint8_t ff_ia                    : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_all_int_src_t;

#define ASM330LHHX_WAKE_UP_SRC                  0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_wu                     : 1;
  uint8_t y_wu                     : 1;
  uint8_t x_wu                     : 1;
  uint8_t wu_ia                    : 1;
  uint8_t sleep_state              : 1;
  uint8_t ff_ia                    : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t not_used_01              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t ff_ia                    : 1;
  uint8_t sleep_state              : 1;
  uint8_t wu_ia                    : 1;
  uint8_t x_wu                     : 1;
  uint8_t y_wu                     : 1;
  uint8_t z_wu                     : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_wake_up_src_t;

#define ASM330LHHX_D6D_SRC                      0x1DU
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
} asm330lhhx_d6d_src_t;

#define ASM330LHHX_STATUS_REG                   0x1EU
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
} asm330lhhx_status_reg_t;

#define ASM330LHHX_OUT_TEMP_L                   0x20U
#define ASM330LHHX_OUT_TEMP_H                   0x21U
#define ASM330LHHX_OUTX_L_G                     0x22U
#define ASM330LHHX_OUTX_H_G                     0x23U
#define ASM330LHHX_OUTY_L_G                     0x24U
#define ASM330LHHX_OUTY_H_G                     0x25U
#define ASM330LHHX_OUTZ_L_G                     0x26U
#define ASM330LHHX_OUTZ_H_G                     0x27U
#define ASM330LHHX_OUTX_L_A                     0x28U
#define ASM330LHHX_OUTX_H_A                     0x29U
#define ASM330LHHX_OUTY_L_A                     0x2AU
#define ASM330LHHX_OUTY_H_A                     0x2BU
#define ASM330LHHX_OUTZ_L_A                     0x2CU
#define ASM330LHHX_OUTZ_H_A                     0x2DU
#define ASM330LHHX_EMB_FUNC_STATUS_MAINPAGE     0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01             : 7;
  uint8_t is_fsm_lc               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc               : 1;
  uint8_t not_used_01             : 7;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_status_mainpage_t;

#define ASM330LHHX_FSM_STATUS_A_MAINPAGE        0x36U
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
} asm330lhhx_fsm_status_a_mainpage_t;

#define ASM330LHHX_FSM_STATUS_B_MAINPAGE        0x37U
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
} asm330lhhx_fsm_status_b_mainpage_t;

#define ASM330LHHX_MLC_STATUS_MAINPAGE          0x38U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1                 : 1;
  uint8_t is_mlc2                 : 1;
  uint8_t is_mlc3                 : 1;
  uint8_t is_mlc4                 : 1;
  uint8_t is_mlc5                 : 1;
  uint8_t is_mlc6                 : 1;
  uint8_t is_mlc7                 : 1;
  uint8_t is_mlc8                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_mlc8                 : 1;
  uint8_t is_mlc7                 : 1;
  uint8_t is_mlc6                 : 1;
  uint8_t is_mlc5                 : 1;
  uint8_t is_mlc4                 : 1;
  uint8_t is_mlc3                 : 1;
  uint8_t is_mlc2                 : 1;
  uint8_t is_mlc1                 : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_mlc_status_mainpage_t;

#define ASM330LHHX_STATUS_MASTER_MAINPAGE       0x39U
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
} asm330lhhx_status_master_mainpage_t;

#define ASM330LHHX_FIFO_STATUS1                 0x3AU
typedef struct
{
  uint8_t diff_fifo                : 8;
} asm330lhhx_fifo_status1_t;

#define ASM330LHHX_FIFO_STATUS2                 0x3BU
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
} asm330lhhx_fifo_status2_t;

#define ASM330LHHX_TIMESTAMP0                   0x40U
#define ASM330LHHX_TIMESTAMP1                   0x41U
#define ASM330LHHX_TIMESTAMP2                   0x42U
#define ASM330LHHX_TIMESTAMP3                   0x43U
#define ASM330LHHX_INT_CFG0                     0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lir                      : 1;
  uint8_t not_used_01              : 3;
  uint8_t slope_fds                : 1;
  uint8_t sleep_status_on_int      : 1;
  uint8_t int_clr_on_read          : 1;
  uint8_t not_used_02              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 1;
  uint8_t int_clr_on_read          : 1;
  uint8_t sleep_status_on_int      : 1;
  uint8_t slope_fds                : 1;
  uint8_t not_used_01              : 3;
  uint8_t lir                      : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_int_cfg0_t;

#define ASM330LHHX_INT_CFG1                     0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t inact_en                 : 2;
  uint8_t interrupts_enable        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t interrupts_enable        : 1;
  uint8_t inact_en                 : 2;
  uint8_t not_used_01              : 5;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_int_cfg1_t;

#define ASM330LHHX_THS_6D                   0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t sixd_ths                 : 2;
  uint8_t d4d_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t d4d_en                   : 1;
  uint8_t sixd_ths                 : 2;
  uint8_t not_used_01              : 5;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_ths_6d_t;

#define ASM330LHHX_WAKE_UP_THS                  0x5BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wk_ths                   : 6;
  uint8_t usr_off_on_wu            : 1;
  uint8_t not_used_01              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t usr_off_on_wu            : 1;
  uint8_t wk_ths                   : 6;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_wake_up_ths_t;

#define ASM330LHHX_WAKE_UP_DUR                  0x5CU
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
} asm330lhhx_wake_up_dur_t;

#define ASM330LHHX_FREE_FALL                    0x5DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths                   : 3;
  uint8_t ff_dur                   : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                   : 5;
  uint8_t ff_ths                   : 3;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_free_fall_t;

#define ASM330LHHX_MD1_CFG                      0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_shub                : 1;
  uint8_t int1_emb_func            : 1;
  uint8_t int1_6d                  : 1;
  uint8_t not_used_01              : 1;
  uint8_t int1_ff                  : 1;
  uint8_t int1_wu                  : 1;
  uint8_t not_used_02              : 1;
  uint8_t int1_sleep_change        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_sleep_change        : 1;
  uint8_t not_used_02              : 1;
  uint8_t int1_wu                  : 1;
  uint8_t int1_ff                  : 1;
  uint8_t not_used_01              : 1;
  uint8_t int1_6d                  : 1;
  uint8_t int1_emb_func            : 1;
  uint8_t int1_shub                : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_md1_cfg_t;

#define ASM330LHHX_MD2_CFG                      0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_timestamp           : 1;
  uint8_t int2_emb_func            : 1;
  uint8_t int2_6d                  : 1;
  uint8_t not_used_01              : 1;
  uint8_t int2_ff                  : 1;
  uint8_t int2_wu                  : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_sleep_change        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_sleep_change        : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_wu                  : 1;
  uint8_t int2_ff                  : 1;
  uint8_t not_used_01              : 1;
  uint8_t int2_6d                  : 1;
  uint8_t int2_emb_func            : 1;
  uint8_t int2_timestamp           : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_md2_cfg_t;

#define ASM330LHHX_I3C_BUS_AVB                  0x62U
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
} asm330lhhx_i3c_bus_avb_t;

#define ASM330LHHX_INTERNAL_FREQ_FINE           0x63U
typedef struct
{
  uint8_t freq_fine                : 8;
} asm330lhhx_internal_freq_fine_t;

#define ASM330LHHX_X_OFS_USR                    0x73U
#define ASM330LHHX_Y_OFS_USR                    0x74U
#define ASM330LHHX_Z_OFS_USR                    0x75U
#define ASM330LHHX_FIFO_DATA_OUT_TAG            0x78U
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
} asm330lhhx_fifo_data_out_tag_t;

#define ASM330LHHX_FIFO_DATA_OUT_X_L            0x79U
#define ASM330LHHX_FIFO_DATA_OUT_X_H            0x7AU
#define ASM330LHHX_FIFO_DATA_OUT_Y_L            0x7BU
#define ASM330LHHX_FIFO_DATA_OUT_Y_H            0x7CU
#define ASM330LHHX_FIFO_DATA_OUT_Z_L            0x7DU
#define ASM330LHHX_FIFO_DATA_OUT_Z_H            0x7EU

#define ASM330LHHX_PAGE_SEL                     0x02U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 1;
  uint8_t emb_func_clk_dis         : 1;
  uint8_t not_used_02              : 2;
  uint8_t page_sel                 : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_sel                 : 4;
  uint8_t not_used_02              : 2;
  uint8_t emb_func_clk_dis         : 1;
  uint8_t not_used_01              : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_page_sel_t;

#define ASM330LHHX_EMB_FUNC_EN_B                0x05U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_en                   : 1;
  uint8_t not_used_01              : 3;
  uint8_t mlc_en                   : 1;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t mlc_en                   : 1;
  uint8_t not_used_01              : 3;
  uint8_t fsm_en                   : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_en_b_t;

#define ASM330LHHX_PAGE_ADDRESS                 0x08U
typedef struct
{
  uint8_t page_addr                : 8;
} asm330lhhx_page_address_t;

#define ASM330LHHX_PAGE_VALUE                   0x09U
typedef struct
{
  uint8_t page_value               : 8;
} asm330lhhx_page_value_t;

#define ASM330LHHX_EMB_FUNC_INT1                0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 7;
  uint8_t int1_fsm_lc              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm_lc              : 1;
  uint8_t not_used_01              : 7;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_int1_t;

#define ASM330LHHX_FSM_INT1_A                   0x0BU
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
} asm330lhhx_fsm_int1_a_t;

#define ASM330LHHX_FSM_INT1_B                   0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_fsm9                : 1;
  uint8_t int1_fsm10               : 1;
  uint8_t int1_fsm11               : 1;
  uint8_t int1_fsm12               : 1;
  uint8_t int1_fsm13               : 1;
  uint8_t int1_fsm14               : 1;
  uint8_t int1_fsm15               : 1;
  uint8_t int1_fsm16               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm16               : 1;
  uint8_t int1_fsm15               : 1;
  uint8_t int1_fsm14               : 1;
  uint8_t int1_fsm13               : 1;
  uint8_t int1_fsm12               : 1;
  uint8_t int1_fsm11               : 1;
  uint8_t int1_fsm10               : 1;
  uint8_t int1_fsm9                : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_fsm_int1_b_t;

#define ASM330LHHX_MLC_INT1                     0x0DU
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
} asm330lhhx_mlc_int1_t;

#define ASM330LHHX_EMB_FUNC_INT2                0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 7;
  uint8_t int2_fsm_lc              : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm_lc              : 1;
  uint8_t not_used_01              : 7;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_int2_t;

#define ASM330LHHX_FSM_INT2_A                   0x0FU
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
} asm330lhhx_fsm_int2_a_t;

#define ASM330LHHX_FSM_INT2_B                   0x10U
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
} asm330lhhx_fsm_int2_b_t;

#define ASM330LHHX_MLC_INT2                     0x11U
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
} asm330lhhx_mlc_int2_t;

#define ASM330LHHX_EMB_FUNC_STATUS              0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01              : 7;
  uint8_t is_fsm_lc                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                : 1;
  uint8_t not_used_01              : 7;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_status_t;

#define ASM330LHHX_FSM_STATUS_A                 0x13U
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
} asm330lhhx_fsm_status_a_t;

#define ASM330LHHX_FSM_STATUS_B                 0x14U
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
} asm330lhhx_fsm_status_b_t;

#define ASM330LHHX_MLC_STATUS                   0x15U
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
} asm330lhhx_mlc_status_t;

#define ASM330LHHX_PAGE_RW                      0x17U
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
} asm330lhhx_page_rw_t;

#define ASM330LHHX_FSM_ENABLE_A                 0x46U
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
} asm330lhhx_fsm_enable_a_t;

#define ASM330LHHX_FSM_ENABLE_B                 0x47U
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
} asm330lhhx_fsm_enable_b_t;

#define ASM330LHHX_FSM_LONG_COUNTER_L           0x48U
#define ASM330LHHX_FSM_LONG_COUNTER_H           0x49U
#define ASM330LHHX_FSM_LONG_COUNTER_CLEAR       0x4AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_clr               : 2;  /* fsm_lc_cleared + fsm_lc_clear */
  uint8_t not_used_01              : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 6;
  uint8_t fsm_lc_clr               : 2;  /* fsm_lc_cleared + fsm_lc_clear */
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_fsm_long_counter_clear_t;

#define ASM330LHHX_FSM_OUTS1                    0x4CU
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
} asm330lhhx_fsm_outs1_t;

#define ASM330LHHX_FSM_OUTS2                    0x4DU
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
} asm330lhhx_fsm_outs2_t;

#define ASM330LHHX_FSM_OUTS3                    0x4EU
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
} asm330lhhx_fsm_outs3_t;

#define ASM330LHHX_FSM_OUTS4                    0x4FU
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
} asm330lhhx_fsm_outs4_t;

#define ASM330LHHX_FSM_OUTS5                    0x50U
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
} asm330lhhx_fsm_outs5_t;

#define ASM330LHHX_FSM_OUTS6                    0x51U
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
} asm330lhhx_fsm_outs6_t;

#define ASM330LHHX_FSM_OUTS7                    0x52U
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
} asm330lhhx_fsm_outs7_t;

#define ASM330LHHX_FSM_OUTS8                    0x53U
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
} asm330lhhx_fsm_outs8_t;

#define ASM330LHHX_FSM_OUTS9                    0x54U
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
} asm330lhhx_fsm_outs9_t;

#define ASM330LHHX_FSM_OUTS10                   0x55U
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
} asm330lhhx_fsm_outs10_t;

#define ASM330LHHX_FSM_OUTS11                   0x56U
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
} asm330lhhx_fsm_outs11_t;

#define ASM330LHHX_FSM_OUTS12                   0x57U
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
} asm330lhhx_fsm_outs12_t;

#define ASM330LHHX_FSM_OUTS13                   0x58U
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
} asm330lhhx_fsm_outs13_t;

#define ASM330LHHX_FSM_OUTS14                   0x59U
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
} asm330lhhx_fsm_outs14_t;

#define ASM330LHHX_FSM_OUTS15                   0x5AU
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
} asm330lhhx_fsm_outs15_t;

#define ASM330LHHX_FSM_OUTS16                   0x5BU
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
} asm330lhhx_fsm_outs16_t;

#define ASM330LHHX_EMB_FUNC_ODR_CFG_B           0x5FU
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
} asm330lhhx_emb_func_odr_cfg_b_t;

#define ASM330LHHX_EMB_FUNC_ODR_CFG_C           0x60U
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
} asm330lhhx_emb_func_odr_cfg_c_t;

#define ASM330LHHX_EMB_FUNC_INIT_B              0x67U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_init                 : 1;
  uint8_t not_used_01              : 3;
  uint8_t mlc_init                 : 1;
  uint8_t not_used_02              : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02              : 3;
  uint8_t mlc_init                 : 1;
  uint8_t not_used_01              : 3;
  uint8_t fsm_init                 : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_emb_func_init_b_t;

#define ASM330LHHX_MLC0_SRC                     0x70U
#define ASM330LHHX_MLC1_SRC                     0x71U
#define ASM330LHHX_MLC2_SRC                     0x72U
#define ASM330LHHX_MLC3_SRC                     0x73U
#define ASM330LHHX_MLC4_SRC                     0x74U
#define ASM330LHHX_MLC5_SRC                     0x75U
#define ASM330LHHX_MLC6_SRC                     0x76U
#define ASM330LHHX_MLC7_SRC                     0x77U

#define ASM330LHHX_MAG_SENSITIVITY_L            0xBAU
#define ASM330LHHX_MAG_SENSITIVITY_H            0xBBU
#define ASM330LHHX_MAG_OFFX_L                   0xC0U
#define ASM330LHHX_MAG_OFFX_H                   0xC1U
#define ASM330LHHX_MAG_OFFY_L                   0xC2U
#define ASM330LHHX_MAG_OFFY_H                   0xC3U
#define ASM330LHHX_MAG_OFFZ_L                   0xC4U
#define ASM330LHHX_MAG_OFFZ_H                   0xC5U
#define ASM330LHHX_MAG_SI_XX_L                  0xC6U
#define ASM330LHHX_MAG_SI_XX_H                  0xC7U
#define ASM330LHHX_MAG_SI_XY_L                  0xC8U
#define ASM330LHHX_MAG_SI_XY_H                  0xC9U
#define ASM330LHHX_MAG_SI_XZ_L                  0xCAU
#define ASM330LHHX_MAG_SI_XZ_H                  0xCBU
#define ASM330LHHX_MAG_SI_YY_L                  0xCCU
#define ASM330LHHX_MAG_SI_YY_H                  0xCDU
#define ASM330LHHX_MAG_SI_YZ_L                  0xCEU
#define ASM330LHHX_MAG_SI_YZ_H                  0xCFU
#define ASM330LHHX_MAG_SI_ZZ_L                  0xD0U
#define ASM330LHHX_MAG_SI_ZZ_H                  0xD1U
#define ASM330LHHX_MAG_CFG_A                    0xD4U
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
} asm330lhhx_mag_cfg_a_t;

#define ASM330LHHX_MAG_CFG_B                    0xD5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mag_x_axis               : 3;
  uint8_t not_used_01              : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 5;
  uint8_t mag_x_axis               : 3;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_mag_cfg_b_t;

#define ASM330LHHX_FSM_LC_TIMEOUT_L             0x17AU
#define ASM330LHHX_FSM_LC_TIMEOUT_H             0x17BU
#define ASM330LHHX_FSM_PROGRAMS                 0x17CU
#define ASM330LHHX_FSM_START_ADD_L              0x17EU
#define ASM330LHHX_FSM_START_ADD_H              0x17FU
#define ASM330LHHX_PEDO_DEB_STEPS_CONF          0x184U
#define ASM330LHHX_PEDO_SC_DELTAT_L             0x1D0U
#define ASM330LHHX_PEDO_SC_DELTAT_H             0x1D1U

#define ASM330LHHX_MLC_MAG_SENSITIVITY_L        0x1E8U
#define ASM330LHHX_MLC_MAG_SENSITIVITY_H        0x1E9U

#define ASM330LHHX_SENSOR_HUB_1                 0x02U
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
} asm330lhhx_sensor_hub_1_t;

#define ASM330LHHX_SENSOR_HUB_2                 0x03U
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
} asm330lhhx_sensor_hub_2_t;

#define ASM330LHHX_SENSOR_HUB_3                 0x04U
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
} asm330lhhx_sensor_hub_3_t;

#define ASM330LHHX_SENSOR_HUB_4                 0x05U
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
} asm330lhhx_sensor_hub_4_t;

#define ASM330LHHX_SENSOR_HUB_5                 0x06U
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
} asm330lhhx_sensor_hub_5_t;

#define ASM330LHHX_SENSOR_HUB_6                 0x07U
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
} asm330lhhx_sensor_hub_6_t;

#define ASM330LHHX_SENSOR_HUB_7                 0x08U
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
} asm330lhhx_sensor_hub_7_t;

#define ASM330LHHX_SENSOR_HUB_8                 0x09U
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
} asm330lhhx_sensor_hub_8_t;

#define ASM330LHHX_SENSOR_HUB_9                 0x0AU
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
} asm330lhhx_sensor_hub_9_t;

#define ASM330LHHX_SENSOR_HUB_10                0x0BU
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
} asm330lhhx_sensor_hub_10_t;

#define ASM330LHHX_SENSOR_HUB_11                0x0CU
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
} asm330lhhx_sensor_hub_11_t;

#define ASM330LHHX_SENSOR_HUB_12                0x0DU
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
} asm330lhhx_sensor_hub_12_t;

#define ASM330LHHX_SENSOR_HUB_13                0x0EU
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
} asm330lhhx_sensor_hub_13_t;

#define ASM330LHHX_SENSOR_HUB_14                0x0FU
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
} asm330lhhx_sensor_hub_14_t;

#define ASM330LHHX_SENSOR_HUB_15                0x10U
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
} asm330lhhx_sensor_hub_15_t;

#define ASM330LHHX_SENSOR_HUB_16                0x11U
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
} asm330lhhx_sensor_hub_16_t;

#define ASM330LHHX_SENSOR_HUB_17                0x12U
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
} asm330lhhx_sensor_hub_17_t;

#define ASM330LHHX_SENSOR_HUB_18                0x13U
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
} asm330lhhx_sensor_hub_18_t;

#define ASM330LHHX_MASTER_CONFIG                0x14U
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
} asm330lhhx_master_config_t;

#define ASM330LHHX_SLV0_ADD                     0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t rw_0                     : 1;
  uint8_t slave0                   : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave0                   : 7;
  uint8_t rw_0                     : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_slv0_add_t;

#define ASM330LHHX_SLV0_SUBADD                  0x16U
typedef struct
{
  uint8_t slave0_reg               : 8;
} asm330lhhx_slv0_subadd_t;

#define ASM330LHHX_SLV0_CONFIG                  0x17U
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
} asm330lhhx_slv0_config_t;

#define ASM330LHHX_SLV1_ADD                     0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_1                      : 1;
  uint8_t slave1_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave1_add               : 7;
  uint8_t r_1                      : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_slv1_add_t;

#define ASM330LHHX_SLV1_SUBADD                  0x19U
typedef struct
{
  uint8_t slave1_reg               : 8;
} asm330lhhx_slv1_subadd_t;

#define ASM330LHHX_SLV1_CONFIG                  0x1AU
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
} asm330lhhx_slv1_config_t;

#define ASM330LHHX_SLV2_ADD                     0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_2                      : 1;
  uint8_t slave2_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave2_add               : 7;
  uint8_t r_2                      : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_slv2_add_t;

#define ASM330LHHX_SLV2_SUBADD                  0x1CU
typedef struct
{
  uint8_t slave2_reg               : 8;
} asm330lhhx_slv2_subadd_t;

#define ASM330LHHX_SLV2_CONFIG                  0x1DU
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
} asm330lhhx_slv2_config_t;

#define ASM330LHHX_SLV3_ADD                     0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t r_3                      : 1;
  uint8_t slave3_add               : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t slave3_add               : 7;
  uint8_t r_3                      : 1;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_slv3_add_t;

#define ASM330LHHX_SLV3_SUBADD                  0x1FU
typedef struct
{
  uint8_t slave3_reg               : 8;
} asm330lhhx_slv3_subadd_t;

#define ASM330LHHX_SLV3_CONFIG                  0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t slave3_numop             : 3;
  uint8_t batch_ext_sens_3_en      : 1;
  uint8_t not_used_01              : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01              : 4;
  uint8_t batch_ext_sens_3_en      : 1;
  uint8_t slave3_numop             : 3;
#endif /* DRV_BYTE_ORDER */
} asm330lhhx_slv3_config_t;

#define ASM330LHHX_DATAWRITE_SLV0  0x21U
typedef struct
{
  uint8_t slave0_dataw             : 8;
} asm330lhhx_datawrite_slv0_t;

#define ASM330LHHX_STATUS_MASTER                0x22U
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
} asm330lhhx_status_master_t;

/**
  * @defgroup ASM330LHHX_Register_Union
  * @brief    This union group all the registers that has a bit-field
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
  asm330lhhx_func_cfg_access_t               func_cfg_access;
  asm330lhhx_pin_ctrl_t                      pin_ctrl;
  asm330lhhx_fifo_ctrl1_t                    fifo_ctrl1;
  asm330lhhx_fifo_ctrl2_t                    fifo_ctrl2;
  asm330lhhx_fifo_ctrl3_t                    fifo_ctrl3;
  asm330lhhx_fifo_ctrl4_t                    fifo_ctrl4;
  asm330lhhx_counter_bdr_reg1_t              counter_bdr_reg1;
  asm330lhhx_counter_bdr_reg2_t              counter_bdr_reg2;
  asm330lhhx_int1_ctrl_t                     int1_ctrl;
  asm330lhhx_int2_ctrl_t                     int2_ctrl;
  asm330lhhx_ctrl1_xl_t                      ctrl1_xl;
  asm330lhhx_ctrl2_g_t                       ctrl2_g;
  asm330lhhx_ctrl3_c_t                       ctrl3_c;
  asm330lhhx_ctrl4_c_t                       ctrl4_c;
  asm330lhhx_ctrl5_c_t                       ctrl5_c;
  asm330lhhx_ctrl6_c_t                       ctrl6_c;
  asm330lhhx_ctrl7_g_t                       ctrl7_g;
  asm330lhhx_ctrl8_xl_t                      ctrl8_xl;
  asm330lhhx_ctrl9_xl_t                      ctrl9_xl;
  asm330lhhx_ctrl10_c_t                      ctrl10_c;
  asm330lhhx_all_int_src_t                   all_int_src;
  asm330lhhx_wake_up_src_t                   wake_up_src;
  asm330lhhx_d6d_src_t                       d6d_src;
  asm330lhhx_status_reg_t                    status_reg;
  asm330lhhx_fifo_status1_t                  fifo_status1;
  asm330lhhx_fifo_status2_t                  fifo_status2;
  asm330lhhx_int_cfg0_t                      int_cfg0;
  asm330lhhx_int_cfg1_t                      int_cfg1;
  asm330lhhx_ths_6d_t                        ths_6d;
  asm330lhhx_wake_up_ths_t                   wake_up_ths;
  asm330lhhx_wake_up_dur_t                   wake_up_dur;
  asm330lhhx_free_fall_t                     free_fall;
  asm330lhhx_md1_cfg_t                       md1_cfg;
  asm330lhhx_md2_cfg_t                       md2_cfg;
  asm330lhhx_i3c_bus_avb_t                   i3c_bus_avb;
  asm330lhhx_internal_freq_fine_t            internal_freq_fine;
  asm330lhhx_fifo_data_out_tag_t             fifo_data_out_tag;
  asm330lhhx_page_sel_t                      page_sel;
  asm330lhhx_emb_func_en_b_t                 emb_func_en_b;
  asm330lhhx_page_address_t                  page_address;
  asm330lhhx_page_value_t                    page_value;
  asm330lhhx_emb_func_int1_t                 emb_func_int1;
  asm330lhhx_fsm_int1_a_t                    fsm_int1_a;
  asm330lhhx_fsm_int1_b_t                    fsm_int1_b;
  asm330lhhx_mlc_int1_t                      mlc_int1;
  asm330lhhx_emb_func_int2_t                 emb_func_int2;
  asm330lhhx_fsm_int2_a_t                    fsm_int2_a;
  asm330lhhx_fsm_int2_b_t                    fsm_int2_b;
  asm330lhhx_mlc_int2_t                      mlc_int2;
  asm330lhhx_emb_func_status_t               emb_func_status;
  asm330lhhx_fsm_status_a_t                  fsm_status_a;
  asm330lhhx_fsm_status_b_t                  fsm_status_b;
  asm330lhhx_mlc_status_mainpage_t           mlc_status_mainpage;
  asm330lhhx_emb_func_odr_cfg_c_t            emb_func_odr_cfg_c;
  asm330lhhx_page_rw_t                       page_rw;
  asm330lhhx_fsm_enable_a_t                  fsm_enable_a;
  asm330lhhx_fsm_enable_b_t                  fsm_enable_b;
  asm330lhhx_fsm_long_counter_clear_t        fsm_long_counter_clear;
  asm330lhhx_fsm_outs1_t                     fsm_outs1;
  asm330lhhx_fsm_outs2_t                     fsm_outs2;
  asm330lhhx_fsm_outs3_t                     fsm_outs3;
  asm330lhhx_fsm_outs4_t                     fsm_outs4;
  asm330lhhx_fsm_outs5_t                     fsm_outs5;
  asm330lhhx_fsm_outs6_t                     fsm_outs6;
  asm330lhhx_fsm_outs7_t                     fsm_outs7;
  asm330lhhx_fsm_outs8_t                     fsm_outs8;
  asm330lhhx_fsm_outs9_t                     fsm_outs9;
  asm330lhhx_fsm_outs10_t                    fsm_outs10;
  asm330lhhx_fsm_outs11_t                    fsm_outs11;
  asm330lhhx_fsm_outs12_t                    fsm_outs12;
  asm330lhhx_fsm_outs13_t                    fsm_outs13;
  asm330lhhx_fsm_outs14_t                    fsm_outs14;
  asm330lhhx_fsm_outs15_t                    fsm_outs15;
  asm330lhhx_fsm_outs16_t                    fsm_outs16;
  asm330lhhx_emb_func_odr_cfg_b_t            emb_func_odr_cfg_b;
  asm330lhhx_emb_func_init_b_t               emb_func_init_b;
  asm330lhhx_mag_cfg_a_t                     mag_cfg_a;
  asm330lhhx_mag_cfg_b_t                     mag_cfg_b;
  asm330lhhx_sensor_hub_1_t                  sensor_hub_1;
  asm330lhhx_sensor_hub_2_t                  sensor_hub_2;
  asm330lhhx_sensor_hub_3_t                  sensor_hub_3;
  asm330lhhx_sensor_hub_4_t                  sensor_hub_4;
  asm330lhhx_sensor_hub_5_t                  sensor_hub_5;
  asm330lhhx_sensor_hub_6_t                  sensor_hub_6;
  asm330lhhx_sensor_hub_7_t                  sensor_hub_7;
  asm330lhhx_sensor_hub_8_t                  sensor_hub_8;
  asm330lhhx_sensor_hub_9_t                  sensor_hub_9;
  asm330lhhx_sensor_hub_10_t                 sensor_hub_10;
  asm330lhhx_sensor_hub_11_t                 sensor_hub_11;
  asm330lhhx_sensor_hub_12_t                 sensor_hub_12;
  asm330lhhx_sensor_hub_13_t                 sensor_hub_13;
  asm330lhhx_sensor_hub_14_t                 sensor_hub_14;
  asm330lhhx_sensor_hub_15_t                 sensor_hub_15;
  asm330lhhx_sensor_hub_16_t                 sensor_hub_16;
  asm330lhhx_sensor_hub_17_t                 sensor_hub_17;
  asm330lhhx_sensor_hub_18_t                 sensor_hub_18;
  asm330lhhx_master_config_t                 master_config;
  asm330lhhx_slv0_add_t                      slv0_add;
  asm330lhhx_slv0_subadd_t                   slv0_subadd;
  asm330lhhx_slv0_config_t                   slv0_config;
  asm330lhhx_slv1_add_t                      slv1_add;
  asm330lhhx_slv1_subadd_t                   slv1_subadd;
  asm330lhhx_slv1_config_t                   slv1_config;
  asm330lhhx_slv2_add_t                      slv2_add;
  asm330lhhx_slv2_subadd_t                   slv2_subadd;
  asm330lhhx_slv2_config_t                   slv2_config;
  asm330lhhx_slv3_add_t                      slv3_add;
  asm330lhhx_slv3_subadd_t                   slv3_subadd;
  asm330lhhx_slv3_config_t                   slv3_config;
  asm330lhhx_datawrite_slv0_t                datawrite_slv0;
  asm330lhhx_status_master_t                 status_master;
  bitwise_t                                  bitwise;
  uint8_t                                    byte;
} asm330lhhx_reg_t;

/**
  * @}
  *
  */

int32_t asm330lhhx_read_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                            uint16_t len);
int32_t asm330lhhx_write_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                             uint16_t len);

float_t asm330lhhx_from_fs2g_to_mg(int16_t lsb);
float_t asm330lhhx_from_fs4g_to_mg(int16_t lsb);
float_t asm330lhhx_from_fs8g_to_mg(int16_t lsb);
float_t asm330lhhx_from_fs16g_to_mg(int16_t lsb);
float_t asm330lhhx_from_fs125dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_fs250dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_fs500dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_fs1000dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_fs2000dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_fs4000dps_to_mdps(int16_t lsb);
float_t asm330lhhx_from_lsb_to_celsius(int16_t lsb);
float_t asm330lhhx_from_lsb_to_nsec(int32_t lsb);

typedef enum
{
  ASM330LHHX_2g   = 0,
  ASM330LHHX_16g  = 1, /* if XL_FS_MODE = '1' -> ASM330LHHX_2g */
  ASM330LHHX_4g   = 2,
  ASM330LHHX_8g   = 3,
} asm330lhhx_fs_xl_t;
int32_t asm330lhhx_xl_full_scale_set(stmdev_ctx_t *ctx, asm330lhhx_fs_xl_t val);
int32_t asm330lhhx_xl_full_scale_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_fs_xl_t *val);

typedef enum
{
  ASM330LHHX_XL_ODR_OFF    = 0,
  ASM330LHHX_XL_ODR_12Hz5  = 1,
  ASM330LHHX_XL_ODR_26Hz   = 2,
  ASM330LHHX_XL_ODR_52Hz   = 3,
  ASM330LHHX_XL_ODR_104Hz  = 4,
  ASM330LHHX_XL_ODR_208Hz  = 5,
  ASM330LHHX_XL_ODR_417Hz  = 6,
  ASM330LHHX_XL_ODR_833Hz  = 7,
  ASM330LHHX_XL_ODR_1667Hz = 8,
  ASM330LHHX_XL_ODR_3333Hz = 9,
  ASM330LHHX_XL_ODR_6667Hz = 10,
  ASM330LHHX_XL_ODR_6Hz5   = 11, /* (low power only) */
} asm330lhhx_odr_xl_t;
int32_t asm330lhhx_xl_data_rate_set(stmdev_ctx_t *ctx, asm330lhhx_odr_xl_t val);
int32_t asm330lhhx_xl_data_rate_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_odr_xl_t *val);

typedef enum
{
  ASM330LHHX_125dps = 2,
  ASM330LHHX_250dps = 0,
  ASM330LHHX_500dps = 4,
  ASM330LHHX_1000dps = 8,
  ASM330LHHX_2000dps = 12,
  ASM330LHHX_4000dps = 1,
} asm330lhhx_fs_g_t;
int32_t asm330lhhx_gy_full_scale_set(stmdev_ctx_t *ctx, asm330lhhx_fs_g_t val);
int32_t asm330lhhx_gy_full_scale_get(stmdev_ctx_t *ctx, asm330lhhx_fs_g_t *val);

typedef enum
{
  ASM330LHHX_GY_ODR_OFF    = 0,
  ASM330LHHX_GY_ODR_12Hz5  = 1,
  ASM330LHHX_GY_ODR_26Hz   = 2,
  ASM330LHHX_GY_ODR_52Hz   = 3,
  ASM330LHHX_GY_ODR_104Hz  = 4,
  ASM330LHHX_GY_ODR_208Hz  = 5,
  ASM330LHHX_GY_ODR_417Hz  = 6,
  ASM330LHHX_GY_ODR_833Hz  = 7,
  ASM330LHHX_GY_ODR_1667Hz = 8,
  ASM330LHHX_GY_ODR_3333Hz = 9,
  ASM330LHHX_GY_ODR_6667Hz = 10,
} asm330lhhx_odr_g_t;
int32_t asm330lhhx_gy_data_rate_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_odr_g_t val);
int32_t asm330lhhx_gy_data_rate_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_odr_g_t *val);

int32_t asm330lhhx_block_data_update_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_block_data_update_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_LSb_1mg  = 0,
  ASM330LHHX_LSb_16mg = 1,
} asm330lhhx_usr_off_w_t;
int32_t asm330lhhx_xl_offset_weight_set(stmdev_ctx_t *ctx,
                                        asm330lhhx_usr_off_w_t val);
int32_t asm330lhhx_xl_offset_weight_get(stmdev_ctx_t *ctx,
                                        asm330lhhx_usr_off_w_t *val);

typedef enum
{
  ASM330LHHX_HIGH_PERFORMANCE_MD  = 0,
  ASM330LHHX_LOW_NORMAL_POWER_MD  = 1,
} asm330lhhx_xl_hm_mode_t;
int32_t asm330lhhx_xl_power_mode_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_xl_hm_mode_t val);
int32_t asm330lhhx_xl_power_mode_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_xl_hm_mode_t *val);

typedef enum
{
  ASM330LHHX_GY_HIGH_PERFORMANCE  = 0,
  ASM330LHHX_GY_NORMAL            = 1,
} asm330lhhx_g_hm_mode_t;
int32_t asm330lhhx_gy_power_mode_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_g_hm_mode_t val);
int32_t asm330lhhx_gy_power_mode_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_g_hm_mode_t *val);

typedef struct
{
  asm330lhhx_all_int_src_t           all_int_src;
  asm330lhhx_wake_up_src_t           wake_up_src;
  asm330lhhx_d6d_src_t               d6d_src;
  asm330lhhx_status_reg_t            status_reg;
  asm330lhhx_emb_func_status_t       emb_func_status;
  asm330lhhx_fsm_status_a_t          fsm_status_a;
  asm330lhhx_fsm_status_b_t          fsm_status_b;
  asm330lhhx_mlc_status_mainpage_t   mlc_status;
} asm330lhhx_all_sources_t;
int32_t asm330lhhx_all_sources_get(stmdev_ctx_t *ctx,
                                   asm330lhhx_all_sources_t *val);

int32_t asm330lhhx_status_reg_get(stmdev_ctx_t *ctx,
                                  asm330lhhx_status_reg_t *val);

int32_t asm330lhhx_xl_flag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_gy_flag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_temp_flag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_xl_usr_offset_x_set(stmdev_ctx_t *ctx, uint8_t *buff);
int32_t asm330lhhx_xl_usr_offset_x_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t asm330lhhx_xl_usr_offset_y_set(stmdev_ctx_t *ctx, uint8_t *buff);
int32_t asm330lhhx_xl_usr_offset_y_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t asm330lhhx_xl_usr_offset_z_set(stmdev_ctx_t *ctx, uint8_t *buff);
int32_t asm330lhhx_xl_usr_offset_z_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t asm330lhhx_xl_usr_offset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_xl_usr_offset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_timestamp_rst(stmdev_ctx_t *ctx);

int32_t asm330lhhx_timestamp_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_timestamp_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_timestamp_raw_get(stmdev_ctx_t *ctx, uint32_t *val);

typedef enum
{
  ASM330LHHX_NO_ROUND      = 0,
  ASM330LHHX_ROUND_XL      = 1,
  ASM330LHHX_ROUND_GY      = 2,
  ASM330LHHX_ROUND_GY_XL   = 3,
} asm330lhhx_rounding_t;
int32_t asm330lhhx_rounding_mode_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_rounding_t val);
int32_t asm330lhhx_rounding_mode_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_rounding_t *val);

int32_t asm330lhhx_temperature_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t asm330lhhx_angular_rate_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t asm330lhhx_acceleration_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t asm330lhhx_fifo_out_raw_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_odr_cal_reg_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_odr_cal_reg_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_USER_BANK           = 0,
  ASM330LHHX_SENSOR_HUB_BANK     = 1,
  ASM330LHHX_EMBEDDED_FUNC_BANK  = 2,
} asm330lhhx_reg_access_t;
int32_t asm330lhhx_mem_bank_set(stmdev_ctx_t *ctx, asm330lhhx_reg_access_t val);
int32_t asm330lhhx_mem_bank_get(stmdev_ctx_t *ctx,
                                asm330lhhx_reg_access_t *val);

int32_t asm330lhhx_ln_pg_write_byte(stmdev_ctx_t *ctx, uint16_t address,
                                    uint8_t *val);
int32_t asm330lhhx_ln_pg_write(stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len);
int32_t asm330lhhx_ln_pg_read_byte(stmdev_ctx_t *ctx, uint16_t add,
                                   uint8_t *val);
int32_t asm330lhhx_ln_pg_read(stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *val);

typedef enum
{
  ASM330LHHX_DRDY_LATCHED = 0,
  ASM330LHHX_DRDY_PULSED  = 1,
} asm330lhhx_dataready_pulsed_t;
int32_t asm330lhhx_data_ready_mode_set(stmdev_ctx_t *ctx,
                                       asm330lhhx_dataready_pulsed_t val);
int32_t asm330lhhx_data_ready_mode_get(stmdev_ctx_t *ctx,
                                       asm330lhhx_dataready_pulsed_t *val);

int32_t asm330lhhx_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t asm330lhhx_reset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_auto_increment_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_auto_increment_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_boot_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_boot_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_XL_ST_DISABLE  = 0,
  ASM330LHHX_XL_ST_POSITIVE = 1,
  ASM330LHHX_XL_ST_NEGATIVE = 2,
} asm330lhhx_st_xl_t;
int32_t asm330lhhx_xl_self_test_set(stmdev_ctx_t *ctx, asm330lhhx_st_xl_t val);
int32_t asm330lhhx_xl_self_test_get(stmdev_ctx_t *ctx, asm330lhhx_st_xl_t *val);

typedef enum
{
  ASM330LHHX_GY_ST_DISABLE  = 0,
  ASM330LHHX_GY_ST_POSITIVE = 1,
  ASM330LHHX_GY_ST_NEGATIVE = 3,
} asm330lhhx_st_g_t;
int32_t asm330lhhx_gy_self_test_set(stmdev_ctx_t *ctx, asm330lhhx_st_g_t val);
int32_t asm330lhhx_gy_self_test_get(stmdev_ctx_t *ctx, asm330lhhx_st_g_t *val);

int32_t asm330lhhx_xl_filter_lp2_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_xl_filter_lp2_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_gy_filter_lp1_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_gy_filter_lp1_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_filter_settling_mask_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_filter_settling_mask_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_ULTRA_LIGHT  = 0,
  ASM330LHHX_VERY_LIGHT   = 1,
  ASM330LHHX_LIGHT        = 2,
  ASM330LHHX_MEDIUM       = 3,
  ASM330LHHX_STRONG       = 4,
  ASM330LHHX_VERY_STRONG  = 5,
  ASM330LHHX_AGGRESSIVE   = 6,
  ASM330LHHX_XTREME       = 7,
} asm330lhhx_ftype_t;
int32_t asm330lhhx_gy_lp1_bandwidth_set(stmdev_ctx_t *ctx,
                                        asm330lhhx_ftype_t val);
int32_t asm330lhhx_gy_lp1_bandwidth_get(stmdev_ctx_t *ctx,
                                        asm330lhhx_ftype_t *val);

int32_t asm330lhhx_xl_lp2_on_6d_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_xl_lp2_on_6d_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_HP_PATH_DISABLE_ON_OUT    = 0x00,
  ASM330LHHX_SLOPE_ODR_DIV_4           = 0x10,
  ASM330LHHX_HP_ODR_DIV_10             = 0x11,
  ASM330LHHX_HP_ODR_DIV_20             = 0x12,
  ASM330LHHX_HP_ODR_DIV_45             = 0x13,
  ASM330LHHX_HP_ODR_DIV_100            = 0x14,
  ASM330LHHX_HP_ODR_DIV_200            = 0x15,
  ASM330LHHX_HP_ODR_DIV_400            = 0x16,
  ASM330LHHX_HP_ODR_DIV_800            = 0x17,
  ASM330LHHX_HP_REF_MD_ODR_DIV_10      = 0x31,
  ASM330LHHX_HP_REF_MD_ODR_DIV_20      = 0x32,
  ASM330LHHX_HP_REF_MD_ODR_DIV_45      = 0x33,
  ASM330LHHX_HP_REF_MD_ODR_DIV_100     = 0x34,
  ASM330LHHX_HP_REF_MD_ODR_DIV_200     = 0x35,
  ASM330LHHX_HP_REF_MD_ODR_DIV_400     = 0x36,
  ASM330LHHX_HP_REF_MD_ODR_DIV_800     = 0x37,
  ASM330LHHX_LP_ODR_DIV_10             = 0x01,
  ASM330LHHX_LP_ODR_DIV_20             = 0x02,
  ASM330LHHX_LP_ODR_DIV_45             = 0x03,
  ASM330LHHX_LP_ODR_DIV_100            = 0x04,
  ASM330LHHX_LP_ODR_DIV_200            = 0x05,
  ASM330LHHX_LP_ODR_DIV_400            = 0x06,
  ASM330LHHX_LP_ODR_DIV_800            = 0x07,
} asm330lhhx_hp_slope_xl_en_t;
int32_t asm330lhhx_xl_hp_path_on_out_set(stmdev_ctx_t *ctx,
                                         asm330lhhx_hp_slope_xl_en_t val);
int32_t asm330lhhx_xl_hp_path_on_out_get(stmdev_ctx_t *ctx,
                                         asm330lhhx_hp_slope_xl_en_t *val);

int32_t asm330lhhx_xl_fast_settling_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_xl_fast_settling_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_USE_SLOPE = 0,
  ASM330LHHX_USE_HPF   = 1,
} asm330lhhx_slope_fds_t;
int32_t asm330lhhx_xl_hp_path_internal_set(stmdev_ctx_t *ctx,
                                           asm330lhhx_slope_fds_t val);
int32_t asm330lhhx_xl_hp_path_internal_get(stmdev_ctx_t *ctx,
                                           asm330lhhx_slope_fds_t *val);

typedef enum
{
  ASM330LHHX_HP_FILTER_NONE     = 0x00,
  ASM330LHHX_HP_FILTER_16mHz    = 0x80,
  ASM330LHHX_HP_FILTER_65mHz    = 0x81,
  ASM330LHHX_HP_FILTER_260mHz   = 0x82,
  ASM330LHHX_HP_FILTER_1Hz04    = 0x83,
} asm330lhhx_hpm_g_t;
int32_t asm330lhhx_gy_hp_path_internal_set(stmdev_ctx_t *ctx,
                                           asm330lhhx_hpm_g_t val);
int32_t asm330lhhx_gy_hp_path_internal_get(stmdev_ctx_t *ctx,
                                           asm330lhhx_hpm_g_t *val);

typedef enum
{
  ASM330LHHX_PULL_UP_DISC       = 0,
  ASM330LHHX_PULL_UP_CONNECT    = 1,
} asm330lhhx_sdo_pu_en_t;
int32_t asm330lhhx_sdo_sa0_mode_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_sdo_pu_en_t val);
int32_t asm330lhhx_sdo_sa0_mode_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_sdo_pu_en_t *val);

typedef enum
{
  ASM330LHHX_PULL_DOWN_CONNECT       = 0,
  ASM330LHHX_PULL_DOWN_DISC          = 1,
} asm330lhhx_pd_dis_int1_t;
int32_t asm330lhhx_int1_mode_set(stmdev_ctx_t *ctx,
                                 asm330lhhx_pd_dis_int1_t val);
int32_t asm330lhhx_int1_mode_get(stmdev_ctx_t *ctx,
                                 asm330lhhx_pd_dis_int1_t *val);

typedef enum
{
  ASM330LHHX_SPI_4_WIRE = 0,
  ASM330LHHX_SPI_3_WIRE = 1,
} asm330lhhx_sim_t;
int32_t asm330lhhx_spi_mode_set(stmdev_ctx_t *ctx, asm330lhhx_sim_t val);
int32_t asm330lhhx_spi_mode_get(stmdev_ctx_t *ctx, asm330lhhx_sim_t *val);

typedef enum
{
  ASM330LHHX_I2C_ENABLE  = 0,
  ASM330LHHX_I2C_DISABLE = 1,
} asm330lhhx_i2c_disable_t;
int32_t asm330lhhx_i2c_interface_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_i2c_disable_t val);
int32_t asm330lhhx_i2c_interface_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_i2c_disable_t *val);

typedef enum
{
  ASM330LHHX_I3C_DISABLE         = 0x80,
  ASM330LHHX_I3C_ENABLE_T_50us   = 0x00,
  ASM330LHHX_I3C_ENABLE_T_2us    = 0x01,
  ASM330LHHX_I3C_ENABLE_T_1ms    = 0x02,
  ASM330LHHX_I3C_ENABLE_T_25ms   = 0x03,
} asm330lhhx_i3c_disable_t;
int32_t asm330lhhx_i3c_disable_set(stmdev_ctx_t *ctx,
                                   asm330lhhx_i3c_disable_t val);
int32_t asm330lhhx_i3c_disable_get(stmdev_ctx_t *ctx,
                                   asm330lhhx_i3c_disable_t *val);

typedef struct
{
  asm330lhhx_int1_ctrl_t          int1_ctrl;
  asm330lhhx_md1_cfg_t            md1_cfg;
  asm330lhhx_emb_func_int1_t      emb_func_int1;
  asm330lhhx_fsm_int1_a_t         fsm_int1_a;
  asm330lhhx_fsm_int1_b_t         fsm_int1_b;
  asm330lhhx_mlc_int1_t           mlc_int1;
} asm330lhhx_pin_int1_route_t;
int32_t asm330lhhx_pin_int1_route_set(stmdev_ctx_t *ctx,
                                      asm330lhhx_pin_int1_route_t *val);
int32_t asm330lhhx_pin_int1_route_get(stmdev_ctx_t *ctx,
                                      asm330lhhx_pin_int1_route_t *val);

typedef struct
{
  asm330lhhx_int2_ctrl_t          int2_ctrl;
  asm330lhhx_md2_cfg_t            md2_cfg;
  asm330lhhx_emb_func_int2_t      emb_func_int2;
  asm330lhhx_fsm_int2_a_t         fsm_int2_a;
  asm330lhhx_fsm_int2_b_t         fsm_int2_b;
  asm330lhhx_mlc_int2_t           mlc_int2;
} asm330lhhx_pin_int2_route_t;
int32_t asm330lhhx_pin_int2_route_set(stmdev_ctx_t *ctx,
                                      asm330lhhx_pin_int2_route_t *val);
int32_t asm330lhhx_pin_int2_route_get(stmdev_ctx_t *ctx,
                                      asm330lhhx_pin_int2_route_t *val);

typedef enum
{
  ASM330LHHX_PUSH_PULL   = 0,
  ASM330LHHX_OPEN_DRAIN  = 1,
} asm330lhhx_pp_od_t;
int32_t asm330lhhx_pin_mode_set(stmdev_ctx_t *ctx, asm330lhhx_pp_od_t val);
int32_t asm330lhhx_pin_mode_get(stmdev_ctx_t *ctx, asm330lhhx_pp_od_t *val);

typedef enum
{
  ASM330LHHX_ACTIVE_HIGH = 0,
  ASM330LHHX_ACTIVE_LOW  = 1,
} asm330lhhx_h_lactive_t;
int32_t asm330lhhx_pin_polarity_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_h_lactive_t val);
int32_t asm330lhhx_pin_polarity_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_h_lactive_t *val);

int32_t asm330lhhx_all_on_int1_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_all_on_int1_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_ALL_INT_PULSED            = 0,
  ASM330LHHX_BASE_LATCHED_EMB_PULSED   = 1,
  ASM330LHHX_BASE_PULSED_EMB_LATCHED   = 2,
  ASM330LHHX_ALL_INT_LATCHED           = 3,
} asm330lhhx_lir_t;
int32_t asm330lhhx_int_notification_set(stmdev_ctx_t *ctx,
                                        asm330lhhx_lir_t val);
int32_t asm330lhhx_int_notification_get(stmdev_ctx_t *ctx,
                                        asm330lhhx_lir_t *val);

typedef enum
{
  ASM330LHHX_LSb_FS_DIV_64       = 0,
  ASM330LHHX_LSb_FS_DIV_256      = 1,
} asm330lhhx_wake_ths_w_t;
int32_t asm330lhhx_wkup_ths_weight_set(stmdev_ctx_t *ctx,
                                       asm330lhhx_wake_ths_w_t val);
int32_t asm330lhhx_wkup_ths_weight_get(stmdev_ctx_t *ctx,
                                       asm330lhhx_wake_ths_w_t *val);

int32_t asm330lhhx_wkup_threshold_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_wkup_threshold_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_xl_usr_offset_on_wkup_set(stmdev_ctx_t *ctx,
                                             uint8_t val);
int32_t asm330lhhx_xl_usr_offset_on_wkup_get(stmdev_ctx_t *ctx,
                                             uint8_t *val);

int32_t asm330lhhx_wkup_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_wkup_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_gy_sleep_mode_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_gy_sleep_mode_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_DRIVE_SLEEP_CHG_EVENT = 0,
  ASM330LHHX_DRIVE_SLEEP_STATUS    = 1,
} asm330lhhx_sleep_status_on_int_t;
int32_t asm330lhhx_act_pin_notification_set(stmdev_ctx_t *ctx,
                                            asm330lhhx_sleep_status_on_int_t val);
int32_t asm330lhhx_act_pin_notification_get(stmdev_ctx_t *ctx,
                                            asm330lhhx_sleep_status_on_int_t *val);

typedef enum
{
  ASM330LHHX_XL_AND_GY_NOT_AFFECTED      = 0,
  ASM330LHHX_XL_12Hz5_GY_NOT_AFFECTED    = 1,
  ASM330LHHX_XL_12Hz5_GY_SLEEP           = 2,
  ASM330LHHX_XL_12Hz5_GY_PD              = 3,
} asm330lhhx_inact_en_t;
int32_t asm330lhhx_act_mode_set(stmdev_ctx_t *ctx,
                                asm330lhhx_inact_en_t val);
int32_t asm330lhhx_act_mode_get(stmdev_ctx_t *ctx,
                                asm330lhhx_inact_en_t *val);

int32_t asm330lhhx_act_sleep_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_act_sleep_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_DEG_80  = 0,
  ASM330LHHX_DEG_70  = 1,
  ASM330LHHX_DEG_60  = 2,
  ASM330LHHX_DEG_50  = 3,
} asm330lhhx_sixd_ths_t;
int32_t asm330lhhx_6d_threshold_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_sixd_ths_t val);
int32_t asm330lhhx_6d_threshold_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_sixd_ths_t *val);

int32_t asm330lhhx_4d_mode_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_4d_mode_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_FF_TSH_156mg = 0,
  ASM330LHHX_FF_TSH_219mg = 1,
  ASM330LHHX_FF_TSH_250mg = 2,
  ASM330LHHX_FF_TSH_312mg = 3,
  ASM330LHHX_FF_TSH_344mg = 4,
  ASM330LHHX_FF_TSH_406mg = 5,
  ASM330LHHX_FF_TSH_469mg = 6,
  ASM330LHHX_FF_TSH_500mg = 7,
} asm330lhhx_ff_ths_t;
int32_t asm330lhhx_ff_threshold_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_ff_ths_t val);
int32_t asm330lhhx_ff_threshold_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_ff_ths_t *val);

int32_t asm330lhhx_ff_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_ff_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_fifo_watermark_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_fifo_watermark_get(stmdev_ctx_t *ctx, uint16_t *val);

int32_t asm330lhhx_fifo_virtual_sens_odr_chg_set(stmdev_ctx_t *ctx,
                                                 uint8_t val);
int32_t asm330lhhx_fifo_virtual_sens_odr_chg_get(stmdev_ctx_t *ctx,
                                                 uint8_t *val);

int32_t asm330lhhx_fifo_stop_on_wtm_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_fifo_stop_on_wtm_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_XL_NOT_BATCHED       =  0,
  ASM330LHHX_XL_BATCHED_AT_12Hz5   =  1,
  ASM330LHHX_XL_BATCHED_AT_26Hz    =  2,
  ASM330LHHX_XL_BATCHED_AT_52Hz    =  3,
  ASM330LHHX_XL_BATCHED_AT_104Hz   =  4,
  ASM330LHHX_XL_BATCHED_AT_208Hz   =  5,
  ASM330LHHX_XL_BATCHED_AT_417Hz   =  6,
  ASM330LHHX_XL_BATCHED_AT_833Hz   =  7,
  ASM330LHHX_XL_BATCHED_AT_1667Hz  =  8,
  ASM330LHHX_XL_BATCHED_AT_3333Hz  =  9,
  ASM330LHHX_XL_BATCHED_AT_6667Hz  = 10,
  ASM330LHHX_XL_BATCHED_AT_6Hz5    = 11,
} asm330lhhx_bdr_xl_t;
int32_t asm330lhhx_fifo_xl_batch_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_bdr_xl_t val);
int32_t asm330lhhx_fifo_xl_batch_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_bdr_xl_t *val);

typedef enum
{
  ASM330LHHX_GY_NOT_BATCHED         = 0,
  ASM330LHHX_GY_BATCHED_AT_12Hz5    = 1,
  ASM330LHHX_GY_BATCHED_AT_26Hz     = 2,
  ASM330LHHX_GY_BATCHED_AT_52Hz     = 3,
  ASM330LHHX_GY_BATCHED_AT_104Hz    = 4,
  ASM330LHHX_GY_BATCHED_AT_208Hz    = 5,
  ASM330LHHX_GY_BATCHED_AT_417Hz    = 6,
  ASM330LHHX_GY_BATCHED_AT_833Hz    = 7,
  ASM330LHHX_GY_BATCHED_AT_1667Hz   = 8,
  ASM330LHHX_GY_BATCHED_AT_3333Hz   = 9,
  ASM330LHHX_GY_BATCHED_AT_6667Hz   = 10,
  ASM330LHHX_GY_BATCHED_6Hz5        = 11,
} asm330lhhx_bdr_gy_t;
int32_t asm330lhhx_fifo_gy_batch_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_bdr_gy_t val);
int32_t asm330lhhx_fifo_gy_batch_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_bdr_gy_t *val);

typedef enum
{
  ASM330LHHX_BYPASS_MODE             = 0,
  ASM330LHHX_FIFO_MODE               = 1,
  ASM330LHHX_STREAM_TO_FIFO_MODE     = 3,
  ASM330LHHX_BYPASS_TO_STREAM_MODE   = 4,
  ASM330LHHX_STREAM_MODE             = 6,
  ASM330LHHX_BYPASS_TO_FIFO_MODE     = 7,
} asm330lhhx_fifo_mode_t;
int32_t asm330lhhx_fifo_mode_set(stmdev_ctx_t *ctx, asm330lhhx_fifo_mode_t val);
int32_t asm330lhhx_fifo_mode_get(stmdev_ctx_t *ctx,
                                 asm330lhhx_fifo_mode_t *val);

typedef enum
{
  ASM330LHHX_TEMP_NOT_BATCHED        = 0,
  ASM330LHHX_TEMP_BATCHED_AT_52Hz    = 1,
  ASM330LHHX_TEMP_BATCHED_AT_12Hz5   = 2,
  ASM330LHHX_TEMP_BATCHED_AT_1Hz6    = 3,
} asm330lhhx_odr_t_batch_t;
int32_t asm330lhhx_fifo_temp_batch_set(stmdev_ctx_t *ctx,
                                       asm330lhhx_odr_t_batch_t val);
int32_t asm330lhhx_fifo_temp_batch_get(stmdev_ctx_t *ctx,
                                       asm330lhhx_odr_t_batch_t *val);

typedef enum
{
  ASM330LHHX_NO_DECIMATION = 0,
  ASM330LHHX_DEC_1         = 1,
  ASM330LHHX_DEC_8         = 2,
  ASM330LHHX_DEC_32        = 3,
} asm330lhhx_dec_ts_batch_t;
int32_t asm330lhhx_fifo_timestamp_decimation_set(stmdev_ctx_t *ctx,
                                                 asm330lhhx_dec_ts_batch_t val);
int32_t asm330lhhx_fifo_timestamp_decimation_get(stmdev_ctx_t *ctx,
                                                 asm330lhhx_dec_ts_batch_t *val);

typedef enum
{
  ASM330LHHX_XL_BATCH_EVENT   = 0,
  ASM330LHHX_GYRO_BATCH_EVENT = 1,
} asm330lhhx_trig_counter_bdr_t;
int32_t asm330lhhx_fifo_cnt_event_batch_set(stmdev_ctx_t *ctx,
                                            asm330lhhx_trig_counter_bdr_t val);
int32_t asm330lhhx_fifo_cnt_event_batch_get(stmdev_ctx_t *ctx,
                                            asm330lhhx_trig_counter_bdr_t *val);

int32_t asm330lhhx_rst_batch_counter_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_rst_batch_counter_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_batch_counter_threshold_set(stmdev_ctx_t *ctx,
                                               uint16_t val);
int32_t asm330lhhx_batch_counter_threshold_get(stmdev_ctx_t *ctx,
                                               uint16_t *val);

int32_t asm330lhhx_fifo_data_level_get(stmdev_ctx_t *ctx, uint16_t *val);

int32_t asm330lhhx_fifo_status_get(stmdev_ctx_t *ctx,
                                   asm330lhhx_fifo_status2_t *val);

int32_t asm330lhhx_fifo_full_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_fifo_ovr_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_fifo_wtm_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_GYRO_NC_TAG          = 0x01,
  ASM330LHHX_XL_NC_TAG            = 0x02,
  ASM330LHHX_TEMPERATURE_TAG      = 0x03,
  ASM330LHHX_TIMESTAMP_TAG        = 0x04,
  ASM330LHHX_CFG_CHANGE_TAG       = 0x05,
  ASM330LHHX_SENSORHUB_SLAVE0_TAG = 0x0E,
  ASM330LHHX_SENSORHUB_SLAVE1_TAG = 0x0F,
  ASM330LHHX_SENSORHUB_SLAVE2_TAG = 0x10,
  ASM330LHHX_SENSORHUB_SLAVE3_TAG = 0x11,
  ASM330LHHX_SENSORHUB_NACK_TAG   = 0x19,
} asm330lhhx_fifo_tag_t;
int32_t asm330lhhx_fifo_sensor_tag_get(stmdev_ctx_t *ctx,
                                       asm330lhhx_fifo_tag_t *val);

int32_t asm330lhhx_sh_batch_slave_0_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_batch_slave_0_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_sh_batch_slave_1_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_batch_slave_1_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_sh_batch_slave_2_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_batch_slave_2_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_sh_batch_slave_3_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_batch_slave_3_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_DEN_DISABLE    = 0,
  ASM330LHHX_LEVEL_FIFO     = 6,
  ASM330LHHX_LEVEL_LETCHED  = 3,
  ASM330LHHX_LEVEL_TRIGGER  = 2,
  ASM330LHHX_EDGE_TRIGGER   = 4,
} asm330lhhx_den_mode_t;
int32_t asm330lhhx_den_mode_set(stmdev_ctx_t *ctx,
                                asm330lhhx_den_mode_t val);
int32_t asm330lhhx_den_mode_get(stmdev_ctx_t *ctx,
                                asm330lhhx_den_mode_t *val);

typedef enum
{
  ASM330LHHX_DEN_ACT_LOW  = 0,
  ASM330LHHX_DEN_ACT_HIGH = 1,
} asm330lhhx_den_lh_t;
int32_t asm330lhhx_den_polarity_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_den_lh_t val);
int32_t asm330lhhx_den_polarity_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_den_lh_t *val);

typedef enum
{
  ASM330LHHX_STAMP_IN_GY_DATA     = 0,
  ASM330LHHX_STAMP_IN_XL_DATA     = 1,
  ASM330LHHX_STAMP_IN_GY_XL_DATA  = 2,
} asm330lhhx_den_xl_g_t;
int32_t asm330lhhx_den_enable_set(stmdev_ctx_t *ctx,
                                  asm330lhhx_den_xl_g_t val);
int32_t asm330lhhx_den_enable_get(stmdev_ctx_t *ctx,
                                  asm330lhhx_den_xl_g_t *val);

int32_t asm330lhhx_den_mark_axis_x_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_den_mark_axis_x_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_den_mark_axis_y_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_den_mark_axis_y_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_den_mark_axis_z_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_den_mark_axis_z_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_mag_sensitivity_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_mag_sensitivity_get(stmdev_ctx_t *ctx, uint16_t *val);

int32_t asm330lhhx_mag_offset_set(stmdev_ctx_t *ctx, int16_t *val);
int32_t asm330lhhx_mag_offset_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t asm330lhhx_mag_soft_iron_set(stmdev_ctx_t *ctx, uint16_t *val);
int32_t asm330lhhx_mag_soft_iron_get(stmdev_ctx_t *ctx, uint16_t *val);

typedef enum
{
  ASM330LHHX_Z_EQ_Y     = 0,
  ASM330LHHX_Z_EQ_MIN_Y = 1,
  ASM330LHHX_Z_EQ_X     = 2,
  ASM330LHHX_Z_EQ_MIN_X = 3,
  ASM330LHHX_Z_EQ_MIN_Z = 4,
  ASM330LHHX_Z_EQ_Z     = 5,
} asm330lhhx_mag_z_axis_t;
int32_t asm330lhhx_mag_z_orient_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_z_axis_t val);
int32_t asm330lhhx_mag_z_orient_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_z_axis_t *val);

typedef enum
{
  ASM330LHHX_Y_EQ_Y     = 0,
  ASM330LHHX_Y_EQ_MIN_Y = 1,
  ASM330LHHX_Y_EQ_X     = 2,
  ASM330LHHX_Y_EQ_MIN_X = 3,
  ASM330LHHX_Y_EQ_MIN_Z = 4,
  ASM330LHHX_Y_EQ_Z     = 5,
} asm330lhhx_mag_y_axis_t;
int32_t asm330lhhx_mag_y_orient_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_y_axis_t val);
int32_t asm330lhhx_mag_y_orient_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_y_axis_t *val);

typedef enum
{
  ASM330LHHX_X_EQ_Y     = 0,
  ASM330LHHX_X_EQ_MIN_Y = 1,
  ASM330LHHX_X_EQ_X     = 2,
  ASM330LHHX_X_EQ_MIN_X = 3,
  ASM330LHHX_X_EQ_MIN_Z = 4,
  ASM330LHHX_X_EQ_Z     = 5,
} asm330lhhx_mag_x_axis_t;
int32_t asm330lhhx_mag_x_orient_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_x_axis_t val);
int32_t asm330lhhx_mag_x_orient_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_mag_x_axis_t *val);

int32_t asm330lhhx_long_cnt_flag_data_ready_get(stmdev_ctx_t *ctx,
                                                uint8_t *val);

int32_t asm330lhhx_emb_func_clk_dis_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_emb_func_clk_dis_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_emb_fsm_en_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_emb_fsm_en_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  asm330lhhx_fsm_enable_a_t          fsm_enable_a;
  asm330lhhx_fsm_enable_b_t          fsm_enable_b;
} asm330lhhx_emb_fsm_enable_t;
int32_t asm330lhhx_fsm_enable_set(stmdev_ctx_t *ctx,
                                  asm330lhhx_emb_fsm_enable_t *val);
int32_t asm330lhhx_fsm_enable_get(stmdev_ctx_t *ctx,
                                  asm330lhhx_emb_fsm_enable_t *val);

int32_t asm330lhhx_long_cnt_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_long_cnt_get(stmdev_ctx_t *ctx, uint16_t *val);

typedef enum
{
  ASM330LHHX_LC_NORMAL     = 0,
  ASM330LHHX_LC_CLEAR      = 1,
  ASM330LHHX_LC_CLEAR_DONE = 2,
} asm330lhhx_fsm_lc_clr_t;
int32_t asm330lhhx_long_clr_set(stmdev_ctx_t *ctx,
                                asm330lhhx_fsm_lc_clr_t val);
int32_t asm330lhhx_long_clr_get(stmdev_ctx_t *ctx,
                                asm330lhhx_fsm_lc_clr_t *val);

typedef struct
{
  asm330lhhx_fsm_outs1_t    fsm_outs1;
  asm330lhhx_fsm_outs2_t    fsm_outs2;
  asm330lhhx_fsm_outs3_t    fsm_outs3;
  asm330lhhx_fsm_outs4_t    fsm_outs4;
  asm330lhhx_fsm_outs5_t    fsm_outs5;
  asm330lhhx_fsm_outs6_t    fsm_outs6;
  asm330lhhx_fsm_outs7_t    fsm_outs7;
  asm330lhhx_fsm_outs8_t    fsm_outs8;
  asm330lhhx_fsm_outs9_t    fsm_outs9;
  asm330lhhx_fsm_outs10_t    fsm_outs10;
  asm330lhhx_fsm_outs11_t    fsm_outs11;
  asm330lhhx_fsm_outs12_t    fsm_outs12;
  asm330lhhx_fsm_outs13_t    fsm_outs13;
  asm330lhhx_fsm_outs14_t    fsm_outs14;
  asm330lhhx_fsm_outs15_t    fsm_outs15;
  asm330lhhx_fsm_outs16_t    fsm_outs16;
} asm330lhhx_fsm_out_t;
int32_t asm330lhhx_fsm_out_get(stmdev_ctx_t *ctx, asm330lhhx_fsm_out_t *val);

typedef enum
{
  ASM330LHHX_ODR_FSM_12Hz5 = 0,
  ASM330LHHX_ODR_FSM_26Hz  = 1,
  ASM330LHHX_ODR_FSM_52Hz  = 2,
  ASM330LHHX_ODR_FSM_104Hz = 3,
} asm330lhhx_fsm_odr_t;
int32_t asm330lhhx_fsm_data_rate_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_fsm_odr_t val);
int32_t asm330lhhx_fsm_data_rate_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_fsm_odr_t *val);

int32_t asm330lhhx_fsm_init_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_fsm_init_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_long_cnt_int_value_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_long_cnt_int_value_get(stmdev_ctx_t *ctx, uint16_t *val);

int32_t asm330lhhx_fsm_number_of_programs_set(stmdev_ctx_t *ctx,
                                              uint8_t *buff);
int32_t asm330lhhx_fsm_number_of_programs_get(stmdev_ctx_t *ctx,
                                              uint8_t *buff);

int32_t asm330lhhx_fsm_start_address_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_fsm_start_address_get(stmdev_ctx_t *ctx, uint16_t *val);

int32_t asm330lhhx_mlc_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_mlc_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_mlc_status_get(stmdev_ctx_t *ctx,
                                  asm330lhhx_mlc_status_mainpage_t *val);

typedef enum
{
  ASM330LHHX_ODR_PRGS_12Hz5 = 0,
  ASM330LHHX_ODR_PRGS_26Hz  = 1,
  ASM330LHHX_ODR_PRGS_52Hz  = 2,
  ASM330LHHX_ODR_PRGS_104Hz = 3,
} asm330lhhx_mlc_odr_t;
int32_t asm330lhhx_mlc_data_rate_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_mlc_odr_t val);
int32_t asm330lhhx_mlc_data_rate_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_mlc_odr_t *val);

int32_t asm330lhhx_mlc_init_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_mlc_init_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t asm330lhhx_mlc_out_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t asm330lhhx_mlc_mag_sensitivity_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t asm330lhhx_mlc_mag_sensitivity_get(stmdev_ctx_t *ctx, uint16_t *val);

typedef struct
{
  asm330lhhx_sensor_hub_1_t   sh_byte_1;
  asm330lhhx_sensor_hub_2_t   sh_byte_2;
  asm330lhhx_sensor_hub_3_t   sh_byte_3;
  asm330lhhx_sensor_hub_4_t   sh_byte_4;
  asm330lhhx_sensor_hub_5_t   sh_byte_5;
  asm330lhhx_sensor_hub_6_t   sh_byte_6;
  asm330lhhx_sensor_hub_7_t   sh_byte_7;
  asm330lhhx_sensor_hub_8_t   sh_byte_8;
  asm330lhhx_sensor_hub_9_t   sh_byte_9;
  asm330lhhx_sensor_hub_10_t  sh_byte_10;
  asm330lhhx_sensor_hub_11_t  sh_byte_11;
  asm330lhhx_sensor_hub_12_t  sh_byte_12;
  asm330lhhx_sensor_hub_13_t  sh_byte_13;
  asm330lhhx_sensor_hub_14_t  sh_byte_14;
  asm330lhhx_sensor_hub_15_t  sh_byte_15;
  asm330lhhx_sensor_hub_16_t  sh_byte_16;
  asm330lhhx_sensor_hub_17_t  sh_byte_17;
  asm330lhhx_sensor_hub_18_t  sh_byte_18;
} asm330lhhx_emb_sh_read_t;
int32_t asm330lhhx_sh_read_data_raw_get(stmdev_ctx_t *ctx,
                                        asm330lhhx_emb_sh_read_t *val);

typedef enum
{
  ASM330LHHX_SLV_0       = 0,
  ASM330LHHX_SLV_0_1     = 1,
  ASM330LHHX_SLV_0_1_2   = 2,
  ASM330LHHX_SLV_0_1_2_3 = 3,
} asm330lhhx_aux_sens_on_t;
int32_t asm330lhhx_sh_slave_connected_set(stmdev_ctx_t *ctx,
                                          asm330lhhx_aux_sens_on_t val);
int32_t asm330lhhx_sh_slave_connected_get(stmdev_ctx_t *ctx,
                                          asm330lhhx_aux_sens_on_t *val);

int32_t asm330lhhx_sh_master_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_master_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_EXT_PULL_UP      = 0,
  ASM330LHHX_INTERNAL_PULL_UP = 1,
} asm330lhhx_shub_pu_en_t;
int32_t asm330lhhx_sh_pin_mode_set(stmdev_ctx_t *ctx,
                                   asm330lhhx_shub_pu_en_t val);
int32_t asm330lhhx_sh_pin_mode_get(stmdev_ctx_t *ctx,
                                   asm330lhhx_shub_pu_en_t *val);

int32_t asm330lhhx_sh_pass_through_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t asm330lhhx_sh_pass_through_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_EXT_ON_INT2_PIN = 0,
  ASM330LHHX_XL_GY_DRDY      = 1,
} asm330lhhx_start_config_t;
int32_t asm330lhhx_sh_syncro_mode_set(stmdev_ctx_t *ctx,
                                      asm330lhhx_start_config_t val);
int32_t asm330lhhx_sh_syncro_mode_get(stmdev_ctx_t *ctx,
                                      asm330lhhx_start_config_t *val);

typedef enum
{
  ASM330LHHX_EACH_SH_CYCLE    = 0,
  ASM330LHHX_ONLY_FIRST_CYCLE = 1,
} asm330lhhx_write_once_t;
int32_t asm330lhhx_sh_write_mode_set(stmdev_ctx_t *ctx,
                                     asm330lhhx_write_once_t val);
int32_t asm330lhhx_sh_write_mode_get(stmdev_ctx_t *ctx,
                                     asm330lhhx_write_once_t *val);

int32_t asm330lhhx_sh_reset_set(stmdev_ctx_t *ctx);
int32_t asm330lhhx_sh_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ASM330LHHX_SH_ODR_104Hz = 0,
  ASM330LHHX_SH_ODR_52Hz  = 1,
  ASM330LHHX_SH_ODR_26Hz  = 2,
  ASM330LHHX_SH_ODR_13Hz  = 3,
} asm330lhhx_shub_odr_t;
int32_t asm330lhhx_sh_data_rate_set(stmdev_ctx_t *ctx,
                                    asm330lhhx_shub_odr_t val);
int32_t asm330lhhx_sh_data_rate_get(stmdev_ctx_t *ctx,
                                    asm330lhhx_shub_odr_t *val);

typedef struct
{
  uint8_t   slv0_add;
  uint8_t   slv0_subadd;
  uint8_t   slv0_data;
} asm330lhhx_sh_cfg_write_t;
int32_t asm330lhhx_sh_cfg_write(stmdev_ctx_t *ctx,
                                asm330lhhx_sh_cfg_write_t *val);

typedef struct
{
  uint8_t   slv_add;
  uint8_t   slv_subadd;
  uint8_t   slv_len;
} asm330lhhx_sh_cfg_read_t;
int32_t asm330lhhx_sh_slv0_cfg_read(stmdev_ctx_t *ctx,
                                    asm330lhhx_sh_cfg_read_t *val);
int32_t asm330lhhx_sh_slv1_cfg_read(stmdev_ctx_t *ctx,
                                    asm330lhhx_sh_cfg_read_t *val);
int32_t asm330lhhx_sh_slv2_cfg_read(stmdev_ctx_t *ctx,
                                    asm330lhhx_sh_cfg_read_t *val);
int32_t asm330lhhx_sh_slv3_cfg_read(stmdev_ctx_t *ctx,
                                    asm330lhhx_sh_cfg_read_t *val);

int32_t asm330lhhx_sh_status_get(stmdev_ctx_t *ctx,
                                 asm330lhhx_status_master_t *val);

/**
  *@}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* ASM330LHHX_REGS_H */
