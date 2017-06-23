#ifndef RECORDTABLE_H
#define RECORDTABLE_H

#include "Record.h"

class QRecordTable : public QRecord
{
	Q_OBJECT

public:
	QRecordTable(QObject *parent = 0);
	~QRecordTable();

public:
	QRecordTable & operator= (const QRecord & other);
	QRecordTable & operator= (const QRecordTable & other);

public:
	int showColCount() const;
	int getActualCol(int showCol);

	QVariant showData(int row, int col) const;
	COLUMN_FLAGS showFlags(int col) const;
	QList<short> & showRecords() const;
	

public:
	void setCol(int col, COLUMN_FLAGS flag, const QString & alias = "");
	void setCol(const QString & name, COLUMN_FLAGS flag, const QString & alias = "");
	void addCol(const QString & name, QVariant::Type type, COLUMN_FLAGS flag, const QString & alias);

	bool setAt(int row, int col, const QVariant & value);

public:
	virtual void clear();

protected:
	mutable QList<short> listShowCols;
};

#endif // RECORDTABLE_H
