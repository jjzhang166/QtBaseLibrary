#include "stdafx.h"
#include "DBConnectionPool.h"
#include "DBConnectionPoolManger.h"

#include <QDebug>

DBConnectionPoolManger::DBConnectionPoolManger()
{
	mapDBConnectionPool.clear();
}


DBConnectionPoolManger::~DBConnectionPoolManger()
{
	qDeleteAll(mapDBConnectionPool);
	mapDBConnectionPool.clear();
}

QSqlDatabase DBConnectionPoolManger::getConnection(const QString configDBName)
{
	if (!mapDBConnectionPool.contains(configDBName))
	{
		DBConnectionPool * pConnectPool = new DBConnectionPool(configDBName);
		mapDBConnectionPool.insert(configDBName, pConnectPool);
		return pConnectPool->getConnection();
	}

	DBConnectionPool * pConnectPool = mapDBConnectionPool.value(configDBName);
	if (pConnectPool)
	{
		return pConnectPool->getConnection();
	}

	qDebug() << "DBConnectionPool is null";
	return QSqlDatabase();
}
