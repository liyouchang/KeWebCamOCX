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