#pragma once
#include "SingletonHelper.h"

#include <qglobal.h>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/************************************************************************
*	在Qt Json类基础上的封装， 方便读写多层级的Json                                                                   
*************************************************************************/

//==================================================================================================
class JsonConfigHandlePrivate;
class JsonConfigHandle
{
public:
	JsonConfigHandle(const QString & jstrFilePath);
	~JsonConfigHandle();

public:
	int getInt(const QString & attrName, int default = 0, const QJsonObject & fromNode = QJsonObject()) const;
	bool getBool(const QString & attrName, bool default = false, const QJsonObject & fromNode = QJsonObject()) const;
	double getDouble(const QString & attrName, double default = 0.0, const QJsonObject & fromNode = QJsonObject()) const;
	QString getString(const QString & attrName, const QString & default = QString(), const QJsonObject & fromNode = QJsonObject()) const;
	
	QStringList getString(const QString & attrName, const QJsonObject & fromNode = QJsonObject()) const;
	QJsonValue getJsonValue(const QString & attrName, const QJsonObject & fromNode = QJsonObject()) const;
	QJsonArray getJsonArray(const QString & attrName, const QJsonObject & fromNode = QJsonObject()) const;
	QJsonObject getJsonObject(const QString & attrName, const QJsonObject & fromNode = QJsonObject()) const;


	//==================================================================================================
	void setValue(const QString & attrName, const QJsonValue & attrValue);
	void setValue(const QString & attrName, const QStringList & attrValue);


	//==================================================================================================
	void saveFile(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

private:
	Q_DECLARE_PRIVATE(JsonConfigHandle);
	JsonConfigHandlePrivate * d_ptr;
};

//==================================================================================================
