#pragma once

#include "DataGlobal.h"
#include "SingletonHelper.h"

#include <QSqlDatabase>

//==================================================================================================
class DBConnectionPoolPrivate;
class DBConnectionPool
{
public:
	QSqlDatabase getConnection();
	void releaseConnection();

private:
	SINGLETONHELPER(DBConnectionPool);
	Q_DECLARE_PRIVATE(DBConnectionPool);
	DBConnectionPoolPrivate * d_ptr;
};

//==================================================================================================

