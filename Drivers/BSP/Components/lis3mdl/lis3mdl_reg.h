/*
 ******************************************************************************
 * @file    lis3mdl_reg.h
 * @author  Sensors Software Solution Team
 * @brief   This file contains all the functions prototypes for the
 *          lis3mdl_reg.c driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIS3MDL_REGS_H
#define LIS3MDL_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/** @addtogroup LIS3MDL
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

typedef struct {
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

typedef int32_t (*stmdev_write_ptr)(void *, uint8_t, uint8_t *,
                                    uint16_t);
typedef int32_t (*stmdev_read_ptr) (void *, uint8_t, uint8_t *,
                                    uint16_t);

typedef struct {
  /** Component mandatory fields **/
  stmdev_write_ptr  write_reg;
  stmdev_read_ptr   read_reg;
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

typedef struct {
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

/** @defgroup LIS3MDL_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> 0x39 if SA0=1 -> 0x3D **/
#define LIS3MDL_I2C_ADD_L   0x39U
#define LIS3MDL_I2C_ADD_H   0x3DU

/** Device Identification (Who am I) **/
#define LIS3MDL_ID          0x3DU

/**
  * @}
  *
  */

#define LIS3MDL_WHO_AM_I       0x0FU
#define LIS3MDL_CTRL_REG1      0x20U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t st              : 1;
  uint8_t om              : 6; /* om + do + fast_odr -> om */
  uint8_t temp_en         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp_en         : 1;
  uint8_t om              : 6; /* om + do + fast_odr -> om */
  uint8_t st              : 1;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_ctrl_reg1_t;

#define LIS3MDL_CTRL_REG2      0x21U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01     : 2;
  uint8_t soft_rst        : 1;
  uint8_t reboot          : 1;
  uint8_t not_used_02     : 1;
  uint8_t fs              : 2;
  uint8_t not_used_03     : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_03     : 1;
  uint8_t fs              : 2;
  uint8_t not_used_02     : 1;
  uint8_t reboot          : 1;
  uint8_t soft_rst        : 1;
  uint8_t not_used_01     : 2;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_ctrl_reg2_t;

#define LIS3MDL_CTRL_REG3      0x22U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t md              : 2;
  uint8_t sim             : 1;
  uint8_t not_used_01     : 2;
  uint8_t lp              : 1;
  uint8_t not_used_02     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02     : 2;
  uint8_t lp              : 1;
  uint8_t not_used_01     : 2;
  uint8_t sim             : 1;
  uint8_t md              : 2;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_ctrl_reg3_t;

#define LIS3MDL_CTRL_REG4      0x23U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01     : 1;
  uint8_t ble             : 1;
  uint8_t omz             : 2;
  uint8_t not_used_02     : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02     : 4;
  uint8_t omz             : 2;
  uint8_t ble             : 1;
  uint8_t not_used_01     : 1;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_ctrl_reg4_t;

#define LIS3MDL_CTRL_REG5      0x24U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01     : 6;
  uint8_t bdu             : 1;
  uint8_t fast_read       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fast_read       : 1;
  uint8_t bdu             : 1;
  uint8_t not_used_01     : 6;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_ctrl_reg5_t;

#define LIS3MDL_STATUS_REG     0x27U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xda             : 1;
  uint8_t yda             : 1;
  uint8_t zda             : 1;
  uint8_t zyxda           : 1;
  uint8_t _xor            : 1;
  uint8_t yor             : 1;
  uint8_t zor             : 1;
  uint8_t zyxor           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t zyxor           : 1;
  uint8_t zor             : 1;
  uint8_t yor             : 1;
  uint8_t _xor            : 1;
  uint8_t zyxda           : 1;
  uint8_t zda             : 1;
  uint8_t yda             : 1;
  uint8_t xda             : 1;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_status_reg_t;

#define LIS3MDL_OUT_X_L        0x28U
#define LIS3MDL_OUT_X_H        0x29U
#define LIS3MDL_OUT_Y_L        0x2AU
#define LIS3MDL_OUT_Y_H        0x2BU
#define LIS3MDL_OUT_Z_L        0x2CU
#define LIS3MDL_OUT_Z_H        0x2DU
#define LIS3MDL_TEMP_OUT_L     0x2EU
#define LIS3MDL_TEMP_OUT_H     0x2FU
#define LIS3MDL_INT_CFG        0x30U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ien             : 1;
  uint8_t lir             : 1;
  uint8_t iea             : 1;
  uint8_t not_used_01     : 2;
  uint8_t zien            : 1;
  uint8_t yien            : 1;
  uint8_t xien            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t xien            : 1;
  uint8_t yien            : 1;
  uint8_t zien            : 1;
  uint8_t not_used_01     : 2;
  uint8_t iea             : 1;
  uint8_t lir             : 1;
  uint8_t ien             : 1;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_int_cfg_t;

#define LIS3MDL_INT_SRC        0x31U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_            : 1;
  uint8_t mroi            : 1;
  uint8_t nth_z           : 1;
  uint8_t nth_y           : 1;
  uint8_t nth_x           : 1;
  uint8_t pth_z           : 1;
  uint8_t pth_y           : 1;
  uint8_t pth_x           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t pth_x           : 1;
  uint8_t pth_y           : 1;
  uint8_t pth_z           : 1;
  uint8_t nth_x           : 1;
  uint8_t nth_y           : 1;
  uint8_t nth_z           : 1;
  uint8_t mroi            : 1;
  uint8_t int_            : 1;
#endif /* DRV_BYTE_ORDER */
} lis3mdl_int_src_t;

#define LIS3MDL_INT_THS_L      0x32U
#define LIS3MDL_INT_THS_H      0x33U

/**
  * @defgroup LIS3MDL_Register_Union
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
typedef union {
  lis3mdl_ctrl_reg1_t      ctrl_reg1;
  lis3mdl_ctrl_reg2_t      ctrl_reg2;
  lis3mdl_ctrl_reg3_t      ctrl_reg3;
  lis3mdl_ctrl_reg4_t      ctrl_reg4;
  lis3mdl_ctrl_reg5_t      ctrl_reg5;
  lis3mdl_status_reg_t     status_reg;
  lis3mdl_int_cfg_t        int_cfg;
  lis3mdl_int_src_t        int_src;
  bitwise_t                bitwise;
  uint8_t                  byte;
} lis3mdl_reg_t;

/**
  * @}
  *
  */

int32_t lis3mdl_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                         uint8_t *data,
                         uint16_t len);
int32_t lis3mdl_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                          uint8_t *data,
                          uint16_t len);

float lis3mdl_from_fs4_to_gauss(int16_t lsb);
float lis3mdl_from_fs8_to_gauss(int16_t lsb);
float lis3mdl_from_fs12_to_gauss(int16_t lsb);
float lis3mdl_from_fs16_to_gauss(int16_t lsb);
float lis3mdl_from_lsb_to_celsius(int16_t lsb);

typedef enum {
  LIS3MDL_LP_Hz625      = 0x00,
  LIS3MDL_LP_1kHz       = 0x01,
  LIS3MDL_MP_560Hz      = 0x11,
  LIS3MDL_HP_300Hz      = 0x21,
  LIS3MDL_UHP_155Hz     = 0x31,

  LIS3MDL_LP_1Hz25      = 0x02,
  LIS3MDL_LP_2Hz5       = 0x04,
  LIS3MDL_LP_5Hz        = 0x06,
  LIS3MDL_LP_10Hz       = 0x08,
  LIS3MDL_LP_20Hz       = 0x0A,
  LIS3MDL_LP_40Hz       = 0x0C,
  LIS3MDL_LP_80Hz       = 0x0E,

  LIS3MDL_MP_1Hz25      = 0x12,
  LIS3MDL_MP_2Hz5       = 0x14,
  LIS3MDL_MP_5Hz        = 0x16,
  LIS3MDL_MP_10Hz       = 0x18,
  LIS3MDL_MP_20Hz       = 0x1A,
  LIS3MDL_MP_40Hz       = 0x1C,
  LIS3MDL_MP_80Hz       = 0x1E,

  LIS3MDL_HP_1Hz25      = 0x22,
  LIS3MDL_HP_2Hz5       = 0x24,
  LIS3MDL_HP_5Hz        = 0x26,
  LIS3MDL_HP_10Hz       = 0x28,
  LIS3MDL_HP_20Hz       = 0x2A,
  LIS3MDL_HP_40Hz       = 0x2C,
  LIS3MDL_HP_80Hz       = 0x2E,

  LIS3MDL_UHP_1Hz25     = 0x32,
  LIS3MDL_UHP_2Hz5      = 0x34,
  LIS3MDL_UHP_5Hz       = 0x36,
  LIS3MDL_UHP_10Hz      = 0x38,
  LIS3MDL_UHP_20Hz      = 0x3A,
  LIS3MDL_UHP_40Hz      = 0x3C,
  LIS3MDL_UHP_80Hz      = 0x3E,

} lis3mdl_om_t;
int32_t lis3mdl_data_rate_set(stmdev_ctx_t *ctx, lis3mdl_om_t val);
int32_t lis3mdl_data_rate_get(stmdev_ctx_t *ctx, lis3mdl_om_t *val);

int32_t lis3mdl_temperature_meas_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_temperature_meas_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum {
  LIS3MDL_4_GAUSS   = 0,
  LIS3MDL_8_GAUSS   = 1,
  LIS3MDL_12_GAUSS  = 2,
  LIS3MDL_16_GAUSS  = 3,
} lis3mdl_fs_t;
int32_t lis3mdl_full_scale_set(stmdev_ctx_t *ctx, lis3mdl_fs_t val);
int32_t lis3mdl_full_scale_get(stmdev_ctx_t *ctx, lis3mdl_fs_t *val);

typedef enum {
  LIS3MDL_CONTINUOUS_MODE  = 0,
  LIS3MDL_SINGLE_TRIGGER   = 1,
  LIS3MDL_POWER_DOWN       = 2,
} lis3mdl_md_t;
int32_t lis3mdl_operating_mode_set(stmdev_ctx_t *ctx,
                                   lis3mdl_md_t val);
int32_t lis3mdl_operating_mode_get(stmdev_ctx_t *ctx,
                                   lis3mdl_md_t *val);

int32_t lis3mdl_fast_low_power_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_fast_low_power_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_block_data_update_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_block_data_update_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lis3mdl_high_part_cycle_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_high_part_cycle_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_mag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_mag_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_magnetic_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t lis3mdl_temperature_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t lis3mdl_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t lis3mdl_self_test_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_self_test_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_reset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_boot_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_boot_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum {
  LIS3MDL_LSB_AT_LOW_ADD  = 0,
  LIS3MDL_MSB_AT_LOW_ADD  = 1,
} lis3mdl_ble_t;
int32_t lis3mdl_data_format_set(stmdev_ctx_t *ctx, lis3mdl_ble_t val);
int32_t lis3mdl_data_format_get(stmdev_ctx_t *ctx,
                                lis3mdl_ble_t *val);

int32_t lis3mdl_status_get(stmdev_ctx_t *ctx,
                           lis3mdl_status_reg_t *val);

int32_t lis3mdl_int_config_set(stmdev_ctx_t *ctx,
                               lis3mdl_int_cfg_t *val);
int32_t lis3mdl_int_config_get(stmdev_ctx_t *ctx,
                               lis3mdl_int_cfg_t *val);

int32_t lis3mdl_int_generation_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_int_generation_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum {
  LIS3MDL_INT_PULSED   = 0,
  LIS3MDL_INT_LATCHED  = 1,
} lis3mdl_lir_t;
int32_t lis3mdl_int_notification_mode_set(stmdev_ctx_t *ctx,
                                          lis3mdl_lir_t val);
int32_t lis3mdl_int_notification_mode_get(stmdev_ctx_t *ctx,
                                          lis3mdl_lir_t *val);

typedef enum {
  LIS3MDL_ACTIVE_HIGH  = 0,
  LIS3MDL_ACTIVE_LOW   = 1,
} lis3mdl_iea_t;
int32_t lis3mdl_int_polarity_set(stmdev_ctx_t *ctx,
                                 lis3mdl_iea_t val);
int32_t lis3mdl_int_polarity_get(stmdev_ctx_t *ctx,
                                 lis3mdl_iea_t *val);

int32_t lis3mdl_int_on_z_ax_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_int_on_z_ax_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_on_y_ax_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_int_on_y_ax_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_on_x_ax_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lis3mdl_int_on_x_ax_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_source_get(stmdev_ctx_t *ctx,
                               lis3mdl_int_src_t *val);

int32_t lis3mdl_interrupt_event_flag_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

int32_t lis3mdl_int_mag_over_range_flag_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lis3mdl_int_neg_z_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_neg_y_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_neg_x_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_pos_z_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_pos_y_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_pos_x_flag_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lis3mdl_int_threshold_set(stmdev_ctx_t *ctx, uint16_t val);
int32_t lis3mdl_int_threshold_get(stmdev_ctx_t *ctx, uint16_t *val);

typedef enum {
  LIS3MDL_SPI_4_WIRE   = 0,
  LIS3MDL_SPI_3_WIRE   = 1,
} lis3mdl_sim_t;
int32_t lis3mdl_spi_mode_set(stmdev_ctx_t *ctx, lis3mdl_sim_t val);
int32_t lis3mdl_spi_mode_get(stmdev_ctx_t *ctx, lis3mdl_sim_t *val);

/**
  *@}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LIS3MDL_REGS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
