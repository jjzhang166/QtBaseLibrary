#pragma once

#include <QSqlTableModel>


/************************************************************************
* 直接连接数据库的Model
* 支持百万级数据的显示
************************************************************************/


class QDBTableModel : public QSqlTableModel
{
	Q_OBJECT

public:
	QDBTableModel(QObject *parent = 0);
	~QDBTableModel();


public:
	void setCol(int col, const QString & alias, bool show = true);
	void setCol(const QString & colName, const QString & alias, bool show = true);

	void setQuery(const QSqlQuery &query);

public:
	QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;



private:
	QList<int> listShowCols;
	QMap<int, QString> mapShowCols;
};
