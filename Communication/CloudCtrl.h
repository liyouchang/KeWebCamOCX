/***************************************************************************
 *  标    题: 云台控制
 *  功能描述: 处理分析云台协议
 *  创建日期: 2008-05-08
 *  版 本 号: ver1.00.00
 *  作    者: 吕立国
 *  修改历史及原因
 *  Date                  Note                 Reason            Updator
 *
 ***************************************************************************/

#ifndef __CLOUDCTRL_H_
#define __CLOUDCTRL_H_

typedef struct SCloudData
{
	BYTE byProtocol;
	BYTE byCtlId;
	WORD wCMD;
	WORD wSpeed;
	WORD wData;
} CLOUDDATA;

extern void IniCloudData();
extern int GetCloudProtocol(int iProtocol, int iAddr, int iCtrlId, WORD wSpeed, char *pData);

#endif