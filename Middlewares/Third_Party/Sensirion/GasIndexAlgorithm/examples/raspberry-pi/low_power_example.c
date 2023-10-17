/*
 * Copyright (c) 2022, Sensirion AG
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
#include "sgp40_i2c.h"
#include "sht4x_i2c.h"

uint16_t DEFAULT_COMPENSATION_RH = 0x8000;  // in ticks as defined by SGP40
uint16_t DEFAULT_COMPENSATION_T = 0x6666;   // in ticks as defined by SGP40


void read_compensation_values(uint16_t* compensation_rh,
                              uint16_t* compensation_t);

int main(void) {
    int16_t error = 0;
    uint16_t compensation_rh = DEFAULT_COMPENSATION_RH;
    uint16_t compensation_t = DEFAULT_COMPENSATION_T;
    uint16_t sraw_voc = 0;
    int32_t voc_index_value = 0;
    // Sampling interval in seconds
    // This code uses a fixed heating pulse of ca. 200 ms for the measurement and thus,
    // the sampling interval defines the duty cycle
    float sampling_interval = 1.f;

    // initialize gas index parameters
    GasIndexAlgorithmParams voc_params;
    GasIndexAlgorithm_init_with_sampling_interval(&voc_params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC, sampling_interval);

    // initialize i2c communication used for SHT4x and SGP40
    sensirion_i2c_hal_init();

    for (int i = 0; i < 100; i += 1) {

        // 1. Sleep: Total loop time should match the chosen sampling interval
        // Therefore we take into account the heater delay and the measurement delays.
        sensirion_i2c_hal_sleep_usec(((uint32_t)(sampling_interval*1000)-240)*1000);

        // 2. Measure SHT4x  RH and T signals and convert to SGP40 ticks
        read_compensation_values(&compensation_rh, &compensation_t);

        // 3. Measure SGP40 signal with low power mode
        // 3.1 Request a first measurement to turn the heater on
        error = sgp40_measure_raw_signal(compensation_rh, compensation_t,
                                          &sraw_voc);
        if (error) {
            printf("Error executing sgp40_measure_raw_signals(): %i\n", error);
            continue;
        }

        // 3.2 Wait for the heater to reach temperature
        sensirion_i2c_hal_sleep_usec(170000);

        // 3.3 Request the actual measurement
        error = sgp40_measure_raw_signal(compensation_rh, compensation_t,
                                          &sraw_voc);

        if (error) {
            printf("Error executing sgp40_measure_raw_signals(): %i\n", error);
            continue;
        }

        // 3.4 Turn the heater off
        error = sgp40_turn_heater_off();

        if (error) {
            printf("Error executing sgp40_turn_heater_off(): %i\n", error);
            continue;
        }

        // 4. Process raw signals by Gas Index Algorithm to get the VOC and NOx
        // index values
        GasIndexAlgorithm_process(&voc_params, sraw_voc, &voc_index_value);
        printf("VOC Raw: %i\tVOC Index: %i\n", sraw_voc, voc_index_value);

    }

    return 0;
}

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
    float s_rh = 0;  // %RH
    float s_temperature = 0; // degC
    error = sht4x_measure_high_precision(&s_temperature, &s_rh);
    if (error) {
        printf("Error executing sht4x_measure_high_precision(): %i\n", error);
        printf("Fallback to default RH and T compensation values.\n");
        *compensation_rh = DEFAULT_COMPENSATION_RH;
        *compensation_t = DEFAULT_COMPENSATION_T;
    } else {
        printf("T: %.2f\tRH: %.2f\n", s_temperature, s_rh);

        // convert temperature and humidity to ticks as defined by SGP40
        // interface 
        // NOTE: in case you read RH and T raw signals check out the
        // ticks specification in the datasheet, as they can be different for
        // different sensors
        *compensation_rh = (uint16_t)s_rh * 65535 / 100;
        *compensation_t = (uint16_t)(s_temperature + 45) * 65535 / 175;
    }
}
