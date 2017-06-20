#include "stdafx.h"
#include "DBConnectionPool.h"

#include <QStack>
#include <QSemaphore>


//==================================================================================================
class DBConnectionPoolPrivate
{
public:
	DBConnectionPoolPrivate();
	~DBConnectionPoolPrivate();

	//数据库信息
	QString strHostName;
	QString strDBName;
	QString strUserName;
	QString strPassword;
	QString strDBType;
	int iPort;

	bool bTestVaild;					//取得连接时验证连接是否有效（断线重连功能）
	int iMaxWaitTime;					//获取连接最大等待时间
	int iMaxConnectionCount;			//最大连接数
	QString strTestSql;					//测试访问数据库使用的SQL

	QSemaphore * pSemaphore;					//互斥信号量
	QStack<QString> usedConnectionNames;		//已使用的连接	
	QStack<QString> unUsedConnectionNames;		//未使用的连接

	QMutex mutex;
	int iLastKey;						//连接名称序号， 保证连接名字不重复
};


//==================================================================================================
DBConnectionPoolPrivate::DBConnectionPoolPrivate()
{
	iPort = 0;
	iLastKey = 0;
	iMaxConnectionCount = 0;

	pSemaphore = new QSemaphore(iMaxConnectionCount);
}

DBConnectionPoolPrivate::~DBConnectionPoolPrivate()
{

}















//==================================================================================================




















//==================================================================================================
DBConnectionPool::DBConnectionPool()
{
}


DBConnectionPool::~DBConnectionPool()
{
}
