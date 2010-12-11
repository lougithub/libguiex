#include "StdAfxEditor.h"
#include "Core\ReAnimTrack.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimTrack::ReAnimTrack()
: m_isSortDirty( false )
{
}


ReAnimTrack::~ReAnimTrack()
{
	TFrameListCItor itor = m_frames.begin();
	TFrameListCItor itorEnd = m_frames.end();
	for( ; itor != itorEnd; ++itor )
	{
		delete *itor;
	}

	m_frames.clear();
}


ReAnimFrame* ReAnimTrack::CreateFrame( qreal _time )
{
	ReAnimFrame* result = new ReAnimFrame( _time );
	result->GetTypeRef() = GetType();
	result->GetNameRef() = QString( QObject::tr( "Frame" ) );
	result->GetParentRef() = this;
	m_frames.push_back( result );
	m_isSortDirty = true;

	return result;
}


void ReAnimTrack::DeleteFrame( ReAnimFrame* _frame )
{
	if( NULL != _frame )
	{
		TFrameListItor itor = m_frames.begin();
		TFrameListItor itorEnd = m_frames.end();
		for( ; itor != itorEnd; ++itor )
		{
			if( _frame == *itor )
			{
				m_frames.erase( itor );
				break;
			}
		}
	}
}


ReAnimFrame* ReAnimTrack::GetFrameByIndex( int _index )
{
	ReAnimFrame* result = NULL;

	if( IsDirty() )
		Sort();

	if( _index >= 0 && _index < m_frames.size() )
		result = m_frames.value( _index );

	return result;
}


bool ReAnimTrack::HasFrame( const ReAnimFrame& _frame ) const
{
	bool result = false;

	TFrameListCItor itor = m_frames.begin();
	TFrameListCItor itorEnd = m_frames.end();
	for( ; itor != itorEnd && !result; ++itor )
		if( &_frame == *itor )
			result = true;

	return result;
}


// -----------------------------------------------------------------------------
// Override ReAnimItem.
// -----------------------------------------------------------------------------
int ReAnimTrack::IndexOfChild( const ReAnimItem* _child )
{
	int result = -1;

	if( NULL != _child )
	{
		if( IsDirty() )
			Sort();

		TFrameListCItor itor = m_frames.begin();
		TFrameListCItor itorEnd = m_frames.end();
		for( int i = 0; itor != itorEnd; ++itor, ++i )
		{
			if( *itor == _child )
			{
				result = i;
				break;
			}
		}
	}

	return result;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
bool ReAnimTrack::IsDirty() const
{
	return m_isSortDirty;
}


void ReAnimTrack::Sort()
{
	qSort( m_frames );
	m_isSortDirty = false;
}




}
