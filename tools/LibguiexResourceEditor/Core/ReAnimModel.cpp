#include "StdAfxEditor.h"
#include "Core\ReAnimModel.h"
#include "Core\ReAnimItem.h"
#include "Core\ReAnimTrack.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimModel::ReAnimModel( QObject* _parent /* = NULL */ )
: TSuper( _parent )
{
	for( int i = 0; i < ETrackType_Count; ++i )
		m_tracks[ i ] = NULL;
}


// -----------------------------------------------------------------------------
// Override QAbstractItemModel.
// -----------------------------------------------------------------------------
QModelIndex ReAnimModel::index( int _row, int _column, const QModelIndex& _parent /* = QModelIndex */ ) const
{
	QModelIndex result = QModelIndex();

	if( _row >= 0 && _column >= 0 )
	{
		if( _parent.isValid() )
		{
			ReAnimItem* item = ItemFromIndex( _parent );
			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( item );
			if( NULL != track )
			{
				// Track level:
				// - parent: a track;
				// - rows: frames.
				if( _row < track->GetFrameCount() )
				{
					result = createIndex( _row, _column, track->GetFrameByIndex( _row ) );
				}
			}
			else
			{
				// Frame level:
				// - parent: a frame;
				// - rows: ( virtual ).
				ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
				if( _row < gDataValueCount( frame->GetType() ) )
				{
					result = createIndex( _row, _column, &frame->GetValueRef( _row ) );
				}
			}
		}
		else
		{
			// Top level: 
			// - parent: the invisible root item;
			// - rows: the tracks.
			result = createIndex( _row, _column, GetTrackByIndex( _row ) );
		}
	}

	return result;
}


QModelIndex ReAnimModel::parent( const QModelIndex& _child ) const
{
	QModelIndex result = QModelIndex();

	if( _child.isValid() )
	{
		ReAnimItem* item = ItemFromIndex( _child );
		ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
		if( NULL != frame )
		{
			// It's a frame. The parent is a track.
			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( frame->GetParent() );
			result = createIndex( GetTrackIndex( track->GetType() ), 0, track );
		}
		else
		{
			ReAnimValueItem* value = dynamic_cast< ReAnimValueItem* >( item );
			if( NULL != value )
			{
				// It's a value. The parent is a frame.
				ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( value->GetParent() );
				ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( frame->GetParent() );
				result = createIndex( track->IndexOfChild( frame ), 0, frame );
			}
		}

		// If it's a track, the parent is the invisible root item.
	}

	return result;
}


QVariant ReAnimModel::data( const QModelIndex &_index, int _role ) const
{
	QVariant result = QVariant();

	if( _index.isValid() )
	{
		if( Qt::DisplayRole == _role )
		{
			ReAnimItem* item = static_cast< ReAnimItem* >( _index.internalPointer() );
			if( NULL != item )
			{
				if( EColumn_Key == _index.column() )
				{
					result = item->ToKeyVariant();
				}
				else if( EColumn_Value == _index.column() )
				{
					result = item->ToValueVariant();
				}
			}
		}
	}

	return result;
}


bool ReAnimModel::setData( const QModelIndex& _index, const QVariant& _value, int _role )
{
	bool result = false;

	if( _index.isValid() && Qt::EditRole == _role )
	{
		if( EColumn_Value == _index.column() )
		{
			ReAnimValueItem* item = dynamic_cast< ReAnimValueItem* >( ItemFromIndex( _index ) );
			if( NULL != item )
			{
				item->GetDataRef() = _value.toDouble();

				emit dataChanged( _index, _index );
				result = true;
			}
		}
	}

	return result;
}


int ReAnimModel::rowCount( const QModelIndex& _parent ) const
{
	int result = 0;

	if( _parent.isValid() )
	{
		ReAnimItem* item = ItemFromIndex( _parent );
		ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( item );
		if( NULL != track )
		{
			result = track->GetFrameCount();
		}
		else
		{
			ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( item );
			if( NULL != frame )
			{
				result = gDataValueCount( frame->GetType() );
			}
		}
	}
	else
	{
		// The invisible root item has all the tracks as its children.
		result = GetTrackCount();
	}

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
		ReAnimItem* item = ItemFromIndex( _index );
		if( NULL != item )
		{
			ReAnimValueItem* valueItem = dynamic_cast< ReAnimValueItem* >( item );
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
	ReAnimTrack* track = GetTrackByType( _trackType );
	if( NULL != track )
	{
		int frameIndex = track->IndexOfChild( _frame );
		if( frameIndex >= 0 )
		{
			QModelIndex trackIndex = index( GetTrackIndex( _trackType ), 0, QModelIndex() );
			QModelIndex frameIndexA = index( frameIndex, EColumn_Key, trackIndex );
			QModelIndex frameIndexB = index( frameIndex, EColumn_Value, trackIndex );
			emit dataChanged( frameIndexA, frameIndexB );
		}
	}
}


// -----------------------------------------------------------------------------
// Tracks and Frames.
// -----------------------------------------------------------------------------
ReAnimTrack* ReAnimModel::CreateTrack( eTrackType _type )
{
	if( NULL == m_tracks[ _type ] )
	{		
		//int row = PositionToInsertTrack( _type );
		//insertRow( row );

		m_tracks[ _type ] = new ReAnimTrack();
		m_tracks[ _type ]->GetNameRef() = gTrackTypeToString( _type );
		m_tracks[ _type ]->GetTypeRef() = _type;
		//setData( QModelIndex( row, EColumn_Key, m_tracks[ _type ] ) );

		reset();
	}

	return m_tracks[ _type ];
}


void ReAnimModel::DeleteTrack( eTrackType _type )
{
	//if( NULL != m_tracks[ _type ] )
	//{
	//	delete m_tracks[ _type ];
	//	m_tracks[ _type ] = NULL;
	//}
}


ReAnimTrack* ReAnimModel::GetTrackByType( eTrackType _type ) const
{
	return m_tracks[ _type ];
}


ReAnimTrack* ReAnimModel::GetTrackByIndex( int _index ) const
{
	ReAnimTrack* result = NULL;

	for( int i = 0, index = 0; i < ETrackType_Count; ++i )
	{
		if( NULL != m_tracks[ i ] )
		{
			if( index == _index )
			{
				result = m_tracks[ i ];
				break;
			}

			++index;
		}
	}

	return result;
}


int ReAnimModel::GetTrackCount() const
{
	int result = 0;

	for( int i = 0; i < ETrackType_Count; ++i )
		if( NULL != m_tracks[ i ] )
			++result;

	return result;
}


int ReAnimModel::GetTrackIndex( eTrackType _type ) const
{
	int result = -1;

	if( NULL != m_tracks[ _type ] )
	{
		result = 0;

		for( int i = 0; i < _type; ++i )
		{
			if( NULL != m_tracks[ i ] )
			{
				++result;
			}
		}
	}

	return result;
}


ReAnimFrame* ReAnimModel::CreateFrame( eTrackType _trackType, qreal _time )
{
	ReAnimFrame* result = NULL;

	ReAnimTrack* track = GetTrackByType( _trackType );
	if( NULL != track )
	{
		result = track->CreateFrame( _time );
		reset();
		//QModelIndex trackIndex = index( GetTrackIndex( _trackType ), 0, QModelIndex() );
		//QModelIndex frameIndexA = index( track->IndexOfChild( result ), EColumn_Key, trackIndex );
		//QModelIndex frameIndexB = index( track->IndexOfChild( result ), EColumn_Value, trackIndex );
		//emit dataChanged( frameIndexA, frameIndexB );
	}

	return result;
}


void ReAnimModel::DeleteFrame( eTrackType _trackType, ReAnimFrame* _frame )
{
	ReAnimTrack* track = GetTrackByType( _trackType );
	if( NULL != track )
	{
		track->DeleteFrame( _frame );
		reset();
	}
}


ReAnimFrame* ReAnimModel::GetFrameByIndex( eTrackType _trackType, int _index ) const
{
	return ( NULL != m_tracks[ _trackType ] ) ? ( m_tracks[ _trackType ]->GetFrameByIndex( _index ) ) : NULL;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
ReAnimItem* ReAnimModel::ItemFromIndex( const QModelIndex& _index ) const
{
	ReAnimItem* result = NULL;

	if( _index.isValid() )
	{
		result = static_cast< ReAnimItem* >( _index.internalPointer() );
	}

	return result;
}


int ReAnimModel::PositionToInsertTrack( eTrackType _trackType ) const
{
	int result = -1;

	if( NULL == m_tracks[ _trackType ] )
	{
		result = 0;
		for( int i = 0; i < _trackType; ++i )
		{
			if( NULL != m_tracks[ i ] )
			{
				++result;
			}
		}
	}

	return result;
}



}
