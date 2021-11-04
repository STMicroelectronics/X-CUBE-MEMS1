/*
 ******************************************************************************
 * @file    lsm303agr_reg.h
 * @author  Sensors Software Solution Team
 * @brief   This file contains all the functions prototypes for the
 *          lsm303agr_reg.c driver.
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
#ifndef LSM303AGR_REGS_H
#define LSM303AGR_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/** @addtogroup LSM303AGR
  * @{
  *
  */

/** @defgroup Endianness definitions
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

/** @defgroup lsm303agr_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format**/
#define LSM303AGR_I2C_ADD_XL       0x33U
#define LSM303AGR_I2C_ADD_MG       0x3DU

/** Device Identification (Who am I) **/
#define LSM303AGR_ID_XL            0x33U
#define LSM303AGR_ID_MG            0x40U

/**
  * @}
  *
  */

#define LSM303AGR_STATUS_REG_AUX_A           0x07U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01            : 2;
  uint8_t tda                    : 1;
  uint8_t not_used_02            : 3;
  uint8_t tor                    : 1;
  uint8_t not_used_03            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_03            : 1;
  uint8_t tor                    : 1;
  uint8_t not_used_02            : 3;
  uint8_t tda                    : 1;
  uint8_t not_used_01            : 2;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_status_reg_aux_a_t;

#define LSM303AGR_OUT_TEMP_L_A               0x0CU
#define LSM303AGR_OUT_TEMP_H_A               0x0DU
#define LSM303AGR_INT_COUNTER_REG_A          0x0EU
#define LSM303AGR_WHO_AM_I_A                 0x0FU

#define LSM303AGR_TEMP_CFG_REG_A             0x1FU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01            : 6;
  uint8_t temp_en                : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t temp_en                : 2;
  uint8_t not_used_01            : 6;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_temp_cfg_reg_a_t;

#define LSM303AGR_CTRL_REG1_A                0x20U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xen                    : 1;
  uint8_t yen                    : 1;
  uint8_t zen                    : 1;
  uint8_t lpen                   : 1;
  uint8_t odr                    : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t odr                    : 4;
  uint8_t lpen                   : 1;
  uint8_t zen                    : 1;
  uint8_t yen                    : 1;
  uint8_t xen                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg1_a_t;

#define LSM303AGR_CTRL_REG2_A                0x21U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
uint8_t hp                     :
  3; /* HPCLICK + HPIS2 + HPIS1 -> HP */
  uint8_t fds                    : 1;
  uint8_t hpcf                   : 2;
  uint8_t hpm                    : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t hpm                    : 2;
  uint8_t hpcf                   : 2;
  uint8_t fds                    : 1;
uint8_t hp                     :
  3; /* HPCLICK + HPIS2 + HPIS1 -> HP */
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg2_a_t;

#define LSM303AGR_CTRL_REG3_A                0x22U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t i1_overrun             : 1;
  uint8_t i1_wtm                 : 1;
  uint8_t i1_drdy2               : 1;
  uint8_t i1_drdy1               : 1;
  uint8_t i1_aoi2                : 1;
  uint8_t i1_aoi1                : 1;
  uint8_t i1_click               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t i1_click               : 1;
  uint8_t i1_aoi1                : 1;
  uint8_t i1_aoi2                : 1;
  uint8_t i1_drdy1               : 1;
  uint8_t i1_drdy2               : 1;
  uint8_t i1_wtm                 : 1;
  uint8_t i1_overrun             : 1;
  uint8_t not_used_01            : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg3_a_t;

#define LSM303AGR_CTRL_REG4_A                0x23U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t spi_enable             : 1;
  uint8_t st                     : 2;
  uint8_t hr                     : 1;
  uint8_t fs                     : 2;
  uint8_t ble                    : 1;
  uint8_t bdu                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bdu                    : 1;
  uint8_t ble                    : 1;
  uint8_t fs                     : 2;
  uint8_t hr                     : 1;
  uint8_t st                     : 2;
  uint8_t spi_enable             : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg4_a_t;

#define LSM303AGR_CTRL_REG5_A                0x24U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t d4d_int2               : 1;
  uint8_t lir_int2               : 1;
  uint8_t d4d_int1               : 1;
  uint8_t lir_int1               : 1;
  uint8_t not_used_01            : 2;
  uint8_t fifo_en                : 1;
  uint8_t boot                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot                   : 1;
  uint8_t fifo_en                : 1;
  uint8_t not_used_01            : 2;
  uint8_t lir_int1               : 1;
  uint8_t d4d_int1               : 1;
  uint8_t lir_int2               : 1;
  uint8_t d4d_int2               : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg5_a_t;

#define LSM303AGR_CTRL_REG6_A               0x25U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t h_lactive              : 1;
  uint8_t not_used_02            : 1;
  uint8_t p2_act                 : 1;
  uint8_t boot_i2                : 1;
  uint8_t i2_int2                : 1;
  uint8_t i2_int1                : 1;
  uint8_t i2_clicken             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t i2_clicken             : 1;
  uint8_t i2_int1                : 1;
  uint8_t i2_int2                : 1;
  uint8_t boot_i2                : 1;
  uint8_t p2_act                 : 1;
  uint8_t not_used_02            : 1;
  uint8_t h_lactive              : 1;
  uint8_t not_used_01            : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_ctrl_reg6_a_t;

#define LSM303AGR_REFERENCE_A               0x26U
#define LSM303AGR_STATUS_REG_A              0x27U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xda                    : 1;
  uint8_t yda                    : 1;
  uint8_t zda                    : 1;
  uint8_t zyxda                  : 1;
  uint8_t _xor                   : 1;
  uint8_t yor                    : 1;
  uint8_t zor                    : 1;
  uint8_t zyxor                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t zyxor                  : 1;
  uint8_t zor                    : 1;
  uint8_t yor                    : 1;
  uint8_t _xor                   : 1;
  uint8_t zyxda                  : 1;
  uint8_t zda                    : 1;
  uint8_t yda                    : 1;
  uint8_t xda                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_status_reg_a_t;

#define LSM303AGR_OUT_X_L_A                 0x28U
#define LSM303AGR_OUT_X_H_A                 0x29U
#define LSM303AGR_OUT_Y_L_A                 0x2AU
#define LSM303AGR_OUT_Y_H_A                 0x2BU
#define LSM303AGR_OUT_Z_L_A                 0x2CU
#define LSM303AGR_OUT_Z_H_A                 0x2DU
#define LSM303AGR_FIFO_CTRL_REG_A           0x2EU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fth                    : 5;
  uint8_t tr                     : 1;
  uint8_t fm                     : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fm                     : 2;
  uint8_t tr                     : 1;
  uint8_t fth                    : 5;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_fifo_ctrl_reg_a_t;

#define LSM303AGR_FIFO_SRC_REG_A            0x2FU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fss                    : 5;
  uint8_t empty                  : 1;
  uint8_t ovrn_fifo              : 1;
  uint8_t wtm                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t wtm                    : 1;
  uint8_t ovrn_fifo              : 1;
  uint8_t empty                  : 1;
  uint8_t fss                    : 5;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_fifo_src_reg_a_t;

#define LSM303AGR_INT1_CFG_A                0x30U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlie                   : 1; /* or XDOWNE */
  uint8_t xhie                   : 1; /* or XUPE */
  uint8_t ylie                   : 1; /* or YDOWNE */
  uint8_t yhie                   : 1; /* or YUPE */
  uint8_t zlie                   : 1; /* or ZDOWNE */
  uint8_t zhie                   : 1; /* or ZUPE */
  uint8_t _6d                    : 1;
  uint8_t aoi                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t aoi                    : 1;
  uint8_t _6d                    : 1;
  uint8_t zhie                   : 1; /* or ZUPE */
  uint8_t zlie                   : 1; /* or ZDOWNE */
  uint8_t yhie                   : 1; /* or YUPE */
  uint8_t ylie                   : 1; /* or YDOWNE */
  uint8_t xhie                   : 1; /* or XUPE */
  uint8_t xlie                   : 1; /* or XDOWNE */
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int1_cfg_a_t;

#define LSM303AGR_INT1_SRC_A                0x31U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl                     : 1;
  uint8_t xh                     : 1;
  uint8_t yl                     : 1;
  uint8_t yh                     : 1;
  uint8_t zl                     : 1;
  uint8_t zh                     : 1;
  uint8_t ia                     : 1;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ia                     : 1;
  uint8_t zh                     : 1;
  uint8_t zl                     : 1;
  uint8_t yh                     : 1;
  uint8_t yl                     : 1;
  uint8_t xh                     : 1;
  uint8_t xl                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int1_src_a_t;

#define LSM303AGR_INT1_THS_A                0x32U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ths                    : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ths                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int1_ths_a_t;

#define LSM303AGR_INT1_DURATION_A           0x33U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t d                      : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t d                      : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int1_duration_a_t;

#define LSM303AGR_INT2_CFG_A                0x34U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xlie                   : 1;
  uint8_t xhie                   : 1;
  uint8_t ylie                   : 1;
  uint8_t yhie                   : 1;
  uint8_t zlie                   : 1;
  uint8_t zhie                   : 1;
  uint8_t _6d                    : 1;
  uint8_t aoi                    : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t aoi                    : 1;
  uint8_t _6d                    : 1;
  uint8_t zhie                   : 1;
  uint8_t zlie                   : 1;
  uint8_t yhie                   : 1;
  uint8_t ylie                   : 1;
  uint8_t xhie                   : 1;
  uint8_t xlie                   : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int2_cfg_a_t;

#define LSM303AGR_INT2_SRC_A                0x35U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xl                     : 1;
  uint8_t xh                     : 1;
  uint8_t yl                     : 1;
  uint8_t yh                     : 1;
  uint8_t zl                     : 1;
  uint8_t zh                     : 1;
  uint8_t ia                     : 1;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ia                     : 1;
  uint8_t zh                     : 1;
  uint8_t zl                     : 1;
  uint8_t yh                     : 1;
  uint8_t yl                     : 1;
  uint8_t xh                     : 1;
  uint8_t xl                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int2_src_a_t;

#define LSM303AGR_INT2_THS_A                0x36U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ths                    : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ths                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int2_ths_a_t;

#define LSM303AGR_INT2_DURATION_A           0x37U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t d                      : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t d                      : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int2_duration_a_t;

#define LSM303AGR_CLICK_CFG_A               0x38U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xs                     : 1;
  uint8_t xd                     : 1;
  uint8_t ys                     : 1;
  uint8_t yd                     : 1;
  uint8_t zs                     : 1;
  uint8_t zd                     : 1;
  uint8_t not_used_01            : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 2;
  uint8_t zd                     : 1;
  uint8_t zs                     : 1;
  uint8_t yd                     : 1;
  uint8_t ys                     : 1;
  uint8_t xd                     : 1;
  uint8_t xs                     : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_click_cfg_a_t;

#define LSM303AGR_CLICK_SRC_A               0x39U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t x                      : 1;
  uint8_t y                      : 1;
  uint8_t z                      : 1;
  uint8_t sign                   : 1;
  uint8_t sclick                 : 1;
  uint8_t dclick                 : 1;
  uint8_t ia                     : 1;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ia                     : 1;
  uint8_t dclick                 : 1;
  uint8_t sclick                 : 1;
  uint8_t sign                   : 1;
  uint8_t z                      : 1;
  uint8_t y                      : 1;
  uint8_t x                      : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_click_src_a_t;

#define LSM303AGR_CLICK_THS_A               0x3AU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ths                    : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t ths                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_click_ths_a_t;

#define LSM303AGR_TIME_LIMIT_A              0x3BU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t tli                    : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t tli                    : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_time_limit_a_t;

#define LSM303AGR_TIME_LATENCY_A            0x3CU
typedef struct {
  uint8_t tla                    : 8;
} lsm303agr_time_latency_a_t;

#define LSM303AGR_TIME_WINDOW_A             0x3DU
typedef struct {
  uint8_t tw                     : 8;
} lsm303agr_time_window_a_t;

#define LSM303AGR_ACT_THS_A                 0x3EU
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t acth                   : 7;
  uint8_t not_used_01            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 1;
  uint8_t acth                   : 7;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_act_ths_a_t;

#define LSM303AGR_ACT_DUR_A                 0x3FU
typedef struct {
  uint8_t actd                   : 8;
} lsm303agr_act_dur_a_t;

#define LSM303AGR_OFFSET_X_REG_L_M          0x45U
#define LSM303AGR_OFFSET_X_REG_H_M          0x46U
#define LSM303AGR_OFFSET_Y_REG_L_M          0x47U
#define LSM303AGR_OFFSET_Y_REG_H_M          0x48U
#define LSM303AGR_OFFSET_Z_REG_L_M          0x49U
#define LSM303AGR_OFFSET_Z_REG_H_M          0x4AU
#define LSM303AGR_WHO_AM_I_M                0x4FU
#define LSM303AGR_CFG_REG_A_M               0x60U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t md                     : 2;
  uint8_t odr                    : 2;
  uint8_t lp                     : 1;
  uint8_t soft_rst               : 1;
  uint8_t reboot                 : 1;
  uint8_t comp_temp_en           : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t comp_temp_en           : 1;
  uint8_t reboot                 : 1;
  uint8_t soft_rst               : 1;
  uint8_t lp                     : 1;
  uint8_t odr                    : 2;
  uint8_t md                     : 2;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_cfg_reg_a_m_t;

#define LSM303AGR_CFG_REG_B_M               0x61U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lpf                    : 1;
  uint8_t set_rst                : 2; /* OFF_CANC + Set_FREQ */
  uint8_t int_on_dataoff         : 1;
  uint8_t off_canc_one_shot      : 1;
  uint8_t not_used_01            : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01            : 3;
  uint8_t off_canc_one_shot      : 1;
  uint8_t int_on_dataoff         : 1;
  uint8_t set_rst                : 2; /* OFF_CANC + Set_FREQ */
  uint8_t lpf                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_cfg_reg_b_m_t;

#define LSM303AGR_CFG_REG_C_M               0x62U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_mag                : 1;
  uint8_t self_test              : 1;
  uint8_t not_used_01            : 1;
  uint8_t ble                    : 1;
  uint8_t bdu                    : 1;
  uint8_t i2c_dis                : 1;
  uint8_t int_mag_pin            : 1;
  uint8_t not_used_02            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_02            : 1;
  uint8_t int_mag_pin            : 1;
  uint8_t i2c_dis                : 1;
  uint8_t bdu                    : 1;
  uint8_t ble                    : 1;
  uint8_t not_used_01            : 1;
  uint8_t self_test              : 1;
  uint8_t int_mag                : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_cfg_reg_c_m_t;

#define LSM303AGR_INT_CRTL_REG_M            0x63U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ien                    : 1;
  uint8_t iel                    : 1;
  uint8_t iea                    : 1;
  uint8_t not_used_01            : 2;
  uint8_t zien                   : 1;
  uint8_t yien                   : 1;
  uint8_t xien                   : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t xien                   : 1;
  uint8_t yien                   : 1;
  uint8_t zien                   : 1;
  uint8_t not_used_01            : 2;
  uint8_t iea                    : 1;
  uint8_t iel                    : 1;
  uint8_t ien                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int_crtl_reg_m_t;

#define LSM303AGR_INT_SOURCE_REG_M          0x64U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t _int                   : 1;
  uint8_t mroi                   : 1;
  uint8_t n_th_s_z               : 1;
  uint8_t n_th_s_y               : 1;
  uint8_t n_th_s_x               : 1;
  uint8_t p_th_s_z               : 1;
  uint8_t p_th_s_y               : 1;
  uint8_t p_th_s_x               : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t p_th_s_x               : 1;
  uint8_t p_th_s_y               : 1;
  uint8_t p_th_s_z               : 1;
  uint8_t n_th_s_x               : 1;
  uint8_t n_th_s_y               : 1;
  uint8_t n_th_s_z               : 1;
  uint8_t mroi                   : 1;
  uint8_t _int                   : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_int_source_reg_m_t;

#define LSM303AGR_INT_THS_L_REG_M           0x65U
#define LSM303AGR_INT_THS_H_REG_M           0x66U
#define LSM303AGR_STATUS_REG_M              0x67U
typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t xda                    : 1;
  uint8_t yda                    : 1;
  uint8_t zda                    : 1;
  uint8_t zyxda                  : 1;
  uint8_t _xor                   : 1;
  uint8_t yor                    : 1;
  uint8_t zor                    : 1;
  uint8_t zyxor                  : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t zyxor                  : 1;
  uint8_t zor                    : 1;
  uint8_t yor                    : 1;
  uint8_t _xor                   : 1;
  uint8_t zyxda                  : 1;
  uint8_t zda                    : 1;
  uint8_t yda                    : 1;
  uint8_t xda                    : 1;
#endif /* DRV_BYTE_ORDER */
} lsm303agr_status_reg_m_t;

#define LSM303AGR_OUTX_L_REG_M              0x68U
#define LSM303AGR_OUTX_H_REG_M              0x69U
#define LSM303AGR_OUTY_L_REG_M              0x6AU
#define LSM303AGR_OUTY_H_REG_M              0x6BU
#define LSM303AGR_OUTZ_L_REG_M              0x6CU
#define LSM303AGR_OUTZ_H_REG_M              0x6DU

/**
  * @defgroup LSM303AGR_Register_Union
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
  lsm303agr_status_reg_aux_a_t       status_reg_aux_a;
  lsm303agr_temp_cfg_reg_a_t         temp_cfg_reg_a;
  lsm303agr_ctrl_reg1_a_t            ctrl_reg1_a;
  lsm303agr_ctrl_reg2_a_t            ctrl_reg2_a;
  lsm303agr_ctrl_reg3_a_t            ctrl_reg3_a;
  lsm303agr_ctrl_reg4_a_t            ctrl_reg4_a;
  lsm303agr_ctrl_reg5_a_t            ctrl_reg5_a;
  lsm303agr_ctrl_reg6_a_t            ctrl_reg6_a;
  lsm303agr_status_reg_a_t           status_reg_a;
  lsm303agr_fifo_ctrl_reg_a_t        fifo_ctrl_reg_a;
  lsm303agr_fifo_src_reg_a_t         fifo_src_reg_a;
  lsm303agr_int1_cfg_a_t             int1_cfg_a;
  lsm303agr_int1_src_a_t             int1_src_a;
  lsm303agr_int1_ths_a_t             int1_ths_a;
  lsm303agr_int1_duration_a_t        int1_duration_a;
  lsm303agr_int2_cfg_a_t             int2_cfg_a;
  lsm303agr_int2_src_a_t             int2_src_a;
  lsm303agr_int2_ths_a_t             int2_ths_a;
  lsm303agr_int2_duration_a_t        int2_duration_a;
  lsm303agr_click_cfg_a_t            click_cfg_a;
  lsm303agr_click_src_a_t            click_src_a;
  lsm303agr_click_ths_a_t            click_ths_a;
  lsm303agr_time_limit_a_t           time_limit_a;
  lsm303agr_time_latency_a_t         time_latency_a;
  lsm303agr_time_window_a_t          time_window_a;
  lsm303agr_act_ths_a_t              act_ths_a;
  lsm303agr_act_dur_a_t              act_dur_a;
  lsm303agr_cfg_reg_a_m_t            cfg_reg_a_m;
  lsm303agr_cfg_reg_b_m_t            cfg_reg_b_m;
  lsm303agr_cfg_reg_c_m_t            cfg_reg_c_m;
  lsm303agr_int_crtl_reg_m_t         int_crtl_reg_m;
  lsm303agr_int_source_reg_m_t       int_source_reg_m;
  lsm303agr_status_reg_m_t           status_reg_m;
  bitwise_t                          bitwise;
  uint8_t                            byte;
} lsm303agr_reg_t;

/**
  * @}
  *
  */

int32_t lsm303agr_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                           uint8_t *data,
                           uint16_t len);
int32_t lsm303agr_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                            uint8_t *data,
                            uint16_t len);

float_t lsm303agr_from_fs_2g_hr_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_4g_hr_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_8g_hr_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_16g_hr_to_mg(int16_t lsb);
float_t lsm303agr_from_lsb_hr_to_celsius(int16_t lsb);

float_t lsm303agr_from_fs_2g_nm_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_4g_nm_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_8g_nm_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_16g_nm_to_mg(int16_t lsb);
float_t lsm303agr_from_lsb_nm_to_celsius(int16_t lsb);

float_t lsm303agr_from_fs_2g_lp_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_4g_lp_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_8g_lp_to_mg(int16_t lsb);
float_t lsm303agr_from_fs_16g_lp_to_mg(int16_t lsb);
float_t lsm303agr_from_lsb_lp_to_celsius(int16_t lsb);

float_t lsm303agr_from_lsb_to_mgauss(int16_t lsb);

int32_t lsm303agr_temp_status_reg_get(stmdev_ctx_t *ctx,
                                      uint8_t *buff);

int32_t lsm303agr_temp_data_ready_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lsm303agr_temp_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_temperature_raw_get(stmdev_ctx_t *ctx,
                                      int16_t *val);

typedef enum {
  LSM303AGR_TEMP_DISABLE  = 0,
  LSM303AGR_TEMP_ENABLE   = 3,
} lsm303agr_temp_en_a_t;
int32_t lsm303agr_temperature_meas_set(stmdev_ctx_t *ctx,
                                       lsm303agr_temp_en_a_t val);
int32_t lsm303agr_temperature_meas_get(stmdev_ctx_t *ctx,
                                       lsm303agr_temp_en_a_t *val);

typedef enum {
  LSM303AGR_HR_12bit   = 0,
  LSM303AGR_NM_10bit   = 1,
  LSM303AGR_LP_8bit    = 2,
} lsm303agr_op_md_a_t;
int32_t lsm303agr_xl_operating_mode_set(stmdev_ctx_t *ctx,
                                        lsm303agr_op_md_a_t val);
int32_t lsm303agr_xl_operating_mode_get(stmdev_ctx_t *ctx,
                                        lsm303agr_op_md_a_t *val);

typedef enum {
  LSM303AGR_XL_POWER_DOWN                      = 0,
  LSM303AGR_XL_ODR_1Hz                         = 1,
  LSM303AGR_XL_ODR_10Hz                        = 2,
  LSM303AGR_XL_ODR_25Hz                        = 3,
  LSM303AGR_XL_ODR_50Hz                        = 4,
  LSM303AGR_XL_ODR_100Hz                       = 5,
  LSM303AGR_XL_ODR_200Hz                       = 6,
  LSM303AGR_XL_ODR_400Hz                       = 7,
  LSM303AGR_XL_ODR_1kHz620_LP                  = 8,
  LSM303AGR_XL_ODR_1kHz344_NM_HP_5kHz376_LP    = 9,
} lsm303agr_odr_a_t;
int32_t lsm303agr_xl_data_rate_set(stmdev_ctx_t *ctx,
                                   lsm303agr_odr_a_t val);
int32_t lsm303agr_xl_data_rate_get(stmdev_ctx_t *ctx,
                                   lsm303agr_odr_a_t *val);

int32_t lsm303agr_xl_high_pass_on_outputs_set(stmdev_ctx_t *ctx,
                                              uint8_t val);
int32_t lsm303agr_xl_high_pass_on_outputs_get(stmdev_ctx_t *ctx,
                                              uint8_t *val);

typedef enum {
  LSM303AGR_AGGRESSIVE  = 0,
  LSM303AGR_STRONG      = 1,
  LSM303AGR_MEDIUM      = 2,
  LSM303AGR_LIGHT       = 3,
} lsm303agr_hpcf_a_t;
int32_t lsm303agr_xl_high_pass_bandwidth_set(stmdev_ctx_t *ctx,
                                             lsm303agr_hpcf_a_t val);
int32_t lsm303agr_xl_high_pass_bandwidth_get(stmdev_ctx_t *ctx,
                                             lsm303agr_hpcf_a_t *val);

typedef enum {
  LSM303AGR_NORMAL_WITH_RST  = 0,
  LSM303AGR_REFERENCE_MODE   = 1,
  LSM303AGR_NORMAL           = 2,
  LSM303AGR_AUTORST_ON_INT   = 3,
} lsm303agr_hpm_a_t;
int32_t lsm303agr_xl_high_pass_mode_set(stmdev_ctx_t *ctx,
                                        lsm303agr_hpm_a_t val);
int32_t lsm303agr_xl_high_pass_mode_get(stmdev_ctx_t *ctx,
                                        lsm303agr_hpm_a_t *val);

typedef enum {
  LSM303AGR_2g   = 0,
  LSM303AGR_4g   = 1,
  LSM303AGR_8g   = 2,
  LSM303AGR_16g  = 3,
} lsm303agr_fs_a_t;
int32_t lsm303agr_xl_full_scale_set(stmdev_ctx_t *ctx,
                                    lsm303agr_fs_a_t val);
int32_t lsm303agr_xl_full_scale_get(stmdev_ctx_t *ctx,
                                    lsm303agr_fs_a_t *val);

int32_t lsm303agr_xl_block_data_update_set(stmdev_ctx_t *ctx,
                                           uint8_t val);
int32_t lsm303agr_xl_block_data_update_get(stmdev_ctx_t *ctx,
                                           uint8_t *val);

int32_t lsm303agr_xl_filter_reference_set(stmdev_ctx_t *ctx,
                                          uint8_t *buff);
int32_t lsm303agr_xl_filter_reference_get(stmdev_ctx_t *ctx,
                                          uint8_t *buff);

int32_t lsm303agr_xl_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_xl_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_acceleration_raw_get(stmdev_ctx_t *ctx,
                                       int16_t *val);

int32_t lsm303agr_xl_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LSM303AGR_ST_DISABLE   = 0,
  LSM303AGR_ST_POSITIVE  = 1,
  LSM303AGR_ST_NEGATIVE  = 2,
} lsm303agr_st_a_t;
int32_t lsm303agr_xl_self_test_set(stmdev_ctx_t *ctx,
                                   lsm303agr_st_a_t val);
int32_t lsm303agr_xl_self_test_get(stmdev_ctx_t *ctx,
                                   lsm303agr_st_a_t *val);

typedef enum {
  LSM303AGR_XL_LSB_AT_LOW_ADD = 0,
  LSM303AGR_XL_MSB_AT_LOW_ADD = 1,
} lsm303agr_ble_a_t;
int32_t lsm303agr_xl_data_format_set(stmdev_ctx_t *ctx,
                                     lsm303agr_ble_a_t val);
int32_t lsm303agr_xl_data_format_get(stmdev_ctx_t *ctx,
                                     lsm303agr_ble_a_t *val);

int32_t lsm303agr_xl_boot_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_xl_boot_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_xl_status_get(stmdev_ctx_t *ctx,
                                lsm303agr_status_reg_a_t *val);

int32_t lsm303agr_xl_int1_gen_conf_set(stmdev_ctx_t *ctx,
                                       lsm303agr_int1_cfg_a_t *val);
int32_t lsm303agr_xl_int1_gen_conf_get(stmdev_ctx_t *ctx,
                                       lsm303agr_int1_cfg_a_t *val);

int32_t lsm303agr_xl_int1_gen_source_get(stmdev_ctx_t *ctx,
                                         lsm303agr_int1_src_a_t *val);

int32_t lsm303agr_xl_int1_gen_threshold_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm303agr_xl_int1_gen_threshold_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lsm303agr_xl_int1_gen_duration_set(stmdev_ctx_t *ctx,
                                           uint8_t val);
int32_t lsm303agr_xl_int1_gen_duration_get(stmdev_ctx_t *ctx,
                                           uint8_t *val);

int32_t lsm303agr_xl_int2_gen_conf_set(stmdev_ctx_t *ctx,
                                       lsm303agr_int2_cfg_a_t *val);
int32_t lsm303agr_xl_int2_gen_conf_get(stmdev_ctx_t *ctx,
                                       lsm303agr_int2_cfg_a_t *val);

int32_t lsm303agr_xl_int2_gen_source_get(stmdev_ctx_t *ctx,
                                         lsm303agr_int2_src_a_t *val);

int32_t lsm303agr_xl_int2_gen_threshold_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm303agr_xl_int2_gen_threshold_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lsm303agr_xl_int2_gen_duration_set(stmdev_ctx_t *ctx,
                                           uint8_t val);
int32_t lsm303agr_xl_int2_gen_duration_get(stmdev_ctx_t *ctx,
                                           uint8_t *val);

typedef enum {
  LSM303AGR_DISC_FROM_INT_GENERATOR  = 0,
  LSM303AGR_ON_INT1_GEN              = 1,
  LSM303AGR_ON_INT2_GEN              = 2,
  LSM303AGR_ON_TAP_GEN               = 4,
  LSM303AGR_ON_INT1_INT2_GEN         = 3,
  LSM303AGR_ON_INT1_TAP_GEN          = 5,
  LSM303AGR_ON_INT2_TAP_GEN          = 6,
  LSM303AGR_ON_INT1_INT2_TAP_GEN     = 7,
} lsm303agr_hp_a_t;
int32_t lsm303agr_xl_high_pass_int_conf_set(stmdev_ctx_t *ctx,
                                            lsm303agr_hp_a_t val);
int32_t lsm303agr_xl_high_pass_int_conf_get(stmdev_ctx_t *ctx,
                                            lsm303agr_hp_a_t *val);

int32_t lsm303agr_xl_pin_int1_config_set(stmdev_ctx_t *ctx,
                                         lsm303agr_ctrl_reg3_a_t *val);
int32_t lsm303agr_xl_pin_int1_config_get(stmdev_ctx_t *ctx,
                                         lsm303agr_ctrl_reg3_a_t *val);

int32_t lsm303agr_xl_int2_pin_detect_4d_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm303agr_xl_int2_pin_detect_4d_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

typedef enum {
  LSM303AGR_INT2_PULSED   = 0,
  LSM303AGR_INT2_LATCHED  = 1,
} lsm303agr_lir_int2_a_t;
int32_t lsm303agr_xl_int2pin_notification_mode_set(stmdev_ctx_t *ctx,
                                                   lsm303agr_lir_int2_a_t val);
int32_t lsm303agr_xl_int2pin_notification_mode_get(stmdev_ctx_t *ctx,
                                                   lsm303agr_lir_int2_a_t *val);

int32_t lsm303agr_xl_int1_pin_detect_4d_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm303agr_xl_int1_pin_detect_4d_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

typedef enum {
  LSM303AGR_INT1_PULSED   = 0,
  LSM303AGR_INT1_LATCHED  = 1,
} lsm303agr_lir_int1_a_t;
int32_t lsm303agr_xl_int1pin_notification_mode_set(stmdev_ctx_t *ctx,
                                                   lsm303agr_lir_int1_a_t val);
int32_t lsm303agr_xl_int1pin_notification_mode_get(stmdev_ctx_t *ctx,
                                                   lsm303agr_lir_int1_a_t *val);

int32_t lsm303agr_xl_pin_int2_config_set(stmdev_ctx_t *ctx,
                                         lsm303agr_ctrl_reg6_a_t *val);
int32_t lsm303agr_xl_pin_int2_config_get(stmdev_ctx_t *ctx,
                                         lsm303agr_ctrl_reg6_a_t *val);

int32_t lsm303agr_xl_fifo_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_xl_fifo_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_xl_fifo_watermark_set(stmdev_ctx_t *ctx,
                                        uint8_t val);
int32_t lsm303agr_xl_fifo_watermark_get(stmdev_ctx_t *ctx,
                                        uint8_t *val);

typedef enum {
  LSM303AGR_INT1_GEN = 0,
  LSM303AGR_INT2_GEN = 1,
} lsm303agr_tr_a_t;
int32_t lsm303agr_xl_fifo_trigger_event_set(stmdev_ctx_t *ctx,
                                            lsm303agr_tr_a_t val);
int32_t lsm303agr_xl_fifo_trigger_event_get(stmdev_ctx_t *ctx,
                                            lsm303agr_tr_a_t *val);

typedef enum {
  LSM303AGR_BYPASS_MODE           = 0,
  LSM303AGR_FIFO_MODE             = 1,
  LSM303AGR_DYNAMIC_STREAM_MODE   = 2,
  LSM303AGR_STREAM_TO_FIFO_MODE   = 3,
} lsm303agr_fm_a_t;
int32_t lsm303agr_xl_fifo_mode_set(stmdev_ctx_t *ctx,
                                   lsm303agr_fm_a_t val);
int32_t lsm303agr_xl_fifo_mode_get(stmdev_ctx_t *ctx,
                                   lsm303agr_fm_a_t *val);

int32_t lsm303agr_xl_fifo_status_get(stmdev_ctx_t *ctx,
                                     lsm303agr_fifo_src_reg_a_t *val);

int32_t lsm303agr_xl_fifo_data_level_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

int32_t lsm303agr_xl_fifo_empty_flag_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

int32_t lsm303agr_xl_fifo_ovr_flag_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm303agr_xl_fifo_fth_flag_get(stmdev_ctx_t *ctx,
                                       uint8_t *val);

int32_t lsm303agr_tap_conf_set(stmdev_ctx_t *ctx,
                               lsm303agr_click_cfg_a_t *val);
int32_t lsm303agr_tap_conf_get(stmdev_ctx_t *ctx,
                               lsm303agr_click_cfg_a_t *val);

int32_t lsm303agr_tap_source_get(stmdev_ctx_t *ctx,
                                 lsm303agr_click_src_a_t *val);

int32_t lsm303agr_tap_threshold_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_tap_threshold_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_shock_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_shock_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_quiet_dur_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_quiet_dur_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_double_tap_timeout_set(stmdev_ctx_t *ctx,
                                         uint8_t val);
int32_t lsm303agr_double_tap_timeout_get(stmdev_ctx_t *ctx,
                                         uint8_t *val);

int32_t lsm303agr_act_threshold_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_act_threshold_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_act_timeout_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_act_timeout_get(stmdev_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM303AGR_SPI_4_WIRE = 0,
  LSM303AGR_SPI_3_WIRE = 1,
} lsm303agr_sim_a_t;
int32_t lsm303agr_xl_spi_mode_set(stmdev_ctx_t *ctx,
                                  lsm303agr_sim_a_t val);
int32_t lsm303agr_xl_spi_mode_get(stmdev_ctx_t *ctx,
                                  lsm303agr_sim_a_t *val);

int32_t lsm303agr_mag_user_offset_set(stmdev_ctx_t *ctx,
                                      int16_t *val);
int32_t lsm303agr_mag_user_offset_get(stmdev_ctx_t *ctx,
                                      int16_t *val);

typedef enum {
  LSM303AGR_CONTINUOUS_MODE  = 0,
  LSM303AGR_SINGLE_TRIGGER   = 1,
  LSM303AGR_POWER_DOWN       = 2,
} lsm303agr_md_m_t;
int32_t lsm303agr_mag_operating_mode_set(stmdev_ctx_t *ctx,
                                         lsm303agr_md_m_t val);
int32_t lsm303agr_mag_operating_mode_get(stmdev_ctx_t *ctx,
                                         lsm303agr_md_m_t *val);

typedef enum {
  LSM303AGR_MG_ODR_10Hz   = 0,
  LSM303AGR_MG_ODR_20Hz   = 1,
  LSM303AGR_MG_ODR_50Hz   = 2,
  LSM303AGR_MG_ODR_100Hz  = 3,
} lsm303agr_mg_odr_m_t;
int32_t lsm303agr_mag_data_rate_set(stmdev_ctx_t *ctx,
                                    lsm303agr_mg_odr_m_t val);
int32_t lsm303agr_mag_data_rate_get(stmdev_ctx_t *ctx,
                                    lsm303agr_mg_odr_m_t *val);

typedef enum {
  LSM303AGR_HIGH_RESOLUTION  = 0,
  LSM303AGR_LOW_POWER        = 1,
} lsm303agr_lp_m_t;
int32_t lsm303agr_mag_power_mode_set(stmdev_ctx_t *ctx,
                                     lsm303agr_lp_m_t val);
int32_t lsm303agr_mag_power_mode_get(stmdev_ctx_t *ctx,
                                     lsm303agr_lp_m_t *val);

int32_t lsm303agr_mag_offset_temp_comp_set(stmdev_ctx_t *ctx,
                                           uint8_t val);
int32_t lsm303agr_mag_offset_temp_comp_get(stmdev_ctx_t *ctx,
                                           uint8_t *val);

typedef enum {
  LSM303AGR_ODR_DIV_2  = 0,
  LSM303AGR_ODR_DIV_4  = 1,
} lsm303agr_lpf_m_t;
int32_t lsm303agr_mag_low_pass_bandwidth_set(stmdev_ctx_t *ctx,
                                             lsm303agr_lpf_m_t val);
int32_t lsm303agr_mag_low_pass_bandwidth_get(stmdev_ctx_t *ctx,
                                             lsm303agr_lpf_m_t *val);

typedef enum {
  LSM303AGR_SET_SENS_ODR_DIV_63        = 0,
  LSM303AGR_SENS_OFF_CANC_EVERY_ODR    = 1,
  LSM303AGR_SET_SENS_ONLY_AT_POWER_ON  = 2,
} lsm303agr_set_rst_m_t;
int32_t lsm303agr_mag_set_rst_mode_set(stmdev_ctx_t *ctx,
                                       lsm303agr_set_rst_m_t val);
int32_t lsm303agr_mag_set_rst_mode_get(stmdev_ctx_t *ctx,
                                       lsm303agr_set_rst_m_t *val);

int32_t lsm303agr_mag_set_rst_sensor_single_set(stmdev_ctx_t *ctx,
                                                uint8_t val);
int32_t lsm303agr_mag_set_rst_sensor_single_get(stmdev_ctx_t *ctx,
                                                uint8_t *val);

int32_t lsm303agr_mag_block_data_update_set(stmdev_ctx_t *ctx,
                                            uint8_t val);
int32_t lsm303agr_mag_block_data_update_get(stmdev_ctx_t *ctx,
                                            uint8_t *val);

int32_t lsm303agr_mag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_mag_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_magnetic_raw_get(stmdev_ctx_t *ctx, int16_t *val);

int32_t lsm303agr_mag_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

int32_t lsm303agr_mag_reset_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_mag_reset_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_mag_boot_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_mag_boot_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_mag_self_test_set(stmdev_ctx_t *ctx,
                                    uint8_t val);
int32_t lsm303agr_mag_self_test_get(stmdev_ctx_t *ctx,
                                    uint8_t *val);

typedef enum {
  LSM303AGR_MG_LSB_AT_LOW_ADD  = 0,
  LSM303AGR_MG_MSB_AT_LOW_ADD  = 1,
} lsm303agr_ble_m_t;
int32_t lsm303agr_mag_data_format_set(stmdev_ctx_t *ctx,
                                      lsm303agr_ble_m_t val);
int32_t lsm303agr_mag_data_format_get(stmdev_ctx_t *ctx,
                                      lsm303agr_ble_m_t *val);

int32_t lsm303agr_mag_status_get(stmdev_ctx_t *ctx,
                                 lsm303agr_status_reg_m_t *val);

typedef enum {
  LSM303AGR_CHECK_BEFORE  = 0,
  LSM303AGR_CHECK_AFTER   = 1,
} lsm303agr_int_on_dataoff_m_t;
int32_t lsm303agr_mag_offset_int_conf_set(stmdev_ctx_t *ctx,
                                          lsm303agr_int_on_dataoff_m_t val);
int32_t lsm303agr_mag_offset_int_conf_get(stmdev_ctx_t *ctx,
                                          lsm303agr_int_on_dataoff_m_t *val);

int32_t lsm303agr_mag_drdy_on_pin_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_mag_drdy_on_pin_get(stmdev_ctx_t *ctx,
                                      uint8_t *val);

int32_t lsm303agr_mag_int_on_pin_set(stmdev_ctx_t *ctx, uint8_t val);
int32_t lsm303agr_mag_int_on_pin_get(stmdev_ctx_t *ctx, uint8_t *val);

int32_t lsm303agr_mag_int_gen_conf_set(stmdev_ctx_t *ctx,
                                       lsm303agr_int_crtl_reg_m_t *val);
int32_t lsm303agr_mag_int_gen_conf_get(stmdev_ctx_t *ctx,
                                       lsm303agr_int_crtl_reg_m_t *val);

int32_t lsm303agr_mag_int_gen_source_get(stmdev_ctx_t *ctx,
                                         lsm303agr_int_source_reg_m_t *val);

int32_t lsm303agr_mag_int_gen_treshold_set(stmdev_ctx_t *ctx,
                                           int16_t val);
int32_t lsm303agr_mag_int_gen_treshold_get(stmdev_ctx_t *ctx,
                                           int16_t *val);

typedef enum {
  LSM303AGR_I2C_ENABLE   = 0,
  LSM303AGR_I2C_DISABLE  = 1,
} lsm303agr_i2c_dis_m_t;
int32_t lsm303agr_mag_i2c_interface_set(stmdev_ctx_t *ctx,
                                        lsm303agr_i2c_dis_m_t val);
int32_t lsm303agr_mag_i2c_interface_get(stmdev_ctx_t *ctx,
                                        lsm303agr_i2c_dis_m_t *val);

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LSM303AGR_REGS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
