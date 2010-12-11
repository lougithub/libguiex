// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101119.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipModel.h"
#include <QPixmap>


namespace RE
{


// -------------------------------------------------------------------------
// General.
// -------------------------------------------------------------------------
ReClipModel::ReClipModel( QWidget* _parent /* = NULL */ )
: TSuper( ( QObject* )_parent )
{
	
}


// -------------------------------------------------------------------------
// Override QStandardItemModel.
// -------------------------------------------------------------------------
bool ReClipModel::setData( const QModelIndex& _index, const QVariant& _value, int _role /* = Qt::EditRole */ )
{
	ReClipData* clipData = dynamic_cast< ReClipData* >( TSuper::item( _index.row() ) );
	//QString name = data( index( _index.row(), EColumn_Name, QModelIndex() ) ).toString();
	QString name = data( index( _index.row(), EColumn_Icon, QModelIndex() ) ).toString();
	qreal x = data( index( _index.row(), EColumn_X, QModelIndex() ) ).toDouble();
	qreal y = data( index( _index.row(), EColumn_Y, QModelIndex() ) ).toDouble();
	qreal w = data( index( _index.row(), EColumn_W, QModelIndex() ) ).toDouble();
	qreal h = data( index( _index.row(), EColumn_H, QModelIndex() ) ).toDouble();

	clipData->GetNameRef() = name;
	clipData->GetOffsetRef().setX( x );
	clipData->GetOffsetRef().setY( y );
	clipData->GetSizeRef().setWidth( w );
	clipData->GetSizeRef().setHeight( h );

	return TSuper::setData( _index, _value, _role );
}


QVariant ReClipModel::data( const QModelIndex& _index, int _role /* = Qt::DisplayRole */ ) const
{
	if( !_index.isValid() )
		return QVariant();

	if( Qt::DecorationRole == _role )
	{
		if( EColumn_Icon == _index.column() )
		{
			ReClipData* clipData = dynamic_cast< ReClipData* >( TSuper::item( _index.row() ) );
			QSize size = m_pixmap.size();
			int x = ( int )( ( qreal )size.width() * clipData->GetOffset().x() );
			int y = ( int )( ( qreal )size.height() * clipData->GetOffset().y() );
			int w = ( int )( ( qreal )size.width() * clipData->GetSize().width() );
			int h = ( int )( ( qreal )size.height() * clipData->GetSize().height() );

			return QIcon( m_pixmap.copy( x, y, w, h ) );
		}
	}
	else if( Qt::TextAlignmentRole == _role )
	{
		if( EColumn_Icon == _index.column() )
			return int( Qt::AlignRight | Qt::AlignCenter );
		else
			return int( Qt::AlignCenter );
	}

	return TSuper::data( _index, _role );
}


ReClipModel::TFlags ReClipModel::flags( const QModelIndex& _index ) const
{
	TFlags result = TSuper::flags( _index );
	if( EColumn_Icon != _index.column() )
	{
		result |= Qt::ItemIsEditable;
	}

	return result;
}


// -------------------------------------------------------------------------
// Override ReModelBase.
// -------------------------------------------------------------------------
ReClipData* ReClipModel::CreateData()
{
	// Reset.
	ReClipData* result = TSuperB::CreateData();
	result->GetOffsetRef().setX( 0 );
	result->GetOffsetRef().setY( 0 );
	result->GetSizeRef().setWidth( 0 );
	result->GetSizeRef().setHeight( 0 );

	// Create a row and associate the clip data with it.
	insertRow( rowCount(), result );

	// Flush the data to the model.
	result->Flush();

	return result;
}


void ReClipModel::RecycleData( ReClipData* _clip )
{
	if( NULL != _clip )
	{
		// Cannot call removeRow here because we are going to recycle 
		// the item and surely don't want Qt to release the memory under
		// the curtain.
		QModelIndex index = indexFromItem( _clip );
		if( index.isValid() )
			takeRow( index.row() );
	}

	TSuperB::RecycleData( _clip );
}


// -------------------------------------------------------------------------
// Model specific.
// -------------------------------------------------------------------------
void ReClipModel::SetDataFromItem( const QModelIndex& _index )
{
	ReClipData* clipData = dynamic_cast< ReClipData* >( TSuper::item( _index.row() ) );
	
	// Call QStandardItemModel's setData instead.
	//TSuper::setData( index( _index.row(), EColumn_Name ), clipData->GetName() );
	TSuper::setData( index( _index.row(), EColumn_Icon ), clipData->GetName() );
	TSuper::setData( index( _index.row(), EColumn_X ), clipData->GetOffset().x() );
	TSuper::setData( index( _index.row(), EColumn_Y ), clipData->GetOffset().y() );
	TSuper::setData( index( _index.row(), EColumn_W ), clipData->GetSize().width() );
	TSuper::setData( index( _index.row(), EColumn_H ), clipData->GetSize().height() );

	emit dataChanged( _index, _index );
}


void ReClipModel::SetImageHandle( TImageHandle _handle )
{
	QString path = _handle;
	m_pixmap.load( path );
}


}
