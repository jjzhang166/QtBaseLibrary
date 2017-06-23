#ifndef RECORDTREE_H
#define RECORDTREE_H

#include "RecordTable.h"

class QRecordTree : public QRecordTable
{
	Q_OBJECT

public:
	QRecordTree(QObject *parent = 0);
	~QRecordTree();

public:
	void setIndexFiled(short id, short pid);

	void reCreateIndexOnID();
	void reCreateIndexOnPID();
	void createIndexOnID(const QVariant & id, int index);
	void createIndexOnPID(const QVariant & pid, int index);

	int childCount(const QVariant & pid) const;
	int getChildIndex(const QVariant & pid, int row) const;
	int getChildRow(const QVariant & cid, const QVariant & pid) const;

protected:
	short ID, PID;
};

#endif // RECORDTREE_H
