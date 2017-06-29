#pragma once

#include "BaseStruct.h"

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QSqlQuery>

class QRecordTable;
class QRecordTree;
//======================================================================================================================
class QRecord : public QObject
{
	Q_OBJECT

public:
	QRecord(QObject *parent = 0);
	QRecord(const QRecord & other, QObject *parent = 0);
	~QRecord();

public:
	QRecord & operator=(const QRecord & other);

public:
	int rowCount() const;
	int colCount() const;
	short getCol(const QString & name) const;
	QString getColName(int col) const;
	QString getColAlias(int col) const;
	QList<ColumnInfo> & getColumns() const;
	QList<QVariantList> & getRows() const;

	QVariant data(int row, int col) const;
	QVariant data(int row, const QString & name) const;

	int getCollectInt(int row, int col) const;
	int getCollectInt(int row, const QString & name) const;
	QString getCollectStr(int row, int col) const;
	QString getCollectStr(int row, const QString & name) const;

public:
	virtual void clear();
	void wipeData();
	void reserve(int maxRows, int maxCols);

public:
	//addCol will clear table data
	void addCol(const ColumnInfo & col);
	void addRow(const QList<QVariant> & row);
	void removeRow(int row);
	void setRow(int row, const QList<QVariant> & value);

public:
	void createIndexOnCol(short col);
	void createIndexOnCol(const QString & name);

	QVariantList getIndexOnCol(short col) const;
	QVariantList getIndexOnCol(const QString & name) const;

	int getValueCountOnCol(short col, const QVariant & value) const;
	int getValueCountOnCol(const QString & name, const QVariant & value) const;

	QList< int > getIndexRowOnCol(short col, const QVariant & value) const;
	QList< int > getIndexRowOnCol(const QString & name, const QVariant & value) const;

	int getFetchRow(short col, const QVariant & value, int start = 0) const;
	int getFetchRow(const QString & name, const QVariant & value, int start = 0) const;

	//支持多个列的值进行匹配
	int getFetchRow(const QMap<int, QVariant> & colValueMap, int start = 0, Qt::CaseSensitivity caseValue = Qt::CaseInsensitive) const;
	
	int getFetchString(short col, const QString & value, int start = 0, Qt::CaseSensitivity caseValue = Qt::CaseInsensitive) const;
	int getFetchString(const QString & name, const QString & value, int start = 0, Qt::CaseSensitivity caseValue = Qt::CaseInsensitive) const;
	int getNotFetchRow(short col, const QVariant & value, int start = 0) const;
	int getNotFetchRow(const QString & name, const QVariant & value, int start = 0) const;

protected:
	bool compareColumns(const QList<ColumnInfo> & newColumns);

protected:
	friend class QRecordTree;
	friend class QRecordTable;

	//Indexes
	QMap<int, QVariantList> mapIndexes;
	mutable QList<ColumnInfo> listColumns;
	mutable QList<QVariantList> listRows;

};

