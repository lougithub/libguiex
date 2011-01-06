#include "StdAfxEditor.h"
#include "Core\ReAnimEntity.h"
#include "Core\ReAnimTrack.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimEntity::ReAnimEntity()
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
			result = new ReAnimTranslationTrack();
			result->SetName( QObject::tr( "Translation" ) );
			break;

		case ETrackType_Rotation:
			result = new ReAnimRotationTrack();
			result->SetName( QObject::tr( "Rotation" ) );
			break;

		case ETrackType_Scale:
			result = new ReAnimScaleTrack();
			result->SetName( QObject::tr( "Scale" ) );
			break;

		case ETrackType_Alpha:
			result = new ReAnimAlphaTrack();
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


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------




}
