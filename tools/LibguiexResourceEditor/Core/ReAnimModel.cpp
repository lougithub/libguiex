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
}


qreal ReAnimModel::GetTotalLength() const
{
	qreal result = 0;

	TEntityListCItor itor = m_entities.begin();
	TEntityListCItor itorEnd = m_entities.end();
	for( ; itor != itorEnd; ++itor )
	{
		const ReAnimEntity* item = *itor;
		result = qMax( result, item->GetTotalLength() );
	}

	return result;
}


bool ReAnimModel::Export( const QString& _filePath ) const
{
	bool result = false;

	

	return result;
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
			ReAnimNode* node = NodeFromIndex( _parent );
			ReAnimEntity* entity = dynamic_cast< ReAnimEntity* >( node );
			if( NULL != entity )
			{
				// Entity level:
				// - parent: an entity;
				// - rows: tracks.
				if( _row < entity->GetChildrenCount() )
				{
					result = createIndex( _row, _column, entity->GetChild( _row ) );
				}
			}
			else
			{
				ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( node );
				if( NULL != track )
				{
					// Track level:
					// - parent: a track;
					// - rows: frames.
					if( _row < track->GetChildrenCount() )
					{
						result = createIndex( _row, _column, track->GetChild( _row ) );
					}
				}
			}
		}
		else
		{
			// Top level: 
			// - parent: the invisible root item;
			// - rows: the entities.
			result = createIndex( _row, _column, GetEntity( _row ) );
		}
	}

	return result;
}


QModelIndex ReAnimModel::parent( const QModelIndex& _child ) const
{
	QModelIndex result = QModelIndex();

	if( _child.isValid() )
	{
		ReAnimNode* node = NodeFromIndex( _child );
		ReAnimFrame* frame = dynamic_cast< ReAnimFrame* >( node );
		if( NULL != frame )
		{
			// It's a frame. The parent is a track.
			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( frame->GetParent() );
			ReAnimEntity* entity = ( ReAnimEntity* )track->GetParent();
			result = createIndex( entity->IndexOfChild( track ), 0, track );
		}
		else
		{
			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( node );
			if( NULL != track )
			{
				// It's a track. The parent is an entity.
				ReAnimEntity* entity = dynamic_cast< ReAnimEntity* >( track->GetParent() );
				result = createIndex( GetEntityIndex( entity ), 0, entity );
			}
		}
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
			ReAnimNode* node = static_cast< ReAnimNode* >( _index.internalPointer() );
			if( NULL != node )
			{
				if( EColumn_Key == _index.column() )
				{
					result = node->GetNameVariant();
				}
				else if( EColumn_Value == _index.column() )
				{
					result = node->GetDataVariant();
				}
			}
		}
	}

	return result;
}


bool ReAnimModel::setData( const QModelIndex& _index, const QVariant& _value, int _role )
{
	bool result = false;

	//if( _index.isValid() && Qt::EditRole == _role )
	//{
	//	if( EColumn_Value == _index.column() )
	//	{
	//		ReAnimValue* item = dynamic_cast< ReAnimValue* >( NodeFromIndex( _index ) );
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

	if( _parent.isValid() )
	{
		ReAnimNode* node = NodeFromIndex( _parent );
		result = node->GetChildrenCount();
	}
	else
	{
		// The invisible root item has all the entities as its children.
		result = GetEntityCount();
	}

	return result;
}


int ReAnimModel::columnCount( const QModelIndex& _parent ) const
{
	return EColumn_Count;
}


//QVariant ReAnimModel::headerData( int _section, Qt::Orientation _orient, int _role /* = Qt::DisplayRole */ )
//{
//	QVariant result = QVariant();
//
//	if( Qt::Horizontal == _orient )
//	{
//		result = tr( "Dota" );
//		//if( EColumn_TrackOrFrame == _section )
//		//	result = tr( "Type" );
//		//else if( EColumn_X == _section )
//		//	result = tr( "X" );
//		//else if( EColumn_Y == _section )
//		//	result = tr( "Y" );
//		//else if( EColumn_Z == _section )
//		//	result = tr( "Z" );
//		//else if( EColumn_W == _section )
//		//	result = tr( "W" );
//	}
//
//	return result;
//}


Qt::ItemFlags ReAnimModel::flags( const QModelIndex& _index ) const
{
	Qt::ItemFlags result = TSuper::flags( _index );

	//if( _index.isValid() && EColumn_Value == _index.column() )
	//{
	//	ReAnimNode* item = NodeFromIndex( _index );
	//	if( NULL != item )
	//	{
	//		ReAnimValue* valueItem = dynamic_cast< ReAnimValue* >( item );
	//		if( NULL != valueItem )
	//		{
	//			result |= Qt::ItemIsEditable;
	//		}
	//	}
	//}

	return result;
}


bool ReAnimModel::insertRows( int _row, int _count, const QModelIndex& _parent /* = QModelIndex */ )
{
	bool result = true;

	beginInsertRows( _parent, _row, _row + _count - 1 );

	if( _parent.isValid() )
	{
		ReAnimNode* node = NodeFromIndex( _parent );
		ReAnimEntity* entity = dynamic_cast< ReAnimEntity* >( node );
		if( NULL != entity )
		{
			entity->CreateChild( _row );
		}
		else
		{
			ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( node );
			track->CreateChild( QVariant() );
		}
	}
	else
	{
		ReAnimEntity* result = new ReAnimEntity( this );
		m_entities.push_back( result );
	}

	endInsertRows();

	return result;
}


bool ReAnimModel::removeRows( int _row, int _count, const QModelIndex& _parent /* = QModelIndex */ )
{
	bool result = false;

	beginRemoveRows( _parent, _row, _row + _count - 1 );

	if( _parent.isValid() )
	{
		ReAnimNode* node = NodeFromIndex( _parent );
		ReAnimNode* child = node->GetChild( _row );
		node->DestroyChild( child );
	}
	else
	{
		ReAnimEntity* entity = GetEntity( _row );
		TEntityListItor itor = m_entities.begin();
		TEntityListItor itorEnd = m_entities.end();
		for( ; itor != itorEnd; ++itor )
		{
			if( *itor == entity )
			{
				m_entities.erase( itor );
				delete entity;
			}
		}
	}

	endRemoveRows();

	return result;
}


// -----------------------------------------------------------------------------
// Tracks and Frames.
// -----------------------------------------------------------------------------
ReAnimEntity* ReAnimModel::CreateEntity()
{
	int entityCount = GetEntityCount();
	insertRow( entityCount, QModelIndex() );

	return GetEntity( entityCount );
}


ReAnimTrack* ReAnimModel::CreateTrack( ReAnimEntity* _entity, eTrackType _type )
{
	ReAnimTrack* result = NULL;

	if( NULL != _entity )
	{		
		QModelIndex parentIndex = index( GetEntityIndex( _entity ), 0, QModelIndex() );
		insertRow( _type, parentIndex );

		result = ( ReAnimTrack* )_entity->GetChild( _type );
	}

	return result;
}


ReAnimFrame* ReAnimModel::CreateFrame( ReAnimTrack* _track )
{
	ReAnimFrame* result = NULL;

	if( NULL != _track )
	{
		ReAnimEntity* entity = ( ReAnimEntity* )_track->GetParent();
		QModelIndex entityIndex = index( GetEntityIndex( entity ), 0, QModelIndex() );
		QModelIndex trackIndex = index( entity->IndexOfChild( _track ), 0, entityIndex );
		int frameCount = _track->GetChildrenCount();
		insertRow( frameCount, trackIndex );

		result = ( ReAnimFrame* )_track->GetChild( frameCount );
	}

	return result;
}


void ReAnimModel::DestroyEntity( ReAnimEntity* _entity )
{
	if( NULL != _entity )
	{
		removeRow( GetEntityIndex( _entity ), QModelIndex() );
	}
}


void ReAnimModel::DestroyTrack( ReAnimTrack* _track )
{
	if( NULL != _track )
	{
		ReAnimEntity* entity = ( ReAnimEntity* )_track->GetParent();
		QModelIndex parentIndex = index( GetEntityIndex( entity ), 0, QModelIndex() );
		removeRow( entity->IndexOfChild( _track ), parentIndex );
	}
}


void ReAnimModel::DestroyFrame( ReAnimFrame* _frame )
{
	if( NULL != _frame )
	{
		ReAnimTrack* track = ( ReAnimTrack* )_frame->GetParent();
		ReAnimEntity* entity = ( ReAnimEntity* )track->GetParent();
		QModelIndex entityIndex = index( GetEntityIndex( entity ), 0, QModelIndex() );
		QModelIndex trackIndex = index( entity->IndexOfChild( track ), 0, entityIndex );
		removeRow( track->IndexOfChild( _frame ), trackIndex );
	}
}


void ReAnimModel::SetEntityName( ReAnimEntity* _entity, const QString& _name )
{
	// Not working, why? Come back later...
	if( NULL != _entity )
	{
		_entity->SetName( _name );
		QModelIndex entityIndex = index( GetEntityIndex( _entity ), EColumn_Value, QModelIndex() );		
		emit dataChanged( entityIndex, entityIndex );
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
ReAnimNode* ReAnimModel::NodeFromIndex( const QModelIndex& _index ) const
{
	ReAnimNode* result = NULL;

	if( _index.isValid() )
	{
		result = static_cast< ReAnimNode* >( _index.internalPointer() );
	}

	return result;
}


ReAnimEntity* ReAnimModel::GetEntity( int _index ) const
{
	ReAnimEntity* result = NULL;

	if( _index >= 0 && _index < GetEntityCount() )
	{
		TEntityListCItor itor = m_entities.begin() + _index;
		result = *itor;
	}

	return result;
}


int ReAnimModel::GetEntityIndex( ReAnimEntity* _entity ) const
{
	int index = -1;

	if( NULL != _entity )
	{
		TEntityListCItor itor = m_entities.begin();
		TEntityListCItor itorEnd = m_entities.end();
		for( int i = 0; itor != itorEnd; ++itor, ++i )
		{
			if( *itor == _entity )
			{
				index = i;
				break;
			}
		}
	}

	return index;
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
