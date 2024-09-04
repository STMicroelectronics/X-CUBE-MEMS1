/**
  ******************************************************************************
  * @file    sht40ad1b_reg.h
  * @author  Sensors Software Solution Team
  * @brief   This file contains all the functions prototypes for the
  *          sht40ad1b_reg.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SHT40AD1B_REGS_H
#define SHT40AD1B_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup SHT40AD1B
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

#define PROPERTY_DISABLE  (0U)
#define PROPERTY_ENABLE   (1U)

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

/**
  * @}
  *
  */

/** @defgroup SHT40AD1B_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  **/
#define SHT40AD1B_I2C_ADDRESS  0x89U

/** Device Identification (Who am I) **/
/* Note: No 'Who am I' value available for this component */
#define SHT40AD1B_ID           0x00U

/**
  * @}
  *
  */

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
int32_t sht40ad1b_read_reg(stmdev_ctx_t *ctx, uint8_t reg,
                           uint8_t *data, uint16_t len);
int32_t sht40ad1b_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                            uint8_t *data, uint16_t len);

int32_t sht40ad1b_data_get(stmdev_ctx_t *ctx, float *buffer);
int32_t sht40ad1b_serial_get(stmdev_ctx_t *ctx, uint32_t *buffer);
int32_t sht40ad1b_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff);

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*SHT40AD1B_REGS_H */
