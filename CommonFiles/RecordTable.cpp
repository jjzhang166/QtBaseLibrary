#include "stdafx.h"
#include "RecordTable.h"

QRecordTable::QRecordTable(QObject *parent)
	: QRecord(parent)
{

}

QRecordTable::~QRecordTable()
{
	clear();
}

QRecordTable & QRecordTable::operator= (const QRecord & other)
{
	wipeData();
	mapIndexes = other.mapIndexes;
	listRows = other.listRows;

	if (!compareColumns(other.listColumns))
	{
		listColumns = other.listColumns;
		listShowCols.clear();
	}

	return *this;
}

QRecordTable & QRecordTable::operator= (const QRecordTable & other)
{
	wipeData();
	mapIndexes = other.mapIndexes;
	listRows = other.listRows;

	if (!compareColumns(other.listColumns))
	{
		listColumns = other.listColumns;
		listShowCols = other.listShowCols;
	}

	return *this;
}

/***********************************************************************************************************************************/
int QRecordTable::showColCount() const
{
	return listShowCols.count();
}

int QRecordTable::getActualCol(int showCol)
{
	CHK_EXP_RET(showCol < 0 || showCol >= colCount(), -1);
	return listShowCols.at(showCol);
}

QVariant QRecordTable::showData(int row, int col) const
{
	CHK_EXP_RET(col < 0 || col >= showColCount(), QVariant());

	int showCol = listShowCols.at(col);
	return data(row, showCol);
}

COLUMN_FLAGS QRecordTable::showFlags(int col) const
{
	CHK_EXP_RET(col < 0 || col >= showColCount(), CF_NONE);

	short showCol = listShowCols.at(col);
    CHK_EXP_RET(showCol < 0 || showCol >= listColumns.count(), CF_NONE);

    return listColumns.at(showCol).Flag;
}

QList<short> & QRecordTable::showRecords() const
{
	return listShowCols;
}

/***********************************************************************************************************************************/
void QRecordTable::setCol(int col, COLUMN_FLAGS flag, const QString & alias)
{
	CHK_EXP_END(col < 0 || col >= colCount());

	listColumns[col].Flag = flag;
	listColumns[col].Alias = alias;

	if (flag & CF_SHOW)
	{
		int index = listShowCols.indexOf(col);
		CHK_EXP_RUN(index < 0, listShowCols.append(col));
	}

}

void QRecordTable::setCol(const QString & name, COLUMN_FLAGS flag, const QString & alias)
{
	int col = getCol(name);
	setCol(col, flag, alias);
}

void QRecordTable::addCol(const QString & name, QVariant::Type type, COLUMN_FLAGS flag, const QString & alias)
{
	wipeData();
	int count = listColumns.count();
	CHK_EXP_RUN(flag & CF_SHOW, listShowCols.append(count));

	ColumnInfo Info;
    Info.Name = name;
	Info.Flag = flag;
	Info.Alias = alias;
	Info.Type = type;
	listColumns.append(Info);
}

bool QRecordTable::setAt(int row, int col, const QVariant & value)
{
	CHK_EXP_RET(row < 0 || row >= rowCount(), false);
	CHK_EXP_RET(col < 0 || col >= colCount(), false);
	listRows[row][col] = value;
	return true;
}


/***********************************************************************************************************************************/
void QRecordTable::clear()
{
	wipeData();
	listShowCols.clear();
	listColumns.clear();
}