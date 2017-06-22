#include "stdafx.h"
#include "SysConfig.h"

//==================================================================================================
SysConfig::SysConfig()
{
	pJsonConfig = NULL;
}


SysConfig::~SysConfig()
{
	destory();
}

//==================================================================================================
void SysConfig::destory()
{
	CHK_POINT_DESTORY(pJsonConfig)
}

bool SysConfig::isIntit()
{
	if (pJsonConfig != NULL)
		return true;
	return false;
}

void SysConfig::init(const QString & path)
{
	pJsonConfig = new JsonConfigHandle(path);
}

//==================================================================================================
QStringList SysConfig::getStringList(const QString & attrName) const
{
	if (pJsonConfig)
		return  pJsonConfig->getStringList(attrName);

	return QStringList();
}

int SysConfig::getInt(const QString & attrName, int default) const
{
	if (pJsonConfig)
		return  pJsonConfig->getInt(attrName);

	return default;
}

bool SysConfig::getBool(const QString & attrName, bool default) const
{
	if (pJsonConfig)
		return  pJsonConfig->getBool(attrName);

	return default;
}

double SysConfig::getDouble(const QString & attrName, double default) const
{
	if (pJsonConfig)
		return  pJsonConfig->getDouble(attrName);

	return default;
}

QString SysConfig::getString(const QString & attrName, QString default) const
{
	if (pJsonConfig)
		return  pJsonConfig->getString(attrName);

	return default;
}


//==================================================================================================
QString SysConfig::getDBType(const QString & dbName) const
{
	QString attrName = QString("%1.type").arg(dbName);
	return getString(attrName, "QODBC");
}

QString SysConfig::getDBHost(const QString & dbName) const
{
	QString attrName = QString("%1.host").arg(dbName);
	return getString(attrName, "127.0.0.1");
}

QString SysConfig::getDBName(const QString & dbName) const
{
	QString attrName = QString("%1.dbname").arg(dbName);
	return getString(attrName, "sql_server");
}

QString SysConfig::getDBUserName(const QString & dbName) const
{
	QString attrName = QString("%1.username").arg(dbName);
	return getString(attrName, "sa");
}

QString SysConfig::getDBPassword(const QString & dbName) const
{
	QString attrName = QString("%1.password").arg(dbName);
	return getString(attrName, "1");
}

QString SysConfig::getDBConnectionName(const QString & dbName) const
{
	QString attrName = QString("%1.connection_name").arg(dbName);
	return getString(attrName, "default");
}

QString SysConfig::getDBTestConnectSQL(const QString & dbName) const
{
	QString attrName = QString("%1.test_sql").arg(dbName);
	return getString(attrName, "select 1");
}

int SysConfig::getDBPort(const QString & dbName) const
{
	QString attrName = QString("%1.port").arg(dbName);
	return getInt(attrName, 0);
}

int SysConfig::getDBMaxWaitTime(const QString & dbName) const
{
	QString attrName = QString("%1.max_wait_time").arg(dbName);
	return getInt(attrName, 5000);
}

int SysConfig::getDBMaxConnectionCount(const QString & dbName) const
{
	QString attrName = QString("%1.max_connection_count").arg(dbName);
	return getInt(attrName, 5);
}

bool SysConfig::isDBTestOnConnect(const QString & dbName) const
{
	QString attrName = QString("%1.test_on_connect").arg(dbName);
	return getBool(attrName, true);
}

bool SysConfig::isDBOutPutDebugSQL(const QString & dbName) const
{
	QString attrName = QString("%1.output_sql").arg(dbName);
	return getBool(attrName, false);
}

QStringList SysConfig::getDBSqlFiles(const QString & dbName) const
{
	QString attrName = QString("%1.sql_files").arg(dbName);
	return getStringList(attrName);
}