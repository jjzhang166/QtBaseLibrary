#include "stdafx.h"
#include "SysConfig.h"
#include "DBConnectionPool.h"

#include <QDebug>
#include <QStack>
#include <QSqlError>
#include <QSqlQuery>
#include <QSemaphore>


//==================================================================================================
class DBConnectionPoolPrivate
{
public:
	DBConnectionPoolPrivate(const QString & configDBName);
	~DBConnectionPoolPrivate();

	QSqlDatabase getConnection();

	void closeConnections();
	void releaseConnection(const QSqlDatabase &connection);

private:
	QSqlDatabase getConnection(const QString & connectionName);
	QSqlDatabase createConnection(const QString & connectionName);

	//数据库信息
	QString strHostName;
	QString strDBName;
	QString strUserName;
	QString strPassword;
	QString strDBType;
	QString strDBConnectionName;
	int iPort;

	bool bTestVaild;					//取得连接时验证连接是否有效（断线重连功能）
	int iMaxWaitTime;					//获取连接最大等待时间
	int iMaxConnectionCount;			//最大连接数
	QString strTestSql;					//测试访问数据库使用的SQL

	QSemaphore * pSemaphore;					//互斥信号量
	QStack<QString> usedConnectionNames;		//已使用的连接	
	QStack<QString> unUsedConnectionNames;		//未使用的连接

	QMutex mutex;
	static int iLastKey;								//连接名称序号， 保证连接名字不重复
};

int DBConnectionPoolPrivate::iLastKey = 0;
//==================================================================================================
DBConnectionPoolPrivate::DBConnectionPoolPrivate(const QString & configDBName)
{
	pSemaphore = NULL;
	iPort = 0;
	iMaxWaitTime = 0;
	iMaxConnectionCount = 0;

	SysConfig * pConfig = SingletonHelper<SysConfig>::getInstance();
	if (pConfig != NULL && pConfig->isIntit())
	{
		strDBName = pConfig->getDBName(configDBName);
		strDBType = pConfig->getDBType(configDBName);
		strHostName = pConfig->getDBHost(configDBName);
		strUserName = pConfig->getDBUserName(configDBName);
		strPassword = pConfig->getDBPassword(configDBName);
		strTestSql = pConfig->getDBTestConnectSQL(configDBName);
		strDBConnectionName = pConfig->getDBConnectionName(configDBName);

		iPort = pConfig->getDBPort(configDBName);
		iMaxWaitTime = pConfig->getDBMaxWaitTime(configDBName);
		iMaxConnectionCount = pConfig->getDBMaxConnectionCount(configDBName);
		bTestVaild = pConfig->isDBTestOnConnect(configDBName);

		pSemaphore = new QSemaphore(iMaxConnectionCount);
	}
	
}

DBConnectionPoolPrivate::~DBConnectionPoolPrivate()
{
	closeConnections();
	CHK_POINT_DESTORY(pSemaphore);
}

//==================================================================================================
QSqlDatabase DBConnectionPoolPrivate::getConnection()
{
	if (pSemaphore && pSemaphore->tryAcquire(1, iMaxWaitTime))
	{
		//有回收的连接则使用新连接， 没有则新创建连接
		mutex.lock();
		QString connectionName = unUsedConnectionNames.size() > 0 ?
			unUsedConnectionNames.pop() : QString("%1_%2").arg(strDBConnectionName).arg(iLastKey++);

		usedConnectionNames.push(connectionName);
		mutex.unlock();

		QSqlDatabase db = getConnection(connectionName);
		if (!db.isOpen())
		{
			mutex.lock();
			usedConnectionNames.removeOne(connectionName);
			mutex.unlock();
			pSemaphore->release();
		}

		return db;
	}
	else
	{
		//创建连接超时， 返回一个无效连接
		qDebug() << "Time out to get connection.";
		return QSqlDatabase();
	}
}

void DBConnectionPoolPrivate::closeConnections()
{
	while (unUsedConnectionNames.size() > 0)
	{
		mutex.lock();
		QSqlDatabase db = QSqlDatabase::database(unUsedConnectionNames.pop());
		mutex.unlock();

		if (db.isOpen())
			db.close();
	}

	while (usedConnectionNames.size() > 0)
	{
		mutex.lock();
		QSqlDatabase db = QSqlDatabase::database(usedConnectionNames.pop());
		mutex.unlock();

		if (db.isOpen())
			db.close();

		pSemaphore->release();
	}

}

void DBConnectionPoolPrivate::releaseConnection(const QSqlDatabase &connection)
{
	QString connectionName = connection.connectionName();
	if (usedConnectionNames.contains(connectionName))
	{
		mutex.lock();
		usedConnectionNames.removeOne(connectionName);
		unUsedConnectionNames.push(connectionName);
		mutex.unlock();

		pSemaphore->release();

	}
}

//==================================================================================================
QSqlDatabase DBConnectionPoolPrivate::getConnection(const QString & connectionName)
{
	if (QSqlDatabase::contains(connectionName))
	{
		//连接已创建， 复用连接
		QSqlDatabase dbConnection = QSqlDatabase::database(connectionName);

		if (bTestVaild)
		{
			//测试连接是否有效， 断线重连。
			QSqlQuery query(strTestSql, dbConnection);
			if (query.lastError().type() != QSqlError::NoError && !dbConnection.open())
			{
				qDebug() << QString("Test connection Failed, executeSQL: %1; connectionName: %2; queryError: %3; dbError: %4.")
					.arg(strTestSql).arg(connectionName).arg(query.lastError().text()).arg(dbConnection.lastError().text());

				return createConnection(connectionName);
			}
		}
		return dbConnection;
	}

	return createConnection(connectionName);
}

QSqlDatabase DBConnectionPoolPrivate::createConnection(const QString & connectionName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase(strDBType, connectionName);
	db.setHostName(strHostName);
	db.setDatabaseName(strDBName);
	db.setUserName(strUserName);
	db.setPassword(strPassword);

	if (iPort != 0)
		db.setPort(iPort);

	if (!db.open())
	{
		qDebug() << "Open database error:" << db.lastError().text();
	}

	return db;
}


//==================================================================================================
DBConnectionPool::DBConnectionPool(const QString configDBName)
{
	d_ptr = new DBConnectionPoolPrivate(configDBName);
}

DBConnectionPool::~DBConnectionPool()
{
	destory();
}

//==================================================================================================
void DBConnectionPool::destory()
{
	d_ptr->closeConnections();
	CHK_POINT_DESTORY(d_ptr);
}

QSqlDatabase DBConnectionPool::getConnection()
{
	Q_D(DBConnectionPool);
	return d->getConnection();
}

void DBConnectionPool::releaseConnection(const QSqlDatabase &connection)
{
	Q_D(DBConnectionPool);
	d->releaseConnection(connection);
}




//==================================================================================================















//==================================================================================================
