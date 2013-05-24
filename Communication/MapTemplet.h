/***************************************************************************
*  标题 ： map模版类。                                               
*  功能描述 ： map双缓冲区处理,利用list和map之间的转入转出来实现                                              
*  创建日期 ： 2008/03/31                                               
*  版本号 ： ver1.00.00                                                     
*  作者  ： 王朋                                                         
*  修改历史及原因                                                        
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
	CT_Map					m_T_Map;								  //map缓冲区,从list链表中读取
	CT_List					m_T_List;								  //list链表

	POSITION				m_pos;									  //当前Map位置
	int                     m_CurrentID;							  //当前Map ID
	T                      *m_CurrentT;								  //当前Map变量指针

public:
	CriticalLock            m_lock;									  //临界区类
	int						GetMapCount();							  //获取map元素个数
	BOOL					InitData(int Count);					  //初始化list数据,增加COUNT个链表数据
	T*						AddData( int vid, int lid);               //向MAP中加数据
	T*						FindData(int vid, int lid);               //在MAP中查找数据
	BOOL					DelData( int vid, int lid);               //在MAP中删除数据，list回收到尾部
	T*						AddData( int ID);                         //移除链表尾数据后向MAP中加数据
	T*						FindData(int ID);                         //根据ID来查找对象
	BOOL					DelData( int ID);
	BOOL					DelAllData();                             //在MAP中删除所有数据
	T*						AddData_Unlimit( int ID);                 //在MAP中增加数据
	T*						AddData_Unlimit( int vid, int lid);		  //向MAP中加数据,防止请求数超过限定的最大数

	T*						GetFirstData();                           //获取MAP表第一个元素
	T*						GetNextData();                            //获取MAP表下一个元素
	T*						GetFirstData_Lock();					  //加锁获取MAP表第一个元素
	T*						GetNextData_Lock();                       //加锁获取MAP表下一个元素
};

/********************************************************************
*	名称:MapTemplet																
*	功能描述:构造函数,为list填充数据														
*	参数:															
*		输入参数:													
*		无															
*		输出参数:														
*		无															
*	日期:2008-3-31													
*	作者:王朋														
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
*	名称:InitData																
*	功能描述:初始化数据,增加COUNT个链表数据-操作链表														
*	参数:															
*		输入参数:													
*		Count - 分配变量个数															
*		输出参数:														
*		成功TRUE															
*	日期:2008-3-31													
*	作者:王朋														
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
*	名称:~MapTemplet																
*	功能描述:析构函数,链表中节点可能超出最大数,并存的最大协议数是未知的														
*	参数:															
*		输入参数:													
*		无															
*		输出参数:														
*		无															
*	日期:2008-3-31													
*	作者:王朋														
*********************************************************************/
template<class T, int COUNT>
MapTemplet<T, COUNT>::~MapTemplet()
{
	//清空m_T_List
	T *pt;
	while (!m_T_List.IsEmpty())
	{
		pt = m_T_List.RemoveTail();
		delete pt;
	}
	
	//清空m_T_Map
	pt = GetFirstData();
	while (pt)
	{
		delete pt;
		pt = GetNextData();
	}
	m_T_Map.RemoveAll();
}

/********************************************************************
 *	名称:AddData
 *	功能描述:移除链表尾数据后向MAP中加数据
 *	参数:
 *		输入参数:
 *		ID - 键值
 *		输出参数:
 *		返回添加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:FindData
 *	功能描述:在MAP中查找数据
 *	参数:
 *		输入参数:
 *		ID - 键值
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
*	名称:DelData																
*	功能描述:在MAP中删除数据,添加到list表中														
*	参数:															
*		输入参数:													
*			ID - 键值															
*		输出参数:														
*			TRUE- 成功 FALSE- 失败															
*	日期:2008-3-31													
*	作者:王朋														
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
 *	名称:DelAllData
 *	功能描述:在MAP中删除所有数据
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		成功TRUE, 失败FALSE
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:AddData
 *	功能描述:向MAP中加数据
 *	参数:
 *		输入参数:
 *		vid, lid - 键值
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:FindData
 *	功能描述:在MAP中查找数据
 *	参数:
 *		输入参数:
 *		vid, lid - 键值
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::FindData(int vid, int lid)
{
	int ID = (vid << 8) + lid;
	T *t = FindData(ID);
	return t;
}

/********************************************************************
 *	名称:DelData
 *	功能描述:在MAP中删除数据
 *	参数:
 *		输入参数:
 *		vid, lid - 键值
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
 *********************************************************************/
template<class T, int COUNT>
BOOL MapTemplet<T, COUNT>::DelData(int vid, int lid)
{
	int ID = (vid << 8) + lid;
	BOOL iRes = DelData(ID);
	return iRes;
}

/********************************************************************
 *	名称:AddData_Unlimit
 *	功能描述:向MAP中加数据,防止请求数超过限定的最大数
 *	参数:
 *		输入参数:
 *		ID - 键值
 *		输出参数:
 *		返回添加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:AddData_Unlimit
 *	功能描述:向MAP中加数据
 *	参数:
 *		输入参数:
 *		ID - 键值
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
 *********************************************************************/
template<class T, int COUNT>
T* MapTemplet<T, COUNT>::AddData_Unlimit( int vid, int lid)
{
	int ID = (vid << 8) + lid;
	T *t = AddData_Unlimit(ID);
	return t;
}

/********************************************************************
 *	名称:GetFirstData
 *	功能描述:获取map第一个元素
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:GetNextData
 *	功能描述:获取map下一个元素
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		返回变量参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:GetRecCount
 *	功能描述:返回map中元素个数
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		元素个数
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:GetFirstData_Lock
 *	功能描述:获取map第一个元素
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		返回天加的参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
 *	名称:GetNextData_Lock
 *	功能描述:获取map下一个元素
 *	参数:
 *		输入参数:
 *		无
 *		输出参数:
 *		返回变量参数指针
 *	日期:2008-3-31
 *	作者:王朋
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
