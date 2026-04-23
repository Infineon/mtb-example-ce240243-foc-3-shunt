/******************************************************************************
* File Name:   MCU.h
*
 Description: This file implements the PSOC Control C3 peripheral configuration
* header file used for 3 shunt foc algorithm.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#pragma once
#include "cybsp.h"
#include "cy_em_eeprom.h"
#include "MotorCtrlHWConfig.h"
#include "Controller.h"

#define MOTOR_CTRL_ID_MOTOR0 (0U)
#define MOTOR_CTRL_ID_MOTOR1 (1U)

/* EEPROM storage Emulated EEPROM flash. */
extern const uint8_t *Em_Eeprom_Storage[MOTOR_CTRL_NO_OF_MOTOR];

typedef struct
{
    float i_uvw;    // [A/ticks]
    float v_uvw;    // [V/ticks]
    float v_dc;     // [V/ticks]
    float v_pot;    // [%/ticks]
    float temp_ps;  // [Celsius/ticks] for active IC, [1/ticks] for passive NTC
} MCU_ADC_SCALE_t;

typedef struct
{
    uint32_t count;         // [ticks]
    uint32_t period;        // [ticks]
    float duty_cycle_coeff; // [ticks/%]
    uint32_t deadtime;      // [ticks]
} MCU_TIMER_t;

typedef struct
{    // For time multiplexing ADC measurements and analog routing
    bool en;        // only enabled in high-z state to save computation time when switching
    uint8_t seq;    // mux sequence
    uint8_t idx_isamp[3];    // indices for sampled currents inside dma-results array
} MCU_ADC_MUX_t;

typedef struct
{
    // Must use int32_t so overflow would be OK (using 32bit TCPWM)
    int32_t start;              // [ticks]
    int32_t stop;               // [ticks]
    int32_t duration_ticks;     // [ticks]

    float duration_sec;         // [sec]
    float util;                 // [%], utilization percentage

    float sec_per_tick;         // [sec/tick], based on timer's frequency
    float inv_max_time;         // [1/sec], inverse of max. time corresponding to 100% utilization
} MCU_TIME_CAP_t;

typedef struct
{
  TIMER_t timer;
  volatile float cpu_load;      // [pu] CPU load (0.0 = 0%, 1.0 = 100%), volatile: written by main loop, read by other contexts
}MCU_CPU_CALC_t;
typedef struct
{
    bool init_done;

    cy_stc_eeprom_config_t config;
    cy_en_em_eeprom_status_t status;
    cy_stc_eeprom_context_t context;
} MCU_EEPROM_t;

typedef struct
{
    IRQn_Type nvic_adc0_isr0;
    IRQn_Type nvic_dma_adc_1;
    IRQn_Type nvic_dma_adc_2;
    IRQn_Type nvic_sync_isr1;
    uint32_t state;
} MCU_INT_t;  // interrupts

typedef struct
{
    float tcpwm;
    float hall;
    float encoder;
} MCU_CLK_FRQ_t;  // [Hz]

typedef struct
{
    MCU_CLK_FRQ_t clk;
    MCU_ADC_SCALE_t adc_scale;
    MCU_TIMER_t pwm;
    MCU_TIMER_t isr0;
    MCU_TIMER_t isr1;
    MCU_EEPROM_t eeprom;
    MCU_INT_t interrupt;
    MCU_TIME_CAP_t isr0_exe;
    MCU_TIME_CAP_t isr1_exe;
    MCU_ADC_MUX_t adc_mux;
    MCU_CPU_CALC_t cpu_calc;
} MCU_t;

/** @brief Global MCU context for all motor instances. */
extern MCU_t mcu[MOTOR_CTRL_NO_OF_MOTOR];

/**
 * @brief Initialize MCU hardware and runtime state for a motor instance.
 * @param motor_id Motor instance index.
 */
void MCU_Init(uint8_t motor_id);

/** @brief Enter critical section. */
void MCU_EnterCriticalSection(void);

/** @brief Exit critical section. */
void MCU_ExitCriticalSection(void);

/**
 * @brief Put gate driver outputs into high-impedance state.
 * @param motor_id Motor instance index.
 */
void MCU_GateDriverEnterHighZ(uint8_t motor_id);

/**
 * @brief Exit high-impedance state and restore gate driver outputs.
 * @param motor_id Motor instance index.
 */
void MCU_GateDriverExitHighZ(uint8_t motor_id);

/**
 * @brief Read parameter block from non-volatile storage.
 * @param motor_id Motor instance index.
 * @param id Expected parameter identifier metadata.
 * @param ram_data Destination buffer in RAM.
 * @return true if read succeeds and identifier matches.
 */
bool MCU_FlashRead(uint8_t motor_id, PARAMS_ID_t id, PARAMS_t* ram_data);

/**
 * @brief Write parameter block to non-volatile storage.
 * @param motor_id Motor instance index.
 * @param ram_data Source parameter buffer in RAM.
 * @return true if write succeeds.
 */
bool MCU_FlashWrite(uint8_t motor_id, PARAMS_t* ram_data);

/**
 * @brief Start PWM/ADC/DMA/timer peripherals and related interrupts.
 * @param motor_id Motor instance index.
 */
void MCU_StartPeripherals(uint8_t motor_id);

/**
 * @brief Stop PWM/ADC/DMA/timer peripherals and related interrupts.
 * @param motor_id Motor instance index.
 */
void MCU_StopPeripherals(uint8_t motor_id);

// Indicating whether phase voltages are currently being sampled
bool MCU_ArePhaseVoltagesMeasured(uint8_t motor_id);

// Time-ciritcal control interrupts
void MCU_RunISR0(void); // Fast, highest priority
void MCU_RunISR1(void); // Slow, second highest priority

#if defined (EXE_TIMER_ENABLED)
void MCU_CPULoadCalc(void);
// Handling execution time capture measurements
void MCU_StartTimeCap(MCU_TIME_CAP_t *time_cap);       
void MCU_StopTimeCap(MCU_TIME_CAP_t *time_cap);        
void MCU_ProcessTimeCapISR1(MCU_TIME_CAP_t *time_cap); 
#endif
