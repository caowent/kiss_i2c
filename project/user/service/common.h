/**
  ******************************************************************************
  * @file    common
  * @author  ������
  * @date    2019-12-30
  * @brief   ���е�����
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

#include "gpio.h"
#include <string.h>
#include <stdio.h>

/* ����״̬ö�� */
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

/* ����������֧�� */
typedef enum
{
    /* ���̽���״̬ */
    UNLOCKED,
    /* ��������״̬ */
    LOCKED,
} lock_typ;

/* �������� */
#define PROCESS_LOCK(handle)                \
do                                          \
{                                           \
    if(handle == LOCKED)                    \
    {                                       \
        /* ���̴�������״̬����ֱ�ӷ��� */  \
        return;                             \
    }                                       \
    else                                    \
    {                                       \
        /* ����������ִ�н��� */            \
        handle = LOCKED;                    \
    }                                       \
}while(0)

/* ���̽��� */
#define PROCESS_UNLOCK(handle)              \
do                                          \
{                                           \
    handle = UNLOCKED;                      \
}while(0)

/* debug��־���� */
#define DEBUG_LOG               (0)

/* ��־���API */
#if DEBUG_LOG
#define print_log       SEGGER_SYSVIEW_PrintfTarget
#else
#define print_log(...)
#endif

/* cmsis os */
#define osDelay rt_thread_mdelay

#endif /* __COMMON_H */

/*********************************** END OF FILE ***********************************/
