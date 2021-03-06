

#ifndef __RRMRCMINTERFACE_H__
#define __RRMRCMINTERFACE_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
#include  "vos_Id.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


enum RRM_RCM_MSG_TYPE_ENUM
{
    ID_RRM_RCM_PROTECT_RF_IND           = 0x0001,       /* _H2ASN_MsgChoice RRM_RCM_PROTECT_RF_IND_STRU */

    ID_RRM_RCM_DEPROTECT_RF_IND         = 0x0002,       /* _H2ASN_MsgChoice RRM_RCM_DEPROTECT_RF_IND_STRU */

    ID_RRM_RCM_ACTIVE_ID_NTF            = 0x0003,       /* _H2ASN_MsgChoice RRM_RCM_ACTIVE_ID_NTF_STRU */

    ID_RRM_RCM_MSG_TYPE_BUTT
};
typedef VOS_UINT32 RRM_RCM_MSG_TYPE_ENUM_UINT32;

/*******************************************************************************
 枚举名    : RRM_RCM_ACTIVE_ID_ENUM
 结构说明  : RRM RCM Active Id标识定义
*******************************************************************************/
enum RRM_RCM_ACTIVE_ID_ENUM
{
    RRM_RCM_ACTIVE_ID_0               = 0,   /* Active Id 0 标识 */
    RRM_RCM_ACTIVE_ID_1               = 1,   /* Active Id 1 标识 */
    RRM_RCM_ACTIVE_ID_2               = 2,   /* Active Id 2 标识 */
    RRM_RCM_ACTIVE_ID_1X              = 3,   /* Active Id 1x 标识 */
    RRM_RCM_ACTIVE_ID_BUTT
};
typedef VOS_UINT16 RRM_RCM_ACTIVE_ID_ENUM_UINT16;

/*****************************************************************************
 枚举名    : RRM_RCM_MODE_ID_ENUM_UINT16
 协议表格  :
 ASN.1描述 :
 枚举说明  : 物理层模式标示 排列次序和校准次序一致 与 RCM_MODE_ID_ENUM_UINT16保持一致
*****************************************************************************/
enum RRM_RCM_MODE_ID_ENUM
{
    RRM_RCM_LTE                         = 0,                                    /* LTE */
    RRM_RCM_W0                          = 1,                                    /* W0 */
    RRM_RCM_W1                          = 2,                                    /* W1 */
    RRM_RCM_1X                          = 3,                                    /* 1X */
    RRM_RCM_HRPD                        = 4,                                    /* HRPD */
    RRM_RCM_TD                          = 5,                                    /* TD */
    RRM_RCM_G0                          = 6,                                    /* G0 */
    RRM_RCM_G1                          = 7,                                    /* G1 */
    RRM_RCM_G2                          = 8,                                    /* G2 */
    RRM_RCM_MODE_ID_BUTT
};
typedef VOS_UINT16  RRM_RCM_MODE_ID_ENUM_UINT16;

/*****************************************************************************
 枚举名    : RRM_RCM_RESOURCE_ID_ENUM_UINT16
 协议表格  :
 ASN.1描述 :
 枚举说明  : resoure ID
*****************************************************************************/
enum RRM_RCM_RESOURCE_ID_ENUM
{
    RRM_RCM_RESOURCE_0                            = 0,                          /* RESOURCE 0 */
    RRM_RCM_RESOURCE_1                            = 1,                          /* RESOURCE 1 */
    RRM_RCM_RESOURCE_ID_BUTT
};
typedef VOS_UINT16  RRM_RCM_RESOURCE_ID_ENUM_UINT16;


/*****************************************************************************
 枚举名    : RRM_RCM_DSDS_FLG_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRM_RCM_DSDS_FLG_ENUM
*****************************************************************************/
enum RRM_RCM_DSDS_FLG_ENUM
{
    RRM_RCM_DSDS_CLOSE                            = 0,                          /* DSDS CLOSE */
    RRM_RCM_DSDS_OPEN                             = 1,                          /* DSDS OPEN  */
    RRM_RCM_DSDS_BUTT
};
typedef VOS_UINT16  RRM_RCM_DSDS_FLG_ENUM_UINT16;

/*****************************************************************************
 枚举名    : RRM_RCM_DSDS_VERSION_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRM_RCM_DSDS_VERSION_ENUM
*****************************************************************************/
enum RRM_RCM_DSDS_VERSION_ENUM
{
    RRM_RCM_DSDS_1_0                            = 0,                          /* DSDS1.0 标识 */
    RRM_RCM_DSDS_2_0                            = 1,                          /* DSDS2.0 标识  */
    RRM_RCM_DSDS_VERSION_BUTT
};
typedef VOS_UINT16  RRM_RCM_DSDS_VERSION_ENUM_UINT16;


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    RRM_RCM_MSG_TYPE_ENUM_UINT32                enMsgId;                        /* 消息类型 */  /* _H2ASN_Skip */
    RRM_RCM_ACTIVE_ID_ENUM_UINT16               enActiveId;                     /* Active Id */
    RRM_RCM_RESOURCE_ID_ENUM_UINT16             enResourceId;                   /* Resource Id */

} RRM_RCM_PROTECT_RF_IND_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    RRM_RCM_MSG_TYPE_ENUM_UINT32                enMsgId;                        /* 消息类型 */  /* _H2ASN_Skip */
    RRM_RCM_ACTIVE_ID_ENUM_UINT16               enActiveId;                     /* Active Id */
    RRM_RCM_RESOURCE_ID_ENUM_UINT16             enResourceId;                   /* Resource Id */
} RRM_RCM_DEPROTECT_RF_IND_STRU;


typedef struct
{
    RRM_RCM_MODE_ID_ENUM_UINT16                 enModeId;
    RRM_RCM_ACTIVE_ID_ENUM_UINT16               enActiveId;                     /* Active Id */
    RRM_RCM_RESOURCE_ID_ENUM_UINT16             enResourceId;                   /* Resource Id */
    RRM_RCM_DSDS_FLG_ENUM_UINT16                enDsdsFlg;                      /* DSDS flag */
} RRM_RCM_MODE_ID_PARA_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    RRM_RCM_MSG_TYPE_ENUM_UINT32                enMsgId;                        /* 消息类型 */  /* _H2ASN_Skip */
    VOS_UINT16                                  usModeIdNum;
    RRM_RCM_DSDS_VERSION_ENUM_UINT16            enDsdsVersion;                  /* Dsds Version */
    RRM_RCM_MODE_ID_PARA_STRU                   astModeIdPara[RRM_RCM_MODE_ID_BUTT];
} RRM_RCM_ACTIVE_ID_NTF_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  H2ASN顶级消息结构定义
*****************************************************************************/

/*****************************************************************************
 结构名    : RRM_RCM_MSG_DATA
 协议表格  :
 ASN.1描述 :
 结构说明  : RRM_RCM_MSG_DATA数据结构，用于生成ASN文件
*****************************************************************************/
typedef struct
{
    RRM_RCM_MSG_TYPE_ENUM_UINT32        enMsgId;    /*_H2ASN_MsgChoice_Export RRM_RCM_MSG_TYPE_ENUM_UINT32 */
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          RRM_RCM_MSG_TYPE_ENUM_UINT32
    ****************************************************************************/
}RRM_RCM_MSG_DATA;

/*_H2ASN_Length UINT32*/

/*****************************************************************************
 结构名    : RrmRcmInterface_MSG
 协议表格  :
 ASN.1描述 :
 结构说明  : RrmRcmInterface_MSG数据结构,用于生成ASN文件
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    RRM_RCM_MSG_DATA                             stMsgData;
}RrmRcmInterface_MSG;


/*****************************************************************************
  10 函数声明
*****************************************************************************/





#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of RrmRcmInterface.h */

