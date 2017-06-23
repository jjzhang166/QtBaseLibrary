#include "stdafx.h"
#include "RecordTableModel.h"

//======================================================================================================================

QRecordTableModel::QRecordTableModel( QObject * parent ) : QAbstractItemModel( parent )
{

}

QRecordTableModel::~QRecordTableModel( )
{

}

void QRecordTableModel::refresh( )
{
	beginResetModel( );
	endResetModel( );
}

void QRecordTableModel::setRecord(const QRecord & record)
{
	dsRecords = record;
}

void QRecordTableModel::setTransRecord(const QRecord & record)
{
	dsTransRecord = record;
}

void QRecordTableModel::addCol(const QString & name, QVariant::Type type, COLUMN_FLAGS flags, const QString & alias)
{
	dsRecords.addCol(name, type, flags, alias);
}

void QRecordTableModel::addRow(const QList<QVariant> & value)
{
	dsRecords.addRow(value);
}

void QRecordTableModel::setRow(int row, const QList<QVariant> & value)
{
	dsRecords.setRow(row,value);
}

void QRecordTableModel::removeRow(int row)
{
	dsRecords.removeRow(row);
}

void QRecordTableModel::removeRow(QList<int> & listRow)
{
	int count = listRow.count();
	for (int i = count-1; i >= 0; i--)
	{
		removeRow(listRow[i]);
	}
}

void QRecordTableModel::setCol( QString name, COLUMN_FLAGS flags, const QString & alias )
{
	dsRecords.setCol( name, flags, alias );
}

void QRecordTableModel::setCol( int col, COLUMN_FLAGS flags, const QString & alias )
{
	dsRecords.setCol( col, flags, alias );
}

void QRecordTableModel::setAt(int row, int col, QVariant value)
{
	dsRecords.setAt(row, col, value);
}

QRecordTable & QRecordTableModel::records( )const
{
	return dsRecords;
}

QModelIndex QRecordTableModel::getModelByIndex( int rowIndex ) const
{
	return createIndex( rowIndex , 0 , rowIndex ); 
}

QModelIndex QRecordTableModel::getFindIndex( int col, const QString & text )
{
	//模糊匹配
    Q_UNUSED(col)
    Q_UNUSED(text)
	return QModelIndex( );
}

QVariant QRecordTableModel::getItemData(int row, int col)
{
	return dsRecords.data(row, col);
}

//======================================================================================================================
QVariant QRecordTableModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		if( dsRecords.colCount( ) > section )
		{
			short col = dsRecords.showRecords( ).at( section );
			return dsRecords.getColAlias(col);
		}
	}
	return QVariant( );
}

QModelIndex QRecordTableModel::index( int row , int column , const QModelIndex & parent ) const
{
    if( !hasIndex( row, column, parent ) )
        return QModelIndex( );

	return createIndex( row, column, row );
}

QVariant QRecordTableModel::data( const QModelIndex & index , int role ) const
{
	if (role == Qt::TextAlignmentRole) 
	{
		if (dsRecords.showFlags(index.column())&CF_RIGHT)
		{
			return Qt::AlignRight;
		}
		else if (dsRecords.showFlags(index.column())&CF_CENTER)
		{
			return Qt::AlignCenter;
		}
		return Qt::AlignLeft;
	}

	if( role != Qt::DisplayRole || !index.isValid( ) ) return QVariant( );
	
	return display( index.column( ) , dsRecords.showData( index.row( ), index.column( ) ) ) ;

}

bool QRecordTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return QAbstractItemModel::setData(index, value, role);
}

QModelIndex QRecordTableModel::parent( const QModelIndex & child ) const
{
    Q_UNUSED(child)
	return QModelIndex( );
}

int QRecordTableModel::columnCount( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return dsRecords.showColCount( );
}

int QRecordTableModel::rowCount( const QModelIndex & parent ) const
{
    if( parent.column( ) > 0 )
        return 0;

	if( parent == QModelIndex( ) )
		return dsRecords.rowCount( );

	return 0;
}

Qt::ItemFlags QRecordTableModel::flags( const QModelIndex &index ) const
{
    if(!index.isValid( )) 
		return 0;

	if(index.column( ) >= dsRecords.showRecords( ).count( )) 
		return QAbstractItemModel::flags( index );

	if(dsRecords.showFlags(index.column()) & CF_EDIT)
		return Qt::ItemIsEditable | QAbstractItemModel::flags( index );

	return QAbstractItemModel::flags( index );
}

QVariant QRecordTableModel::display( int col , const QVariant & value ) const
{
	Q_UNUSED(col);
	
	//翻译使用。
	if (dsTransRecord.rowCount() > 0 && dsTransRecord.colCount() == 3)
	{
		const QString & colName = dsRecords.getColName(col);
		QMap<int, QVariant> mapFind;
		mapFind.insert(0, colName);
		mapFind.insert(1, value);
		int row = dsTransRecord.getFetchRow(mapFind);

		if (row >= 0)
		{
			return dsTransRecord.data(row, 2);
		}
	}
	return value;
}