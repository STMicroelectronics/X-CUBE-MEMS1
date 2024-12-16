/**
  ******************************************************************************
  * @file    st1vafe6ax_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          st1vafe6ax_reg.c driver.
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
#ifndef ST1VAFE6AX_REGS_H
#define ST1VAFE6AX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup ST1VAFE6AX
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

/** @defgroup ST1VAFE6AX_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define ST1VAFE6AX_I2C_ADD_L                     0xD5U
#define ST1VAFE6AX_I2C_ADD_H                     0xD7U

/** Device Identification (Who am I) **/
#define ST1VAFE6AX_ID                            0x71U

/**
  * @}
  *
  */

/** @defgroup bitfields page main
  * @{
  *
  */

#define ST1VAFE6AX_FUNC_CFG_ACCESS               0x1U
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
} st1vafe6ax_func_cfg_access_t;

#define ST1VAFE6AX_PIN_CTRL                      0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 5;
  uint8_t ibhr_por_en                   : 1;
  uint8_t sdo_pu_en                     : 1;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t sdo_pu_en                     : 1;
  uint8_t ibhr_por_en                   : 1;
  uint8_t not_used0                     : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_pin_ctrl_t;

#define ST1VAFE6AX_IF_CFG                        0x3U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t i2c_i3c_disable               : 1;
  uint8_t not_used0                     : 1;
  uint8_t sim                           : 1;
  uint8_t pp_od                         : 1;
  uint8_t h_lactive                     : 1;
  uint8_t asf_ctrl                      : 1;
  uint8_t not_used1                     : 1;
  uint8_t sda_pu_en                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t sda_pu_en                     : 1;
  uint8_t not_used1                     : 1;
  uint8_t asf_ctrl                      : 1;
  uint8_t h_lactive                     : 1;
  uint8_t pp_od                         : 1;
  uint8_t sim                           : 1;
  uint8_t not_used0                     : 1;
  uint8_t i2c_i3c_disable               : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_if_cfg_t;

#define ST1VAFE6AX_FIFO_CTRL1                    0x7U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm                           : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wtm                           : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_ctrl1_t;

#define ST1VAFE6AX_FIFO_CTRL2                    0x8U
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
} st1vafe6ax_fifo_ctrl2_t;

#define ST1VAFE6AX_FIFO_CTRL3                    0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bdr_xl                        : 4;
  uint8_t bdr_gy                        : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bdr_gy                        : 4;
  uint8_t bdr_xl                        : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_ctrl3_t;

#define ST1VAFE6AX_FIFO_CTRL4                    0x0AU
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
} st1vafe6ax_fifo_ctrl4_t;

#define ST1VAFE6AX_COUNTER_BDR_REG1              0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t cnt_bdr_th                    : 2;
  uint8_t ah_bio_batch_en               : 1;
  uint8_t not_used0                     : 2;
  uint8_t trig_counter_bdr              : 2;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t trig_counter_bdr              : 2;
  uint8_t not_used0                     : 2;
  uint8_t ah_bio_batch_en               : 1;
  uint8_t cnt_bdr_th                    : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_counter_bdr_reg1_t;

#define ST1VAFE6AX_COUNTER_BDR_REG2              0x0CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t cnt_bdr_th                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t cnt_bdr_th                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_counter_bdr_reg2_t;

#define ST1VAFE6AX_INT1_CTRL                     0x0DU
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
} st1vafe6ax_int1_ctrl_t;

#define ST1VAFE6AX_INT2_CTRL                     0x0EU
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
} st1vafe6ax_int2_ctrl_t;

#define ST1VAFE6AX_WHO_AM_I                      0x0FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t id                            : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t id                            : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_who_am_i_t;

#define ST1VAFE6AX_CTRL1                         0x10U
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
} st1vafe6ax_ctrl1_t;

#define ST1VAFE6AX_CTRL2                         0x11U
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
} st1vafe6ax_ctrl2_t;

#define ST1VAFE6AX_CTRL3                         0x12U
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
} st1vafe6ax_ctrl3_t;

#define ST1VAFE6AX_CTRL4                         0x13U
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
} st1vafe6ax_ctrl4_t;

#define ST1VAFE6AX_CTRL5                         0x14U
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
} st1vafe6ax_ctrl5_t;

#define ST1VAFE6AX_CTRL6                         0x15U
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
} st1vafe6ax_ctrl6_t;

#define ST1VAFE6AX_CTRL7                         0x16U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lpf1_g_en                     : 1;
  uint8_t not_used0                     : 1;
  uint8_t ah_bio2_en                    : 1;
  uint8_t ah_bio1_en                    : 1;
  uint8_t ah_bio_c_zin                  : 2;
  uint8_t int2_drdy_ah_bio              : 1;
  uint8_t ah_bio_en                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio_en                     : 1;
  uint8_t int2_drdy_ah_bio              : 1;
  uint8_t ah_bio_c_zin                  : 2;
  uint8_t ah_bio1_en                    : 1;
  uint8_t ah_bio2_en                    : 1;
  uint8_t not_used0                     : 1;
  uint8_t lpf1_g_en                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ctrl7_t;

#define ST1VAFE6AX_CTRL8                         0x17U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fs_xl                         : 2;
  uint8_t not_used0                     : 1;
  uint8_t xl_dualc_en                   : 1;
  uint8_t ah_bio_hpf                    : 1;
  uint8_t hp_lpf2_xl_bw                 : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t hp_lpf2_xl_bw                 : 3;
  uint8_t ah_bio_hpf                    : 1;
  uint8_t xl_dualc_en                   : 1;
  uint8_t not_used0                     : 1;
  uint8_t fs_xl                         : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ctrl8_t;

#define ST1VAFE6AX_CTRL9                         0x18U
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
  uint8_t ah_bio_lpf                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio_lpf                    : 1;
  uint8_t hp_ref_mode_xl                : 1;
  uint8_t xl_fastsettl_mode             : 1;
  uint8_t hp_slope_xl_en                : 1;
  uint8_t lpf2_xl_en                    : 1;
  uint8_t not_used0                     : 1;
  uint8_t usr_off_w                     : 1;
  uint8_t usr_off_on_out                : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ctrl9_t;

#define ST1VAFE6AX_CTRL10                        0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st_xl                         : 2;
  uint8_t st_g                          : 2;
  uint8_t xl_st_offset                  : 1;
  uint8_t ah_bio_sw                     : 1;
  uint8_t emb_func_debug                : 1;
  uint8_t not_used0                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t emb_func_debug                : 1;
  uint8_t ah_bio_sw                     : 1;
  uint8_t xl_st_offset                  : 1;
  uint8_t st_g                          : 2;
  uint8_t st_xl                         : 2;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ctrl10_t;

#define ST1VAFE6AX_CTRL_STATUS                   0x1AU
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
} st1vafe6ax_ctrl_status_t;

#define ST1VAFE6AX_FIFO_STATUS1                  0x1BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t diff_fifo                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t diff_fifo                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_status1_t;

#define ST1VAFE6AX_FIFO_STATUS2                  0x1CU
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
} st1vafe6ax_fifo_status2_t;

#define ST1VAFE6AX_ALL_INT_SRC                   0x1DU
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
} st1vafe6ax_all_int_src_t;

#define ST1VAFE6AX_STATUS_REG                    0x1EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlda                          : 1;
  uint8_t gda                           : 1;
  uint8_t tda                           : 1;
  uint8_t ah_bioda                      : 1;
  uint8_t not_used0                     : 3;
  uint8_t timestamp_endcount            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp_endcount            : 1;
  uint8_t not_used0                     : 3;
  uint8_t ah_bioda                      : 1;
  uint8_t tda                           : 1;
  uint8_t gda                           : 1;
  uint8_t xlda                          : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_status_reg_t;

#define ST1VAFE6AX_OUT_TEMP_L                    0x20U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                          : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_out_temp_l_t;

#define ST1VAFE6AX_OUT_TEMP_H                    0x21U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t temp                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp                          : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_out_temp_h_t;

#define ST1VAFE6AX_OUTX_L_G                      0x22U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outx_l_g_t;

#define ST1VAFE6AX_OUTX_H_G                      0x23U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outx_h_g_t;

#define ST1VAFE6AX_OUTY_L_G                      0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outy_l_g_t;

#define ST1VAFE6AX_OUTY_H_G                      0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outy_h_g_t;

#define ST1VAFE6AX_OUTZ_L_G                      0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outz_l_g_t;

#define ST1VAFE6AX_OUTZ_H_G                      0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_g                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_g                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outz_h_g_t;

#define ST1VAFE6AX_OUTZ_L_A                      0x28U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outz_l_a_t;

#define ST1VAFE6AX_OUTZ_H_A                      0x29U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outz_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outz_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outz_h_a_t;

#define ST1VAFE6AX_OUTY_L_A                      0x2AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outy_l_a_t;

#define ST1VAFE6AX_OUTY_H_A                      0x2BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outy_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outy_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outy_h_a_t;

#define ST1VAFE6AX_OUTX_L_A                      0x2CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outx_l_a_t;

#define ST1VAFE6AX_OUTX_H_A                      0x2DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t outx_a                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t outx_a                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_outx_h_a_t;

#define ST1VAFE6AX_UI_OUTZ_L_A_DUALC             0x34U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outz_l_a_dualc_t;

#define ST1VAFE6AX_UI_OUTZ_H_A_DUALC             0x35U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outz_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outz_h_a_dualc_t;

#define ST1VAFE6AX_UI_OUTY_L_A_DUALC             0x36U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outy_l_a_dualc_t;

#define ST1VAFE6AX_UI_OUTY_H_A_DUALC             0x37U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outy_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outy_h_a_dualc_t;

#define ST1VAFE6AX_UI_OUTX_L_A_DUALC             0x38U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outx_l_a_dualc_t;

#define ST1VAFE6AX_UI_OUTX_H_A_DUALC             0x39U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ui_outx_a_dualc               : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ui_outx_h_a_dualc_t;

#define ST1VAFE6AX_AH_BIO_OUT_L                  0x3AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ah_bio_out_l_t;

#define ST1VAFE6AX_AH_BIO_OUT_H                  0x3BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ah_bio                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ah_bio                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_ah_bio_out_h_t;

#define ST1VAFE6AX_TIMESTAMP0                    0x40U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_timestamp0_t;

#define ST1VAFE6AX_TIMESTAMP1                    0x41U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_timestamp1_t;

#define ST1VAFE6AX_TIMESTAMP2                    0x42U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_timestamp2_t;

#define ST1VAFE6AX_TIMESTAMP3                    0x43U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t timestamp                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t timestamp                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_timestamp3_t;

#define ST1VAFE6AX_WAKE_UP_SRC                   0x45U
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
} st1vafe6ax_wake_up_src_t;

#define ST1VAFE6AX_TAP_SRC                       0x46U
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
} st1vafe6ax_tap_src_t;

#define ST1VAFE6AX_D6D_SRC                       0x47U
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
} st1vafe6ax_d6d_src_t;

#define ST1VAFE6AX_EMB_FUNC_STATUS_MAINPAGE      0x49U
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
} st1vafe6ax_emb_func_status_mainpage_t;

#define ST1VAFE6AX_FSM_STATUS_MAINPAGE           0x4AU
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
} st1vafe6ax_fsm_status_mainpage_t;

#define ST1VAFE6AX_MLC_STATUS_MAINPAGE           0x4BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1                       : 1;
  uint8_t is_mlc2                       : 1;
  uint8_t is_mlc3                       : 1;
  uint8_t is_mlc4                       : 1;
  uint8_t not_used0                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t is_mlc4                       : 1;
  uint8_t is_mlc3                       : 1;
  uint8_t is_mlc2                       : 1;
  uint8_t is_mlc1                       : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_status_mainpage_t;

#define ST1VAFE6AX_INTERNAL_FREQ                 0x4FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t freq_fine                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t freq_fine                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_internal_freq_t;

#define ST1VAFE6AX_FUNCTIONS_ENABLE              0x50U
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
} st1vafe6ax_functions_enable_t;

#define ST1VAFE6AX_INACTIVITY_DUR                0x54U
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
} st1vafe6ax_inactivity_dur_t;

#define ST1VAFE6AX_INACTIVITY_THS                0x55U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t inact_ths                     : 6;
  uint8_t not_used0                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 2;
  uint8_t inact_ths                     : 6;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_inactivity_ths_t;

#define ST1VAFE6AX_TAP_CFG0                      0x56U
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
} st1vafe6ax_tap_cfg0_t;

#define ST1VAFE6AX_TAP_CFG1                      0x57U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_z                     : 5;
  uint8_t tap_priority                  : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t tap_priority                  : 3;
  uint8_t tap_ths_z                     : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_tap_cfg1_t;

#define ST1VAFE6AX_TAP_CFG2                      0x58U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tap_ths_y                     : 5;
  uint8_t not_used0                     : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 3;
  uint8_t tap_ths_y                     : 5;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_tap_cfg2_t;

#define ST1VAFE6AX_TAP_THS_6D                    0x59U
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
} st1vafe6ax_tap_ths_6d_t;

#define ST1VAFE6AX_TAP_DUR                       0x5AU
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
} st1vafe6ax_tap_dur_t;

#define ST1VAFE6AX_WAKE_UP_THS                   0x5BU
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
} st1vafe6ax_wake_up_ths_t;

#define ST1VAFE6AX_WAKE_UP_DUR                   0x5CU
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
} st1vafe6ax_wake_up_dur_t;

#define ST1VAFE6AX_FREE_FALL                     0x5DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ff_ths                        : 3;
  uint8_t ff_dur                        : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t ff_dur                        : 5;
  uint8_t ff_ths                        : 3;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_free_fall_t;

#define ST1VAFE6AX_MD1_CFG                       0x5EU
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
} st1vafe6ax_md1_cfg_t;

#define ST1VAFE6AX_MD2_CFG                       0x5FU
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
} st1vafe6ax_md2_cfg_t;

#define ST1VAFE6AX_EMB_FUNC_CFG                  0x63U
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
} st1vafe6ax_emb_func_cfg_t;

#define ST1VAFE6AX_Z_OFS_USR                     0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t z_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t z_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_z_ofs_usr_t;

#define ST1VAFE6AX_Y_OFS_USR                     0x74U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t y_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t y_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_y_ofs_usr_t;

#define ST1VAFE6AX_X_OFS_USR                     0x75U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t x_ofs_usr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t x_ofs_usr                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_x_ofs_usr_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_TAG             0x78U
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
} st1vafe6ax_fifo_data_out_tag_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_0          0x79U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_0_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_1          0x7AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_1_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_2          0x7BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_2_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_3          0x7CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_3_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_4          0x7DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_4_t;

#define ST1VAFE6AX_FIFO_DATA_OUT_BYTE_5          0x7EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fifo_data_out                 : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_data_out                 : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fifo_data_out_byte_5_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page embedded
  * @{
  *
  */

#define ST1VAFE6AX_PAGE_SEL                      0x2U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t page_sel                      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_sel                      : 4;
  uint8_t not_used0                     : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_page_sel_t;

#define ST1VAFE6AX_EMB_FUNC_EN_A                 0x4U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used2                     : 1;
  uint8_t sflp_game_en                  : 1;
  uint8_t not_used0                     : 1;
  uint8_t pedo_en                       : 1;
  uint8_t tilt_en                       : 1;
  uint8_t sign_motion_en                : 1;
  uint8_t not_used1                     : 1;
  uint8_t mlc_before_fsm_en             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_before_fsm_en             : 1;
  uint8_t not_used1                     : 1;
  uint8_t sign_motion_en                : 1;
  uint8_t tilt_en                       : 1;
  uint8_t pedo_en                       : 1;
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_en                  : 1;
  uint8_t not_used2                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_en_a_t;

#define ST1VAFE6AX_EMB_FUNC_EN_B                 0x5U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_en                        : 1;
  uint8_t not_used0                     : 2;
  uint8_t fifo_compr_en                 : 1;
  uint8_t mlc_en                        : 1;
  uint8_t not_used1                     : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 3;
  uint8_t mlc_en                        : 1;
  uint8_t fifo_compr_en                 : 1;
  uint8_t not_used0                     : 2;
  uint8_t fsm_en                        : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_en_b_t;

#define ST1VAFE6AX_EMB_FUNC_EXEC_STATUS          0x7U
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
} st1vafe6ax_emb_func_exec_status_t;

#define ST1VAFE6AX_PAGE_ADDRESS                  0x8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_addr                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_addr                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_page_address_t;

#define ST1VAFE6AX_PAGE_VALUE                    0x9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t page_value                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t page_value                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_page_value_t;

#define ST1VAFE6AX_EMB_FUNC_INT1                 0x0AU
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
} st1vafe6ax_emb_func_int1_t;

#define ST1VAFE6AX_FSM_INT1                      0x0BU
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
} st1vafe6ax_fsm_int1_t;

#define ST1VAFE6AX_MLC_INT1                      0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int1_mlc1                     : 1;
  uint8_t int1_mlc2                     : 1;
  uint8_t int1_mlc3                     : 1;
  uint8_t int1_mlc4                     : 1;
  uint8_t not_used0                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t int1_mlc4                     : 1;
  uint8_t int1_mlc3                     : 1;
  uint8_t int1_mlc2                     : 1;
  uint8_t int1_mlc1                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_int1_t;

#define ST1VAFE6AX_EMB_FUNC_INT2                 0x0EU
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
} st1vafe6ax_emb_func_int2_t;

#define ST1VAFE6AX_FSM_INT2                      0x0FU
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
} st1vafe6ax_fsm_int2_t;

#define ST1VAFE6AX_MLC_INT2                      0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int2_mlc1                     : 1;
  uint8_t int2_mlc2                     : 1;
  uint8_t int2_mlc3                     : 1;
  uint8_t int2_mlc4                     : 1;
  uint8_t not_used0                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t int2_mlc4                     : 1;
  uint8_t int2_mlc3                     : 1;
  uint8_t int2_mlc2                     : 1;
  uint8_t int2_mlc1                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_int2_t;

#define ST1VAFE6AX_EMB_FUNC_STATUS               0x12U
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
} st1vafe6ax_emb_func_status_t;

#define ST1VAFE6AX_FSM_STATUS                    0x13U
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
} st1vafe6ax_fsm_status_t;

#define ST1VAFE6AX_MLC_STATUS                    0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t is_mlc1                       : 1;
  uint8_t is_mlc2                       : 1;
  uint8_t is_mlc3                       : 1;
  uint8_t is_mlc4                       : 1;
  uint8_t not_used0                     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t is_mlc4                       : 1;
  uint8_t is_mlc3                       : 1;
  uint8_t is_mlc2                       : 1;
  uint8_t is_mlc1                       : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_status_t;

#define ST1VAFE6AX_PAGE_RW                       0x17U
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
} st1vafe6ax_page_rw_t;

#define ST1VAFE6AX_EMB_FUNC_FIFO_EN_A            0x44U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_fifo_en             : 1;
  uint8_t not_used1                     : 2;
  uint8_t sflp_gravity_fifo_en          : 1;
  uint8_t sflp_gbias_fifo_en            : 1;
  uint8_t step_counter_fifo_en          : 1;
  uint8_t mlc_fifo_en                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_fifo_en                   : 1;
  uint8_t step_counter_fifo_en          : 1;
  uint8_t sflp_gbias_fifo_en            : 1;
  uint8_t sflp_gravity_fifo_en          : 1;
  uint8_t not_used1                     : 2;
  uint8_t sflp_game_fifo_en             : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_fifo_en_a_t;

#define ST1VAFE6AX_EMB_FUNC_FIFO_EN_B            0x45U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t mlc_filter_feature_fifo_en    : 1;
  uint8_t not_used1                     : 6;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 6;
  uint8_t mlc_filter_feature_fifo_en    : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_fifo_en_b_t;

#define ST1VAFE6AX_FSM_ENABLE                    0x46U
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
} st1vafe6ax_fsm_enable_t;

#define ST1VAFE6AX_FSM_LONG_COUNTER_L            0x48U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_long_counter_l_t;

#define ST1VAFE6AX_FSM_LONG_COUNTER_H            0x49U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_long_counter_h_t;

#define ST1VAFE6AX_INT_ACK_MASK                  0x4BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t iack_mask                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t iack_mask                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_int_ack_mask_t;

#define ST1VAFE6AX_FSM_OUTS1                     0x4CU
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
} st1vafe6ax_fsm_outs1_t;

#define ST1VAFE6AX_FSM_OUTS2                     0x4DU
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
} st1vafe6ax_fsm_outs2_t;

#define ST1VAFE6AX_FSM_OUTS3                     0x4EU
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
} st1vafe6ax_fsm_outs3_t;

#define ST1VAFE6AX_FSM_OUTS4                     0x4FU
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
} st1vafe6ax_fsm_outs4_t;

#define ST1VAFE6AX_FSM_OUTS5                     0x50U
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
} st1vafe6ax_fsm_outs5_t;

#define ST1VAFE6AX_FSM_OUTS6                     0x51U
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
} st1vafe6ax_fsm_outs6_t;

#define ST1VAFE6AX_FSM_OUTS7                     0x52U
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
} st1vafe6ax_fsm_outs7_t;

#define ST1VAFE6AX_FSM_OUTS8                     0x53U
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
} st1vafe6ax_fsm_outs8_t;

#define ST1VAFE6AX_SFLP_ODR                      0x5EU
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
} st1vafe6ax_sflp_odr_t;

#define ST1VAFE6AX_FSM_ODR                       0x5FU
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
} st1vafe6ax_fsm_odr_t;

#define ST1VAFE6AX_MLC_ODR                       0x60U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 4;
  uint8_t mlc_odr                       : 3;
  uint8_t not_used1                     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 1;
  uint8_t mlc_odr                       : 3;
  uint8_t not_used0                     : 4;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_odr_t;

#define ST1VAFE6AX_STEP_COUNTER_L                0x62U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                          : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_step_counter_l_t;

#define ST1VAFE6AX_STEP_COUNTER_H                0x63U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t step                          : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t step                          : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_step_counter_h_t;

#define ST1VAFE6AX_EMB_FUNC_SRC                  0x64U
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
} st1vafe6ax_emb_func_src_t;

#define ST1VAFE6AX_EMB_FUNC_INIT_A               0x66U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used0                     : 1;
  uint8_t sflp_game_init                : 1;
  uint8_t not_used2                     : 1;
  uint8_t step_det_init                 : 1;
  uint8_t tilt_init                     : 1;
  uint8_t sig_mot_init                  : 1;
  uint8_t not_used1                     : 1;
  uint8_t mlc_before_fsm_init           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_before_fsm_init           : 1;
  uint8_t not_used1                     : 1;
  uint8_t sig_mot_init                  : 1;
  uint8_t tilt_init                     : 1;
  uint8_t step_det_init                 : 1;
  uint8_t not_used2                     : 1;
  uint8_t sflp_game_init                : 1;
  uint8_t not_used0                     : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_init_a_t;

#define ST1VAFE6AX_EMB_FUNC_INIT_B               0x67U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_init                      : 1;
  uint8_t not_used0                     : 2;
  uint8_t fifo_compr_init               : 1;
  uint8_t mlc_init                      : 1;
  uint8_t not_used1                     : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used1                     : 3;
  uint8_t mlc_init                      : 1;
  uint8_t fifo_compr_init               : 1;
  uint8_t not_used0                     : 2;
  uint8_t fsm_init                      : 1;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_emb_func_init_b_t;

#define ST1VAFE6AX_MLC1_SRC                      0x70U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc1_src                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc1_src                      : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc1_src_t;

#define ST1VAFE6AX_MLC2_SRC                      0x71U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc2_src                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc2_src                      : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc2_src_t;

#define ST1VAFE6AX_MLC3_SRC                      0x72U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc3_src                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc3_src                      : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc3_src_t;

#define ST1VAFE6AX_MLC4_SRC                      0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc4_src                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc4_src                      : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc4_src_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page pg0_emb_adv
  * @{
  *
  */
#define ST1VAFE6AX_EMB_ADV_PG_0                  0x000

#define ST1VAFE6AX_SFLP_GAME_GBIASX_L            0x6EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasx                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasx                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasx_l_t;

#define ST1VAFE6AX_SFLP_GAME_GBIASX_H            0x6FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasx                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasx                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasx_h_t;

#define ST1VAFE6AX_SFLP_GAME_GBIASY_L            0x70U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasy                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasy                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasy_l_t;

#define ST1VAFE6AX_SFLP_GAME_GBIASY_H            0x71U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasy                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasy                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasy_h_t;

#define ST1VAFE6AX_SFLP_GAME_GBIASZ_L            0x72U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasz                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasz                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasz_l_t;

#define ST1VAFE6AX_SFLP_GAME_GBIASZ_H            0x73U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t gbiasz                        : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t gbiasz                        : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_sflp_game_gbiasz_h_t;

#define ST1VAFE6AX_FSM_BIO_SENSITIVITY_L         0xBAU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_bio_sensitivity_l_t;

#define ST1VAFE6AX_FSM_BIO_SENSITIVITY_H         0xBBU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_bio_sensitivity_h_t;

/**
  * @}
  *
  */

/** @defgroup bitfields page pg1_emb_adv
  * @{
  *
  */

#define ST1VAFE6AX_EMB_ADV_PG_1                  0x001

#define ST1VAFE6AX_FSM_LC_TIMEOUT_L              0x17AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_lc_timeout_l_t;

#define ST1VAFE6AX_FSM_LC_TIMEOUT_H              0x17BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_lc_timeout                : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_lc_timeout_h_t;

#define ST1VAFE6AX_FSM_PROGRAMS                  0x17CU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_n_prog                    : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_n_prog                    : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_programs_t;

#define ST1VAFE6AX_FSM_START_ADD_L               0x17EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_start_add_l_t;

#define ST1VAFE6AX_FSM_START_ADD_H               0x17FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fsm_start                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fsm_start                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_fsm_start_add_h_t;

#define ST1VAFE6AX_PEDO_CMD_REG                  0x183U
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
} st1vafe6ax_pedo_cmd_reg_t;

#define ST1VAFE6AX_PEDO_DEB_STEPS_CONF           0x184U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t deb_step                      : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t deb_step                      : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_pedo_deb_steps_conf_t;

#define ST1VAFE6AX_PEDO_SC_DELTAT_L              0x1D0U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_pedo_sc_deltat_l_t;

#define ST1VAFE6AX_PEDO_SC_DELTAT_H              0x1D1U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pd_sc                         : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pd_sc                         : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_pedo_sc_deltat_h_t;

#define ST1VAFE6AX_MLC_BIO_SENSITIVITY_L         0x1E8U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_bio_sensitivity_l_t;

#define ST1VAFE6AX_MLC_BIO_SENSITIVITY_H         0x1E9U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t mlc_bio_s                     : 8;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t mlc_bio_s                     : 8;
#endif /* DRV_BYTE_ORDER */
} st1vafe6ax_mlc_bio_sensitivity_h_t;

/**
  * @}
  *
  */

#define ST1VAFE6AX_START_FSM_ADD                0x035CU

/**
  * @defgroup ST1VAFE6AX_Register_Union
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
  st1vafe6ax_func_cfg_access_t    func_cfg_access;
  st1vafe6ax_pin_ctrl_t    pin_ctrl;
  st1vafe6ax_if_cfg_t    if_cfg;
  st1vafe6ax_fifo_ctrl1_t    fifo_ctrl1;
  st1vafe6ax_fifo_ctrl2_t    fifo_ctrl2;
  st1vafe6ax_fifo_ctrl3_t    fifo_ctrl3;
  st1vafe6ax_fifo_ctrl4_t    fifo_ctrl4;
  st1vafe6ax_counter_bdr_reg1_t    counter_bdr_reg1;
  st1vafe6ax_counter_bdr_reg2_t    counter_bdr_reg2;
  st1vafe6ax_int1_ctrl_t    int1_ctrl;
  st1vafe6ax_int2_ctrl_t    int2_ctrl;
  st1vafe6ax_who_am_i_t    who_am_i;
  st1vafe6ax_ctrl1_t    ctrl1;
  st1vafe6ax_ctrl2_t    ctrl2;
  st1vafe6ax_ctrl3_t    ctrl3;
  st1vafe6ax_ctrl4_t    ctrl4;
  st1vafe6ax_ctrl5_t    ctrl5;
  st1vafe6ax_ctrl6_t    ctrl6;
  st1vafe6ax_ctrl7_t    ctrl7;
  st1vafe6ax_ctrl8_t    ctrl8;
  st1vafe6ax_ctrl9_t    ctrl9;
  st1vafe6ax_ctrl10_t    ctrl10;
  st1vafe6ax_fifo_status1_t    fifo_status1;
  st1vafe6ax_fifo_status2_t    fifo_status2;
  st1vafe6ax_all_int_src_t    all_int_src;
  st1vafe6ax_status_reg_t    status_reg;
  st1vafe6ax_out_temp_l_t    out_temp_l;
  st1vafe6ax_out_temp_h_t    out_temp_h;
  st1vafe6ax_outx_l_g_t    outx_l_g;
  st1vafe6ax_outx_h_g_t    outx_h_g;
  st1vafe6ax_outy_l_g_t    outy_l_g;
  st1vafe6ax_outy_h_g_t    outy_h_g;
  st1vafe6ax_outz_l_g_t    outz_l_g;
  st1vafe6ax_outz_h_g_t    outz_h_g;
  st1vafe6ax_outz_l_a_t    outz_l_a;
  st1vafe6ax_outz_h_a_t    outz_h_a;
  st1vafe6ax_outy_l_a_t    outy_l_a;
  st1vafe6ax_outy_h_a_t    outy_h_a;
  st1vafe6ax_outx_l_a_t    outx_l_a;
  st1vafe6ax_outx_h_a_t    outx_h_a;
  st1vafe6ax_ui_outz_l_a_dualc_t    ui_outz_l_a_dualc;
  st1vafe6ax_ui_outz_h_a_dualc_t    ui_outz_h_a_dualc;
  st1vafe6ax_ui_outy_l_a_dualc_t    ui_outy_l_a_dualc;
  st1vafe6ax_ui_outy_h_a_dualc_t    ui_outy_h_a_dualc;
  st1vafe6ax_ui_outx_l_a_dualc_t    ui_outx_l_a_dualc;
  st1vafe6ax_ui_outx_h_a_dualc_t    ui_outx_h_a_dualc;
  st1vafe6ax_ah_bio_out_l_t     ah_bio_out_l;
  st1vafe6ax_ah_bio_out_h_t     ah_bio_out_h;
  st1vafe6ax_timestamp0_t    timestamp0;
  st1vafe6ax_timestamp1_t    timestamp1;
  st1vafe6ax_timestamp2_t    timestamp2;
  st1vafe6ax_timestamp3_t    timestamp3;
  st1vafe6ax_wake_up_src_t    wake_up_src;
  st1vafe6ax_tap_src_t    tap_src;
  st1vafe6ax_d6d_src_t    d6d_src;
  st1vafe6ax_emb_func_status_mainpage_t    emb_func_status_mainpage;
  st1vafe6ax_fsm_status_mainpage_t    fsm_status_mainpage;
  st1vafe6ax_mlc_status_mainpage_t    mlc_status_mainpage;
  st1vafe6ax_internal_freq_t    internal_freq;
  st1vafe6ax_functions_enable_t    functions_enable;
  st1vafe6ax_inactivity_dur_t    inactivity_dur;
  st1vafe6ax_inactivity_ths_t    inactivity_ths;
  st1vafe6ax_tap_cfg0_t    tap_cfg0;
  st1vafe6ax_tap_cfg1_t    tap_cfg1;
  st1vafe6ax_tap_cfg2_t    tap_cfg2;
  st1vafe6ax_tap_ths_6d_t    tap_ths_6d;
  st1vafe6ax_tap_dur_t    int_dur2;
  st1vafe6ax_wake_up_ths_t    wake_up_ths;
  st1vafe6ax_wake_up_dur_t    wake_up_dur;
  st1vafe6ax_free_fall_t    free_fall;
  st1vafe6ax_md1_cfg_t    md1_cfg;
  st1vafe6ax_md2_cfg_t    md2_cfg;
  st1vafe6ax_emb_func_cfg_t    emb_func_cfg;
  st1vafe6ax_z_ofs_usr_t    z_ofs_usr;
  st1vafe6ax_y_ofs_usr_t    y_ofs_usr;
  st1vafe6ax_x_ofs_usr_t    x_ofs_usr;
  st1vafe6ax_fifo_data_out_tag_t    fifo_data_out_tag;
  st1vafe6ax_fifo_data_out_byte_0_t    fifo_data_out_byte_0;
  st1vafe6ax_fifo_data_out_byte_1_t    fifo_data_out_byte_1;
  st1vafe6ax_fifo_data_out_byte_2_t    fifo_data_out_byte_2;
  st1vafe6ax_fifo_data_out_byte_3_t    fifo_data_out_byte_3;
  st1vafe6ax_fifo_data_out_byte_4_t    fifo_data_out_byte_4;
  st1vafe6ax_fifo_data_out_byte_5_t    fifo_data_out_byte_5;
  st1vafe6ax_page_sel_t    page_sel;
  st1vafe6ax_emb_func_en_a_t    emb_func_en_a;
  st1vafe6ax_emb_func_en_b_t    emb_func_en_b;
  st1vafe6ax_emb_func_exec_status_t    emb_func_exec_status;
  st1vafe6ax_page_address_t    page_address;
  st1vafe6ax_page_value_t    page_value;
  st1vafe6ax_emb_func_int1_t    emb_func_int1;
  st1vafe6ax_fsm_int1_t    fsm_int1;
  st1vafe6ax_mlc_int1_t    mlc_int1;
  st1vafe6ax_emb_func_int2_t    emb_func_int2;
  st1vafe6ax_fsm_int2_t    fsm_int2;
  st1vafe6ax_mlc_int2_t    mlc_int2;
  st1vafe6ax_emb_func_status_t    emb_func_status;
  st1vafe6ax_fsm_status_t    fsm_status;
  st1vafe6ax_mlc_status_t    mlc_status;
  st1vafe6ax_page_rw_t    page_rw;
  st1vafe6ax_emb_func_fifo_en_a_t    emb_func_fifo_en_a;
  st1vafe6ax_emb_func_fifo_en_b_t    emb_func_fifo_en_b;
  st1vafe6ax_fsm_enable_t    fsm_enable;
  st1vafe6ax_fsm_long_counter_l_t    fsm_long_counter_l;
  st1vafe6ax_fsm_long_counter_h_t    fsm_long_counter_h;
  st1vafe6ax_fsm_outs1_t    fsm_outs1;
  st1vafe6ax_fsm_outs2_t    fsm_outs2;
  st1vafe6ax_fsm_outs3_t    fsm_outs3;
  st1vafe6ax_fsm_outs4_t    fsm_outs4;
  st1vafe6ax_fsm_outs5_t    fsm_outs5;
  st1vafe6ax_fsm_outs6_t    fsm_outs6;
  st1vafe6ax_fsm_outs7_t    fsm_outs7;
  st1vafe6ax_fsm_outs8_t    fsm_outs8;
  st1vafe6ax_fsm_odr_t    fsm_odr;
  st1vafe6ax_mlc_odr_t    mlc_odr;
  st1vafe6ax_step_counter_l_t    step_counter_l;
  st1vafe6ax_step_counter_h_t    step_counter_h;
  st1vafe6ax_emb_func_src_t    emb_func_src;
  st1vafe6ax_emb_func_init_a_t    emb_func_init_a;
  st1vafe6ax_emb_func_init_b_t    emb_func_init_b;
  st1vafe6ax_mlc1_src_t    mlc1_src;
  st1vafe6ax_mlc2_src_t    mlc2_src;
  st1vafe6ax_mlc3_src_t    mlc3_src;
  st1vafe6ax_mlc4_src_t    mlc4_src;
  st1vafe6ax_fsm_bio_sensitivity_l_t    fsm_bio_sensitivity_l;
  st1vafe6ax_fsm_bio_sensitivity_h_t    fsm_bio_sensitivity_h;
  st1vafe6ax_fsm_lc_timeout_l_t    fsm_lc_timeout_l;
  st1vafe6ax_fsm_lc_timeout_h_t    fsm_lc_timeout_h;
  st1vafe6ax_fsm_programs_t    fsm_programs;
  st1vafe6ax_fsm_start_add_l_t    fsm_start_add_l;
  st1vafe6ax_fsm_start_add_h_t    fsm_start_add_h;
  st1vafe6ax_pedo_cmd_reg_t    pedo_cmd_reg;
  st1vafe6ax_pedo_deb_steps_conf_t    pedo_deb_steps_conf;
  st1vafe6ax_pedo_sc_deltat_l_t    pedo_sc_deltat_l;
  st1vafe6ax_pedo_sc_deltat_h_t    pedo_sc_deltat_h;
  st1vafe6ax_mlc_bio_sensitivity_l_t    mlc_bio_sensitivity_l;
  st1vafe6ax_mlc_bio_sensitivity_h_t    mlc_bio_sensitivity_h;
  bitwise_t    bitwise;
  uint8_t    byte;
} st1vafe6ax_reg_t;


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

int32_t st1vafe6ax_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                            uint8_t *data,
                            uint16_t len);
int32_t st1vafe6ax_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                             uint8_t *data,
                             uint16_t len);

float_t st1vafe6ax_from_sflp_to_mg(int16_t lsb);
float_t st1vafe6ax_from_fs2_to_mg(int16_t lsb);
float_t st1vafe6ax_from_fs4_to_mg(int16_t lsb);
float_t st1vafe6ax_from_fs8_to_mg(int16_t lsb);
float_t st1vafe6ax_from_fs16_to_mg(int16_t lsb);

float_t st1vafe6ax_from_fs125_to_mdps(int16_t lsb);
float_t st1vafe6ax_from_fs500_to_mdps(int16_t lsb);
float_t st1vafe6ax_from_fs250_to_mdps(int16_t lsb);
float_t st1vafe6ax_from_fs1000_to_mdps(int16_t lsb);
float_t st1vafe6ax_from_fs2000_to_mdps(int16_t lsb);
float_t st1vafe6ax_from_fs4000_to_mdps(int16_t lsb);

float_t st1vafe6ax_from_lsb_to_celsius(int16_t lsb);

uint64_t st1vafe6ax_from_lsb_to_nsec(uint32_t lsb);

float_t st1vafe6ax_from_lsb_to_mv(int16_t lsb);

typedef enum
{
  ST1VAFE6AX_READY                               = 0x0,
  ST1VAFE6AX_GLOBAL_RST                          = 0x1,
  ST1VAFE6AX_RESTORE_CAL_PARAM                   = 0x2,
  ST1VAFE6AX_RESTORE_CTRL_REGS                   = 0x4,
} st1vafe6ax_reset_t;
int32_t st1vafe6ax_reset_set(const stmdev_ctx_t *ctx, st1vafe6ax_reset_t val);
int32_t st1vafe6ax_reset_get(const stmdev_ctx_t *ctx, st1vafe6ax_reset_t *val);

typedef enum
{
  ST1VAFE6AX_MAIN_MEM_BANK                       = 0x0,
  ST1VAFE6AX_EMBED_FUNC_MEM_BANK                 = 0x1,
} st1vafe6ax_mem_bank_t;
int32_t st1vafe6ax_mem_bank_set(const stmdev_ctx_t *ctx, st1vafe6ax_mem_bank_t val);
int32_t st1vafe6ax_mem_bank_get(const stmdev_ctx_t *ctx,
                                st1vafe6ax_mem_bank_t *val);

int32_t st1vafe6ax_device_id_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_XL_ODR_OFF                          = 0x0,
  ST1VAFE6AX_XL_ODR_AT_1Hz875                    = 0x1,
  ST1VAFE6AX_XL_ODR_AT_7Hz5                      = 0x2,
  ST1VAFE6AX_XL_ODR_AT_15Hz                      = 0x3,
  ST1VAFE6AX_XL_ODR_AT_30Hz                      = 0x4,
  ST1VAFE6AX_XL_ODR_AT_60Hz                      = 0x5,
  ST1VAFE6AX_XL_ODR_AT_120Hz                     = 0x6,
  ST1VAFE6AX_XL_ODR_AT_240Hz                     = 0x7,
  ST1VAFE6AX_XL_ODR_AT_480Hz                     = 0x8,
  ST1VAFE6AX_XL_ODR_AT_960Hz                     = 0x9,
  ST1VAFE6AX_XL_ODR_AT_1920Hz                    = 0xA,
  ST1VAFE6AX_XL_ODR_AT_3840Hz                    = 0xB,
} st1vafe6ax_xl_data_rate_t;
int32_t st1vafe6ax_xl_data_rate_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_data_rate_t val);
int32_t st1vafe6ax_xl_data_rate_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_data_rate_t *val);

typedef enum
{
  ST1VAFE6AX_XL_HIGH_PERFORMANCE_MD              = 0x0,
  ST1VAFE6AX_XL_HIGH_PERFORMANCE_2_MD            = 0x2,
  ST1VAFE6AX_XL_LOW_POWER_2_AVG_MD               = 0x4,
  ST1VAFE6AX_XL_LOW_POWER_4_AVG_MD               = 0x5,
  ST1VAFE6AX_XL_LOW_POWER_8_AVG_MD               = 0x6,
} st1vafe6ax_xl_mode_t;
int32_t st1vafe6ax_xl_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_xl_mode_t val);
int32_t st1vafe6ax_xl_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_xl_mode_t *val);

typedef enum
{
  ST1VAFE6AX_GY_ODR_OFF                          = 0x0,
  ST1VAFE6AX_GY_ODR_AT_7Hz5                      = 0x2,
  ST1VAFE6AX_GY_ODR_AT_15Hz                      = 0x3,
  ST1VAFE6AX_GY_ODR_AT_30Hz                      = 0x4,
  ST1VAFE6AX_GY_ODR_AT_60Hz                      = 0x5,
  ST1VAFE6AX_GY_ODR_AT_120Hz                     = 0x6,
  ST1VAFE6AX_GY_ODR_AT_240Hz                     = 0x7,
  ST1VAFE6AX_GY_ODR_AT_480Hz                     = 0x8,
  ST1VAFE6AX_GY_ODR_AT_960Hz                     = 0x9,
  ST1VAFE6AX_GY_ODR_AT_1920Hz                    = 0xa,
  ST1VAFE6AX_GY_ODR_AT_3840Hz                    = 0xb,
} st1vafe6ax_gy_data_rate_t;
int32_t st1vafe6ax_gy_data_rate_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_data_rate_t val);
int32_t st1vafe6ax_gy_data_rate_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_data_rate_t *val);

typedef enum
{
  ST1VAFE6AX_GY_HIGH_PERFORMANCE_MD              = 0x0,
  ST1VAFE6AX_GY_SLEEP_MD                         = 0x4,
  ST1VAFE6AX_GY_LOW_POWER_MD                     = 0x5,
} st1vafe6ax_gy_mode_t;
int32_t st1vafe6ax_gy_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_gy_mode_t val);
int32_t st1vafe6ax_gy_mode_get(const stmdev_ctx_t *ctx, st1vafe6ax_gy_mode_t *val);

int32_t st1vafe6ax_auto_increment_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_auto_increment_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_DRDY_LATCHED                        = 0x0,
  ST1VAFE6AX_DRDY_PULSED                         = 0x1,
} st1vafe6ax_data_ready_mode_t;
int32_t st1vafe6ax_data_ready_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_mode_t val);
int32_t st1vafe6ax_data_ready_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_mode_t *val);

typedef enum
{
  ST1VAFE6AX_125dps                              = 0x0,
  ST1VAFE6AX_250dps                              = 0x1,
  ST1VAFE6AX_500dps                              = 0x2,
  ST1VAFE6AX_1000dps                             = 0x3,
  ST1VAFE6AX_2000dps                             = 0x4,
  ST1VAFE6AX_4000dps                             = 0xc,
} st1vafe6ax_gy_full_scale_t;
int32_t st1vafe6ax_gy_full_scale_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_gy_full_scale_t val);
int32_t st1vafe6ax_gy_full_scale_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_gy_full_scale_t *val);

typedef enum
{
  ST1VAFE6AX_2g                                  = 0x0,
  ST1VAFE6AX_4g                                  = 0x1,
  ST1VAFE6AX_8g                                  = 0x2,
} st1vafe6ax_xl_full_scale_t;
int32_t st1vafe6ax_xl_full_scale_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_xl_full_scale_t val);
int32_t st1vafe6ax_xl_full_scale_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_xl_full_scale_t *val);

int32_t st1vafe6ax_xl_dual_channel_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_xl_dual_channel_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_XL_ST_DISABLE                       = 0x0,
  ST1VAFE6AX_XL_ST_POSITIVE                      = 0x1,
  ST1VAFE6AX_XL_ST_NEGATIVE                      = 0x2,
  ST1VAFE6AX_XL_ST_OFFSET_POS                    = 0x5,
  ST1VAFE6AX_XL_ST_OFFSET_NEG                    = 0x6,
} st1vafe6ax_xl_self_test_t;
int32_t st1vafe6ax_xl_self_test_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_self_test_t val);
int32_t st1vafe6ax_xl_self_test_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_self_test_t *val);

typedef enum
{
  ST1VAFE6AX_GY_ST_DISABLE                       = 0x0,
  ST1VAFE6AX_GY_ST_POSITIVE                      = 0x1,
  ST1VAFE6AX_GY_ST_NEGATIVE                      = 0x2,
} st1vafe6ax_gy_self_test_t;
int32_t st1vafe6ax_gy_self_test_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_self_test_t val);
int32_t st1vafe6ax_gy_self_test_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_gy_self_test_t *val);

typedef struct
{
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
  uint8_t drdy_ah_bio                   : 1;
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
  uint8_t mlc1                          : 1;
  uint8_t mlc2                          : 1;
  uint8_t mlc3                          : 1;
  uint8_t mlc4                          : 1;
  uint8_t fifo_bdr                      : 1;
  uint8_t fifo_full                     : 1;
  uint8_t fifo_ovr                      : 1;
  uint8_t fifo_th                       : 1;
} st1vafe6ax_all_sources_t;
int32_t st1vafe6ax_all_sources_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_all_sources_t *val);

typedef struct
{
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
  uint8_t drdy_ah_bio                   : 1;
} st1vafe6ax_data_ready_t;
int32_t st1vafe6ax_flag_data_ready_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_data_ready_t *val);

int32_t st1vafe6ax_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t st1vafe6ax_angular_rate_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t st1vafe6ax_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t st1vafe6ax_dual_acceleration_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t st1vafe6ax_dual_acceleration_raw_get(const stmdev_ctx_t *ctx,
                                             int16_t *val);

int32_t st1vafe6ax_ah_bio_raw_get(const stmdev_ctx_t *ctx, int16_t *val);

int32_t st1vafe6ax_odr_cal_reg_get(const stmdev_ctx_t *ctx, int8_t *val);

int32_t st1vafe6ax_ln_pg_write(const stmdev_ctx_t *ctx, uint16_t address,
                               uint8_t *buf, uint8_t len);
int32_t st1vafe6ax_ln_pg_read(const stmdev_ctx_t *ctx, uint16_t address,
                              uint8_t *buf, uint8_t len);

int32_t st1vafe6ax_timestamp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_timestamp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_timestamp_raw_get(const stmdev_ctx_t *ctx, uint32_t *val);

typedef enum
{
  ST1VAFE6AX_AUTO                                = 0x0,
  ST1VAFE6AX_ALWAYS_ACTIVE                       = 0x1,
} st1vafe6ax_filt_anti_spike_t;
int32_t st1vafe6ax_filt_anti_spike_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_anti_spike_t val);
int32_t st1vafe6ax_filt_anti_spike_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_anti_spike_t *val);

typedef struct
{
  uint8_t drdy                          : 1;
  uint8_t irq_xl                        : 1;
  uint8_t irq_g                         : 1;
} st1vafe6ax_filt_settling_mask_t;
int32_t st1vafe6ax_filt_settling_mask_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_settling_mask_t val);
int32_t st1vafe6ax_filt_settling_mask_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_settling_mask_t *val);

typedef enum
{
  ST1VAFE6AX_GY_ULTRA_LIGHT                      = 0x0,
  ST1VAFE6AX_GY_VERY_LIGHT                       = 0x1,
  ST1VAFE6AX_GY_LIGHT                            = 0x2,
  ST1VAFE6AX_GY_MEDIUM                           = 0x3,
  ST1VAFE6AX_GY_STRONG                           = 0x4,
  ST1VAFE6AX_GY_VERY_STRONG                      = 0x5,
  ST1VAFE6AX_GY_AGGRESSIVE                       = 0x6,
  ST1VAFE6AX_GY_XTREME                           = 0x7,
} st1vafe6ax_filt_gy_lp1_bandwidth_t;
int32_t st1vafe6ax_filt_gy_lp1_bandwidth_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_gy_lp1_bandwidth_t val);
int32_t st1vafe6ax_filt_gy_lp1_bandwidth_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_gy_lp1_bandwidth_t *val);

int32_t st1vafe6ax_filt_gy_lp1_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_filt_gy_lp1_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  uint8_t hpf                           : 1;
  uint8_t lpf                           : 1;
} st1vafe6ax_filt_ah_bio_conf_t;
int32_t st1vafe6ax_filt_ah_bio_conf_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_filt_ah_bio_conf_t val);
int32_t st1vafe6ax_filt_ah_bio_conf_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_filt_ah_bio_conf_t *val);

typedef enum
{
  ST1VAFE6AX_XL_ULTRA_LIGHT                      = 0x0,
  ST1VAFE6AX_XL_VERY_LIGHT                       = 0x1,
  ST1VAFE6AX_XL_LIGHT                            = 0x2,
  ST1VAFE6AX_XL_MEDIUM                           = 0x3,
  ST1VAFE6AX_XL_STRONG                           = 0x4,
  ST1VAFE6AX_XL_VERY_STRONG                      = 0x5,
  ST1VAFE6AX_XL_AGGRESSIVE                       = 0x6,
  ST1VAFE6AX_XL_XTREME                           = 0x7,
} st1vafe6ax_filt_xl_lp2_bandwidth_t;
int32_t st1vafe6ax_filt_xl_lp2_bandwidth_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_xl_lp2_bandwidth_t val);
int32_t st1vafe6ax_filt_xl_lp2_bandwidth_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_filt_xl_lp2_bandwidth_t *val);

int32_t st1vafe6ax_filt_xl_lp2_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_filt_xl_lp2_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_filt_xl_hp_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_filt_xl_hp_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_filt_xl_fast_settling_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_filt_xl_fast_settling_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_HP_MD_NORMAL                        = 0x0,
  ST1VAFE6AX_HP_MD_REFERENCE                     = 0x1,
} st1vafe6ax_filt_xl_hp_mode_t;
int32_t st1vafe6ax_filt_xl_hp_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_xl_hp_mode_t val);
int32_t st1vafe6ax_filt_xl_hp_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_filt_xl_hp_mode_t *val);

typedef enum
{
  ST1VAFE6AX_WK_FEED_SLOPE                       = 0x0,
  ST1VAFE6AX_WK_FEED_HIGH_PASS                   = 0x1,
  ST1VAFE6AX_WK_FEED_LP_WITH_OFFSET              = 0x2,
} st1vafe6ax_filt_wkup_act_feed_t;
int32_t st1vafe6ax_filt_wkup_act_feed_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_wkup_act_feed_t val);
int32_t st1vafe6ax_filt_wkup_act_feed_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_filt_wkup_act_feed_t *val);

int32_t st1vafe6ax_mask_trigger_xl_settl_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_mask_trigger_xl_settl_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_SIXD_FEED_ODR_DIV_2                 = 0x0,
  ST1VAFE6AX_SIXD_FEED_LOW_PASS                  = 0x1,
} st1vafe6ax_filt_sixd_feed_t;
int32_t st1vafe6ax_filt_sixd_feed_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_filt_sixd_feed_t val);
int32_t st1vafe6ax_filt_sixd_feed_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_filt_sixd_feed_t *val);

int32_t st1vafe6ax_ui_sdo_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_ui_sdo_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_I2C_I3C_ENABLE                      = 0x0,
  ST1VAFE6AX_I2C_I3C_DISABLE                     = 0x1,
} st1vafe6ax_ui_i2c_i3c_mode_t;
int32_t st1vafe6ax_ui_i2c_i3c_mode_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_ui_i2c_i3c_mode_t val);
int32_t st1vafe6ax_ui_i2c_i3c_mode_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_ui_i2c_i3c_mode_t *val);

typedef enum
{
  ST1VAFE6AX_SPI_4_WIRE                          = 0x0,
  ST1VAFE6AX_SPI_3_WIRE                          = 0x1,
} st1vafe6ax_spi_mode_t;
int32_t st1vafe6ax_spi_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_spi_mode_t val);
int32_t st1vafe6ax_spi_mode_get(const stmdev_ctx_t *ctx,
                                st1vafe6ax_spi_mode_t *val);

int32_t st1vafe6ax_ui_sda_pull_up_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_ui_sda_pull_up_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_IBI_2us                             = 0x0,
  ST1VAFE6AX_IBI_50us                            = 0x1,
  ST1VAFE6AX_IBI_1ms                             = 0x2,
  ST1VAFE6AX_IBI_25ms                            = 0x3,
} st1vafe6ax_i3c_ibi_time_t;
int32_t st1vafe6ax_i3c_ibi_time_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_i3c_ibi_time_t val);
int32_t st1vafe6ax_i3c_ibi_time_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_i3c_ibi_time_t *val);

typedef enum
{
  ST1VAFE6AX_PUSH_PULL                           = 0x0,
  ST1VAFE6AX_OPEN_DRAIN                          = 0x1,
} st1vafe6ax_int_pin_mode_t;
int32_t st1vafe6ax_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_int_pin_mode_t val);
int32_t st1vafe6ax_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_int_pin_mode_t *val);

typedef enum
{
  ST1VAFE6AX_ACTIVE_HIGH                         = 0x0,
  ST1VAFE6AX_ACTIVE_LOW                          = 0x1,
} st1vafe6ax_pin_polarity_t;
int32_t st1vafe6ax_pin_polarity_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_pin_polarity_t val);
int32_t st1vafe6ax_pin_polarity_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_pin_polarity_t *val);

typedef struct
{
  uint8_t boot                          : 1;
  uint8_t drdy_xl                       : 1;
  uint8_t drdy_gy                       : 1;
  uint8_t drdy_temp                     : 1;
  uint8_t drdy_ah_bio                   : 1;
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
  uint8_t mlc1                          : 1;
  uint8_t mlc2                          : 1;
  uint8_t mlc3                          : 1;
  uint8_t mlc4                          : 1;
} st1vafe6ax_pin_int_route_t;
int32_t st1vafe6ax_pin_int1_route_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t val);
int32_t st1vafe6ax_pin_int1_route_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t *val);
int32_t st1vafe6ax_pin_int2_route_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t val);
int32_t st1vafe6ax_pin_int2_route_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_pin_int_route_t *val);

int32_t st1vafe6ax_pin_int_en_when_i2c_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_pin_int_en_when_i2c_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_ALL_INT_PULSED                      = 0x0,
  ST1VAFE6AX_BASE_LATCHED_EMB_PULSED             = 0x1,
  ST1VAFE6AX_BASE_PULSED_EMB_LATCHED             = 0x2,
  ST1VAFE6AX_ALL_INT_LATCHED                     = 0x3,
} st1vafe6ax_int_notification_t;
int32_t st1vafe6ax_int_notification_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_int_notification_t val);
int32_t st1vafe6ax_int_notification_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_int_notification_t *val);

typedef enum
{
  ST1VAFE6AX_XL_AND_GY_NOT_AFFECTED              = 0x0,
  ST1VAFE6AX_XL_LOW_POWER_GY_NOT_AFFECTED        = 0x1,
  ST1VAFE6AX_XL_LOW_POWER_GY_SLEEP               = 0x2,
  ST1VAFE6AX_XL_LOW_POWER_GY_POWER_DOWN          = 0x3,
} st1vafe6ax_act_mode_t;
int32_t st1vafe6ax_act_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_act_mode_t val);
int32_t st1vafe6ax_act_mode_get(const stmdev_ctx_t *ctx,
                                st1vafe6ax_act_mode_t *val);

typedef enum
{
  ST1VAFE6AX_SLEEP_TO_ACT_AT_1ST_SAMPLE          = 0x0,
  ST1VAFE6AX_SLEEP_TO_ACT_AT_2ND_SAMPLE          = 0x1,
  ST1VAFE6AX_SLEEP_TO_ACT_AT_3RD_SAMPLE          = 0x2,
  ST1VAFE6AX_SLEEP_TO_ACT_AT_4th_SAMPLE          = 0x3,
} st1vafe6ax_act_from_sleep_to_act_dur_t;
int32_t st1vafe6ax_act_from_sleep_to_act_dur_set(const stmdev_ctx_t *ctx,
                                                 st1vafe6ax_act_from_sleep_to_act_dur_t val);
int32_t st1vafe6ax_act_from_sleep_to_act_dur_get(const stmdev_ctx_t *ctx,
                                                 st1vafe6ax_act_from_sleep_to_act_dur_t *val);

typedef enum
{
  ST1VAFE6AX_1Hz875                              = 0x0,
  ST1VAFE6AX_15Hz                                = 0x1,
  ST1VAFE6AX_30Hz                                = 0x2,
  ST1VAFE6AX_60Hz                                = 0x3,
} st1vafe6ax_act_sleep_xl_odr_t;
int32_t st1vafe6ax_act_sleep_xl_odr_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_act_sleep_xl_odr_t val);
int32_t st1vafe6ax_act_sleep_xl_odr_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_act_sleep_xl_odr_t *val);

typedef struct
{
  uint32_t wk_ths_mg;
  uint32_t inact_ths_mg;
} st1vafe6ax_act_thresholds_t;
int32_t st1vafe6ax_act_thresholds_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_act_thresholds_t val);
int32_t st1vafe6ax_act_thresholds_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_act_thresholds_t *val);

typedef struct
{
  uint8_t shock                         : 2;
  uint8_t quiet                         : 4;
} st1vafe6ax_act_wkup_time_windows_t;
int32_t st1vafe6ax_act_wkup_time_windows_set(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_act_wkup_time_windows_t val);
int32_t st1vafe6ax_act_wkup_time_windows_get(const stmdev_ctx_t *ctx,
                                             st1vafe6ax_act_wkup_time_windows_t *val);

typedef struct
{
  uint8_t tap_x_en                      : 1;
  uint8_t tap_y_en                      : 1;
  uint8_t tap_z_en                      : 1;
} st1vafe6ax_tap_detection_t;
int32_t st1vafe6ax_tap_detection_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_tap_detection_t val);
int32_t st1vafe6ax_tap_detection_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_tap_detection_t *val);

typedef struct
{
  uint8_t x                             : 1;
  uint8_t y                             : 1;
  uint8_t z                             : 1;
} st1vafe6ax_tap_thresholds_t;
int32_t st1vafe6ax_tap_thresholds_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_tap_thresholds_t val);
int32_t st1vafe6ax_tap_thresholds_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_tap_thresholds_t *val);


typedef enum
{
  ST1VAFE6AX_XYZ                                 = 0x3,
  ST1VAFE6AX_YXZ                                 = 0x5,
  ST1VAFE6AX_XZY                                 = 0x6,
  ST1VAFE6AX_ZYX                                 = 0x0,
  ST1VAFE6AX_YZX                                 = 0x1,
  ST1VAFE6AX_ZXY                                 = 0x2,
} st1vafe6ax_tap_axis_priority_t;
int32_t st1vafe6ax_tap_axis_priority_set(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_tap_axis_priority_t val);
int32_t st1vafe6ax_tap_axis_priority_get(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_tap_axis_priority_t *val);

typedef struct
{
  uint8_t shock                         : 1;
  uint8_t quiet                         : 1;
  uint8_t tap_gap                       : 1;
} st1vafe6ax_tap_time_windows_t;
int32_t st1vafe6ax_tap_time_windows_set(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_tap_time_windows_t val);
int32_t st1vafe6ax_tap_time_windows_get(const stmdev_ctx_t *ctx,
                                        st1vafe6ax_tap_time_windows_t *val);

typedef enum
{
  ST1VAFE6AX_ONLY_SINGLE                         = 0x0,
  ST1VAFE6AX_BOTH_SINGLE_DOUBLE                  = 0x1,
} st1vafe6ax_tap_mode_t;
int32_t st1vafe6ax_tap_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_tap_mode_t val);
int32_t st1vafe6ax_tap_mode_get(const stmdev_ctx_t *ctx,
                                st1vafe6ax_tap_mode_t *val);

typedef enum
{
  ST1VAFE6AX_DEG_80                              = 0x0,
  ST1VAFE6AX_DEG_70                              = 0x1,
  ST1VAFE6AX_DEG_60                              = 0x2,
  ST1VAFE6AX_DEG_50                              = 0x3,
} st1vafe6ax_6d_threshold_t;
int32_t st1vafe6ax_6d_threshold_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_6d_threshold_t val);
int32_t st1vafe6ax_6d_threshold_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_6d_threshold_t *val);

int32_t st1vafe6ax_ff_time_windows_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_ff_time_windows_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_156_mg                              = 0x0,
  ST1VAFE6AX_219_mg                              = 0x1,
  ST1VAFE6AX_250_mg                              = 0x2,
  ST1VAFE6AX_312_mg                              = 0x3,
  ST1VAFE6AX_344_mg                              = 0x4,
  ST1VAFE6AX_406_mg                              = 0x5,
  ST1VAFE6AX_469_mg                              = 0x6,
  ST1VAFE6AX_500_mg                              = 0x7,
} st1vafe6ax_ff_thresholds_t;
int32_t st1vafe6ax_ff_thresholds_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_ff_thresholds_t val);
int32_t st1vafe6ax_ff_thresholds_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_ff_thresholds_t *val);

int32_t st1vafe6ax_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_fifo_xl_dual_fsm_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_xl_dual_fsm_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_CMP_DISABLE                         = 0x0,
  ST1VAFE6AX_CMP_8_TO_1                          = 0x1,
  ST1VAFE6AX_CMP_16_TO_1                         = 0x2,
  ST1VAFE6AX_CMP_32_TO_1                         = 0x3,
} st1vafe6ax_fifo_compress_algo_t;
int32_t st1vafe6ax_fifo_compress_algo_set(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_fifo_compress_algo_t val);
int32_t st1vafe6ax_fifo_compress_algo_get(const stmdev_ctx_t *ctx,
                                          st1vafe6ax_fifo_compress_algo_t *val);

int32_t st1vafe6ax_fifo_virtual_sens_odr_chg_set(const stmdev_ctx_t *ctx,
                                                 uint8_t val);
int32_t st1vafe6ax_fifo_virtual_sens_odr_chg_get(const stmdev_ctx_t *ctx,
                                                 uint8_t *val);

int32_t st1vafe6ax_fifo_compress_algo_real_time_set(const stmdev_ctx_t *ctx,
                                                    uint8_t val);
int32_t st1vafe6ax_fifo_compress_algo_real_time_get(const stmdev_ctx_t *ctx,
                                                    uint8_t *val);

int32_t st1vafe6ax_fifo_stop_on_wtm_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_stop_on_wtm_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_XL_NOT_BATCHED                      = 0x0,
  ST1VAFE6AX_XL_BATCHED_AT_1Hz875                = 0x1,
  ST1VAFE6AX_XL_BATCHED_AT_7Hz5                  = 0x2,
  ST1VAFE6AX_XL_BATCHED_AT_15Hz                  = 0x3,
  ST1VAFE6AX_XL_BATCHED_AT_30Hz                  = 0x4,
  ST1VAFE6AX_XL_BATCHED_AT_60Hz                  = 0x5,
  ST1VAFE6AX_XL_BATCHED_AT_120Hz                 = 0x6,
  ST1VAFE6AX_XL_BATCHED_AT_240Hz                 = 0x7,
  ST1VAFE6AX_XL_BATCHED_AT_480Hz                 = 0x8,
  ST1VAFE6AX_XL_BATCHED_AT_960Hz                 = 0x9,
  ST1VAFE6AX_XL_BATCHED_AT_1920Hz                = 0xA,
  ST1VAFE6AX_XL_BATCHED_AT_3840Hz                = 0xB,
} st1vafe6ax_fifo_xl_batch_t;
int32_t st1vafe6ax_fifo_xl_batch_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_xl_batch_t val);
int32_t st1vafe6ax_fifo_xl_batch_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_xl_batch_t *val);

typedef enum
{
  ST1VAFE6AX_GY_NOT_BATCHED                      = 0x0,
  ST1VAFE6AX_GY_BATCHED_AT_1Hz875                = 0x1,
  ST1VAFE6AX_GY_BATCHED_AT_7Hz5                  = 0x2,
  ST1VAFE6AX_GY_BATCHED_AT_15Hz                  = 0x3,
  ST1VAFE6AX_GY_BATCHED_AT_30Hz                  = 0x4,
  ST1VAFE6AX_GY_BATCHED_AT_60Hz                  = 0x5,
  ST1VAFE6AX_GY_BATCHED_AT_120Hz                 = 0x6,
  ST1VAFE6AX_GY_BATCHED_AT_240Hz                 = 0x7,
  ST1VAFE6AX_GY_BATCHED_AT_480Hz                 = 0x8,
  ST1VAFE6AX_GY_BATCHED_AT_960Hz                 = 0x9,
  ST1VAFE6AX_GY_BATCHED_AT_1920Hz                = 0xa,
  ST1VAFE6AX_GY_BATCHED_AT_3840Hz                = 0xb,
} st1vafe6ax_fifo_gy_batch_t;
int32_t st1vafe6ax_fifo_gy_batch_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_gy_batch_t val);
int32_t st1vafe6ax_fifo_gy_batch_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fifo_gy_batch_t *val);

typedef enum
{
  ST1VAFE6AX_BYPASS_MODE                         = 0x0,
  ST1VAFE6AX_FIFO_MODE                           = 0x1,
  ST1VAFE6AX_STREAM_WTM_TO_FULL_MODE             = 0x2,
  ST1VAFE6AX_STREAM_TO_FIFO_MODE                 = 0x3,
  ST1VAFE6AX_BYPASS_TO_STREAM_MODE               = 0x4,
  ST1VAFE6AX_STREAM_MODE                         = 0x6,
  ST1VAFE6AX_BYPASS_TO_FIFO_MODE                 = 0x7,
} st1vafe6ax_fifo_mode_t;
int32_t st1vafe6ax_fifo_mode_set(const stmdev_ctx_t *ctx,
                                 st1vafe6ax_fifo_mode_t val);
int32_t st1vafe6ax_fifo_mode_get(const stmdev_ctx_t *ctx,
                                 st1vafe6ax_fifo_mode_t *val);

typedef enum
{
  ST1VAFE6AX_TEMP_NOT_BATCHED                    = 0x0,
  ST1VAFE6AX_TEMP_BATCHED_AT_1Hz875              = 0x1,
  ST1VAFE6AX_TEMP_BATCHED_AT_15Hz                = 0x2,
  ST1VAFE6AX_TEMP_BATCHED_AT_60Hz                = 0x3,
} st1vafe6ax_fifo_temp_batch_t;
int32_t st1vafe6ax_fifo_temp_batch_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_temp_batch_t val);
int32_t st1vafe6ax_fifo_temp_batch_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_temp_batch_t *val);

typedef enum
{
  ST1VAFE6AX_TMSTMP_NOT_BATCHED                  = 0x0,
  ST1VAFE6AX_TMSTMP_DEC_1                        = 0x1,
  ST1VAFE6AX_TMSTMP_DEC_8                        = 0x2,
  ST1VAFE6AX_TMSTMP_DEC_32                       = 0x3,
} st1vafe6ax_fifo_timestamp_batch_t;
int32_t st1vafe6ax_fifo_timestamp_batch_set(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_timestamp_batch_t val);
int32_t st1vafe6ax_fifo_timestamp_batch_get(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_timestamp_batch_t *val);

int32_t st1vafe6ax_fifo_batch_counter_threshold_set(const stmdev_ctx_t *ctx,
                                                    uint16_t val);
int32_t st1vafe6ax_fifo_batch_counter_threshold_get(const stmdev_ctx_t *ctx,
                                                    uint16_t *val);

int32_t st1vafe6ax_fifo_batch_ah_bio_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_batch_ah_bio_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef enum
{
  ST1VAFE6AX_XL_BATCH_EVENT                      = 0x0,
  ST1VAFE6AX_GY_BATCH_EVENT                      = 0x1,
} st1vafe6ax_fifo_batch_cnt_event_t;
int32_t st1vafe6ax_fifo_batch_cnt_event_set(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_batch_cnt_event_t val);
int32_t st1vafe6ax_fifo_batch_cnt_event_get(const stmdev_ctx_t *ctx,
                                            st1vafe6ax_fifo_batch_cnt_event_t *val);

typedef struct
{
  uint8_t game_rotation                 : 1;
  uint8_t gravity                       : 1;
  uint8_t gbias                         : 1;
} st1vafe6ax_fifo_sflp_raw_t;
int32_t st1vafe6ax_fifo_sflp_batch_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_sflp_raw_t val);
int32_t st1vafe6ax_fifo_sflp_batch_get(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_fifo_sflp_raw_t *val);

typedef struct
{
  uint16_t fifo_level                   : 9;
  uint8_t fifo_bdr                      : 1;
  uint8_t fifo_full                     : 1;
  uint8_t fifo_ovr                      : 1;
  uint8_t fifo_th                       : 1;
} st1vafe6ax_fifo_status_t;

int32_t st1vafe6ax_fifo_status_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_fifo_status_t *val);

typedef struct
{
  enum
  {
    ST1VAFE6AX_FIFO_EMPTY                        = 0x0,
    ST1VAFE6AX_GY_NC_TAG                         = 0x1,
    ST1VAFE6AX_XL_NC_TAG                         = 0x2,
    ST1VAFE6AX_TEMPERATURE_TAG                   = 0x3,
    ST1VAFE6AX_TIMESTAMP_TAG                     = 0x4,
    ST1VAFE6AX_CFG_CHANGE_TAG                    = 0x5,
    ST1VAFE6AX_XL_NC_T_2_TAG                     = 0x6,
    ST1VAFE6AX_XL_NC_T_1_TAG                     = 0x7,
    ST1VAFE6AX_XL_2XC_TAG                        = 0x8,
    ST1VAFE6AX_XL_3XC_TAG                        = 0x9,
    ST1VAFE6AX_GY_NC_T_2_TAG                     = 0xA,
    ST1VAFE6AX_GY_NC_T_1_TAG                     = 0xB,
    ST1VAFE6AX_GY_2XC_TAG                        = 0xC,
    ST1VAFE6AX_GY_3XC_TAG                        = 0xD,
    ST1VAFE6AX_STEP_COUNTER_TAG                  = 0x12,
    ST1VAFE6AX_SFLP_GAME_ROTATION_VECTOR_TAG     = 0x13,
    ST1VAFE6AX_SFLP_GYROSCOPE_BIAS_TAG           = 0x16,
    ST1VAFE6AX_SFLP_GRAVITY_VECTOR_TAG           = 0x17,
    ST1VAFE6AX_MLC_RESULT_TAG                    = 0x1A,
    ST1VAFE6AX_MLC_FILTER                        = 0x1B,
    ST1VAFE6AX_MLC_FEATURE                       = 0x1C,
    ST1VAFE6AX_XL_DUAL_CORE                      = 0x1D,
    ST1VAFE6AX_AH_VAFE                           = 0x1F,
  } tag;
  uint8_t cnt;
  uint8_t data[6];
} st1vafe6ax_fifo_out_raw_t;
int32_t st1vafe6ax_fifo_out_raw_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_fifo_out_raw_t *val);

int32_t st1vafe6ax_fifo_stpcnt_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_stpcnt_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_fifo_mlc_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_mlc_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_fifo_mlc_filt_batch_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fifo_mlc_filt_batch_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  uint8_t step_counter_enable           : 1;
  uint8_t false_step_rej                : 1;
} st1vafe6ax_stpcnt_mode_t;
int32_t st1vafe6ax_stpcnt_mode_set(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_stpcnt_mode_t val);
int32_t st1vafe6ax_stpcnt_mode_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_stpcnt_mode_t *val);

int32_t st1vafe6ax_stpcnt_steps_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe6ax_stpcnt_rst_step_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_stpcnt_rst_step_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_stpcnt_debounce_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_stpcnt_debounce_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_stpcnt_period_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_stpcnt_period_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe6ax_sigmot_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_sigmot_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_tilt_mode_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_tilt_mode_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_sflp_game_rotation_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_sflp_game_rotation_get(const stmdev_ctx_t *ctx, uint16_t *val);

typedef struct
{
  float_t gbias_x; /* dps */
  float_t gbias_y; /* dps */
  float_t gbias_z; /* dps */
} st1vafe6ax_sflp_gbias_t;
int32_t st1vafe6ax_sflp_game_gbias_set(const stmdev_ctx_t *ctx,
                                       st1vafe6ax_sflp_gbias_t *val);

int32_t st1vafe6ax_sflp_configure(const stmdev_ctx_t *ctx);

typedef enum
{
  ST1VAFE6AX_SFLP_15Hz                           = 0x0,
  ST1VAFE6AX_SFLP_30Hz                           = 0x1,
  ST1VAFE6AX_SFLP_60Hz                           = 0x2,
  ST1VAFE6AX_SFLP_120Hz                          = 0x3,
  ST1VAFE6AX_SFLP_240Hz                          = 0x4,
  ST1VAFE6AX_SFLP_480Hz                          = 0x5,
} st1vafe6ax_sflp_data_rate_t;
int32_t st1vafe6ax_sflp_data_rate_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_sflp_data_rate_t val);
int32_t st1vafe6ax_sflp_data_rate_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_sflp_data_rate_t *val);

typedef enum
{
  ST1VAFE6AX_PROTECT_CTRL_REGS                   = 0x0,
  ST1VAFE6AX_WRITE_CTRL_REG                      = 0x1,
} st1vafe6ax_fsm_permission_t;
int32_t st1vafe6ax_fsm_permission_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_fsm_permission_t val);
int32_t st1vafe6ax_fsm_permission_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_fsm_permission_t *val);

typedef enum
{
  ST1VAFE6AX_STD_IF_CONTROL                      = 0x0,
  ST1VAFE6AX_FSM_CONTROL                         = 0x1,
} st1vafe6ax_fsm_permission_status_t;
int32_t st1vafe6ax_fsm_permission_status(const stmdev_ctx_t *ctx,
                                         st1vafe6ax_fsm_permission_status_t *val);

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
} st1vafe6ax_fsm_mode_t;
int32_t st1vafe6ax_fsm_mode_set(const stmdev_ctx_t *ctx, st1vafe6ax_fsm_mode_t val);
int32_t st1vafe6ax_fsm_mode_get(const stmdev_ctx_t *ctx,
                                st1vafe6ax_fsm_mode_t *val);

int32_t st1vafe6ax_fsm_long_cnt_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_fsm_long_cnt_get(const stmdev_ctx_t *ctx, uint16_t *val);

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
} st1vafe6ax_fsm_out_t;
int32_t st1vafe6ax_fsm_out_get(const stmdev_ctx_t *ctx, st1vafe6ax_fsm_out_t *val);

typedef enum
{
  ST1VAFE6AX_FSM_15Hz                            = 0x0,
  ST1VAFE6AX_FSM_30Hz                            = 0x1,
  ST1VAFE6AX_FSM_60Hz                            = 0x2,
  ST1VAFE6AX_FSM_120Hz                           = 0x3,
  ST1VAFE6AX_FSM_240Hz                           = 0x4,
  ST1VAFE6AX_FSM_480Hz                           = 0x5,
  ST1VAFE6AX_FSM_960Hz                           = 0x6,
} st1vafe6ax_fsm_data_rate_t;
int32_t st1vafe6ax_fsm_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fsm_data_rate_t val);
int32_t st1vafe6ax_fsm_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_fsm_data_rate_t *val);

int32_t st1vafe6ax_fsm_long_cnt_timeout_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_fsm_long_cnt_timeout_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe6ax_fsm_number_of_programs_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_fsm_number_of_programs_get(const stmdev_ctx_t *ctx, uint8_t *val);

int32_t st1vafe6ax_fsm_start_address_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_fsm_start_address_get(const stmdev_ctx_t *ctx, uint16_t *val);

typedef enum
{
  ST1VAFE6AX_MLC_OFF                             = 0x0,
  ST1VAFE6AX_MLC_ON                              = 0x1,
  ST1VAFE6AX_MLC_ON_BEFORE_FSM                   = 0x2,
} st1vafe6ax_mlc_mode_t;
int32_t st1vafe6ax_mlc_set(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_mode_t val);
int32_t st1vafe6ax_mlc_get(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_mode_t *val);

typedef enum
{
  ST1VAFE6AX_MLC_15Hz                            = 0x0,
  ST1VAFE6AX_MLC_30Hz                            = 0x1,
  ST1VAFE6AX_MLC_60Hz                            = 0x2,
  ST1VAFE6AX_MLC_120Hz                           = 0x3,
  ST1VAFE6AX_MLC_240Hz                           = 0x4,
  ST1VAFE6AX_MLC_480Hz                           = 0x5,
  ST1VAFE6AX_MLC_960Hz                           = 0x6,
} st1vafe6ax_mlc_data_rate_t;
int32_t st1vafe6ax_mlc_data_rate_set(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_mlc_data_rate_t val);
int32_t st1vafe6ax_mlc_data_rate_get(const stmdev_ctx_t *ctx,
                                     st1vafe6ax_mlc_data_rate_t *val);

typedef struct
{
  uint8_t mlc1_src;
  uint8_t mlc2_src;
  uint8_t mlc3_src;
  uint8_t mlc4_src;
} st1vafe6ax_mlc_out_t;
int32_t st1vafe6ax_mlc_out_get(const stmdev_ctx_t *ctx, st1vafe6ax_mlc_out_t *val);

int32_t st1vafe6ax_mlc_bio_sensitivity_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_mlc_bio_sensitivity_get(const stmdev_ctx_t *ctx, uint16_t *val);

int32_t st1vafe6ax_xl_offset_on_out_set(const stmdev_ctx_t *ctx, uint8_t val);
int32_t st1vafe6ax_xl_offset_on_out_get(const stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  float_t z_mg;
  float_t y_mg;
  float_t x_mg;
} st1vafe6ax_xl_offset_mg_t;
int32_t st1vafe6ax_xl_offset_mg_set(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_offset_mg_t val);
int32_t st1vafe6ax_xl_offset_mg_get(const stmdev_ctx_t *ctx,
                                    st1vafe6ax_xl_offset_mg_t *val);

typedef struct
{
  uint8_t ah_bio1_en                    : 1;
  uint8_t ah_bio2_en                    : 1;
  uint8_t swaps                         : 1;
} st1vafe6ax_ah_bio_mode_t;
int32_t st1vafe6ax_ah_bio_mode_set(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_ah_bio_mode_t val);
int32_t st1vafe6ax_ah_bio_mode_get(const stmdev_ctx_t *ctx,
                                   st1vafe6ax_ah_bio_mode_t *val);

typedef enum
{
  ST1VAFE6AX_2400MOhm                            = 0x0,
  ST1VAFE6AX_730MOhm                             = 0x1,
  ST1VAFE6AX_300MOhm                             = 0x2,
  ST1VAFE6AX_255MOhm                             = 0x3,
} st1vafe6ax_ah_bio_zin_t;
int32_t st1vafe6ax_ah_bio_zin_set(const stmdev_ctx_t *ctx,
                                  st1vafe6ax_ah_bio_zin_t val);
int32_t st1vafe6ax_ah_bio_zin_get(const stmdev_ctx_t *ctx,
                                  st1vafe6ax_ah_bio_zin_t *val);

int32_t st1vafe6ax_fsm_bio_sensitivity_set(const stmdev_ctx_t *ctx, uint16_t val);
int32_t st1vafe6ax_fsm_bio_sensitivity_get(const stmdev_ctx_t *ctx, uint16_t *val);

typedef enum
{
  ST1VAFE6AX_SW_RST_DYN_ADDRESS_RST              = 0x0,
  ST1VAFE6AX_I3C_GLOBAL_RST                      = 0x1,
} st1vafe6ax_i3c_reset_mode_t;
int32_t st1vafe6ax_i3c_reset_mode_set(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_i3c_reset_mode_t val);
int32_t st1vafe6ax_i3c_reset_mode_get(const stmdev_ctx_t *ctx,
                                      st1vafe6ax_i3c_reset_mode_t *val);

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*ST1VAFE6AX_DRIVER_H */
