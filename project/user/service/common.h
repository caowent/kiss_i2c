/**
  ******************************************************************************
  * @file    common
  * @author  曹文涛
  * @date    2019-12-30
  * @brief   公有的依赖
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

#include "gpio.h"
#include <string.h>
#include <stdio.h>

/* 流程状态枚举 */
typedef enum 
{
    STEP1,
    STEP2,
    STEP3,
    STEP4,
    STEP5,
    STEP6,
    STEP7
} STEP_Typ;

/* 进程锁控制支持 */
typedef enum
{
    /* 进程解锁状态 */
    UNLOCKED,
    /* 进程上锁状态 */
    LOCKED,
} lock_typ;

/* 进程上锁 */
#define PROCESS_LOCK(handle)                \
do                                          \
{                                           \
    if(handle == LOCKED)                    \
    {                                       \
        /* 进程处于上锁状态，则直接返回 */  \
        return;                             \
    }                                       \
    else                                    \
    {                                       \
        /* 先上锁，再执行进程 */            \
        handle = LOCKED;                    \
    }                                       \
}while(0)

/* 进程解锁 */
#define PROCESS_UNLOCK(handle)              \
do                                          \
{                                           \
    handle = UNLOCKED;                      \
}while(0)

/* debug日志开关 */
#define DEBUG_LOG               (0)

/* 日志输出API */
#if DEBUG_LOG
#define print_log       SEGGER_SYSVIEW_PrintfTarget
#else
#define print_log(...)
#endif

/* cmsis os */
#define osDelay rt_thread_mdelay

#endif /* __COMMON_H */

/*********************************** END OF FILE ***********************************/
