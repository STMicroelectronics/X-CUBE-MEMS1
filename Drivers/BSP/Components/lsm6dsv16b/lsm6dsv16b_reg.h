/**
  ******************************************************************************
  * @file    lsm6dsv16b_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          lsm6dsv16b_reg.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSV16B_REGS_H
#define LSM6DSV16B_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LSM6DSV16B
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
  uint8_t bit0                          : 1;
  uint8_t bit1                          : 1;
  uint8_t bit2                          : 1;
  uint8_t bit3                          : 1;
  uint8_t bit4                          : 1;
  uint8_t bit5                          : 1;
  uint8_t bit6                          : 1;
  uint8_t bit7                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7                          : 1;
  uint8_t bit6                          : 1;
  uint8_t bit5                          : 1;
  uint8_t bit4                          : 1;
  uint8_t bit3                          : 1;
  uint8_t bit2                          : 1;
  uint8_t bit1                          : 1;
  uint8_t bit0                          : 1;
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

/** @defgroup LSM6DSV16B_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define LSM6DSV16B_I2C_ADD_L                     0xD5U
#define LSM6DSV16B_I2C_ADD_H                     0xD7U

/** Device Identification (Who am I) **/
#define LSM6DSV16B_ID                            0x71U

/**
  * @}
  *
  */

/** @defgroup bitfields page main
  * @{
  *
  */

#define LSM6DSV16B_FUNC_CFG_ACCESS               0x1U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t sw_por                        : 1;
  uint8_t fsm_wr_ctrl_en                : 1;
  uint8_t not_used1                     : 3;
  uint8_t emb_func_reg_access           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_reg_access           : 1;
  uint8_t not_used1                     : 3;
  uint8_t fsm_wr_ctrl_en                : 1;
  uint8_t sw_por                        : 1;
  uint8_t not_used0                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_func_cfg_access_t;

#define LSM6DSV16B_PIN_CTRL                      0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 5;
  uint8_t ibhr_por_en                   : 1;
  uint8_t sdo_pu_en                     : 1;
  uint8_t tdm_wclk_pu_dis               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tdm_wclk_pu_dis               : 1;
  uint8_t sdo_pu_en                     : 1;
  uint8_t ibhr_por_en                   : 1;
  uint8_t not_used0                     : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_pin_ctrl_t;

#define LSM6DSV16B_IF_CFG                        0x3U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t i2c_i3c_disable               : 1;
  uint8_t not_used0                     : 1;
  uint8_t sim                           : 1;
  uint8_t pp_od                         : 1;
  uint8_t h_lactive                     : 1;
  uint8_t asf_ctrl                      : 1;
  uint8_t tdm_out_pu_en                 : 1;
  uint8_t sda_pu_en                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sda_pu_en                     : 1;
  uint8_t tdm_out_pu_en                 : 1;
  uint8_t asf_ctrl                      : 1;
  uint8_t h_lactive                     : 1;
  uint8_t pp_od                         : 1;
  uint8_t sim                           : 1;
  uint8_t not_used0                     : 1;
  uint8_t i2c_i3c_disable               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_if_cfg_t;

#define LSM6DSV16B_FIFO_CTRL1                    0x7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm                           : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wtm                           : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_ctrl1_t;

#define LSM6DSV16B_FIFO_CTRL2                    0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl_dualc_batch_from_fsm       : 1;
  uint8_t uncompr_rate                  : 2;
  uint8_t not_used0                     : 1;
  uint8_t odr_chg_en                    : 1;
  uint8_t not_used1                     : 1;
  uint8_t fifo_compr_rt_en              : 1;
  uint8_t stop_on_wtm                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t stop_on_wtm                   : 1;
  uint8_t fifo_compr_rt_en              : 1;
  uint8_t not_used1                     : 1;
  uint8_t odr_chg_en                    : 1;
  uint8_t not_used0                     : 1;
  uint8_t uncompr_rate                  : 2;
  uint8_t xl_dualc_batch_from_fsm       : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_ctrl2_t;

#define LSM6DSV16B_FIFO_CTRL3                    0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bdr_xl                        : 4;
  uint8_t bdr_gy                        : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bdr_gy                        : 4;
  uint8_t bdr_xl                        : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_ctrl3_t;

#define LSM6DSV16B_FIFO_CTRL4                    0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_mode                     : 3;
  uint8_t not_used0                     : 1;
  uint8_t odr_t_batch                   : 2;
  uint8_t dec_ts_batch                  : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dec_ts_batch                  : 2;
  uint8_t odr_t_batch                   : 2;
  uint8_t not_used0                     : 1;
  uint8_t fifo_mode                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_ctrl4_t;

#define LSM6DSV16B_COUNTER_BDR_REG1              0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t cnt_bdr_th                    : 2;
  uint8_t not_used0                     : 3;
  uint8_t trig_counter_bdr              : 2;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t trig_counter_bdr              : 2;
  uint8_t not_used0                     : 3;
  uint8_t cnt_bdr_th                    : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_counter_bdr_reg1_t;

#define LSM6DSV16B_COUNTER_BDR_REG2              0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t cnt_bdr_th                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t cnt_bdr_th                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_counter_bdr_reg2_t;

#define LSM6DSV16B_INT1_CTRL                     0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_drdy_xl                  : 1;
  uint8_t int1_drdy_g                   : 1;
  uint8_t not_used0                     : 1;
  uint8_t int1_fifo_th                  : 1;
  uint8_t int1_fifo_ovr                 : 1;
  uint8_t int1_fifo_full                : 1;
  uint8_t int1_cnt_bdr                  : 1;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t int1_cnt_bdr                  : 1;
  uint8_t int1_fifo_full                : 1;
  uint8_t int1_fifo_ovr                 : 1;
  uint8_t int1_fifo_th                  : 1;
  uint8_t not_used0                     : 1;
  uint8_t int1_drdy_g                   : 1;
  uint8_t int1_drdy_xl                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_int1_ctrl_t;

#define LSM6DSV16B_INT2_CTRL                     0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_drdy_xl                  : 1;
  uint8_t int2_drdy_g                   : 1;
  uint8_t not_used0                     : 1;
  uint8_t int2_fifo_th                  : 1;
  uint8_t int2_fifo_ovr                 : 1;
  uint8_t int2_fifo_full                : 1;
  uint8_t int2_cnt_bdr                  : 1;
  uint8_t int2_emb_func_endop           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_emb_func_endop           : 1;
  uint8_t int2_cnt_bdr                  : 1;
  uint8_t int2_fifo_full                : 1;
  uint8_t int2_fifo_ovr                 : 1;
  uint8_t int2_fifo_th                  : 1;
  uint8_t not_used0                     : 1;
  uint8_t int2_drdy_g                   : 1;
  uint8_t int2_drdy_xl                  : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_int2_ctrl_t;

#define LSM6DSV16B_WHO_AM_I                      0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t id                            : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t id                            : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_who_am_i_t;

#define LSM6DSV16B_CTRL1                         0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t odr_xl                        : 4;
  uint8_t op_mode_xl                    : 3;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t op_mode_xl                    : 3;
  uint8_t odr_xl                        : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl1_t;

#define LSM6DSV16B_CTRL2                         0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t odr_g                         : 4;
  uint8_t op_mode_g                     : 3;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t op_mode_g                     : 3;
  uint8_t odr_g                         : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl2_t;

#define LSM6DSV16B_CTRL3                         0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sw_reset                      : 1;
  uint8_t not_used0                     : 1;
  uint8_t if_inc                        : 1;
  uint8_t not_used1                     : 3;
  uint8_t bdu                           : 1;
  uint8_t boot                          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot                          : 1;
  uint8_t bdu                           : 1;
  uint8_t not_used1                     : 3;
  uint8_t if_inc                        : 1;
  uint8_t not_used0                     : 1;
  uint8_t sw_reset                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl3_t;

#define LSM6DSV16B_CTRL4                         0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t drdy_pulsed                   : 1;
  uint8_t int2_drdy_temp                : 1;
  uint8_t drdy_mask                     : 1;
  uint8_t int2_on_int1                  : 1;
  uint8_t not_used0                     : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t int2_on_int1                  : 1;
  uint8_t drdy_mask                     : 1;
  uint8_t int2_drdy_temp                : 1;
  uint8_t drdy_pulsed                   : 1;
  uint8_t not_used1                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl4_t;

#define LSM6DSV16B_CTRL5                         0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_en_i3c                    : 1;
  uint8_t bus_act_sel                   : 2;
  uint8_t not_used0                     : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 5;
  uint8_t bus_act_sel                   : 2;
  uint8_t int_en_i3c                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl5_t;

#define LSM6DSV16B_CTRL6                         0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs_g                          : 4;
  uint8_t lpf1_g_bw                     : 3;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t lpf1_g_bw                     : 3;
  uint8_t fs_g                          : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl6_t;

#define LSM6DSV16B_CTRL7                         0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lpf1_g_en                     : 1;
  uint8_t not_used0                     : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 7;
  uint8_t lpf1_g_en                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl7_t;

#define LSM6DSV16B_CTRL8                         0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs_xl                         : 2;
  uint8_t not_used0                     : 1;
  uint8_t xl_dualc_en                   : 1;
  uint8_t not_used1                     : 1;
  uint8_t hp_lpf2_xl_bw                 : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t hp_lpf2_xl_bw                 : 3;
  uint8_t not_used1                     : 1;
  uint8_t xl_dualc_en                   : 1;
  uint8_t not_used0                     : 1;
  uint8_t fs_xl                         : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl8_t;

#define LSM6DSV16B_CTRL9                         0x18U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t usr_off_on_out                : 1;
  uint8_t usr_off_w                     : 1;
  uint8_t not_used0                     : 1;
  uint8_t lpf2_xl_en                    : 1;
  uint8_t hp_slope_xl_en                : 1;
  uint8_t xl_fastsettl_mode             : 1;
  uint8_t hp_ref_mode_xl                : 1;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t hp_ref_mode_xl                : 1;
  uint8_t xl_fastsettl_mode             : 1;
  uint8_t hp_slope_xl_en                : 1;
  uint8_t lpf2_xl_en                    : 1;
  uint8_t not_used0                     : 1;
  uint8_t usr_off_w                     : 1;
  uint8_t usr_off_on_out                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl9_t;

#define LSM6DSV16B_CTRL10                        0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_xl                         : 2;
  uint8_t st_g                          : 2;
  uint8_t xl_st_offset                  : 1;
  uint8_t not_used1                     : 1;
  uint8_t emb_func_debug                : 1;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t emb_func_debug                : 1;
  uint8_t not_used1                     : 1;
  uint8_t xl_st_offset                  : 1;
  uint8_t st_g                          : 2;
  uint8_t st_xl                         : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl10_t;

#define LSM6DSV16B_CTRL_STATUS                   0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t fsm_wr_ctrl_status            : 1;
  uint8_t not_used1                     : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 5;
  uint8_t fsm_wr_ctrl_status            : 1;
  uint8_t not_used0                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ctrl_status_t;

#define LSM6DSV16B_FIFO_STATUS1                  0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t diff_fifo                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t diff_fifo                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_status1_t;

#define LSM6DSV16B_FIFO_STATUS2                  0x1CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t diff_fifo                     : 1;
  uint8_t not_used0                     : 2;
  uint8_t fifo_ovr_latched              : 1;
  uint8_t counter_bdr_ia                : 1;
  uint8_t fifo_full_ia                  : 1;
  uint8_t fifo_ovr_ia                   : 1;
  uint8_t fifo_wtm_ia                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_wtm_ia                   : 1;
  uint8_t fifo_ovr_ia                   : 1;
  uint8_t fifo_full_ia                  : 1;
  uint8_t counter_bdr_ia                : 1;
  uint8_t fifo_ovr_latched              : 1;
  uint8_t not_used0                     : 2;
  uint8_t diff_fifo                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_status2_t;

#define LSM6DSV16B_ALL_INT_SRC                   0x1DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ia                         : 1;
  uint8_t wu_ia                         : 1;
  uint8_t tap_ia                        : 1;
  uint8_t not_used0                     : 1;
  uint8_t d6d_ia                        : 1;
  uint8_t sleep_change_ia               : 1;
  uint8_t not_used1                     : 1;
  uint8_t emb_func_ia                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_ia                   : 1;
  uint8_t not_used1                     : 1;
  uint8_t sleep_change_ia               : 1;
  uint8_t d6d_ia                        : 1;
  uint8_t not_used0                     : 1;
  uint8_t tap_ia                        : 1;
  uint8_t wu_ia                         : 1;
  uint8_t ff_ia                         : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_all_int_src_t;

#define LSM6DSV16B_STATUS_REG                    0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlda                          : 1;
  uint8_t gda                           : 1;
  uint8_t tda                           : 1;
  uint8_t not_used0                     : 4;
  uint8_t timestamp_endcount            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_endcount            : 1;
  uint8_t not_used0                     : 4;
  uint8_t tda                           : 1;
  uint8_t gda                           : 1;
  uint8_t xlda                          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_status_reg_t;

#define LSM6DSV16B_OUT_TEMP_L                    0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                          : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_out_temp_l_t;

#define LSM6DSV16B_OUT_TEMP_H                    0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                          : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_out_temp_h_t;

#define LSM6DSV16B_OUTX_L_G                      0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outx_l_g_t;

#define LSM6DSV16B_OUTX_H_G                      0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outx_h_g_t;

#define LSM6DSV16B_OUTY_L_G                      0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outy_l_g_t;

#define LSM6DSV16B_OUTY_H_G                      0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outy_h_g_t;

#define LSM6DSV16B_OUTZ_L_G                      0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outz_l_g_t;

#define LSM6DSV16B_OUTZ_H_G                      0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outz_h_g_t;

#define LSM6DSV16B_OUTZ_L_A                      0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outz_l_a_t;

#define LSM6DSV16B_OUTZ_H_A                      0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outz_h_a_t;

#define LSM6DSV16B_OUTY_L_A                      0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outy_l_a_t;

#define LSM6DSV16B_OUTY_H_A                      0x2BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outy_h_a_t;

#define LSM6DSV16B_OUTX_L_A                      0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outx_l_a_t;

#define LSM6DSV16B_OUTX_H_A                      0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_outx_h_a_t;

#define LSM6DSV16B_UI_OUTZ_L_A_DUALC             0x34U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outz_l_a_dualc_t;

#define LSM6DSV16B_UI_OUTZ_H_A_DUALC             0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outz_h_a_dualc_t;

#define LSM6DSV16B_UI_OUTY_L_A_DUALC             0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outy_l_a_dualc_t;

#define LSM6DSV16B_UI_OUTY_H_A_DUALC             0x37U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outy_h_a_dualc_t;

#define LSM6DSV16B_UI_OUTX_L_A_DUALC             0x38U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outx_l_a_dualc_t;

#define LSM6DSV16B_UI_OUTX_H_A_DUALC             0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_ui_outx_h_a_dualc_t;

#define LSM6DSV16B_TIMESTAMP0                    0x40U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_timestamp0_t;

#define LSM6DSV16B_TIMESTAMP1                    0x41U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_timestamp1_t;

#define LSM6DSV16B_TIMESTAMP2                    0x42U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_timestamp2_t;

#define LSM6DSV16B_TIMESTAMP3                    0x43U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_timestamp3_t;

#define LSM6DSV16B_WAKE_UP_SRC                   0x45U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t x_wu                          : 1;
  uint8_t y_wu                          : 1;
  uint8_t z_wu                          : 1;
  uint8_t wu_ia                         : 1;
  uint8_t sleep_state                   : 1;
  uint8_t ff_ia                         : 1;
  uint8_t sleep_change_ia               : 1;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sleep_change_ia               : 1;
  uint8_t ff_ia                         : 1;
  uint8_t sleep_state                   : 1;
  uint8_t wu_ia                         : 1;
  uint8_t z_wu                          : 1;
  uint8_t y_wu                          : 1;
  uint8_t x_wu                          : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_wake_up_src_t;

#define LSM6DSV16B_TAP_SRC                       0x46U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t x_tap                         : 1;
  uint8_t y_tap                         : 1;
  uint8_t z_tap                         : 1;
  uint8_t tap_sign                      : 1;
  uint8_t not_used0                     : 1;
  uint8_t double_tap                    : 1;
  uint8_t single_tap                    : 1;
  uint8_t tap_ia                        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_ia                        : 1;
  uint8_t single_tap                    : 1;
  uint8_t double_tap                    : 1;
  uint8_t not_used0                     : 1;
  uint8_t tap_sign                      : 1;
  uint8_t z_tap                         : 1;
  uint8_t y_tap                         : 1;
  uint8_t x_tap                         : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_src_t;

#define LSM6DSV16B_D6D_SRC                       0x47U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t zl                            : 1;
  uint8_t zh                            : 1;
  uint8_t yl                            : 1;
  uint8_t yh                            : 1;
  uint8_t xl                            : 1;
  uint8_t xh                            : 1;
  uint8_t d6d_ia                        : 1;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t d6d_ia                        : 1;
  uint8_t xh                            : 1;
  uint8_t xl                            : 1;
  uint8_t yh                            : 1;
  uint8_t yl                            : 1;
  uint8_t zh                            : 1;
  uint8_t zl                            : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_d6d_src_t;

#define LSM6DSV16B_EMB_FUNC_STATUS_MAINPAGE      0x49U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t is_step_det                   : 1;
  uint8_t is_tilt                       : 1;
  uint8_t is_sigmot                     : 1;
  uint8_t not_used1                     : 1;
  uint8_t is_fsm_lc                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                     : 1;
  uint8_t not_used1                     : 1;
  uint8_t is_sigmot                     : 1;
  uint8_t is_tilt                       : 1;
  uint8_t is_step_det                   : 1;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_status_mainpage_t;

#define LSM6DSV16B_FSM_STATUS_MAINPAGE           0x4AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                       : 1;
  uint8_t is_fsm2                       : 1;
  uint8_t is_fsm3                       : 1;
  uint8_t is_fsm4                       : 1;
  uint8_t is_fsm5                       : 1;
  uint8_t is_fsm6                       : 1;
  uint8_t is_fsm7                       : 1;
  uint8_t is_fsm8                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                       : 1;
  uint8_t is_fsm7                       : 1;
  uint8_t is_fsm6                       : 1;
  uint8_t is_fsm5                       : 1;
  uint8_t is_fsm4                       : 1;
  uint8_t is_fsm3                       : 1;
  uint8_t is_fsm2                       : 1;
  uint8_t is_fsm1                       : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_status_mainpage_t;

#define LSM6DSV16B_INTERNAL_FREQ                 0x4FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t freq_fine                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t freq_fine                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_internal_freq_t;

#define LSM6DSV16B_FUNCTIONS_ENABLE              0x50U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t inact_en                      : 2;
  uint8_t not_used0                     : 1;
  uint8_t dis_rst_lir_all_int           : 1;
  uint8_t not_used1                     : 2;
  uint8_t timestamp_en                  : 1;
  uint8_t interrupts_enable             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t interrupts_enable             : 1;
  uint8_t timestamp_en                  : 1;
  uint8_t not_used1                     : 2;
  uint8_t dis_rst_lir_all_int           : 1;
  uint8_t not_used0                     : 1;
  uint8_t inact_en                      : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_functions_enable_t;

#define LSM6DSV16B_INACTIVITY_DUR                0x54U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t inact_dur                     : 2;
  uint8_t xl_inact_odr                  : 2;
  uint8_t wu_inact_ths_w                : 3;
  uint8_t sleep_status_on_int           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sleep_status_on_int           : 1;
  uint8_t wu_inact_ths_w                : 3;
  uint8_t xl_inact_odr                  : 2;
  uint8_t inact_dur                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_inactivity_dur_t;

#define LSM6DSV16B_INACTIVITY_THS                0x55U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t inact_ths                     : 6;
  uint8_t not_used0                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t inact_ths                     : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_inactivity_ths_t;

#define LSM6DSV16B_TAP_CFG0                      0x56U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lir                           : 1;
  uint8_t tap_x_en                      : 1;
  uint8_t tap_y_en                      : 1;
  uint8_t tap_z_en                      : 1;
  uint8_t slope_fds                     : 1;
  uint8_t hw_func_mask_xl_settl         : 1;
  uint8_t low_pass_on_6d                : 1;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t low_pass_on_6d                : 1;
  uint8_t hw_func_mask_xl_settl         : 1;
  uint8_t slope_fds                     : 1;
  uint8_t tap_z_en                      : 1;
  uint8_t tap_y_en                      : 1;
  uint8_t tap_x_en                      : 1;
  uint8_t lir                           : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_cfg0_t;

#define LSM6DSV16B_TAP_CFG1                      0x57U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_z                     : 5;
  uint8_t tap_priority                  : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_priority                  : 3;
  uint8_t tap_ths_z                     : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_cfg1_t;

#define LSM6DSV16B_TAP_CFG2                      0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_y                     : 5;
  uint8_t not_used0                     : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t tap_ths_y                     : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_cfg2_t;

#define LSM6DSV16B_TAP_THS_6D                    0x59U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_x                     : 5;
  uint8_t sixd_ths                      : 2;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sixd_ths                      : 2;
  uint8_t tap_ths_x                     : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_ths_6d_t;

#define LSM6DSV16B_TAP_DUR                       0x5AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t shock                         : 2;
  uint8_t quiet                         : 2;
  uint8_t dur                           : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t dur                           : 4;
  uint8_t quiet                         : 2;
  uint8_t shock                         : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tap_dur_t;

#define LSM6DSV16B_WAKE_UP_THS                   0x5BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wk_ths                        : 6;
  uint8_t usr_off_on_wu                 : 1;
  uint8_t single_double_tap             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t single_double_tap             : 1;
  uint8_t usr_off_on_wu                 : 1;
  uint8_t wk_ths                        : 6;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_wake_up_ths_t;

#define LSM6DSV16B_WAKE_UP_DUR                   0x5CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sleep_dur                     : 4;
  uint8_t not_used0                     : 1;
  uint8_t wake_dur                      : 2;
  uint8_t ff_dur                        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                        : 1;
  uint8_t wake_dur                      : 2;
  uint8_t not_used0                     : 1;
  uint8_t sleep_dur                     : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_wake_up_dur_t;

#define LSM6DSV16B_FREE_FALL                     0x5DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths                        : 3;
  uint8_t ff_dur                        : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                        : 5;
  uint8_t ff_ths                        : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_free_fall_t;

#define LSM6DSV16B_MD1_CFG                       0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t int1_emb_func                 : 1;
  uint8_t int1_6d                       : 1;
  uint8_t int1_double_tap               : 1;
  uint8_t int1_ff                       : 1;
  uint8_t int1_wu                       : 1;
  uint8_t int1_single_tap               : 1;
  uint8_t int1_sleep_change             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_sleep_change             : 1;
  uint8_t int1_single_tap               : 1;
  uint8_t int1_wu                       : 1;
  uint8_t int1_ff                       : 1;
  uint8_t int1_double_tap               : 1;
  uint8_t int1_6d                       : 1;
  uint8_t int1_emb_func                 : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_md1_cfg_t;

#define LSM6DSV16B_MD2_CFG                       0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_timestamp                : 1;
  uint8_t int2_emb_func                 : 1;
  uint8_t int2_6d                       : 1;
  uint8_t int2_double_tap               : 1;
  uint8_t int2_ff                       : 1;
  uint8_t int2_wu                       : 1;
  uint8_t int2_single_tap               : 1;
  uint8_t int2_sleep_change             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_sleep_change             : 1;
  uint8_t int2_single_tap               : 1;
  uint8_t int2_wu                       : 1;
  uint8_t int2_ff                       : 1;
  uint8_t int2_double_tap               : 1;
  uint8_t int2_6d                       : 1;
  uint8_t int2_emb_func                 : 1;
  uint8_t int2_timestamp                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_md2_cfg_t;

#define LSM6DSV16B_EMB_FUNC_CFG                  0x63U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t emb_func_disable              : 1;
  uint8_t emb_func_irq_mask_xl_settl    : 1;
  uint8_t emb_func_irq_mask_g_settl     : 1;
  uint8_t not_used1                     : 1;
  uint8_t xl_dualc_batch_from_if        : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t xl_dualc_batch_from_if        : 1;
  uint8_t not_used1                     : 1;
  uint8_t emb_func_irq_mask_g_settl     : 1;
  uint8_t emb_func_irq_mask_xl_settl    : 1;
  uint8_t emb_func_disable              : 1;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_cfg_t;

#define LSM6DSV16B_TDM_CFG0                      0x6CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tdm_wclk_bclk_sel             : 1;
  uint8_t tdm_wclk                      : 2;
  uint8_t not_used0                     : 1;
  uint8_t tdm_slot_sel                  : 1;
  uint8_t tdm_bclk_edge_sel             : 1;
  uint8_t tdm_delayed_cfg               : 1;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t tdm_delayed_cfg               : 1;
  uint8_t tdm_bclk_edge_sel             : 1;
  uint8_t tdm_slot_sel                  : 1;
  uint8_t not_used0                     : 1;
  uint8_t tdm_wclk                      : 2;
  uint8_t tdm_wclk_bclk_sel             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tdm_cfg0_t;

#define LSM6DSV16B_TDM_CFG1                      0x6DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t tdm_axes_ord_sel              : 2;
  uint8_t tdm_xl_z_en                   : 1;
  uint8_t tdm_xl_y_en                   : 1;
  uint8_t tdm_xl_x_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tdm_xl_x_en                   : 1;
  uint8_t tdm_xl_y_en                   : 1;
  uint8_t tdm_xl_z_en                   : 1;
  uint8_t tdm_axes_ord_sel              : 2;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tdm_cfg1_t;

#define LSM6DSV16B_TDM_CFG2                      0x6EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tdm_fs_xl                     : 2;
  uint8_t not_used0                     : 1;
  uint8_t tdm_data_mask                 : 1;
  uint8_t not_used1                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 4;
  uint8_t tdm_data_mask                 : 1;
  uint8_t not_used0                     : 1;
  uint8_t tdm_fs_xl                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_tdm_cfg2_t;

#define LSM6DSV16B_Z_OFS_USR                     0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t z_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_z_ofs_usr_t;

#define LSM6DSV16B_Y_OFS_USR                     0x74U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t y_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t y_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_y_ofs_usr_t;

#define LSM6DSV16B_X_OFS_USR                     0x75U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t x_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t x_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_x_ofs_usr_t;

#define LSM6DSV16B_FIFO_DATA_OUT_TAG             0x78U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t tag_cnt                       : 2;
  uint8_t tag_sensor                    : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tag_sensor                    : 5;
  uint8_t tag_cnt                       : 2;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_tag_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_0          0x79U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_0_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_1          0x7AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_1_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_2          0x7BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_2_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_3          0x7CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_3_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_4          0x7DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_4_t;

#define LSM6DSV16B_FIFO_DATA_OUT_BYTE_5          0x7EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fifo_data_out_byte_5_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page embedded
  * @{
  *
  */

#define LSM6DSV16B_PAGE_SEL                      0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t page_sel                      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_sel                      : 4;
  uint8_t not_used0                     : 4;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_page_sel_t;

#define LSM6DSV16B_EMB_FUNC_EN_A                 0x4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used2                     : 1;
  uint8_t sflp_game_en                  : 1;
  uint8_t not_used0                     : 1;
  uint8_t pedo_en                       : 1;
  uint8_t tilt_en                       : 1;
  uint8_t sign_motion_en                : 1;
  uint8_t not_used1                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 2;
  uint8_t sign_motion_en                : 1;
  uint8_t tilt_en                       : 1;
  uint8_t pedo_en                       : 1;
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_en                  : 1;
  uint8_t not_used2                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_en_a_t;

#define LSM6DSV16B_EMB_FUNC_EN_B                 0x5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_en                        : 1;
  uint8_t not_used0                     : 2;
  uint8_t fifo_compr_en                 : 1;
  uint8_t not_used1                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 4;
  uint8_t fifo_compr_en                 : 1;
  uint8_t not_used0                     : 2;
  uint8_t fsm_en                        : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_en_b_t;

#define LSM6DSV16B_EMB_FUNC_EXEC_STATUS          0x7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t emb_func_endop                : 1;
  uint8_t emb_func_exec_ovr             : 1;
  uint8_t not_used0                     : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 6;
  uint8_t emb_func_exec_ovr             : 1;
  uint8_t emb_func_endop                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_exec_status_t;

#define LSM6DSV16B_PAGE_ADDRESS                  0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_addr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_addr                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_page_address_t;

#define LSM6DSV16B_PAGE_VALUE                    0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_value                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_value                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_page_value_t;

#define LSM6DSV16B_EMB_FUNC_INT1                 0x0AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t int1_step_detector            : 1;
  uint8_t int1_tilt                     : 1;
  uint8_t int1_sig_mot                  : 1;
  uint8_t not_used1                     : 1;
  uint8_t int1_fsm_lc                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm_lc                   : 1;
  uint8_t not_used1                     : 1;
  uint8_t int1_sig_mot                  : 1;
  uint8_t int1_tilt                     : 1;
  uint8_t int1_step_detector            : 1;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_int1_t;

#define LSM6DSV16B_FSM_INT1                      0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_fsm1                     : 1;
  uint8_t int1_fsm2                     : 1;
  uint8_t int1_fsm3                     : 1;
  uint8_t int1_fsm4                     : 1;
  uint8_t int1_fsm5                     : 1;
  uint8_t int1_fsm6                     : 1;
  uint8_t int1_fsm7                     : 1;
  uint8_t int1_fsm8                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int1_fsm8                     : 1;
  uint8_t int1_fsm7                     : 1;
  uint8_t int1_fsm6                     : 1;
  uint8_t int1_fsm5                     : 1;
  uint8_t int1_fsm4                     : 1;
  uint8_t int1_fsm3                     : 1;
  uint8_t int1_fsm2                     : 1;
  uint8_t int1_fsm1                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_int1_t;

#define LSM6DSV16B_EMB_FUNC_INT2                 0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t int2_step_detector            : 1;
  uint8_t int2_tilt                     : 1;
  uint8_t int2_sig_mot                  : 1;
  uint8_t not_used1                     : 1;
  uint8_t int2_fsm_lc                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm_lc                   : 1;
  uint8_t not_used1                     : 1;
  uint8_t int2_sig_mot                  : 1;
  uint8_t int2_tilt                     : 1;
  uint8_t int2_step_detector            : 1;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_int2_t;

#define LSM6DSV16B_FSM_INT2                      0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_fsm1                     : 1;
  uint8_t int2_fsm2                     : 1;
  uint8_t int2_fsm3                     : 1;
  uint8_t int2_fsm4                     : 1;
  uint8_t int2_fsm5                     : 1;
  uint8_t int2_fsm6                     : 1;
  uint8_t int2_fsm7                     : 1;
  uint8_t int2_fsm8                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int2_fsm8                     : 1;
  uint8_t int2_fsm7                     : 1;
  uint8_t int2_fsm6                     : 1;
  uint8_t int2_fsm5                     : 1;
  uint8_t int2_fsm4                     : 1;
  uint8_t int2_fsm3                     : 1;
  uint8_t int2_fsm2                     : 1;
  uint8_t int2_fsm1                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_int2_t;

#define LSM6DSV16B_EMB_FUNC_STATUS               0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t is_step_det                   : 1;
  uint8_t is_tilt                       : 1;
  uint8_t is_sigmot                     : 1;
  uint8_t not_used1                     : 1;
  uint8_t is_fsm_lc                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm_lc                     : 1;
  uint8_t not_used1                     : 1;
  uint8_t is_sigmot                     : 1;
  uint8_t is_tilt                       : 1;
  uint8_t is_step_det                   : 1;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_status_t;

#define LSM6DSV16B_FSM_STATUS                    0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_fsm1                       : 1;
  uint8_t is_fsm2                       : 1;
  uint8_t is_fsm3                       : 1;
  uint8_t is_fsm4                       : 1;
  uint8_t is_fsm5                       : 1;
  uint8_t is_fsm6                       : 1;
  uint8_t is_fsm7                       : 1;
  uint8_t is_fsm8                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t is_fsm8                       : 1;
  uint8_t is_fsm7                       : 1;
  uint8_t is_fsm6                       : 1;
  uint8_t is_fsm5                       : 1;
  uint8_t is_fsm4                       : 1;
  uint8_t is_fsm3                       : 1;
  uint8_t is_fsm2                       : 1;
  uint8_t is_fsm1                       : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_status_t;

#define LSM6DSV16B_PAGE_RW                       0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 5;
  uint8_t page_read                     : 1;
  uint8_t page_write                    : 1;
  uint8_t emb_func_lir                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t emb_func_lir                  : 1;
  uint8_t page_write                    : 1;
  uint8_t page_read                     : 1;
  uint8_t not_used0                     : 5;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_page_rw_t;

#define LSM6DSV16B_EMB_FUNC_FIFO_EN_A            0x44U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_fifo_en             : 1;
  uint8_t not_used1                     : 2;
  uint8_t sflp_gravity_fifo_en          : 1;
  uint8_t sflp_gbias_fifo_en            : 1;
  uint8_t step_counter_fifo_en          : 1;
  uint8_t not_used2                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used2                     : 1;
  uint8_t step_counter_fifo_en          : 1;
  uint8_t sflp_gbias_fifo_en            : 1;
  uint8_t sflp_gravity_fifo_en          : 1;
  uint8_t not_used1                     : 2;
  uint8_t sflp_game_fifo_en             : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_fifo_en_a_t;

#define LSM6DSV16B_FSM_ENABLE                    0x46U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm1_en                       : 1;
  uint8_t fsm2_en                       : 1;
  uint8_t fsm3_en                       : 1;
  uint8_t fsm4_en                       : 1;
  uint8_t fsm5_en                       : 1;
  uint8_t fsm6_en                       : 1;
  uint8_t fsm7_en                       : 1;
  uint8_t fsm8_en                       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm8_en                       : 1;
  uint8_t fsm7_en                       : 1;
  uint8_t fsm6_en                       : 1;
  uint8_t fsm5_en                       : 1;
  uint8_t fsm4_en                       : 1;
  uint8_t fsm3_en                       : 1;
  uint8_t fsm2_en                       : 1;
  uint8_t fsm1_en                       : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_enable_t;

#define LSM6DSV16B_FSM_LONG_COUNTER_L            0x48U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_long_counter_l_t;

#define LSM6DSV16B_FSM_LONG_COUNTER_H            0x49U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_long_counter_h_t;

#define LSM6DSV16B_INT_ACK_MASK                  0x4BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t iack_mask                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t iack_mask                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_int_ack_mask_t;

#define LSM6DSV16B_FSM_OUTS1                     0x4CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm1_n_v                      : 1;
  uint8_t fsm1_p_v                      : 1;
  uint8_t fsm1_n_3                      : 1;
  uint8_t fsm1_p_3                      : 1;
  uint8_t fsm1_n_2                      : 1;
  uint8_t fsm1_p_2                      : 1;
  uint8_t fsm1_n_1                      : 1;
  uint8_t fsm1_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm1_p_1                      : 1;
  uint8_t fsm1_n_1                      : 1;
  uint8_t fsm1_p_2                      : 1;
  uint8_t fsm1_n_2                      : 1;
  uint8_t fsm1_p_3                      : 1;
  uint8_t fsm1_n_3                      : 1;
  uint8_t fsm1_p_v                      : 1;
  uint8_t fsm1_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs1_t;

#define LSM6DSV16B_FSM_OUTS2                     0x4DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm2_n_v                      : 1;
  uint8_t fsm2_p_v                      : 1;
  uint8_t fsm2_n_3                      : 1;
  uint8_t fsm2_p_3                      : 1;
  uint8_t fsm2_n_2                      : 1;
  uint8_t fsm2_p_2                      : 1;
  uint8_t fsm2_n_1                      : 1;
  uint8_t fsm2_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm2_p_1                      : 1;
  uint8_t fsm2_n_1                      : 1;
  uint8_t fsm2_p_2                      : 1;
  uint8_t fsm2_n_2                      : 1;
  uint8_t fsm2_p_3                      : 1;
  uint8_t fsm2_n_3                      : 1;
  uint8_t fsm2_p_v                      : 1;
  uint8_t fsm2_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs2_t;

#define LSM6DSV16B_FSM_OUTS3                     0x4EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm3_n_v                      : 1;
  uint8_t fsm3_p_v                      : 1;
  uint8_t fsm3_n_3                      : 1;
  uint8_t fsm3_p_3                      : 1;
  uint8_t fsm3_n_2                      : 1;
  uint8_t fsm3_p_2                      : 1;
  uint8_t fsm3_n_1                      : 1;
  uint8_t fsm3_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm3_p_1                      : 1;
  uint8_t fsm3_n_1                      : 1;
  uint8_t fsm3_p_2                      : 1;
  uint8_t fsm3_n_2                      : 1;
  uint8_t fsm3_p_3                      : 1;
  uint8_t fsm3_n_3                      : 1;
  uint8_t fsm3_p_v                      : 1;
  uint8_t fsm3_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs3_t;

#define LSM6DSV16B_FSM_OUTS4                     0x4FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm4_n_v                      : 1;
  uint8_t fsm4_p_v                      : 1;
  uint8_t fsm4_n_3                      : 1;
  uint8_t fsm4_p_3                      : 1;
  uint8_t fsm4_n_2                      : 1;
  uint8_t fsm4_p_2                      : 1;
  uint8_t fsm4_n_1                      : 1;
  uint8_t fsm4_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm4_p_1                      : 1;
  uint8_t fsm4_n_1                      : 1;
  uint8_t fsm4_p_2                      : 1;
  uint8_t fsm4_n_2                      : 1;
  uint8_t fsm4_p_3                      : 1;
  uint8_t fsm4_n_3                      : 1;
  uint8_t fsm4_p_v                      : 1;
  uint8_t fsm4_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs4_t;

#define LSM6DSV16B_FSM_OUTS5                     0x50U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm5_n_v                      : 1;
  uint8_t fsm5_p_v                      : 1;
  uint8_t fsm5_n_3                      : 1;
  uint8_t fsm5_p_3                      : 1;
  uint8_t fsm5_n_2                      : 1;
  uint8_t fsm5_p_2                      : 1;
  uint8_t fsm5_n_1                      : 1;
  uint8_t fsm5_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm5_p_1                      : 1;
  uint8_t fsm5_n_1                      : 1;
  uint8_t fsm5_p_2                      : 1;
  uint8_t fsm5_n_2                      : 1;
  uint8_t fsm5_p_3                      : 1;
  uint8_t fsm5_n_3                      : 1;
  uint8_t fsm5_p_v                      : 1;
  uint8_t fsm5_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs5_t;

#define LSM6DSV16B_FSM_OUTS6                     0x51U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm6_n_v                      : 1;
  uint8_t fsm6_p_v                      : 1;
  uint8_t fsm6_n_3                      : 1;
  uint8_t fsm6_p_3                      : 1;
  uint8_t fsm6_n_2                      : 1;
  uint8_t fsm6_p_2                      : 1;
  uint8_t fsm6_n_1                      : 1;
  uint8_t fsm6_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm6_p_1                      : 1;
  uint8_t fsm6_n_1                      : 1;
  uint8_t fsm6_p_2                      : 1;
  uint8_t fsm6_n_2                      : 1;
  uint8_t fsm6_p_3                      : 1;
  uint8_t fsm6_n_3                      : 1;
  uint8_t fsm6_p_v                      : 1;
  uint8_t fsm6_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs6_t;

#define LSM6DSV16B_FSM_OUTS7                     0x52U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm7_n_v                      : 1;
  uint8_t fsm7_p_v                      : 1;
  uint8_t fsm7_n_3                      : 1;
  uint8_t fsm7_p_3                      : 1;
  uint8_t fsm7_n_2                      : 1;
  uint8_t fsm7_p_2                      : 1;
  uint8_t fsm7_n_1                      : 1;
  uint8_t fsm7_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm7_p_1                      : 1;
  uint8_t fsm7_n_1                      : 1;
  uint8_t fsm7_p_2                      : 1;
  uint8_t fsm7_n_2                      : 1;
  uint8_t fsm7_p_3                      : 1;
  uint8_t fsm7_n_3                      : 1;
  uint8_t fsm7_p_v                      : 1;
  uint8_t fsm7_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs7_t;

#define LSM6DSV16B_FSM_OUTS8                     0x53U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm8_n_v                      : 1;
  uint8_t fsm8_p_v                      : 1;
  uint8_t fsm8_n_3                      : 1;
  uint8_t fsm8_p_3                      : 1;
  uint8_t fsm8_n_2                      : 1;
  uint8_t fsm8_p_2                      : 1;
  uint8_t fsm8_n_1                      : 1;
  uint8_t fsm8_p_1                      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm8_p_1                      : 1;
  uint8_t fsm8_n_1                      : 1;
  uint8_t fsm8_p_2                      : 1;
  uint8_t fsm8_n_2                      : 1;
  uint8_t fsm8_p_3                      : 1;
  uint8_t fsm8_n_3                      : 1;
  uint8_t fsm8_p_v                      : 1;
  uint8_t fsm8_n_v                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_outs8_t;

#define LSM6DSV16B_SFLP_ODR                      0x5EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t sflp_game_odr                 : 3;
  uint8_t not_used1                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 2;
  uint8_t sflp_game_odr                 : 3;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_odr_t;

#define LSM6DSV16B_FSM_ODR                       0x5FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t fsm_odr                       : 3;
  uint8_t not_used1                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 2;
  uint8_t fsm_odr                       : 3;
  uint8_t not_used0                     : 3;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_odr_t;

#define LSM6DSV16B_STEP_COUNTER_L                0x62U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                          : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_step_counter_l_t;

#define LSM6DSV16B_STEP_COUNTER_H                0x63U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                          : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_step_counter_h_t;

#define LSM6DSV16B_EMB_FUNC_SRC                  0x64U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t stepcounter_bit_set           : 1;
  uint8_t step_overflow                 : 1;
  uint8_t step_count_delta_ia           : 1;
  uint8_t step_detected                 : 1;
  uint8_t not_used1                     : 1;
  uint8_t pedo_rst_step                 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pedo_rst_step                 : 1;
  uint8_t not_used1                     : 1;
  uint8_t step_detected                 : 1;
  uint8_t step_count_delta_ia           : 1;
  uint8_t step_overflow                 : 1;
  uint8_t stepcounter_bit_set           : 1;
  uint8_t not_used0                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_src_t;

#define LSM6DSV16B_EMB_FUNC_INIT_A               0x66U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_init                : 1;
  uint8_t not_used2                     : 1;
  uint8_t step_det_init                 : 1;
  uint8_t tilt_init                     : 1;
  uint8_t sig_mot_init                  : 1;
  uint8_t not_used1                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 2;
  uint8_t not_used1                     : 1;
  uint8_t sig_mot_init                  : 1;
  uint8_t tilt_init                     : 1;
  uint8_t step_det_init                 : 1;
  uint8_t not_used2                     : 1;
  uint8_t sflp_game_init                : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_init_a_t;

#define LSM6DSV16B_EMB_FUNC_INIT_B               0x67U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_init                      : 1;
  uint8_t not_used0                     : 2;
  uint8_t fifo_compr_init               : 1;
  uint8_t not_used1                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 4;
  uint8_t fifo_compr_init               : 1;
  uint8_t not_used0                     : 2;
  uint8_t fsm_init                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_emb_func_init_b_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page pg0_emb_adv
  * @{
  *
  */
#define LSM6DSV16B_EMB_ADV_PG_0                  0x000

#define LSM6DSV16B_SFLP_GAME_GBIASX_L            0x6EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasx                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasx                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasx_l_t;

#define LSM6DSV16B_SFLP_GAME_GBIASX_H            0x6FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasx                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasx                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasx_h_t;

#define LSM6DSV16B_SFLP_GAME_GBIASY_L            0x70U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasy                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasy                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasy_l_t;

#define LSM6DSV16B_SFLP_GAME_GBIASY_H            0x71U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasy                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasy                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasy_h_t;

#define LSM6DSV16B_SFLP_GAME_GBIASZ_L            0x72U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasz                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasz                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasz_l_t;

#define LSM6DSV16B_SFLP_GAME_GBIASZ_H            0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasz                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasz                        : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_sflp_game_gbiasz_h_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page pg1_emb_adv
  * @{
  *
  */

#define LSM6DSV16B_EMB_ADV_PG_1                  0x001

#define LSM6DSV16B_FSM_LC_TIMEOUT_L              0x17AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_lc_timeout_l_t;

#define LSM6DSV16B_FSM_LC_TIMEOUT_H              0x17BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_lc_timeout_h_t;

#define LSM6DSV16B_FSM_PROGRAMS                  0x17CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_n_prog                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_n_prog                    : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_programs_t;

#define LSM6DSV16B_FSM_START_ADD_L               0x17EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_start_add_l_t;

#define LSM6DSV16B_FSM_START_ADD_H               0x17FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                     : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_fsm_start_add_h_t;

#define LSM6DSV16B_PEDO_CMD_REG                  0x183U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t fp_rejection_en               : 1;
  uint8_t carry_count_en                : 1;
  uint8_t not_used1                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 4;
  uint8_t carry_count_en                : 1;
  uint8_t fp_rejection_en               : 1;
  uint8_t not_used0                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_pedo_cmd_reg_t;

#define LSM6DSV16B_PEDO_DEB_STEPS_CONF           0x184U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t deb_step                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t deb_step                      : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_pedo_deb_steps_conf_t;

#define LSM6DSV16B_PEDO_SC_DELTAT_L              0x1D0U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_pedo_sc_deltat_l_t;

#define LSM6DSV16B_PEDO_SC_DELTAT_H              0x1D1U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                         : 8;
#endif /* DRV_BYTE_ORDER */
} lsm6dsv16b_pedo_sc_deltat_h_t;

/**
  * @}
  *
  */

#define LSM6DSV16B_START_FSM_ADD                0x035CU

/**
  * @defgroup LSM6DSV16B_Register_Union
  * @brief    These unions group all the registers having a bit-field
  *           description.
  *           These unions are useful but it's not needed by the driver.
  *
  *           REMOVING this unions you are compliant with:
  *           MISRA-C 2012 [Rule 19.2] -> " Union are not allowed "
  *
  * @{
  *
  */
typedef union
{
  lsm6dsv16b_func_cfg_access_t    func_cfg_access;
  lsm6dsv16b_pin_ctrl_t    pin_ctrl;
  lsm6dsv16b_if_cfg_t    if_cfg;
  lsm6dsv16b_fifo_ctrl1_t    fifo_ctrl1;
  lsm6dsv16b_fifo_ctrl2_t    fifo_ctrl2;
  lsm6dsv16b_fifo_ctrl3_t    fifo_ctrl3;
  lsm6dsv16b_fifo_ctrl4_t    fifo_ctrl4;
  lsm6dsv16b_counter_bdr_reg1_t    counter_bdr_reg1;
  lsm6dsv16b_counter_bdr_reg2_t    counter_bdr_reg2;
  lsm6dsv16b_int1_ctrl_t    int1_ctrl;
  lsm6dsv16b_int2_ctrl_t    int2_ctrl;
  lsm6dsv16b_who_am_i_t    who_am_i;
  lsm6dsv16b_ctrl1_t    ctrl1;
  lsm6dsv16b_ctrl2_t    ctrl2;
  lsm6dsv16b_ctrl3_t    ctrl3;
  lsm6dsv16b_ctrl4_t    ctrl4;
  lsm6dsv16b_ctrl5_t    ctrl5;
  lsm6dsv16b_ctrl6_t    ctrl6;
  lsm6dsv16b_ctrl7_t    ctrl7;
  lsm6dsv16b_ctrl8_t    ctrl8;
  lsm6dsv16b_ctrl9_t    ctrl9;
  lsm6dsv16b_ctrl10_t    ctrl10;
  lsm6dsv16b_fifo_status1_t    fifo_status1;
  lsm6dsv16b_fifo_status2_t    fifo_status2;
  lsm6dsv16b_all_int_src_t    all_int_src;
  lsm6dsv16b_status_reg_t    status_reg;
  lsm6dsv16b_out_temp_l_t    out_temp_l;
  lsm6dsv16b_out_temp_h_t    out_temp_h;
  lsm6dsv16b_outx_l_g_t    outx_l_g;
  lsm6dsv16b_outx_h_g_t    outx_h_g;
  lsm6dsv16b_outy_l_g_t    outy_l_g;
  lsm6dsv16b_outy_h_g_t    outy_h_g;
  lsm6dsv16b_outz_l_g_t    outz_l_g;
  lsm6dsv16b_outz_h_g_t    outz_h_g;
  lsm6dsv16b_outz_l_a_t    outz_l_a;
  lsm6dsv16b_outz_h_a_t    outz_h_a;
  lsm6dsv16b_outy_l_a_t    outy_l_a;
  lsm6dsv16b_outy_h_a_t    outy_h_a;
  lsm6dsv16b_outx_l_a_t    outx_l_a;
  lsm6dsv16b_outx_h_a_t    outx_h_a;
  lsm6dsv16b_ui_outz_l_a_dualc_t    ui_outz_l_a_dualc;
  lsm6dsv16b_ui_outz_h_a_dualc_t    ui_outz_h_a_dualc;
  lsm6dsv16b_ui_outy_l_a_dualc_t    ui_outy_l_a_dualc;
  lsm6dsv16b_ui_outy_h_a_dualc_t    ui_outy_h_a_dualc;
  lsm6dsv16b_ui_outx_l_a_dualc_t    ui_outx_l_a_dualc;
  lsm6dsv16b_ui_outx_h_a_dualc_t    ui_outx_h_a_dualc;
  lsm6dsv16b_timestamp0_t    timestamp0;
  lsm6dsv16b_timestamp1_t    timestamp1;
  lsm6dsv16b_timestamp2_t    timestamp2;
  lsm6dsv16b_timestamp3_t    timestamp3;
  lsm6dsv16b_wake_up_src_t    wake_up_src;
  lsm6dsv16b_tap_src_t    tap_src;
  lsm6dsv16b_d6d_src_t    d6d_src;
  lsm6dsv16b_emb_func_status_mainpage_t    emb_func_status_mainpage;
  lsm6dsv16b_fsm_status_mainpage_t    fsm_status_mainpage;
  lsm6dsv16b_internal_freq_t    internal_freq;
  lsm6dsv16b_functions_enable_t    functions_enable;
  lsm6dsv16b_inactivity_dur_t    inactivity_dur;
  lsm6dsv16b_inactivity_ths_t    inactivity_ths;
  lsm6dsv16b_tap_cfg0_t    tap_cfg0;
  lsm6dsv16b_tap_cfg1_t    tap_cfg1;
  lsm6dsv16b_tap_cfg2_t    tap_cfg2;
  lsm6dsv16b_tap_ths_6d_t    tap_ths_6d;
  lsm6dsv16b_tap_dur_t    int_dur2;
  lsm6dsv16b_wake_up_ths_t    wake_up_ths;
  lsm6dsv16b_wake_up_dur_t    wake_up_dur;
  lsm6dsv16b_free_fall_t    free_fall;
  lsm6dsv16b_md1_cfg_t    md1_cfg;
  lsm6dsv16b_md2_cfg_t    md2_cfg;
  lsm6dsv16b_emb_func_cfg_t    emb_func_cfg;
  lsm6dsv16b_tdm_cfg0_t    tdm_cfg0;
  lsm6dsv16b_tdm_cfg1_t    tdm_cfg1;
  lsm6dsv16b_tdm_cfg2_t    tdm_cfg2;
  lsm6dsv16b_z_ofs_usr_t    z_ofs_usr;
  lsm6dsv16b_y_ofs_usr_t    y_ofs_usr;
  lsm6dsv16b_x_ofs_usr_t    x_ofs_usr;
  lsm6dsv16b_fifo_data_out_tag_t    fifo_data_out_tag;
  lsm6dsv16b_fifo_data_out_byte_0_t    fifo_data_out_byte_0;
  lsm6dsv16b_fifo_data_out_byte_1_t    fifo_data_out_byte_1;
  lsm6dsv16b_fifo_data_out_byte_2_t    fifo_data_out_byte_2;
  lsm6dsv16b_fifo_data_out_byte_3_t    fifo_data_out_byte_3;
  lsm6dsv16b_fifo_data_out_byte_4_t    fifo_data_out_byte_4;
  lsm6dsv16b_fifo_data_out_byte_5_t    fifo_data_out_byte_5;
  lsm6dsv16b_page_sel_t    page_sel;
  lsm6dsv16b_emb_func_en_a_t    emb_func_en_a;
  lsm6dsv16b_emb_func_en_b_t    emb_func_en_b;
  lsm6dsv16b_emb_func_exec_status_t    emb_func_exec_status;
  lsm6dsv16b_page_address_t    page_address;
  lsm6dsv16b_page_value_t    page_value;
  lsm6dsv16b_emb_func_int1_t    emb_func_int1;
  lsm6dsv16b_fsm_int1_t    fsm_int1;
  lsm6dsv16b_emb_func_int2_t    emb_func_int2;
  lsm6dsv16b_fsm_int2_t    fsm_int2;
  lsm6dsv16b_emb_func_status_t    emb_func_status;
  lsm6dsv16b_fsm_status_t    fsm_status;
  lsm6dsv16b_page_rw_t    page_rw;
  lsm6dsv16b_emb_func_fifo_en_a_t    emb_func_fifo_en_a;
  lsm6dsv16b_fsm_enable_t    fsm_enable;
  lsm6dsv16b_fsm_long_counter_l_t    fsm_long_counter_l;
  lsm6dsv16b_fsm_long_counter_h_t    fsm_long_counter_h;
  lsm6dsv16b_fsm_outs1_t    fsm_outs1;
  lsm6dsv16b_fsm_outs2_t    fsm_outs2;
  lsm6dsv16b_fsm_outs3_t    fsm_outs3;
  lsm6dsv16b_fsm_outs4_t    fsm_outs4;
  lsm6dsv16b_fsm_outs5_t    fsm_outs5;
  lsm6dsv16b_fsm_outs6_t    fsm_outs6;
  lsm6dsv16b_fsm_outs7_t    fsm_outs7;
  lsm6dsv16b_fsm_outs8_t    fsm_outs8;
  lsm6dsv16b_fsm_odr_t    fsm_odr;
  lsm6dsv16b_step_counter_l_t    step_counter_l;
  lsm6dsv16b_step_counter_h_t    step_counter_h;
  lsm6dsv16b_emb_func_src_t    emb_func_src;
  lsm6dsv16b_emb_func_init_a_t    emb_func_init_a;
  lsm6dsv16b_emb_func_init_b_t    emb_func_init_b;
  lsm6dsv16b_fsm_lc_timeout_l_t    fsm_lc_timeout_l;
  lsm6dsv16b_fsm_lc_timeout_h_t    fsm_lc_timeout_h;
  lsm6dsv16b_fsm_programs_t    fsm_programs;
  lsm6dsv16b_fsm_start_add_l_t    fsm_start_add_l;
  lsm6dsv16b_fsm_start_add_h_t    fsm_start_add_h;
  lsm6dsv16b_pedo_cmd_reg_t    pedo_cmd_reg;
  lsm6dsv16b_pedo_deb_steps_conf_t    pedo_deb_steps_conf;
  lsm6dsv16b_pedo_sc_deltat_l_t    pedo_sc_deltat_l;
  lsm6dsv16b_pedo_sc_deltat_h_t    pedo_sc_deltat_h;
  bitwise_t    bitwise;
  uint8_t    byte;
} lsm6dsv16b_reg_t;


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

int32_t lsm6dsv16b_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                            uint8_t *data,
                            uint16_t len);
int32_t lsm6dsv16b_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                             uint8_t *data,
                             uint16_t len);

float_t lsm6dsv16b_from_sflp_to_mg(int16_t lsb);
float_t lsm6dsv16b_from_fs2_to_mg(int16_t lsb);
float_t lsm6dsv16b_from_fs4_to_mg(int16_t lsb);
float_t lsm6dsv16b_from_fs8_to_mg(int16_t lsb);
float_t lsm6dsv16b_from_fs16_to_mg(int16_t lsb);

float_t lsm6dsv16b_from_fs125_to_mdps(int16_t lsb);
float_t lsm6dsv16b_from_fs500_to_mdps(int16_t lsb);
float_t lsm6dsv16b_from_fs250_to_mdps(int16_t lsb);
float_t lsm6dsv16b_from_fs1000_to_mdps(int16_t lsb);
float_t lsm6dsv16b_from_fs2000_to_mdps(int16_t lsb);
float_t lsm6dsv16b_from_fs4000_to_mdps(int16_t lsb);

float_t lsm6dsv16b_from_lsb_to_celsius(int16_t lsb);

uint64_t lsm6dsv16b_from_lsb_to_nsec(uint32_t lsb);

float_t lsm6dsv16b_from_lsb_to_mv(int16_t lsb);

typedef enum
{
  LSM6DSV16B_READY                               = 0x0,
  LSM6DSV16B_GLOBAL_RST                          = 0x1,
  LSM6DSV16B_RESTORE_CAL_PARAM                   = 0x2,
  LSM6DSV16B_RESTORE_CTRL_REGS                   = 0x4,
} lsm6dsv16b_reset_t;
int32_t lsm6dsv16b_reset_set(const stmdev_ctx_t *ctx, lsm6dsv16b_reset_t val);
int32_t lsm6dsv16b_reset_get(const stmdev_ctx_t *ctx, lsm6dsv16b_reset_t *val);

typedef enum
{
  LSM6DSV16B_MAIN_MEM_BANK                       = 0x0,
  LSM6DSV16B_EMBED_FUNC_MEM_BANK                 = 0x1,
} lsm6dsv16b_mem_bank_t;
int32_t lsm6dsv16b_mem_bank_set(const stmdev_ctx_t *ctx, lsm6dsv16b_mem_bank_t val);
int32_t lsm6dsv16b_mem_bank_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_mem_bank_t *val);

int32_t lsm6dsv16b_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_XL_ODR_OFF                          = 0x0,
  LSM6DSV16B_XL_ODR_AT_1Hz875                    = 0x1,
  LSM6DSV16B_XL_ODR_AT_7Hz5                      = 0x2,
  LSM6DSV16B_XL_ODR_AT_15Hz                      = 0x3,
  LSM6DSV16B_XL_ODR_AT_30Hz                      = 0x4,
  LSM6DSV16B_XL_ODR_AT_60Hz                      = 0x5,
  LSM6DSV16B_XL_ODR_AT_120Hz                     = 0x6,
  LSM6DSV16B_XL_ODR_AT_240Hz                     = 0x7,
  LSM6DSV16B_XL_ODR_AT_480Hz                     = 0x8,
  LSM6DSV16B_XL_ODR_AT_960Hz                     = 0x9,
  LSM6DSV16B_XL_ODR_AT_1920Hz                    = 0xA,
  LSM6DSV16B_XL_ODR_AT_3840Hz                    = 0xB,
  LSM6DSV16B_XL_ODR_AT_7680Hz                    = 0xC,
} lsm6dsv16b_xl_data_rate_t;
int32_t lsm6dsv16b_xl_data_rate_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_data_rate_t val);
int32_t lsm6dsv16b_xl_data_rate_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_data_rate_t *val);

typedef enum
{
  LSM6DSV16B_XL_HIGH_PERFORMANCE_MD              = 0x0,
  LSM6DSV16B_XL_HIGH_PERFORMANCE_TDM_MD          = 0x2,
  LSM6DSV16B_XL_LOW_POWER_2_AVG_MD               = 0x4,
  LSM6DSV16B_XL_LOW_POWER_4_AVG_MD               = 0x5,
  LSM6DSV16B_XL_LOW_POWER_8_AVG_MD               = 0x6,
} lsm6dsv16b_xl_mode_t;
int32_t lsm6dsv16b_xl_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_xl_mode_t val);
int32_t lsm6dsv16b_xl_mode_get(const stmdev_ctx_t *ctx, lsm6dsv16b_xl_mode_t *val);

typedef enum
{
  LSM6DSV16B_GY_ODR_OFF                          = 0x0,
  LSM6DSV16B_GY_ODR_AT_7Hz5                      = 0x2,
  LSM6DSV16B_GY_ODR_AT_15Hz                      = 0x3,
  LSM6DSV16B_GY_ODR_AT_30Hz                      = 0x4,
  LSM6DSV16B_GY_ODR_AT_60Hz                      = 0x5,
  LSM6DSV16B_GY_ODR_AT_120Hz                     = 0x6,
  LSM6DSV16B_GY_ODR_AT_240Hz                     = 0x7,
  LSM6DSV16B_GY_ODR_AT_480Hz                     = 0x8,
  LSM6DSV16B_GY_ODR_AT_960Hz                     = 0x9,
  LSM6DSV16B_GY_ODR_AT_1920Hz                    = 0xa,
  LSM6DSV16B_GY_ODR_AT_3840Hz                    = 0xb,
  LSM6DSV16B_GY_ODR_AT_7680Hz                    = 0xc,
} lsm6dsv16b_gy_data_rate_t;
int32_t lsm6dsv16b_gy_data_rate_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_gy_data_rate_t val);
int32_t lsm6dsv16b_gy_data_rate_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_gy_data_rate_t *val);

typedef enum
{
  LSM6DSV16B_GY_HIGH_PERFORMANCE_MD              = 0x0,
  LSM6DSV16B_GY_SLEEP_MD                         = 0x4,
  LSM6DSV16B_GY_LOW_POWER_MD                     = 0x5,
} lsm6dsv16b_gy_mode_t;
int32_t lsm6dsv16b_gy_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_gy_mode_t val);
int32_t lsm6dsv16b_gy_mode_get(const stmdev_ctx_t *ctx, lsm6dsv16b_gy_mode_t *val);

int32_t lsm6dsv16b_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_auto_increment_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_DRDY_LATCHED                        = 0x0,
  LSM6DSV16B_DRDY_PULSED                         = 0x1,
} lsm6dsv16b_data_ready_mode_t;
int32_t lsm6dsv16b_data_ready_mode_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_data_ready_mode_t val);
int32_t lsm6dsv16b_data_ready_mode_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_data_ready_mode_t *val);

typedef enum
{
  LSM6DSV16B_125dps                              = 0x0,
  LSM6DSV16B_250dps                              = 0x1,
  LSM6DSV16B_500dps                              = 0x2,
  LSM6DSV16B_1000dps                             = 0x3,
  LSM6DSV16B_2000dps                             = 0x4,
  LSM6DSV16B_4000dps                             = 0xc,
} lsm6dsv16b_gy_full_scale_t;
int32_t lsm6dsv16b_gy_full_scale_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_full_scale_t val);
int32_t lsm6dsv16b_gy_full_scale_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_gy_full_scale_t *val);

typedef enum
{
  LSM6DSV16B_2g                                  = 0x0,
  LSM6DSV16B_4g                                  = 0x1,
  LSM6DSV16B_8g                                  = 0x2,
  LSM6DSV16B_16g                                 = 0x3,
} lsm6dsv16b_xl_full_scale_t;
int32_t lsm6dsv16b_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_full_scale_t val);
int32_t lsm6dsv16b_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_xl_full_scale_t *val);

int32_t lsm6dsv16b_xl_dual_channel_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_xl_dual_channel_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_XL_ST_DISABLE                       = 0x0,
  LSM6DSV16B_XL_ST_POSITIVE                      = 0x1,
  LSM6DSV16B_XL_ST_NEGATIVE                      = 0x2,
  LSM6DSV16B_XL_ST_OFFSET_POS                    = 0x5,
  LSM6DSV16B_XL_ST_OFFSET_NEG                    = 0x6,
} lsm6dsv16b_xl_self_test_t;
int32_t lsm6dsv16b_xl_self_test_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_self_test_t val);
int32_t lsm6dsv16b_xl_self_test_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_self_test_t *val);

typedef enum
{
  LSM6DSV16B_GY_ST_DISABLE                       = 0x0,
  LSM6DSV16B_GY_ST_POSITIVE                      = 0x1,
  LSM6DSV16B_GY_ST_NEGATIVE                      = 0x2,
} lsm6dsv16b_gy_self_test_t;
int32_t lsm6dsv16b_gy_self_test_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_gy_self_test_t val);
int32_t lsm6dsv16b_gy_self_test_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_gy_self_test_t *val);

typedef struct
{
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
  uint8_t gy_settling                   : 1;
  uint8_t den_flag                      : 1;
  uint8_t timestamp                     : 1;
  uint8_t free_fall                     : 1;
  uint8_t wake_up                       : 1;
  uint8_t wake_up_z                     : 1;
  uint8_t wake_up_y                     : 1;
  uint8_t wake_up_x                     : 1;
  uint8_t single_tap                    : 1;
  uint8_t double_tap                    : 1;
  uint8_t tap_z                         : 1;
  uint8_t tap_y                         : 1;
  uint8_t tap_x                         : 1;
  uint8_t tap_sign                      : 1;
  uint8_t six_d                         : 1;
  uint8_t six_d_xl                      : 1;
  uint8_t six_d_xh                      : 1;
  uint8_t six_d_yl                      : 1;
  uint8_t six_d_yh                      : 1;
  uint8_t six_d_zl                      : 1;
  uint8_t six_d_zh                      : 1;
  uint8_t sleep_change                  : 1;
  uint8_t sleep_state                   : 1;
  uint8_t step_detector                 : 1;
  uint8_t step_count_inc                : 1;
  uint8_t step_count_overflow           : 1;
  uint8_t step_on_delta_time            : 1;
  uint8_t emb_func_stand_by             : 1;
  uint8_t emb_func_time_exceed: 1;
  uint8_t tilt                          : 1;
  uint8_t sig_mot                       : 1;
  uint8_t fsm_lc                        : 1;
  uint8_t fsm1                          : 1;
  uint8_t fsm2                          : 1;
  uint8_t fsm3                          : 1;
  uint8_t fsm4                          : 1;
  uint8_t fsm5                          : 1;
  uint8_t fsm6                          : 1;
  uint8_t fsm7                          : 1;
  uint8_t fsm8                          : 1;
  uint8_t fifo_bdr                      : 1;
  uint8_t fifo_full                     : 1;
  uint8_t fifo_ovr                      : 1;
  uint8_t fifo_th                       : 1;
} lsm6dsv16b_all_sources_t;
int32_t lsm6dsv16b_all_sources_get(const stmdev_ctx_t *ctx,
                                   lsm6dsv16b_all_sources_t *val);

typedef struct
{
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
} lsm6dsv16b_data_ready_t;
int32_t lsm6dsv16b_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_data_ready_t *val);

int32_t lsm6dsv16b_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dsv16b_angular_rate_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dsv16b_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dsv16b_dual_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm6dsv16b_dual_acceleration_raw_get(const stmdev_ctx_t *ctx,
                                             int16_t *val);

int32_t lsm6dsv16b_odr_cal_reg_get(const stmdev_ctx_t *ctx, int8_t *val);

typedef struct
{
  uint8_t x                             : 1;
  uint8_t y                             : 1;
  uint8_t z                             : 1;
} lsm6dsv16b_tdm_xl_axis_t;
int32_t lsm6dsv16b_tdm_xl_axis_set(const stmdev_ctx_t *ctx, lsm6dsv16b_tdm_xl_axis_t val);
int32_t lsm6dsv16b_tdm_xl_axis_get(const stmdev_ctx_t *ctx, lsm6dsv16b_tdm_xl_axis_t *val);

int32_t lsm6dsv16b_ln_pg_write(const stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len);
int32_t lsm6dsv16b_ln_pg_read(const stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *buf, uint8_t len);

int32_t lsm6dsv16b_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val);

typedef enum
{
  LSM6DSV16B_AUTO                                = 0x0,
  LSM6DSV16B_ALWAYS_ACTIVE                       = 0x1,
} lsm6dsv16b_filt_anti_spike_t;
int32_t lsm6dsv16b_filt_anti_spike_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_anti_spike_t val);
int32_t lsm6dsv16b_filt_anti_spike_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_anti_spike_t *val);

typedef struct
{
  uint8_t drdy                          : 1;
  uint8_t irq_xl                        : 1;
  uint8_t irq_g                         : 1;
  uint8_t tdm_excep_code                : 1;
} lsm6dsv16b_filt_settling_mask_t;
int32_t lsm6dsv16b_filt_settling_mask_set(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_filt_settling_mask_t val);
int32_t lsm6dsv16b_filt_settling_mask_get(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_filt_settling_mask_t *val);

typedef enum
{
  LSM6DSV16B_GY_ULTRA_LIGHT                      = 0x0,
  LSM6DSV16B_GY_VERY_LIGHT                       = 0x1,
  LSM6DSV16B_GY_LIGHT                            = 0x2,
  LSM6DSV16B_GY_MEDIUM                           = 0x3,
  LSM6DSV16B_GY_STRONG                           = 0x4,
  LSM6DSV16B_GY_VERY_STRONG                      = 0x5,
  LSM6DSV16B_GY_AGGRESSIVE                       = 0x6,
  LSM6DSV16B_GY_XTREME                           = 0x7,
} lsm6dsv16b_filt_gy_lp1_bandwidth_t;
int32_t lsm6dsv16b_filt_gy_lp1_bandwidth_set(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_filt_gy_lp1_bandwidth_t val);
int32_t lsm6dsv16b_filt_gy_lp1_bandwidth_get(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_filt_gy_lp1_bandwidth_t *val);

int32_t lsm6dsv16b_filt_gy_lp1_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_filt_gy_lp1_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_XL_ULTRA_LIGHT                      = 0x0,
  LSM6DSV16B_XL_VERY_LIGHT                       = 0x1,
  LSM6DSV16B_XL_LIGHT                            = 0x2,
  LSM6DSV16B_XL_MEDIUM                           = 0x3,
  LSM6DSV16B_XL_STRONG                           = 0x4,
  LSM6DSV16B_XL_VERY_STRONG                      = 0x5,
  LSM6DSV16B_XL_AGGRESSIVE                       = 0x6,
  LSM6DSV16B_XL_XTREME                           = 0x7,
} lsm6dsv16b_filt_xl_lp2_bandwidth_t;
int32_t lsm6dsv16b_filt_xl_lp2_bandwidth_set(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_filt_xl_lp2_bandwidth_t val);
int32_t lsm6dsv16b_filt_xl_lp2_bandwidth_get(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_filt_xl_lp2_bandwidth_t *val);

int32_t lsm6dsv16b_filt_xl_lp2_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_filt_xl_lp2_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_filt_xl_hp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_filt_xl_hp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_filt_xl_fast_settling_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_filt_xl_fast_settling_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_HP_MD_NORMAL                        = 0x0,
  LSM6DSV16B_HP_MD_REFERENCE                     = 0x1,
} lsm6dsv16b_filt_xl_hp_mode_t;
int32_t lsm6dsv16b_filt_xl_hp_mode_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_xl_hp_mode_t val);
int32_t lsm6dsv16b_filt_xl_hp_mode_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_filt_xl_hp_mode_t *val);

typedef enum
{
  LSM6DSV16B_WK_FEED_SLOPE                       = 0x0,
  LSM6DSV16B_WK_FEED_HIGH_PASS                   = 0x1,
  LSM6DSV16B_WK_FEED_LP_WITH_OFFSET              = 0x2,
} lsm6dsv16b_filt_wkup_act_feed_t;
int32_t lsm6dsv16b_filt_wkup_act_feed_set(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_filt_wkup_act_feed_t val);
int32_t lsm6dsv16b_filt_wkup_act_feed_get(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_filt_wkup_act_feed_t *val);

int32_t lsm6dsv16b_mask_trigger_xl_settl_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_mask_trigger_xl_settl_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_SIXD_FEED_ODR_DIV_2                 = 0x0,
  LSM6DSV16B_SIXD_FEED_LOW_PASS                  = 0x1,
} lsm6dsv16b_filt_sixd_feed_t;
int32_t lsm6dsv16b_filt_sixd_feed_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_filt_sixd_feed_t val);
int32_t lsm6dsv16b_filt_sixd_feed_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_filt_sixd_feed_t *val);

int32_t lsm6dsv16b_ui_sdo_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_ui_sdo_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_I2C_I3C_ENABLE                      = 0x0,
  LSM6DSV16B_I2C_I3C_DISABLE                     = 0x1,
} lsm6dsv16b_ui_i2c_i3c_mode_t;
int32_t lsm6dsv16b_ui_i2c_i3c_mode_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_ui_i2c_i3c_mode_t val);
int32_t lsm6dsv16b_ui_i2c_i3c_mode_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_ui_i2c_i3c_mode_t *val);

typedef enum
{
  LSM6DSV16B_SPI_4_WIRE                          = 0x0,
  LSM6DSV16B_SPI_3_WIRE                          = 0x1,
} lsm6dsv16b_spi_mode_t;
int32_t lsm6dsv16b_spi_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_spi_mode_t val);
int32_t lsm6dsv16b_spi_mode_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_spi_mode_t *val);

int32_t lsm6dsv16b_ui_sda_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_ui_sda_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_IBI_2us                             = 0x0,
  LSM6DSV16B_IBI_50us                            = 0x1,
  LSM6DSV16B_IBI_1ms                             = 0x2,
  LSM6DSV16B_IBI_25ms                            = 0x3,
} lsm6dsv16b_i3c_ibi_time_t;
int32_t lsm6dsv16b_i3c_ibi_time_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_i3c_ibi_time_t val);
int32_t lsm6dsv16b_i3c_ibi_time_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_i3c_ibi_time_t *val);

typedef enum
{
  LSM6DSV16B_PUSH_PULL                           = 0x0,
  LSM6DSV16B_OPEN_DRAIN                          = 0x1,
} lsm6dsv16b_int_pin_mode_t;
int32_t lsm6dsv16b_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_int_pin_mode_t val);
int32_t lsm6dsv16b_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_int_pin_mode_t *val);

typedef enum
{
  LSM6DSV16B_ACTIVE_HIGH                         = 0x0,
  LSM6DSV16B_ACTIVE_LOW                          = 0x1,
} lsm6dsv16b_pin_polarity_t;
int32_t lsm6dsv16b_pin_polarity_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_pin_polarity_t val);
int32_t lsm6dsv16b_pin_polarity_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_pin_polarity_t *val);

typedef struct
{
  uint8_t boot                          : 1;
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
  uint8_t fifo_th                       : 1;
  uint8_t fifo_ovr                      : 1;
  uint8_t fifo_full                     : 1;
  uint8_t fifo_bdr                      : 1;
  uint8_t den_flag                      : 1;
  uint8_t timestamp                     : 1; // impact on int2 signals
  uint8_t six_d                         : 1;
  uint8_t double_tap                    : 1;
  uint8_t free_fall                     : 1;
  uint8_t wake_up                       : 1;
  uint8_t single_tap                    : 1;
  uint8_t sleep_change                  : 1;
  uint8_t sleep_status                  : 1;
  uint8_t step_detector                 : 1;
  uint8_t step_count_overflow           : 1;
  uint8_t tilt                          : 1;
  uint8_t sig_mot                       : 1;
  uint8_t emb_func_stand_by             : 1; // impact on int2 signals
  uint8_t fsm_lc                        : 1;
  uint8_t fsm1                          : 1;
  uint8_t fsm2                          : 1;
  uint8_t fsm3                          : 1;
  uint8_t fsm4                          : 1;
  uint8_t fsm5                          : 1;
  uint8_t fsm6                          : 1;
  uint8_t fsm7                          : 1;
  uint8_t fsm8                          : 1;
} lsm6dsv16b_pin_int_route_t;
int32_t lsm6dsv16b_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_pin_int_route_t val);
int32_t lsm6dsv16b_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_pin_int_route_t *val);
int32_t lsm6dsv16b_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_pin_int_route_t val);
int32_t lsm6dsv16b_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_pin_int_route_t *val);

int32_t lsm6dsv16b_pin_int_en_when_i2c_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_pin_int_en_when_i2c_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_ALL_INT_PULSED                      = 0x0,
  LSM6DSV16B_BASE_LATCHED_EMB_PULSED             = 0x1,
  LSM6DSV16B_BASE_PULSED_EMB_LATCHED             = 0x2,
  LSM6DSV16B_ALL_INT_LATCHED                     = 0x3,
} lsm6dsv16b_int_notification_t;
int32_t lsm6dsv16b_int_notification_set(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_int_notification_t val);
int32_t lsm6dsv16b_int_notification_get(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_int_notification_t *val);

typedef enum
{
  LSM6DSV16B_XL_AND_GY_NOT_AFFECTED              = 0x0,
  LSM6DSV16B_XL_LOW_POWER_GY_NOT_AFFECTED        = 0x1,
  LSM6DSV16B_XL_LOW_POWER_GY_SLEEP               = 0x2,
  LSM6DSV16B_XL_LOW_POWER_GY_POWER_DOWN          = 0x3,
} lsm6dsv16b_act_mode_t;
int32_t lsm6dsv16b_act_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_act_mode_t val);
int32_t lsm6dsv16b_act_mode_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_act_mode_t *val);

typedef enum
{
  LSM6DSV16B_SLEEP_TO_ACT_AT_1ST_SAMPLE          = 0x0,
  LSM6DSV16B_SLEEP_TO_ACT_AT_2ND_SAMPLE          = 0x1,
  LSM6DSV16B_SLEEP_TO_ACT_AT_3RD_SAMPLE          = 0x2,
  LSM6DSV16B_SLEEP_TO_ACT_AT_4th_SAMPLE          = 0x3,
} lsm6dsv16b_act_from_sleep_to_act_dur_t;
int32_t lsm6dsv16b_act_from_sleep_to_act_dur_set(const stmdev_ctx_t *ctx,
                                                 lsm6dsv16b_act_from_sleep_to_act_dur_t val);
int32_t lsm6dsv16b_act_from_sleep_to_act_dur_get(const stmdev_ctx_t *ctx,
                                                 lsm6dsv16b_act_from_sleep_to_act_dur_t *val);

typedef enum
{
  LSM6DSV16B_1Hz875                              = 0x0,
  LSM6DSV16B_15Hz                                = 0x1,
  LSM6DSV16B_30Hz                                = 0x2,
  LSM6DSV16B_60Hz                                = 0x3,
} lsm6dsv16b_act_sleep_xl_odr_t;
int32_t lsm6dsv16b_act_sleep_xl_odr_set(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_act_sleep_xl_odr_t val);
int32_t lsm6dsv16b_act_sleep_xl_odr_get(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_act_sleep_xl_odr_t *val);

typedef struct
{
  uint32_t wk_ths_mg;
  uint32_t inact_ths_mg;
} lsm6dsv16b_act_thresholds_t;
int32_t lsm6dsv16b_act_thresholds_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_act_thresholds_t val);
int32_t lsm6dsv16b_act_thresholds_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_act_thresholds_t *val);

typedef struct
{
  uint8_t shock                         : 2;
  uint8_t quiet                         : 4;
} lsm6dsv16b_act_wkup_time_windows_t;
int32_t lsm6dsv16b_act_wkup_time_windows_set(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_act_wkup_time_windows_t val);
int32_t lsm6dsv16b_act_wkup_time_windows_get(const stmdev_ctx_t *ctx,
                                             lsm6dsv16b_act_wkup_time_windows_t *val);

typedef struct
{
  uint8_t tap_x_en                      : 1;
  uint8_t tap_y_en                      : 1;
  uint8_t tap_z_en                      : 1;
} lsm6dsv16b_tap_detection_t;
int32_t lsm6dsv16b_tap_detection_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tap_detection_t val);
int32_t lsm6dsv16b_tap_detection_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tap_detection_t *val);

typedef struct
{
  uint8_t x                             : 1;
  uint8_t y                             : 1;
  uint8_t z                             : 1;
} lsm6dsv16b_tap_thresholds_t;
int32_t lsm6dsv16b_tap_thresholds_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tap_thresholds_t val);
int32_t lsm6dsv16b_tap_thresholds_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tap_thresholds_t *val);


typedef enum
{
  LSM6DSV16B_XYZ                                 = 0x3,
  LSM6DSV16B_YXZ                                 = 0x5,
  LSM6DSV16B_XZY                                 = 0x6,
  LSM6DSV16B_ZYX                                 = 0x0,
  LSM6DSV16B_YZX                                 = 0x1,
  LSM6DSV16B_ZXY                                 = 0x2,
} lsm6dsv16b_tap_axis_priority_t;
int32_t lsm6dsv16b_tap_axis_priority_set(const stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tap_axis_priority_t val);
int32_t lsm6dsv16b_tap_axis_priority_get(const stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tap_axis_priority_t *val);

typedef struct
{
  uint8_t shock                         : 1;
  uint8_t quiet                         : 1;
  uint8_t tap_gap                       : 1;
} lsm6dsv16b_tap_time_windows_t;
int32_t lsm6dsv16b_tap_time_windows_set(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_tap_time_windows_t val);
int32_t lsm6dsv16b_tap_time_windows_get(const stmdev_ctx_t *ctx,
                                        lsm6dsv16b_tap_time_windows_t *val);

typedef enum
{
  LSM6DSV16B_ONLY_SINGLE                         = 0x0,
  LSM6DSV16B_BOTH_SINGLE_DOUBLE                  = 0x1,
} lsm6dsv16b_tap_mode_t;
int32_t lsm6dsv16b_tap_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_tap_mode_t val);
int32_t lsm6dsv16b_tap_mode_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_tap_mode_t *val);

typedef enum
{
  LSM6DSV16B_DEG_80                              = 0x0,
  LSM6DSV16B_DEG_70                              = 0x1,
  LSM6DSV16B_DEG_60                              = 0x2,
  LSM6DSV16B_DEG_50                              = 0x3,
} lsm6dsv16b_6d_threshold_t;
int32_t lsm6dsv16b_6d_threshold_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_6d_threshold_t val);
int32_t lsm6dsv16b_6d_threshold_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_6d_threshold_t *val);

int32_t lsm6dsv16b_ff_time_windows_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_ff_time_windows_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_156_mg                              = 0x0,
  LSM6DSV16B_219_mg                              = 0x1,
  LSM6DSV16B_250_mg                              = 0x2,
  LSM6DSV16B_312_mg                              = 0x3,
  LSM6DSV16B_344_mg                              = 0x4,
  LSM6DSV16B_406_mg                              = 0x5,
  LSM6DSV16B_469_mg                              = 0x6,
  LSM6DSV16B_500_mg                              = 0x7,
} lsm6dsv16b_ff_thresholds_t;
int32_t lsm6dsv16b_ff_thresholds_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_ff_thresholds_t val);
int32_t lsm6dsv16b_ff_thresholds_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_ff_thresholds_t *val);

int32_t lsm6dsv16b_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_fifo_xl_dual_fsm_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_fifo_xl_dual_fsm_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_CMP_DISABLE                         = 0x0,
  LSM6DSV16B_CMP_8_TO_1                          = 0x1,
  LSM6DSV16B_CMP_16_TO_1                         = 0x2,
  LSM6DSV16B_CMP_32_TO_1                         = 0x3,
} lsm6dsv16b_fifo_compress_algo_t;
int32_t lsm6dsv16b_fifo_compress_algo_set(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_fifo_compress_algo_t val);
int32_t lsm6dsv16b_fifo_compress_algo_get(const stmdev_ctx_t *ctx,
                                          lsm6dsv16b_fifo_compress_algo_t *val);

int32_t lsm6dsv16b_fifo_virtual_sens_odr_chg_set(const stmdev_ctx_t *ctx,
                                                 uint8_t val);
int32_t lsm6dsv16b_fifo_virtual_sens_odr_chg_get(const stmdev_ctx_t *ctx,
                                                 uint8_t *val);

int32_t lsm6dsv16b_fifo_compress_algo_real_time_set(const stmdev_ctx_t *ctx,
                                                    uint8_t val);
int32_t lsm6dsv16b_fifo_compress_algo_real_time_get(const stmdev_ctx_t *ctx,
                                                    uint8_t *val);

int32_t lsm6dsv16b_fifo_stop_on_wtm_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_fifo_stop_on_wtm_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_XL_NOT_BATCHED                      = 0x0,
  LSM6DSV16B_XL_BATCHED_AT_1Hz875                = 0x1,
  LSM6DSV16B_XL_BATCHED_AT_7Hz5                  = 0x2,
  LSM6DSV16B_XL_BATCHED_AT_15Hz                  = 0x3,
  LSM6DSV16B_XL_BATCHED_AT_30Hz                  = 0x4,
  LSM6DSV16B_XL_BATCHED_AT_60Hz                  = 0x5,
  LSM6DSV16B_XL_BATCHED_AT_120Hz                 = 0x6,
  LSM6DSV16B_XL_BATCHED_AT_240Hz                 = 0x7,
  LSM6DSV16B_XL_BATCHED_AT_480Hz                 = 0x8,
  LSM6DSV16B_XL_BATCHED_AT_960Hz                 = 0x9,
  LSM6DSV16B_XL_BATCHED_AT_1920Hz                = 0xA,
  LSM6DSV16B_XL_BATCHED_AT_3840Hz                = 0xB,
  LSM6DSV16B_XL_BATCHED_AT_7680Hz                = 0xC,
} lsm6dsv16b_fifo_xl_batch_t;
int32_t lsm6dsv16b_fifo_xl_batch_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fifo_xl_batch_t val);
int32_t lsm6dsv16b_fifo_xl_batch_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fifo_xl_batch_t *val);

typedef enum
{
  LSM6DSV16B_GY_NOT_BATCHED                      = 0x0,
  LSM6DSV16B_GY_BATCHED_AT_1Hz875                = 0x1,
  LSM6DSV16B_GY_BATCHED_AT_7Hz5                  = 0x2,
  LSM6DSV16B_GY_BATCHED_AT_15Hz                  = 0x3,
  LSM6DSV16B_GY_BATCHED_AT_30Hz                  = 0x4,
  LSM6DSV16B_GY_BATCHED_AT_60Hz                  = 0x5,
  LSM6DSV16B_GY_BATCHED_AT_120Hz                 = 0x6,
  LSM6DSV16B_GY_BATCHED_AT_240Hz                 = 0x7,
  LSM6DSV16B_GY_BATCHED_AT_480Hz                 = 0x8,
  LSM6DSV16B_GY_BATCHED_AT_960Hz                 = 0x9,
  LSM6DSV16B_GY_BATCHED_AT_1920Hz                = 0xa,
  LSM6DSV16B_GY_BATCHED_AT_3840Hz                = 0xb,
  LSM6DSV16B_GY_BATCHED_AT_7680Hz                = 0xc,
} lsm6dsv16b_fifo_gy_batch_t;
int32_t lsm6dsv16b_fifo_gy_batch_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fifo_gy_batch_t val);
int32_t lsm6dsv16b_fifo_gy_batch_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fifo_gy_batch_t *val);

typedef enum
{
  LSM6DSV16B_BYPASS_MODE                         = 0x0,
  LSM6DSV16B_FIFO_MODE                           = 0x1,
  LSM6DSV16B_STREAM_WTM_TO_FULL_MODE             = 0x2,
  LSM6DSV16B_STREAM_TO_FIFO_MODE                 = 0x3,
  LSM6DSV16B_BYPASS_TO_STREAM_MODE               = 0x4,
  LSM6DSV16B_STREAM_MODE                         = 0x6,
  LSM6DSV16B_BYPASS_TO_FIFO_MODE                 = 0x7,
} lsm6dsv16b_fifo_mode_t;
int32_t lsm6dsv16b_fifo_mode_set(const stmdev_ctx_t *ctx,
                                 lsm6dsv16b_fifo_mode_t val);
int32_t lsm6dsv16b_fifo_mode_get(const stmdev_ctx_t *ctx,
                                 lsm6dsv16b_fifo_mode_t *val);

typedef enum
{
  LSM6DSV16B_TEMP_NOT_BATCHED                    = 0x0,
  LSM6DSV16B_TEMP_BATCHED_AT_1Hz875              = 0x1,
  LSM6DSV16B_TEMP_BATCHED_AT_15Hz                = 0x2,
  LSM6DSV16B_TEMP_BATCHED_AT_60Hz                = 0x3,
} lsm6dsv16b_fifo_temp_batch_t;
int32_t lsm6dsv16b_fifo_temp_batch_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fifo_temp_batch_t val);
int32_t lsm6dsv16b_fifo_temp_batch_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fifo_temp_batch_t *val);

typedef enum
{
  LSM6DSV16B_TMSTMP_NOT_BATCHED                  = 0x0,
  LSM6DSV16B_TMSTMP_DEC_1                        = 0x1,
  LSM6DSV16B_TMSTMP_DEC_8                        = 0x2,
  LSM6DSV16B_TMSTMP_DEC_32                       = 0x3,
} lsm6dsv16b_fifo_timestamp_batch_t;
int32_t lsm6dsv16b_fifo_timestamp_batch_set(const stmdev_ctx_t *ctx,
                                            lsm6dsv16b_fifo_timestamp_batch_t val);
int32_t lsm6dsv16b_fifo_timestamp_batch_get(const stmdev_ctx_t *ctx,
                                            lsm6dsv16b_fifo_timestamp_batch_t *val);

int32_t lsm6dsv16b_fifo_batch_counter_threshold_set(const stmdev_ctx_t *ctx,
                                                    uint16_t val);
int32_t lsm6dsv16b_fifo_batch_counter_threshold_get(const stmdev_ctx_t *ctx,
                                                    uint16_t *val);

typedef enum
{
  LSM6DSV16B_XL_BATCH_EVENT                      = 0x0,
  LSM6DSV16B_GY_BATCH_EVENT                      = 0x1,
} lsm6dsv16b_fifo_batch_cnt_event_t;
int32_t lsm6dsv16b_fifo_batch_cnt_event_set(const stmdev_ctx_t *ctx,
                                            lsm6dsv16b_fifo_batch_cnt_event_t val);
int32_t lsm6dsv16b_fifo_batch_cnt_event_get(const stmdev_ctx_t *ctx,
                                            lsm6dsv16b_fifo_batch_cnt_event_t *val);

typedef struct
{
  uint8_t game_rotation                 : 1;
  uint8_t gravity                       : 1;
  uint8_t gbias                         : 1;
} lsm6dsv16b_fifo_sflp_raw_t;
int32_t lsm6dsv16b_fifo_sflp_batch_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fifo_sflp_raw_t val);
int32_t lsm6dsv16b_fifo_sflp_batch_get(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_fifo_sflp_raw_t *val);

typedef struct
{
  uint16_t fifo_level                   : 9;
  uint8_t fifo_bdr                      : 1;
  uint8_t fifo_full                     : 1;
  uint8_t fifo_ovr                      : 1;
  uint8_t fifo_th                       : 1;
} lsm6dsv16b_fifo_status_t;

int32_t lsm6dsv16b_fifo_status_get(const stmdev_ctx_t *ctx,
                                   lsm6dsv16b_fifo_status_t *val);

typedef struct
{
  enum
  {
    LSM6DSV16B_FIFO_EMPTY                        = 0x0,
    LSM6DSV16B_GY_NC_TAG                         = 0x1,
    LSM6DSV16B_XL_NC_TAG                         = 0x2,
    LSM6DSV16B_TEMPERATURE_TAG                   = 0x3,
    LSM6DSV16B_TIMESTAMP_TAG                     = 0x4,
    LSM6DSV16B_CFG_CHANGE_TAG                    = 0x5,
    LSM6DSV16B_XL_NC_T_2_TAG                     = 0x6,
    LSM6DSV16B_XL_NC_T_1_TAG                     = 0x7,
    LSM6DSV16B_XL_2XC_TAG                        = 0x8,
    LSM6DSV16B_XL_3XC_TAG                        = 0x9,
    LSM6DSV16B_GY_NC_T_2_TAG                     = 0xA,
    LSM6DSV16B_GY_NC_T_1_TAG                     = 0xB,
    LSM6DSV16B_GY_2XC_TAG                        = 0xC,
    LSM6DSV16B_GY_3XC_TAG                        = 0xD,
    LSM6DSV16B_STEP_COUNTER_TAG                  = 0x12,
    LSM6DSV16B_SFLP_GAME_ROTATION_VECTOR_TAG     = 0x13,
    LSM6DSV16B_SFLP_GYROSCOPE_BIAS_TAG           = 0x16,
    LSM6DSV16B_SFLP_GRAVITY_VECTOR_TAG           = 0x17,
    LSM6DSV16B_XL_DUAL_CORE                      = 0x1D,
  } tag;
  uint8_t cnt;
  uint8_t data[6];
} lsm6dsv16b_fifo_out_raw_t;
int32_t lsm6dsv16b_fifo_out_raw_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_fifo_out_raw_t *val);

int32_t lsm6dsv16b_fifo_stpcnt_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_fifo_stpcnt_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  uint8_t step_counter_enable           : 1;
  uint8_t false_step_rej                : 1;
} lsm6dsv16b_stpcnt_mode_t;
int32_t lsm6dsv16b_stpcnt_mode_set(const stmdev_ctx_t *ctx,
                                   lsm6dsv16b_stpcnt_mode_t val);
int32_t lsm6dsv16b_stpcnt_mode_get(const stmdev_ctx_t *ctx,
                                   lsm6dsv16b_stpcnt_mode_t *val);

int32_t lsm6dsv16b_stpcnt_steps_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t lsm6dsv16b_stpcnt_rst_step_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_stpcnt_rst_step_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_stpcnt_debounce_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_stpcnt_debounce_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_stpcnt_period_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dsv16b_stpcnt_period_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t lsm6dsv16b_sigmot_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_sigmot_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_tilt_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_tilt_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_sflp_game_rotation_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dsv16b_sflp_game_rotation_get(const stmdev_ctx_t *ctx, uint16_t *val);

typedef struct
{
  float_t gbias_x; /* dps */
  float_t gbias_y; /* dps */
  float_t gbias_z; /* dps */
} lsm6dsv16b_sflp_gbias_t;
int32_t lsm6dsv16b_sflp_game_gbias_set(const stmdev_ctx_t *ctx,
                                       lsm6dsv16b_sflp_gbias_t *val);

int32_t lsm6dsv16b_sflp_configure(const stmdev_ctx_t *ctx);

typedef enum
{
  LSM6DSV16B_SFLP_15Hz                           = 0x0,
  LSM6DSV16B_SFLP_30Hz                           = 0x1,
  LSM6DSV16B_SFLP_60Hz                           = 0x2,
  LSM6DSV16B_SFLP_120Hz                          = 0x3,
  LSM6DSV16B_SFLP_240Hz                          = 0x4,
  LSM6DSV16B_SFLP_480Hz                          = 0x5,
} lsm6dsv16b_sflp_data_rate_t;
int32_t lsm6dsv16b_sflp_data_rate_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_sflp_data_rate_t val);
int32_t lsm6dsv16b_sflp_data_rate_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_sflp_data_rate_t *val);

typedef enum
{
  LSM6DSV16B_PROTECT_CTRL_REGS                   = 0x0,
  LSM6DSV16B_WRITE_CTRL_REG                      = 0x1,
} lsm6dsv16b_fsm_permission_t;
int32_t lsm6dsv16b_fsm_permission_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fsm_permission_t val);
int32_t lsm6dsv16b_fsm_permission_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_fsm_permission_t *val);

typedef enum
{
  LSM6DSV16B_STD_IF_CONTROL                      = 0x0,
  LSM6DSV16B_FSM_CONTROL                         = 0x1,
} lsm6dsv16b_fsm_permission_status_t;
int32_t lsm6dsv16b_fsm_permission_status(const stmdev_ctx_t *ctx,
                                         lsm6dsv16b_fsm_permission_status_t *val);

typedef struct
{
  uint8_t fsm1_en                       : 1;
  uint8_t fsm2_en                       : 1;
  uint8_t fsm3_en                       : 1;
  uint8_t fsm4_en                       : 1;
  uint8_t fsm5_en                       : 1;
  uint8_t fsm6_en                       : 1;
  uint8_t fsm7_en                       : 1;
  uint8_t fsm8_en                       : 1;
} lsm6dsv16b_fsm_mode_t;
int32_t lsm6dsv16b_fsm_mode_set(const stmdev_ctx_t *ctx, lsm6dsv16b_fsm_mode_t val);
int32_t lsm6dsv16b_fsm_mode_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_fsm_mode_t *val);

int32_t lsm6dsv16b_fsm_long_cnt_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dsv16b_fsm_long_cnt_get(const stmdev_ctx_t *ctx, uint16_t *val);

typedef struct
{
  uint8_t fsm_outs1;
  uint8_t fsm_outs2;
  uint8_t fsm_outs3;
  uint8_t fsm_outs4;
  uint8_t fsm_outs5;
  uint8_t fsm_outs6;
  uint8_t fsm_outs7;
  uint8_t fsm_outs8;
} lsm6dsv16b_fsm_out_t;
int32_t lsm6dsv16b_fsm_out_get(const stmdev_ctx_t *ctx, lsm6dsv16b_fsm_out_t *val);

typedef enum
{
  LSM6DSV16B_FSM_15Hz                            = 0x0,
  LSM6DSV16B_FSM_30Hz                            = 0x1,
  LSM6DSV16B_FSM_60Hz                            = 0x2,
  LSM6DSV16B_FSM_120Hz                           = 0x3,
  LSM6DSV16B_FSM_240Hz                           = 0x4,
  LSM6DSV16B_FSM_480Hz                           = 0x5,
  LSM6DSV16B_FSM_960Hz                           = 0x6,
} lsm6dsv16b_fsm_data_rate_t;
int32_t lsm6dsv16b_fsm_data_rate_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fsm_data_rate_t val);
int32_t lsm6dsv16b_fsm_data_rate_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_fsm_data_rate_t *val);

int32_t lsm6dsv16b_fsm_long_cnt_timeout_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dsv16b_fsm_long_cnt_timeout_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t lsm6dsv16b_fsm_number_of_programs_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_fsm_number_of_programs_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_fsm_start_address_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t lsm6dsv16b_fsm_start_address_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t lsm6dsv16b_xl_offset_on_out_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_xl_offset_on_out_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  float_t z_mg;
  float_t y_mg;
  float_t x_mg;
} lsm6dsv16b_xl_offset_mg_t;
int32_t lsm6dsv16b_xl_offset_mg_set(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_offset_mg_t val);
int32_t lsm6dsv16b_xl_offset_mg_get(const stmdev_ctx_t *ctx,
                                    lsm6dsv16b_xl_offset_mg_t *val);

typedef enum
{
  LSM6DSV16B_SW_RST_DYN_ADDRESS_RST              = 0x0,
  LSM6DSV16B_I3C_GLOBAL_RST                      = 0x1,
} lsm6dsv16b_i3c_reset_mode_t;
int32_t lsm6dsv16b_i3c_reset_mode_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_i3c_reset_mode_t val);
int32_t lsm6dsv16b_i3c_reset_mode_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_i3c_reset_mode_t *val);

int32_t lsm6dsv16b_tdm_dis_wclk_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_tdm_dis_wclk_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm6dsv16b_tdm_tdmout_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_tdm_tdmout_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_WCLK_16kHZ_BCLK_2048kHz             = 0x1,
  LSM6DSV16B_WCLK_8kHZ_BCLK_2048kHz              = 0x4,
} lsm6dsv16b_tdm_wclk_bclk_t;
int32_t lsm6dsv16b_tdm_wclk_bclk_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tdm_wclk_bclk_t val);
int32_t lsm6dsv16b_tdm_wclk_bclk_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tdm_wclk_bclk_t *val);

typedef enum
{
  LSM6DSV16B_SLOT_012                            = 0x0,
  LSM6DSV16B_SLOT_456                            = 0x1,
} lsm6dsv16b_tdm_slot_t;
int32_t lsm6dsv16b_tdm_slot_set(const stmdev_ctx_t *ctx, lsm6dsv16b_tdm_slot_t val);
int32_t lsm6dsv16b_tdm_slot_get(const stmdev_ctx_t *ctx,
                                lsm6dsv16b_tdm_slot_t *val);

typedef enum
{
  LSM6DSV16B_BCLK_RISING                         = 0x0,
  LSM6DSV16B_BCLK_FALLING                        = 0x1,
} lsm6dsv16b_tdm_bclk_edge_t;
int32_t lsm6dsv16b_tdm_bclk_edge_set(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tdm_bclk_edge_t val);
int32_t lsm6dsv16b_tdm_bclk_edge_get(const stmdev_ctx_t *ctx,
                                     lsm6dsv16b_tdm_bclk_edge_t *val);

int32_t lsm6dsv16b_tdm_delayed_conf_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm6dsv16b_tdm_delayed_conf_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  LSM6DSV16B_TDM_ORDER_ZYX                       = 0x0,
  LSM6DSV16B_TDM_ORDER_XZY                       = 0x1,
  LSM6DSV16B_TDM_ORDER_XYZ                       = 0x2,
} lsm6dsv16b_tdm_axis_order_t;
int32_t lsm6dsv16b_tdm_axis_order_set(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_axis_order_t val);
int32_t lsm6dsv16b_tdm_axis_order_get(const stmdev_ctx_t *ctx,
                                      lsm6dsv16b_tdm_axis_order_t *val);

typedef enum
{
  LSM6DSV16B_TDM_2g                              = 0x0,
  LSM6DSV16B_TDM_4g                              = 0x1,
  LSM6DSV16B_TDM_8g                              = 0x2,
} lsm6dsv16b_tdm_xl_full_scale_t;
int32_t lsm6dsv16b_tdm_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tdm_xl_full_scale_t val);
int32_t lsm6dsv16b_tdm_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                         lsm6dsv16b_tdm_xl_full_scale_t *val);

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*LSM6DSV16B_DRIVER_H */
