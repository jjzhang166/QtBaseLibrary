#include "stdafx.h"
#include "SysConfig.h"
#include "DBConnectionPool.h"

#include <QDebug>
#include <QQueue>
#include <QSqlError>
#include <QSqlQuery>
#include <QSemaphore>

#include <QTime>


//==================================================================================================
class DBConnectionPoolPrivate
{
public:
	DBConnectionPoolPrivate(const QString & configDBName);
	~DBConnectionPoolPrivate();

	QSqlDatabase getConnection();
	void closeConnections();

private:
	void initConnections();
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

	bool bDefaultConnection;			//程序中的默认的数据库连接，设置多次会被后面的覆盖
	bool bTestVaild;					//取得连接时验证连接是否有效（断线重连功能）
	int iMaxWaitTime;					//获取连接最大等待时间
	int iMaxConnectionCount;			//最大连接数
	QString strTestSql;					//测试访问数据库使用的SQL

	QQueue<QString> queueConnectionNames;		//存储数据库连接名的队列

	QMutex mutex;
	static int iLastKey;						//连接名称序号， 保证连接名字不重复
};

int DBConnectionPoolPrivate::iLastKey = 0;
//==================================================================================================
DBConnectionPoolPrivate::DBConnectionPoolPrivate(const QString & configDBName)
{
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
		bDefaultConnection = pConfig->isDBDefaultConnection(configDBName);

		//初始化连接；
		QTime time;
		time.start();
		initConnections();
		qDebug() << endl << "Init Use Time:"  << time.elapsed() / 1000.0 << "s";
	}
	
}

DBConnectionPoolPrivate::~DBConnectionPoolPrivate()
{
	closeConnections();
}

//==================================================================================================
QSqlDatabase DBConnectionPoolPrivate::getConnection()
{
	while (queueConnectionNames.count() > 0)
	{
		//有回收的连接则使用新连接， 没有则新创建连接
		mutex.lock();
		QString connectionName = queueConnectionNames.dequeue();
		mutex.unlock();

		const QSqlDatabase & db = getConnection(connectionName);
		if (!db.isOpen())
		{
			qDebug() << "Failed to open DB. ConnectionName: " << connectionName;
		}

		mutex.lock();
		queueConnectionNames.enqueue(connectionName);
		mutex.unlock();

		return db;
	}

	initConnections();
	//创建连接超时， 返回一个无效连接
	qDebug() << "Time out to get connection.";
	return QSqlDatabase();
}

void DBConnectionPoolPrivate::closeConnections()
{
	while (queueConnectionNames.size() > 0)
	{
		mutex.lock();
		QSqlDatabase db = QSqlDatabase::database(queueConnectionNames.dequeue());
		mutex.unlock();

		if (db.isOpen())
			db.close();
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

void DBConnectionPoolPrivate::initConnections()
{
	const QString & connectionName = QString("%1_%2").arg(strDBConnectionName).arg(iLastKey++);
	const QSqlDatabase & db = createConnection(connectionName);

	if (db.isOpen())
	{
		//设置默认连接
		if (bDefaultConnection)
		{
			QSqlDatabase::cloneDatabase(db, QSqlDatabase::defaultConnection);
		}

		queueConnectionNames.enqueue(connectionName);
	}
	
	while (iLastKey < iMaxConnectionCount)
	{
		QSqlDatabase dbClone;
		const QString & cloneName = QString("%1_%2").arg(strDBConnectionName).arg(iLastKey++);
		if (db.isValid())
		{
			dbClone = QSqlDatabase::cloneDatabase(db, cloneName);
		}

		if (!dbClone.isOpen())
		{
			dbClone = createConnection(cloneName);
		}

		if (dbClone.isOpen())
		{
			queueConnectionNames.enqueue(cloneName);
		}
	}

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
	QTime times;
	times.start();
	Q_D(DBConnectionPool);
	QSqlDatabase db =  d->getConnection();

	qDebug() << endl << "get Connection Time:" << times.elapsed() / 1000.0 << "s";

	return db;
}

//==================================================================================================















//==================================================================================================
