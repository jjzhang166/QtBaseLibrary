#pragma once

#include "SingletonHelper.h"
#include "JsonConfigHandle.h"

/************************************************************************
*	用于读写配置文件， 此类可重载读写自定义的配置文件。
*	配置文件使用json格式。                                                                
*************************************************************************/


//==================================================================================================
class SysConfig
{
	SINGLETONHELPER(SysConfig);
public:
	//销毁与初始化
	void destory();
	bool isIntit();

	//使用配置文件必须调用此函数进行初始化，初始化一次即可。
	/************************************************************************
	*	调用方法：(path 为配置文件路径)
	*	Singleton<SysConfig>::init(path);
	*************************************************************************/
	void init(const QString & path);
	
	//==================================================================================================
	//基本读取配置文件函数
	QStringList getStringList(const QString & attrName) const;
	int getInt(const QString & attrName, int default = 0) const;
	bool getBool(const QString & attrName, bool default = false) const;
	double getDouble(const QString & attrName, double default = 0.0) const;
	QString getString(const QString & attrName, QString default = QString()) const;


	//==================================================================================================
	//自定义读取函数(数据库配置读取), 支持连接多个数据库
	QString getDBType(const QString & dbName = "db") const;
	QString getDBHost(const QString & dbName = "db") const;
	QString getDBName(const QString & dbName = "db") const;
	QString getDBUserName(const QString & dbName = "db") const;
	QString getDBPassword(const QString & dbName = "db") const;
	QString getDBConnectionName(const QString & dbName = "db") const;
	QString getDBTestConnectSQL(const QString & dbName = "db") const;

	int getDBPort(const QString & dbName = "db") const;
	int getDBMaxWaitTime(const QString & dbName = "db") const;
	int getDBMaxConnectionCount(const QString & dbName = "db") const;

	bool isDBTestOnConnect(const QString & dbName = "db") const;
	bool isDBOutPutDebugSQL(const QString & dbName = "db") const;

	QStringList getDBSqlFiles(const QString & dbName = "db") const;


protected:
	JsonConfigHandle * pJsonConfig;
};

//==================================================================================================

