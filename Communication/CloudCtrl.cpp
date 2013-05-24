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
#include "StdAfx.h"
#include "CloudCtrl.h"
#include "MapTemplet.h"

//变量定义
MapTemplet<CLOUDDATA, 0> g_cloudDataList;

/********************************************************************
 *	名称:AddCloudData
 *	功能描述:加云台数据
 *	参数:
 *		输入参数:
 *		iProtocol - 协议, iCtrlId - 指令Id, wCMD - 云台命令, wSpeed - 云台速度
 *		输出参数:
 *		无
 *	日期:2008-05-08
 *	作者:吕立国
 *********************************************************************/
void AddCloudData(int iProtocol, int iCtrlId, WORD wCMD, WORD wSpeed = 0)
{
	CLOUDDATA *pCloud = g_cloudDataList.AddData_Unlimit(iProtocol, iCtrlId);
	pCloud->byProtocol = iProtocol;
	pCloud->byCtlId = iCtrlId;
	pCloud->wCMD = wCMD;
	pCloud->wSpeed = wSpeed;
}

/********************************************************************
 *	名称:IniCloudData
 *	功能描述:初始化云台数居
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		无
 *	日期:2008-05-08
 *	作者:吕立国
 *********************************************************************/
void IniCloudData()
{
	int iProtocol = 0;
	//_D协议
	//上
	AddCloudData(iProtocol,  0, 0x0008);
	//下
	AddCloudData(iProtocol,  1, 0x0010);
	//左
	AddCloudData(iProtocol,  2, 0x0004);
	//右
	AddCloudData(iProtocol,  3, 0x0002);
	//光圈关
	AddCloudData(iProtocol,  4, 0x0400);
	//光圈开
	AddCloudData(iProtocol,  5, 0x0200);
	AddCloudData(iProtocol,  6, 0x0100);
	AddCloudData(iProtocol,  7, 0x0080);
	
	AddCloudData(iProtocol,  8, 0x0040);
	AddCloudData(iProtocol,  9, 0x0020);
	
	AddCloudData(iProtocol, 14, 0x9000);
	AddCloudData(iProtocol, 15, 0x0090);
	AddCloudData(iProtocol, 16, 0x0007);
	AddCloudData(iProtocol, 17, 0x001b);
	
	AddCloudData(iProtocol, 18, 0x001d);
	AddCloudData(iProtocol, 19, 0x0000);
	
	AddCloudData(iProtocol, 24, 0x0003);
	AddCloudData(iProtocol, 25, 0x0007);
	
	AddCloudData(iProtocol, 27, 0x001f);
	AddCloudData(iProtocol, 28, 0x0021);
	AddCloudData(iProtocol, 29, 0x0023);
	AddCloudData(iProtocol, 30, 0x0009);
	AddCloudData(iProtocol, 31, 0x000b);
	
	//_P协议
	iProtocol = 1;
	AddCloudData(iProtocol,  0, 0x0008);//上
	AddCloudData(iProtocol,  1, 0x0010);//下
	AddCloudData(iProtocol,  2, 0x0004);//左
	AddCloudData(iProtocol,  3, 0x0002);//右
	
	AddCloudData(iProtocol,  4, 0x0800);//光圈关
	AddCloudData(iProtocol,  5, 0x0400);//光圈开
	AddCloudData(iProtocol,  6, 0x0100);
	AddCloudData(iProtocol,  7, 0x0200);
	
	AddCloudData(iProtocol,  8, 0x0040);
	AddCloudData(iProtocol,  9, 0x0020);
	
	AddCloudData(iProtocol, 14, 0x0019);
	AddCloudData(iProtocol, 15, 0x0200);
	
	AddCloudData(iProtocol, 19, 0x0000);
	
	AddCloudData(iProtocol, 24, 0x0003);
	AddCloudData(iProtocol, 25, 0x0007);
	
	AddCloudData(iProtocol, 27, 0x001f);
	AddCloudData(iProtocol, 28, 0x0021);
	AddCloudData(iProtocol, 29, 0x0023);
	AddCloudData(iProtocol, 30, 0x0009);
	AddCloudData(iProtocol, 31, 0x000b);
	//其他
}

/********************************************************************
 *	名称:GetCloudProtocol_D
 *	功能描述:协议分析
 *	参数:
 *		输入参数:
 *		iAddr - 地址, pCloud - 云台数居, pData - 缓冲区
 *		输出参数:
 *		iLen - 协议长度
 *	日期:2008-05-08
 *	作者:吕立国
 *********************************************************************/
int GetCloudProtocol_D(int iAddr, CLOUDDATA *pCloud, char *pData)
{
	PBYTE pBuf = PBYTE(pData);
	int iLen = 7;
	pBuf[0] = 0xff;
	pBuf[1] = iAddr;
	pBuf[2] = pCloud->wCMD >> 8;
	pBuf[3] = pCloud->wCMD & 0xff;
	pBuf[4] = pCloud->wSpeed >> 8;
	pBuf[5] = pCloud->wSpeed & 0xff;
	pBuf[6] = 0;
	for (int i = 1; i < 6; i ++)
	{
		pBuf[6] += pBuf[i];
	}
	return iLen;
}

/********************************************************************
 *	名称:GetCloudProtocol_P
 *	功能描述:协议分析
 *	参数:
 *		输入参数:
 *		iAddr - 地址, pCloud - 云台数居, pData - 缓冲区
 *		输出参数:
 *		iLen - 协议长度
 *	日期:2008-05-08
 *	作者:吕立国
 *********************************************************************/
int GetCloudProtocol_P(int iAddr, CLOUDDATA *pCloud, char *pData)
{
	iAddr--;
	PBYTE pBuf = PBYTE(pData);
	int iLen = 8;
	pBuf[0] = 0xa0;
	pBuf[1] = iAddr;
	pBuf[2] = pCloud->wCMD >> 8;
	pBuf[3] = pCloud->wCMD & 0xff;
	pBuf[4] = pCloud->wSpeed >> 8;
	pBuf[5] = pCloud->wSpeed & 0xff;
	pBuf[6] = 0xaf;
	pBuf[7] = 0;
	for (int i = 1; i < 7; i ++)
	{
		pBuf[7] ^= pBuf[i];
	}
	return iLen;
}

/********************************************************************
 *	名称:GetCloudProtocol
 *	功能描述:协议分析
 *	参数:
 *		输入参数:
 *		iProtocol - 协议, iCtrlId - 指令Id, wCMD - 云台命令, wSpeed - 云台速度, pData - 缓冲区
 *		输出参数:
 *		iLen - 协议长度
 *	日期:2008-05-08
 *	作者:吕立国
 *********************************************************************/
int GetCloudProtocol(int iProtocol, int iAddr, int iCtrlId, WORD wSpeed, char *pData)
{
	int iLen = -1;
	CLOUDDATA *pCloud = g_cloudDataList.FindData(iProtocol, iCtrlId);
	if (pCloud == NULL)
	{
		return iLen;
	}
	
	pCloud->wSpeed = wSpeed;
	switch (iProtocol)
	{
	case 0://_D
		{
			iLen = GetCloudProtocol_D(iAddr, pCloud, pData);
		} break;
	case 1://_P
		{
			iLen = GetCloudProtocol_P(iAddr, pCloud, pData);
		} break;
	default:
		{
		} break;
	}
	return iLen;
}
