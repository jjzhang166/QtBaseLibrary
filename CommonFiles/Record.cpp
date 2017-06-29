#include "stdafx.h"
#include "Record.h"

/***********************************************************************************************************************************/
QRecord::QRecord(QObject *parent)
	: QObject(parent)
{

}

QRecord::QRecord(const QRecord & other, QObject *parent)
	: QObject(parent)
{
	clear();
	mapIndexes = other.mapIndexes;
	listColumns = other.listColumns;
	listRows = other.listRows;
}

QRecord::~QRecord()
{
	clear();
}

QRecord & QRecord::operator=(const QRecord & other)
{
	wipeData();
	mapIndexes = other.mapIndexes;
	listRows = other.listRows;

	if (!compareColumns(other.listColumns))
	{
		listColumns = other.listColumns;
	}

	return *this;
}

/***********************************************************************************************************************************/
int QRecord::rowCount() const
{
	return listRows.count();
}

int QRecord::colCount() const
{
	return listColumns.count();
}

short QRecord::getCol(const QString & name) const
{
	for (int col = 0; col < listColumns.count(); ++col)
	{
		const QString & colName = listColumns.at(col).Name;
		if (colName.compare(name, Qt::CaseInsensitive) == 0)
			return col;
	}

	return -1;
}

QList<ColumnInfo> & QRecord::getColumns() const
{
	return listColumns;
}

QList<QVariantList> & QRecord::getRows() const
{
	return listRows;
}

QString QRecord::getColName(int col) const
{
	return listColumns.at(col).Name;
}

QString QRecord::getColAlias(int col) const
{
	return listColumns.at(col).Alias;
}

QVariant QRecord::data(int row, int col) const
{
	CHK_EXP_RET(row < 0 || row >= rowCount(), QVariant());
	CHK_EXP_RET(col < 0 || col >= colCount(), QVariant());
	return listRows[row][col];
}

QVariant QRecord::data(int row, const QString & name) const
{
	short col = getCol(name);
	return data(row, col);
}

int QRecord::getCollectInt(int row, int col) const
{
	return data(row, col).toInt();
}

int QRecord::getCollectInt(int row, const QString & name) const
{
	return data(row, name).toInt();
}

QString QRecord::getCollectStr(int row, int col) const
{
	return data(row, col).toString();
}

QString QRecord::getCollectStr(int row, const QString & name) const
{
	return data(row, name).toString();
}

/***********************************************************************************************************************************/
void QRecord::clear()
{
	wipeData();
	listColumns.clear();
}

void QRecord::wipeData()
{
	mapIndexes.clear();
	listRows.clear();
}

void QRecord::reserve(int maxRows, int maxCols)
{
	listRows.reserve(maxRows);
	listColumns.reserve(maxCols);
}


/***********************************************************************************************************************************/
void QRecord::addCol(const ColumnInfo & col)
{
	wipeData();
	listColumns.append(col);
}

void QRecord::addRow(const QList<QVariant> & row)
{
	listRows.append(row);
}

void QRecord::removeRow(int row)
{
	listRows.removeAt(row);
}

void QRecord::setRow(int row, const QList<QVariant> & value)
{
	listRows[row] = value;
}

/***********************************************************************************************************************************/
void QRecord::createIndexOnCol(short col)
{
	CHK_EXP_END(col < 0 || col >= colCount());

	QVariantList index;
	for (int i = 0; i < rowCount(); ++i)
	{
		index.append(data(i, col));
	}

	mapIndexes[col] = index;

}

void QRecord::createIndexOnCol(const QString & name)
{
	int col = getCol(name);
	createIndexOnCol(col);
}

QVariantList QRecord::getIndexOnCol(short col) const
{
	if (mapIndexes.contains(col))
		return mapIndexes.value(col);

	return QVariantList();
}

QVariantList QRecord::getIndexOnCol(const QString & name) const
{
	short col = getCol(name);
	return getIndexOnCol(col);
}

int QRecord::getValueCountOnCol(short col, const QVariant & value) const
{
	const QVariantList & index = getIndexOnCol(col);
	return index.count(value);
}

int QRecord::getValueCountOnCol(const QString & name, const QVariant & value) const
{
	int col = getCol(name);
	return getValueCountOnCol(col, value);
}

QList< int > QRecord::getIndexRowOnCol(short col, const QVariant & value) const
{
	const QVariantList & index = getIndexOnCol(col);

	QList<int> listIndex;

	int pos = -1;
	do
	{
		pos = index.indexOf(value, pos + 1);
		if (pos >= 0)
			listIndex.append(pos);
	}while (pos != -1);

	return  listIndex;
}

QList< int > QRecord::getIndexRowOnCol(const QString & name, const QVariant & value) const
{
	short col = getCol(name);
	return getIndexRowOnCol(col, value);
}

int QRecord::getFetchRow(short col, const QVariant & value, int start) const
{
	const QVariantList & Index = getIndexOnCol(col);
	if (Index.count() > 0)
	{
		return Index.indexOf(value, start);
	}
	else
	{
		for (int row = start; row < rowCount(); ++row)
		{
			if (data(row, col) == value)
				return row;
		}
	}

	return -1;
}

int QRecord::getFetchRow(const QString & name, const QVariant & value, int start) const
{
	short col = getCol(name);
	return getFetchRow(col, value, start);
}

int QRecord::getFetchRow(const QMap<int, QVariant> & colValueMap, int start, Qt::CaseSensitivity caseValue) const
{
	const QList<int> & Cols = colValueMap.keys();
	int keyRows = Cols.count();

	for (int row = start; row < rowCount(); ++row)
	{
		for(int keyRow = 0; keyRow < keyRows; ++keyRow)
		{
			int col = Cols.at(keyRow);
			const QVariant & value = colValueMap.value(col);

			if (value.type() == QVariant::String)
			{
				const QString & strValue = value.toString();
				const QString & strData = data(row, col).toString();
				if(strValue.compare(strData, caseValue) != 0)
					break;
			}
			
			else
			{
				if (value != data(row, col))
				{
					break;
				}
			}
			
			if (keyRow == keyRows - 1)
				return row;
		}
	}

	return -1;
}

int QRecord::getFetchString(short col, const QString & value, int start, Qt::CaseSensitivity caseValue) const
{
	for (int row = start; row < rowCount(); ++row)
	{
		const QString & strValue = data(row, col).toString();
		if (strValue.compare(value, caseValue) == 0)
			return row;
	}

	return -1;
}

int QRecord::getFetchString(const QString & name, const QString & value, int start, Qt::CaseSensitivity caseValue) const
{
	short col = getCol(name);
	return getFetchString(col, value, start, caseValue);
}

int QRecord::getNotFetchRow(short col, const QVariant & value, int start) const
{
    const QList< int > & Indexs = getIndexRowOnCol(col, value);
	if (Indexs.count() > 0)
	{
		int rowIndex = start;
		for (int i = 0; i < Indexs.count(); ++i)
		{
			if (Indexs.at(i) > rowIndex)
			{
				return rowIndex;
			}
			++rowIndex;
		}
	}
	else
	{
		for (int row = start; row < rowCount(); ++row)
		{
			if (data(row, col) != value)
				return row;
		}
	}

	return -1;
}

int QRecord::getNotFetchRow(const QString & name, const QVariant & value, int start) const
{
	short col = getCol(name);
	return getNotFetchRow(col, value, start);
}


bool QRecord::compareColumns(const QList<ColumnInfo> & newColumns)
{
	if (this->listColumns.count() != newColumns.count())
		return false;

	for (int col = 0; col < listColumns.count(); ++col)
	{
		if (listColumns.at(col).Name.compare(newColumns.at(col).Name, Qt::CaseInsensitive) != 0)
			return false;
	}
	return true;	
}
