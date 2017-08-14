#pragma once

#include "SingletonHelper.h"

#include <QSqlDatabase>


//==================================================================================================
class DBConnectionPoolPrivate;
class DBConnectionPool
{
public:
	DBConnectionPool(const QString configDBName = "db");
	~DBConnectionPool();

public:
	void destory();
	QSqlDatabase getConnection();

private:
	Q_DECLARE_PRIVATE(DBConnectionPool);
	DBConnectionPoolPrivate * d_ptr;
};

//==================================================================================================

