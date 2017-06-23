#include "stdafx.h"
#include "SysConfig.h"
#include "DatabaseUtil.h"
#include "DBConnectionPoolManger.h"


bool DataBaseUtil::initDBConfig(const QString & filePath)
{
	SysConfig * pDBConfig = SingletonHelper<SysConfig>::getInstance();
	pDBConfig->init(filePath);
	return pDBConfig->isIntit();
}

int DataBaseUtil::insert(const DBUtilArguments & args)
{
	int id = -1;

	executeSql(args, [&id](QSqlQuery *query) 
	{
		id = query->lastInsertId().toInt(); // 插入行的主键
	});

	return id;
}

bool DataBaseUtil::update(const DBUtilArguments & args)
{
	bool result;

	executeSql(args, [&result](QSqlQuery *query) 
	{
		result = query->lastError().type() == QSqlError::NoError;
	});

	return result;
}

int DataBaseUtil::selectInt(const DBUtilArguments & args)
{
	return selectVariant(args).toInt();
}

qint64 DataBaseUtil::selectInt64(const DBUtilArguments & args)
{
	return selectVariant(args).toLongLong();
}

QString DataBaseUtil::selectString(const DBUtilArguments & args)
{
	return selectVariant(args).toString();
}

QDate DataBaseUtil::selectDate(const DBUtilArguments & args)
{
	return selectVariant(args).toDate();
}

QDateTime DataBaseUtil::selectDateTime(const DBUtilArguments & args)
{
	return selectVariant(args).toDateTime();
}

QVariant DataBaseUtil::selectVariant(const DBUtilArguments & args)
{
	QVariant result;

	executeSql(args, [&result](QSqlQuery *query) 
	{
		if (query->next()) 
		{
			result = query->value(0);
		}
	});

	return result;
}

QStringList DataBaseUtil::selectStrings(const DBUtilArguments & args) 
{
	QStringList strings;

	executeSql(args, [&strings](QSqlQuery *query) 
	{
		while (query->next()) 
		{
			strings.append(query->value(0).toString());
		}
	});

	return strings;
}

QVariantMap DataBaseUtil::selectMap(const DBUtilArguments & args)
{
	QVariantMap map;
	
	executeSql(args, [&map](QSqlQuery *query)
	{
		QStringList fieldNames = getFieldNames(*query);
		while (query->next())
		{
			foreach(const QString &fieldName, fieldNames)
			{
				map.insert(fieldName, query->value(fieldName));
			}

			break;
		}
	});

	return map;
}

QList<QVariantMap> DataBaseUtil::selectMaps(const DBUtilArguments & args)
{
	QList<QVariantMap> maps;

	executeSql(args, [&maps](QSqlQuery *query) 
	{
		queryToMaps(query, maps);
	});

	return maps;
}


QRecord DataBaseUtil::selectRecord(const DBUtilArguments & args)
{
	QRecord record;

	executeSql(args, [&record](QSqlQuery * query)
	{
		queryToRecords(query, record);
	});

	return record;
}



void DataBaseUtil::bindValues(QSqlQuery *query, const QVariantMap &params) 
{
	for (QVariantMap::const_iterator i = params.constBegin(); i != params.constEnd(); ++i) 
	{
		query->bindValue(":" + i.key(), i.value());
	}
}

QStringList DataBaseUtil::getFieldNames(const QSqlQuery &query) 
{
	QSqlRecord record = query.record();
	QStringList names;
	int count = record.count();

	for (int i = 0; i < count; ++i) 
	{
		names << record.fieldName(i);
	}

	return names;
}

 void DataBaseUtil::queryToMaps(QSqlQuery *query, QList<QVariantMap> & maps)
{
	QStringList fieldNames = getFieldNames(*query);

	while (query->next()) 
	{
		QVariantMap rowMap;

		foreach(const QString &fieldName, fieldNames) 
		{
			rowMap.insert(fieldName, query->value(fieldName));
		}

		maps.append(rowMap);
	}
}

void DataBaseUtil::queryToRecords(QSqlQuery *query, QRecord & record)
{
	QStringList fieldNames = getFieldNames(*query);

	//首先初始化表头
	record.clear();
	const QSqlRecord & fields = query->record();
	int colCount = fields.count();
	for (int col = 0; col < colCount; ++col)
	{
		const QSqlField & field = fields.field(col);

		ColumnInfo info;
		info.Name = field.name();
		info.Type = field.type();
		info.Length = field.length();
		record.addCol(info);
	}

	//加载数据
	while (query->next())
	{
		QVariantList rowData;
		for(int col = 0; col < colCount; ++col)
		{
			rowData.append(query->value(col));
		}
		record.addRow(rowData);
	}
}

void DataBaseUtil::debug(const QSqlQuery &query, const QVariantMap &params) 
{
	if (SingletonHelper<SysConfig>::getInstance()->isDBOutPutDebugSQL()) 
	{
		if (query.lastError().type() != QSqlError::NoError) 
		{
			qDebug() << "    => SQL Error: " << query.lastError().text().trimmed();
		}

		qDebug() << "    => SQL Query:" << query.lastQuery();

		if (params.size() > 0) 
		{
			qDebug() << "    => SQL Params: " << params;
		}
	}
}

void DataBaseUtil::executeSql(const DBUtilArguments & args, std::function<void(QSqlQuery *query)> handleResult) 
{
	DBConnectionPoolManger * dbManger = SingletonHelper<DBConnectionPoolManger>::getInstance();

	if (dbManger)
	{
		QSqlDatabase db = dbManger->getConnection(args.strDBName);
		QSqlQuery query(db);
		query.prepare(args.strSQL);
		bindValues(&query, args.mapParams);

		if (query.exec()) 
		{
			handleResult(&query);
		}

		debug(query, args.mapParams);
		dbManger->releaseConnection(db, args.strDBName);
	}

}
