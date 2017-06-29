#pragma once

#include "Record.h"
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
	void setTransRecord(const QRecord & record);

public:
	QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;



private:
	QVariant display(int col, const QVariant & value) const;

private:
	/*翻译数据为固定格式， 包含三列， 分别为：字段名、字段值、字段翻译， 需按顺序设置*/
	QRecord dsTransRecord;


	QList<int> listShowCols;
	QMap<int, QString> mapShowCols;


};
