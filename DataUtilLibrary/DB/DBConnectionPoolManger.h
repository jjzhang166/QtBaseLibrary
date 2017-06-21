#pragma once

#include "DataGlobal.h"
#include "SingletonHelper.h"

#include <QMap>
#include <QSqlDatabase>

//==================================================================================================
class DBConnectionPool;
class DBConnectionPoolManger
{
	SINGLETONHELPER(DBConnectionPoolManger);

public:
	QSqlDatabase getConnection(const QString configDBName = "db");
	void releaseConnection(const QSqlDatabase &connection, const QString configDBName = "db");

private:
	QMap<QString, DBConnectionPool *> mapDBConnectionPool;
};