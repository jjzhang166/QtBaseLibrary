#include "stdafx.h"


#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "DBTableModel.h"

QDBTableModel::QDBTableModel(QObject *parent) : QSqlTableModel()
{
	

}

QDBTableModel::~QDBTableModel()
{

}

void QDBTableModel::setCol(int col, const QString & alias, bool show)
{
	if (show)
	{
		mapShowCols[col] = alias;
	}
	else
	{
		mapShowCols.remove(col);
	}

	listShowCols = mapShowCols.keys();
	qSort(listShowCols);
}

void QDBTableModel::setCol(const QString & colName, const QString & alias, bool show)
{
	int col = query().record().indexOf(colName);
	setCol(col, alias, show);
}


void QDBTableModel::setQuery(const QSqlQuery &query)
{
	QSqlTableModel::setQuery(query);
}


QVariant QDBTableModel::data(const QModelIndex &idx, int role ) const
{
	if (listShowCols.count() <= idx.column())
		return QVariant();

	//对应的实际的列值
	int col = listShowCols.at(idx.column());
	QModelIndex index = createIndex(idx.row(), col);
	return QSqlTableModel::data(index, role);

}

QVariant QDBTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
	{
		return QSqlTableModel::headerData(section, orientation, role);
	}

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (listShowCols.count() > section)
		{
			int col = listShowCols.at(section);
			return mapShowCols.value(col);
		}
	}
	return QVariant();
}

int QDBTableModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return listShowCols.count();
}