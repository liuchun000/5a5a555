/*
 * @ ����: bootloader_main.c
 * @ ����:
 * @ ����: Tomy
 * @ ����: 2021��2��5��
 * @ �汾: V1.0
 * @ ��ʷ: V1.0 2021��2��5�� Summary
 *
 * MIT License. Copyright (c) 2021 SummerFalls.
 */

#include "bootloader_main.h"
#include "includes.h"
#include "uds_app.h"
#include "TP.h"
#include "fls_app.h"
#include "timer_hal.h"
#include "watchdog_hal.h"
#include "boot.h"
#include "CRC_HAL.h"


void UDS_MAIN_Init(void (*pfAbortTxMsg)(void))
{
 boolean bResult = FALSE;

#ifdef UDS_PROJECT_FOR_BOOTLOADER

    if (TRUE == Boot_IsPowerOnTriggerReset())//�жϸ�λ���ͣ������ϵ縴λ�������ram�еı�־
    {
        Boot_PowerONClearAllFlag();
    }
    #ifdef STAYINBOOT
        bResult = StayInBootCheck();//��ʱһ��ʱ�����Ƿ��յ�����
    #endif
    if(FALSE == bResult)
    {
        Boot_JumpToAppOrNot();//�ж��Ƿ���ת��app
    }

#endif
 
    TP_Init();
  
    TIMER_HAL_Init();
 
    UDS_Init();

#ifdef UDS_PROJECT_FOR_BOOTLOADER
    Boot_CheckReqBootloaderMode();
#endif

#ifdef UDS_PROJECT_FOR_APP
    Boot_CheckDownlaodAPPStatus();
#endif

    TP_RegisterAbortTxMsg(pfAbortTxMsg);
    FLASH_APP_Init();
}

void UDS_MAIN_Process(void)
{
#ifdef EN_DEBUG_IO
    static uint16 timerCnt1Ms = 0u;
#endif

    if (TRUE == TIMER_HAL_Is1msTickTimeout())
    {
        TP_SystemTickCtl();
        UDS_SystemTickCtl();
#ifdef EN_DEBUG_IO

        timerCnt1Ms++;

        if (250u == timerCnt1Ms)
        {
            timerCnt1Ms = 0u;

            /* Blink LED */
            ToggleDebugIO();
        }

#endif
    }

    if (TRUE == TIMER_HAL_Is100msTickTimeout())
    {
        /* Feed watch dog every 100ms */
        WATCHDOG_HAL_Feed();
    }

    TP_MainFun();
    UDS_MainFun();
    //Flash_OperateMainFunction();
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
