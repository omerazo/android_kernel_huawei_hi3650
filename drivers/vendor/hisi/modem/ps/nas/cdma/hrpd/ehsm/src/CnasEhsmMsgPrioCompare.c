

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "CnasEhsmMsgPrioCompare.h"
#include  "NasFsm.h"
#include  "CnasEhsmSndInternalMsg.h"
#include  "CnasCcb.h"
#include  "ehsm_hsd_pif.h"
#include  "CnasEhsmSndAps.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_CNAS_EHSM_MSG_PRIO_COMPARE_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

CNAS_EHSM_MSG_COMPARE_STRU g_astEhsmMsgPrioCompareWithActivatingTbl[] =
{
    /* Activating�������յ�HSD�Ĺػ��������Ϣ���ȼ��Ƚ� */
    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSD, ID_HSD_EHSM_POWER_OFF_REQ),
                               CNAS_EHSM_CompareHsdPowerOffReqPrioWithActivating),

    /* Activating�������յ�HSD�Ĺػ��������Ϣ���ȼ��Ƚ� */
    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSM, ID_HSM_EHSM_SUSPEND_IND),
                               CNAS_EHSM_CompareHsmSuspendIndPrioWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_DETACH_IND),
                               CNAS_EHSM_CompareCttfEhrpdDetachIndPrioWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ),
                               CNAS_EHSM_CompareTafPdnDeactivateReqWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ),
                               CNAS_EHSM_CompareTafPdnReconnReqWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ),
                               CNAS_EHSM_CompareTafPdnActivateReqWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_LOC_DEACTIVATE_NOTIFY),
                               CNAS_EHSM_CompareTafLocalDeactivateNotifyWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSM, ID_HSM_EHSM_SESSION_INFO_IND),
                               CNAS_EHSM_CompareHsmSessionInfoIndPrioWithActivating),
    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(PS_PID_ESM, ID_ESM_EHSM_CLEAR_ALL_BEARER_NOTIFY),
                               CNAS_EHSM_CompareEsmClearAllBearerNotifyWithActivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_PDN_DISC_IND),
                               CNAS_EHSM_CompareCttfEhrpdDiscIndPrioWithActivating),
};


CNAS_EHSM_MSG_COMPARE_STRU g_astEhsmMsgPrioCompareWithDeactivatingTbl[] =
{
    /* Deactivating�������յ�HSD�Ĺػ��������Ϣ���ȼ��Ƚ� */
    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSD, ID_HSD_EHSM_POWER_OFF_REQ),
                               CNAS_EHSM_CompareHsdPowerOffReqPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSM, ID_HSM_EHSM_SUSPEND_IND),
                               CNAS_EHSM_CompareHsmSuspendIndPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_DETACH_IND),
                               CNAS_EHSM_CompareCttfEhrpdDetachIndPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ),
                               CNAS_EHSM_CompareTafPdnActivateReqPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ),
                               CNAS_EHSM_CompareTafPdnDeactivateReqPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ),
                               CNAS_EHSM_CompareTafReconnReqPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_LOC_DEACTIVATE_NOTIFY),
                               CNAS_EHSM_CompareTafLocalDeactivateNotifyWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(UEPS_PID_HSM, ID_HSM_EHSM_SESSION_INFO_IND),
                               CNAS_EHSM_CompareHsmSessionInfoIndPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_DISC_NOTIFY),
                               CNAS_EHSM_CompareTafDiscNotifyPrioWithDeactivating),

    CNAS_EHSM_COMPARE_TBL_ITEM(CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_PDN_DISC_IND),
                           CNAS_EHSM_CompareCttfEhrpdDiscIndPrioWithDeactivating),

};




CNAS_EHSM_FSM_MSG_COMPARE_STRU g_astEhsmMsgCompareTbl[] =
{
    /* ACTIVATING ״̬���ȽϺ�����*/
    CNAS_EHSM_FSM_COMPARE_TBL_ITEM(CNAS_EHSM_FSM_ACTIVATING,
                                 g_astEhsmMsgPrioCompareWithActivatingTbl),

    /* DEACTIVATING ״̬���ȽϺ�����*/
    CNAS_EHSM_FSM_COMPARE_TBL_ITEM(CNAS_EHSM_FSM_DEACTIVATING,
                                 g_astEhsmMsgPrioCompareWithDeactivatingTbl),
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_UINT32 CNAS_EHSM_CompareHsdPowerOffReqPrioWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    CNAS_EHSM_StartTimer(TI_CNAS_EHSM_PROTECTING_POWER_OFF_PROCESS, TI_CNAS_EHSM_PROTECTING_POWER_OFF_PROCESS_LEN);

    CNAS_EHSM_ClearAllCacheMsg();

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}



VOS_UINT32 CNAS_EHSM_CompareHsmSuspendIndPrioWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_FSM_IMMEDIATELY;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}


VOS_UINT32 CNAS_EHSM_CompareCttfEhrpdDetachIndPrioWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_INT32                               i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    REL_TIMER_MSG                          *pstTimerMsg;
    VOS_UINT32                              ulCacheMsgEventType;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    /* ���֮ǰ����Ĳ���Ҫ��������Ϣ */
    for (i = (pstMsgQueue->ucCacheMsgNum - 1); i >= 0; i--)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if ((CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ)  == ulCacheMsgEventType)
         || (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ)          == ulCacheMsgEventType)
         || (CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_PDN_DISC_IND) == ulCacheMsgEventType))
        {
            CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
        }
    }

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}


VOS_UINT32 CNAS_EHSM_CompareCttfEhrpdDiscIndPrioWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_FSM_DELAY;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 CNAS_EHSM_CompareTafPdnDeactivateReqWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_UINT32                          i;
    VOS_UINT8                           ucCid;
    APS_EHSM_PDN_DEACTIVATE_REQ_STRU   *pstPdnDeactReq;
    APS_EHSM_PDN_ACTIVATE_REQ_STRU     *pstPdnActReq;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU     *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                    *pstCacheMsgHdr;
    REL_TIMER_MSG                      *pstTimerMsg;
    VOS_UINT32                          ulCacheMsgEventType;

   *penAbortType   = CNAS_EHSM_ABORT_BUTT;
    pstPdnDeactReq = (APS_EHSM_PDN_DEACTIVATE_REQ_STRU*)pstMsg;
    ucCid          = CNAS_EHSM_GetCid_Activating();
    pstMsgQueue    = CNAS_EHSM_GetCacheMsgAddr();

    /* ���֮ǰ����Ĳ���Ҫ��������Ϣ */
    for (i = 0; i < pstMsgQueue->ucCacheMsgNum; i++)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ)  == ulCacheMsgEventType)
        {
            pstPdnActReq = (APS_EHSM_PDN_ACTIVATE_REQ_STRU *)pstCacheMsgHdr;

            /* ���֮ǰ��������ͬCID�ļ���������Ϣ�����������û�����Ϣ */
            if (pstPdnActReq->ucCid == pstPdnDeactReq->ucCid)
            {
                /* ȥ����CNF�ظ�ʱ��δ����PDNID, UcCid������д��ȷ */
                CNAS_EHSM_SndApsPdnDeactivateCnf(CNAS_EHSM_INVALID_PDN_ID, pstPdnDeactReq->ucCid, EHSM_APS_CAUSE_SUCCESS);

                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);

                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }
    }

    if (ucCid == pstPdnDeactReq->ucCid)
    {
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
    }


    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 CNAS_EHSM_CompareTafPdnReconnReqWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{

    VOS_UINT32                              i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    REL_TIMER_MSG                          *pstTimerMsg;
    VOS_UINT32                              ulCacheMsgEventType;
    APS_EHSM_RECONN_REQ_STRU               *pstReconnReq;
    APS_EHSM_RECONN_REQ_STRU               *pstReconnCah;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    *penAbortType   = CNAS_EHSM_ABORT_FSM_DELAY;

    /* ֮ǰ�Ѿ������˴������Ϣ������Ҫ�ظ����� */
    for (i = 0; i < pstMsgQueue->ucCacheMsgNum; i++)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ) == ulCacheMsgEventType)
        {
            pstReconnReq = (APS_EHSM_RECONN_REQ_STRU *)pstMsg;
            pstReconnCah = (APS_EHSM_RECONN_REQ_STRU *)pstCacheMsgHdr;

            /* ����Ѿ�����ͬ��PDN�����ˣ����������� */
            if (pstReconnReq->ucPdnId == pstReconnCah->ucPdnId)
            {
                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }
    }

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;


}


VOS_UINT32 CNAS_EHSM_CompareTafPdnActivateReqWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}




VOS_UINT32 CNAS_EHSM_CompareTafLocalDeactivateNotifyWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_INT32                               i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    APS_EHSM_PDN_ACTIVATE_REQ_STRU         *pstPdnAct;
    APS_EHSM_PDN_DEACTIVATE_REQ_STRU       *pstPdnDeact;
    APS_EHSM_LOC_DEACTIVATE_NOTIFY_STRU    *pstLocPdnDeact;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    REL_TIMER_MSG                          *pstTimerMsg;
    VOS_UINT32                              ulCacheMsgEventType;

    APS_EHSM_RECONN_REQ_STRU               *pstReconnCah;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    pstLocPdnDeact  = (APS_EHSM_LOC_DEACTIVATE_NOTIFY_STRU*)pstMsg;
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    /* �鿴��������û����ͬCID�Ĳ������еĻ���������� */
    for  (i = (pstMsgQueue->ucCacheMsgNum - 1); i >= 0; i--)
    {
        pstCacheMsgHdr          = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ) == ulCacheMsgEventType)
        {
            pstPdnAct = (APS_EHSM_PDN_ACTIVATE_REQ_STRU*)pstCacheMsgHdr;
            if (pstLocPdnDeact->ucCid == pstPdnAct->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ) == ulCacheMsgEventType)
        {
            pstPdnDeact = (APS_EHSM_PDN_DEACTIVATE_REQ_STRU*)pstCacheMsgHdr;
            if (pstLocPdnDeact->ucCid == pstPdnDeact->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
            }
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ) == ulCacheMsgEventType)
        {
            pstReconnCah = (APS_EHSM_RECONN_REQ_STRU *)pstCacheMsgHdr;

            /* ����Ѿ�����ͬ��PDN�����ˣ����������� */
            if (pstLocPdnDeact->ucCid == pstReconnCah->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
            }
        }
    }

    if (pstLocPdnDeact->ucCid == CNAS_EHSM_GetCid_Activating())
    {
        /* ֱ�Ӵ�ϵ�ǰ�������� */
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
    }
    else if (CNAS_EHSM_GetLteActivePdnNum() > 1)
    {
        /* ���EHSM�����LTE��PDN��Ϣ��������Ч��HandOver���� */
        CNAS_EHSM_ClearLteLocalPdnInfo(pstLocPdnDeact->ucCid);

        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }
    else
    {
        /* Ϊ�˱�����©�������������Ϣ */
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
    }
}


VOS_UINT32 CNAS_EHSM_CompareEsmClearAllBearerNotifyWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}


VOS_UINT32 CNAS_EHSM_CompareHsdPowerOffReqPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    CNAS_EHSM_StartTimer(TI_CNAS_EHSM_PROTECTING_POWER_OFF_PROCESS, TI_CNAS_EHSM_PROTECTING_POWER_OFF_PROCESS_LEN);

    CNAS_EHSM_ClearAllCacheMsg();

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}


VOS_UINT32 CNAS_EHSM_CompareHsmSuspendIndPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_FSM_IMMEDIATELY;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}



VOS_UINT32 CNAS_EHSM_CompareCttfEhrpdDetachIndPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_INT32                               i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    REL_TIMER_MSG                          *pstTimerMsg;
    VOS_UINT32                              ulCacheMsgEventType;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    /* ���֮ǰ����Ĳ���Ҫ��������Ϣ */
    for (i = (pstMsgQueue->ucCacheMsgNum - 1); i >= 0; i--)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if ((CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ)  == ulCacheMsgEventType)
         || (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ)          == ulCacheMsgEventType)
         || (CNAS_BuildEventType(MSPS_PID_PPPC, ID_CTTF_CNAS_EHRPD_PDN_DISC_IND) == ulCacheMsgEventType))
        {
            CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
        }
    }

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
}


VOS_UINT32 CNAS_EHSM_CompareCttfEhrpdDiscIndPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_FSM_DELAY;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 CNAS_EHSM_CompareTafPdnActivateReqPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 CNAS_EHSM_CompareTafPdnDeactivateReqPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_UINT32                          i;
    APS_EHSM_PDN_DEACTIVATE_REQ_STRU   *pstPdnDeactReq;
    APS_EHSM_PDN_ACTIVATE_REQ_STRU     *pstPdnActReq;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU     *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                    *pstCacheMsgHdr;
    REL_TIMER_MSG                      *pstTimerMsg;
    VOS_UINT32                          ulCacheMsgEventType;

   *penAbortType   = CNAS_EHSM_ABORT_BUTT;
    pstPdnDeactReq = (APS_EHSM_PDN_DEACTIVATE_REQ_STRU*)pstMsg;
    pstMsgQueue    = CNAS_EHSM_GetCacheMsgAddr();

    /* ���֮ǰ����Ĳ���Ҫ��������Ϣ */
    for (i = 0; i < pstMsgQueue->ucCacheMsgNum; i++)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ)  == ulCacheMsgEventType)
        {
            pstPdnActReq = (APS_EHSM_PDN_ACTIVATE_REQ_STRU *)pstCacheMsgHdr;

            /* ���֮ǰ��������ͬCID�ļ���������Ϣ�����������û�����Ϣ */
            if (pstPdnActReq->ucCid == pstPdnDeactReq->ucCid)
            {
                /* ȥ����CNF�ظ�ʱ��δ����PDNID, UcCid������д��ȷ */
                CNAS_EHSM_SndApsPdnDeactivateCnf(CNAS_EHSM_INVALID_PDN_ID, pstPdnDeactReq->ucCid, EHSM_APS_CAUSE_SUCCESS);

                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);

                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }
    }

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 CNAS_EHSM_CompareTafReconnReqPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{

    VOS_UINT32                              i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    REL_TIMER_MSG                          *pstTimerMsg;
    VOS_UINT32                              ulCacheMsgEventType;
    APS_EHSM_RECONN_REQ_STRU               *pstReconnReq;
    APS_EHSM_RECONN_REQ_STRU               *pstReconnCah;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    *penAbortType   = CNAS_EHSM_ABORT_FSM_DELAY;

    /* ֮ǰ�Ѿ������˴������Ϣ������Ҫ�ظ����� */
    for (i = 0; i < pstMsgQueue->ucCacheMsgNum; i++)
    {
        pstCacheMsgHdr = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_NULL_PTR == pstCacheMsgHdr)
        {
            continue;
        }

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ) == ulCacheMsgEventType)
        {
            pstReconnReq = (APS_EHSM_RECONN_REQ_STRU *)pstMsg;
            pstReconnCah = (APS_EHSM_RECONN_REQ_STRU *)pstCacheMsgHdr;

            /* ����Ѿ�����ͬ��PDN�����ˣ����������� */
            if (pstReconnReq->ucPdnId == pstReconnCah->ucPdnId)
            {
                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }
    }

    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;


}


VOS_UINT32 CNAS_EHSM_CompareTafLocalDeactivateNotifyWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    VOS_INT32                               i;
    CNAS_EHSM_CACHE_MSG_QUEUE_STRU         *pstMsgQueue = VOS_NULL_PTR;
    APS_EHSM_PDN_ACTIVATE_REQ_STRU         *pstPdnAct;
    APS_EHSM_PDN_DEACTIVATE_REQ_STRU       *pstPdnDeact;
    APS_EHSM_LOC_DEACTIVATE_NOTIFY_STRU    *pstLocPdnDeact;
    MSG_HEADER_STRU                        *pstCacheMsgHdr;
    VOS_UINT32                              ulCacheMsgEventType;
    REL_TIMER_MSG                          *pstTimerMsg;
    CNAS_EHSM_MSG_STRU                     *pstEntryMsg    = VOS_NULL_PTR;
    APS_EHSM_PDN_DEACTIVATE_REQ_STRU       *pstPdnDeactReq = VOS_NULL_PTR;

    APS_EHSM_RECONN_REQ_STRU               *pstReconnCah;

    pstMsgQueue     = CNAS_EHSM_GetCacheMsgAddr();
    pstLocPdnDeact  = (APS_EHSM_LOC_DEACTIVATE_NOTIFY_STRU*)pstMsg;
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    /* ��ȡ��ǰȥ����״̬���������Ϣ */
    pstEntryMsg = CNAS_EHSM_GetCurrFsmEntryMsgAddr();

    /* �鿴��������û����ͬCID�Ĳ������еĻ���������� */
    for  (i = (pstMsgQueue->ucCacheMsgNum - 1); i >= 0; i--)
    {
        pstCacheMsgHdr          = (MSG_HEADER_STRU *)(pstMsgQueue->pastCacheMsg[i]);

        if (VOS_PID_TIMER == pstCacheMsgHdr->ulSenderPid)
        {
            pstTimerMsg = (REL_TIMER_MSG *)pstCacheMsgHdr;

            ulCacheMsgEventType  = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstTimerMsg->ulName);
        }
        else
        {
            ulCacheMsgEventType = CNAS_BuildEventType(pstCacheMsgHdr->ulSenderPid, pstCacheMsgHdr->ulMsgName);
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_ACTIVATE_REQ) == ulCacheMsgEventType)
        {
            pstPdnAct = (APS_EHSM_PDN_ACTIVATE_REQ_STRU*)pstCacheMsgHdr;
            if (pstLocPdnDeact->ucCid == pstPdnAct->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
                return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
            }
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ) == ulCacheMsgEventType)
        {
            pstPdnDeact = (APS_EHSM_PDN_DEACTIVATE_REQ_STRU*)pstCacheMsgHdr;
            if (pstLocPdnDeact->ucCid == pstPdnDeact->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
            }
        }

        if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_RECONN_REQ) == ulCacheMsgEventType)
        {
            pstReconnCah = (APS_EHSM_RECONN_REQ_STRU *)pstCacheMsgHdr;

            /* ����Ѿ�����ͬ��PDN�����ˣ����������� */
            if (pstLocPdnDeact->ucCid == pstReconnCah->ucCid)
            {
                CNAS_EHSM_ClearCacheMsg((VOS_UINT8)i);
            }
        }

    }

    if (CNAS_BuildEventType(UEPS_PID_HSD, ID_HSD_EHSM_POWER_OFF_REQ) == pstEntryMsg->ulEventType)
    {
        /* Power off���������ٴ����˴����Ϣ */
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }
    else if (CNAS_BuildEventType(WUEPS_PID_TAF, ID_APS_EHSM_PDN_DEACTIVATE_REQ) == pstEntryMsg->ulEventType)
    {
        pstPdnDeactReq = (APS_EHSM_PDN_DEACTIVATE_REQ_STRU *)(pstEntryMsg->aucMsgBuffer);

        if (pstLocPdnDeact->ucCid == pstPdnDeactReq->ucCid)
        {
            /* ���ȥ�������̣�ֱ��������ȥ���� */
            return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
        }
        else
        {
            return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
        }
    }
    else
    {
        /* ����ȥ���������Ϣ����ϴ��� */
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_STORE;
    }
}


VOS_UINT32 CNAS_EHSM_CompareHsmSessionInfoIndPrioWithActivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    HSM_EHSM_SESSION_INFO_IND_STRU     *pstSessionInfo = VOS_NULL_PTR;

    pstSessionInfo = (HSM_EHSM_SESSION_INFO_IND_STRU *)pstMsg;
    *penAbortType = CNAS_EHSM_ABORT_BUTT;

    if (EHSM_HSM_SESSION_TYPE_EHRPD != pstSessionInfo->enSessionType)
    {
        /* �л���HRPDģʽ�º�֮ǰ�������Ϣ�������壬��Ҫ�ȴ�������Ϣ */
        *penAbortType = CNAS_EHSM_ABORT_FSM_IMMEDIATELY;
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
    }
    else
    {
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }
}


VOS_UINT32 CNAS_EHSM_CompareHsmSessionInfoIndPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    HSM_EHSM_SESSION_INFO_IND_STRU     *pstSessionInfo = VOS_NULL_PTR;

    pstSessionInfo = (HSM_EHSM_SESSION_INFO_IND_STRU *)pstMsg;

    if (EHSM_HSM_SESSION_TYPE_EHRPD != pstSessionInfo->enSessionType)
    {
        *penAbortType   = CNAS_EHSM_ABORT_BUTT;

        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ABORT;
    }
    else
    {
        *penAbortType = CNAS_EHSM_ABORT_BUTT;

        /* ����Ҫ���棬��������ڴ�����SUSPEND�¼��󣬰�EHRPD��״̬���������ΪACTIVE */
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }
}


VOS_UINT32 CNAS_EHSM_CompareTafDiscNotifyPrioWithDeactivating(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    *penAbortType   = CNAS_EHSM_ABORT_BUTT;

    /* �����detach���̣���״̬��ǰ��Ͽ����ӣ������ͨ��ȥ���ȥ������ɺ������PDN�����ݻ��ᴥ���Ͽ����ӵ� */
    return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
}



CNAS_EHSM_MSG_COMPARE_FUNC  CNAS_EHSM_FindMsgPrioCompareFun(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_FSM_ID_ENUM_UINT32        enFsmId
)
{
    CNAS_EHSM_FSM_MSG_COMPARE_STRU     *pstFsmCompareTbl = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;

    ulTblSize = sizeof(g_astEhsmMsgCompareTbl)/sizeof(CNAS_EHSM_FSM_MSG_COMPARE_STRU);

    for ( i = 0 ; i < ulTblSize ; i++ )
    {
        if ( enFsmId == g_astEhsmMsgCompareTbl[i].enFsmId)
        {
            pstFsmCompareTbl = &g_astEhsmMsgCompareTbl[i];
            break;
        }
    }

    /* δ���ҵ���״̬���ıȽϺ�����,ֱ�ӷ��ؿ�ָ�� */
    if ( VOS_NULL_PTR == pstFsmCompareTbl )
    {
        return VOS_NULL_PTR;
    }

    for ( i = 0 ; i < pstFsmCompareTbl->ulSize; i++ )
    {
        /* ���Ҳ�ͬ�¼��ıȽϺ��� */
        if ( pstFsmCompareTbl->pfCompareTbl[i].ulCurrEventType == ulEventType)
        {
            return pstFsmCompareTbl->pfCompareTbl[i].pfCompareFun;
        }
    }

    /* δ�ҵ���������ֱ�ӷ��ؿ�ָ�� */
    return VOS_NULL_PTR;
}




CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32 CNAS_EHSM_GetMsgComparePrioRslt(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    CNAS_EHSM_ABORT_FSM_TYPE_UINT8     *penAbortType
)
{
    CNAS_EHSM_MSG_COMPARE_FUNC                      pfCompareFunc;
    CNAS_EHSM_FSM_ID_ENUM_UINT32                    enCurrFsmId;
    CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32     enRslt;

    /*  ��ȡ��ǰ״̬����ID */
    enCurrFsmId         = CNAS_EHSM_GetCurrFsmId();

    if (CNAS_EHSM_FSM_BUTT <= enCurrFsmId)
    {
        CNAS_ERROR_LOG(UEPS_PID_EHSM, "ERROR:FsmId Error");
    }

    /*  ��ȡ�ȽϺ��� CNAS_EHSM_FindMsgPrioCompareFun() */
    pfCompareFunc       = CNAS_EHSM_FindMsgPrioCompareFun(ulEventType, pstMsg, enCurrFsmId);

    /* δ�ҵ���������,��Ĭ����Ϣ���� */
    if ( VOS_NULL_PTR == pfCompareFunc )
    {
        *penAbortType = CNAS_EHSM_ABORT_BUTT;
        return CNAS_EHSM_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }

    /* ���ñȽϺ���,�������ȼ��ʹ������ */
    enRslt = pfCompareFunc(ulEventType, pstMsg, penAbortType);

    return enRslt;

}


#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
