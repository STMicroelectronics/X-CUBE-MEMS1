/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_gas_index_algorithm.h"
#include <stdio.h>  // printf

#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "sgp41_i2c.h"
#include "sht4x_i2c.h"

uint16_t DEFAULT_COMPENSATION_RH = 0x8000;  // in ticks as defined by SGP41
uint16_t DEFAULT_COMPENSATION_T = 0x6666;   // in ticks as defined by SGP41

/*
 * Get the humidity and temperature values to use for compensating SGP41
 * measurement values. The returned humidity and temperature is in ticks
 * as defined by SGP41 interface.
 *
 * @param compensation_rh: out variable for humidity
 * @param compensation_t: out variable for temperature
 */
void read_compensation_values(uint16_t* compensation_rh,
                              uint16_t* compensation_t) {
    int16_t error = 0;
    float s_rh = 0;
    float s_temperature = 0;
    error = sht4x_measure_high_precision(&s_temperature, &s_rh);
    if (error) {
        printf("Error executing sht4x_measure_high_precision(): %i\n", error);
        printf("Fallback to default RH and T compensation values.\n");
        *compensation_rh = DEFAULT_COMPENSATION_RH;
        *compensation_t = DEFAULT_COMPENSATION_T;
    } else {
        printf("T: %.2f\tRH: %.2f\n", s_temperature, s_rh);

        // convert temperature and humidity to ticks as defined by SGP41
        // interface NOTE: in case you read RH and T raw signals check out the
        // ticks specification in the datasheet, as they can be different for
        // different sensors
        *compensation_rh = (uint16_t)s_rh * 65535 / 100;
        *compensation_t = (uint16_t)(s_temperature + 45) * 65535 / 175;
    }
}

int main(void) {
    int16_t error = 0;
    uint16_t nox_conditioning_s = 10;
    uint16_t compensation_rh = DEFAULT_COMPENSATION_RH;
    uint16_t compensation_t = DEFAULT_COMPENSATION_T;
    uint16_t sraw_voc = 0;
    uint16_t sraw_nox = 0;
    int32_t voc_index_value = 0;
    int32_t nox_index_value = 0;

    // initialize gas index parameters
    GasIndexAlgorithmParams voc_params;
    GasIndexAlgorithm_init(&voc_params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
    GasIndexAlgorithmParams nox_params;
    GasIndexAlgorithm_init(&nox_params, GasIndexAlgorithm_ALGORITHM_TYPE_NOX);

    // initialize i2c communication used for SHT4x and SGP41
    sensirion_i2c_hal_init();

    for (int i = 0; i < 100; i += 1) {

        // 1. Sleep: Measure every second (1Hz), as defined by the Gas Index
        // Algorithm prerequisite
        sensirion_i2c_hal_sleep_usec(1000000);

        // 2. Measure SHT4x  RH and T signals and convert to SGP41 ticks
        read_compensation_values(&compensation_rh, &compensation_t);

        // 3. Measure SGP4x signals
        if (nox_conditioning_s > 0) {
            // During NOx conditioning (10s) SRAW NOx will remain 0
            error = sgp41_execute_conditioning(compensation_rh, compensation_t,
                                               &sraw_voc);
            nox_conditioning_s--;
            printf("NOx conditioning remaining time: %i s\n",
                   nox_conditioning_s);
        } else {
            error = sgp41_measure_raw_signals(compensation_rh, compensation_t,
                                              &sraw_voc, &sraw_nox);
        }

        // 4. Process raw signals by Gas Index Algorithm to get the VOC and NOx
        // index values
        if (error) {
            printf("Error executing sgp41_measure_raw_signals(): %i\n", error);
        } else {
            GasIndexAlgorithm_process(&voc_params, sraw_voc, &voc_index_value);
            GasIndexAlgorithm_process(&nox_params, sraw_nox, &nox_index_value);
            printf("VOC Raw: %i\tVOC Index: %i\n", sraw_voc, voc_index_value);
            printf("NOx Raw: %i\tNOx Index: %i\n", sraw_nox, nox_index_value);
        }
    }

    return 0;
}
