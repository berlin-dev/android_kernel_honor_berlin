

/******************************************************************************
   1 头文件包含
******************************************************************************/
#include "PsLogFilterInterface.h"
#include "PsLib.h"


#define    THIS_FILE_ID        PS_FILE_ID_LOG_FILTER_COMM_C

/******************************************************************************
   2 外部函数变量声明
******************************************************************************/

/******************************************************************************
   3 私有定义
******************************************************************************/


/******************************************************************************
   4 全局变量定义
******************************************************************************/


/******************************************************************************
   5 函数实现
******************************************************************************/


/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgMatchFuncCommReg
 功能描述  : A C核注册MsgMatch回调接口
 输入参数  : 需要注册的MsgMatchFunc函数:
                PS_OM_LAYER_MSG_MATCH_PFUNC         pFunc,
             记录注册的MsgMatchFunc的个数，每注册一个+1:
                VOS_UINT32                         *pulRegCnt,
             存储MsgMatchFunc函数的数组:
                PS_OM_LAYER_MSG_MATCH_PFUNC         apfuncMatchEntry[],
             存储MsgMatchFunc函数的数组的最大数量:
                VOS_UINT32                          ulMsgMatchItemMaxCnt
输入参数回调接口规则:
             1.如果注册回调内部没有对消息进行处理，则需要将入参指针返回，否则
               本模块不知道是否需要将此消息传递给下一个注册回调进行处理
             2.如果注册回调内部对消息进行了处理，则返回值能够实现两个功能:
               ①返回VOS_NULL，则将此消息进行完全过滤，不会再勾取出来
               ②返回与入参指针不同的另一个指针，则勾取的消息将会使用返回的指
                 针内容替换原消息的内容。另本模块不负责对替换的内存进行释放，
                 替换原消息使用的内存请各模块自行管理。
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : z00383822
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgMatchFuncCommReg
(
    PS_OM_LAYER_MSG_MATCH_PFUNC         pFunc,
    VOS_UINT32                         *pulRegCnt,
    PS_OM_LAYER_MSG_MATCH_PFUNC        *apfuncMatchEntry,
    VOS_UINT32                          ulMsgMatchItemMaxCnt
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMaxLoop;

    if ((VOS_NULL_PTR == pFunc)
        || (VOS_NULL_PTR == pulRegCnt)
        || (VOS_NULL_PTR == apfuncMatchEntry))
    {
        return VOS_ERR;
    }

    ulIndex     = *pulRegCnt;
    ulMaxLoop   = PS_MIN(ulIndex, ulMsgMatchItemMaxCnt);

    for (ulLoop = 0; ulLoop < ulMaxLoop; ulLoop++)
    {
        if (apfuncMatchEntry[ulLoop] == pFunc)
        {
            return VOS_OK;
        }
    }

    if (ulMsgMatchItemMaxCnt > ulIndex)
    {
       *pulRegCnt   += 1;
        apfuncMatchEntry[ulIndex] = pFunc;

        return VOS_OK;
    }

    return VOS_ERR;
}


/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgCommMatch
 功能描述  : A C核层间消息替换接口
 输入参数  : 需要处理的消息:
                VOS_VOID                           *pMsg,
             记录已经注册的MsgMatchFunc的个数:
                VOS_UINT32                          ulRegCnt,
             记录已经注册的MsgMatchFunc的数组:
                PS_OM_LAYER_MSG_MATCH_PFUNC         apfuncMatchEntry[],
             存储MsgMatchFunc函数的数组的最大数量:
                VOS_UINT32                          ulMsgMatchItemMaxCnt
 输出参数  : 无
 返 回 值  : VOS_VOID*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年11月14日
    作    者   : z00383822
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID* PS_OM_LayerMsgCommMatch
(
    VOS_VOID                           *pMsg,
    VOS_UINT32                          ulRegCnt,
    PS_OM_LAYER_MSG_MATCH_PFUNC        *apfuncMatchEntry,
    VOS_UINT32                          ulMsgMatchItemMaxCnt
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulEntryCnt;
    VOS_VOID                           *pResult    = pMsg;

    ulEntryCnt      = PS_MIN(ulRegCnt, ulMsgMatchItemMaxCnt);

    for (ulIndex = 0; ulIndex < ulEntryCnt; ulIndex++)
    {
        if (VOS_NULL != apfuncMatchEntry[ulIndex])
        {
            pResult = apfuncMatchEntry[ulIndex]((MsgBlock*)pMsg);
            if (pMsg != pResult)
            {
                break;
            }
        }
    }

    return pResult;
}


/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgFilterFuncReg
 功能描述  : A、C 核注册MsgMatch回调接口
 输入参数  :需要注册的MsgFilterFunc函数:
                PS_OM_LAYER_MSG_FILTER_PFUNC         pFunc,
            记录已经注册的MsgFilterFunc的个数:
                VOS_UINT32                          *pulRegCnt,
            记录已经注册的MsgFilterFunc的数组:
                PS_OM_LAYER_MSG_FILTER_PFUNC         apfuncFilterEntry[],
            存储MsgFilterFunc函数的数组的最大数量:
                VOS_UINT32                           ulMsgFilterItemMaxCnt
            输入参数回调接口规则:
             1.如果注册回调内部没有对消息进行处理，返回VOS_FALSE，否则
               本模块不知道是否需要将此消息传递给下一个注册回调进行处理
             2.如果注册回调内部对消息进行了处理，返回VOS_TRUE表示该消息
               需要进行过滤。
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : z00383822
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgFilterFuncCommReg
(
    PS_OM_LAYER_MSG_FILTER_PFUNC         pFunc,
    VOS_UINT32                          *pulRegCnt,
    PS_OM_LAYER_MSG_FILTER_PFUNC        *apfuncFilterEntry,
    VOS_UINT32                           ulMsgFilterItemMaxCnt
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMaxLoop;

    if ((VOS_NULL_PTR == pFunc)
        || (VOS_NULL_PTR == pulRegCnt)
        || (VOS_NULL_PTR == apfuncFilterEntry))
    {
        return VOS_ERR;
    }

    ulIndex     = *pulRegCnt;
    ulMaxLoop   = PS_MIN(ulIndex, ulMsgFilterItemMaxCnt);

    for (ulLoop = 0; ulLoop < ulMaxLoop; ulLoop++)
    {
        if (apfuncFilterEntry[ulLoop] == pFunc)
        {
            return VOS_OK;
        }
    }

    if (ulIndex < ulMsgFilterItemMaxCnt)
    {
        *pulRegCnt   += 1;
        apfuncFilterEntry[ulIndex] = pFunc;

        return VOS_OK;
    }

    return VOS_ERR;
}

/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgCommFilter
 功能描述  : A、C 核层间消息过滤接口
 输入参数  : 需要处理的消息:
                const VOS_VOID                     *pMsg,
             记录已经注册的MsgFilterFunc的个数:
                VOS_UINT32                          ulRegCnt,
             记录已经注册的MsgFilterFunc的数组:
                PS_OM_LAYER_MSG_MATCH_PFUNC         apfuncFilterEntry[],
             存储MsgFilterFunc函数的数组的最大数量:
                VOS_UINT32                          ulCount
 输出参数  : 无
 返 回 值  : VOS_UINT32:
                返回VOS_TRUE:  表示该消息需要进行过滤
                返回VOS_FALSE: 表示该消息无需进行过滤
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : z00383822
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgCommFilter
(
    const VOS_VOID                     *pMsg,
    VOS_UINT32                          ulRegCnt,
    PS_OM_LAYER_MSG_FILTER_PFUNC       *apfuncFilterEntry,
    VOS_UINT32                          ulMsgFilterItemMaxCnt
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulEntryCnt;
    VOS_UINT32                          ulResult    = VOS_FALSE;

    ulEntryCnt      = PS_MIN(ulRegCnt, ulMsgFilterItemMaxCnt);

    for (ulIndex = 0; ulIndex < ulEntryCnt; ulIndex++)
    {
        if (VOS_NULL != apfuncFilterEntry[ulIndex])
        {
            ulResult = apfuncFilterEntry[ulIndex](pMsg);
            if (VOS_FALSE != ulResult)
            {
                break;
            }
        }
    }

    return ulResult;

}


