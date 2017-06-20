#include "stdafx.h"
#include "JsonConfigHandle.h"

#include <QFile>
#include <QDebug>

#include <QTextStream>
#include <QJsonParseError>
#include <QRegularExpression>

//==================================================================================================
class JsonConfigHandlePrivate
{
public:
	JsonConfigHandlePrivate(const QString & strPath);

	QJsonValue getValue(const QString & attrName, const QJsonObject & fromNode) const;
	void setValue(const QString & attrName, const QJsonValue & attrValue);
	void setValue(QJsonObject & parent, const QString & attrName, const QJsonValue & attrValue);

	void saveFile(QJsonDocument::JsonFormat format) const;

	QJsonObject root;
	QString strFilePath;
};


//==================================================================================================
JsonConfigHandlePrivate::JsonConfigHandlePrivate(const QString & strPath) :strFilePath(strPath)
{
	if (strFilePath.isEmpty())
		return;

	QByteArray json("{}");
	QFile fileJson(strFilePath);
	if (fileJson.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		json = fileJson.readAll();
	}
	else
	{
		qDebug() << QString("Cannot open the file: %1").arg(strFilePath);
	}

	//解析JSON
	QJsonParseError error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &error);
	root = jsonDoc.object();

	if (QJsonParseError::NoError != error.error)
	{
		qDebug() << error.errorString() << ", Offset: " + error.offset;
	}

}


QJsonValue JsonConfigHandlePrivate::getValue(const QString & attrName, const QJsonObject & fromNode) const
{
	QJsonObject parent(fromNode.isEmpty() ? root : fromNode);

	QStringList keys = attrName.split(QRegularExpression("\\."));
	int size = keys.size();
	for (int i = 0; i < size - 1; ++i)
	{
		if (parent.isEmpty())
			return QJsonValue();

		parent = parent.value(keys.at(i)).toObject();
	}

	return parent.value(keys.last());
}

//遍历方式设置值
void JsonConfigHandlePrivate::setValue(const QString & attrName, const QJsonValue & attrValue)
{
	QStringList keys = attrName.split(QRegularExpression("\\."));

	QJsonObject attrObject = root;
	int size = keys.size();
	for (int i = 0; i < size - 1; ++i)
	{
		attrObject = attrObject[keys.at(i)].toObject();
	}

	attrObject[keys.last()] = attrValue;
}

//递归方法设置值
void JsonConfigHandlePrivate::setValue(QJsonObject & parent, const QString & attrName, const QJsonValue & attrValue)
{
	const int indexOfDot = attrName.indexOf('.');
	const QString property = attrName.left(indexOfDot); 
	const QString subPath = (indexOfDot > 0) ? attrName.mid(indexOfDot + 1) : QString();

	QJsonValue subValue = parent[property];

	if (subPath.isEmpty()) 
	{
		subValue = attrValue;
	}
	else 
	{
		QJsonObject obj = subValue.toObject();
		setValue(obj, subPath, attrValue);
		subValue = obj;
	}

	parent[property] = subValue;
}


void JsonConfigHandlePrivate::saveFile(QJsonDocument::JsonFormat format) const
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << QJsonDocument(root).toJson(format);
}



//==================================================================================================
JsonConfigHandle::JsonConfigHandle(const QString & jstrFilePath)
{
}


JsonConfigHandle::~JsonConfigHandle()
{
}

//==================================================================================================
int JsonConfigHandle::getInt(const QString & attrName, int default, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toInt(default);
}

bool JsonConfigHandle::getBool(const QString & attrName, bool default, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toBool(default);
}

double JsonConfigHandle::getDouble(const QString & attrName, double default, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toDouble(default);
}

QString JsonConfigHandle::getString(const QString & attrName, const QString & default, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toString(default);
}

QStringList JsonConfigHandle::getString(const QString & attrName, const QJsonObject & fromNode) const
{
	QStringList result;
	const QJsonArray & array = getJsonArray(attrName, fromNode);
	for (QJsonArray::const_iterator it = array.begin(); it != array.end(); ++it)
	{
		const QJsonValue & value = *it;
		result << value.toString();
	}
	return result;
}

QJsonValue JsonConfigHandle::getJsonValue(const QString & attrName, const QJsonObject & fromNode) const
{
	Q_D(const JsonConfigHandle);
	return d->getValue(attrName, fromNode);
}

QJsonArray JsonConfigHandle::getJsonArray(const QString & attrName, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toArray();
}

QJsonObject JsonConfigHandle::getJsonObject(const QString & attrName, const QJsonObject & fromNode) const
{
	return getJsonValue(attrName, fromNode).toObject();
}



//==================================================================================================
void JsonConfigHandle::setValue(const QString & attrName, const QJsonValue & attrValue)
{
	Q_D(JsonConfigHandle);
	d->setValue(attrName, attrValue);
}

void JsonConfigHandle::setValue(const QString & attrName, const QStringList & attrValue)
{
	Q_D(JsonConfigHandle);

	QJsonArray array;
	foreach(const QString &str, attrValue)
	{
		array.append(str);
	}

	d->setValue(attrName, array);
}


//==================================================================================================
void JsonConfigHandle::saveFile(QJsonDocument::JsonFormat format)
{
	Q_D(JsonConfigHandle);
	d->saveFile(format);
}