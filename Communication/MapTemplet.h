/***************************************************************************
*  ���� �� mapģ���ࡣ                                               
*  �������� �� map˫����������,����list��map֮���ת��ת����ʵ��                                              
*  �������� �� 2008/03/31                                               
*  �汾�� �� ver1.00.00                                                     
*  ����  �� ����                                                         
*  �޸���ʷ��ԭ��                                                        
*  Date                  Note                 Reason            Updator 
*	
***************************************************************************/
#ifndef __MAPTEMPLET_H_ 
#define __MAPTEMPLET_H_

#include <Afxtempl.h>
#include "CriticalLock.h"

template<class T, int COUNT>
class MapTemplet
{
public:
	typedef CMap<int, int &, T*, T*>	CT_Map;
	typedef CList<T*,    T*>            CT_List;

public:
	MapTemplet();
	~MapTemplet();

private:
	CT_Map					m_T_Map;								  //map������,��list�����ж�ȡ
	CT_List					m_T_List;								  //list����

	POSITION				m_pos;									  //��ǰMapλ��
	int                     m_CurrentID;							  //��ǰMap ID
	T                      *m_CurrentT;								  //��ǰMap����ָ��

public:
	CriticalLock            m_lock;									  //�ٽ�����
	int						GetMapCount();							  //��ȡmapԪ�ظ���
	BOOL					InitData(int Count);					  //��ʼ��list����,����COUNT����������
	T*						AddData( int vid, int lid);               //��MAP�м�����
	T*						FindData(int vid, int lid);               //��MAP�в�������
	BOOL					DelData( int vid, int lid);               //��MAP��ɾ�����ݣ�list���յ�β��
	T*						AddData( int ID);                         //�Ƴ�����β���ݺ���MAP�м�����
	T*						FindData(int ID);                         //����ID�����Ҷ���
	BOOL					DelData( int ID);
	BOOL					DelAllData();                             //��MAP��ɾ����������
	T*						AddData_Unlimit( int ID);                 //��MAP����������
	T*						AddData_Unlimit( int vid, int lid);		  //��MAP�м�����,��ֹ�����������޶��������

	T*						GetFirstData();                           //��ȡMAP���һ��Ԫ��
	T*						GetNextData();                            //��ȡMAP����һ��Ԫ��
	T*						GetFirstData_Lock();					  //������ȡMAP���һ��Ԫ��
	T*						GetNextData_Lock();                       //������ȡMAP����һ��Ԫ��
};

/********************************************************************
*	����:MapTemplet																
*	��������:���캯��,Ϊlist�������														
*	����:															
*		�������:													
*		��															
*		�������:														
*		��															
*	����:2008-3-31													
*	����:����														
*********************************************************************/
template<class T, int COUNT>
MapTemplet<T, COUNT>::MapTemplet()
{
	m_T_Map.RemoveAll();
	m_T_List.RemoveAll();
	
	m_pos = NULL;
	InitData(COUNT);
}

/********************************************************************
*	����:InitData																
*	��������:��ʼ������,����COUNT����������-��������														
*	����:															
*		�������:													
*		Count - �����������															
*		�������:														
*		�ɹ�TRUE															
*	����:2008-3-31													
*	����:����														
*********************************************************************/
template<class T, int COUNT>
BOOL MapTemplet<T, COUNT>::InitData(int Count)
{
	T *t;
	m_lock.Lock();
	for (int i = 0; i < Count; i ++)
	{
		t = new T;
		m_T_List.AddTail(t);
	}
	m_lock.UnLock();

	return TRUE;
}

/********************************************************************
*	����:~MapTemplet																
*	��������:��������,�����нڵ���ܳ��������,��������Э������δ֪��														
*	����:															
*		�������:													
*		��															
*		�������:														
*		��															
*	����:2008-3-31													
*	����:����														
*********************************************************************/
template<class T, int COUNT>
MapTemplet<T, COUNT>::~MapTemplet()
{
	//���m_T_List
	T *pt;
	while (!m_T_List.IsEmpty())
	{
		pt = m_T_List.RemoveTail();
		delete pt;
	}
	
	//���m_T_Map
	pt = GetFirstData();
	while (pt)
	{
		delete pt;
		pt = GetNextData();
	}
	m_T_Map.RemoveAll();
}

/********************************************************************
 *	����:AddData
 *	��������:�Ƴ�����β���ݺ���MAP�м�����
 *	����:
 *		�������:
 *		ID - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::AddData(int ID)
{
	T *t;
	m_lock.Lock();

	if (!m_T_Map.Lookup(ID, t))
	{
		if (!m_T_List.IsEmpty())
		{
			t = m_T_List.RemoveTail();

			m_T_Map.SetAt(ID, t);
		}
		else
			t = NULL;
	}

	m_lock.UnLock();
	return t;
}

/********************************************************************
 *	����:FindData
 *	��������:��MAP�в�������
 *	����:
 *		�������:
 *		ID - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::FindData(int ID)
{
	T *t;
	m_lock.Lock();

	if (!m_T_Map.Lookup(ID, t))
	{
		t = NULL;
	}

	m_lock.UnLock();

	return t;
}

/********************************************************************
*	����:DelData																
*	��������:��MAP��ɾ������,��ӵ�list����														
*	����:															
*		�������:													
*			ID - ��ֵ															
*		�������:														
*			TRUE- �ɹ� FALSE- ʧ��															
*	����:2008-3-31													
*	����:����														
*********************************************************************/
template<class T, int COUNT>
BOOL MapTemplet<T, COUNT>::DelData(int ID)
{
	T *t;
	BOOL bDel = TRUE;
	m_lock.Lock();

	if (m_T_Map.Lookup(ID, t))
	{
		m_pos = NULL;
		m_T_Map.RemoveKey(ID);
		m_T_List.AddTail(t);
	}
	else
	{
		bDel = FALSE;
	}

	m_lock.UnLock();
	return bDel;
}

/********************************************************************
 *	����:DelAllData
 *	��������:��MAP��ɾ����������
 *	����:
 *		�������:
 *		��
 *		�������:
 *		�ɹ�TRUE, ʧ��FALSE
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
BOOL MapTemplet<T, COUNT>::DelAllData()
{
	BOOL bDel = TRUE;
	m_lock.Lock();

	T *t = GetFirstData();
	if (!t)
	{
		bDel = FALSE;
	}
	while (t)
	{
		m_T_List.AddTail(t);
		t = GetNextData();
	}

	m_T_Map.RemoveAll();
	m_lock.UnLock();
	return bDel;
}

/********************************************************************
 *	����:AddData
 *	��������:��MAP�м�����
 *	����:
 *		�������:
 *		vid, lid - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::AddData(int vid, int lid)
{
	//<<8 = *256
	int ID = (vid << 8) + lid;
	T *t = AddData(ID);
	return t;
}

/********************************************************************
 *	����:FindData
 *	��������:��MAP�в�������
 *	����:
 *		�������:
 *		vid, lid - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::FindData(int vid, int lid)
{
	int ID = (vid << 8) + lid;
	T *t = FindData(ID);
	return t;
}

/********************************************************************
 *	����:DelData
 *	��������:��MAP��ɾ������
 *	����:
 *		�������:
 *		vid, lid - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
BOOL MapTemplet<T, COUNT>::DelData(int vid, int lid)
{
	int ID = (vid << 8) + lid;
	BOOL iRes = DelData(ID);
	return iRes;
}

/********************************************************************
 *	����:AddData_Unlimit
 *	��������:��MAP�м�����,��ֹ�����������޶��������
 *	����:
 *		�������:
 *		ID - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::AddData_Unlimit( int ID)
{
	T *t= NULL;
	m_lock.Lock();

	if (!m_T_Map.Lookup(ID, t))
	{
		if (m_T_List.IsEmpty())
			t = new T;
		else
			t = m_T_List.RemoveTail();
		m_T_Map.SetAt(ID, t);
	}

	m_lock.UnLock();
	return t;
}

/********************************************************************
 *	����:AddData_Unlimit
 *	��������:��MAP�м�����
 *	����:
 *		�������:
 *		ID - ��ֵ
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::AddData_Unlimit( int vid, int lid)
{
	int ID = (vid << 8) + lid;
	T *t = AddData_Unlimit(ID);
	return t;
}

/********************************************************************
 *	����:GetFirstData
 *	��������:��ȡmap��һ��Ԫ��
 *	����:
 *		�������:
 *		��
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::GetFirstData()
{
	m_CurrentT = NULL;
	m_pos = m_T_Map.GetStartPosition();
	if (m_pos)
	{
		m_T_Map.GetNextAssoc(m_pos, m_CurrentID, m_CurrentT);
	}
	return m_CurrentT;
}

/********************************************************************
 *	����:GetNextData
 *	��������:��ȡmap��һ��Ԫ��
 *	����:
 *		�������:
 *		��
 *		�������:
 *		���ر�������ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::GetNextData()
{
	m_CurrentT = NULL;
	if (m_pos)
	{
		m_T_Map.GetNextAssoc(m_pos, m_CurrentID, m_CurrentT);
	}
	return m_CurrentT;
}

/********************************************************************
 *	����:GetRecCount
 *	��������:����map��Ԫ�ظ���
 *	����:
 *		�������:
 *		��
 *		�������:
 *		Ԫ�ظ���
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
int	MapTemplet<T, COUNT>::GetMapCount()
{
	m_lock.Lock();
	int iCount = m_T_Map.GetCount();
	m_lock.UnLock();
	return iCount;
}

/********************************************************************
 *	����:GetFirstData_Lock
 *	��������:��ȡmap��һ��Ԫ��
 *	����:
 *		�������:
 *		��
 *		�������:
 *		������ӵĲ���ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::GetFirstData_Lock()
{
	m_lock.Lock();
	m_CurrentT = NULL;
	m_pos = m_T_Map.GetStartPosition();
	if (m_pos)
	{
		m_T_Map.GetNextAssoc(m_pos, m_CurrentID, m_CurrentT);
	}
	m_lock.UnLock();
	return m_CurrentT;
}

/********************************************************************
 *	����:GetNextData_Lock
 *	��������:��ȡmap��һ��Ԫ��
 *	����:
 *		�������:
 *		��
 *		�������:
 *		���ر�������ָ��
 *	����:2008-3-31
 *	����:����
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::GetNextData_Lock()
{
	m_lock.Lock();
	m_CurrentT = NULL;
	if (m_pos)
	{
		m_T_Map.GetNextAssoc(m_pos, m_CurrentID, m_CurrentT);
	}
	m_lock.UnLock();
	return m_CurrentT;
}

#endif
