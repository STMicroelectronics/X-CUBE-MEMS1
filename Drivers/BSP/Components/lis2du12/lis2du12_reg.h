/*
 ******************************************************************************
 * @file    lis2du12_reg.h
 * @author  Sensors Software Solution Team
 * @brief   This file contains all the functions prototypes for the
 *          lis2du12_reg.c driver.
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
#ifndef LIS2DU12_REGS_H
#define LIS2DU12_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LIS2DU12
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

/** @defgroup LIS2DU12_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> 0x if SA0=1 -> 0x **/
#define LIS2DU12_I2C_ADD_L     0x31U
#define LIS2DU12_I2C_ADD_H     0x33U

/** Device Identification (Who am I) **/
#define LIS2DU12_ID            0x45U

/**
  * @}
  *
  */

#define LIS2DU12_IF_PU_CTRL    0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01      : 2;
  uint8_t cs_pu_disc       : 1;
  uint8_t not_used_02      : 3;
  uint8_t sda_pu_en        : 1;
  uint8_t sdo_pu_disc      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sdo_pu_disc      : 1;
  uint8_t sda_pu_en        : 1;
  uint8_t not_used_02      : 3;
  uint8_t cs_pu_disc       : 1;
  uint8_t not_used_01      : 2;
#endif /* DRV_BYTE_ORDER */
} lis2du12_if_pu_ctrl_t;

#define LIS2DU12_IF_CTRL       0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t i2c_disable      : 1;
  uint8_t i3c_disable      : 1;
  uint8_t pd_dis_int1      : 1;
  uint8_t not_used_01      : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 5;
  uint8_t pd_dis_int1      : 1;
  uint8_t i3c_disable      : 1;
  uint8_t i2c_disable      : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_if_ctrl_t;

#define LIS2DU12_CTRL1         0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wu_z_en      : 1;
  uint8_t wu_y_en      : 1;
  uint8_t wu_x_en      : 1;
  uint8_t drdy_pulsed  : 1;
  uint8_t if_add_inc   : 1;
  uint8_t sw_reset     : 1;
  uint8_t sim          : 1;
  uint8_t pp_od        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pp_od        : 1;
  uint8_t sim          : 1;
  uint8_t sw_reset     : 1;
  uint8_t if_add_inc   : 1;
  uint8_t drdy_pulsed  : 1;
  uint8_t wu_x_en      : 1;
  uint8_t wu_y_en      : 1;
  uint8_t wu_z_en      : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_ctrl1_t;

#define LIS2DU12_CTRL2         0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01    : 3;
  uint8_t int1_drdy      : 1;
  uint8_t int1_f_ovr     : 1;
  uint8_t int1_f_fth     : 1;
  uint8_t int1_f_full    : 1;
  uint8_t int1_boot      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_boot      : 1;
  uint8_t int1_f_full    : 1;
  uint8_t int1_f_fth     : 1;
  uint8_t int1_f_ovr     : 1;
  uint8_t int1_drdy      : 1;
  uint8_t not_used_01    : 3;
#endif /* DRV_BYTE_ORDER */
} lis2du12_ctrl2_t;

#define LIS2DU12_CTRL3         0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st            : 2;
  uint8_t not_used_01   : 1;
  uint8_t int2_drdy     : 1;
  uint8_t int2_f_ovr    : 1;
  uint8_t int2_f_fth    : 1;
  uint8_t int2_f_full   : 1;
  uint8_t int2_boot     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_boot     : 1;
  uint8_t int2_f_full   : 1;
  uint8_t int2_f_fth    : 1;
  uint8_t int2_f_ovr    : 1;
  uint8_t int2_drdy     : 1;
  uint8_t not_used_01   : 1;
  uint8_t st            : 2;
#endif /* DRV_BYTE_ORDER */
} lis2du12_ctrl3_t;

#define LIS2DU12_CTRL4         0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t boot          : 1;
  uint8_t soc           : 1;
  uint8_t not_used_01   : 3;
  uint8_t bdu           : 1;
  uint8_t inact_odr     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t inact_odr     : 2;
  uint8_t bdu           : 1;
  uint8_t not_used_01   : 3;
  uint8_t soc           : 1;
  uint8_t boot          : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_ctrl4_t;

#define LIS2DU12_CTRL5         0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs            : 2;
  uint8_t bw            : 2;
  uint8_t odr           : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr           : 4;
  uint8_t bw            : 2;
  uint8_t fs            : 2;
#endif /* DRV_BYTE_ORDER */
} lis2du12_ctrl5_t;

#define LIS2DU12_FIFO_CTRL     0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t f_mode        : 3;
  uint8_t stop_on_fth   : 1;
  uint8_t not_used_01   : 2;
  uint8_t fifo_depth    : 1;
  uint8_t rounding_xyz  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t rounding_xyz  : 1;
  uint8_t fifo_depth    : 1;
  uint8_t not_used_01   : 2;
  uint8_t stop_on_fth   : 1;
  uint8_t f_mode        : 3;
#endif /* DRV_BYTE_ORDER */
} lis2du12_fifo_ctrl_t;

#define LIS2DU12_FIFO_WTM      0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fth          : 7;
  uint8_t not_used_01  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01  : 1;
  uint8_t fth          : 7;
#endif /* DRV_BYTE_ORDER */
} lis2du12_fifo_wtm_t;

#define LIS2DU12_INTERRUPT_CFG 0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t interrupts_enable   : 1;
  uint8_t lir                 : 1;
  uint8_t h_lactive           : 1;
  uint8_t sleep_status_on_int : 1;
  uint8_t not_used_01         : 1;
  uint8_t wake_ths_w          : 1;
  uint8_t int_short_en        : 1;
  uint8_t not_used_02         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02         : 1;
  uint8_t int_short_en        : 1;
  uint8_t wake_ths_w          : 1;
  uint8_t not_used_01         : 1;
  uint8_t sleep_status_on_int : 1;
  uint8_t h_lactive           : 1;
  uint8_t lir                 : 1;
  uint8_t interrupts_enable   : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_interrupt_cfg_t;

#define LIS2DU12_TAP_THS_X     0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_x          : 5;
  uint8_t d6d_ths            : 2;
  uint8_t d4d_en             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t d4d_en             : 1;
  uint8_t d6d_ths            : 2;
  uint8_t tap_ths_x          : 5;
#endif /* DRV_BYTE_ORDER */
} lis2du12_tap_ths_x_t;

#define LIS2DU12_TAP_THS_Y     0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_y         : 5;
  uint8_t tap_priority      : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_priority      : 3;
  uint8_t tap_ths_y         : 5;
#endif /* DRV_BYTE_ORDER */
} lis2du12_tap_ths_y_t;

#define LIS2DU12_TAP_THS_Z     0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_z      : 5;
  uint8_t tap_z_en       : 1;
  uint8_t tap_y_en       : 1;
  uint8_t tap_x_en       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_x_en       : 1;
  uint8_t tap_y_en       : 1;
  uint8_t tap_z_en       : 1;
  uint8_t tap_ths_z      : 5;
#endif /* DRV_BYTE_ORDER */
} lis2du12_tap_ths_z_t;

#define LIS2DU12_INT_DUR       0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t shock        : 2;
  uint8_t quiet        : 2;
  uint8_t latency      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t latency      : 4;
  uint8_t quiet        : 2;
  uint8_t shock        : 2;
#endif /* DRV_BYTE_ORDER */
} lis2du12_int_dur_t;

#define LIS2DU12_WAKE_UP_THS   0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wk_ths             : 6;
  uint8_t sleep_on           : 1;
  uint8_t single_double_tap  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t single_double_tap  : 1;
  uint8_t sleep_on           : 1;
  uint8_t wk_ths             : 6;
#endif /* DRV_BYTE_ORDER */
} lis2du12_wake_up_ths_t;

#define LIS2DU12_WAKE_UP_DUR    0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sleep_dur      : 4;
  uint8_t not_used_01    : 1;
  uint8_t wake_dur       : 2;
  uint8_t ff_dur         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur         : 1;
  uint8_t wake_dur       : 2;
  uint8_t not_used_01    : 1;
  uint8_t sleep_dur      : 4;
#endif /* DRV_BYTE_ORDER */
} lis2du12_wake_up_dur_t;

#define LIS2DU12_FREE_FALL     0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths      : 3;
  uint8_t ff_dur      : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur      : 5;
  uint8_t ff_ths      : 3;
#endif /* DRV_BYTE_ORDER */
} lis2du12_free_fall_t;

#define LIS2DU12_MD1_CFG        0x1FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01        : 1;
  uint8_t wu_dur_x4          : 1;
  uint8_t int1_6d            : 1;
  uint8_t int1_double_tap    : 1;
  uint8_t int1_ff            : 1;
  uint8_t int1_wu            : 1;
  uint8_t int1_single_tap    : 1;
  uint8_t int1_sleep_change  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_sleep_change  : 1;
  uint8_t int1_single_tap    : 1;
  uint8_t int1_wu            : 1;
  uint8_t int1_ff            : 1;
  uint8_t int1_double_tap    : 1;
  uint8_t int1_6d            : 1;
  uint8_t wu_dur_x4          : 1;
  uint8_t not_used_01        : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_md1_cfg_t;

#define LIS2DU12_MD2_CFG       0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01        : 1;
  uint8_t pd_dis_int2        : 1;
  uint8_t int2_6d            : 1;
  uint8_t int2_double_tap    : 1;
  uint8_t int2_ff            : 1;
  uint8_t int2_wu            : 1;
  uint8_t int2_single_tap    : 1;
  uint8_t int2_sleep_change  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_sleep_change  : 1;
  uint8_t int2_single_tap    : 1;
  uint8_t int2_wu            : 1;
  uint8_t int2_ff            : 1;
  uint8_t int2_double_tap    : 1;
  uint8_t int2_6d            : 1;
  uint8_t pd_dis_int2        : 1;
  uint8_t not_used_01        : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_md2_cfg_t;

#define LIS2DU12_WAKE_UP_SRC   0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_wu            : 1;
  uint8_t y_wu            : 1;
  uint8_t x_wu            : 1;
  uint8_t wu_ia           : 1;
  uint8_t sleep_state     : 1;
  uint8_t ff_ia           : 1;
  uint8_t sleep_change_ia : 1;
  uint8_t not_used_01     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01     : 1;
  uint8_t sleep_change_ia : 1;
  uint8_t ff_ia           : 1;
  uint8_t sleep_state     : 1;
  uint8_t wu_ia           : 1;
  uint8_t x_wu            : 1;
  uint8_t y_wu            : 1;
  uint8_t z_wu            : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_wake_up_src_t;

#define LIS2DU12_TAP_SRC       0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_tap         : 1;
  uint8_t y_tap         : 1;
  uint8_t x_tap         : 1;
  uint8_t tap_sign      : 1;
  uint8_t double_tap_ia : 1;
  uint8_t single_tap_ia : 1;
  uint8_t tap_ia        : 1;
  uint8_t not_used_01   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01   : 1;
  uint8_t tap_ia        : 1;
  uint8_t single_tap_ia : 1;
  uint8_t double_tap_ia : 1;
  uint8_t tap_sign      : 1;
  uint8_t x_tap         : 1;
  uint8_t y_tap         : 1;
  uint8_t z_tap         : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_tap_src_t;

#define LIS2DU12_SIXD_SRC      0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl          : 1;
  uint8_t xh          : 1;
  uint8_t yl          : 1;
  uint8_t yh          : 1;
  uint8_t zl          : 1;
  uint8_t zh          : 1;
  uint8_t d6d_ia     : 1;
  uint8_t not_used_01 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01 : 1;
  uint8_t d6d_ia     : 1;
  uint8_t zh          : 1;
  uint8_t zl          : 1;
  uint8_t yh          : 1;
  uint8_t yl          : 1;
  uint8_t xh          : 1;
  uint8_t xl          : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_sixd_src_t;

#define LIS2DU12_ALL_INT_SRC   0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ia_all       : 1;
  uint8_t wu_ia_all       : 1;
  uint8_t single_tap_all  : 1;
  uint8_t double_tap_all  : 1;
  uint8_t d6d_ia_all     : 1;
  uint8_t sleep_change_ia_all : 1;
  uint8_t int_global      : 1;
  uint8_t not_used_01     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01     : 1;
  uint8_t int_global      : 1;
  uint8_t sleep_change_ia_all : 1;
  uint8_t d6d_ia_all     : 1;
  uint8_t double_tap_all  : 1;
  uint8_t single_tap_all  : 1;
  uint8_t wu_ia_all       : 1;
  uint8_t ff_ia_all       : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_all_int_src_t;

#define LIS2DU12_STATUS        0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t drdy        : 1;
  uint8_t pd_status   : 1;
  uint8_t not_used_01 : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01 : 6;
  uint8_t pd_status   : 1;
  uint8_t drdy        : 1;
#endif /* DRV_BYTE_ORDER */
} lis2du12_status_register_t;

#define LIS2DU12_FIFO_STATUS1  0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01 : 6;
  uint8_t fifo_ovr    : 1;
  uint8_t fth         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fth         : 1;
  uint8_t fifo_ovr    : 1;
  uint8_t not_used_01 : 6;
#endif /* DRV_BYTE_ORDER */
} lis2du12_fifo_status1_t;

#define LIS2DU12_FIFO_STATUS2  0x27U
typedef struct
{
  uint8_t fss    : 8;
} lis2du12_fifo_status2_t;

#define LIS2DU12_OUTX_L        0x28U
#define LIS2DU12_OUTX_H        0x29U
#define LIS2DU12_OUTY_L        0x2AU
#define LIS2DU12_OUTY_H        0x2BU
#define LIS2DU12_OUTZ_L        0x2CU
#define LIS2DU12_OUTZ_H        0x2DU
#define LIS2DU12_OUTT_L        0x2EU
#define LIS2DU12_OUTT_H        0x2FU
#define LIS2DU12_TEMP_OUT_L    0x30U
#define LIS2DU12_TEMP_OUT_H    0x31U
#define LIS2DU12_WHO_AM_I      0x43U

#define LIS2DU12_ST_SIGN       0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01 : 5;
  uint8_t stsign      : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t stsign      : 3;
  uint8_t not_used_01 : 5;
#endif /* DRV_BYTE_ORDER */
} lis2du12_st_sign_t;

/**
  * @defgroup LIS2DU12_Register_Union
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
  lis2du12_if_pu_ctrl_t      if_pu_ctrl;
  lis2du12_if_ctrl_t         if_ctrl;
  lis2du12_ctrl1_t           ctrl1;
  lis2du12_ctrl2_t           ctrl2;
  lis2du12_ctrl3_t           ctrl3;
  lis2du12_ctrl4_t           ctrl4;
  lis2du12_ctrl5_t           ctrl5;
  lis2du12_fifo_ctrl_t       fifo_ctrl;
  lis2du12_fifo_wtm_t        fifo_wtm;
  lis2du12_interrupt_cfg_t   interrupt_cfg;
  lis2du12_tap_ths_x_t       tap_ths_x;
  lis2du12_tap_ths_y_t       tap_ths_y;
  lis2du12_tap_ths_z_t       tap_ths_z;
  lis2du12_int_dur_t         int_dur;
  lis2du12_wake_up_ths_t     wake_up_ths;
  lis2du12_wake_up_dur_t     wake_up_dur;
  lis2du12_free_fall_t       free_fall;
  lis2du12_md1_cfg_t         md1_cfg;
  lis2du12_md2_cfg_t         md2_cfg;
  lis2du12_wake_up_src_t     wake_up_src;
  lis2du12_tap_src_t         tap_src;
  lis2du12_sixd_src_t        sixd_src;
  lis2du12_all_int_src_t     all_int_src;
  lis2du12_status_register_t status_register;
  lis2du12_fifo_status1_t    fifo_status1;
  lis2du12_fifo_status2_t    fifo_status2;
  bitwise_t                  bitwise;
  uint8_t                    byte;
} lis2du12_reg_t;

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

int32_t lis2du12_read_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                          uint16_t len);
int32_t lis2du12_write_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                           uint16_t len);

float_t lis2du12_from_fs2g_to_mg(int16_t lsb);
float_t lis2du12_from_fs4g_to_mg(int16_t lsb);
float_t lis2du12_from_fs8g_to_mg(int16_t lsb);
float_t lis2du12_from_fs16g_to_mg(int16_t lsb);
float_t lis2du12_from_lsb_to_celsius(int16_t lsb);

typedef struct
{
  uint8_t whoami;
} lis2du12_id_t;
int32_t lis2du12_id_get(const stmdev_ctx_t *ctx, lis2du12_id_t *val);

typedef enum
{
  LIS2DU12_SEL_BY_HW   = 0x00, /* bus mode select by HW (SPI 3W disable) */
  LIS2DU12_SPI_4W      = 0x03, /* Only SPI: SDO / SDI separated pins */
  LIS2DU12_SPI_3W      = 0x07, /* Only SPI: SDO / SDI share the same pin */
  LIS2DU12_I3C_DISABLE = 0x01, /* Select by HW (SPI 3W and I3C disable) */
} lis2du12_bus_mode_t;
int32_t lis2du12_bus_mode_set(const stmdev_ctx_t *ctx, lis2du12_bus_mode_t val);
int32_t lis2du12_bus_mode_get(const stmdev_ctx_t *ctx, lis2du12_bus_mode_t *val);

typedef enum
{
  LIS2DU12_DRV_RDY   = 0x00, /* Initialize the device for driver usage */
  LIS2DU12_BOOT      = 0x01, /* Restore calib. param. ( it takes 10ms ) */
  LIS2DU12_RESET     = 0x02, /* Reset configuration registers */
} lis2du12_init_t;
int32_t lis2du12_init_set(const stmdev_ctx_t *ctx, lis2du12_init_t val);

typedef struct
{
  uint8_t sw_reset           : 1; /* Restoring configuration registers */
  uint8_t boot               : 1; /* Restoring calibration parameters */
  uint8_t drdy_xl            : 1; /* Accelerometer data ready */
  uint8_t power_down         : 1; /* Monitors power-down. */
} lis2du12_status_t;
int32_t lis2du12_status_get(const stmdev_ctx_t *ctx, lis2du12_status_t *val);

typedef struct
{
  uint8_t sdo_pull_up         : 1; /* 1 = pull up enable */
  uint8_t sda_pull_up         : 1; /* 1 = pull up enable */
  uint8_t cs_pull_up          : 1; /* 1 = pull up enable */
  uint8_t int1_int2_push_pull : 1; /* 1 = push-pull / 0 = open-drain*/
  uint8_t int1_pull_down      : 1; /* 1 = pull-down always disabled (0=auto) */
  uint8_t int2_pull_down      : 1; /* 1 = pull-down always disabled (0=auto) */
} lis2du12_pin_conf_t;
int32_t lis2du12_pin_conf_set(const stmdev_ctx_t *ctx, lis2du12_pin_conf_t *val);
int32_t lis2du12_pin_conf_get(const stmdev_ctx_t *ctx, lis2du12_pin_conf_t *val);

typedef struct
{
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
  uint8_t six_d            :  1; /* orientation change (6D/4D detection) */
  uint8_t six_d_xl         :  1; /* X-axis low 6D/4D event (under threshold) */
  uint8_t six_d_xh         :  1; /* X-axis high 6D/4D event (over threshold) */
  uint8_t six_d_yl         :  1; /* Y-axis low 6D/4D event (under threshold) */
  uint8_t six_d_yh         :  1; /* Y-axis high 6D/4D event (over threshold) */
  uint8_t six_d_zl         :  1; /* Z-axis low 6D/4D event (under threshold) */
  uint8_t six_d_zh         :  1; /* Z-axis high 6D/4D event (over threshold) */
  uint8_t sleep_change     :  1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t sleep_state      :  1; /* Act/Inact status flag (0-Act / 1-Inact) */
} lis2du12_all_sources_t;
int32_t lis2du12_all_sources_get(const stmdev_ctx_t *ctx,
                                 lis2du12_all_sources_t *val);

typedef enum
{
  LIS2DU12_OFF               = 0x00, /* in power down */
  LIS2DU12_1Hz6_ULP          = 0x01, /* @1Hz6(ultralow power) */
  LIS2DU12_3Hz_ULP           = 0x02, /* @3Hz (ultralow power) */
  LIS2DU12_6Hz_ULP           = 0x03, /* @6Hz (ultralow power) */
  LIS2DU12_6Hz               = 0x04, /* @6Hz (normal) */
  LIS2DU12_12Hz5             = 0x05, /* @12Hz5 (normal) */
  LIS2DU12_25Hz              = 0x06, /* @25Hz  (normal) */
  LIS2DU12_50Hz              = 0x07, /* @50Hz  (normal) */
  LIS2DU12_100Hz             = 0x08, /* @100Hz (normal) */
  LIS2DU12_200Hz             = 0x09, /* @200Hz (normal) */
  LIS2DU12_400Hz             = 0x0A, /* @400Hz (normal) */
  LIS2DU12_800Hz             = 0x0B, /* @800Hz (normal) */
  LIS2DU12_TRIG_PIN          = 0x0E, /* Single-shot high latency by INT2 */
  LIS2DU12_TRIG_SW           = 0x0F, /* Single-shot high latency by IF */
} lis2du12_odr_t;

typedef enum
{
  LIS2DU12_2g   = 0,
  LIS2DU12_4g   = 1,
  LIS2DU12_8g   = 2,
  LIS2DU12_16g  = 3,
} lis2du12_fs_t;

typedef enum
{
  LIS2DU12_ODR_div_2   = 0,
  LIS2DU12_ODR_div_4   = 1,
  LIS2DU12_ODR_div_8   = 2,
  LIS2DU12_ODR_div_16  = 3,
} lis2du12_bw_t;

typedef struct
{
  lis2du12_odr_t odr;
  lis2du12_fs_t fs;
  lis2du12_bw_t bw;
} lis2du12_md_t;
int32_t lis2du12_mode_set(const stmdev_ctx_t *ctx, lis2du12_md_t *val);
int32_t lis2du12_mode_get(const stmdev_ctx_t *ctx, lis2du12_md_t *val);

int32_t lis2du12_trigger_sw(const stmdev_ctx_t *ctx, lis2du12_md_t *val);

typedef struct
{
  struct
  {
    float_t mg[3];
    int16_t raw[3];
  } xl;
  struct
  {
    float_t deg_c;
    int16_t raw;
  } heat;
} lis2du12_data_t;
int32_t lis2du12_data_get(const stmdev_ctx_t *ctx, lis2du12_md_t *md,
                          lis2du12_data_t *data);

typedef enum
{
  LIS2DU12_ST_DISABLE  = 0,
  LIS2DU12_ST_POSITIVE = 6,
  LIS2DU12_ST_NEGATIVE = 1,
} lis2du12_st_t;
int32_t lis2du12_self_test_sign_set(const stmdev_ctx_t *ctx, lis2du12_st_t val);
int32_t lis2du12_self_test_start(const stmdev_ctx_t *ctx, uint8_t val); /* valid values: 1 or 2 */
int32_t lis2du12_self_test_stop(const stmdev_ctx_t *ctx);

typedef enum
{
  LIS2DU12_BYPASS            = 0,
  LIS2DU12_FIFO              = 1,
  LIS2DU12_STREAM            = 6,
  LIS2DU12_STREAM_TO_FIFO    = 3, /* Dynamic-Stream, FIFO on Trigger */
  LIS2DU12_BYPASS_TO_STREAM  = 4, /* Bypass, Dynamic-Stream on Trigger */
  LIS2DU12_BYPASS_TO_FIFO    = 7, /* Bypass, FIFO on Trigger */
} lis2du12_fifo_operation_t;

typedef enum
{
  LIS2DU12_8_BIT    = 0,
  LIS2DU12_16_BIT   = 1,
} lis2du12_fifo_store_t;

typedef struct
{
  lis2du12_fifo_operation_t operation;
  lis2du12_fifo_store_t store;
  uint8_t watermark; /* (0 disable) max 127 @16bit, even and max 256 @8bit.*/
} lis2du12_fifo_md_t;
int32_t lis2du12_fifo_mode_set(const stmdev_ctx_t *ctx, lis2du12_fifo_md_t *val);
int32_t lis2du12_fifo_mode_get(const stmdev_ctx_t *ctx, lis2du12_fifo_md_t *val);

typedef struct
{
  uint8_t fifo_fth         : 1; /* 1 = fifo threshold event */
  uint8_t fifo_ovr         : 1; /* 1 = fifo overrun event */
} lis2du12_fifo_status_t;
int32_t lis2du12_fifo_status_get(const stmdev_ctx_t *ctx, lis2du12_fifo_status_t *val);

int32_t lis2du12_fifo_level_get(const stmdev_ctx_t *ctx, lis2du12_fifo_md_t *md,
                                uint8_t *val);

typedef struct
{
  struct
  {
    float_t mg[3];
    int16_t raw[3];
  } xl[2];
  struct
  {
    float_t deg_c;
    int16_t raw;
  } heat;
} lis2du12_fifo_data_t;
int32_t lis2du12_fifo_data_get(const stmdev_ctx_t *ctx, lis2du12_md_t *md,
                               lis2du12_fifo_md_t *fmd,
                               lis2du12_fifo_data_t *data);

typedef enum
{
  LIS2DU12_INT_LEVEL    = 0, /* active till event condition persist */
  LIS2DU12_INT_LATCHED  = 3, /* read ALL_INT_SRC for reset (API all_sources)*/
} lis2du12_base_sig_t; /* base functions are: FF, WU(W2S), 4/6D, Tap */

typedef struct
{
  uint8_t enable       : 1; /* 1 = enabled / 0 = disabled */
  uint8_t active_low   : 1; /* 1 = active low / 0 = active high */
  uint8_t drdy_latched : 1; /* drdy returns to 0 after reading data */
  lis2du12_base_sig_t base_sig;
} lis2du12_int_mode_t;
int32_t lis2du12_interrupt_mode_set(const stmdev_ctx_t *ctx,
                                    lis2du12_int_mode_t *val);
int32_t lis2du12_interrupt_mode_get(const stmdev_ctx_t *ctx,
                                    lis2du12_int_mode_t *val);

typedef struct
{
  uint8_t drdy_xl       : 1; /* Accelerometer data ready */
  uint8_t boot          : 1; /* Restoring calibration parameters */
  uint8_t fifo_th       : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr      : 1; /* FIFO overrun */
  uint8_t fifo_full     : 1; /* FIFO full */
  uint8_t free_fall     : 1; /* free fall event */
  uint8_t six_d         : 1; /* orientation change (6D/4D detection) */
  uint8_t single_tap    : 1; /* single-tap event */
  uint8_t double_tap    : 1; /* double tap event */
  uint8_t wake_up       : 1; /* wake up event */
  uint8_t sleep_change  : 1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t sleep_state   :  1; /* Act/Inact status flag */
} lis2du12_pin_int_route_t;
int32_t lis2du12_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                    lis2du12_pin_int_route_t *val);
int32_t lis2du12_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                    lis2du12_pin_int_route_t *val);

int32_t lis2du12_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                    lis2du12_pin_int_route_t *val);
int32_t lis2du12_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                    lis2du12_pin_int_route_t *val);

typedef enum
{
  LIS2DU12_DO_NOT_CHANGE = 0,
  LIS2DU12_SLEEP_AT_6Hz  = 1,
  LIS2DU12_SLEEP_AT_3Hz  = 2,
  LIS2DU12_SLEEP_AT_1Hz6 = 3,
} lis2du12_sleep_odr_t;

typedef struct
{
  uint8_t x_en  : 1; /* Detection on X-axis */
  uint8_t y_en  : 1; /* Detection on Y-axis */
  uint8_t z_en  : 1; /* Detection on Z-axis */
  uint8_t threshold; /* full scale dependent */
  uint8_t duration;  /* 1 LSb: 1 ODR_time */
  struct
  {
    uint8_t en  : 1; /* Enable sleep detection */
    uint8_t duration; /* 0 is 16 ODR_time, 1 LSB: 512 ODR_time. */
    lis2du12_sleep_odr_t odr;
  } sleep;
} lis2du12_wkup_md_t;
int32_t lis2du12_wake_up_mode_set(const stmdev_ctx_t *ctx, lis2du12_wkup_md_t *val);
int32_t lis2du12_wake_up_mode_get(const stmdev_ctx_t *ctx, lis2du12_wkup_md_t *val);

typedef enum
{
  LIS2DU12_XYZ = 0,
  LIS2DU12_YXZ = 1,
  LIS2DU12_XZY = 2,
  LIS2DU12_ZYX = 3,
  LIS2DU12_YZX = 5,
  LIS2DU12_ZXY = 6,
} lis2du12_priority_t;

typedef struct
{
  uint8_t x_en  : 1; /* Detection on X-axis */
  uint8_t y_en  : 1; /* Detection on Y-axis */
  uint8_t z_en  : 1; /* Detection on Z-axis */
  struct
  {
    uint8_t x;
    uint8_t y;
    uint8_t z;
  } threshold;
  uint8_t shock; /* max shock time. 0 is 4 ODR_time, 1 LSb : 8 ODR_time. */
  uint8_t quiet; /* Time after a tap. 0 is 2 ODR_time, 1 LSB : 4 ODR_time.*/
  lis2du12_priority_t priority;
  struct
  {
    uint8_t en  : 1; /* Double tap detection */
    uint8_t latency; /* Max time gap. 0 is 16 ODR_time, 1 LSB : 32 ODR_time. */
  } tap_double;
} lis2du12_tap_md_t;
int32_t lis2du12_tap_mode_set(const stmdev_ctx_t *ctx, lis2du12_tap_md_t *val);
int32_t lis2du12_tap_mode_get(const stmdev_ctx_t *ctx, lis2du12_tap_md_t *val);

typedef enum
{
  LIS2DU12_156mg = 0,
  LIS2DU12_219mg = 1,
  LIS2DU12_250mg = 2,
  LIS2DU12_312mg = 3,
  LIS2DU12_344mg = 4,
  LIS2DU12_406mg = 5,
  LIS2DU12_469mg = 6,
  LIS2DU12_500mg = 7,
} lis2du12_threshold_t;

typedef struct
{
  lis2du12_threshold_t threshold;
  uint8_t duration; /* 1 LSb: 1 ODR_time */
} lis2du12_ff_md_t;
int32_t lis2du12_free_fall_mode_set(const stmdev_ctx_t *ctx, lis2du12_ff_md_t *val);
int32_t lis2du12_free_fall_mode_get(const stmdev_ctx_t *ctx, lis2du12_ff_md_t *val);

typedef enum
{
  LIS2DU12_DEG_80 = 0,
  LIS2DU12_DEG_70 = 1,
  LIS2DU12_DEG_60 = 2,
  LIS2DU12_DEG_50 = 3,
} lis2du12_orient_trshld_t;

typedef enum
{
  LIS2DU12_SIX  = 0,
  LIS2DU12_FOUR = 1,
} lis2du12_deg_of_freedom_t;

typedef struct
{
  lis2du12_orient_trshld_t threshold;
  lis2du12_deg_of_freedom_t deg_of_freedom;
} lis2du12_orient_md_t;
int32_t lis2du12_orientation_mode_set(const stmdev_ctx_t *ctx,
                                      lis2du12_orient_md_t *val);
int32_t lis2du12_orientation_mode_get(const stmdev_ctx_t *ctx,
                                      lis2du12_orient_md_t *val);

/**
  *@}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LIS2DU12_REGS_H */
