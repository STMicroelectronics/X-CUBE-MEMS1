/**
 *******************************************************************************
 * @file    sensor_unicleo_id.h
 * @author  MEMS Software Solutions Team
 * @brief   List of sensor IDs for Unicleo-GUI
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSOR_UNICLEO_ID_H
#define SENSOR_UNICLEO_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported defines ----------------------------------------------------------*/
#define UNKNOWN_UNICLEO_ID          0xFF

#define LSM6DS0_UNICLEO_ID          1
#define LSM6DS3_UNICLEO_ID          2
#define LSM6DSL_UNICLEO_ID          3
#define LIS2DH12_UNICLEO_ID         4
#define H3LIS331DL_UNICLEO_ID       5
#define LSM303AGR_UNICLEO_ID_ACC    6
#define LSM6DSO_UNICLEO_ID          7
#define LIS2DW12_UNICLEO_ID         8
#define LSM6DSR_UNICLEO_ID          9
#define LSM6DSOX_UNICLEO_ID         10
#define ISM330DHCX_UNICLEO_ID       11
#define IIS2DLPC_UNICLEO_ID         12
#define ISM303DAC_UNICLEO_ID_ACC    13
#define ASM330LHH_UNICLEO_ID        14
#define ISM330DLC_UNICLEO_ID        15
#define AIS2DW12_UNICLEO_ID         16
#define A3G4250D_UNICLEO_ID         17
#define AIS328DQ_UNICLEO_ID         18
#define AIS3624DQ_UNICLEO_ID        19
#define LSM6DSRX_UNICLEO_ID         20
#define LSM6DSO32_UNICLEO_ID        21
#define IIS2ICLX_UNICLEO_ID         22
#define LSM6DSM_UNICLEO_ID          23
#define IIS3DWB_UNICLEO_ID          24
#define IIS2DH_UNICLEO_ID           25
#define AIS2IH_UNICLEO_ID           26
#define LSM6DSO32X_UNICLEO_ID       27
#define LIS2DTW12_UNICLEO_ID_ACC    28

#define LIS3MDL_UNICLEO_ID          1
#define LSM303AGR_UNICLEO_ID_MAG    2
#define LIS2MDL_UNICLEO_ID          3
#define IIS2MDC_UNICLEO_ID          4
#define ISM303DAC_UNICLEO_ID_MAG    5

#define LPS25HB_UNICLEO_ID_ONBOARD  1
#define LPS25HB_UNICLEO_ID_DIL      2
#define LPS22HB_UNICLEO_ID          3
#define LPS22HH_UNICLEO_ID          4
#define LPS33HW_UNICLEO_ID          5
#define LPS33K_UNICLEO_ID           6
#define LPS22CH_UNICLEO_ID          7
#define LPS27HHTW_UNICLEO_ID        8
#define LPS22DF_UNICLEO_ID          9

#define HTS221_UNICLEO_ID           1
#define STTS751_UNICLEO_ID          2
#define STTS22H_UNICLEO_ID          3
#define LIS2DTW12_UNICLEO_ID_TEMP   4

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_UNICLEO_ID_H */
