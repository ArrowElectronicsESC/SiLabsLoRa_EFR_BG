/***************************************************************************//**
 * @file
 * @brief os_cfg.h
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/*
 *********************************************************************************************************
 *
 *                                         KERNEL CONFIGURATION
 *
 *                                      CONFIGURATION TEMPLATE FILE
 *
 * File : os_cfg.h
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                               MODULE
 *********************************************************************************************************
 *********************************************************************************************************
 */

#ifndef  _OS_CFG_H_
#define  _OS_CFG_H_

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                             INCLUDE FILES
 *********************************************************************************************************
 *********************************************************************************************************
 */

#include  <common/include/lib_def.h>

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                            MISCELLANEOUS
 *
 * Note(s) : (1) Configure OS_CFG_APP_HOOKS_EN to enable or disable Application-specific Hooks.
 *
 *           (2) Configure OS_CFG_DBG_EN to enable or disable debug helper code and variables.
 *
 *           (3) Configure OS_CFG_DYN_TICK_EN to enable or disable the Dynamic Tick feature.
 *
 *           (4) Configure OS_CFG_TS_EN to enable or disable Timestamping capabilities.
 *
 *           (5) Configure OS_CFG_PRIO_MAX to set the maximum number of Task Priorities (see OS_PRIO data
 *               type).
 *
 *           (6) Configure OS_CFG_SCHED_LOCK_TIME_MEAS_EN to enable or disable the Scheduler Lock time
 *               measurement code.
 *
 *           (7) Configure OS_CFG_SCHED_ROUND_ROBIN_EN to enable or disable the Round-Robin Scheduler.
 *
 *           (8) Configure OS_CFG_STK_SIZE_MIN to set the minimum allowable Task Stack size (in CPU_STK
 *               elements).
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_APP_HOOKS_EN                                DEF_ENABLED

#define  OS_CFG_COMPAT_INIT_EN                              DEF_DISABLED

#define  OS_CFG_DYN_TICK_EN                                 DEF_ENABLED

#define  OS_CFG_PRIO_MAX                                    32u

#define  OS_CFG_SCHED_LOCK_TIME_MEAS_EN                     DEF_DISABLED

#define  OS_CFG_SCHED_ROUND_ROBIN_EN                        DEF_DISABLED

#if (RTOS_DEBUG_MODE == DEF_ENABLED)

#define  OS_CFG_DBG_EN                                      DEF_ENABLED

#define  OS_CFG_TS_EN                                       DEF_ENABLED

#define  OS_CFG_STK_SIZE_MIN                                64u

#else

#define  OS_CFG_DBG_EN                                      DEF_DISABLED

#define  OS_CFG_TS_EN                                       DEF_DISABLED

#define  OS_CFG_STK_SIZE_MIN                                48u

#endif

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                             EVENT FLAGS
 *
 * Note(s) : (1) Configure OS_CFG_FLAG_EN to enable or disable the Event Flags synchronization construct.
 *
 *           (2) Configure OS_CFG_FLAG_MODE_CLR_EN to enable or disable the active-low mode of Event
 *               Flags.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_FLAG_EN                                     DEF_ENABLED

#define  OS_CFG_FLAG_DEL_EN                                 DEF_ENABLED

#define  OS_CFG_FLAG_MODE_CLR_EN                            DEF_DISABLED

#define  OS_CFG_FLAG_PEND_ABORT_EN                          DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                          MEMORY MANAGEMENT
 *
 * Note(s) : (1) Configure OS_CFG_MEM_EN to enable or disable the built-in Memory Manager.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_MEM_EN                                      DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                     MUTUAL EXCLUSION SEMAPHORES
 *
 * Note(s) : (1) Configure OS_CFG_MUTEX_EN to enable or disable the Mutual Exclusion (Mutex)
 *               synchronization construct.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_MUTEX_EN                                    DEF_ENABLED

#define  OS_CFG_MUTEX_DEL_EN                                DEF_ENABLED

#define  OS_CFG_MUTEX_PEND_ABORT_EN                         DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                            MESSAGE QUEUES
 *
 * Note(s) : (1) Configure OS_CFG_Q_EN to enable or disable the Message Queue construct.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_Q_EN                                        DEF_ENABLED

#define  OS_CFG_Q_DEL_EN                                    DEF_ENABLED

#define  OS_CFG_Q_FLUSH_EN                                  DEF_ENABLED

#define  OS_CFG_Q_PEND_ABORT_EN                             DEF_ENABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                              SEMAPHORES
 *
 * Note(s) : (1) Configure OS_CFG_SEM_EN to enable or disable the Semaphore synchronization construct.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_SEM_EN                                      DEF_ENABLED

#define  OS_CFG_SEM_DEL_EN                                  DEF_ENABLED

#define  OS_CFG_SEM_PEND_ABORT_EN                           DEF_ENABLED

#define  OS_CFG_SEM_SET_EN                                  DEF_ENABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                               MONITORS
 *
 * Note(s) : (1) Configure OS_CFG_MON_EN to enable or disable the Monitor (condition variable)
 *               synchronization construct.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_MON_EN                                      DEF_DISABLED

#define  OS_CFG_MON_DEL_EN                                  DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                           TASK MANAGEMENT
 *
 * Note(s) : (1) Configure OS_CFG_STAT_TASK_EN to enable or disable the Statistics gathering Task.
 *
 *           (2) Configure OS_CFG_STAT_TASK_STK_CHK_EN to enable or disable the stack overflow detection
 *               of the Statistics Task.
 *
 *           (3) Configure OS_CFG_TASK_DEL_EN to enable or disable the deletion of Tasks.
 *
 *           (4) Configure OS_CFG_TASK_IDLE_EN to enable or disable the Idle Task.
 *
 *           (5) Configure OS_CFG_TASK_PROFILE_EN to enable or disable Task profiling instrumentation.
 *
 *           (6) Configure OS_CFG_TASK_Q_EN to enable or disable built-in Task Message Queues.
 *
 *           (7) Configure OS_CFG_TASK_Q_PEND_ABORT_EN to enable or disable the pend abort feature of
 *               Task Message Queues.
 *
 *           (8) Configure OS_CFG_TASK_REG_TBL_SIZE to set the number of Task Registers.
 *
 *           (9) Configure OS_CFG_TASK_STK_REDZONE_EN to enable or disable the Redzone Stack Protection.
 *
 *           (10) Configure OS_CFG_TASK_STK_REDZONE_DEPTH to set the depth of the Redzone Stack
 *               Protection.
 *
 *           (11) Configure OS_CFG_TASK_SEM_PEND_ABORT_EN to enable or disable the pend abort feature of
 *               Task Semaphores.
 *
 *           (12) Configure OS_CFG_TASK_SUSPEND_EN to enable or disable the suspension of Tasks.
 *
 *           (13) Configure OS_CFG_TASK_TICK_EN to enable or disable the Tick Task.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#if (RTOS_DEBUG_MODE == DEF_ENABLED)
#define  OS_CFG_TASK_PROFILE_EN                             DEF_ENABLED

#define  OS_CFG_STAT_TASK_EN                                DEF_ENABLED
#else
#define  OS_CFG_TASK_PROFILE_EN                             DEF_DISABLED

#define  OS_CFG_STAT_TASK_EN                                DEF_DISABLED
#endif

#define  OS_CFG_STAT_TASK_STK_CHK_EN                        DEF_ENABLED

#define  OS_CFG_TASK_DEL_EN                                 DEF_ENABLED

#define  OS_CFG_TASK_IDLE_EN                                DEF_ENABLED

#define  OS_CFG_TASK_Q_EN                                   DEF_DISABLED

#define  OS_CFG_TASK_Q_PEND_ABORT_EN                        DEF_DISABLED

#define  OS_CFG_TASK_REG_TBL_SIZE                           0u

#define  OS_CFG_TASK_STK_REDZONE_EN                         DEF_ENABLED

#define  OS_CFG_TASK_STK_REDZONE_DEPTH                      8u

#define  OS_CFG_TASK_SEM_PEND_ABORT_EN                      DEF_ENABLED

#define  OS_CFG_TASK_SUSPEND_EN                             DEF_DISABLED

#define  OS_CFG_TASK_TICK_EN                                DEF_ENABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                    TASK LOCAL STORAGE MANAGEMENT
 *
 * Note(s) : (1) Configure OS_CFG_TLS_TBL_SIZE to set the number of Task Local Storage registers.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_TLS_TBL_SIZE                                0u

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                           TIME MANAGEMENT
 *
 * Note(s) : (1) Configure OS_CFG_TIME_DLY_HMSM_EN to enable or disable the human-readable (HH:MM:SS.mm)
 *               time delay feature.
 *
 *           (2) Configure OS_CFG_TIME_DLY_RESUME_EN to enable or disable the cancel delay feature.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_TIME_DLY_HMSM_EN                            DEF_ENABLED

#define  OS_CFG_TIME_DLY_RESUME_EN                          DEF_ENABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                           TIMER MANAGEMENT
 *
 * Note(s) : (1) Configure OS_CFG_TMR_EN to enable or disable Software Timers.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_TMR_EN                                      DEF_DISABLED

#define  OS_CFG_TMR_DEL_EN                                  DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                           Trace Recorders
 *
 * Note(s) : (1) Configure OS_CFG_TRACE_EN to enable or disable the kernel events trace recorder.
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  OS_CFG_TRACE_EN                                    DEF_DISABLED

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                             MODULE END
 *********************************************************************************************************
 *********************************************************************************************************
 */

#endif /* End of os_cfg.h module include.                      */
