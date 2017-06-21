#include "stdafx.h"
#include "SysConfig.h"
#include "DatabaseUtil.h"
#include "DBConnectionPoolManger.h"

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
	return selectMaps(args).value(0);
}

QList<QVariantMap> DataBaseUtil::selectMaps(const DBUtilArguments & args)
{
	QList<QVariantMap> maps;

	executeSql(args, [&maps](QSqlQuery *query) 
	{
		maps = queryToMaps(query);
	});

	return maps;
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

QList<QVariantMap > DataBaseUtil::queryToMaps(QSqlQuery *query) 
{
	QList<QVariantMap > rowMaps;
	QStringList fieldNames = getFieldNames(*query);

	while (query->next()) 
	{
		QVariantMap rowMap;

		foreach(const QString &fieldName, fieldNames) 
		{
			rowMap.insert(fieldName, query->value(fieldName));
		}

		rowMaps.append(rowMap);
	}

	return rowMaps;
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

		if (query.exec()) {
			handleResult(&query);
		}

		debug(query, args.mapParams);
		dbManger->releaseConnection(db, args.strDBName);
	}

}
