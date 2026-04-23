/*******************************************************************************
* Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
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

#include "MotorCtrlHWConfig.h"
#include "Controller.h"


/*Parameter Configuration for Motor 0*/
#define PARAMS_ALWAYS_OVERWRITE     (false)     // For testing only. Using this will ensure that parameters are always overwritten.
/*******************************************************************************/
/******************Parameter Configuration for Motor 0**************************/
/*******************************************************************************/
/*Power Board configuration*/
#define ADC_VREF_GAIN                            ((5.0f)/(3.3f))               /*[V/V], voltage-reference buffer gain (e.g. scaling 5.0V down to 3.3V)*/

#define ADC_CS_CURRENT_MEASUREMENT_TYPE          (Shunt_Res)                   /*Shunt_resistance "Shunt_res" =0 or active sensor "Active_Sensor" =1*/
#define ADC_CS_CURRENT_SENSE_POLARITY            (LS_Current_Sense)            /*Low side current sense "LS_Current_Sense" =0 or High side current Sense "HS_Current_sense"=1*/

#define ADC_CS_SHUNT_TYPE                        (Three_Shunt)                 /*Three_Shunt =0, Single_Shunt =1, Two_Shunt =2*/

#define ADC_CS_SHUNT_RES                         (10.0E-3f)                    /*[Ohm], Applicable for Current shunt type of measurement*/

#define ADC_CS_CURRENT_SENSITIVITY               (10.0E-3f)                    /*[V/A], Applicable for active current sense type of measurement*/

#define ADC_CS_OPAMP_GAIN                        (12.0f)                       /*[V/V], external amplifier gain for current measurement*/

#define ADC_CS_SETTLE_RATIO                      (0.8f)                        /*[], settling ratio used for single-shunt current sampling*/
#define ADC_CS_SS_MIN_SEGMENT_TIME               (3.0E-6f)                     /*[sec], Current measurement single shunt minimum measurable window */

#define ADC_SCALE_VUVW                           ((5.6f)/(56.0f+5.6f))         /*[V/V] = [Ohm/Ohm]*/
#define ADC_SCALE_VDC                            ((5.6f)/(56.0f+5.6f))         /*[V/V] = [Ohm/Ohm]*/
/*******************************************************************************/
/*******************************************************************************/
/*Parameter Controls*/
/***MCU***/
/*****System*****/
#define MOTOR_CTRL_BOOTSTRAP_TIME                  (0.25f)                     /*[sec], Bootstrap capacitor charge time*/
#define MOTOR_CTRL_BOOTSTRAP_MODE                  (Boot_and_Brake)            /*[], Boot_and_Brake = 0U, Boot_Only =1U*/ 
/*******Sampling*******/
#define MOTOR_CTRL_FPWM_FS0_RATIO                  (1U)                        /*[], PWM to fast-loop frequency ratio*/
#define MOTOR_CTRL_FASTLOOP_FREQ                   (15000.0f)                  /*[Hz], fast-loop frequency at least 1.2 decade above maximum electrical frequency, 1.5kHz~>25kHz*/
#define MOTOR_CTRL_FS0_FS1_RATIO                   (5U)                        /*[], Fast-loop to slow-loop frequency ratio*/

#define MOTOR_CTRL_PWM_DEADTIME                    (0.5E-6f)                   /*[sec], PWM Deadtime value*/

/*******Analog Sensors*******/
/*********Shunts*********/
/*Note:Shunt resistance and current amplifier gain value configuration at Power Board configuration*/
#define MOTOR_CTRL_SS_HMOD_KI                      (0.5f) 
#define MOTOR_CTRL_SS_PS_SH_DELAY                  (0.0E-6f)                   /* [sec], Switch delay configuration for ADC measurement for Phase shift modulation*/


#define MOTOR_CTRL_SS_MEAS_TYPE                   (Phase_Shift)                /*   Hyb_Mod =  0U, Hybrid modulation; Phase_Shift = 1U Phase Shift modulation*/  

/*********Rate Limiters*********/
#define MOTOR_CTRL_SPEED_CMD_RATE                  (1000.0f)                    /*[RPM/sec], Speed command rate*/
#define MOTOR_CTRL_SPEED_CMD_RATE_OPEN_LOOP        (1000.0f)                    /*[RPM/sec], Speed command rate during open loop state (Voltage OL and current OL)*/
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_TBC)
#define MOTOR_CTRL_CURRENT_CMD_RATE                (10.0f*MOTOR_CURRENT_PEAK)   /*[A/sec], Current command rate*/
#elif defined(CTRL_METHOD_SFO)
#define MOTOR_CTRL_TORQUE_CMD_RATE                 (10.0f*MOTOR_TORQUE_MAX)     /*[Nm/sec], Torque command rate*/
#endif

#if defined(CTRL_METHOD_RFO)
#define MOTOR_CTRL_POSITION_CMD_RATE               (180.0f)                     /*[Deg/sec], Position command rate*/
#endif
/*********Faults*********/
#define MOTOR_CTRL_OVER_CURRENT_THRESH             (120.0f)                     /*[%], over current fault threshold, percentage of motor continuous current*/
#define MOTOR_CTRL_VDC_DEBOUNCE_TIME               (0.3f)                       /*[sec], VDC fault detection debouncing time*/
#define MOTOR_CTRL_OVER_TEMP_THRESH                (90.0f)                      /*[Celsius], over temperature fault threshold*/

#define MOTOR_CTLR_FAULT_PHASE_LOSS_TIME            (1.0f)                      /*[sec],phase loss time detection constant*/
#define MOTOR_CTLR_FAULT_PHASE_LOSS_MIN_CURRENT     (0.001f)                    /*[A],phase loss detection minimum current threshold*/

#define MOTOR_CTRL_FAULT_SHORT_METHOD              (Alternate_Short)            /*During fault switch status, Low_Side_Short = 0U, High_Side_Short = 1U, Alternate_Short = 2U*/
#define MOTOR_CTRL_MAX_FAULT_CLR_TRIES             (10U)                        /*[], maximum nuumber of fault clear tries*/
#define MOTOR_CTRL_FAULT_CLR_TRY_PERIOD            (10.0f)                      /*[sec], Fault clear retry period*/  
/*********Command*********/
#define MOTOR_CTRL_COMMAND_SOURCE                  (Internal)                   /*Internal= 0(From potentiometer), External =1(From GUI, UART, etc.)*/

#define MOTOR_CTRL_COMMAND_MAX_SPEED               (MOTOR_NORM_SPEED)           /*[RPM], maximum speed command*/
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_TBC)
#define MOTOR_CTRL_COMMAND_MAX_CURRENT             (MOTOR_CURRENT_CONT*0.5f)    /*[A], maximum current command*/
#elif defined(CTRL_METHOD_SFO)
#define MOTOR_CTRL_COMMAND_MAX_TORQUE              (MOTOR_TORQUE_MAX*0.5f)      /*[Nm], Maximum troque command*/
#endif
#if defined(CTRL_METHOD_RFO)
#define MOTOR_CTRL_COMMAND_MAX_POSITION             (360.f)                    /*[deg], maximum current command*/
#endif
/*******Feedback*******/
/*********Hall Sensor*********/
#define MOTOR_CTRL_HALL_ANGLE_OFFSET               (0.0f)                      /*[deg], Hall angle offset*/
#define MOTOR_CTRL_HALL_ANGLE_OFFSET_COMP          (Dis)                       /*Dis =0, En=1, Enable/disable offset compensation*/

/*********Incremental Encoder*********/
#define MOTOR_CTRL_ENCODER_CPR                     (16384U)                    /*[], Edges per revolution*/
#define MOTOR_CTRL_ENCODER_TIME_CONST              (1.5f)                      /*[sec], Time constant ratio*/
#define MOTOR_CTRL_ENCODER_ZERO_SPEED_THRESH       (MOTOR_NORM_SPEED*0.008f)   /*[RPM], Zero speed detection threshold*/

/*******************************************************************************/
/*******Observer*******/
#define MOTOR_CTRL_OBS_SPEED_THRESH                (MOTOR_NORM_SPEED*0.2f)     /*[RPM], Observer activation speed threshold*/
#define MOTOR_CTRL_OBS_MIN_LOCK_TIME               (0.5f)                      /*[sec], Observer minimum lock time*/
#define MOTOR_CTRL_OBS_MAX_SPEED                   (MOTOR_MAX_SPEED*1.5f)      /*[RPM], Observer PLL maximum speed limit */
/*******************************************************************************/
/*******Filter*******/
#define MOTOR_CTRL_TORQUE_FILT_BW                  (1.0f)                      /*[Hz], Estimated torque filter bandwidth*/

/*********SPEED Anti_Resonant Filter*********/
#define MOTOR_CTRL_SPEED_AR_FILTER                 (En)                        /*Dis =0, En=1, Enable/disable speed anti-resosant filter*/

/*******************************************************************************/
/*******Control*******/
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
#define MOTOR_CTRL_CTRL_MODE                       (Speed_Mode_FOC_Sensorless_Volt_Startup)/*Control mode*/
#elif defined(CTRL_METHOD_TBC)
#define MOTOR_CTRL_CTRL_MODE                       (Speed_Mode_Block_Comm_Hall)  /*Control mode*/
#endif

/*********SPEED Controller*********/
#define MOTOR_CTRL_SPEED_BW                        (5.0f)                      /*[Hz], Speed loop bandwidth*/ //(15.0f)
#define MOTOR_CTRL_SPEED_OL_CL_TR_COEFF            (100.0f)                    /*[%], Open-loop to closed-loop transition coefficient*/
#define MOTOR_CTRL_SPEED_KI_MULTIPLE               (10.0f)                     /*[], Ki multiple for speed looop*/
#define MOTOR_CTRL_SPEED_FF_COEFF                  (100.0f)                    /*[%], Speed loop feed-forward coefficient*/ 

#if defined(CTRL_METHOD_TBC)    
#define MOTOR_CTRL_CURRENT_BYPASS                   (false)                    /* Current Control bypass switch*/
#endif

#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_TBC)
/*********Current Controller*********/
#define MOTOR_CTRL_CURRENT_BW                      (750.0f)                    /*[Hz], Current loop bandwidth*/
#define MOTOR_CTRL_CURRENT_FF_COEFF                (100.0f)                    /*[%], Current loop feed-forward coefficient*/
#define MOTOR_CTRL_CURRENT_STARTUP_THRESH          (MOTOR_CURRENT_CONT*0.15f)  /*[A], Current control startup threshold*/
#define MOTOR_CTRL_CURRENT_OPEN_LOOP_CMD           (MOTOR_CURRENT_CONT*0.4f)   /*[A], Current control open loop command value*/

#elif defined(CTRL_METHOD_SFO)
/*********Torque Controller*********/
#define MOTOR_CTRL_TORQUE_BW                       (150.0f)                    /*[Hz], Torque controller bandwidth*/
#define MOTOR_CTRL_POLE_ZERO_RATIO                 (25.0f)                     /*[%], Pole to zero ratio*/
#define MOTOR_CTRL_MAX_LOAD_ANGLE                  (120.0f)                    /*[Deg], Maximum load angle*/
#define MOTOR_CTRL_STARTUP_THRESH                  (MOTOR_TORQUE_MAX*0.2f)     /*[Nm], Startup threshold*/
#define MOTOR_CTRL_STARTUP_THRESH_HYS              (MOTOR_CTRL_STARTUP_THRESH*0.9f)           /*[Nm], Startup threshold hysteresis*/
#define MOTOR_CTRL_TIME_SM_CURRENT                 (0.04f)                     /*[sec], Reach time for sliding-mode current limiter*/

/*********Flux Controller*********/
#define MOTOR_CTRL_FLUX_BW                         (300.0f)                    /*[Hz], Flux loop bandwidth*/
#define MOTOR_CTRL_FLUX_POLE_SEP_RATIO             (1.1f)                      /*[#], Pole separation ratio*/

/*********Load Angle Controller*********/
#define MOTOR_CTRL_LOAD_ANGLE_BW                   (750.0f)                    /*[Hz], Load angle bandwidth*/
#define MOTOR_CTRL_BW_MUL                          (1.5f)                      /*[], Bandwidth multiplier*/
#define MOTOR_CTRL_BW_MUL_LS_THRESH                (150.0f)                    /*[Hz], Bandwidth multiplier low speed threshold*/
#define MOTOR_CTRL_BW_MUL_HS_THRESH                (300.0f)                    /*[Hz], Bandwidth multiplier high speed threshold*/
#define MOTOR_CTRL_LOAD_POLE_SEP_RATIO             (1.01f)                     /*[], Pole separation ration*/
#endif

#if defined(CTRL_METHOD_RFO)
#define MOTOR_CTRL_POSITION_BW                     (5.0f)                      /*[Hz], Position loop bandwidth*/  
#define MOTOR_CTRL_POSITION_POLE_SEP_RATIO         (1.001f)                    /*[#], Pole separation ration*/
#define MOTOR_CTRL_POSITION_FF_COEFF               (100.0f)                    /*[%], Position loop feed-forward coefficient*/ 
#define MOTOR_CTRL_POSITION_PI_LIMIT                (1000.0f)                  /*[RPM], Position PI  output limit*/
#endif

/*********Voltage Controller*********/
#define MOTOR_CTRL_VOLT_STARTUP_THRESH             (MOTOR_NORM_SPEED*0.05f)    /*[RPM], startup threshold*/
#define MOTOR_CTRL_VOLT_VF_OFFSET                  (0.15f)                     /*[V], V/F ramp voltage offset*/
#define MOTOR_CTRL_VOLT_VF_RATIO                   (7.5E-3f)                   /*[V/(Rad/secz], V/f ramp slope*/

#define MOTOR_CTRL_VOLT_MOD_SCHEME                 (Neutral_Point_Modulation)  /*Neutral_Point_Modulation =0,Space_Vector_Modulation =1*/

/***********Five Segment Modulation***********/
#define MOTOR_CTRL_FIVE_SEG_MOD                    (Dis)                       /*Dis =0, En=1, Enable/disable five segment modulation*/
#define MOTOR_CTRL_FIVE_SEG_MOD_ACT_THRESH         (10.0f)                     /*[%], Five segment modulation activation threshold*/
#define MOTOR_CTRL_FIVE_SEG_MOD_INACT_THRESH       (5.0f)                      /*[%], Five segment modulation deactivation threshold*/

#if defined(CTRL_METHOD_TBC)
/***********Trapezoidal Commutation***********/
#define MOTOR_CTRL_TBC_MODE                        (Block_Commutation)         /*Block_Commutation =0, Trapezoidal_Commutation =1*/
#define MOTOR_CTRL_TBC_TRAP_RAMP_COUNT             (5U)                        /*[#], Current ramp's sample count*/
#define MOTOR_CTRL_TBC_TRAP_RAMP_BW_RATIO          (0.80f)                     /*[#], Ramp to main controller bandwidth ratio*/
#define MOTOR_CTRL_TBC_TRAP_RAMP_FF_COEF           (4.0f)                      /*[V/A], Ramp feed-forware coefficient*/
#define MOTOR_CTRL_TBC_TRAP_MAIN_FF_COEF           (1.0f)                      /*[#], Main feed-formware coefficient*/
#endif

/*********Flux Weakening*********/
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
#define MOTOR_CTRL_FLUX_WEAKEN                     (En)                        /*Dis =0, En=1, Enable/disable flux weakening*/
#define MOTOR_CTRL_FLUX_WEAKEN_VOLT_MARGIN         (0.8f)                      /*[Hz], Flux weakening voltage margin*/
#if defined(CTRL_METHOD_RFO)
#define MOTOR_CTRL_FLUX_WEAKEN_BW                  (3.0f)                      /*[Hz], Flux weakening loop bandwidth*/
#endif
/*********Rotor Pre-Alignment*********/
#define MOTOR_CTRL_ALIGN_TIME                      (1.0f)                      /*[sec], Alignment time*/
#define MOTOR_CTRL_ALIGN_VOLTAGE                   (0.5f * (MOTOR_R * MOTOR_CURRENT_CONT))  /*[Vpk], Alignment voltage*/

/*********Six-Pulse Injection*********/
#define MOTOR_CTRL_SIX_PULSE_INJ_MAX_CURRENT       (3.5f)                      /*[A], Maximum current during  six pulse injection*/

/*********High Frequency Injection*********/
#define MOTOR_CTRL_HFI_TYPE                        (Sine_Wave)                 /*[], Type: sine-wave or square-wave*/
#define MOTOR_CTRL_HFI_MAX_CURRENT                 (1.75f)                     /*[A], Maximum excitation current d-axis*/
#define MOTOR_CTRL_HFI_INJECT_FREQ                 (1500.0f)                   /*[Hz], Injection Frequency*/
#define MOTOR_CTRL_HFI_SEPARATION_FREQ             (150.0f)                    /*[Hz], Separation frequency*/
#endif

#if defined(CTRL_METHOD_RFO)
/*********Catch free running motor*********/
#define MOTOR_CTRL_CATCH_SPIN_MODE                (Zero_Current_Control)       /*[], mode:Zero_Current_Control=0,Direct_Bemf_Measure =1 */
#define MOTOR_CTRL_CATCH_SPIN_TIME                (0.0f)                       /*[sec], Catch Spin time*/
#define MOTOR_CTRL_CATCH_SPIN_SPEED_THRESH        (MOTOR_CTRL_OBS_SPEED_THRESH)/*[RPM], Catch spin motor minimum speed threshold */
#endif

/*******************************************************************************/
/*******DC Supply*******/
#define MOTOR_CTRL_VDC_NOM_VOLT                    (24.0f)                     /*[V], Nominal DC bus voltage*/

/*******************************************************************************/
/*******Motor*******/
#define MOTOR_POLE                                 (8.0f)                      /*[],  motor poles*/
#define MOTOR_LQ                                   (670.0E-6f)                 /*[H], Stator q-axis inductance*/
#define MOTOR_LD                                   (670.0E-6f)                 /*[H], Stator d-axis inductance*/
#define MOTOR_I_AM                                 (6.0E-3f)                   /*[Wb],  Rotor flux linkage*/
#define MOTOR_R                                    (450.0E-3f)                 /*{Ohm],  stator resistance*/
#define MOTOR_TORQUE_MAX                           (0.390f)                    /*[Nm],  maximum torque*/
#define MOTOR_CURRENT_PEAK                         (10.80f)                    /*[A],  peak current rating*/
#define MOTOR_CURRENT_CONT                         (3.50f)                     /*[A],  continuous current rating*/
#define MOTOR_ID_MAX                               (1.75f)                     /*[A], maximum d-axis current*/
#define MOTOR_VOLTAGE                              (24.0f)                     /*[V], motor voltage*/
#define MOTOR_NORM_SPEED                           (4000.0f)                   /*[RPM], nominal speed*/
#define MOTOR_MAX_SPEED                            (6000.0f)                   /*[RPM],  maximum no load speed*/
#if defined(CTRL_METHOD_SFO)
#define MOTOR_MTPV_TORQUE_MARGIN                   (90.0f)                     /*[%],  MTPV torque margin*/
#endif

/*******I2T Protection*******/
#define MOTOR_CTRL_I2T_THERM_TAU                   (2.5f)                      /*[sec], Thermal time constant*/

/*******Profiler*******/
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
#define MOTOR_CTRL_PROFILER_PARAM_OVERWRITE        (Dis)                       /*Write the parameter value calculated from profiler*/
#define MOTOR_CTRL_PROFILER_CMD_THRESH             (5.0f)                      /*[%], Activation command threshold*/
#define MOTOR_CTRL_PROFILER_CMD_HYST               (2.5f)                      /*[%], Activation command hysteresis*/
#define MOTOR_CTRL_PROFILER_I_CMD_DC   (MOTOR_CURRENT_CONT * 0.5f)             /*[A], Target DC current*/
#define MOTOR_CTRL_PROFILER_I_CMD_AC   (MOTOR_CURRENT_CONT * 0.25f)            /*[A], Target AC current*/
#define MOTOR_CTRL_PROFILER_SPEED_CMD_MIN  (2.0f * MOTOR_CTRL_OBS_SPEED_THRESH)/*[RPM], Initial electrical speed*/
#define MOTOR_CTRL_PROFILER_SPEED_CMD_MAX   (0.75f * MOTOR_NORM_SPEED)         /*[RPM], Final electrical speed*/
#define MOTOR_CTRL_PROFILER_ROTOR_LOCK_TIME        (1.0f)                      /*[sec], Rotor locking time*/
#define MOTOR_CTRL_PROFILER_FLUX_EST_TIME          (1.5f)                      /*[sec], Flux estimation time*/
#endif
/*******************************************************************************/

/*******Mechanical Load*******/
#define MECH_INERTIA                              (1.1E-5f)                    /*[kg.m^2],  Inertia*/
#define MECH_VISCOUS                              (1.2E-5f)                    /*[kg.m^2/sec], Viscous Damping*/
#define MECH_FRICTION                             (6.0E-3f)                    /*[kg.m^2/sec^2], Frittion*/


