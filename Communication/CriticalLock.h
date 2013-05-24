/***************************************************************************
 *  标题 ： 临界区锁类头文件。
 *  功能描述 ： 临界区资源冲突
 *  创建日期 ： 2008/03/31
 *  版本号 ： ver1.00.00
 *  作者  ： 吕立国
 *  修改历史及原因
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
	void Lock(); //加锁
	void UnLock(); //解锁
	
private:
	CRITICAL_SECTION m_section; //临界区变量
};

#endif
