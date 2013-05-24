/***************************************************************************
 *  ��    ��: ��̨����
 *  ��������: ���������̨Э��
 *  ��������: 2008-05-08
 *  �� �� ��: ver1.00.00
 *  ��    ��: ������
 *  �޸���ʷ��ԭ��
 *  Date                  Note                 Reason            Updator
 *
 ***************************************************************************/
#include "StdAfx.h"
#include "CloudCtrl.h"
#include "MapTemplet.h"

//��������
MapTemplet<CLOUDDATA, 0> g_cloudDataList;

/********************************************************************
 *	����:AddCloudData
 *	��������:����̨����
 *	����:
 *		�������:
 *		iProtocol - Э��, iCtrlId - ָ��Id, wCMD - ��̨����, wSpeed - ��̨�ٶ�
 *		�������:
 *		��
 *	����:2008-05-08
 *	����:������
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
 *	����:IniCloudData
 *	��������:��ʼ����̨����
 *	����:
 *		�������:
 *		��
 *		�������:
 *		��
 *	����:2008-05-08
 *	����:������
 *********************************************************************/
void IniCloudData()
{
	int iProtocol = 0;
	//_DЭ��
	//��
	AddCloudData(iProtocol,  0, 0x0008);
	//��
	AddCloudData(iProtocol,  1, 0x0010);
	//��
	AddCloudData(iProtocol,  2, 0x0004);
	//��
	AddCloudData(iProtocol,  3, 0x0002);
	//��Ȧ��
	AddCloudData(iProtocol,  4, 0x0400);
	//��Ȧ��
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
	
	//_PЭ��
	iProtocol = 1;
	AddCloudData(iProtocol,  0, 0x0008);//��
	AddCloudData(iProtocol,  1, 0x0010);//��
	AddCloudData(iProtocol,  2, 0x0004);//��
	AddCloudData(iProtocol,  3, 0x0002);//��
	
	AddCloudData(iProtocol,  4, 0x0800);//��Ȧ��
	AddCloudData(iProtocol,  5, 0x0400);//��Ȧ��
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
	//����
}

/********************************************************************
 *	����:GetCloudProtocol_D
 *	��������:Э�����
 *	����:
 *		�������:
 *		iAddr - ��ַ, pCloud - ��̨����, pData - ������
 *		�������:
 *		iLen - Э�鳤��
 *	����:2008-05-08
 *	����:������
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
 *	����:GetCloudProtocol_P
 *	��������:Э�����
 *	����:
 *		�������:
 *		iAddr - ��ַ, pCloud - ��̨����, pData - ������
 *		�������:
 *		iLen - Э�鳤��
 *	����:2008-05-08
 *	����:������
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
 *	����:GetCloudProtocol
 *	��������:Э�����
 *	����:
 *		�������:
 *		iProtocol - Э��, iCtrlId - ָ��Id, wCMD - ��̨����, wSpeed - ��̨�ٶ�, pData - ������
 *		�������:
 *		iLen - Э�鳤��
 *	����:2008-05-08
 *	����:������
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
