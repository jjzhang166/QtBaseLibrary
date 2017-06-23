#pragma once

#include "RecordTree.h"
#include <QAbstractItemModel>

//==================================================================================================
enum TREE_MODEL_ROLE
{
	TREE_ROLE_ID = Qt::UserRole,
	TREE_ROLE_PID 
};

//自定义树模型
//==================================================================================================
class QRecordTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	QRecordTreeModel( QObject * parent = NULL );
	~QRecordTreeModel( );

public:
	bool select(const QString & SQL);
	bool select(const QString & SQL, QString & Error);

	void refresh(void);

	//设置根节点的 ID，默认为 0, 列从0开始。 先设置根节点和列。
	void setRoot( const QVariant & identify = 0 );
	void setFieldCol(const QString & id, const QString & pid);
	void setFieldCol( short idCol, short pidCol , short typeCol = -1 );
	void setCol(int col, COLUMN_FLAGS flags, const QString & alias = "");
	void setCol(const QString & name, COLUMN_FLAGS flags, const QString & alias = "");
	
public:
	QRecordTree & records( ) const;
	QModelIndex getModelByIndex( int rowIndex ) const;
	QModelIndex getModelByID( const QVariant & cid ) const;

public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex( ) ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
    virtual QModelIndex parent( const QModelIndex & child ) const;
    virtual int columnCount( const QModelIndex & parent = QModelIndex( ) ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex( ) ) const;
	virtual Qt::ItemFlags flags( const QModelIndex &index ) const;

protected:
	int getChildCount( const QModelIndex & parent ) const;

protected:
	QVariant Root;
	short ID, PID, TYPE;			//TYPE标识是否增加同级节点的列
	mutable QRecordTree dsRecords;
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
