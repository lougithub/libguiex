#include "StdAfxEditor.h"
#include "Core\ReAnimModel.h"
#include "Core\ReAnimNode.h"
#include "Core\ReAnimTrack.h"
#include "Core\ReAnimEntity.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimModel::ReAnimModel( QObject* _parent /* = NULL */ )
: TSuper( _parent )
{
	//for( int i = 0; i < ETrackType_Count; ++i )
	//	m_tracks[ i ] = NULL;
}


qreal ReAnimModel::GetTotalLength() const
{
	qreal result = 0;

	TItemListCItor itor = m_items.begin();
	TItemListCItor itorEnd = m_items.end();
	for( ; itor != itorEnd; ++itor )
	{
		const ReAnimEntity* item = *itor;
		result = qMax( result, item->GetTotalLength() );
	}

	return result;
}


// -----------------------------------------------------------------------------
// Override QAbstractItemModel.
// -----------------------------------------------------------------------------
QModelIndex ReAnimModel::index( int _row, int _column, const QModelIndex& _parent /* = QModelIndex */ ) const
{
	QModelIndex result = QModelIndex();

	//if( _row >= 0 && _column >= 0 )
	//{
	//	if( _parent.isValid() )
	//	{
	//		ReAnimNode* item = ItemFromIndex( _parent );
	//		ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( item );
	//		if( NULL != track )
	//		{
	//			// Track level:
	//			// - parent: a track;
	//			// - rows: frames.
	//			if( _row < track->GetFrameCount() )
	//			{
	//				result = createIndex( _row, _column, track->GetFrameByIndex( _row ) );
	//			}
	//		}
	//		else
	//		{
	//			// Frame level:
	//			// - parent: a frame;
	//			// - rows: ( virtual ).
	//			ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
	//			if( _row < gDataValueCount( frame->GetType() ) )
	//			{
	//				result = createIndex( _row, _column, &frame->GetValueRef( _row ) );
	//			}
	//		}
	//	}
	//	else
	//	{
	//		// Top level: 
	//		// - parent: the invisible root item;
	//		// - rows: the tracks.
	//		result = createIndex( _row, _column, GetTrackByIndex( _row ) );
	//	}
	//}

	return result;
}


QModelIndex ReAnimModel::parent( const QModelIndex& _child ) const
{
	QModelIndex result = QModelIndex();

	//if( _child.isValid() )
	//{
	//	ReAnimNode* item = ItemFromIndex( _child );
	//	ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
	//	if( NULL != frame )
	//	{
	//		// It's a frame. The parent is a track.
	//		ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( frame->GetParent() );
	//		result = createIndex( GetTrackIndex( track->GetType() ), 0, track );
	//	}
	//	else
	//	{
	//		ReAnimValue* value = dynamic_cast< ReAnimValue* >( item );
	//		if( NULL != value )
	//		{
	//			// It's a value. The parent is a frame.
	//			ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( value->GetParent() );
	//			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( frame->GetParent() );
	//			result = createIndex( track->IndexOfChild( frame ), 0, frame );
	//		}
	//	}

	//	// If it's a track, the parent is the invisible root item.
	//}

	return result;
}


QVariant ReAnimModel::data( const QModelIndex &_index, int _role ) const
{
	QVariant result = QVariant();

	//if( _index.isValid() )
	//{
	//	if( Qt::DisplayRole == _role )
	//	{
	//		ReAnimNode* item = static_cast< ReAnimNode* >( _index.internalPointer() );
	//		if( NULL != item )
	//		{
	//			if( EColumn_Key == _index.column() )
	//			{
	//				result = item->ToKeyVariant();
	//			}
	//			else if( EColumn_Value == _index.column() )
	//			{
	//				result = item->ToValueVariant();
	//			}
	//		}
	//	}
	//}

	return result;
}


bool ReAnimModel::setData( const QModelIndex& _index, const QVariant& _value, int _role )
{
	bool result = false;

	//if( _index.isValid() && Qt::EditRole == _role )
	//{
	//	if( EColumn_Value == _index.column() )
	//	{
	//		ReAnimValue* item = dynamic_cast< ReAnimValue* >( ItemFromIndex( _index ) );
	//		if( NULL != item )
	//		{
	//			item->GetDataRef() = _value.toDouble();

	//			emit dataChanged( _index, _index );
	//			result = true;
	//		}
	//	}
	//}

	return result;
}


int ReAnimModel::rowCount( const QModelIndex& _parent ) const
{
	int result = 0;

	//if( _parent.isValid() )
	//{
	//	ReAnimNode* item = ItemFromIndex( _parent );
	//	ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( item );
	//	if( NULL != track )
	//	{
	//		result = track->GetFrameCount();
	//	}
	//	else
	//	{
	//		ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
	//		if( NULL != frame )
	//		{
	//			result = gDataValueCount( frame->GetType() );
	//		}
	//	}
	//}
	//else
	//{
	//	// The invisible root item has all the tracks as its children.
	//	result = GetTrackCount();
	//}

	return result;
}


int ReAnimModel::columnCount( const QModelIndex& _parent ) const
{
	return EColumn_Count;
}


QVariant ReAnimModel::headerData( int _section, Qt::Orientation _orient, int _role /* = Qt::DisplayRole */ )
{
	QVariant result = QVariant();

	if( Qt::Horizontal == _orient )
	{
		result = tr( "Dota" );
		//if( EColumn_TrackOrFrame == _section )
		//	result = tr( "Type" );
		//else if( EColumn_X == _section )
		//	result = tr( "X" );
		//else if( EColumn_Y == _section )
		//	result = tr( "Y" );
		//else if( EColumn_Z == _section )
		//	result = tr( "Z" );
		//else if( EColumn_W == _section )
		//	result = tr( "W" );
	}

	return result;
}


Qt::ItemFlags ReAnimModel::flags( const QModelIndex& _index ) const
{
	Qt::ItemFlags result = TSuper::flags( _index );

	if( _index.isValid() && EColumn_Value == _index.column() )
	{
		ReAnimNode* item = ItemFromIndex( _index );
		if( NULL != item )
		{
			ReAnimValue* valueItem = dynamic_cast< ReAnimValue* >( item );
			if( NULL != valueItem )
			{
				result |= Qt::ItemIsEditable;
			}
		}
	}

	return result;
}


//bool ReAnimModel::insertRows( int _row, int _count, const QModelIndex& _parent /* = QModelIndex */ )
//{
//	beginInsertRows( _parent, _row, _row + _count - 1 );
//	endInsertRows();
//
//	return true;
//}
//
//
//bool ReAnimModel::removeRows( int _row, int _count, const QModelIndex& _parent /* = QModelIndex */ )
//{
//	beginRemoveRows( _parent, _row, _row + _count - 1 );
//	endRemoveRows();
//
//	return true;
//}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimModel::OnFrameChanged( eTrackType _trackType, ReAnimFrame* _frame )
{
	//ReAnimTrack* track = GetTrackByType( _trackType );
	//if( NULL != track )
	//{
	//	int frameIndex = track->IndexOfChild( _frame );
	//	if( frameIndex >= 0 )
	//	{
	//		QModelIndex trackIndex = index( GetTrackIndex( _trackType ), 0, QModelIndex() );
	//		QModelIndex frameIndexA = index( frameIndex, EColumn_Key, trackIndex );
	//		QModelIndex frameIndexB = index( frameIndex, EColumn_Value, trackIndex );
	//		emit dataChanged( frameIndexA, frameIndexB );
	//	}
	//}
}


// -----------------------------------------------------------------------------
// Tracks and Frames.
// -----------------------------------------------------------------------------
ReAnimEntity* ReAnimModel::CreateEntity()
{
	ReAnimEntity* result = new ReAnimEntity();

	m_items.push_back( result );

	return result;
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
ReAnimNode* ReAnimModel::ItemFromIndex( const QModelIndex& _index ) const
{
	ReAnimNode* result = NULL;

	if( _index.isValid() )
	{
		result = static_cast< ReAnimNode* >( _index.internalPointer() );
	}

	return result;
}


//int ReAnimModel::PositionToInsertTrack( eTrackType _trackType ) const
//{
//	int result = -1;
//
//	if( NULL == m_tracks[ _trackType ] )
//	{
//		result = 0;
//		for( int i = 0; i < _trackType; ++i )
//		{
//			if( NULL != m_tracks[ i ] )
//			{
//				++result;
//			}
//		}
//	}
//
//	return result;
//}



}
