/*
  ******************************************************************************
  * @file    lps28dfw_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          lps28dfw_reg.c driver.
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
#ifndef LPS28DFW_REGS_H
#define LPS28DFW_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LPS28DFW
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

/** @defgroup LPS28DFW_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format **/
#define LPS28DFW_I2C_ADD_L               0xB9U
#define LPS28DFW_I2C_ADD_H               0xBBU

/** Device Identification (Who am I) **/
#define LPS28DFW_ID                      0xB4U

/**
  * @}
  *
  */

#define LPS28DFW_INTERRUPT_CFG           0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t phe              : 1;
  uint8_t ple              : 1;
  uint8_t lir              : 1;
  uint8_t not_used_01      : 1;
  uint8_t reset_az         : 1;
  uint8_t autozero         : 1;
  uint8_t reset_arp        : 1;
  uint8_t autorefp         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t autorefp         : 1;
  uint8_t reset_arp        : 1;
  uint8_t autozero         : 1;
  uint8_t reset_az         : 1;
  uint8_t not_used_01      : 1;
  uint8_t lir              : 1;
  uint8_t ple              : 1;
  uint8_t phe              : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_interrupt_cfg_t;

#define LPS28DFW_THS_P_L                 0x0CU
typedef struct
{
  uint8_t ths              : 8;
} lps28dfw_ths_p_l_t;

#define LPS28DFW_THS_P_H                 0x0DU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ths              : 7;
  uint8_t not_used_01      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 1;
  uint8_t ths              : 7;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_ths_p_h_t;

#define LPS28DFW_IF_CTRL                 0x0EU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01      : 2;
  uint8_t int_pd_dis      : 1;
  uint8_t not_used_02      : 1;
  uint8_t sda_pu_en        : 1;
  uint8_t not_used_03      : 2;
  uint8_t int_en_i3c       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_en_i3c       : 1;
  uint8_t not_used_03      : 2;
  uint8_t sda_pu_en        : 1;
  uint8_t not_used_02      : 1;
  uint8_t int_pd_dis      : 1;
  uint8_t not_used_01      : 2;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_if_ctrl_t;

#define LPS28DFW_WHO_AM_I                0x0FU
#define LPS28DFW_CTRL_REG1               0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t avg              : 3;
  uint8_t odr              : 4;
  uint8_t not_used_01      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 1;
  uint8_t odr              : 4;
  uint8_t avg              : 3;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_ctrl_reg1_t;

#define LPS28DFW_CTRL_REG2               0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t oneshot          : 1;
  uint8_t not_used_01      : 1;
  uint8_t swreset          : 1;
  uint8_t bdu              : 1;
  uint8_t en_lpfp          : 1;
  uint8_t lfpf_cfg         : 1;
  uint8_t fs_mode          : 1;
  uint8_t boot             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot             : 1;
  uint8_t fs_mode          : 1;
  uint8_t lfpf_cfg         : 1;
  uint8_t en_lpfp          : 1;
  uint8_t bdu              : 1;
  uint8_t swreset          : 1;
  uint8_t not_used_01      : 1;
  uint8_t oneshot          : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_ctrl_reg2_t;

#define LPS28DFW_CTRL_REG3               0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t if_add_inc       : 1;
  uint8_t pp_od            : 1;
  uint8_t not_used_02      : 1;
  uint8_t int_h_l          : 1;
  uint8_t not_used_01      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 4;
  uint8_t int_h_l          : 1;
  uint8_t not_used_02      : 1;
  uint8_t pp_od            : 1;
  uint8_t if_add_inc       : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_ctrl_reg3_t;

#define LPS28DFW_CTRL_REG4               0x13U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_f_ovr        : 1;
  uint8_t int_f_wtm        : 1;
  uint8_t int_f_full       : 1;
  uint8_t not_used_02      : 1;
  uint8_t int_en           : 1;
  uint8_t drdy             : 1;
  uint8_t drdy_pls         : 1;
  uint8_t not_used_01      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 1;
  uint8_t drdy_pls         : 1;
  uint8_t drdy             : 1;
  uint8_t int_en           : 1;
  uint8_t not_used_02      : 1;
  uint8_t int_f_full       : 1;
  uint8_t int_f_wtm        : 1;
  uint8_t int_f_ovr        : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_ctrl_reg4_t;

#define LPS28DFW_FIFO_CTRL               0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t f_mode           : 2;
  uint8_t trig_modes       : 1;
  uint8_t stop_on_wtm      : 1;
  uint8_t not_used_01      : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 4;
  uint8_t stop_on_wtm      : 1;
  uint8_t trig_modes       : 1;
  uint8_t f_mode           : 2;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_fifo_ctrl_t;

#define LPS28DFW_FIFO_WTM                0x15U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm              : 7;
  uint8_t not_used_01      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 1;
  uint8_t wtm              : 7;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_fifo_wtm_t;

#define LPS28DFW_REF_P_L                 0x16U
typedef struct
{
  uint8_t refp             : 8;
} lps28dfw_ref_p_l_t;

#define LPS28DFW_REF_P_H                 0x17U
typedef struct
{
  uint8_t refp             : 8;
} lps28dfw_ref_p_h_t;

#define LPS28DFW_I3C_IF_CTRL_ADD         0x19U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t I3C_Bus_Avb_Sel  : 2;
  uint8_t not_used_02      : 3;
  uint8_t asf_on           : 1;
  uint8_t not_used_01      : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 2;
  uint8_t asf_on           : 1;
  uint8_t not_used_02      : 3;
  uint8_t I3C_Bus_Avb_Sel  : 2;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_i3c_if_ctrl_add_t;

#define LPS28DFW_RPDS_L                  0x1AU
#define LPS28DFW_RPDS_H                  0x1BU
#define LPS28DFW_INT_SOURCE              0x24U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ph               : 1;
  uint8_t pl               : 1;
  uint8_t ia               : 1;
  uint8_t not_used_01      : 4;
  uint8_t boot_on          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot_on          : 1;
  uint8_t not_used_01      : 4;
  uint8_t ia               : 1;
  uint8_t pl               : 1;
  uint8_t ph               : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_int_source_t;

#define LPS28DFW_FIFO_STATUS1            0x25U
typedef struct
{
  uint8_t fss              : 8;
} lps28dfw_fifo_status1_t;

#define LPS28DFW_FIFO_STATUS2            0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01      : 5;
  uint8_t fifo_full_ia     : 1;
  uint8_t fifo_ovr_ia      : 1;
  uint8_t fifo_wtm_ia      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fifo_wtm_ia      : 1;
  uint8_t fifo_ovr_ia      : 1;
  uint8_t fifo_full_ia     : 1;
  uint8_t not_used_01      : 5;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_fifo_status2_t;

#define LPS28DFW_STATUS                  0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t p_da             : 1;
  uint8_t t_da             : 1;
  uint8_t not_used_01      : 2;
  uint8_t p_or             : 1;
  uint8_t t_or             : 1;
  uint8_t not_used_02      : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02      : 2;
  uint8_t t_or             : 1;
  uint8_t p_or             : 1;
  uint8_t not_used_01      : 2;
  uint8_t t_da             : 1;
  uint8_t p_da             : 1;
#endif /* DRV_BYTE_ORDER */
} lps28dfw_status_t;

#define LPS28DFW_PRESS_OUT_XL            0x28U
#define LPS28DFW_PRESS_OUT_L             0x29U
#define LPS28DFW_PRESS_OUT_H             0x2AU
#define LPS28DFW_TEMP_OUT_L              0x2BU
#define LPS28DFW_TEMP_OUT_H              0x2CU
#define LPS28DFW_FIFO_DATA_OUT_PRESS_XL  0x78U
#define LPS28DFW_FIFO_DATA_OUT_PRESS_L   0x79U
#define LPS28DFW_FIFO_DATA_OUT_PRESS_H   0x7AU

/**
  * @defgroup LPS28DFW_Register_Union
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
  lps28dfw_interrupt_cfg_t    interrupt_cfg;
  lps28dfw_ths_p_l_t          ths_p_l;
  lps28dfw_ths_p_h_t          ths_p_h;
  lps28dfw_if_ctrl_t          if_ctrl;
  lps28dfw_ctrl_reg1_t        ctrl_reg1;
  lps28dfw_ctrl_reg2_t        ctrl_reg2;
  lps28dfw_ctrl_reg3_t        ctrl_reg3;
  lps28dfw_ctrl_reg4_t        ctrl_reg4;
  lps28dfw_fifo_ctrl_t        fifo_ctrl;
  lps28dfw_fifo_wtm_t         fifo_wtm;
  lps28dfw_ref_p_l_t          ref_p_l;
  lps28dfw_ref_p_h_t          ref_p_h;
  lps28dfw_i3c_if_ctrl_add_t  i3c_if_ctrl_add;
  lps28dfw_int_source_t       int_source;
  lps28dfw_fifo_status1_t     fifo_status1;
  lps28dfw_fifo_status2_t     fifo_status2;
  lps28dfw_status_t           status;
  bitwise_t                  bitwise;
  uint8_t                    byte;
} lps28dfw_reg_t;

int32_t lps28dfw_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                          uint8_t *data, uint16_t len);
int32_t lps28dfw_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                           uint8_t *data, uint16_t len);

extern float_t lps28dfw_from_fs1260_to_hPa(int32_t lsb);
extern float_t lps28dfw_from_fs4000_to_hPa(int32_t lsb);

extern float_t lps28dfw_from_lsb_to_celsius(int16_t lsb);

typedef struct
{
  uint8_t whoami;
} lps28dfw_id_t;
int32_t lps28dfw_id_get(stmdev_ctx_t *ctx, lps28dfw_id_t *val);

typedef struct
{
  enum
  {
    LPS28DFW_SEL_BY_HW      = 0x00, /* bus mode select by HW (SPI 3W disable) */
    LPS28DFW_INT_PIN_ON_I3C = 0x04, /* INT pin polarized as OUT with I3C */
  } interface;
  enum
  {
    LPS28DFW_AUTO      = 0x00, /* bus mode select by HW (SPI 3W disable) */
    LPS28DFW_ALWAYS_ON = 0x01, /* Only SPI: SDO / SDI separated pins */
  } filter;
  enum
  {
    LPS28DFW_BUS_AVB_TIME_50us      = 0x00, /* bus available time equal to 50 us */
    LPS28DFW_BUS_AVB_TIME_2us       = 0x01, /* bus available time equal to 2 us */
    LPS28DFW_BUS_AVB_TIME_1ms       = 0x02, /* bus available time equal to 1 ms */
    LPS28DFW_BUS_AVB_TIME_25ms      = 0x03, /* bus available time equal to 25 ms */
  } bus_avb_time;
} lps28dfw_bus_mode_t;
int32_t lps28dfw_bus_mode_set(stmdev_ctx_t *ctx, lps28dfw_bus_mode_t *val);
int32_t lps28dfw_bus_mode_get(stmdev_ctx_t *ctx, lps28dfw_bus_mode_t *val);

typedef enum
{
  LPS28DFW_DRV_RDY = 0x00, /* Initialize the device for driver usage */
  LPS28DFW_BOOT    = 0x01, /* Restore calib. param. ( it takes 10ms ) */
  LPS28DFW_RESET   = 0x02, /* Reset configuration registers */
} lps28dfw_init_t;
int32_t lps28dfw_init_set(stmdev_ctx_t *ctx, lps28dfw_init_t val);

typedef struct
{
  uint8_t sw_reset  : 1; /* Restoring configuration registers. */
  uint8_t boot      : 1; /* Restoring calibration parameters. */
  uint8_t drdy_pres : 1; /* Pressure data ready. */
  uint8_t drdy_temp : 1; /* Temperature data ready. */
  uint8_t ovr_pres  : 1; /* Pressure data overrun. */
  uint8_t ovr_temp  : 1; /* Temperature data overrun. */
  uint8_t end_meas  : 1; /* Single measurement is finished. */
  uint8_t ref_done  : 1; /* Auto-Zero value is set. */
} lps28dfw_stat_t;
int32_t lps28dfw_status_get(stmdev_ctx_t *ctx, lps28dfw_stat_t *val);

typedef struct
{
  uint8_t int_push_pull : 1; /* 1 = push-pull / 0 = open-drain*/
  uint8_t int_pull_down : 1; /* 1 = pull-down always disabled (0=auto) */
  uint8_t sda_pull_up : 1; /* 1 = pull-up always disabled */
} lps28dfw_pin_conf_t;
int32_t lps28dfw_pin_conf_set(stmdev_ctx_t *ctx, lps28dfw_pin_conf_t *val);
int32_t lps28dfw_pin_conf_get(stmdev_ctx_t *ctx, lps28dfw_pin_conf_t *val);

typedef struct
{
  uint8_t drdy_pres   :  1; /* Pressure data ready */
  uint8_t drdy_temp   :  1; /* Temperature data ready */
  uint8_t over_pres   :  1; /* Over pressure event */
  uint8_t under_pres  :  1; /* Under pressure event */
  uint8_t thrsld_pres :  1; /* Over/Under pressure event */
  uint8_t fifo_full   :  1; /* FIFO full */
  uint8_t fifo_ovr    :  1; /* FIFO overrun */
  uint8_t fifo_th     :  1; /* FIFO threshold reached */
} lps28dfw_all_sources_t;
int32_t lps28dfw_all_sources_get(stmdev_ctx_t *ctx,
                                 lps28dfw_all_sources_t *val);

typedef struct
{
  enum
  {
    LPS28DFW_1260hPa = 0x00,
    LPS28DFW_4000hPa = 0x01,
  } fs;
  enum
  {
    LPS28DFW_ONE_SHOT = 0x00, /* Device in power down till software trigger */
    LPS28DFW_1Hz      = 0x01,
    LPS28DFW_4Hz      = 0x02,
    LPS28DFW_10Hz     = 0x03,
    LPS28DFW_25Hz     = 0x04,
    LPS28DFW_50Hz     = 0x05,
    LPS28DFW_75Hz     = 0x06,
    LPS28DFW_100Hz    = 0x07,
    LPS28DFW_200Hz    = 0x08,
  } odr;
  enum
  {
    LPS28DFW_4_AVG   = 0,
    LPS28DFW_8_AVG   = 1,
    LPS28DFW_16_AVG  = 2,
    LPS28DFW_32_AVG  = 3,
    LPS28DFW_64_AVG  = 4,
    LPS28DFW_128_AVG = 5,
    LPS28DFW_256_AVG = 6,
    LPS28DFW_512_AVG = 7,
  } avg;
  enum
  {
    LPS28DFW_LPF_DISABLE   = 0,
    LPS28DFW_LPF_ODR_DIV_4 = 1,
    LPS28DFW_LPF_ODR_DIV_9 = 3,
  } lpf;
} lps28dfw_md_t;
int32_t lps28dfw_mode_set(stmdev_ctx_t *ctx, lps28dfw_md_t *val);
int32_t lps28dfw_mode_get(stmdev_ctx_t *ctx, lps28dfw_md_t *val);

int32_t lps28dfw_trigger_sw(stmdev_ctx_t *ctx, lps28dfw_md_t *md);

typedef struct
{
  struct
  {
    float_t hpa;
    int32_t raw; /* 32 bit signed-left algned  format left  */
  } pressure;
  struct
  {
    float_t deg_c;
    int16_t raw;
  } heat;
} lps28dfw_data_t;
int32_t lps28dfw_data_get(stmdev_ctx_t *ctx, lps28dfw_md_t *md,
                          lps28dfw_data_t *data);

typedef struct
{
  enum
  {
    LPS28DFW_BYPASS           = 0,
    LPS28DFW_FIFO             = 1,
    LPS28DFW_STREAM           = 2,
    LPS28DFW_STREAM_TO_FIFO   = 7, /* Dynamic-Stream, FIFO on Trigger */
    LPS28DFW_BYPASS_TO_STREAM = 6, /* Bypass, Dynamic-Stream on Trigger */
    LPS28DFW_BYPASS_TO_FIFO   = 5, /* Bypass, FIFO on Trigger */
  } operation;
  uint8_t watermark; /* (0 disable) max 128.*/
} lps28dfw_fifo_md_t;
int32_t lps28dfw_fifo_mode_set(stmdev_ctx_t *ctx, lps28dfw_fifo_md_t *val);
int32_t lps28dfw_fifo_mode_get(stmdev_ctx_t *ctx, lps28dfw_fifo_md_t *val);

int32_t lps28dfw_fifo_level_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef struct
{
  float_t hpa;
  int32_t raw;
} lps28dfw_fifo_data_t;
int32_t lps28dfw_fifo_data_get(stmdev_ctx_t *ctx, uint8_t samp,
                               lps28dfw_md_t *md, lps28dfw_fifo_data_t *data);

typedef struct
{
  uint8_t int_latched  : 1; /* int events are: int on threshold, FIFO */
  uint8_t active_low   : 1; /* 1 = active low / 0 = active high */
  uint8_t drdy_latched : 1; /* pulsed ~5 us with enabled drdy_pres " */
} lps28dfw_int_mode_t;
int32_t lps28dfw_interrupt_mode_set(stmdev_ctx_t *ctx,
                                    lps28dfw_int_mode_t *val);
int32_t lps28dfw_interrupt_mode_get(stmdev_ctx_t *ctx,
                                    lps28dfw_int_mode_t *val);

typedef struct
{
  uint8_t drdy_pres : 1; /* Pressure data ready */
  uint8_t fifo_th   : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr  : 1; /* FIFO overrun */
  uint8_t fifo_full : 1; /* FIFO full */
} lps28dfw_pin_int_route_t;
int32_t lps28dfw_pin_int_route_set(stmdev_ctx_t *ctx,
                                   lps28dfw_pin_int_route_t *val);
int32_t lps28dfw_pin_int_route_get(stmdev_ctx_t *ctx,
                                   lps28dfw_pin_int_route_t *val);

typedef struct
{
  uint16_t threshold;   /* Threshold in hPa * 16 (@1260hPa)
                         * Threshold in hPa * 8  (@4000hPa)
                         */
  uint8_t over_th  : 1; /* Pressure data over threshold event */
  uint8_t under_th : 1; /* Pressure data under threshold event */
} lps28dfw_int_th_md_t;
int32_t lps28dfw_int_on_threshold_mode_set(stmdev_ctx_t *ctx,
                                           lps28dfw_int_th_md_t *val);
int32_t lps28dfw_int_on_threshold_mode_get(stmdev_ctx_t *ctx,
                                           lps28dfw_int_th_md_t *val);

typedef struct
{
  enum
  {
    LPS28DFW_OUT_AND_INTERRUPT = 0,
    LPS28DFW_ONLY_INTERRUPT    = 1,
    LPS28DFW_RST_REFS          = 2,
  } apply_ref;
  uint8_t get_ref : 1; /* Use current pressure value as reference */
} lps28dfw_ref_md_t;
int32_t lps28dfw_reference_mode_set(stmdev_ctx_t *ctx,
                                    lps28dfw_ref_md_t *val);
int32_t lps28dfw_reference_mode_get(stmdev_ctx_t *ctx,
                                    lps28dfw_ref_md_t *val);

int32_t lps28dfw_refp_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t lps28dfw_opc_set(stmdev_ctx_t *ctx, int16_t val);
int32_t lps28dfw_opc_get(stmdev_ctx_t *ctx, int16_t *val);

/**
  *@}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LPS28DFW_REGS_H */
