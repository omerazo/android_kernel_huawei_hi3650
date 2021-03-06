

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "PsTypeDef.h"
#include "ImsaProcMsccMsg.h"
#include "ImsaEntity.h"
#include "ImsaPublic.h"
#include "ImsaServiceManagement.h"
#include "ImsaImsAdaption.h"
#include "ImsaMain.h"
#include "ImsaImsApi.h"
#include "ImsaProcSpmMsg.h"
#include "ImsaConnManagement.h"
#include "ImsaProcWifiMsg.h"
#include "ImsaDomainManagement.h"

/*lint -e767*/
#define    THIS_FILE_ID       PS_FILE_ID_IMSAPROCMSCCMSG_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function
*****************************************************************************/

/*lint -e961*/

VOS_VOID IMSA_ProcMsccMsg(const VOS_VOID *pRcvMsg )
{
    /* 定义消息头指针*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* 获取消息头指针*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        case ID_MSCC_IMSA_START_REQ:
            IMSA_ProcMsccMsgStartReq();
            break;

        case ID_MSCC_IMSA_STOP_REQ:
            IMSA_ProcMsccMsgStopReq(pRcvMsg);
            break;

         case ID_MSCC_IMSA_SERVICE_CHANGE_IND:
            IMSA_ProcMsccMsgServiceChangeInd(pRcvMsg);
            break;

        case ID_MSCC_IMSA_CAMP_INFO_CHANGE_IND:
            IMSA_ProcMsccMsgCampInfoChangeInd(pRcvMsg);
            break;
        case ID_MSCC_IMSA_VOICE_DOMAIN_CHANGE_IND:
            IMSA_ProcMsccMsgVoiceDomainChangeInd(pRcvMsg);
            break;

        case ID_MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ:
            IMSA_ProcMsccMsgRoamingImsSetReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_MSCC_IMSA_AREA_LOST_IND:
            IMSA_ProcMsccMsgAreaLostInd(pRcvMsg);
            break;
        default:
            break;
    }
}


VOS_VOID IMSA_ProcMsccMsgStartReq(VOS_VOID  )
{
    IMSA_CONTROL_MANAGER_STRU  *pstControlManager;
    static VOS_UINT8            ucD2ImsTaskStart = 0;

    pstControlManager = IMSA_GetControlManagerAddress();

    if(ucD2ImsTaskStart == 0)
    {
        /*初始化D2协议栈*/
        (VOS_VOID)IMSA_ImsInit();

        ucD2ImsTaskStart ++;
    }

    /*判断开机状态，如果已开机，则回复开机成功*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTED)
    {
        IMSA_SndMsccMsgStartCnf();
        return;
    }

    /*已处于开机过程状态，则丢弃*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTING)
    {
        IMSA_WARN_LOG("IMSA_ProcMsccMsgStartReq: Status is Starting!");
        TLPS_PRINT2LAYER_WARNING(IMSA_ProcMsccMsgStartReq_ENUM, IMSA_PRINT_STATUS_STARTING);
        return;
    }

    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*初始化*/

    /*读取NV*/
    IMSA_ReadImsaNV();

    /*通知IMS开机，转状态*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STARTING;

    IMSA_SndImsMsgStartReq();

    IMSA_StartTimer(&pstControlManager->stProtectTimer);

    /*卡在位，则读取卡信息*/
    if(pstControlManager->enImsaIsimStatus == IMSA_ISIM_STATUS_AVAILABLE)
    {
        IMSA_INFO_LOG("IMSA_ProcMsccMsgStartReq: ISIM is available!");
        TLPS_PRINT2LAYER_INFO(IMSA_ProcMsccMsgStartReq_ENUM, 2);
        IMSA_ReadIsimFile();
    }
}


VOS_VOID IMSA_SndMsccMsgStartCnf(VOS_VOID  )
{
    IMSA_MSCC_START_CNF_STRU           *pstImsaMsccStartCnf;

    /*分配空间并检验分配是否成功*/
    pstImsaMsccStartCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_START_CNF_STRU));

    /*检测是否分配成功*/
    if (VOS_NULL_PTR == pstImsaMsccStartCnf)
    {
        /*打印异常信息*/
        IMSA_ERR_LOG("IMSA_SndMsccMsgStartCnf:ERROR:Alloc Msg fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndMsccMsgStartCnf_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*清空*/
    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccStartCnf),
                    IMSA_GET_MSG_LENGTH(pstImsaMsccStartCnf),
                    0,
                    IMSA_GET_MSG_LENGTH(pstImsaMsccStartCnf));

    /*填写消息头*/
    IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccStartCnf, ID_IMSA_MSCC_START_CNF);

    /*调用消息发送函数 */
    IMSA_SND_MSG(pstImsaMsccStartCnf);
}





VOS_VOID IMSA_ProcMsccMsgStopReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_CONTROL_MANAGER_STRU *pstControlManager;

    IMSA_INFO_LOG("IMSA_ProcMsccMsgStopReq: enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_ProcMsccMsgStopReq_ENUM, LNAS_ENTRY);
    (VOS_VOID)pRcvMsg;
    pstControlManager = IMSA_GetControlManagerAddress();

    /* SRVCC异常(例如关机，DEREG REQ，状态迁离CONN+REG)，清缓存 */
    IMSA_SrvccAbormalClearBuff(IMSA_SRVCC_ABNORMAL_STOP_REQ);

    /*判断状态，如果已关机，则回复关机成功*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_NULL)
    {
        IMSA_SndMsccMsgStopCnf();
        return;
    }

    /*已处于关机状态，则丢弃*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STOPING)
    {
        IMSA_WARN_LOG("IMSA_ProcMsccMsgStopReq: Status is Stoping!");
        TLPS_PRINT2LAYER_WARNING(IMSA_ProcMsccMsgStopReq_ENUM, IMSA_PRINT_STATUS_STOPING);
        return;
    }

    /*停止开关机保护定时器*/
    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*通知IMS关机，转状态*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STOPING;

    /*关机后，卡在位时，需要将卡状态置为有效，因为可能有卡无效的场景，需要关机后清除状态*/
    if(pstControlManager->enImsaIsimStatus != IMSA_ISIM_STATUS_ABSENT)
    {
        pstControlManager->enImsaIsimStatus = IMSA_ISIM_STATUS_AVAILABLE;
    }

    /*启动开关机保护定时器*/
    IMSA_StartTimer(&pstControlManager->stProtectTimer);

    /* 如果当前状态时IDLE+DEREG，则直接通知IMS关机，不需要申请资源 */
    #if (FEATURE_ON == FEATURE_DSDS)
    if (PS_RRM_RF_SHARED == IMSA_Dsds_RRM_RfResourceIsShared())
    {
        if (IMSA_TRUE == IMSA_SRV_IsNotNeedNwDereg())
        {
            IMSA_SndImsMsgStopReq();
            return;
        }
    }
    #endif
    /* 需要根据当前的注册状态，判断是否需要申请资源 */
    switch (IMSA_DsdsGetResourceStatus(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER))
    {
        case IMSA_RESOURCE_STATUS_ACTIVE:

            IMSA_SndImsMsgStopReq();
            #if (FEATURE_ON == FEATURE_DSDS)
            if (PS_RRM_RF_SHARED == IMSA_Dsds_RRM_RfResourceIsShared())
            {
                IMSA_DsdsModifyTaskSrcByTaskType(IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                                 RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);
            }
            #endif
            break;
        #if (FEATURE_ON == FEATURE_DSDS)
        case IMSA_RESOURCE_STATUS_APPLY_NOT_NEED_WAIT_CNF:

            IMSA_DsdsProcResourceApplyReq(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER,
                                          IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                          VOS_FALSE,
                                          VOS_FALSE);

            IMSA_SndImsMsgStopReq();
            break;

        case  IMSA_RESOURCE_STATUS_NULL:

            IMSA_DsdsProcResourceApplyReq(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER,
                                          IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                          VOS_FALSE,
                                          VOS_FALSE);

            //IMSA_DsdsSaveMoSmsMsg(pstRpDataReq);
            break;

        case IMSA_RESOURCE_STATUS_APPLYING:
            /* 如果当前状态为pending，则取消任务的注册，然后通知IMS本地去注册，然后直接关机 */
            IMSA_DsdsModifyTaskSrcByTaskType(IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                             RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);
            break;

        case IMSA_RESOURCE_STATUS_WAIT_STATUS_IND:

            IMSA_DsdsProcResourceRelease(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);

            IMSA_SRV_ProcStopMsgWhenNoRf();

            IMSA_SndImsMsgStopReq();
            break;
        #endif
        default:
            break;
    }

    //IMSA_SndImsMsgStopReq();
}


VOS_VOID IMSA_ProcStopResourceApplyCnf
(
    VOS_VOID
)
{
    IMSA_SndImsMsgStopReq();
}


VOS_VOID IMSA_ProcStopResourceApplyTimerExp
(
    VOS_VOID
)
{
    /* 先通知IMS本地去注册 */
    IMSA_SRV_SndRegDeregReq(IMSA_REG_TYPE_NORMAL, IMSA_DEREG_LOCAL);
    IMSA_SRV_SetServcieStatus(IMSA_SRV_TYPE_NORMAL, IMSA_SRV_STATUS_CONN_DEREG);

    /* 然后在通知关机 */
    IMSA_SndImsMsgStopReq();

}



VOS_VOID IMSA_SndMsccMsgStopCnf(VOS_VOID  )
{
    IMSA_MSCC_STOP_CNF_STRU           *pstImsaMsccStopCnf;

    IMSA_INFO_LOG("IMSA_SndMsccMsgStopCnf:enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_SndMsccMsgStopCnf_ENUM, LNAS_ENTRY);

    /*分配空间并检验分配是否成功*/
    pstImsaMsccStopCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_STOP_CNF_STRU));

    /*检测是否分配成功*/
    if (VOS_NULL_PTR == pstImsaMsccStopCnf)
    {
        /*打印异常信息*/
        IMSA_ERR_LOG("IMSA_SndMsccMsgStopCnf:ERROR:Alloc Msg fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndMsccMsgStopCnf_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*清空*/
    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccStopCnf),
                    IMSA_GET_MSG_LENGTH(pstImsaMsccStopCnf),
                    0,
                    IMSA_GET_MSG_LENGTH(pstImsaMsccStopCnf));

    /*填写消息头*/
    IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccStopCnf, ID_IMSA_MSCC_STOP_CNF);

    /*调用消息发送函数 */
    IMSA_SND_MSG(pstImsaMsccStopCnf);
}


VOS_VOID IMSA_ProcMsccMsgServiceChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_SRV_ProcServiceChangeInd((MSCC_IMSA_SERVICE_CHANGE_IND_STRU*)pRcvMsg);

    return ;
}


VOS_VOID IMSA_ProcMsccMsgCampInfoChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_SRV_ProcCampInfoChangeInd((MSCC_IMSA_CAMP_INFO_CHANGE_IND_STRU*)pRcvMsg);

    return ;
}



VOS_VOID IMSA_SndMsccMsgDeregCnf(VOS_VOID )
{
    IMSA_MSCC_DEREGISTER_CNF_STRU           *pstImsaMsccDeregCnf;

    /* 清除去注册原因值 */
    IMSA_SetDeregCause(IMSA_SRV_DEREG_CAUSE_BUTT);

    /*分配空间并检验分配是否成功*/
    pstImsaMsccDeregCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_DEREGISTER_CNF_STRU));

    /*检测是否分配成功*/
    if (VOS_NULL_PTR == pstImsaMsccDeregCnf)
    {
        /*打印异常信息*/
        IMSA_ERR_LOG("IMSA_SndMsccMsgDeregCnf:ERROR:Alloc Msg fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndMsccMsgDeregCnf_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*清空*/
    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccDeregCnf),
                    IMSA_GET_MSG_LENGTH(pstImsaMsccDeregCnf),
                    0,
                    IMSA_GET_MSG_LENGTH(pstImsaMsccDeregCnf));

    /*填写消息头*/
    IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccDeregCnf, ID_IMSA_MSCC_DEREG_CNF);

    /*调用消息发送函数 */
    IMSA_SND_MSG(pstImsaMsccDeregCnf);
}


VOS_VOID IMSA_SndMsccMsgRegCnf(VOS_VOID )
{
    IMSA_MSCC_REG_CNF_STRU             *pstImsaMsccRegCnf;

    /*分配空间并检验分配是否成功*/
    pstImsaMsccRegCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_REG_CNF_STRU));

    /*检测是否分配成功*/
    if (VOS_NULL_PTR == pstImsaMsccRegCnf)
    {
        /*打印异常信息*/
        IMSA_ERR_LOG("IMSA_SndMsccMsgDeregCnf:ERROR:Alloc Msg fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndMsccMsgDeregCnf_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*清空*/
    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccRegCnf),
                    IMSA_GET_MSG_LENGTH(pstImsaMsccRegCnf),
                    0,
                    IMSA_GET_MSG_LENGTH(pstImsaMsccRegCnf));

    /*填写消息头*/
    IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccRegCnf, ID_IMSA_MSCC_REG_CNF);

    /*调用消息发送函数 */
    IMSA_SND_MSG(pstImsaMsccRegCnf);
}

VOS_VOID IMSA_ProcMsccMsgVoiceDomainChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_SRV_ProcVoiceDomainChangeInd((MSCC_IMSA_VOICE_DOMAIN_CHANGE_IND_STRU*)pRcvMsg);

    return ;
}


VOS_VOID IMSA_ProcMsccMsgAreaLostInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    (VOS_VOID)pRcvMsg;
    IMSA_SRV_ProcAreaLostInd();

    return ;
}

VOS_VOID IMSA_ProcImsMsgStartOrStopCnf(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager;
    IMSA_WIFI_NORMAL_CONN_STRU         *pstWifiNormalConn  = VOS_NULL_PTR;
    IMSA_DOMAIN_MANAGER_STRU     *pstDomainManager;

    IMSA_NORM_LOG("IMSA_ProcImsMsgStartOrStopCnf: Enter!");
    TLPS_PRINT2LAYER_ERROR(IMSA_ProcImsMsgStartOrStopCnf_ENUM, LNAS_ENTRY);

    pstControlManager = IMSA_GetControlManagerAddress();
    pstDomainManager    = IMSA_Domain_GetDomainManagerAddr();

    /*停止开关机保护定时器*/
    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*关机流程*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STOPING)
    {
        /* IMS动态开关不再通过开关机消息通知 */
        pstWifiNormalConn = IMSA_CONN_GetWifiNormalConnAddr();
        if (IMSA_CONN_STATUS_IDLE != pstWifiNormalConn->enImsaConnStatus)
        {
            /* 请求WIFI释放连接 */
            IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL);
        }

        if( IMSA_STATUS_STOPING == pstControlManager->enImsaWifiStatus)
        {
            /* 指示SPM WIFI关机成功 */
//            IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_OFF);

            /* 回复WIFI关机成功 */
            IMSA_SndWifiPowerOffCnf();

            pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;
        }
        else if((IMSA_STATUS_STOPING == pstControlManager->enImsaCellularStatus)
                || (IMSA_STATUS_STOPING == pstControlManager->enImsaStatus))
        {
            IMSA_SndMsccMsgStopCnf();
            pstControlManager->enImsaCellularStatus = IMSA_STATUS_NULL;
        }

        /*清除资源*/
        IMSA_ClearResource();

        /*进入等待开机状态*/
        pstControlManager->enImsaStatus = IMSA_STATUS_NULL;
    }
    else if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTING)
    {
        /*进入开机状态*/
        pstControlManager->enImsaStatus = IMSA_STATUS_STARTED;

        if( VOS_TRUE == pstControlManager->ucInternalImsStackStartFlag)
        {
            /* IMSA内部开机行为,不需要回复开机成功消息 */
            pstControlManager->ucInternalImsStackStartFlag = VOS_FALSE;
        }
        else
        {
            if( IMSA_STATUS_STARTING == pstControlManager->enImsaWifiStatus)
            {
                /* WIFI进入开机状态 */
                pstControlManager->enImsaWifiStatus = IMSA_STATUS_STARTED;

                /* 指示SPM WIFI开机成功 */
                IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_ON);

                /* 回复WIFI开机成功 */
                IMSA_SndWifiPowerOnCnf();
                (VOS_VOID)IMSA_Domain_SetDomainModeInWifiPowerOn();
            }

            if( IMSA_STATUS_STARTING == pstControlManager->enImsaCellularStatus)
            {
                pstControlManager->enImsaCellularStatus = IMSA_STATUS_STARTED;
                (VOS_VOID)IMSA_Domain_SetDomainModeInCellularPowerOn();
                IMSA_SndMsccMsgStartCnf();
            }

            /* 通知ERRC启动上报LTE信号质量 */
            if( (VOS_FALSE == pstDomainManager->ucLteQualityCfgFlag)
                && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
            {
                IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
                pstDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
            }
        }

        /* 给IMS配置UE能力 */
        IMSA_ConfigUeCapabilityInfo2Ims();

        if ((IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpImpi)
            && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpTImpu)
            && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.bitOpHomeNetDomainName))
        {
            /* config account info to ims */
            IMSA_ConfigAccoutInfo2Ims(IMSA_REG_TYPE_NORMAL);

            /* config account info to ims */
            IMSA_ConfigAccoutInfo2Ims(IMSA_REG_TYPE_EMC);
        }

        /* set imei */
        IMSA_ConfigImei2Ims();

        /* set retry time */
        IMSA_ConfigTimerLength2Ims();

        IMSA_ConfigSipPort2Ims();

        /* 给IMS配置SIP信息 */
        IMSA_ConfigSipInfo2Ims();

        IMSA_ConfigVoipInfo2Ims();

        IMSA_ConfigCodeInfo2Ims();

        IMSA_ConfigSsConfInfo2Ims();

        IMSA_ConfigSecurityInfo2Ims();

        IMSA_ConfigMediaParmInfo2Ims();

        IMSA_ConfigNvUeCapabilityInfo2Ims();

    }
    else
    {
        IMSA_WARN_LOG("IMSA_ProcImsMsgStartOrStopCnf: err stat!");
        TLPS_PRINT2LAYER_WARNING(IMSA_ProcImsMsgStartOrStopCnf_ENUM, IMSA_STATE_ERROR);
    }


}


 
 VOS_VOID IMSA_SndMsccMsgImsVoiceCapNotify
 (
    MSCC_IMSA_IMS_VOICE_CAP_ENUM_UINT8   enImsVoiceCap,
    VOS_UINT32                          ucIsExistPersistentBearer
 )
 {
     IMSA_MSCC_IMS_VOICE_CAP_NOTIFY_STRU     *pstImsaMsccImsVoiceCap;
     IMSA_CONTROL_MANAGER_STRU               *pstImsaControlManager   = VOS_NULL_PTR;

     pstImsaControlManager   = IMSA_GetControlManagerAddress();

     /*分配空间并检验分配是否成功*/
     pstImsaMsccImsVoiceCap = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_IMS_VOICE_CAP_NOTIFY_STRU));

     /*检测是否分配成功*/
     if (VOS_NULL_PTR == pstImsaMsccImsVoiceCap)
     {
         /*打印异常信息*/
         IMSA_ERR_LOG("IMSA_SndMsccMsgImsVoiceCapNotify:ERROR:Alloc Msg fail!");
         TLPS_PRINT2LAYER_ERROR(IMSA_SndMsccMsgImsVoiceCapNotify_ENUM, LNAS_NULL_PTR);
         return ;
     }

     /*清空*/
     IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccImsVoiceCap),
                     IMSA_GET_MSG_LENGTH(pstImsaMsccImsVoiceCap),
                     0,
                     IMSA_GET_MSG_LENGTH(pstImsaMsccImsVoiceCap));

     /*填写消息头*/
     IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccImsVoiceCap, ID_IMSA_MSCC_IMS_VOICE_CAP_NOTIFY);


     pstImsaMsccImsVoiceCap->enImsVoiceCap = enImsVoiceCap;

    pstImsaMsccImsVoiceCap->ucIsExistPersistentBearer = (VOS_UINT8)ucIsExistPersistentBearer;
    /*lint -e539*/
    if( VOS_TRUE == pstImsaControlManager->ucRcvServiceChangeIndFlag)
    {
        pstImsaMsccImsVoiceCap->ucRcvServiceChangeIndFlag = VOS_TRUE;
        pstImsaControlManager->ucRcvServiceChangeIndFlag = VOS_FALSE;
    }
    else
    {
        pstImsaMsccImsVoiceCap->ucRcvServiceChangeIndFlag = VOS_FALSE;
    }

    /*调用消息发送函数 */
     IMSA_SND_MSG(pstImsaMsccImsVoiceCap);
      /*lint +e539*/
 }


/*****************************************************************************
 Function Name  : IMSA_SndMsccImsDomainCfgSetCnf()
 Description    : 发送MSCC IMS优先域设置成功消息处理函数
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndMsccImsDomainCfgSetCnf(VOS_VOID )
{
   IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF_STRU           *pstImsaMsccImsDomainCfgSetCnf;

   IMSA_INFO_LOG("IMSA_SndMsccImsDomainCfgSetCnf:enter!");

   /*分配空间并检验分配是否成功*/
   pstImsaMsccImsDomainCfgSetCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF_STRU));

   /*检测是否分配成功*/
   if (VOS_NULL_PTR == pstImsaMsccImsDomainCfgSetCnf)
   {
    /*打印异常信息*/
      IMSA_ERR_LOG("IMSA_SndMsccImsDomainCfgSetCnf:ERROR:Alloc Msg fail!");
      return ;
    }

   /*清空*/
   IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaMsccImsDomainCfgSetCnf),IMSA_GET_MSG_LENGTH(pstImsaMsccImsDomainCfgSetCnf), 0, IMSA_GET_MSG_LENGTH(pstImsaMsccImsDomainCfgSetCnf));

   /*填写消息头*/
   IMSA_WRITE_MSCC_MSG_HEAD(pstImsaMsccImsDomainCfgSetCnf, ID_IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF);

   pstImsaMsccImsDomainCfgSetCnf->enResult = IMSA_MSCC_SET_IMS_DOMAIN_RESULT_SUCC;

   /*调用消息发送函数 */
   IMSA_SND_MSG(pstImsaMsccImsDomainCfgSetCnf);
}

#if (FEATURE_ON == FEATURE_DSDS)

VOS_VOID IMSA_ProcTimerMsgWaitPowerOffRfExp(const VOS_VOID *pRcvMsg)
{
    /* 通知RRM模块释放资源 */
    IMSA_DsdsProcResourceRelease(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);

    /* 需要先根据当前的注册状态，通知IMS本地去注册，然后再通知IMS关机 */
    IMSA_SRV_ProcStopMsgWhenNoRf();
    IMSA_SndImsMsgStopReq();
}
#endif


VOS_VOID IMSA_ProcMsccMsgRoamingImsSetReq
(
    const MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ_STRU *pstRoamingImsSetReq
)
{
    IMSA_NV_IMS_RAT_SUPPORT_STRU        stNvImsRatSupport;
    VOS_UINT32                          ulRslt;

    IMSA_INFO_LOG("IMSA_ProcMsgRoamingImsSetReq is entered! ");
    TLPS_PRINT2LAYER_INFO(IMSA_ProcMsgRoamingImsSetReq_ENUM, LNAS_ENTRY);

    /* 如果当前有IMS业务存在，则直接返回失败 */
    if (VOS_TRUE == IMSA_IsImsExist())
    {
        /* 返回AT命令失败 */
        IMSA_SndMsccMsgRoamingImsSetCnf(IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_FAILED);
        return;
    }

    ulRslt = IMSA_NV_Read(EN_NV_ID_IMS_RAT_SUPPORT,&stNvImsRatSupport,\
        sizeof(IMSA_NV_IMS_RAT_SUPPORT_STRU));

    /* 读NV失败，返回失败 */
    if (VOS_OK != ulRslt)
    {
        IMSA_INFO_LOG("IMSA_ProcMsgRoamingImsSetReq: Read NV Err! ");
        TLPS_PRINT2LAYER_INFO1(IMSA_ProcMsgRoamingImsSetReq_ENUM, IMSA_CALL_API_ERROR, 1);

        /* 返回AT命令失败 */
        IMSA_SndMsccMsgRoamingImsSetCnf(IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_FAILED);
        return;
    }

    /* 根据配置的不同写NV结构体 */
    switch (pstRoamingImsSetReq->enRoamImsSupport)
    {
        case MSCC_IMSA_SET_ROAM_IMS_SUPPORT:
            stNvImsRatSupport.ucRoamingImsNotSupportFlag = VOS_FALSE;
            break;

        case MSCC_IMSA_SET_ROAM_IMS_UNSUPPORT:
            stNvImsRatSupport.ucRoamingImsNotSupportFlag = VOS_TRUE;
            break;

        /* 参数错误分支，直接返回失败 */
        default:
            /* 返回AT命令失败 */
            IMSA_SndMsccMsgRoamingImsSetCnf(IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_FAILED);
            return;
    }

    ulRslt = IMSA_NV_Write(EN_NV_ID_IMS_RAT_SUPPORT,&stNvImsRatSupport,\
            sizeof(IMSA_NV_IMS_RAT_SUPPORT_STRU));

    /* 写NV失败，返回失败 */
    if (VOS_OK != ulRslt)
    {
        IMSA_INFO_LOG("IMSA_ProcMsgRoamingImsSetReq: Write NV Err! ");
        TLPS_PRINT2LAYER_INFO1(IMSA_ProcMsgRoamingImsSetReq_ENUM, IMSA_CALL_API_ERROR, 2);

        /* 返回AT命令失败 */
        IMSA_SndMsccMsgRoamingImsSetCnf(IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_FAILED);
        return;
    }

    /* 读写NV全部成功，则修改本地维护的全局变量 */
    IMSA_GetConfigParaAddress()->ucRoamingImsNotSupportFlag = stNvImsRatSupport.ucRoamingImsNotSupportFlag;

    /* 返回AT命令处理结果 */
    IMSA_SndMsccMsgRoamingImsSetCnf(IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_SUCCESS);

    /* 如果配置漫游不支持，则发起去注册，反之则发起注册 */
    if (VOS_TRUE == IMSA_GetConfigParaAddress()->ucRoamingImsNotSupportFlag)
    {
        /* 调用漫游去注册流程接口 */
        IMSA_SRV_ProcWhenRoamNotSatisfied();
    }
    else
    {
        /* 调用注册流程接口 */
        IMSA_SRV_ProcWhenRoamIsSatisfied();
    }
    return;
}


VOS_VOID IMSA_SndMsccMsgRoamingImsSetCnf
(
    IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_ENUM_UINT32   enResult
)
{
    IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF_STRU          *pstRoamingImsSetCnf;

    /*分配消息空间*/
    pstRoamingImsSetCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF_STRU));

    /*检测是否分配成功*/
    if (VOS_NULL_PTR == pstRoamingImsSetCnf)
    {
        /*打印异常信息*/
        IMSA_ERR_LOG("IMSA_SndMsgAtRoamingImsSetCnf:ERROR:Alloc Msg fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndMsgAtRoamingImsSetCnf_ENUM, LNAS_NULL_PTR);
        return ;
    }

    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstRoamingImsSetCnf),
                    IMSA_GET_MSG_LENGTH(pstRoamingImsSetCnf),
                    0,
                    IMSA_GET_MSG_LENGTH(pstRoamingImsSetCnf));

    IMSA_WRITE_MSCC_MSG_HEAD(pstRoamingImsSetCnf, ID_IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF);

    /*填写消息内容*/
    pstRoamingImsSetCnf->enResult = enResult;

    /*调用消息发送函数 */
    IMSA_SND_MSG(pstRoamingImsSetCnf);
    return;
}


/*lint +e961*/


#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcMmaMsg.c */



