#include "stdafx.h"
#include "RecordTreeModel.h"

//======================================================================================================================
QRecordTreeModel::QRecordTreeModel( QObject * parent ) : QAbstractItemModel( parent )
{
	Root = 0;
	ID = -1;
	PID = -1;
	TYPE = -1;
}

QRecordTreeModel::~QRecordTreeModel( )
{

}

void QRecordTreeModel::setRecord(const QRecord & record)
{
	dsRecords = record;
}

void QRecordTreeModel::setRoot(const QVariant & identify)
{
	this->Root = identify;
	refresh();
}

void QRecordTreeModel::refresh( )
{
	beginResetModel( );
	endResetModel( );
}

void QRecordTreeModel::setFieldCol( short idCol, short pidCol, short typeCol )
{
	ID = idCol;
	PID = pidCol;
	TYPE = typeCol;
	dsRecords.setIndexFiled( idCol, pidCol );
	refresh( );
}

void QRecordTreeModel::setFieldCol(const QString & id, const QString & pid)
{
	ID = dsRecords.getCol(id);
	PID = dsRecords.getCol(pid);
	dsRecords.setIndexFiled(ID, PID);
	refresh();
}

void QRecordTreeModel::setCol( const QString & name, COLUMN_FLAGS flags, const QString & alias )
{
	dsRecords.setCol( name, flags, alias );
}

void QRecordTreeModel::setCol( int col, COLUMN_FLAGS flags, const QString & alias )
{
	dsRecords.setCol( col, flags, alias );
}

QRecordTree & QRecordTreeModel::records( ) const
{
	return dsRecords;
}

QModelIndex QRecordTreeModel::getModelByIndex( int rowIndex ) const
{
	QVariant pID = dsRecords.data( rowIndex , PID );
	QVariant cID = dsRecords.data( rowIndex , ID );
	int row = dsRecords.getChildRow( cID , pID );
	if( row >= 0 ) return createIndex( row , 0 , rowIndex );

	rowIndex = dsRecords.getChildIndex( Root , 0 );
	if( row >= 0 ) return createIndex( 0 , 0 , rowIndex );

	return QModelIndex( );
}

QModelIndex QRecordTreeModel::getModelByID( const QVariant & cid ) const
{
	int row = dsRecords.getFetchRow( ID , cid ); 
	if( row < 0 ) return QModelIndex( );

	QVariant pid = dsRecords.data( row , PID );
	int cRow = dsRecords.getChildRow( cid , pid );
	if( cRow < 0 ) return QModelIndex( );
	return createIndex( cRow , 0 , row );
}

//======================================================================================================================
QVariant QRecordTreeModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		if (dsRecords.colCount() > section)
		{
			short col = dsRecords.showRecords().at(section);
			return dsRecords.getColAlias(col);
		}
	}
	return QVariant( );
}

QModelIndex QRecordTreeModel::index( int row , int column , const QModelIndex & parent ) const
{
    if( !hasIndex( row, column, parent ) )
        return QModelIndex( );

	int internalId = -1;
	if( !parent.isValid( ) )
	{
		internalId = dsRecords.getChildIndex( Root , row );
	}
	else
	{	
		int ParentRow = parent.internalId( );
		QVariant Value = dsRecords.data( ParentRow , ID );
		internalId = dsRecords.getChildIndex( Value , row );
	}

	if( internalId == -1 ) return QModelIndex( );
	return createIndex( row, column , internalId );
}

QVariant QRecordTreeModel::data( const QModelIndex & index , int role ) const
{
	if( !index.isValid( ) ) return QVariant( );
	
	int RowIndex = index.internalId( );
	short Col = dsRecords.showRecords( ).at( index.column( ) );
	if( role == Qt::DisplayRole ) return dsRecords.data( RowIndex , Col );
	if( role == TREE_ROLE_ID ) return dsRecords.data( RowIndex , ID );
	if( role == TREE_ROLE_PID ) return dsRecords.data( RowIndex , PID );
	
	return QVariant( );
}

QModelIndex QRecordTreeModel::parent( const QModelIndex & child ) const
{
    if( !child.isValid( ) )
        return QModelIndex( );
	if( dsRecords.rowCount( ) <= 0 ) return QModelIndex( );
	
	int cRow = child.internalId( ); 
	QVariant pID = dsRecords.data( cRow , PID );
	int pRow = dsRecords.getFetchRow( ID , pID );
	QVariant ppID = dsRecords.data( pRow , PID );
	int row = dsRecords.getChildRow( pID , ppID );
	if( row == -1 ) return QModelIndex( );
	return createIndex( row, 0, pRow );
}

int QRecordTreeModel::columnCount( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return dsRecords.showColCount( );
}

int QRecordTreeModel::rowCount( const QModelIndex & parent ) const
{
	if( parent.column( ) > 0 )
        return 0;

	QVariant ParentId;
	if( !parent.isValid( ) )
	{
		ParentId = Root;
	}
	else
	{
		if(ID < 0) return 0;
		int Index = parent.internalId( );
		ParentId = dsRecords.data( Index , ID );
	}
	int Count = dsRecords.childCount( ParentId );
	return Count;
}

Qt::ItemFlags QRecordTreeModel::flags( const QModelIndex &index ) const
{
    if(!index.isValid())  
		return 0;

	if( index.column() >= dsRecords.showRecords().count()) 
		return QAbstractItemModel::flags(index);

	if( dsRecords.showFlags(index.column()) & CF_EDIT) 
		return Qt::ItemIsEditable | QAbstractItemModel::flags(index);

	return QAbstractItemModel::flags(index);
}