/*
 ******************************************************************************
 * @file    st1vafe3bx_reg.h
 * @author  Sensors Software Solution Team
 * @brief   This file contains all the functions prototypes for the
 *          st1vafe3bx_reg.c driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST1VAFE3BX_REGS_H
#define ST1VAFE3BX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup ST1VAFE3BX
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

typedef int32_t (*stmdev_write_ptr)(void *ctx, uint8_t reg,
                                    uint8_t *data, uint16_t len);
typedef int32_t (*stmdev_read_ptr)(void *ctx, uint8_t reg,
                                   uint8_t *data, uint16_t len);
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

/** @defgroup ST1VAFE3BX_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> 0x if SA0=1 -> 0x **/
#define ST1VAFE3BX_I2C_ADD_L                            0x41U
#define ST1VAFE3BX_I2C_ADD_H                            0x43U

/** Device Identification (Who am I) **/
#define ST1VAFE3BX_ID                                   0x48U

/**
  * @}
  *
  */

#define ST1VAFE3BX_EXT_CLK_CFG                          0x08U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 7;
  uint8_t ext_clk_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ext_clk_en                   : 1;
  uint8_t not_used0                    : 7;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ext_clk_cfg_t;

#define ST1VAFE3BX_PIN_CTRL                             0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sim                          : 1;
  uint8_t pp_od                        : 1;
  uint8_t cs_pu_dis                    : 1;
  uint8_t h_lactive                    : 1;
  uint8_t not_used0                    : 2;
  uint8_t sda_pu_en                    : 1;
  uint8_t sdo_pu_en                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sdo_pu_en                    : 1;
  uint8_t sda_pu_en                    : 1;
  uint8_t not_used0                    : 2;
  uint8_t h_lactive                    : 1;
  uint8_t cs_pu_dis                    : 1;
  uint8_t pp_od                        : 1;
  uint8_t sim                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_pin_ctrl_t;

#define ST1VAFE3BX_WAKE_UP_DUR_EXT                      0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t wu_dur_extended              : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t wu_dur_extended              : 1;
  uint8_t not_used0                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_wake_up_dur_ext_t;

#define ST1VAFE3BX_WHO_AM_I                             0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t id                           : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t id                           : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_who_am_i_t;

#define ST1VAFE3BX_CTRL1                                0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wu_z_en                      : 1;
  uint8_t wu_y_en                      : 1;
  uint8_t wu_x_en                      : 1;
  uint8_t drdy_pulsed                  : 1;
  uint8_t if_add_inc                   : 1;
  uint8_t sw_reset                     : 1;
  uint8_t int_pin_en                   : 1;
  uint8_t smart_power_en               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t smart_power_en               : 1;
  uint8_t int_pin_en                   : 1;
  uint8_t sw_reset                     : 1;
  uint8_t if_add_inc                   : 1;
  uint8_t drdy_pulsed                  : 1;
  uint8_t wu_x_en                      : 1;
  uint8_t wu_y_en                      : 1;
  uint8_t wu_z_en                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ctrl1_t;

#define ST1VAFE3BX_CTRL2                                0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t int_drdy                     : 1;
  uint8_t int_fifo_ovr                 : 1;
  uint8_t int_fifo_th                  : 1;
  uint8_t int_fifo_full                : 1;
  uint8_t int_boot                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_boot                     : 1;
  uint8_t int_fifo_full                : 1;
  uint8_t int_fifo_th                  : 1;
  uint8_t int_fifo_ovr                 : 1;
  uint8_t int_drdy                     : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ctrl2_t;

#define ST1VAFE3BX_CTRL3                                0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_sign_x                    : 1;
  uint8_t st_sign_y                    : 1;
  uint8_t hp_en                        : 1;
  uint8_t not_used0                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t hp_en                        : 1;
  uint8_t st_sign_y                    : 1;
  uint8_t st_sign_x                    : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ctrl3_t;

#define ST1VAFE3BX_CTRL4                                0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t boot                         : 1;
  uint8_t soc                          : 1;
  uint8_t not_used0                    : 1;
  uint8_t fifo_en                      : 1;
  uint8_t emb_func_en                  : 1;
  uint8_t bdu                          : 1;
  uint8_t inact_odr                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t inact_odr                    : 2;
  uint8_t bdu                          : 1;
  uint8_t emb_func_en                  : 1;
  uint8_t fifo_en                      : 1;
  uint8_t not_used0                    : 1;
  uint8_t soc                          : 1;
  uint8_t boot                         : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ctrl4_t;

#define ST1VAFE3BX_CTRL5                                0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs                           : 2;
  uint8_t bw                           : 2;
  uint8_t odr                          : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr                          : 4;
  uint8_t bw                           : 2;
  uint8_t fs                           : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ctrl5_t;

#define ST1VAFE3BX_FIFO_CTRL                            0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_mode                    : 3;
  uint8_t stop_on_fth                  : 1;
  uint8_t fifo_en_adv                  : 1;
  uint8_t not_used0                    : 1;
  uint8_t fifo_depth                   : 1;
  uint8_t cfg_chg_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t cfg_chg_en                   : 1;
  uint8_t fifo_depth                   : 1;
  uint8_t not_used0                    : 1;
  uint8_t fifo_en_adv                  : 1;
  uint8_t stop_on_fth                  : 1;
  uint8_t fifo_mode                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_ctrl_t;

#define ST1VAFE3BX_FIFO_WTM                             0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fth                          : 7;
  uint8_t xl_only_fifo                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t xl_only_fifo                 : 1;
  uint8_t fth                          : 7;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_wtm_t;

#define ST1VAFE3BX_INTERRUPT_CFG                        0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t interrupts_enable            : 1;
  uint8_t lir                          : 1;
  uint8_t dis_rst_lir_all_int          : 1;
  uint8_t sleep_status_on_int          : 1;
  uint8_t not_used0                    : 1;
  uint8_t wake_ths_w                   : 1;
  uint8_t not_used1                    : 1;
  uint8_t timestamp_en                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_en                 : 1;
  uint8_t not_used1                    : 1;
  uint8_t wake_ths_w                   : 1;
  uint8_t not_used0                    : 1;
  uint8_t sleep_status_on_int          : 1;
  uint8_t dis_rst_lir_all_int          : 1;
  uint8_t lir                          : 1;
  uint8_t interrupts_enable            : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_interrupt_cfg_t;

#define ST1VAFE3BX_SIXD                                 0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t d6d_ths                      : 2;
  uint8_t d4d_en                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t d4d_en                       : 1;
  uint8_t d6d_ths                      : 2;
  uint8_t not_used0                    : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_sixd_t;

#define ST1VAFE3BX_WAKE_UP_THS                          0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wk_ths                       : 6;
  uint8_t sleep_on                     : 1;
  uint8_t not_used0                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t sleep_on                     : 1;
  uint8_t wk_ths                       : 6;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_wake_up_ths_t;

#define ST1VAFE3BX_WAKE_UP_DUR                          0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sleep_dur                    : 4;
  uint8_t st_sign_z                    : 1;
  uint8_t wake_dur                     : 2;
  uint8_t ff_dur                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                       : 1;
  uint8_t wake_dur                     : 2;
  uint8_t st_sign_z                    : 1;
  uint8_t sleep_dur                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_wake_up_dur_t;

#define ST1VAFE3BX_FREE_FALL                            0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths                       : 3;
  uint8_t ff_dur                       : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                       : 5;
  uint8_t ff_ths                       : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_free_fall_t;

#define ST1VAFE3BX_MD1_CFG                              0x1FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_emb_func                 : 1;
  uint8_t int_timestamp                : 1;
  uint8_t int_6d                       : 1;
  uint8_t int_tap                      : 1;
  uint8_t int_ff                       : 1;
  uint8_t int_wu                       : 1;
  uint8_t not_used0                    : 1;
  uint8_t int_sleep_change             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_sleep_change             : 1;
  uint8_t not_used0                    : 1;
  uint8_t int_wu                       : 1;
  uint8_t int_ff                       : 1;
  uint8_t int_tap                      : 1;
  uint8_t int_6d                       : 1;
  uint8_t int_timestamp                : 1;
  uint8_t int_emb_func                 : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_md1_cfg_t;

#define ST1VAFE3BX_WAKE_UP_SRC                          0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_wu                         : 1;
  uint8_t y_wu                         : 1;
  uint8_t x_wu                         : 1;
  uint8_t wu_ia                        : 1;
  uint8_t sleep_state                  : 1;
  uint8_t ff_ia                        : 1;
  uint8_t sleep_change_ia              : 1;
  uint8_t not_used0                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t sleep_change_ia              : 1;
  uint8_t ff_ia                        : 1;
  uint8_t sleep_state                  : 1;
  uint8_t wu_ia                        : 1;
  uint8_t x_wu                         : 1;
  uint8_t y_wu                         : 1;
  uint8_t z_wu                         : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_wake_up_src_t;

#define ST1VAFE3BX_TAP_SRC                              0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t triple_tap_ia                : 1;
  uint8_t double_tap_ia                : 1;
  uint8_t single_tap_ia                : 1;
  uint8_t tap_ia                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_ia                       : 1;
  uint8_t single_tap_ia                : 1;
  uint8_t double_tap_ia                : 1;
  uint8_t triple_tap_ia                : 1;
  uint8_t not_used0                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_src_t;

#define ST1VAFE3BX_SIXD_SRC                             0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl                           : 1;
  uint8_t xh                           : 1;
  uint8_t yl                           : 1;
  uint8_t yh                           : 1;
  uint8_t zl                           : 1;
  uint8_t zh                           : 1;
  uint8_t d6d_ia                       : 1;
  uint8_t not_used0                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t d6d_ia                       : 1;
  uint8_t zh                           : 1;
  uint8_t zl                           : 1;
  uint8_t yh                           : 1;
  uint8_t yl                           : 1;
  uint8_t xh                           : 1;
  uint8_t xl                           : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_sixd_src_t;

#define ST1VAFE3BX_ALL_INT_SRC                          0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ia_all                    : 1;
  uint8_t wu_ia_all                    : 1;
  uint8_t single_tap_all               : 1;
  uint8_t double_tap_all               : 1;
  uint8_t triple_tap_all               : 1;
  uint8_t d6d_ia_all                   : 1;
  uint8_t sleep_change_ia_all          : 1;
  uint8_t not_used0                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t sleep_change_ia_all          : 1;
  uint8_t d6d_ia_all                   : 1;
  uint8_t triple_tap_all               : 1;
  uint8_t double_tap_all               : 1;
  uint8_t single_tap_all               : 1;
  uint8_t wu_ia_all                    : 1;
  uint8_t ff_ia_all                    : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_all_int_src_t;

#define ST1VAFE3BX_STATUS                               0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t drdy                         : 1;
  uint8_t not_used0                    : 4;
  uint8_t int_global                   : 1;
  uint8_t not_used1                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 2;
  uint8_t int_global                   : 1;
  uint8_t not_used0                    : 4;
  uint8_t drdy                         : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_status_register_t;

#define ST1VAFE3BX_FIFO_STATUS1                         0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 6;
  uint8_t fifo_ovr_ia                  : 1;
  uint8_t fifo_wtm_ia                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_wtm_ia                  : 1;
  uint8_t fifo_ovr_ia                  : 1;
  uint8_t not_used0                    : 6;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_status1_t;

#define ST1VAFE3BX_FIFO_STATUS2                         0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fss                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fss                          : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_status2_t;

#define ST1VAFE3BX_OUT_X_L                              0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_x_l_t;

#define ST1VAFE3BX_OUT_X_H                              0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_x_h_t;

#define ST1VAFE3BX_OUT_Y_L                              0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_y_l_t;

#define ST1VAFE3BX_OUT_Y_H                              0x2BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_y_h_t;

#define ST1VAFE3BX_OUT_Z_L                              0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_z_l_t;

#define ST1VAFE3BX_OUT_Z_H                              0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_z_h_t;

#define ST1VAFE3BX_OUT_AH_BIO_L                         0x2EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t out_ah_bio                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t out_ah_bio                   : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_ah_bio_l_t;

#define ST1VAFE3BX_OUT_AH_BIO_H                         0x2FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t out_ah_bio                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t out_ah_bio                   : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_out_ah_bio_h_t;

#define ST1VAFE3BX_AH_BIO_CFG1                          0x30U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t ah_bio_zin_dis_ah2_bio1      : 1;
  uint8_t ah_bio_zin_dis_ah2_bio2      : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t ah_bio_zin_dis_ah2_bio2      : 1;
  uint8_t ah_bio_zin_dis_ah2_bio1      : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ah_bio_cfg1_t;

#define ST1VAFE3BX_AH_BIO_CFG2                          0x31U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio_en                    : 1;
  uint8_t ah_bio_gain                  : 2;
  uint8_t ah_bio_c_zin                 : 2;
  uint8_t ah_bio_mode                  : 2;
  uint8_t not_used0                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t ah_bio_mode                  : 2;
  uint8_t ah_bio_c_zin                 : 2;
  uint8_t ah_bio_gain                  : 2;
  uint8_t ah_bio_en                    : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ah_bio_cfg2_t;

#define ST1VAFE3BX_AH_BIO_CFG3                          0x32U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio_active                : 1;
  uint8_t not_used0                    : 3;
  uint8_t st                           : 2;
  uint8_t not_used1                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 2;
  uint8_t st                           : 2;
  uint8_t not_used0                    : 3;
  uint8_t ah_bio_active                : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ah_bio_cfg3_t;

#define ST1VAFE3BX_I3C_IF_CTRL                          0x33U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bus_act_sel                  : 2;
  uint8_t not_used0                    : 3;
  uint8_t asf_on                       : 1;
  uint8_t not_used1                    : 1;
  uint8_t dis_drstdaa                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dis_drstdaa                  : 1;
  uint8_t not_used1                    : 1;
  uint8_t asf_on                       : 1;
  uint8_t not_used0                    : 3;
  uint8_t bus_act_sel                  : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_i3c_if_ctrl_t;

#define ST1VAFE3BX_EMB_FUNC_STATUS_MAINPAGE             0x34U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t is_step_det                  : 1;
  uint8_t is_tilt                      : 1;
  uint8_t is_sigmot                    : 1;
  uint8_t not_used1                    : 1;
  uint8_t is_fsm_lc                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                    : 1;
  uint8_t not_used1                    : 1;
  uint8_t is_sigmot                    : 1;
  uint8_t is_tilt                      : 1;
  uint8_t is_step_det                  : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_status_mainpage_t;

#define ST1VAFE3BX_FSM_STATUS_MAINPAGE                  0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                      : 1;
  uint8_t is_fsm2                      : 1;
  uint8_t is_fsm3                      : 1;
  uint8_t is_fsm4                      : 1;
  uint8_t is_fsm5                      : 1;
  uint8_t is_fsm6                      : 1;
  uint8_t is_fsm7                      : 1;
  uint8_t is_fsm8                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                      : 1;
  uint8_t is_fsm7                      : 1;
  uint8_t is_fsm6                      : 1;
  uint8_t is_fsm5                      : 1;
  uint8_t is_fsm4                      : 1;
  uint8_t is_fsm3                      : 1;
  uint8_t is_fsm2                      : 1;
  uint8_t is_fsm1                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_status_mainpage_t;

#define ST1VAFE3BX_MLC_STATUS_MAINPAGE                  0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1                      : 1;
  uint8_t is_mlc2                      : 1;
  uint8_t is_mlc3                      : 1;
  uint8_t is_mlc4                      : 1;
  uint8_t not_used0                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t is_mlc4                      : 1;
  uint8_t is_mlc3                      : 1;
  uint8_t is_mlc2                      : 1;
  uint8_t is_mlc1                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc_status_mainpage_t;

#define ST1VAFE3BX_EN_DEVICE_CONFIG                     0x3EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t en_dev_conf                  : 1;
  uint8_t not_used0                    : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 7;
  uint8_t en_dev_conf                  : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_en_device_config_t;

#define ST1VAFE3BX_FUNC_CFG_ACCESS                      0x3FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_wr_ctrl_en               : 1;
  uint8_t not_used0                    : 6;
  uint8_t emb_func_reg_access          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_reg_access          : 1;
  uint8_t not_used0                    : 6;
  uint8_t fsm_wr_ctrl_en               : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_func_cfg_access_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_TAG                    0x40U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t tag_sensor                   : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tag_sensor                   : 5;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_tag_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_X_L                    0x41U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_x_l_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_X_H                    0x42U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_x_h_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_Y_L                    0x43U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_y_l_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_Y_H                    0x44U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_y_h_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_Z_L                    0x45U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_z_l_t;

#define ST1VAFE3BX_FIFO_DATA_OUT_Z_H                    0x46U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_data_out_z_h_t;

#define ST1VAFE3BX_FIFO_BATCH_DEC                       0x47U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bdr_xl                       : 3;
  uint8_t dec_ts_batch                 : 2;
  uint8_t not_used0                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t dec_ts_batch                 : 2;
  uint8_t bdr_xl                       : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fifo_batch_dec_t;

#define ST1VAFE3BX_TAP_CFG0                             0x6FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 1;
  uint8_t invert_t                     : 5;
  uint8_t axis                         : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t axis                         : 2;
  uint8_t invert_t                     : 5;
  uint8_t not_used0                    : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg0_t;

#define ST1VAFE3BX_TAP_CFG1                             0x70U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t post_still_t                 : 4;
  uint8_t pre_still_ths                : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pre_still_ths                : 4;
  uint8_t post_still_t                 : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg1_t;

#define ST1VAFE3BX_TAP_CFG2                             0x71U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wait_t                       : 6;
  uint8_t post_still_t                 : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t post_still_t                 : 2;
  uint8_t wait_t                       : 6;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg2_t;

#define ST1VAFE3BX_TAP_CFG3                             0x72U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t latency_t                    : 4;
  uint8_t post_still_ths               : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t post_still_ths               : 4;
  uint8_t latency_t                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg3_t;

#define ST1VAFE3BX_TAP_CFG4                             0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t peak_ths                     : 6;
  uint8_t not_used0                    : 1;
  uint8_t wait_end_latency             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wait_end_latency             : 1;
  uint8_t not_used0                    : 1;
  uint8_t peak_ths                     : 6;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg4_t;

#define ST1VAFE3BX_TAP_CFG5                             0x74U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t rebound_t                    : 5;
  uint8_t single_tap_en                : 1;
  uint8_t double_tap_en                : 1;
  uint8_t triple_tap_en                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t triple_tap_en                : 1;
  uint8_t double_tap_en                : 1;
  uint8_t single_tap_en                : 1;
  uint8_t rebound_t                    : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg5_t;

#define ST1VAFE3BX_TAP_CFG6                             0x75U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pre_still_n                  : 4;
  uint8_t pre_still_st                 : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pre_still_st                 : 4;
  uint8_t pre_still_n                  : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_tap_cfg6_t;

#define ST1VAFE3BX_TIMESTAMP0                           0x7AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_timestamp0_t;

#define ST1VAFE3BX_TIMESTAMP1                           0x7BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_timestamp1_t;

#define ST1VAFE3BX_TIMESTAMP2                           0x7CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_timestamp2_t;

#define ST1VAFE3BX_TIMESTAMP3                           0x7DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_timestamp3_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page embedded
  * @{
  *
  */

#define ST1VAFE3BX_PAGE_SEL                             0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t page_sel                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_sel                     : 4;
  uint8_t not_used0                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_page_sel_t;

#define ST1VAFE3BX_EMB_FUNC_EN_A                        0x4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t pedo_en                      : 1;
  uint8_t tilt_en                      : 1;
  uint8_t sign_motion_en               : 1;
  uint8_t not_used1                    : 1;
  uint8_t mlc_before_fsm_en            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_before_fsm_en            : 1;
  uint8_t not_used1                    : 1;
  uint8_t sign_motion_en               : 1;
  uint8_t tilt_en                      : 1;
  uint8_t pedo_en                      : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_en_a_t;

#define ST1VAFE3BX_EMB_FUNC_EN_B                        0x5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_en                       : 1;
  uint8_t not_used0                    : 3;
  uint8_t mlc_en                       : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t mlc_en                       : 1;
  uint8_t not_used0                    : 3;
  uint8_t fsm_en                       : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_en_b_t;

#define ST1VAFE3BX_EMB_FUNC_EXEC_STATUS                 0x7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t emb_func_endop               : 1;
  uint8_t emb_func_exec_ovr            : 1;
  uint8_t not_used0                    : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 6;
  uint8_t emb_func_exec_ovr            : 1;
  uint8_t emb_func_endop               : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_exec_status_t;

#define ST1VAFE3BX_PAGE_ADDRESS                         0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_addr                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_addr                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_page_address_t;

#define ST1VAFE3BX_PAGE_VALUE                           0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_value                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_value                   : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_page_value_t;

#define ST1VAFE3BX_EMB_FUNC_INT                         0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t int_step_det                 : 1;
  uint8_t int_tilt                     : 1;
  uint8_t int_sig_mot                  : 1;
  uint8_t not_used1                    : 1;
  uint8_t int_fsm_lc                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_fsm_lc                   : 1;
  uint8_t not_used1                    : 1;
  uint8_t int_sig_mot                  : 1;
  uint8_t int_tilt                     : 1;
  uint8_t int_step_det                 : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_int_t;

#define ST1VAFE3BX_FSM_INT                              0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_fsm1                     : 1;
  uint8_t int_fsm2                     : 1;
  uint8_t int_fsm3                     : 1;
  uint8_t int_fsm4                     : 1;
  uint8_t int_fsm5                     : 1;
  uint8_t int_fsm6                     : 1;
  uint8_t int_fsm7                     : 1;
  uint8_t int_fsm8                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_fsm8                     : 1;
  uint8_t int_fsm7                     : 1;
  uint8_t int_fsm6                     : 1;
  uint8_t int_fsm5                     : 1;
  uint8_t int_fsm4                     : 1;
  uint8_t int_fsm3                     : 1;
  uint8_t int_fsm2                     : 1;
  uint8_t int_fsm1                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_int_t;

#define ST1VAFE3BX_MLC_INT                              0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_mlc1                     : 1;
  uint8_t int_mlc2                     : 1;
  uint8_t int_mlc3                     : 1;
  uint8_t int_mlc4                     : 1;
  uint8_t not_used0                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t int_mlc4                     : 1;
  uint8_t int_mlc3                     : 1;
  uint8_t int_mlc2                     : 1;
  uint8_t int_mlc1                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc_int_t;

#define ST1VAFE3BX_EMB_FUNC_STATUS                      0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t is_step_det                  : 1;
  uint8_t is_tilt                      : 1;
  uint8_t is_sigmot                    : 1;
  uint8_t not_used1                    : 1;
  uint8_t is_fsm_lc                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                    : 1;
  uint8_t not_used1                    : 1;
  uint8_t is_sigmot                    : 1;
  uint8_t is_tilt                      : 1;
  uint8_t is_step_det                  : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_status_t;

#define ST1VAFE3BX_FSM_STATUS                           0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                      : 1;
  uint8_t is_fsm2                      : 1;
  uint8_t is_fsm3                      : 1;
  uint8_t is_fsm4                      : 1;
  uint8_t is_fsm5                      : 1;
  uint8_t is_fsm6                      : 1;
  uint8_t is_fsm7                      : 1;
  uint8_t is_fsm8                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                      : 1;
  uint8_t is_fsm7                      : 1;
  uint8_t is_fsm6                      : 1;
  uint8_t is_fsm5                      : 1;
  uint8_t is_fsm4                      : 1;
  uint8_t is_fsm3                      : 1;
  uint8_t is_fsm2                      : 1;
  uint8_t is_fsm1                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_status_t;

#define ST1VAFE3BX_MLC_STATUS                           0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1                      : 1;
  uint8_t is_mlc2                      : 1;
  uint8_t is_mlc3                      : 1;
  uint8_t is_mlc4                      : 1;
  uint8_t not_used0                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t is_mlc4                      : 1;
  uint8_t is_mlc3                      : 1;
  uint8_t is_mlc2                      : 1;
  uint8_t is_mlc1                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc_status_t;

#define ST1VAFE3BX_PAGE_RW                              0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 5;
  uint8_t page_read                    : 1;
  uint8_t page_write                   : 1;
  uint8_t emb_func_lir                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_lir                 : 1;
  uint8_t page_write                   : 1;
  uint8_t page_read                    : 1;
  uint8_t not_used0                    : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_page_rw_t;

#define ST1VAFE3BX_EMB_FUNC_FIFO_EN                     0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step_counter_fifo_en         : 1;
  uint8_t mlc_fifo_en                  : 1;
  uint8_t mlc_filter_feature_fifo_en   : 1;
  uint8_t fsm_fifo_en                  : 1;
  uint8_t not_used0                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t fsm_fifo_en                  : 1;
  uint8_t mlc_filter_feature_fifo_en   : 1;
  uint8_t mlc_fifo_en                  : 1;
  uint8_t step_counter_fifo_en         : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_fifo_en_t;

#define ST1VAFE3BX_FSM_ENABLE                           0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm1_en                      : 1;
  uint8_t fsm2_en                      : 1;
  uint8_t fsm3_en                      : 1;
  uint8_t fsm4_en                      : 1;
  uint8_t fsm5_en                      : 1;
  uint8_t fsm6_en                      : 1;
  uint8_t fsm7_en                      : 1;
  uint8_t fsm8_en                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm8_en                      : 1;
  uint8_t fsm7_en                      : 1;
  uint8_t fsm6_en                      : 1;
  uint8_t fsm5_en                      : 1;
  uint8_t fsm4_en                      : 1;
  uint8_t fsm3_en                      : 1;
  uint8_t fsm2_en                      : 1;
  uint8_t fsm1_en                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_enable_t;

#define ST1VAFE3BX_FSM_LONG_COUNTER_L                   0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                       : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_long_counter_l_t;

#define ST1VAFE3BX_FSM_LONG_COUNTER_H                   0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                       : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                       : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_long_counter_h_t;

#define ST1VAFE3BX_INT_ACK_MASK                         0x1FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t iack_mask0                   : 1;
  uint8_t iack_mask1                   : 1;
  uint8_t iack_mask2                   : 1;
  uint8_t iack_mask3                   : 1;
  uint8_t iack_mask4                   : 1;
  uint8_t iack_mask5                   : 1;
  uint8_t iack_mask6                   : 1;
  uint8_t iack_mask7                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t iack_mask7                   : 1;
  uint8_t iack_mask6                   : 1;
  uint8_t iack_mask5                   : 1;
  uint8_t iack_mask4                   : 1;
  uint8_t iack_mask3                   : 1;
  uint8_t iack_mask2                   : 1;
  uint8_t iack_mask1                   : 1;
  uint8_t iack_mask0                   : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_int_ack_mask_t;

#define ST1VAFE3BX_FSM_OUTS1                            0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs1_t;

#define ST1VAFE3BX_FSM_OUTS2                            0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs2_t;

#define ST1VAFE3BX_FSM_OUTS3                            0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs3_t;

#define ST1VAFE3BX_FSM_OUTS4                            0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs4_t;

#define ST1VAFE3BX_FSM_OUTS5                            0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs5_t;

#define ST1VAFE3BX_FSM_OUTS6                            0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs6_t;

#define ST1VAFE3BX_FSM_OUTS7                            0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs7_t;

#define ST1VAFE3BX_FSM_OUTS8                            0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t n_v                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_x                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_x                          : 1;
  uint8_t n_x                          : 1;
  uint8_t p_y                          : 1;
  uint8_t n_y                          : 1;
  uint8_t p_z                          : 1;
  uint8_t n_z                          : 1;
  uint8_t p_v                          : 1;
  uint8_t n_v                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_outs8_t;

#define ST1VAFE3BX_STEP_COUNTER_L                       0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_step_counter_l_t;

#define ST1VAFE3BX_STEP_COUNTER_H                       0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_step_counter_h_t;

#define ST1VAFE3BX_EMB_FUNC_SRC                         0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t stepcounter_bit_set          : 1;
  uint8_t step_overflow                : 1;
  uint8_t step_count_delta_ia          : 1;
  uint8_t step_detected                : 1;
  uint8_t not_used1                    : 1;
  uint8_t pedo_rst_step                : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pedo_rst_step                : 1;
  uint8_t not_used1                    : 1;
  uint8_t step_detected                : 1;
  uint8_t step_count_delta_ia          : 1;
  uint8_t step_overflow                : 1;
  uint8_t stepcounter_bit_set          : 1;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_src_t;

#define ST1VAFE3BX_EMB_FUNC_INIT_A                      0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t step_det_init                : 1;
  uint8_t tilt_init                    : 1;
  uint8_t sig_mot_init                 : 1;
  uint8_t not_used1                    : 1;
  uint8_t mlc_before_fsm_init          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_before_fsm_init          : 1;
  uint8_t not_used1                    : 1;
  uint8_t sig_mot_init                 : 1;
  uint8_t tilt_init                    : 1;
  uint8_t step_det_init                : 1;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_init_a_t;

#define ST1VAFE3BX_EMB_FUNC_INIT_B                      0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_init                     : 1;
  uint8_t not_used0                    : 3;
  uint8_t mlc_init                     : 1;
  uint8_t not_used1                    : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 3;
  uint8_t mlc_init                     : 1;
  uint8_t not_used0                    : 3;
  uint8_t fsm_init                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_emb_func_init_b_t;

#define ST1VAFE3BX_MLC1_SRC                             0x34U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc1_src                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc1_src                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc1_src_t;

#define ST1VAFE3BX_MLC2_SRC                             0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc2_src                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc2_src                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc2_src_t;

#define ST1VAFE3BX_MLC3_SRC                             0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc3_src                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc3_src                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc3_src_t;

#define ST1VAFE3BX_MLC4_SRC                             0x37U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc4_src                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc4_src                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc4_src_t;

#define ST1VAFE3BX_FSM_ODR                              0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 3;
  uint8_t fsm_odr                      : 3;
  uint8_t not_used1                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 2;
  uint8_t fsm_odr                      : 3;
  uint8_t not_used0                    : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_odr_t;

#define ST1VAFE3BX_MLC_ODR                              0x3AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 4;
  uint8_t mlc_odr                      : 3;
  uint8_t not_used1                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 1;
  uint8_t mlc_odr                      : 3;
  uint8_t not_used0                    : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_mlc_odr_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page pg0_emb_adv
  * @{
  *
  */
#define ST1VAFE3BX_EMB_ADV_PG_0                         0x000U

#define ST1VAFE3BX_FSM_LC_TIMEOUT_L                     0x54U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_lc_timeout_l_t;

#define ST1VAFE3BX_FSM_LC_TIMEOUT_H                     0x55U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_lc_timeout_h_t;

#define ST1VAFE3BX_FSM_PROGRAMS                         0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_n_prog                   : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_n_prog                   : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_programs_t;

#define ST1VAFE3BX_FSM_START_ADD_L                      0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_start_add_l_t;

#define ST1VAFE3BX_FSM_START_ADD_H                      0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_fsm_start_add_h_t;

#define ST1VAFE3BX_PEDO_CMD_REG                         0x5DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                    : 2;
  uint8_t fp_rejection_en              : 1;
  uint8_t carry_count_en               : 1;
  uint8_t not_used1                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                    : 4;
  uint8_t carry_count_en               : 1;
  uint8_t fp_rejection_en              : 1;
  uint8_t not_used0                    : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_pedo_cmd_reg_t;

#define ST1VAFE3BX_PEDO_DEB_STEPS_CONF                  0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t deb_step                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t deb_step                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_pedo_deb_steps_conf_t;

#define ST1VAFE3BX_PEDO_SC_DELTAT_L                     0xAAU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_pedo_sc_deltat_l_t;

#define ST1VAFE3BX_PEDO_SC_DELTAT_H                     0xABU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_pedo_sc_deltat_h_t;

#define ST1VAFE3BX_AH_BIO_SENSITIVITY_L                 0xB6U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ah_bio_sensitivity_l_t;

#define ST1VAFE3BX_AH_BIO_SENSITIVITY_H                 0xB7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_ah_bio_sensitivity_h_t;

#define ST1VAFE3BX_SMART_POWER_CTRL                     0xD2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t smart_power_ctrl_win         : 4;
  uint8_t smart_power_ctrl_dur         : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t smart_power_ctrl_dur         : 4;
  uint8_t smart_power_ctrl_win         : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe3bx_smart_power_ctrl_t;

/**
  * @}
  *
  */

typedef union
{
  st1vafe3bx_ext_clk_cfg_t                 ext_clk_cfg;
  st1vafe3bx_pin_ctrl_t                    pin_ctrl;
  st1vafe3bx_wake_up_dur_ext_t             wake_up_dur_ext;
  st1vafe3bx_who_am_i_t                    who_am_i;
  st1vafe3bx_ctrl1_t                       ctrl1;
  st1vafe3bx_ctrl2_t                       ctrl2;
  st1vafe3bx_ctrl3_t                       ctrl3;
  st1vafe3bx_ctrl4_t                       ctrl4;
  st1vafe3bx_ctrl5_t                       ctrl5;
  st1vafe3bx_fifo_ctrl_t                   fifo_ctrl;
  st1vafe3bx_fifo_wtm_t                    fifo_wtm;
  st1vafe3bx_interrupt_cfg_t               interrupt_cfg;
  st1vafe3bx_sixd_t                        sixd;
  st1vafe3bx_wake_up_ths_t                 wake_up_ths;
  st1vafe3bx_wake_up_dur_t                 wake_up_dur;
  st1vafe3bx_free_fall_t                   free_fall;
  st1vafe3bx_md1_cfg_t                     md1_cfg;
  st1vafe3bx_wake_up_src_t                 wake_up_src;
  st1vafe3bx_tap_src_t                     tap_src;
  st1vafe3bx_sixd_src_t                    sixd_src;
  st1vafe3bx_all_int_src_t                 all_int_src;
  st1vafe3bx_status_register_t             status;
  st1vafe3bx_fifo_status1_t                fifo_status1;
  st1vafe3bx_fifo_status2_t                fifo_status2;
  st1vafe3bx_out_x_l_t                     out_x_l;
  st1vafe3bx_out_x_h_t                     out_x_h;
  st1vafe3bx_out_y_l_t                     out_y_l;
  st1vafe3bx_out_y_h_t                     out_y_h;
  st1vafe3bx_out_z_l_t                     out_z_l;
  st1vafe3bx_out_z_h_t                     out_z_h;
  st1vafe3bx_out_ah_bio_l_t                out_ah_bio_l;
  st1vafe3bx_out_ah_bio_h_t                out_ah_bio_h;
  st1vafe3bx_ah_bio_cfg1_t                 ah_bio_cfg1;
  st1vafe3bx_ah_bio_cfg2_t                 ah_bio_cfg2;
  st1vafe3bx_ah_bio_cfg3_t                 ah_bio_cfg3;
  st1vafe3bx_i3c_if_ctrl_t                 i3c_if_ctrl;
  st1vafe3bx_emb_func_status_mainpage_t    emb_func_status_mainpage;
  st1vafe3bx_fsm_status_mainpage_t         fsm_status_mainpage;
  st1vafe3bx_mlc_status_mainpage_t         mlc_status_mainpage;
  st1vafe3bx_en_device_config_t            en_device_config;
  st1vafe3bx_func_cfg_access_t             func_cfg_access;
  st1vafe3bx_fifo_data_out_tag_t           fifo_data_out_tag;
  st1vafe3bx_fifo_data_out_x_l_t           fifo_data_out_x_l;
  st1vafe3bx_fifo_data_out_x_h_t           fifo_data_out_x_h;
  st1vafe3bx_fifo_data_out_y_l_t           fifo_data_out_y_l;
  st1vafe3bx_fifo_data_out_y_h_t           fifo_data_out_y_h;
  st1vafe3bx_fifo_data_out_z_l_t           fifo_data_out_z_l;
  st1vafe3bx_fifo_data_out_z_h_t           fifo_data_out_z_h;
  st1vafe3bx_fifo_batch_dec_t              fifo_batch_dec;
  st1vafe3bx_tap_cfg0_t                    tap_cfg0;
  st1vafe3bx_tap_cfg1_t                    tap_cfg1;
  st1vafe3bx_tap_cfg2_t                    tap_cfg2;
  st1vafe3bx_tap_cfg3_t                    tap_cfg3;
  st1vafe3bx_tap_cfg4_t                    tap_cfg4;
  st1vafe3bx_tap_cfg5_t                    tap_cfg5;
  st1vafe3bx_tap_cfg6_t                    tap_cfg6;
  st1vafe3bx_timestamp0_t                  timestamp0;
  st1vafe3bx_timestamp1_t                  timestamp1;
  st1vafe3bx_timestamp2_t                  timestamp2;
  st1vafe3bx_timestamp3_t                  timestamp3;
  st1vafe3bx_page_sel_t                    page_sel;
  st1vafe3bx_emb_func_en_a_t               emb_func_en_a;
  st1vafe3bx_emb_func_en_b_t               emb_func_en_b;
  st1vafe3bx_emb_func_exec_status_t        emb_func_exec_status;
  st1vafe3bx_page_address_t                page_address;
  st1vafe3bx_page_value_t                  page_value;
  st1vafe3bx_emb_func_int_t                emb_func_int;
  st1vafe3bx_fsm_int_t                     fsm_int;
  st1vafe3bx_mlc_int_t                     mlc_int;
  st1vafe3bx_emb_func_status_t             emb_func_status;
  st1vafe3bx_fsm_status_t                  fsm_status;
  st1vafe3bx_mlc_status_t                  mlc_status;
  st1vafe3bx_page_rw_t                     page_rw;
  st1vafe3bx_emb_func_fifo_en_t            emb_func_fifo_en;
  st1vafe3bx_fsm_enable_t                  fsm_enable;
  st1vafe3bx_fsm_long_counter_l_t          fsm_long_counter_l;
  st1vafe3bx_fsm_long_counter_h_t          fsm_long_counter_h;
  st1vafe3bx_int_ack_mask_t                int_ack_mask;
  st1vafe3bx_fsm_outs1_t                   fsm_outs1;
  st1vafe3bx_fsm_outs2_t                   fsm_outs2;
  st1vafe3bx_fsm_outs3_t                   fsm_outs3;
  st1vafe3bx_fsm_outs4_t                   fsm_outs4;
  st1vafe3bx_fsm_outs5_t                   fsm_outs5;
  st1vafe3bx_fsm_outs6_t                   fsm_outs6;
  st1vafe3bx_fsm_outs7_t                   fsm_outs7;
  st1vafe3bx_fsm_outs8_t                   fsm_outs8;
  st1vafe3bx_step_counter_l_t              step_counter_l;
  st1vafe3bx_step_counter_h_t              step_counter_h;
  st1vafe3bx_emb_func_src_t                emb_func_src;
  st1vafe3bx_emb_func_init_a_t             emb_func_init_a;
  st1vafe3bx_emb_func_init_b_t             emb_func_init_b;
  st1vafe3bx_mlc1_src_t                    mlc1_src;
  st1vafe3bx_mlc2_src_t                    mlc2_src;
  st1vafe3bx_mlc3_src_t                    mlc3_src;
  st1vafe3bx_mlc4_src_t                    mlc4_src;
  st1vafe3bx_fsm_odr_t                     fsm_odr;
  st1vafe3bx_mlc_odr_t                     mlc_odr;
  st1vafe3bx_fsm_lc_timeout_l_t            fsm_lc_timeout_l;
  st1vafe3bx_fsm_lc_timeout_h_t            fsm_lc_timeout_h;
  st1vafe3bx_fsm_programs_t                fsm_programs;
  st1vafe3bx_fsm_start_add_l_t             fsm_start_add_l;
  st1vafe3bx_fsm_start_add_h_t             fsm_start_add_h;
  st1vafe3bx_pedo_cmd_reg_t                pedo_cmd_reg;
  st1vafe3bx_pedo_deb_steps_conf_t         pedo_deb_steps_conf;
  st1vafe3bx_pedo_sc_deltat_l_t            pedo_sc_deltat_l;
  st1vafe3bx_pedo_sc_deltat_h_t            pedo_sc_deltat_h;
  st1vafe3bx_ah_bio_sensitivity_l_t        ah_bio_sensitivity_l;
  st1vafe3bx_ah_bio_sensitivity_h_t        ah_bio_sensitivity_h;
  st1vafe3bx_smart_power_ctrl_t            smart_power_ctrl;
  bitwise_t    bitwise;
  uint8_t    byte;
} st1vafe3bx_reg_t;

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

int32_t st1vafe3bx_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                            uint8_t *data,
                            uint16_t len);
int32_t st1vafe3bx_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                             uint8_t *data,
                             uint16_t len);

float_t st1vafe3bx_from_fs2g_to_mg(int16_t lsb);
float_t st1vafe3bx_from_fs4g_to_mg(int16_t lsb);
float_t st1vafe3bx_from_fs8g_to_mg(int16_t lsb);
float_t st1vafe3bx_from_fs16g_to_mg(int16_t lsb);
float_t st1vafe3bx_from_lsb_to_celsius(int16_t lsb);
float_t st1vafe3bx_from_lsb_to_mv(int16_t lsb);

int32_t st1vafe3bx_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE3BX_SENSOR_ONLY_ON     = 0x00, /* Initialize the driver for sensor usage */
  ST1VAFE3BX_BOOT               = 0x01, /* Restore calib. param. (it takes 10ms) */
  ST1VAFE3BX_RESET              = 0x02, /* Reset configuration registers */
  ST1VAFE3BX_SENSOR_EMB_FUNC_ON = 0x03, /* Initialize the driver for sensor and/or
                                           embedded functions usage (it takes 10ms) */
  ST1VAFE3BX_VAFE_ONLY_LP       = 0x04, /* Enable sensor in vAFE only mode - low performance */
  ST1VAFE3BX_VAFE_ONLY_HP       = 0x05, /* Enable sensor in vAFE only mode - high performance */
} st1vafe3bx_init_t;
int32_t st1vafe3bx_init_set(const stmdev_ctx_t *ctx, st1vafe3bx_init_t val);

typedef struct
{
  uint8_t pwr_en                       : 1; /* Smart power enable */
  uint8_t pwr_ctrl_win                 : 4; /* Number of consecutive windows */
  uint8_t pwr_ctrl_dur                 : 4; /* Duration threshold */
} st1vafe3bx_smart_power_t;
int32_t st1vafe3bx_smart_power_set(const stmdev_ctx_t *ctx, st1vafe3bx_smart_power_t val);
int32_t st1vafe3bx_smart_power_get(const stmdev_ctx_t *ctx, st1vafe3bx_smart_power_t *val);

typedef struct
{
  uint8_t sw_reset                     : 1; /* Restoring configuration registers */
  uint8_t boot                         : 1; /* Restoring calibration parameters */
  uint8_t drdy                         : 1; /* Accelerometer data ready */
  uint8_t power_down                   : 1; /* Monitors power-down. */
} st1vafe3bx_status_t;
int32_t st1vafe3bx_status_get(const stmdev_ctx_t *ctx, st1vafe3bx_status_t *val);
int32_t st1vafe3bx_drdy_status_get(const stmdev_ctx_t *ctx, st1vafe3bx_status_t *val);

typedef struct
{
  uint8_t is_step_det                  : 1; /* Step detected */
  uint8_t is_tilt                      : 1; /* Tilt detected */
  uint8_t is_sigmot                    : 1; /* Significant motion detected */
  uint8_t is_fsm_lc                    : 1; /* FSM long counter timeout */
} st1vafe3bx_embedded_status_t;
int32_t st1vafe3bx_embedded_status_get(const stmdev_ctx_t *ctx, st1vafe3bx_embedded_status_t *val);

typedef enum
{
  ST1VAFE3BX_DRDY_LATCHED = 0x0,
  ST1VAFE3BX_DRDY_PULSED  = 0x1,
} st1vafe3bx_data_ready_mode_t;
int32_t st1vafe3bx_data_ready_mode_set(const stmdev_ctx_t *ctx, st1vafe3bx_data_ready_mode_t val);
int32_t st1vafe3bx_data_ready_mode_get(const stmdev_ctx_t *ctx, st1vafe3bx_data_ready_mode_t *val);

typedef enum
{
  ST1VAFE3BX_OFF               = 0x00, /* in power down */
  ST1VAFE3BX_1Hz6_ULP          = 0x01, /* @1Hz6 (ultra low power) */
  ST1VAFE3BX_3Hz_ULP           = 0x02, /* @3Hz (ultra low power) */
  ST1VAFE3BX_25Hz_ULP          = 0x03, /* @25Hz (ultra low power) */
  ST1VAFE3BX_6Hz_LP            = 0x04, /* @6Hz (low power) */
  ST1VAFE3BX_12Hz5_LP          = 0x05, /* @12Hz5 (low power) */
  ST1VAFE3BX_25Hz_LP           = 0x06, /* @25Hz  (low power ) */
  ST1VAFE3BX_50Hz_LP           = 0x07, /* @50Hz  (low power) */
  ST1VAFE3BX_100Hz_LP          = 0x08, /* @100Hz (low power) */
  ST1VAFE3BX_200Hz_LP          = 0x09, /* @200Hz (low power) */
  ST1VAFE3BX_400Hz_LP          = 0x0A, /* @400Hz (low power) */
  ST1VAFE3BX_800Hz_LP          = 0x0B, /* @800Hz (low power) */
  ST1VAFE3BX_6Hz_HP            = 0x14, /* @6Hz (high performance) */
  ST1VAFE3BX_12Hz5_HP          = 0x15, /* @12Hz5 (high performance) */
  ST1VAFE3BX_25Hz_HP           = 0x16, /* @25Hz  (high performance ) */
  ST1VAFE3BX_50Hz_HP           = 0x17, /* @50Hz  (high performance) */
  ST1VAFE3BX_100Hz_HP          = 0x18, /* @100Hz (high performance) */
  ST1VAFE3BX_200Hz_HP          = 0x19, /* @200Hz (high performance) */
  ST1VAFE3BX_400Hz_HP          = 0x1A, /* @400Hz (high performance) */
  ST1VAFE3BX_800Hz_HP          = 0x1B, /* @800Hz (high performance) */
  ST1VAFE3BX_TRIG_PIN          = 0x2E, /* Single-shot high latency by INT2 */
  ST1VAFE3BX_TRIG_SW           = 0x2F, /* Single-shot high latency by IF */
  ST1VAFE3BX_3200Hz_VAFE_LP    = 0x2B, /* @3200Hz (vAFE only low power) */
  ST1VAFE3BX_800Hz_VAFE_HP     = 0x3B, /* @800Hz (vAFE only high performance) */
} st1vafe3bx_odr_t;

typedef enum
{
  ST1VAFE3BX_2g   = 0,
  ST1VAFE3BX_4g   = 1,
  ST1VAFE3BX_8g   = 2,
  ST1VAFE3BX_16g  = 3,
} st1vafe3bx_fs_t;

typedef enum
{
  ST1VAFE3BX_BW_VAFE_45Hz      = 0,
  ST1VAFE3BX_BW_VAFE_90Hz      = 1,
  ST1VAFE3BX_BW_VAFE_180Hz     = 2,
  ST1VAFE3BX_BW_VAFE_360Hz     = 3,
  ST1VAFE3BX_BW_VAFE_700Hz     = 4,
  ST1VAFE3BX_BW_VAFE_1600Hz    = 5,
  ST1VAFE3BX_BW_ODR_div_2      = 7,
  ST1VAFE3BX_BW_ODR_div_4      = 8,
  ST1VAFE3BX_BW_ODR_div_8      = 9,
  ST1VAFE3BX_BW_ODR_div_16     = 10,
  ST1VAFE3BX_BW_LP_3Hz         = 11,
  ST1VAFE3BX_BW_LP_6Hz         = 12,
  ST1VAFE3BX_BW_LP_12Hz5       = 13,
} st1vafe3bx_bw_t;

typedef struct
{
  /* if the hp_en bit is set the AH / vAFE data are in 14-bit format */
  uint8_t hp_en                        : 1;
  st1vafe3bx_fs_t fs                 : 2;
  st1vafe3bx_odr_t odr;
  st1vafe3bx_bw_t bw;
} st1vafe3bx_md_t;
int32_t st1vafe3bx_mode_set(const stmdev_ctx_t *ctx,
                            const st1vafe3bx_md_t *val);
int32_t st1vafe3bx_mode_get(const stmdev_ctx_t *ctx, st1vafe3bx_md_t *val);

int32_t st1vafe3bx_trigger_sw(const stmdev_ctx_t *ctx,
                              const st1vafe3bx_md_t *md);

typedef struct
{
  uint8_t drdy                         : 1;
  uint8_t timestamp                    : 1;
  uint8_t free_fall                    : 1;
  uint8_t wake_up                      : 1;
  uint8_t wake_up_z                    : 1;
  uint8_t wake_up_y                    : 1;
  uint8_t wake_up_x                    : 1;
  uint8_t single_tap                   : 1;
  uint8_t double_tap                   : 1;
  uint8_t triple_tap                   : 1;
  uint8_t six_d                        : 1;
  uint8_t six_d_xl                     : 1;
  uint8_t six_d_xh                     : 1;
  uint8_t six_d_yl                     : 1;
  uint8_t six_d_yh                     : 1;
  uint8_t six_d_zl                     : 1;
  uint8_t six_d_zh                     : 1;
  uint8_t sleep_change                 : 1;
  uint8_t sleep_state                  : 1;
  uint8_t tilt                         : 1;
  uint8_t fifo_bdr                     : 1;
  uint8_t fifo_full                    : 1;
  uint8_t fifo_ovr                     : 1;
  uint8_t fifo_th                      : 1;
} st1vafe3bx_all_sources_t;
int32_t st1vafe3bx_all_sources_get(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_all_sources_t *val);

typedef struct
{
  float_t mg[3];
  int16_t raw[3];
} st1vafe3bx_xl_data_t;
int32_t st1vafe3bx_xl_data_get(const stmdev_ctx_t *ctx,
                               const st1vafe3bx_md_t *md,
                               st1vafe3bx_xl_data_t *data);

typedef struct
{
  float_t mv;
  int16_t raw;
} st1vafe3bx_ah_bio_data_t;
int32_t st1vafe3bx_ah_bio_data_get(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_ah_bio_data_t *data);

typedef enum
{
  ST1VAFE3BX_XL_ST_DISABLE  = 0x0,
  ST1VAFE3BX_XL_ST_POSITIVE = 0x1,
  ST1VAFE3BX_XL_ST_NEGATIVE = 0x2,
} st1vafe3bx_xl_self_test_t;
int32_t st1vafe3bx_self_test_sign_set(const stmdev_ctx_t *ctx,
                                      st1vafe3bx_xl_self_test_t val);
int32_t st1vafe3bx_self_test_start(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_self_test_stop(const stmdev_ctx_t *ctx);

int32_t st1vafe3bx_enter_deep_power_down(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_exit_deep_power_down(const stmdev_ctx_t *ctx);

typedef struct
{
  enum
  {
    ST1VAFE3BX_I3C_BUS_AVAIL_TIME_20US = 0x0,
    ST1VAFE3BX_I3C_BUS_AVAIL_TIME_50US = 0x1,
    ST1VAFE3BX_I3C_BUS_AVAIL_TIME_1MS  = 0x2,
    ST1VAFE3BX_I3C_BUS_AVAIL_TIME_25MS = 0x3,
  } bus_act_sel;
  uint8_t asf_on                       : 1;
  uint8_t drstdaa_en                   : 1;
} st1vafe3bx_i3c_cfg_t;
int32_t st1vafe3bx_i3c_configure_set(const stmdev_ctx_t *ctx,
                                     const st1vafe3bx_i3c_cfg_t *val);
int32_t st1vafe3bx_i3c_configure_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_i3c_cfg_t *val);

typedef enum
{
  ST1VAFE3BX_MAIN_MEM_BANK       = 0x0,
  ST1VAFE3BX_EMBED_FUNC_MEM_BANK = 0x1,
} st1vafe3bx_mem_bank_t;
int32_t st1vafe3bx_mem_bank_set(const stmdev_ctx_t *ctx,
                                st1vafe3bx_mem_bank_t val);
int32_t st1vafe3bx_mem_bank_get(const stmdev_ctx_t *ctx,
                                st1vafe3bx_mem_bank_t *val);

int32_t st1vafe3bx_ln_pg_write(const stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len);
int32_t st1vafe3bx_ln_pg_read(const stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *buf, uint8_t len);

int32_t st1vafe3bx_ext_clk_en_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_ext_clk_en_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  uint8_t sdo_pull_up                  : 1; /* 1 = pull up enable */
  uint8_t sda_pull_up                  : 1; /* 1 = pull up enable */
  uint8_t cs_pull_up                   : 1; /* 1 = pull up enable */
  uint8_t int_push_pull                : 1; /* 1 = push-pull / 0 = open-drain*/
} st1vafe3bx_pin_conf_t;
int32_t st1vafe3bx_pin_conf_set(const stmdev_ctx_t *ctx,
                                const st1vafe3bx_pin_conf_t *val);
int32_t st1vafe3bx_pin_conf_get(const stmdev_ctx_t *ctx,
                                st1vafe3bx_pin_conf_t *val);

typedef enum
{
  ST1VAFE3BX_ACTIVE_HIGH = 0x0,
  ST1VAFE3BX_ACTIVE_LOW  = 0x1,
} st1vafe3bx_int_pin_polarity_t;
int32_t st1vafe3bx_int_pin_polarity_set(const stmdev_ctx_t *ctx,
                                        st1vafe3bx_int_pin_polarity_t val);
int32_t st1vafe3bx_int_pin_polarity_get(const stmdev_ctx_t *ctx,
                                        st1vafe3bx_int_pin_polarity_t *val);

typedef enum
{
  ST1VAFE3BX_SPI_4_WIRE  = 0x0, /* SPI 4 wires */
  ST1VAFE3BX_SPI_3_WIRE  = 0x1, /* SPI 3 wires */
} st1vafe3bx_spi_mode;
int32_t st1vafe3bx_spi_mode_set(const stmdev_ctx_t *ctx,
                                st1vafe3bx_spi_mode val);
int32_t st1vafe3bx_spi_mode_get(const stmdev_ctx_t *ctx,
                                st1vafe3bx_spi_mode *val);

typedef struct
{
  uint8_t drdy                         : 1; /* Accelerometer data ready */
  uint8_t boot                         : 1; /* Restoring calibration parameters */
  uint8_t fifo_th                      : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr                     : 1; /* FIFO overrun */
  uint8_t fifo_full                    : 1; /* FIFO full */
  uint8_t free_fall                    : 1; /* free fall event */
  uint8_t six_d                        : 1; /* orientation change (6D/4D detection) */
  uint8_t tap                          : 1; /* all tap event */
  uint8_t wake_up                      : 1; /* wake up event */
  uint8_t sleep_change                 : 1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t emb_function                 : 1; /* Embedded Function */
  uint8_t timestamp                    : 1; /* Timestamp */
} st1vafe3bx_pin_int_route_t;
int32_t st1vafe3bx_pin_int_route_set(const stmdev_ctx_t *ctx,
                                     const st1vafe3bx_pin_int_route_t *val);
int32_t st1vafe3bx_pin_int_route_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_pin_int_route_t *val);

typedef struct
{
  uint8_t step_det                     : 1; /* route step detection event on INT pad */
  uint8_t tilt                         : 1; /* route tilt event on INT pad */
  uint8_t sig_mot                      : 1; /* route significant motion event on INT pad */
  uint8_t fsm_lc                       : 1; /* route FSM long counter event on INT pad */
} st1vafe3bx_emb_pin_int_route_t;
int32_t st1vafe3bx_emb_pin_int_route_set(const stmdev_ctx_t *ctx,
                                         const st1vafe3bx_emb_pin_int_route_t *val);
int32_t st1vafe3bx_emb_pin_int_route_get(const stmdev_ctx_t *ctx,
                                         st1vafe3bx_emb_pin_int_route_t *val);

typedef enum
{
  ST1VAFE3BX_INT_DISABLED             = 0x0,
  ST1VAFE3BX_INT_LEVEL                = 0x1,
  ST1VAFE3BX_INT_LATCHED              = 0x2,
} st1vafe3bx_int_cfg_t;

typedef struct
{
  st1vafe3bx_int_cfg_t int_cfg;
  uint8_t sleep_status_on_int          : 1;  /* route sleep_status on interrupt */
  uint8_t dis_rst_lir_all_int          : 1;  /* disable LIR reset when reading ALL_INT_SRC */
} st1vafe3bx_int_config_t;
int32_t st1vafe3bx_int_config_set(const stmdev_ctx_t *ctx,
                                  const st1vafe3bx_int_config_t *val);
int32_t st1vafe3bx_int_config_get(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_int_config_t *val);

typedef enum
{
  ST1VAFE3BX_EMBEDDED_INT_LEVEL     = 0x0,
  ST1VAFE3BX_EMBEDDED_INT_LATCHED   = 0x1,
} st1vafe3bx_embedded_int_config_t;
int32_t st1vafe3bx_embedded_int_cfg_set(const stmdev_ctx_t *ctx,
                                        st1vafe3bx_embedded_int_config_t val);
int32_t st1vafe3bx_embedded_int_cfg_get(const stmdev_ctx_t *ctx,
                                        st1vafe3bx_embedded_int_config_t *val);

typedef struct
{
  enum st1vafe3bx_operation_t
  {
    ST1VAFE3BX_BYPASS_MODE            = 0x0,
    ST1VAFE3BX_FIFO_MODE              = 0x1,
    ST1VAFE3BX_STREAM_TO_FIFO_MODE    = 0x3,
    ST1VAFE3BX_BYPASS_TO_STREAM_MODE  = 0x4,
    ST1VAFE3BX_STREAM_MODE            = 0x6,
    ST1VAFE3BX_BYPASS_TO_FIFO_MODE    = 0x7,
    ST1VAFE3BX_FIFO_OFF               = 0x8,
  } operation;

  enum st1vafe3bx_store_t
  {
    ST1VAFE3BX_FIFO_1X                = 0,
    ST1VAFE3BX_FIFO_2X                = 1,
  } store;

  uint8_t xl_only                      : 1; /* only XL samples (16-bit) are stored in FIFO */

  uint8_t watermark                    : 7; /* (0 disable) max 127 @16bit, even and max 256 @8bit.*/
  uint8_t cfg_change_in_fifo           : 1;
  struct
  {
    enum st1vafe3bx_dec_ts_t
    {
      ST1VAFE3BX_DEC_TS_OFF             = 0x0,
      ST1VAFE3BX_DEC_TS_1               = 0x1,
      ST1VAFE3BX_DEC_TS_8               = 0x2,
      ST1VAFE3BX_DEC_TS_32              = 0x3,
    } dec_ts; /* decimation for timestamp batching*/

    enum st1vafe3bx_bdr_xl_t
    {
      ST1VAFE3BX_BDR_ODR               = 0x0,
      ST1VAFE3BX_BDR_ODR_DIV_2         = 0x1,
      ST1VAFE3BX_BDR_ODR_DIV_4         = 0x2,
      ST1VAFE3BX_BDR_ODR_DIV_8         = 0x3,
      ST1VAFE3BX_BDR_ODR_DIV_16        = 0x4,
      ST1VAFE3BX_BDR_ODR_DIV_32        = 0x5,
      ST1VAFE3BX_BDR_ODR_DIV_64        = 0x6,
      ST1VAFE3BX_BDR_ODR_OFF           = 0x7,
    } bdr_xl; /* accelerometer batch data rate*/
  } batch;
} st1vafe3bx_fifo_mode_t;
int32_t st1vafe3bx_fifo_mode_set(const stmdev_ctx_t *ctx,
                                 st1vafe3bx_fifo_mode_t val);
int32_t st1vafe3bx_fifo_mode_get(const stmdev_ctx_t *ctx,
                                 st1vafe3bx_fifo_mode_t *val);

int32_t st1vafe3bx_fifo_data_level_get(const stmdev_ctx_t *ctx, uint16_t *val);
int32_t st1vafe3bx_fifo_wtm_flag_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE3BX_FIFO_EMPTY_TAG             = 0x0,
  ST1VAFE3BX_XL_ONLY_2X_TAG             = 0x3,
  ST1VAFE3BX_TIMESTAMP_CFG_CHG_TAG      = 0x4,
  ST1VAFE3BX_STEP_COUNTER_TAG           = 0x12,
  ST1VAFE3BX_MLC_RESULT_TAG             = 0x1A,
  ST1VAFE3BX_MLC_FILTER_TAG             = 0x1B,
  ST1VAFE3BX_MLC_FEATURE_TAG            = 0x1C,
  ST1VAFE3BX_FSM_RESULT_TAG             = 0x1D,
  ST1VAFE3BX_AH_VAFE_ONLY_TAG           = 0x1E,
  ST1VAFE3BX_XL_AND_AH_VAFE1_TAG        = 0x1F,
} st1vafe3bx_fifo_sensor_tag_t;
int32_t st1vafe3bx_fifo_sensor_tag_get(const stmdev_ctx_t *ctx,
                                       st1vafe3bx_fifo_sensor_tag_t *val);

int32_t st1vafe3bx_fifo_out_raw_get(const stmdev_ctx_t *ctx, uint8_t *buff);

typedef struct
{
  uint8_t tag;
  struct
  {
    float_t mg[3];
    int16_t raw[3];
  } xl[2];
  struct
  {
    float_t mv;
    int16_t raw;
  } ah_bio;

  struct
  {
    uint32_t steps;
    uint32_t timestamp;
  } pedo;
  struct
  {
    uint8_t cfg_change                 : 1; /* 1 if ODR/BDR configuration is changed */
    uint8_t odr                        : 4; /* ODR */
    uint8_t bw                         : 2; /* BW */
    uint8_t lp_hp                      : 1; /* Power: 0 for LP, 1 for HP */
    uint8_t ah_en                      : 1; /* AH is enabled */
    uint8_t fs                         : 2; /* FS */
    uint8_t dec_ts                     : 2; /* Timestamp decimator value */
    uint8_t odr_xl_batch               : 1; /* Accelerometer ODR is batched */
    uint32_t timestamp;
  } cfg_chg;
} st1vafe3bx_fifo_data_t;
int32_t st1vafe3bx_fifo_data_get(const stmdev_ctx_t *ctx,
                                 const st1vafe3bx_md_t *md,
                                 const st1vafe3bx_fifo_mode_t *fmd,
                                 st1vafe3bx_fifo_data_t *data);


typedef struct
{
  uint8_t ah_bio_zin_dis_1                    : 1;
  uint8_t ah_bio_zin_dis_2                    : 1;

  enum
  {
    ST1VAFE3BX_DIFFERENTIAL_MODE            = 0x0,
    ST1VAFE3BX_SINGLE_MODE_01               = 0x1,
    ST1VAFE3BX_SINGLE_MODE_10               = 0x2,
    ST1VAFE3BX_FORCED_RESET                 = 0x3,
  } mode;

  enum
  {
    ST1VAFE3BX_100MOhm                      = 0x0,
    ST1VAFE3BX_200MOhm                      = 0x1,
    ST1VAFE3BX_500MOhm                      = 0x2,
    ST1VAFE3BX_1GOhm                        = 0x3,
  } zin;

  enum
  {
    ST1VAFE3BX_GAIN_2                       = 0x0,
    ST1VAFE3BX_GAIN_4                       = 0x1,
    ST1VAFE3BX_GAIN_8                       = 0x2,
    ST1VAFE3BX_GAIN_16                      = 0x3,
  } gain;
} st1vafe3bx_ah_bio_config_t;
int32_t st1vafe3bx_ah_bio_config_set(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_ah_bio_config_t val);
int32_t st1vafe3bx_ah_bio_config_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_ah_bio_config_t *val);

int32_t st1vafe3bx_enter_vafe_only(const stmdev_ctx_t *ctx);
int32_t st1vafe3bx_exit_vafe_only(const stmdev_ctx_t *ctx);
int32_t st1vafe3bx_ah_bio_active(const stmdev_ctx_t *ctx, uint8_t filter_on);

typedef struct
{
  uint8_t false_step_rej               : 1;
  uint8_t step_counter_enable          : 1;
  uint8_t step_counter_in_fifo         : 1;
} st1vafe3bx_stpcnt_mode_t;
int32_t st1vafe3bx_stpcnt_mode_set(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_stpcnt_mode_t val);
int32_t st1vafe3bx_stpcnt_mode_get(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_stpcnt_mode_t *val);

int32_t st1vafe3bx_stpcnt_steps_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe3bx_stpcnt_rst_step_set(const stmdev_ctx_t *ctx);

int32_t st1vafe3bx_stpcnt_debounce_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_stpcnt_debounce_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe3bx_stpcnt_period_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe3bx_stpcnt_period_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe3bx_tilt_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_tilt_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);
int32_t st1vafe3bx_sigmot_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_sigmot_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);


int32_t st1vafe3bx_ff_duration_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_ff_duration_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE3BX_156_mg = 0x0,
  ST1VAFE3BX_219_mg = 0x1,
  ST1VAFE3BX_250_mg = 0x2,
  ST1VAFE3BX_312_mg = 0x3,
  ST1VAFE3BX_344_mg = 0x4,
  ST1VAFE3BX_406_mg = 0x5,
  ST1VAFE3BX_469_mg = 0x6,
  ST1VAFE3BX_500_mg = 0x7,
} st1vafe3bx_ff_thresholds_t;
int32_t st1vafe3bx_ff_thresholds_set(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_ff_thresholds_t val);
int32_t st1vafe3bx_ff_thresholds_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_ff_thresholds_t *val);

typedef enum
{
  ST1VAFE3BX_DEG_80 = 0x0,
  ST1VAFE3BX_DEG_70 = 0x1,
  ST1VAFE3BX_DEG_60 = 0x2,
  ST1VAFE3BX_DEG_50 = 0x3,
} st1vafe3bx_threshold_t;

typedef enum
{
  ST1VAFE3BX_6D = 0x0,
  ST1VAFE3BX_4D = 0x1,
} st1vafe3bx_mode_t;

typedef struct
{
  st1vafe3bx_threshold_t threshold;
  st1vafe3bx_mode_t mode;
} st1vafe3bx_sixd_config_t;

int32_t st1vafe3bx_sixd_config_set(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_sixd_config_t val);
int32_t st1vafe3bx_sixd_config_get(const stmdev_ctx_t *ctx,
                                   st1vafe3bx_sixd_config_t *val);

typedef enum
{
  ST1VAFE3BX_0_ODR  = 0x000, /* 0 ODR time */
  ST1VAFE3BX_1_ODR  = 0x001, /* 1 ODR time */
  ST1VAFE3BX_2_ODR  = 0x002, /* 2 ODR time */
  ST1VAFE3BX_3_ODR  = 0x100, /* 3 ODR time */
  ST1VAFE3BX_7_ODR  = 0x101, /* 7 ODR time */
  ST1VAFE3BX_11_ODR = 0x102, /* 11 ODR time */
  ST1VAFE3BX_15_ODR = 0x103, /* 15 ODR time */
} st1vafe3bx_wake_dur_t;

typedef enum
{
  ST1VAFE3BX_SLEEP_OFF = 0,
  ST1VAFE3BX_SLEEP_ON  = 1,
} st1vafe3bx_wake_enable_t;

typedef enum
{
  ST1VAFE3BX_ODR_NO_CHANGE       = 0,  /* no odr change during inactivity state */
  ST1VAFE3BX_ODR_1_6_HZ          = 1,  /* set odr to 1.6Hz during inactivity state */
  ST1VAFE3BX_ODR_3_HZ            = 1,  /* set odr to 3Hz during inactivity state */
  ST1VAFE3BX_ODR_25_HZ           = 1,  /* set odr to 25Hz during inactivity state */
} st1vafe3bx_inact_odr_t;

typedef struct
{
  st1vafe3bx_wake_dur_t wake_dur;
  uint8_t sleep_dur                    : 4;       /* 1 LSB == 512 ODR time */
  uint8_t wake_ths                     : 7;       /* wakeup threshold */
  uint8_t wake_ths_weight              : 1;       /* 0: 1LSB = FS_XL/2^6, 1: 1LSB = FS_XL/2^8 */
  st1vafe3bx_wake_enable_t wake_enable;
  st1vafe3bx_inact_odr_t inact_odr;
} st1vafe3bx_wakeup_config_t;

int32_t st1vafe3bx_wakeup_config_set(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_wakeup_config_t val);
int32_t st1vafe3bx_wakeup_config_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_wakeup_config_t *val);

typedef enum
{
  ST1VAFE3BX_TAP_NONE  = 0x0, /* No axis */
  ST1VAFE3BX_TAP_ON_X  = 0x1, /* Detect tap on X axis */
  ST1VAFE3BX_TAP_ON_Y  = 0x2, /* Detect tap on Y axis */
  ST1VAFE3BX_TAP_ON_Z  = 0x3, /* Detect tap on Z axis */
} st1vafe3bx_axis_t;

typedef struct
{
  st1vafe3bx_axis_t axis;
  uint8_t inverted_peak_time           : 5; /* 1 LSB == 1 sample */
  uint8_t pre_still_ths                : 4; /* 1 LSB == 62.5 mg */
  uint8_t post_still_ths               : 4; /* 1 LSB == 62.5 mg */
  uint8_t post_still_time              : 6; /* samples num during stationary condition */
  uint8_t shock_wait_time              : 6; /* samples num during shock condition */
  uint8_t latency                      : 4; /* samples max num between taps */
  uint8_t wait_end_latency             : 1; /* wait end of latency time to generate tap events */
  uint8_t peak_ths                     : 6; /* 1 LSB == 62.5 mg */
  uint8_t rebound                      : 5; /* samples num during rebound condition */
  uint8_t pre_still_start              : 4; /* pre still start */
  uint8_t pre_still_n                  : 4; /* pre still n */
  uint8_t single_tap_on                : 1; /* enable single tap */
  uint8_t double_tap_on                : 1; /* enable double tap */
  uint8_t triple_tap_on                : 1; /* enable triple tap */
} st1vafe3bx_tap_config_t;

int32_t st1vafe3bx_tap_config_set(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_tap_config_t val);
int32_t st1vafe3bx_tap_config_get(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_tap_config_t *val);

int32_t st1vafe3bx_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe3bx_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val);

int32_t st1vafe3bx_long_cnt_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                                uint8_t *val);

int32_t st1vafe3bx_emb_fsm_en_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_emb_fsm_en_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  st1vafe3bx_fsm_enable_t fsm_enable;
} st1vafe3bx_emb_fsm_enable_t;
int32_t st1vafe3bx_fsm_enable_set(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_emb_fsm_enable_t *val);
int32_t st1vafe3bx_fsm_enable_get(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_emb_fsm_enable_t *val);

int32_t st1vafe3bx_long_cnt_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe3bx_long_cnt_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe3bx_fsm_status_get(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_fsm_status_mainpage_t *val);
int32_t st1vafe3bx_fsm_out_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE3BX_ODR_FSM_12Hz5       = 0x00,
  ST1VAFE3BX_ODR_FSM_25Hz        = 0x01,
  ST1VAFE3BX_ODR_FSM_50Hz        = 0x02,
  ST1VAFE3BX_ODR_FSM_100Hz       = 0x03,
  ST1VAFE3BX_ODR_FSM_200Hz       = 0x04,
  ST1VAFE3BX_ODR_FSM_400Hz       = 0x05,
  ST1VAFE3BX_ODR_FSM_800Hz       = 0x06,
  ST1VAFE3BX_ODR_FSM_VAFE_50Hz   = 0x10,
  ST1VAFE3BX_ODR_FSM_VAFE_100Hz  = 0x11,
  ST1VAFE3BX_ODR_FSM_VAFE_200Hz  = 0x12,
  ST1VAFE3BX_ODR_FSM_VAFE_400Hz  = 0x13,
  ST1VAFE3BX_ODR_FSM_VAFE_800Hz  = 0x14,
  ST1VAFE3BX_ODR_FSM_VAFE_1600Hz = 0x15,
} st1vafe3bx_fsm_val_odr_t;
int32_t st1vafe3bx_fsm_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_fsm_val_odr_t val);
int32_t st1vafe3bx_fsm_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_fsm_val_odr_t *val);

int32_t st1vafe3bx_fsm_init_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_fsm_init_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe3bx_fsm_fifo_en_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_fsm_fifo_en_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe3bx_long_cnt_int_value_set(const stmdev_ctx_t *ctx,
                                          uint16_t val);
int32_t st1vafe3bx_long_cnt_int_value_get(const stmdev_ctx_t *ctx,
                                          uint16_t *val);

int32_t st1vafe3bx_fsm_programs_num_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_fsm_programs_num_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe3bx_fsm_start_address_set(const stmdev_ctx_t *ctx,
                                         uint16_t val);
int32_t st1vafe3bx_fsm_start_address_get(const stmdev_ctx_t *ctx,
                                         uint16_t *val);

typedef enum
{
  ST1VAFE3BX_MLC_OFF              = 0,
  ST1VAFE3BX_MLC_ON               = 1,
  ST1VAFE3BX_MLC_ON_BEFORE_FSM    = 2,
} st1vafe3bx_mlc_mode_t;
int32_t st1vafe3bx_mlc_set(const stmdev_ctx_t *ctx, st1vafe3bx_mlc_mode_t val);
int32_t st1vafe3bx_mlc_get(const stmdev_ctx_t *ctx, st1vafe3bx_mlc_mode_t *val);

int32_t st1vafe3bx_mlc_status_get(const stmdev_ctx_t *ctx,
                                  st1vafe3bx_mlc_status_mainpage_t *val);

int32_t st1vafe3bx_mlc_out_get(const stmdev_ctx_t *ctx, uint8_t *buff);

typedef enum
{
  ST1VAFE3BX_ODR_PRGS_12Hz5       = 0x00,
  ST1VAFE3BX_ODR_PRGS_25Hz        = 0x01,
  ST1VAFE3BX_ODR_PRGS_50Hz        = 0x02,
  ST1VAFE3BX_ODR_PRGS_100Hz       = 0x03,
  ST1VAFE3BX_ODR_PRGS_200Hz       = 0x04,
  ST1VAFE3BX_ODR_PRGS_VAFE_50Hz   = 0x10,
  ST1VAFE3BX_ODR_PRGS_VAFE_100Hz  = 0x11,
  ST1VAFE3BX_ODR_PRGS_VAFE_200Hz  = 0x12,
  ST1VAFE3BX_ODR_PRGS_VAFE_400Hz  = 0x13,
  ST1VAFE3BX_ODR_PRGS_VAFE_800Hz  = 0x14,
  ST1VAFE3BX_ODR_PRGS_VAFE_1600Hz = 0x15,
} st1vafe3bx_mlc_odr_val_t;
int32_t st1vafe3bx_mlc_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_mlc_odr_val_t val);
int32_t st1vafe3bx_mlc_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe3bx_mlc_odr_val_t *val);

int32_t st1vafe3bx_mlc_fifo_en_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe3bx_mlc_fifo_en_get(const stmdev_ctx_t *ctx, uint8_t *val);

#ifdef __cplusplus
}
#endif

#endif /* ST1VAFE3BX_REGS_H */
