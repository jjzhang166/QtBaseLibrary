#pragma once

#include "RecordTable.h"
#include <QAbstractItemModel>

class QAbstractItemView;
//==================================================================================================
//自定义表模型

class QRecordTableModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	QRecordTableModel( QObject * parent = NULL );
	~QRecordTableModel( );

public:
	void setRecord(const QRecord & record);
	void setTransRecord(const QRecord & record);

public:
	void addRow(const QList<QVariant> & value);
	void setRow(int row, const QList<QVariant> & value);
	void removeRow(int row);
	void removeRow(QList<int> & listRow);
	void addCol(const QString & name, QVariant::Type type, COLUMN_FLAGS flags = CF_NONE, const QString & alias = "");
	void setCol( QString name, COLUMN_FLAGS flags, const QString & alias = "" );
	void setCol( int col, COLUMN_FLAGS flags, const QString & alias = "" );
	void setAt(int row, int col, QVariant value);
	
	void refresh( void );
	QRecordTable & records( )const;
	QModelIndex getModelByIndex( int rowIndex ) const;
	QModelIndex getFindIndex( int col, const QString & text );
	QVariant getItemData(int row,int col);

public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex( ) ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex parent( const QModelIndex & child ) const;
    virtual int columnCount( const QModelIndex & parent = QModelIndex( ) ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex( ) ) const;
	virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
	virtual QVariant display( int col , const QVariant & value ) const;

protected:
	/*翻译数据为固定格式， 包含三列， 分别为：字段名、字段值、字段翻译， 需按顺序设置*/
	QRecord dsTransRecord;
	mutable QRecordTable dsRecords;

};
