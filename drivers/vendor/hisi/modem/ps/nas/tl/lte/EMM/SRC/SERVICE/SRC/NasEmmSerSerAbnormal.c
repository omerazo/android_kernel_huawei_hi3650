


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "NasEmmAttDetInclude.h"
#include "NasEmmSerProc.h"
#if (FEATURE_LPP == FEATURE_ON)
#include "NasEmmLppMsgProc.h"
#include "EmmLppInterface.h"
#endif
#if (FEATURE_AGPS == FEATURE_ON)
#include "NasEmmSsMsgProc.h"
#endif
#include "ImsaLmmInterface.h"

/*lint -e767*/
#define    THIS_FILE_ID            PS_FILE_ID_NASEMMSERVICEABNORMAL_C
#define    THIS_NAS_FILE_ID        NAS_FILE_ID_NASEMMSERVICEABNORMAL_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/* 函数名改变 */
NAS_LMM_SER_REJ_CAUSE_PROC_STRU  gstEmmSerRejCauseProcTbl[] =
{
    {NAS_LMM_CAUSE_ILLEGAL_UE,                                  NAS_EMM_SER_SERREJ368},
    {NAS_LMM_CAUSE_ILLEGAL_ME,                                  NAS_EMM_SER_SERREJ368},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW,                          NAS_EMM_SER_SERREJ7},
    {NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,         NAS_EMM_SER_SERREJ8},
    {NAS_LMM_CAUSE_UE_ID_NOT_DERIVED,                           NAS_EMM_SER_SERREJ9},
    {NAS_LMM_CAUSE_IMPLICIT_DETACHED,                           NAS_EMM_SER_SERREJ10},
    {NAS_LMM_CAUSE_PLMN_NOT_ALLOW,                              NAS_EMM_SER_SERREJ11},
    {NAS_LMM_CAUSE_TA_NOT_ALLOW,                                NAS_EMM_SER_SERREJ12},
    {NAS_LMM_CAUSE_ROAM_NOT_ALLOW,                              NAS_EMM_SER_SERREJ13},
    {NAS_LMM_CAUSE_NO_SUITABL_CELL,                             NAS_EMM_SER_SERREJ15},
    {NAS_LMM_CAUSE_CS_NOT_AVAIL,                                NAS_EMM_SER_SERREJ18},
	{NAS_LMM_CAUSE_PROCEDURE_CONGESTION,                        NAS_EMM_SER_SERREJ22},
#if(FEATURE_ON == FEATURE_CSG)
    {NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG,                 NAS_EMM_SER_SERREJ25},
#endif
    {NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN, NAS_EMM_SER_SERREJ35},
    {NAS_LMM_CAUSE_CS_DOMAIN_TMP_NOT_ALLOWED,                   NAS_EMM_SER_SERREJ39},
    {NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,             NAS_EMM_SER_SERREJ40},
    {NAS_LMM_CAUSE_SERVER_NETWORK_FAILURE,                      NAS_EMM_SER_SERREJ42}
};
VOS_UINT32        gstEmmSerRejCauseProcTblLen = sizeof(gstEmmSerRejCauseProcTbl)
                                                / sizeof(NAS_LMM_SER_REJ_CAUSE_PROC_STRU);
VOS_UINT32        g_ulEsrRej39OptimizeCtr = NAS_EMM_NO;

/*******************************************************************************
  Module   : NAS_EMM_SER_AbnormalNeedtoContinueTau
  Function :
  Input    :
  Output   :
  NOTE     : 无
  Return   : 无
  History  :
    1.FTY    2012-02-27  新规作成
*******************************************************************************/
VOS_UINT32 NAS_EMM_SER_AbnormalNeedtoContinueTau(VOS_VOID)
{
    if((NAS_EMM_T3412_EXP_NO < NAS_LMM_GetEmmInfoT3412ExpCtrl())
      &&(NAS_EMM_T3412_EXP_BUTT > NAS_LMM_GetEmmInfoT3412ExpCtrl()))
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_EPS_BEARER_STATUS_CHANGEED == NAS_EMM_GetEpsContextStatusChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE == NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
        return NAS_EMM_YES;
    }
    if(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE == NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
        return NAS_EMM_YES;
    }
    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_LMM_UE_RADIO_CAP_NOT_CHG != NAS_LMM_GetEmmInfoUeRadioCapChgFlag())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_EMM_GetVoiceDomainChange() )
    {
        return NAS_EMM_YES;
    }


    return NAS_EMM_NO;

}


VOS_UINT32 NAS_EMM_SER_SuccNeedtoContinueTau(NAS_LMM_OM_TAU_START_TYPE_ENUM_UINT32 *pulTauStartType)
{
    *pulTauStartType = NAS_LMM_OM_TAU_START_TYPE_OTHERS;

    if(NAS_EMM_EPS_BEARER_STATUS_CHANGEED == NAS_EMM_GetEpsContextStatusChange())
    {
        *pulTauStartType = NAS_LMM_OM_TAU_START_TYPE_BEARER_STATUS_CHANGEED;
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
        *pulTauStartType = NAS_LMM_OM_TAU_START_TYPE_DRX_CHANGEED;
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange())
    {
        *pulTauStartType = NAS_LMM_OM_TAU_START_TYPE_INTER_CHANGE;
        return NAS_EMM_YES;
    }
    return NAS_EMM_NO;

}


VOS_VOID    NAS_EMM_SER_RcvRrcRelInd(VOS_VOID)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_RcvRrcRelInd is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_RcvRrcRelInd_ENUM,LNAS_ENTRY);

    /*设置SER的结果为 NAS_EMM_SER_RESULT_ABNORMAL*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_ABNORMAL);*/
    NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    /*转换EMM状态机MS_REG+EMM_SS_REG_NORMAL_SERVICE*/
    /*NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);*/

    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

        /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
        NAS_EMM_CommProcConn2Ilde();
        return ;
    }

    /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
    NAS_EMM_CommProcConn2Ilde();

    /* 如果有未完成的TAU，无需在此触发，有定时器等定时器超时，无定时器等后续收到系统消息再触发 */

    return;
}


VOS_VOID  NAS_EMM_ServiceReqRejectOtherCause
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_ServiceReqRejectOtherCause is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_ServiceReqRejectOtherCause_ENUM,LNAS_ENTRY);

    /* 美国AT&T定制需求，对SERVICE失败场景计数的处理 */
    NAS_EMM_SER_CServiceCountProc();

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    NAS_EMM_SER_TbackoffProc();

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_ServiceReqRejectOtherCause: CAUSE_ESM_DATA_REQ_EMC.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_ServiceReqRejectOtherCause_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    }
    /* 如果有未完成的TAU，等在REG+NORMAL_SERVICE状态下收到系统消息后再处理 */

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);


    return;
}


/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_EMM_SerClearEsmDataBuf( VOS_VOID )
{
    VOS_UINT32                          i      = 0;
    VOS_UINT32                          ulRslt = 0;

     NAS_EMM_SER_LOG_INFO("NAS_EMM_SerClearEsmDataBuf entered!");
     TLPS_PRINT2LAYER_INFO(NAS_EMM_SerClearEsmDataBuf_ENUM,LNAS_ENTRY);

     /**清除保存数据的标志和数据区**************/
     for (i = 0; i < g_stEmmEsmMsgBuf.ulEsmMsgCnt; i++)
     {
         if (NAS_LMM_NULL_PTR != g_stEmmEsmMsgBuf.apucEsmMsgBuf[i])
         {
            ulRslt = NAS_COMM_FreeBuffItem(NAS_COMM_BUFF_TYPE_EMM,
                                (VOS_VOID *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[i]);

            if (NAS_COMM_BUFF_SUCCESS != ulRslt)
            {
                NAS_EMM_SER_LOG_WARN("NAS_EMM_SerClearEsmDataBuf, Memory Free is not succ");
                TLPS_PRINT2LAYER_WARNING(NAS_EMM_SerClearEsmDataBuf_ENUM,LNAS_FUNCTION_LABEL1);
            }

            g_stEmmEsmMsgBuf.apucEsmMsgBuf[i] = NAS_LMM_NULL_PTR;
         }
         else
         {
             NAS_EMM_SER_LOG1_WARN("NAS_EMM_SerClearEsmDataBuf, Buffer item is null:",i);
             TLPS_PRINT2LAYER_WARNING1(NAS_EMM_SerClearEsmDataBuf_ENUM,LNAS_FUNCTION_LABEL2,i);
         }
     }

     g_stEmmEsmMsgBuf.ulEsmMsgCnt = 0;

}



VOS_UINT32 NAS_EMM_SER_IsNotEmergencyCsfb(VOS_VOID)
{
    if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ != NAS_EMM_SER_GetEmmSERStartCause())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_EMM_IsEsrRej39OptimizeOpen(VOS_VOID)
{
    TLPS_PRINT2LAYER_INFO(NAS_EMM_IsEsrRej39OptimizeOpen_ENUM,LNAS_ENTRY);

    /* 判断是否是测试卡 */
    if (PS_SUCC == LPS_OM_IsTestMode())
    {
        TLPS_PRINT2LAYER_INFO(NAS_EMM_IsEsrRej39OptimizeOpen_ENUM,LNAS_TEST_CARD);
        return NAS_EMM_NO;
    }

    /* 判断NV是否打开 */
    if (NAS_EMM_NO == g_ulEsrRej39OptimizeCtr)
    {
        TLPS_PRINT2LAYER_INFO(NAS_EMM_IsEsrRej39OptimizeOpen_ENUM,LNAS_NV_NOT_OPEN);
        return NAS_EMM_NO;
    }

    return NAS_EMM_YES;
}


/* #7需要去GU下尝试 */
VOS_VOID  NAS_EMM_SER_SERREJ368
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ367 entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ368_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);
    /*USIM无效，直到关机或拔卡*/
    /*暂不处理*/
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI, TI_NAS_EMM_STATE_NO_TIMER);

    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }


    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*向LRRC发送LRRC_LMM_NAS_INFO_CHANGE_REQ携带USIM卡状态*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return;
}

VOS_VOID  NAS_EMM_SER_SERREJ7
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ367 entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ7_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);
    /*USIM无效，直到关机或拔卡*/
    /*暂不处理*/
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI, TI_NAS_EMM_STATE_NO_TIMER);

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*向LRRC发送LRRC_LMM_NAS_INFO_CHANGE_REQ携带USIM卡状态*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    #if (FEATURE_ON == FEATURE_PTM)
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MT_CALL);
    }
    else if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
    }
    else
    {
    }
    #endif

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ8
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    if (NAS_RELEASE_R11)
    {
        /* 修改函数名 */
        NAS_EMM_SER_SERREJ368(pMsgStru);
    }
    else
    {
        NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
    }
}

VOS_VOID  NAS_EMM_SER_SERREJ9
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ9 entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ9_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /* 设置设置EU2 NOT UPDATED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_NOT_UPDATED_EU2);

    /* 删除GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /* 主叫呼通率提升: 在R13协议里描述,SerRej #9,需要去GU上尝试 */
    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);
    /* 如果是紧急呼PDN建立引起的SERVICE，则直接发起紧急注册；
       如果是紧急CSFB,直接释放链路，否则发内部ATTACH; */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SERREJ9: CAUSE_ESM_DATA_REQ_EMC");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ9_ENUM,LNAS_FUNCTION_LABEL1);

        /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
        NAS_EMM_EmcPndReqSerTAURejProc();

    }
    else if (VOS_FALSE == NAS_EMM_SER_IsCsfbProcedure())
    {/* CSFB 收到#9时需要去GU下尝试打电话 */
        if (NAS_RELEASE_R11)
        {
            /*等待RRC_REL_IND*/
            NAS_EMM_WaitNetworkRelInd();
            return;
        }
        else
        {
            /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
               待延时定时器超时后, 直接发起ATTACH流程*/
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);
        }
    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* 清空SER和TAU所有资源*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ10
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ10 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ10_ENUM,LNAS_ENTRY);
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /* 主叫呼通率提升: 在R13协议里描述,SerRej #10,需要去GU上尝试 */

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* 转换EMM状态机MS_DEREG+SS_DEREG_NORMAL_SERVICE:状态转换函数执行删除新安全上下文*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* 进入DEREG态不再写统一写安全上下文，参照标杆，在SER被#10拒绝，将安全上下文写卡 */
    NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
    /* 如果是紧急呼PDN建立引起的SERVICE，则直接发起紧急注册；
       如果是紧急CSFB,直接释放链路，否则发内部ATTACH; */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SERREJ10: CAUSE_ESM_DATA_REQ_EMC");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ10_ENUM,LNAS_FUNCTION_LABEL1);

        /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
        NAS_EMM_EmcPndReqSerTAURejProc();

    }
    else if (VOS_FALSE == NAS_EMM_SER_IsCsfbProcedure())
    {/* CSFB流程收到ESR #10时,需要去GU下尝试 */
        if (NAS_RELEASE_R11)
        {
            /*等待RRC_REL_IND*/
            NAS_EMM_WaitNetworkRelInd();
            return;
        }
        else
        {
            /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
               待延时定时器超时后, 直接发起ATTACH流程*/
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);

        }

    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* 清空SER和TAU所有资源*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ11
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
     /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ11 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ11_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+EMM_SS_DEREG_LIMITED_SERVICE*/
    /*收到MMC搜网指令后再转入PLMN_SEARCH态*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }

    #if (FEATURE_ON == FEATURE_PTM)
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
    }
    #endif

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*等待RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ12
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stTa;

    /* 获取当前TA */
    NAS_EMM_GetCurrentTa(&stTa);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ12 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ12_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*store the current TAI in the list of
                  "forbidden tracking areas for regional provision of service"*/
    NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRposAddr());
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /* 注册/服务状态上报上移MMC，删除注册/服务状态相关代码 */

    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }
    #if (FEATURE_ON == FEATURE_PTM)
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
    }
    #endif

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*等待RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ13
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ13 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ13_ENUM,LNAS_ENTRY);

    /*设置EU3 ROAMING NOT ALLOWED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*保存PS LOC信息*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);

#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
    {
        /* 需要把当前TA从临时列表中剔除 */
        NAS_LMM_DelCurrTaFromGradualForbTaList(&stCurrentTa);
        /*store the current TAI in the list of
                "forbidden tracking areas for roaming"*/
        NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());

        NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
    }

    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }

    #if (FEATURE_ON == FEATURE_PTM)
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
    }
    #endif

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_SER_SERREJ13: CAUSE_ESM_DATA_REQ_EMC.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ13_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*为上报服务状态,转换EMM状态机MS_REG+SS_REG_LIMITED_SERVICE收到MMC搜网指令
          后再转入PLMN_SEARCH态*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
    }
    /*等待RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ15
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;
    NAS_EMM_GRADUAL_FORBIDDEN_RESULT_ENUM_UINT32 enGradualForbResult = NAS_EMM_GRADUAL_FORB_TA_NEED_ADD_ROAM_LIST;
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ15 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ15_ENUM,LNAS_ENTRY);

    /*设置EU3 ROAMING NOT ALLOWED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*保存PS LOC信息*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);
#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
    {
        /* 判断加入临时被禁列表中 */
        enGradualForbResult = NAS_LMM_AddTaInGradualForbTempTaList(&stCurrentTa);

        /* 判断是否需要加入ROAM Forb列表 */
        if (NAS_EMM_GRADUAL_FORB_TA_NEED_ADD_ROAM_LIST == enGradualForbResult)
        {
            /*store the current TAI in the list of
                       "forbidden tracking areas for roaming"*/
            NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());
        }
        NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
    }
    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }

    #if (FEATURE_ON == FEATURE_PTM)
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ== NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
    }
    #endif

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_SER_SERREJ15: CAUSE_ESM_DATA_REQ_EMC.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ15_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*为上报服务状态,转换EMM状态机MS_REG+SS_REG_LIMITED_SERVICE收到MMC搜网指令
          后再转入PLMN_SEARCH态*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
    }
    /*等待RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ18
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
    /* 测试卡模式下，按照协议要求结束CSFB流程
       非测试卡模式下,为提升用户体验，不中止电话，搜网到GU下继续打电话 */
    if ((PS_SUCC == LPS_OM_IsTestMode())
        &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }

    /*转换EMM状态机MS_REG+SS_REG_NORMAL_SERVIC*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* 连接态下，释放链路，搜网到GU下继续打电话 */
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

        #if (FEATURE_ON == FEATURE_PTM)
        if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MO_CALL);
        }
        else if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            NAS_EMM_ImprovePerformceeErrReport(EMM_OM_ERRLOG_IMPROVEMENT_TYPE_CS_MT_CALL);
        }
        else
        {
        }
        #endif
    }

    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ22
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_ServiceReqRejectOtherCause is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_ServiceReqRejectOtherCause_ENUM,LNAS_ENTRY);

    /* 美国AT&T定制需求，对SERVICE失败场景计数的处理 */
    NAS_EMM_SER_CServiceCountProc();

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);
    NAS_EMM_SER_TbackoffProc();
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_ServiceReqRejectOtherCause: CAUSE_ESM_DATA_REQ_EMC.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_ServiceReqRejectOtherCause_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    }
    /* 如果有未完成的TAU，等在REG+NORMAL_SERVICE状态下收到系统消息后再处理 */

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);


    return;
}
#if(FEATURE_ON == FEATURE_CSG)

VOS_VOID NAS_EMM_SER_SERREJ25
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ25 entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ25_ENUM,LNAS_ENTRY);
    if(NAS_EMM_NO == NAS_LMM_IsCsgCampOn())
    {
        NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
    }
    else
    {
        /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
        NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

        /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
        NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

        if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        {
            NAS_EMM_TAU_LOG_INFO("NAS_EMM_SER_SERREJ25: CAUSE_ESM_DATA_REQ_EMC.");
            TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ25_ENUM,LNAS_FUNCTION_LABEL1);

            NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_LIMITED_SERVICE);
        }
        else
        {
            /*为上报服务状态,转换EMM状态机MS_REG+SS_REG_LIMITED_SERVICE收到MMC搜网指令
              后再转入PLMN_SEARCH态*/
            NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                            EMM_SS_REG_LIMITED_SERVICE,
                                            TI_NAS_EMM_STATE_NO_TIMER);
        }

        /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
        NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    }
    return;
}
#endif


VOS_VOID NAS_EMM_SER_SERREJ35
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    /* 判断nas r10开关是否打开 */
    if (NAS_RELEASE_CTRL)
    {
        NAS_EMM_SER_SERREJ11(pMsgStru);
    }
    else
    {
        NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
    }
    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ40
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ40 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ40_ENUM,LNAS_ENTRY);

    (VOS_VOID)pMsgStru;

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /* 转换EMM状态机MS_DEREG+SS_DEREG_NORMAL_SERVICE:状态转换函数执行删除新安全上下文*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SERREJ40: CAUSE_ESM_DATA_REQ_EMC");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ40_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerTAURejProc();
    }
    else
    {
        /* 如果是CSFB流程或紧急CSFB，则重选到GU模 */
        if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
            || (VOS_FALSE == NAS_EMM_SER_IsNotEmergencyCsfb()))
        {
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        }
        else
        {
            if (NAS_RELEASE_R11)
            {
                /*等待RRC_REL_IND*/
                NAS_EMM_WaitNetworkRelInd();
                return;
            }
            else
            {
                    /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
                       待延时定时器超时后, 直接发起ATTACH流程
                    */
                    NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);
            }
        }
    }
    /* 清空SER和TAU所有资源*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;
}



VOS_VOID  NAS_EMM_SER_SERREJ39
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    VOS_UINT32                      ulTimerLen      = NAS_EMM_NULL;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ39 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ39_ENUM,LNAS_ENTRY);

    /* 如果是CSFB流程，当Esr Rej #39优化没有打开，且不是紧急CSFB，则给MM发终止消息 */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb())
        && (NAS_EMM_NO == NAS_EMM_IsEsrRej39OptimizeOpen()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ, pMsgStru->ucEMMCause);
    }

    /*转换EMM状态机MS_REG+SS_REG_NORMAL_SERVIC*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* 如果是紧急CSFB或者不是数据连接态或Esr Rej #39优化打开的CSFB,主动释放 */
    if ((VOS_FALSE == NAS_EMM_SER_IsNotEmergencyCsfb())
        || (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
        || ((NAS_EMM_YES == NAS_EMM_IsEsrRej39OptimizeOpen())
            && (NAS_EMM_YES == NAS_EMM_SER_IsCsfbProcedure())))
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    if (NAS_EMM_BIT_SLCT != pMsgStru->ucBitOpT3442)
    {
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_SERREJ39:No T3442!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_SER_SERREJ39_ENUM,LNAS_FUNCTION_LABEL1);
        return ;
    }

    ulTimerLen = NAS_EMM_TimerValTransfer(  pMsgStru->stT3442.ucUnit,
                                            pMsgStru->stT3442.ucTimerVal);

    if (NAS_EMM_NULL != ulTimerLen)
    {
        NAS_LMM_ModifyStateTimer(TI_NAS_EMM_STATE_SERVICE_T3442,ulTimerLen);
        NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3442);
    }

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ42
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
     /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ42 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_SERREJ42_ENUM,LNAS_ENTRY);

    if (NAS_RELEASE_R11)
    {
        /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
        /*设置EU3 ROAMING NOT ALLOWED*/
        NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

        /*删除GUTI,KSIasme,TAI list,L.V.R TAI*/
        NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);
        if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        {
            NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

            /* 记录ATTACH触发原因值 */
            NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
        }
        else
        {
            NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
        }
        /*转换EMM状态机MS_DEREG+EMM_SS_DEREG_LIMITED_SERVICE*/
        /*收到MMC搜网指令后再转入PLMN_SEARCH态*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

        /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
        if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
            && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
        {
            NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ,pMsgStru->ucEMMCause);
        }

        /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
        NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }
    else
    {
        NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
    }

    return;
}


#if (FEATURE_PTM == FEATURE_ON)
 
 VOS_VOID NAS_EMM_SerCnRejErrRecord(
                    NAS_EMM_CN_SER_REJ_STRU *pMsgStru,
                    EMM_OM_LMM_CSFB_FAIL_CAUSE_ENUM_UINT8 enCsfbFailCause,
                    EMM_OM_ERRLOG_TYPE_ENUM_UINT16   enErrlogType)
 {
      if(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
      {
          NAS_EMM_ExtServiceErrRecord(
                  pMsgStru->ucEMMCause,
                  enCsfbFailCause);
      }
      else
      {
          NAS_EMM_NorServiceErrRecord((VOS_VOID*)pMsgStru, enErrlogType);
      }
 }
#endif


VOS_VOID    NAS_EMM_SER_RcvServiceRejectMsg(NAS_EMM_CN_SER_REJ_STRU   *pMsgStru)
{
    VOS_UINT32                              ulIndex = 0;
    NAS_LMM_SER_REJ_CAUSE_PROC_FUN          pfRejCauseProcFun;

    NAS_EMM_SER_LOG_INFO( "Nas_Emm_Ser_RcvServiceRejectMsg is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_RcvServiceRejectMsg_ENUM,LNAS_ENTRY);

    /*异常停止 SER*/
    NAS_EMM_SER_AbnormalOver();

    NAS_EMM_SetCsfbProcedureFlag(PS_FALSE);

    /*设置SER的结果为 NAS_EMM_SER_RESULT_REJ*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_REJ);*/

    NAS_LMM_AdaptRegRejCau(&(pMsgStru->ucEMMCause));



    #if (FEATURE_PTM == FEATURE_ON)
    NAS_EMM_SerCnRejErrRecord((VOS_VOID*)pMsgStru, EMM_OM_LMM_CSFB_FAIL_CAUSE_CN_REJ, EMM_OM_ERRLOG_TYPE_CN_REJ);
    #endif

    for (ulIndex = 0; ulIndex < gstEmmSerRejCauseProcTblLen; ulIndex++)
    {
        if (pMsgStru->ucEMMCause == gstEmmSerRejCauseProcTbl[ulIndex].enRejCauseVal)
        {
            pfRejCauseProcFun = gstEmmSerRejCauseProcTbl[ulIndex].pfRejCauseProcFun;

            /* 有处理函数 */
            if (VOS_NULL_PTR != pfRejCauseProcFun)
            {
                (pfRejCauseProcFun)(pMsgStru);
            }
            return;
        }
    }

    /* 其他原因值的处理 */
    NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);

    return;

}


VOS_VOID NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm(VOS_VOID)
{
    NAS_LMM_PTL_TI_ENUM_UINT16  enPtlTimerId = NAS_LMM_STATE_TI_BUTT;

    if (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
    {
        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm:Upt-MM state.");
            TLPS_PRINT2LAYER_INFO(NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm_ENUM,LNAS_FUNCTION_LABEL1);

            /* 将状态转移至MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM状态 */
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /* 将状态转移至MS_REG + SS_REG_NORMAL_SERVICE状态 */
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);
        }

    }
    else
    {
        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm: with imsi attach");
            TLPS_PRINT2LAYER_INFO(NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm_ENUM,LNAS_FUNCTION_LABEL2);
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            NAS_EMM_SendIntraTauReq(    ID_APP_MM_MSG_TYPE_BUTT,
                                    NAS_LMM_INTRA_TAU_TYPE_UPDATE_MM);
            return;
        }

        NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm: normal service state.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm_ENUM,LNAS_FUNCTION_LABEL3);
        /* 将状态转移至MS_REG + SS_REG_NORMAL_SERVICE状态 */
        NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

    }
    return;

}


VOS_VOID NAS_EMM_TranStatePsNormalServiceOrPsLimitService(VOS_VOID)
{
    if (VOS_TRUE == NAS_EMM_IsEnterRegLimitService())
    {
        NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_LIMITED_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();
    }
}

VOS_VOID  NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd( VOS_UINT32 ulCause)
{
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd_ENUM,LNAS_ENTRY);


    /* CSFB流程的处理 */
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        if((VOS_TRUE == NAS_EMM_IsSupportSrlte()))
        {
            NAS_EMM_MsSerInitSsWaitCnSerCnfSrlteProcMsgRrcRelInd();
            return;
        }
        /*停止SERVICE流程*/
        NAS_EMM_SER_AbnormalOver();
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd:CSFB");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd_ENUM,LNAS_FUNCTION_LABEL1);

        /*转换EMM状态机MS_REG+SS_REG_NORMAL_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
        if ((LRRC_LNAS_REL_CAUSE_L2GU_REDIRECT != ulCause) && (LRRC_LNAS_REL_CAUSE_CSFB_HIGH_PRIOR != ulCause))
        {
            /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
            NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

            #if (FEATURE_PTM == FEATURE_ON)
            if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
            {
                NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_REL_OTHER);
            }
            #endif
        }

        /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
        NAS_EMM_CommProcConn2Ilde();
        return;
    }

    NAS_EMM_SER_AbnormalOver();

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd:CAUSE_ESM_DATA_REQ_EMC");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd_ENUM,LNAS_FUNCTION_LABEL2);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);

        /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
        NAS_EMM_CommProcConn2Ilde();
        return;
    }

    /*upon different cause value, enter different dealing*/
    switch (ulCause)
    {
        case LRRC_LNAS_REL_CAUSE_LOAD_BALANCE_REQ:
            /*NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_RRC_REL_LOAD_BALANCE);*/
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE);
            NAS_EMM_TAU_SaveEmmCollisionCtrl(  NAS_EMM_COLLISION_SERVICE);
            /*NAS_EMM_TAU_StartTAUREQ(             NAS_EMM_MSG_LOAD_BALANCING_TAU);*/
            /*
            NAS_EMM_TAUSER_FSMTranState(       EMM_MS_REG,
                                                EMM_SS_REG_PLMN_SEARCH,
                                                TI_NAS_EMM_STATE_NO_TIMER);
            */
            NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

            /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
            NAS_EMM_CommProcConn2Ilde();
            break;

        case LRRC_LNAS_REL_CAUSE_CONN_FAIL:
            /* NAS_EMM_TAU_SaveEmmTAUStartCause(    NAS_EMM_TAU_START_CAUSE_RRC_REL_CONN_FAILURE); */
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE);
            /*NAS_EMM_TAU_SaveEmmCollisionCtrl(    NAS_EMM_COLLISION_SERVICE);*/
            /*NAS_EMM_TAU_StartTAUREQ(             NAS_EMM_MSG_LOAD_BALANCING_TAU);*/
            /*
            NAS_EMM_TAUSER_FSMTranState(       EMM_MS_REG,
                                                EMM_SS_REG_PLMN_SEARCH,
                                                TI_NAS_EMM_STATE_NO_TIMER);
            */
            NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

            /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
            NAS_EMM_CommProcConn2Ilde();
            break;

        default:
            NAS_EMM_SER_RcvRrcRelInd();
            break;
    }

    return;
}


VOS_UINT32  NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthRej(
                                        VOS_UINT32  ulMsgId,
                                        VOS_VOID   *pMsgStru)

{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthRej enter!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthRej_ENUM,LNAS_ENTRY);

    /* 鉴权拒绝优化处理 */
    if (NAS_EMM_YES == NAS_EMM_IsNeedIgnoreAuthRej())
    {
        return  NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL);


    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgRrcRelInd(
                                                            VOS_UINT32  ulMsgId,
                                                            VOS_VOID   *pMsgStru)
{
    LRRC_LMM_REL_IND_STRU                 *pRrcRelInd     = (LRRC_LMM_REL_IND_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgRrcRelInd is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgRrcRelInd_ENUM,LNAS_ENTRY);

    /*获得原因值*/
    ulCause                                             =   pRrcRelInd->enRelCause;
    /* 大数据: 更新Ser Rel Fail计数 */
    NAS_LMM_AddSerCounter(NAS_LMM_OM_PCE_TIMES_TYPE_LRRC_REL);

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_EMM_NorServiceErrRecord((VOS_VOID*)pMsgStru, EMM_OM_ERRLOG_TYPE_LRRC_REL);
    #endif

    NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd(        ulCause);

    return NAS_LMM_MSG_HANDLED;
}

VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp(  VOS_UINT32  ulMsgId,
                                                             VOS_VOID   *pMsgStru
                                                          )
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp_ENUM,LNAS_ENTRY);

    /* 函数输入指针参数检查, 状态匹配检查,若不匹配,退出*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp ERROR !!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp_ENUM,LNAS_ERROR);
        return NAS_LMM_MSG_DISCARD;
    }
    /* 大数据: 更新Ser 定时器超时计数 */
    NAS_LMM_AddSerCounter(NAS_LMM_OM_PCE_TIMES_TYPE_EXP);
    /* 美国AT&T定制需求，对SERVICE失败场景计数的处理 */
    NAS_EMM_SER_CServiceCountProc();

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_EMM_NorServiceErrRecord((VOS_VOID*)pMsgStru, EMM_OM_ERRLOG_TYPE_TIMEOUT);
    #endif

    /*如果SR流程是由于SMS触发，需要回复SMS建链失败，并且清除SR的发起原因，*/
    if(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }

    #if (FEATURE_LPP == FEATURE_ON)
    if(NAS_EMM_SER_START_CAUSE_LPP_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_EMM_SendLppEstCnf(LMM_LPP_EST_RESULT_FAIL_SERVICE_FAIL);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }
    #endif

    #if (FEATURE_AGPS == FEATURE_ON)
    if(NAS_EMM_SER_START_CAUSE_SS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_LMM_SndLmmLcsEstCnf( LMM_LCS_EST_RSLT_FAIL_SERVICE_FAIL,
                                NAS_EMM_SER_GetSsOrLppOpid());
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }
    #endif

    /*清空ESM_DATA缓存*/
    NAS_EMM_SerClearEsmDataBuf();

    /* 美国AT&T定制需求，SERVICE超时需通知MMC失败次数 */
    NAS_EMM_MmcSendSerResultIndT3417Exp();

    NAS_EMM_SER_TbackoffProc();

    /*Inform RABM that SER fail*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp: CAUSE_ESM_DATA_REQ_EMC.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    }
    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return NAS_LMM_MSG_HANDLED;

}

VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417ExtExp
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417ExtExp is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417ExtExp_ENUM,LNAS_ENTRY);

    /* 函数输入指针参数检查, 状态匹配检查,若不匹配,退出*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417ExtExp_ENUM,LNAS_ERROR);
        return NAS_LMM_MSG_DISCARD;
    }

    if(VOS_TRUE == NAS_EMM_IsSupportSrlte())
    {
        NAS_EMM_MsSerInitSsWaitCnSerCnfMsgSrlteTimer3417ExtExp();
        return NAS_LMM_MSG_HANDLED;
    }


    NAS_EMM_SER_AbnormalOver();

    #if (FEATURE_PTM == FEATURE_ON)
        if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        {
            NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_EXT3417_EXP);
        }
    #endif

    /* 状态迁移到REG.NORMAL_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                            EMM_SS_REG_NORMAL_SERVICE,
                            TI_NAS_EMM_STATE_NO_TIMER);

    if (NAS_EMM_CSFB_ABORT_FLAG_VALID == NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {

        if (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
        {
            /* 如果有未完成的TAU，等在REG+NORMAL_SERVICE状态下收到系统消息后再处理 */
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        }
    }
    /* 如果不是紧急CSFB和MO CSFB给MM发终止消息,否则给MMC上报SER结果释放等挂起 */
    else if ((VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb())
         && (VOS_FALSE == NAS_EMM_SER_IsMoCsfbProcedure()))
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_NULL);

        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_T3417EXT_TIME_OUT, NAS_LMM_CAUSE_NULL);
        if (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
        {
            /* 如果有未完成的TAU，等在REG+NORMAL_SERVICE状态下收到系统消息后再处理 */
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        }
    }
    else
    {
        /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
        /* NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);*/

        /* 美国AT&T定制需求修改接口，EXT SERVICE超时需通知MMC超时时失败触发搜网 */
        NAS_EMM_MmcSendSerResultIndT3417Exp();

        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32 NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject(  VOS_UINT32  ulMsgId,
                                                                  VOS_VOID   *pMsgStru
                                                              )
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;
    NAS_EMM_CN_SER_REJ_STRU         *pstserrej      = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject_ENUM,LNAS_ENTRY);

    /* 函数输入指针参数检查, 状态匹配检查,若不匹配,退出*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);

    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject_ENUM,LNAS_ERROR);
        return NAS_LMM_MSG_DISCARD;
    }
    /* 大数据: 更新Ser Rej计数 */
    NAS_LMM_AddSerCounter(NAS_LMM_OM_PCE_TIMES_TYPE_CN_REJ);
    /*停止T3416，删除RAND,RES*/
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    /*调用消息处理函数*/
    pstserrej  =  (NAS_EMM_CN_SER_REJ_STRU*)pMsgStru;
    NAS_EMM_SER_RcvServiceRejectMsg(pstserrej);

    return NAS_LMM_MSG_HANDLED;
}


VOS_VOID NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej(VOS_UINT32  ulCause)
{
    NAS_EMM_SER_LOG_INFO(              "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej_ENUM,LNAS_ENTRY);

    (VOS_VOID)ulCause;

    /*终止SER*/
    NAS_EMM_SER_AbnormalOver();
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    /* 如果是CSFB流程，且不是紧急CSFB，则给MM发终止消息 */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_SetCsfbProcedureFlag(PS_FALSE);

        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_AUTH_REJ, NAS_LMM_CAUSE_NULL);
    }

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    /*USIM无效，直到关机或拔卡*/
    /*暂不处理*/
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG == NAS_EMM_GetCurrentCellType())
    {
        /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
#endif
    {
        NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);
        /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI, TI_NAS_EMM_STATE_NO_TIMER);

    }

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_AUTH_REJ);

#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
    {
        /*向LRRC发送LRRC_LMM_NAS_INFO_CHANGE_REQ携带USIM卡状态*/
        NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);
    }

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);


    return;
}
/*lint +e961*/
/*lint +e960*/

VOS_VOID NAS_EMM_SerbarCommProc(VOS_VOID)
{
    /*停止SERVICE流程*/
    NAS_EMM_SER_AbnormalOver();
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND, TI_NAS_EMM_STATE_NO_TIMER);

    /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_ACCESS_BARED);

    /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
    NAS_EMM_CommProcConn2Ilde();

    #if (FEATURE_PTM == FEATURE_ON)
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_EST_ACCESS_BAR);
    }
    #endif


    return;
 }



VOS_VOID  NAS_EMM_SER_ProcMoCallAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCallAccessBar is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_ProcMoCallAccessBar_ENUM,LNAS_ENTRY);
    if((NAS_EMM_SER_START_CAUSE_RRC_PAGING == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
       /*响应寻呼的SERVICE流程不出现RRC_EST_ACCESS_BARRED_MO_CALL的情况*/
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcMoCallAccessBar:Barred Type ERR!!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_SER_ProcMoCallAccessBar_ENUM,LNAS_ERROR);
        return;
    }

    NAS_EMM_SerbarCommProc();

    return;
}


VOS_VOID  NAS_EMM_SER_ProcBarAllAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcBarAllAccessBar is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_ProcBarAllAccessBar_ENUM,LNAS_ENTRY);
    if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcBarAllAccessBar:emergency Barred Type ERR!!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_SER_ProcBarAllAccessBar_ENUM,LNAS_ERROR);
        return;
    }

    NAS_EMM_SerbarCommProc();

    return;
}


VOS_VOID  NAS_EMM_SER_ProcMoCsfbAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCsfbAccessBar is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_ProcMoCsfbAccessBar_ENUM,LNAS_ENTRY);
    if(NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        /*停止SERVICE流程*/
        NAS_EMM_SER_AbnormalOver();

        /*不需给MMC上报结果，继续在L模，解bar后重新触发CSFB流程*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND, TI_NAS_EMM_STATE_NO_TIMER);

        /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
        NAS_EMM_CommProcConn2Ilde();
    }
    return;
}


VOS_VOID  NAS_EMM_SER_ProcMoCallAndCsfbAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCallAndCsfbAccessBar is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_ProcMoCallAndCsfbAccessBar_ENUM,LNAS_ENTRY);
    if((NAS_EMM_SER_START_CAUSE_RRC_PAGING == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
        /*响应寻呼的SERVICE流程不出现RRC_EST_ACCESS_BARRED_MO_CALL_AND_CSFB的情况*/
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcMoCallAndCsfbAccessBar:mo call and csfb Barred Type ERR!!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_SER_ProcMoCallAndCsfbAccessBar_ENUM,LNAS_ERROR);
        return;
    }
    NAS_EMM_SerbarCommProc();

    return;
}


VOS_VOID  NAS_EMM_SER_ProcCellSearchFail(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcCellSearchFail is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SER_ProcCellSearchFail_ENUM,LNAS_ENTRY);

    /* ERABM收到ID_EMM_ERABM_REEST_IND结果失败或者ID_EMM_ERABM_RRC_CON_REL_IND时，会立刻停止1s定时器，若此时有上行数据时，
       ERABM会CDS通知再次触发LMM发起建链，
       而LRRC cell search时间可能会很长，这样会导致乒乓效应(ERABM一直触发LMM发起建链)，导致其他低优先级任务调度不到
       所以此处:ser建链失败原因为LRRC_EST_CELL_SEARCHING时，不给ERABM发送ID_EMM_ERABM_REEST_IND和ID_EMM_ERABM_RRC_CON_REL_IND消息 */

    /*停止T3417定时器*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*停止T3440定时器*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    /*停止T3417ext定时器*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*如果SR流程是由于SMS触发，需要回复SMS建链失败，并且清除SR的发起原因，*/
    if(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        /* SER异常的原因值上报暂时报LMM_SMS_ERR_CAUSE_OTHERS，
           以后可能要根据相应的原因值进行具体细分处理，上报准确的原因值
           此处作为遗留问题 */
        NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }

    #if (FEATURE_LPP == FEATURE_ON)
    if(NAS_EMM_SER_START_CAUSE_LPP_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_EMM_SendLppEstCnf(LMM_LPP_EST_RESULT_FAIL_SERVICE_FAIL);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }
    #endif

    #if (FEATURE_AGPS == FEATURE_ON)
    if(NAS_EMM_SER_START_CAUSE_SS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_LMM_SndLmmLcsEstCnf( LMM_LCS_EST_RSLT_FAIL_SERVICE_FAIL,
                                NAS_EMM_SER_GetSsOrLppOpid());
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }
    #endif

    /*清空ESM_DATA缓存*/
    NAS_EMM_SerClearEsmDataBuf();

    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        if( VOS_FALSE == NAS_EMM_IsSupportSrlte())
        {
            /* 在UE搜网过程中发起CSFB，ext_service发出之后收到LRRC_LMM_EST_CNF,原因值为RRC_EST_CELL_SEARCHING，
            增加延时Timer，不再发送MMC_LMM_SERVICE_RSLT_FAILURE;begin*/
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_CSFB_DELAY);
        }
        else
        {

            /* 给MMC上报SERVICE失败 */
            NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

            NAS_EMM_SER_SaveXccCallFlg(NAS_EMM_XCC_CALL_FLAG_VALID);

            NAS_EMM_ERABM_SendCallStartInd();

            NAS_EMM_XCC_SendEsrEndInd(LMM_XCC_ESR_RSLT_SEND_FAIL);
        }
    }

    NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

    /*如果当前连接状态不是IDLE，则向 MMC上报*/
    if(NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        NAS_EMM_SendMmcStatusInd(MMC_LMM_STATUS_TYPE_CONN_STATE,
                                 MMC_LMM_CONN_IDLE);
    }

    /* 把RRC连接状态设置为IDLE */
    NAS_EMM_SetConnState(NAS_EMM_CONN_IDLE);

    NAS_EMM_ClrAllUlDataReqBufferMsg();


    return;
}


VOS_VOID  NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailWithEmc
(
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU      *pMrrcConnectFailRelInd
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg               = NAS_EMM_NULL_PTR;

    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailWithEmc: enter");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailWithEmc_ENUM, LNAS_ENTRY);

    /*停止SERVICE流程*/
    NAS_EMM_SER_AbnormalOver();

    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

    /* 本地DETACH释放资源:动态内存、赋初值 */
    NAS_LMM_DeregReleaseResource();

    /*向MMC发送本地LMM_MMC_DETACH_IND消息*/
    NAS_EMM_MmcSendDetIndLocal( MMC_LMM_L_LOCAL_DETACH_OTHERS);

#if (FEATURE_PTM == FEATURE_ON)
    NAS_EMM_LocalDetachErrRecord(EMM_ERR_LOG_LOCAL_DETACH_TYPE_OTHER);
#endif
    NAS_EMM_CommProcConn2Ilde();

    if (LRRC_EST_CELL_SEARCHING == pMrrcConnectFailRelInd->enEstResult)
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_AdStateConvert( EMM_MS_DEREG,
                                EMM_SS_DEREG_PLMN_SEARCH,
                                TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        /*保存ESM消息*/
        /*lint -e960*/
        NAS_LMM_MEM_CPY_S(  (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                            sizeof(NAS_EMM_CN_MSGCON_STRU),
                            &(pstEsmMsg->stEsmMsg),
                            (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);
        /*lint +e960*/
        NAS_EMM_StartEmergencyAttach();
    }

    return;
}


VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd(   VOS_UINT32  ulMsgId,
                                                         VOS_VOID   *pMsgStru)
{

    VOS_UINT32                          ulRslt;
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *pMrrcConnectFailRelInd       = NAS_EMM_NULL_PTR;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd_ENUM,LNAS_ENTRY);
    pMrrcConnectFailRelInd              = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)pMsgStru;

    /* 函数输入指针参数检查, 状态匹配检查,若不匹配,退出*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd ERROR !!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd_ENUM,LNAS_ERROR);
        return NAS_LMM_MSG_DISCARD;
    }

    /* 大数据: 根据ESM FAIL原因值不同进行更新计数 */
    NAS_LMM_UpdateSerEstFailCounter(pMrrcConnectFailRelInd->enEstResult);
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailWithEmc(pMrrcConnectFailRelInd);

        return  NAS_LMM_MSG_HANDLED;
    }

    /*Process according different EST result*/
    switch(pMrrcConnectFailRelInd->enEstResult)
    {
        case    LRRC_EST_ACCESS_BARRED_MO_CALL:

                /*设置bar流程和类型*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCallAccessBar();

                break;
        case    LRRC_EST_ACCESS_BARRED_ALL:

                /*设置bar流程和类型*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcBarAllAccessBar();
                break;

        case    LRRC_EST_ACCESS_BARRED_MO_CSFB:
                /*设置bar流程和类型*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCsfbAccessBar();
                break;

        case    LRRC_EST_ACCESS_BARRED_MO_CALL_AND_CSFB:

                /*设置bar流程和类型*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCallAndCsfbAccessBar();
                break;
                /* 建链失败，RRC正在进行小区搜索，迁移到Reg.Plmn_Search*/
        case    LRRC_EST_CELL_SEARCHING:

                NAS_EMM_SER_ProcCellSearchFail();
                break;

        case    LRRC_EST_EST_CONN_FAIL:

                if((VOS_TRUE == NAS_EMM_IsSupportSrlte())
                    &&(PS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
                {
                    NAS_EMM_SER_SaveXccEsrFlg(NAS_EMM_XCC_ESR_FLAG_VALID);
                }
                /*停止SERVICE流程*/
                NAS_EMM_SER_AbnormalOver();
                NAS_EMM_SER_RcvRrcRelInd();


                #if (FEATURE_PTM == FEATURE_ON)
                if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
                {
                    NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_EST_FAIL);
                }
                #endif

                break;

        case    LRRC_EST_ACCESS_BARRED_IMS_EMC_CALL:
        case    LRRC_EST_NOT_SUPPORT_IMS_EMC_CALL:
                /* 给IMSA发送通知 */
                NAS_EMM_SndImsaEmcEstFailInd(pMrrcConnectFailRelInd->enEstResult);
                /*停止SERVICE流程*/
                NAS_EMM_SER_AbnormalOver();
                NAS_EMM_SER_RcvRrcRelInd();

                #if (FEATURE_PTM == FEATURE_ON)
                if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
                {
                    NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_EST_FAIL);
                }
                #endif
                break;

        default:
                /*停止SERVICE流程*/
                NAS_EMM_SER_AbnormalOver();
                NAS_EMM_SER_RcvRrcRelInd();

                #if (FEATURE_PTM == FEATURE_ON)
                if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
                {
                    NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_EST_FAIL);
                }
                #endif

                break;

    }
    return  NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_SndServiceReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{

    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    /*检查状态是否匹配，若不匹配，退出*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF))
    {
        /*打印出错信息*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndServiceReqFailProc: STATE ERR!");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SndServiceReqFailProc_ENUM,LNAS_ERROR);
        return  NAS_EMM_SUCC;
    }
    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
        return NAS_EMM_FAIL;
    }
    /* 大数据: 更新Ser REQ Snd Fail计数 */
    NAS_LMM_AddSerCounter(NAS_LMM_OM_PCE_TIMES_TYPE_DATA_CNF_FAIL);
    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:

            /*  需要将对应的缓存消息的ulHoWaitSysInfoFlag设置为true */
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_TRUE;
            *pulIsDelBuff = VOS_FALSE;

            break;
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            /*停止T3417定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*停止T3440定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            /*停止T3417ext定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*启动定时器3417*/
            NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*转换EMM状态机MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

            /*组合并发送MRRC_DATA_REQ(SERVICE_REQ)*/
            NAS_EMM_SER_SendMrrcDataReq_ServiceReq();

            break;

        default:
            break;
        }

    return NAS_EMM_SUCC;
}

VOS_UINT32 NAS_EMM_SndExtendedServiceReqSuccProc(VOS_VOID* pMsg)
{
    /*问题背景:主叫走CSFB流程被用户快速挂断电话，此时走CSFB回退流程，
    回退到L的时候，由于TA在TALIST里面，所以不会发起TAU跟网侧交互，但是
    此时核心网PS域已经开始往2/3G迁移，这样会导致被叫不通，或者收不到短信
    改动:增加标识维护识别上面的这种场景，在回到L的时候保证发起TAU*/

    NAS_EMM_SetCsfbProcedureFlag(PS_TRUE);

    /* ESR空口发送成功(非REJ BY UE),通知LRRC CSFB流程启动,LRRC收到该消息后会启动定时器,当定时器超时之前未收到网侧释放
       消息,LRRC主动释放,用于解决CSFB流程网侧不下发重定向释放的问题*/
    if((NAS_EMM_CSFB_RSP_REJECTED_BY_UE == NAS_EMM_SER_GetEmmSerCsfbRsp())
        &&(NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqSuccProc CSFB Rej By Ue");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_SndExtendedServiceReqSuccProc_ENUM, LNAS_FUNCTION_LABEL1);
        return NAS_EMM_SUCC;
    }
    /* 记录发起过ESR标识，用于后续收到CALL END时发起TAU的判断 */

    if(VOS_TRUE == NAS_EMM_IsSupportSrlte())
    {
        NAS_EMM_SER_SaveXccEsrFlg(NAS_EMM_XCC_ESR_FLAG_VALID);
    }

    NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_START);
    return NAS_EMM_SUCC;
}


VOS_UINT32 NAS_EMM_SndExtendedServiceReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;


    *pulIsDelBuff = VOS_TRUE;

    /*检查状态是否匹配，若不匹配，退出*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF))
    {
        /*打印出错信息*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqFailProc: STATE ERR!");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SndExtendedServiceReqFailProc_ENUM,LNAS_ERROR);
        return  NAS_EMM_SUCC;
    }

    if (NAS_EMM_CSFB_ABORT_FLAG_VALID == NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {
        /*打印出错信息*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqFailProc: ABORT!");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_SndExtendedServiceReqFailProc_ENUM,LNAS_FUNCTION_LABEL1);
        return  NAS_EMM_SUCC;
    }

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
        return NAS_EMM_FAIL;
    }

    if((VOS_TRUE == NAS_EMM_IsSupportSrlte())
        &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
    {
        return  NAS_EMM_SndSrlteExtendedServiceReqFailProc();
    }

    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:

            /*  需要将对应的缓存消息的ulHoWaitSysInfoFlag设置为true */
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_TRUE;
            *pulIsDelBuff = VOS_FALSE;

            break;
        #if (FEATURE_ON == FEATURE_DSDS)
        /* 发送上行消息时,LRRC会检查RF资源状态,若RF资源不可用LRRC将报该原因值,标志ulNoRfWaitSysInfoFlag用于判断之后LMM收到系统消息后重传该空口。
           当前实现接下来LRRC必然会给LMM上报REL_IND或者SYSTEM_INFO_IND消息*/
        case LRRC_LMM_SEND_RSLT_NO_RF:
            pEmmMrrcMgmtData->ulNoRfWaitSysInfoFlag = VOS_TRUE;
            *pulIsDelBuff = VOS_FALSE;

            break;
        #endif
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            /* 重启SERVICE 流程 */
            /*停止T3417定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*停止T3440定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            /*停止T3417ext定时器*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*启动定时器3417*/
            NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*转换EMM状态机MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*组合并发送MRRC_DATA_REQ(SERVICE_REQ)*/
            NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq();

            break;

        default:
            /*问题背景:主叫走CSFB流程被用户快速挂断电话，此时走CSFB回退流程，
            回退到L的时候，由于TA在TALIST里面，所以不会发起TAU跟网侧交互，但是
            此时核心网PS域已经开始往2/3G迁移，这样会导致被叫不通，或者收不到短信
            改动:增加标识维护识别上面的这种场景，在回到L的时候保证发起TAU*/
            if(PS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
            {
                NAS_EMM_SetCsfbProcedureFlag(PS_TRUE);
            }
            break;
        }

    return NAS_EMM_SUCC;
}



VOS_VOID NAS_EMM_MsSerInitSsWaitCnSerCnfMsgSrlteTimer3417ExtExp( VOS_VOID)
{

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgSrlteTimer3417ExtExp entry");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfMsgSrlteTimer3417ExtExp_ENUM,LNAS_ENTRY);
    if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
    {
        /* 状态迁移到ATTEMPING_TO_UPDATE_MM*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        /* 将状态转移至MS_REG + SS_REG_NORMAL_SERVICE状态 */
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_NORMAL_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }

    #if (FEATURE_PTM == FEATURE_ON)
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_EXT3417_EXP);
    }
    #endif

    NAS_EMM_SER_SaveXccCallFlg(NAS_EMM_XCC_CALL_FLAG_VALID);

    NAS_EMM_ERABM_SendCallStartInd();

    NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_END);

    NAS_EMM_XCC_SendEsrEndInd(LMM_XCC_ESR_RSLT_TIME_EXP);

    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_NULL);

    NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_T3417EXT_TIME_OUT, NAS_LMM_CAUSE_NULL);

    if (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    return;

}

VOS_VOID NAS_EMM_MsSerInitSsWaitCnSerCnfSrlteProcMsgRrcRelInd(VOS_VOID)
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfSrlteProcMsgRrcRelInd entry");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsSerInitSsWaitCnSerCnfSrlteProcMsgRrcRelInd_ENUM,LNAS_ENTRY);

    if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
    {
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        /* 将状态转移至MS_REG + SS_REG_NORMAL_SERVICE状态 */
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_NORMAL_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }

    /*停止T3417ext定时器*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*ESR已经发送成功了*/
    if(NAS_EMM_XCC_ESR_FLAG_VALID == NAS_EMM_SER_GetXccEsrFlg())
    {
        NAS_EMM_XCC_SendEsrEndInd(LMM_XCC_ESR_RSLT_SUCC);
    }
    /*ESR没有发送成功就收到了REL*/
    else
    {
        /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

        NAS_EMM_XCC_SendEsrEndInd(LMM_XCC_ESR_RSLT_SEND_FAIL);

        #if (FEATURE_PTM == FEATURE_ON)
        NAS_EMM_ExtServiceErrRecord(EMM_OM_ERRLOG_CN_CAUSE_NULL, EMM_OM_LMM_CSFB_FAIL_CAUSE_RRC_REL_OTHER);
        #endif
    }

    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_NULL);

    NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_END);

    NAS_EMM_SER_SaveXccCallFlg(NAS_EMM_XCC_CALL_FLAG_VALID);

    NAS_EMM_ERABM_SendCallStartInd();

    /*send INTRA_CONN2IDLE_REQ，更新连接状态*/
    NAS_EMM_CommProcConn2Ilde();
    return;
}

VOS_UINT32 NAS_EMM_SndSrlteExtendedServiceReqFailProc(VOS_VOID)
{

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SndSrlteExtendedServiceReqFailProc entry");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_SndSrlteExtendedServiceReqFailProc_ENUM,LNAS_ENTRY);

    if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
    {
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        /* 将状态转移至MS_REG + SS_REG_NORMAL_SERVICE状态 */
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_NORMAL_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /* 记录发起过ESR标识，用于后续收到CALL END时发起TAU的判断 */
    NAS_EMM_SER_SaveXccEsrFlg(NAS_EMM_XCC_ESR_FLAG_INVALID);

    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_NULL);

    NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_END);

    NAS_EMM_SER_SaveXccCallFlg(NAS_EMM_XCC_CALL_FLAG_VALID);

    NAS_EMM_ERABM_SendCallStartInd();

    NAS_EMM_XCC_SendEsrEndInd(LMM_XCC_ESR_RSLT_SEND_FAIL);
    return NAS_EMM_SUCC;
}



VOS_VOID NAS_EMM_SndImsaEmcEstFailInd(LRRC_LNAS_EST_RESULT_ENUM_UINT32 enEstResult)
{
    LMM_IMSA_EMC_EST_FAIL_IND_STRU     *pstEmcEstFailInd;


    /*申请消息内存*/
    pstEmcEstFailInd = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LMM_IMSA_EMC_EST_FAIL_IND_STRU));

    /*判断申请结果，若失败退出*/
    if (NAS_EMM_NULL_PTR == pstEmcEstFailInd)
    {
        return;
    }

    /*构造LMM_IMSA_EMC_EST_FAIL_IND消息*/
    /*填充消息头*/
    NAS_EMM_COMP_AD_IMSA_MSG_HEADER(    pstEmcEstFailInd,
                                        (sizeof(LMM_IMSA_EMC_EST_FAIL_IND_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstEmcEstFailInd->ulMsgName   = ID_LMM_IMSA_EMC_EST_FAIL_IND;

    /*建链失败原因*/
    pstEmcEstFailInd->enEstResult = enEstResult;

    /*向IMSA发送ID_LMM_IMSA_ATTACH_BEGIN_IND消息*/
    NAS_LMM_SEND_MSG(pstEmcEstFailInd);
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


