/*
 * @ 名称: bootloader_main.c
 * @ 描述:
 * @ 作者: Tomy
 * @ 日期: 2021年2月5日
 * @ 版本: V1.0
 * @ 历史: V1.0 2021年2月5日 Summary
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

    if (TRUE == Boot_IsPowerOnTriggerReset())//判断复位类型，若是上电复位，则清除ram中的标志
    {
        Boot_PowerONClearAllFlag();
    }
    #ifdef STAYINBOOT
        bResult = StayInBootCheck();//延时一段时间检测是否收到报文
    #endif
    if(FALSE == bResult)
    {
        Boot_JumpToAppOrNot();//判断是否跳转到app
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
