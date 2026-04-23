/*******************************************************************************
 * Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company)
 *or an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
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
#include "MotorCtrlLibConfig.h"

/* Array of motor instances, one entry per motor.
 * Each entry holds pointers to the state machine, control variables, parameters,
 * observer, faults, protection, hall sensor, sensor interface, encoder, and profiler
 * (profiler available for RFO/SFO methods only).
 * Motor 0 is always active; Motor 1 is compiled in only if MOTOR_CTRL_MOTOR1_ENABLED is set. */
MOTOR_t motor[MOTOR_CTRL_NO_OF_MOTOR] = {
    {   /* Motor 0 */
        .motor_instance = 0,
        .sm_ptr           = &sm[0],              // State machine
        .vars_ptr         = &vars[0],            // variables
        .ctrl_ptr         = &ctrl[0],            // Control variables 
        .params_ptr       = &params[0],          // parameters
        .obs_ptr          = &obs[0],             // Observer (sensorless estimation)
        .faults_ptr       = &faults[0],          // Fault flags and handling
        .protect_ptr      = &protect[0],         // Protection 
        .hall_ptr         = &hall[0],            // Hall sensor interface
        .sensor_iface_ptr = &sensor_iface[0],    // Generic sensor interface
        .inc_encoder_ptr  = &inc_encoder[0],     // Incremental encoder interface
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
        .profiler_ptr     = &profiler[0],        // Motor profiler (RFO/SFO only)
#endif
    }
};


/*******************************************************************************
 * @brief   Hysteresis comparator for drive enable/disable control.
 *
 * Prevents rapid toggling of the drive enable signal by applying two separate
 * thresholds: the drive enables only when the command exceeds enable_thres, and
 * disables only when it drops below disable_thres. In the dead-band between the
 * two thresholds the previous state is preserved.
 *
 * @param   current_state   Current drive enable state (true = enabled).
 *                          Returned unchanged when cmd is within the dead-band
 *                          [disable_thres, enable_thres].
 * @param   cmd             Absolute command value to evaluate (e.g. |w|, |i|, |T|).
 *                          Units depend on the calling context (rad/s, A, Nm, etc.).
 * @param   enable_thres    Upper threshold. Drive is enabled when cmd > enable_thres.
 * @param   disable_thres   Lower threshold. Drive is disabled when cmd < disable_thres.
 *                          Must satisfy disable_thres < enable_thres to form a valid dead-band.
 *
 * @return  true  – drive should be enabled.
 *          false – drive should be disabled.
 *******************************************************************************/
static inline bool DriveEnableControl(bool  current_state, float cmd, float enable_thres, float disable_thres)   
{
    bool ret_val = current_state;   // Default: preserve previous state in the hysteresis dead-band

    if (cmd > enable_thres)             // Command crossed upper threshold ? enable drive
    {
        ret_val = true;
    }
    else if (cmd < disable_thres)       // Command dropped below lower threshold ? disable drive
    {
        ret_val = false;
    }
    // else: cmd is in [disable_thres, enable_thres] dead-band ? ret_val unchanged (hysteresis hold)
    return (ret_val);
}

/*******************************************************************************
 * @brief   Automatically controls drive enable/disable via potentiometer command.
 *
 * When the command source is Internal (potentiometer), no external enable signal
 * is available. This function applies hysteresis-based thresholds to determine
 * whether the drive should be enabled or disabled, based on the active control mode:
 *
 *  - Speed / Volt modes : |w_cmd_ext| vs. w_thresh / w_hyst  [elec. rad/s]
 *  - Current modes      : |i_cmd_ext| vs. i_cmd_thresh / i_cmd_hyst  [A]
 *  - Torque modes (SFO) : |T_cmd_ext| vs. T_cmd_thresh / T_cmd_hyst  [Nm]
 *  - Profiler mode      : cmd_final   vs. profiler.cmd_thresh / cmd_hyst
 *  - Position mode      : drive always enabled (en = true)
 *
 * Has no effect when cmd.source != Internal.
 *
 * @param   motor_ptr   Pointer to the motor instance to update.
 *******************************************************************************/
void DriveEnableControlforPot(MOTOR_t *motor_ptr)
{
    PARAMS_t*    params_ptr = motor_ptr->params_ptr;    // Control parameters (thresholds, hysteresis, mode)
    CTRL_VARS_t* vars_ptr   = motor_ptr->vars_ptr;      // Runtime variables (command, enable flag)

    if (params_ptr->sys.cmd.source == Internal) // Only applicable for potentiometer (Internal) command source
    {
        switch (params_ptr->ctrl.mode)
        {
        default:
        // Speed / Voltage modes: threshold on electrical speed command (rad/s)
        case Volt_Mode_Open_Loop:
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
        case Speed_Mode_FOC_Sensorless_Align_Startup:
        case Speed_Mode_FOC_Sensorless_SixPulse_Startup:
        case Speed_Mode_FOC_Sensorless_HighFreq_Startup:
        case Speed_Mode_FOC_Sensorless_Volt_Startup:
#endif
#if defined(CTRL_METHOD_TBC)
        case Speed_Mode_Block_Comm_Hall:
#elif defined(CTRL_METHOD_RFO)
        case Speed_Mode_FOC_Encoder_Align_Startup:
        case Speed_Mode_FOC_Hall:
        case Curr_Mode_Open_Loop:
#endif
            vars_ptr->en = DriveEnableControl(vars_ptr->en,                                                            // Current enable state
                                              ABS(vars_ptr->w_cmd_ext.elec),                                                     // |speed command| [elec. rad/s]
                                              params_ptr->ctrl.volt.w_thresh.elec,                                      // Enable threshold  [elec. rad/s]
                                              params_ptr->ctrl.volt.w_thresh.elec - params_ptr->ctrl.volt.w_hyst.elec); // Disable threshold [elec. rad/s]
            break;

        // Current modes: threshold on current command (A)
#if defined(CTRL_METHOD_RFO)
        case Curr_Mode_FOC_Sensorless_Align_Startup:
        case Curr_Mode_FOC_Sensorless_SixPulse_Startup:
        case Curr_Mode_FOC_Sensorless_HighFreq_Startup:
        case Curr_Mode_FOC_Sensorless_Dyno:
        case Curr_Mode_FOC_Encoder_Align_Startup:
        case Curr_Mode_FOC_Hall:
#elif defined(CTRL_METHOD_TBC)
        case Curr_Mode_Block_Comm_Hall:
#endif
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_TBC)
            vars_ptr->en = DriveEnableControl(vars_ptr->en,                                                           // Current enable state
                                              ABS(vars_ptr->i_cmd_ext),                                                         // |current command| [A]
                                              params_ptr->ctrl.curr.i_cmd_thresh,                                      // Enable threshold  [A]
                                              params_ptr->ctrl.curr.i_cmd_thresh - params_ptr->ctrl.curr.i_cmd_hyst); // Disable threshold [A]
            break;
#endif

        // Torque modes (SFO): threshold on torque command (Nm)
#if defined(CTRL_METHOD_SFO)
        case Trq_Mode_FOC_Sensorless_Align_Startup:
        case Trq_Mode_FOC_Sensorless_SixPulse_Startup:
        case Trq_Mode_FOC_Sensorless_HighFreq_Startup:
        case Trq_Mode_FOC_Sensorless_Dyno:
            vars_ptr->en = DriveEnableControl(vars_ptr->en,                                                         // Current enable state
                                              ABS(vars_ptr->T_cmd_ext),                                             // |torque command| [Nm]
                                              params_ptr->ctrl.trq.T_cmd_thresh,                                    // Enable threshold  [Nm]
                                              params_ptr->ctrl.trq.T_cmd_thresh - params_ptr->ctrl.trq.T_cmd_hyst); // Disable threshold [Nm]
            break;
#endif

        // Profiler mode: threshold on generic profiler command
#if defined(CTRL_METHOD_RFO) || defined(CTRL_METHOD_SFO)
        case Profiler_Mode:
            vars_ptr->en = DriveEnableControl(vars_ptr->en,                                                         // Current enable state
                                              vars_ptr->cmd_final,                                                            // Profiler command 
                                              params_ptr->profiler.cmd_thresh,                                       // Enable threshold
                                              params_ptr->profiler.cmd_thresh - params_ptr->profiler.cmd_hyst);     // Disable threshold
            break;
#endif

        // Position mode: drive always enabled; direction is controlled by position command
#if defined(CTRL_METHOD_RFO)
        case Position_Mode_FOC_Encoder_Align_Startup:
            vars_ptr->en = true;    // Always enabled; position command determines direction
            break;
#endif
        }
    }
}