#include "stdafx.h"
#include "RecordTree.h"

QRecordTree::QRecordTree(QObject *parent)
	: QRecordTable(parent)
{
	ID = -1; PID = -1;
}

QRecordTree::~QRecordTree()
{

}

/***********************************************************************************************************************************/
void QRecordTree::setIndexFiled(short id, short pid)
{
	if (this->ID != id)
	{
		ID = id;
		reCreateIndexOnID();
	}

	if (this->PID != pid)
	{
		PID = pid;
		reCreateIndexOnPID();
	}
}

/***********************************************************************************************************************************/
void QRecordTree::reCreateIndexOnID()
{
	createIndexOnCol(ID);
}

void QRecordTree::reCreateIndexOnPID()
{
	createIndexOnCol(PID);
}

void QRecordTree::createIndexOnID(const QVariant & id, int index)
{
	if (mapIndexes.contains(ID))
	{
		if (index == 0) 
			mapIndexes[ID].clear();
		mapIndexes[ID][index] = id;
	}
	else
	{
		QVariantList item;
		item.append(id);
		mapIndexes.insert(ID, item);
	}
}

void QRecordTree::createIndexOnPID(const QVariant & pid, int index)
{
	if (mapIndexes.contains(PID))
	{
		if (index == 0)
			mapIndexes[ID].clear();
		mapIndexes[ID][index] = pid;
	}
	else
	{
		QVariantList item;
		item.append(pid);
		mapIndexes.insert(PID, item);
	}
}

/***********************************************************************************************************************************/
int QRecordTree::childCount(const QVariant & pid) const
{
	return getValueCountOnCol(PID, pid);
}

int QRecordTree::getChildIndex(const QVariant & pid, int row) const
{
    const QList< int > & Index = getIndexRowOnCol(PID, pid);
	if (row >= Index.count()) return -1;
	return Index.at(row);
}

int QRecordTree::getChildRow(const QVariant & cid, const QVariant & pid) const
{
    const QList< int > & Index = getIndexRowOnCol(PID, pid);
	for (int row = 0; row < Index.count(); ++row)
	{
		if (data(Index.at(row), ID) == cid)
		{
			return row;
		}
	}
	return -1;
}


/***********************************************************************************************************************************/