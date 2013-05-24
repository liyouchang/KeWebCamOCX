/***************************************************************************
 *  ���� �� �ٽ�������ͷ�ļ���
 *  �������� �� �ٽ�����Դ��ͻ
 *  �������� �� 2008/03/31
 *  �汾�� �� ver1.00.00
 *  ����  �� ������
 *  �޸���ʷ��ԭ��
 *  Date                  Note                 Reason            Updator
 *
 ***************************************************************************/

#ifndef __CRITICALLOCK_H_
#define __CRITICALLOCK_H_

#include <Afxtempl.h>

class CriticalLock
{
public:
	CriticalLock();
	~CriticalLock();
	void Lock(); //����
	void UnLock(); //����
	
private:
	CRITICAL_SECTION m_section; //�ٽ�������
};

#endif
