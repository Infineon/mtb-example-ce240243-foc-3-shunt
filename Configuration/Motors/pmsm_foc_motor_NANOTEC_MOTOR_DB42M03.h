/******************************************************************************
* File Name:   pmsm_foc_motor_NANOTEC_MOTOR_DB42M03.h
*
* Description: This file include NANOTECH MOTOR (DB42M03) configurations.
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

#ifndef PMSM_FOC_MOTOR_NANOTEC_MOTOR_DB42M03_H_
#define PMSM_FOC_MOTOR_NANOTEC_MOTOR_DB42M03_H_


#define VOLTAGE      (24.0f)       // Rated Voltage
#define N_SPEED      (4000.0f)     // nominal speed [RPM]
#define MAX_SPEED    (6000.0f)     // maximum no load speed [RPM]
#define POLE         (8.0f)       // pole pairs
#define LD           (670.0E-6f)  // d-axis inductance [H]
#define LQ           (670.0E-6f)  // q-axis inductance [H]
#define I_AM         (6.0E-3f)    // permanent magnet's flux linkage [Wb]
#define R            (450.0E-3f)  // stator resistance [Ohm]
#define T_MAX        (0.390f)     // maximum torque [Nm]
#define I_PEAK       (10.80f)     // peak current rating [A]
#define I_CONST      (3.50f)      // continuous current rating [A]
#define ID_MAX       (1.75f)      // maximum demagnetization current [A]
#define MTPV_MARGIN  (0.90f)      // [%]
#define THERM_TAU    (2.0f)       // [sec]
#define ON_LEVEL     (1.00f)      // [%]
#define OFF_LEVEL    (0.95f)      // [%]

/* Parameter related to motor as well as mechanical load */
#define MECH_INERTIA    (1.1E-5f)    // [kg.m^2]
#define MECH_VISCOUS    (1.2E-5f)    // [kg.m^2/sec]
#define MECH_FRICTION   (6.0E-3f)    // [kg.m^2/sec^2]

#define VOLT_VMIN       (0.15f)    // [Vpk]
#define VOLT_VF_RATIO   (8.5E-3f)    // [Vpk/(Ra/sec-elec)]

#endif /* PMSM_FOC_MOTOR_NANOTEC_MOTOR_DB42M03_H_ */
