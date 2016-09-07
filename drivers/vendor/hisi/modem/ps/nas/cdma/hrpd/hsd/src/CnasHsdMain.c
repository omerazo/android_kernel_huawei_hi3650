

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHsdMain.h"
#include "CnasHsdCtx.h"
#include "CnasTimerMgmt.h"
#include "CnasCcb.h"
#include "CnasMntn.h"
#include "CnasHsdFsmMainTbl.h"
#include "CnasHsdFsmSwitchOnTbl.h"
#include "CnasHsdFsmPowerOffTbl.h"
#include "CnasHsdFsmSysAcqTbl.h"
#include "CnasHsdPreProcTbl.h"
#include "CnasHsdFsmPowerSaveTbl.h"
#include "CnasHsdFsmInterSysTbl.h"
#include "CnasHsdMntn.h"
#include "CnasHsdFsmBgSrchTbl.h"


#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HSD_MAIN_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/

VOS_UINT32 CNAS_HSD_RegPreFsm(VOS_VOID)
{
    /* Ԥ����״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetPreFsmDescAddr()),
                         "CNAS:FSM:HSD:PRE",
                         (VOS_UINT32)(CNAS_HSD_GetPreProcessStaTblSize()),
                         CNAS_HSD_GetPreProcStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);
    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RegMainL1Fsm(VOS_VOID)
{
    /* main״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetMainFsmDescAddr()),
                         "CNAS:FSM:HSD:MAIN",
                         (VOS_UINT32)(CNAS_HSD_GetL1MainStaTblSize()),
                         CNAS_HSD_GetL1MainStaTbl(),
                         VOS_NULL_PTR,
                         CNAS_HSD_InitFsmCtx_L1Main);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RegL2Fsm(VOS_VOID)
{
    /* SwitchOn״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetSwitchOnFsmDescAddr()),
                         "CNAS:FSM:HSD:SwitchOn",
                         (VOS_UINT32)CNAS_HSD_GetSwitchOnStaTblSize(),
                         CNAS_HSD_GetSwitchOnStaTbl(),
                         VOS_NULL_PTR,
                         CNAS_HSD_InitFsmCtx_SwitchOn);

    /* PowerOff״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetPowerOffFsmDescAddr()),
                         "CNAS:FSM:HSD:PowerOff",
                         (VOS_UINT32)CNAS_HSD_GetPowerOffStaTblSize(),
                         CNAS_HSD_GetPowerOffStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);

    /* SysAcq״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetSysAcqFsmDescAddr()),
                         "CNAS:FSM:HSD:SysAcq",
                         (VOS_UINT32)CNAS_HSD_GetSysAcqStaTblSize(),
                         CNAS_HSD_GetSysAcqStaTbl(),
                         VOS_NULL_PTR,
                         CNAS_HSD_InitFsmCtx_SysAcq);

    /* PowerSave״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetPowerSaveFsmDescAddr()),
                         "CNAS:FSM:HSD:PowerSave",
                         (VOS_UINT32)CNAS_HSD_GetPowerSaveStaTblSize(),
                         CNAS_HSD_GetPowerSaveStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);

    /* InterSys״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetInterSysFsmDescAddr()),
                         "CNAS:FSM:HSD:InterSys",
                         (VOS_UINT32)CNAS_HSD_GetInterSysStaTblSize(),
                         CNAS_HSD_GetInterSysStaTbl(),
                         VOS_NULL_PTR,
                         CNAS_HSD_InitFsmCtx_InterSys);

    /* BgSrch״̬��ע�� */
    NAS_FSM_RegisterFsm((CNAS_HSD_GetBgSrchFsmDescAddr()),
                         "CNAS:FSM:HSD:BgSrch",
                         (VOS_UINT32)CNAS_HSD_GetBgSrchStaTblSize(),
                         CNAS_HSD_GetBgSrchStaTbl(),
                         VOS_NULL_PTR,
                         CNAS_HSD_InitFsmCtx_BgSrch);

    return VOS_TRUE;
}


VOS_VOID  CNAS_HSD_RegFsm(VOS_VOID)
{
    /* ע��Ԥ����״̬�� */
    CNAS_HSD_RegPreFsm();

    /* ע��L1״̬�� */
    CNAS_HSD_RegMainL1Fsm();

    /* ע��L2״̬�� */
    CNAS_HSD_RegL2Fsm();

    return;
}


VOS_UINT32 CNAS_HSD_PreProcessMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet      = VOS_FALSE;
    NAS_ACTION_FUNC                     pActFun    = VOS_NULL_PTR;
    NAS_FSM_DESC_STRU                  *pstFsmDesc = VOS_NULL_PTR;

    /* ��ȡԤ����״̬����������ַ */
    pstFsmDesc = CNAS_HSD_GetPreFsmDescAddr();

    /* ��ȡ״̬ת�Ʊ��е���Ӧ�Ķ����������� */
    pActFun = NAS_FSM_FindAct(pstFsmDesc, CNAS_HSD_L1_STA_PREPROC, ulEventType);

    /* ������ָ���� */
    if (VOS_NULL_PTR != pActFun )
    {
        /* ִ�к������ܲ��� */
        ulRet = (*pActFun) ( ulEventType, pstMsg);
    }

    return ulRet;
}


VOS_UINT32 CNAS_HSD_FsmProcessEvent(
    VOS_UINT32                          ulCurState,
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pRcvMsg
)
{
    NAS_ACTION_FUNC                     pActFun;
    NAS_FSM_DESC_STRU                  *pstFsmDesc = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

    pstFsmDesc = CNAS_HSD_GetCurFsmDesc();

    /* ����״̬ת�Ʊ��е���Ӧ�Ķ����������� */
    pActFun = NAS_FSM_FindAct(pstFsmDesc, ulCurState, ulEventType);

    if (VOS_NULL_PTR == pActFun)
    {
        /* ��Ϣδ������������������ */
        return VOS_FALSE;
    }

    /* ������ص��¼�����������Ϊ��,�����������¼����� */
    ulRet   = (*pActFun) ( ulEventType,(struct MsgCB*)pRcvMsg);

    CNAS_INFO_LOG3(UEPS_PID_HSD, "CNAS_HSD_FsmProcessEvent, prestate, eventtype,retValue", ulCurState, ulEventType, ulRet);

    return ulRet;
}


VOS_UINT32 CNAS_HSD_ProcessMsgInFsm(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    CNAS_HSD_FSM_CTX_STRU              *pstCurFsm = VOS_NULL_PTR;
    VOS_UINT16                          usOldStackDepth;
    VOS_UINT16                          usNewStackDepth;

    /* ��ȡ��ǰHSD��״̬�������� */
    pstCurFsm           = CNAS_HSD_GetCurFsmAddr();

    /* ��ȡ��ǰHSD��ջ��� */
    usOldStackDepth     = CNAS_HSD_GetFsmStackDepth();

    /* ��Ϣ���� */
    ulRet     = CNAS_HSD_FsmProcessEvent(pstCurFsm->ulState,
                                         ulEventType,
                                         (VOS_VOID*)pstMsg);

    /* ��ȡ��ǰHSD����ջ��� */
    usNewStackDepth    = CNAS_HSD_GetFsmStackDepth();

    /* ջ���û������,����û�д����µ�״̬���������˳�*/
    if (usNewStackDepth <= usOldStackDepth)
    {
        return ulRet;
    }

    /* �µ�ջ��ȴ����ϵ�ջ��� ����������ѹջ,��Ҫ����Ϣ����״̬���м������� */
    /* ���»�ȡ��ǰMMC�Ķ���״̬ */
    pstCurFsm   = CNAS_HSD_GetCurFsmAddr();

    /* ����Ϣ���붥��״̬������������ */
    ulRet       = CNAS_HSD_FsmProcessEvent(pstCurFsm->ulState,
                                           ulEventType,
                                           (VOS_VOID*)pstMsg);


    return ulRet;
}


VOS_VOID CNAS_HSD_ProcessHsdIntMsg(VOS_VOID)
{
    MSG_HEADER_STRU                    *pstIntMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulEventType;

    pstIntMsg = (MSG_HEADER_STRU *)CNAS_HSD_GetNextIntMsg();

    while (VOS_NULL_PTR != pstIntMsg)
    {
        /* ��Ϣ���� */
        CNAS_MNTN_TraceIntMsg(pstIntMsg->ulSenderPid,
                              pstIntMsg->ulLength + VOS_MSG_HEAD_LENGTH,
                              (VOS_VOID *)pstIntMsg);

        ulEventType  = CNAS_BuildEventType(pstIntMsg->ulSenderPid, (VOS_UINT16)pstIntMsg->ulMsgName);

        /* ����Ѿ����������ȡ������Ϣ�������� */
        if (VOS_TRUE == CNAS_HSD_PreProcessMsg(ulEventType, (struct MsgCB*)pstIntMsg))
        {
            /* �ͷ��ڲ���Ϣ */
            PS_MEM_FREE(pstIntMsg->ulSenderPid, pstIntMsg);

            /* ������һ����Ϣ���� */
            pstIntMsg = (MSG_HEADER_STRU *)CNAS_HSD_GetNextIntMsg();

            continue;
        }

        /* ��Ϣ��״̬������ */
        CNAS_HSD_ProcessMsgInFsm(ulEventType, (struct MsgCB *)pstIntMsg);

        /* �ͷ��ڲ���Ϣ */
        PS_MEM_FREE(pstIntMsg->ulSenderPid, pstIntMsg);

        /* ������һ����Ϣ���� */
        pstIntMsg = (MSG_HEADER_STRU *)CNAS_HSD_GetNextIntMsg();
    }

    return;
}


VOS_VOID CNAS_HSD_ProcessBufferMsg(VOS_VOID)
{
    CNAS_HSD_MSG_STRU                  *pstEntryMsg = VOS_NULL_PTR;

    /* �ڴ�ռ���� */
    pstEntryMsg = (CNAS_HSD_MSG_STRU *)PS_MEM_ALLOC(UEPS_PID_HSD, sizeof(CNAS_HSD_MSG_STRU) );

    if (VOS_NULL_PTR == pstEntryMsg)
    {
        return;
    }

    while (VOS_TRUE == CNAS_HSD_GetNextCachedMsg(pstEntryMsg))
    {
        /* ����Ѿ����������ȡ������Ϣ�������� */
        if (VOS_TRUE == CNAS_HSD_PreProcessMsg(pstEntryMsg->ulEventType, (struct MsgCB*)pstEntryMsg->aucMsgBuffer))
        {
            continue;
        }

        /* ����״̬������ */
        CNAS_HSD_ProcessMsgInFsm(pstEntryMsg->ulEventType, (struct MsgCB*)pstEntryMsg->aucMsgBuffer);

        /* �ڲ����е���Ϣ���� */
        CNAS_HSD_ProcessHsdIntMsg();
    }

    PS_MEM_FREE(UEPS_PID_HSD, pstEntryMsg);

    return;
}


VOS_VOID CNAS_HSD_MsgProc (struct MsgCB* pstMsg)
{
    MSG_HEADER_STRU                    *pstMsgHeader = VOS_NULL_PTR;
    REL_TIMER_MSG                      *pRcvTimerMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulEventType;
    VOS_UINT32                          usPrevStackDepth;
    VOS_UINT32                          usNewStackDepth;
    VOS_UINT32                          ulSenderPid;

    ulSenderPid = CNAS_CCB_GetModem0Pid(CNAS_CCB_GetCdmaModeModemId(), pstMsg->ulSenderPid);

    pstMsg->ulSenderPid = ulSenderPid;

    pstMsgHeader = (MSG_HEADER_STRU *)pstMsg;

    CNAS_HSD_LogMsgInfo(pstMsgHeader);

    /* �����¼����� */
    if (VOS_PID_TIMER == pstMsgHeader->ulSenderPid)
    {
        pRcvTimerMsg = (REL_TIMER_MSG *)pstMsg;

        ulEventType  = CNAS_BuildEventType(pstMsgHeader->ulSenderPid, pRcvTimerMsg->ulName);

        /* ֹͣ��Ӧ�Ķ�ʱ�� */
        CNAS_StopTimer(UEPS_PID_HSD, pRcvTimerMsg->ulName, pRcvTimerMsg->ulPara);
    }
    else
    {
        ulEventType  = CNAS_BuildEventType(pstMsgHeader->ulSenderPid, (VOS_UINT16)pstMsgHeader->ulMsgName);
    }

    /* ����Ѿ����������ֱ�ӷ��� */
    if (VOS_TRUE == CNAS_HSD_PreProcessMsg(ulEventType, pstMsg))
    {
        /* �����ڲ���Ϣ���� */
        CNAS_HSD_ProcessHsdIntMsg();

        return;
    }

    /* ��ȡ��ǰHSD��ջ��� */
    usPrevStackDepth = CNAS_HSD_GetFsmStackDepth();

    /* ����״̬������ */
    CNAS_HSD_ProcessMsgInFsm(ulEventType, pstMsg);

    /* ��״̬���˳��󣬻�ȡ�µ�ջ��� */
    usNewStackDepth = CNAS_HSD_GetFsmStackDepth();

    /* �����ڲ���Ϣ���� */
    CNAS_HSD_ProcessHsdIntMsg();

    /* ���ջ����ȼ���,˵���϶���������ջ����,����һ�λ��� */
    if (usNewStackDepth < usPrevStackDepth)
    {
        CNAS_HSD_ProcessBufferMsg();
    }

    return;
}


VOS_UINT32  CNAS_HSD_InitTask(VOS_VOID)
{
    /* ��ʼ��HSDȫ�ֱ��� */
    CNAS_HSD_InitCtx(CNAS_HSD_INIT_CTX_STARTUP);

    /* HSD ״̬��ע�� */
    CNAS_HSD_RegFsm();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_InitPid(
    enum VOS_INIT_PHASE_DEFINE         enInitPhase
)
{
    switch (enInitPhase)
    {
        case VOS_IP_LOAD_CONFIG:
            /* ����HSDģ���ʼ������ */
            CNAS_HSD_InitTask();
            break;

        case VOS_IP_INITIAL:
        case VOS_IP_FARMALLOC:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
            break;

        default:
            break;
    }

    return VOS_OK;
}
/*lint -restore*/

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


