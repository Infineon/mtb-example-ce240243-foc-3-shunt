/******************************************************************************
* File Name:   MotorCtrlHWConfig.h
*
* Description: Motor control hardware configuration header file.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2024, Cypress Semiconductor Corporation (an Infineon company) or
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


/* PWM configurations*/
#define PWM_TRIG_ADVANCE    0U        // [ticks]
/* Miscellaneous BSP definitions */
#define KIT_ID                (0x0009UL)    // For GUI's recognition of HW
/* ADC configurations*/
#define ADC_VREF_GAIN        ((5.0f)/(3.3f))         // [V/V], voltage-reference buffer gain (e.g. scaling 5.0V down to 3.3V)
#define ADC_CS_OPAMP_GAIN   (12.0f)                 // [V/V]
#define ADC_CS_SETTLE_RATIO    (0.8f)                    // [], settling ratio used for single-shunt current sampling
#define ADC_SCALE_VUVW      ((5.6f)/(56.0f+5.6f))   // [V/V] = [Ohm/Ohm]
#define ADC_SCALE_VDC       ((5.6f)/(56.0f+5.6f))   // [V/V] = [Ohm/Ohm]

#define ADC_CS_SHUNT_RES    (10.0E-3f)                // [Ohm], cs shunt-resistor value, default

