#include "StdAfxEditor.h"
#include "Core\ReAnimEntity.h"
#include "Core\ReAnimTrack.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimEntity::ReAnimEntity( ReAnimModel* _model )
: TSuper( _model )
{
	for( int i = 0; i < ETrackType_Count; ++i )
		m_tracks[ i ] = NULL;
}


ReAnimEntity::~ReAnimEntity()
{
	for( int i = 0; i < ETrackType_Count; ++i )
		if( NULL != m_tracks[ i ] )
			delete m_tracks[ i ];
}


ReAnimTrack* ReAnimEntity::CreateTrack( eTrackType _type )
{
	ReAnimTrack* result = NULL;
	if( NULL == m_tracks[ _type ] )
	{
		switch( _type )
		{
		case ETrackType_Translation:
			result = new ReAnimTranslationTrack( GetModel() );
			result->SetName( QObject::tr( "Translation" ) );
			break;

		case ETrackType_Rotation:
			result = new ReAnimRotationTrack( GetModel() );
			result->SetName( QObject::tr( "Rotation" ) );
			break;

		case ETrackType_Scale:
			result = new ReAnimScaleTrack( GetModel() );
			result->SetName( QObject::tr( "Scale" ) );
			break;

		case ETrackType_Alpha:
			result = new ReAnimAlphaTrack( GetModel() );
			result->SetName( QObject::tr( "Alpha" ) );
			break;
		}

		if( NULL != result )
		{
			result->SetParent( this );
			m_tracks[ _type ] = result;
		}		
	}

	return result;
}


qreal ReAnimEntity::GetTotalLength() const
{
	qreal result = 0;

	for( int i = 0; i < ETrackType_Count; ++i )
	{
		if( NULL != m_tracks[ i ] )
		{
			result = qMax( result, m_tracks[ i ]->GetTotalLength() );
		}
	}

	return result;
}


// -----------------------------------------------------------------------------
// Override ReAnimNode.
// -----------------------------------------------------------------------------
ReAnimNode* ReAnimEntity::CreateChild( const QVariant& _arg )
{
	ReAnimTrack* result = NULL;
	int type = _arg.toUInt();
	if( NULL == m_tracks[ type ] )
	{
		switch( type )
		{
		case ETrackType_Translation:
			result = new ReAnimTranslationTrack( GetModel() );
			result->SetName( QObject::tr( "Translation" ) );
			break;

		case ETrackType_Rotation:
			result = new ReAnimRotationTrack( GetModel() );
			result->SetName( QObject::tr( "Rotation" ) );
			break;

		case ETrackType_Scale:
			result = new ReAnimScaleTrack( GetModel() );
			result->SetName( QObject::tr( "Scale" ) );
			break;

		case ETrackType_Alpha:
			result = new ReAnimAlphaTrack( GetModel() );
			result->SetName( QObject::tr( "Alpha" ) );
			break;
		}

		if( NULL != result )
		{
			result->SetParent( this );
			m_tracks[ type ] = result;
		}		
	}

	return result;
}


void ReAnimEntity::DestroyChild( ReAnimNode* _child )
{
	if( NULL != _child )
	{
		ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( _child );
		if( NULL != track )
		{
			for( int i = 0; i < ETrackType_Count; ++i )
			{
				if( m_tracks[ i ] == _child )
				{
					m_tracks[ i ] = NULL;
					delete _child;
				}
			}
		}
	}
}


ReAnimNode* ReAnimEntity::GetChild( int _index )
{
	return _index >= 0 && _index < GetChildrenCount() ? m_tracks[ _index ] : NULL;
}


int ReAnimEntity::IndexOfChild( const ReAnimNode* _child )
{
	int index = -1;

	const ReAnimTrack* track = dynamic_cast< const ReAnimTrack* >( _child );
	if( NULL != track )
	{
		for( int i = 0; i < ETrackType_Count; ++i )
		{
			if( m_tracks[ i ] == _child )
			{
				index = i;
				break;
			}
		}
	}

	return index;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------




}
